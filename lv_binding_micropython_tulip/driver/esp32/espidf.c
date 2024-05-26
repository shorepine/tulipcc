#include "../include/common.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/gc.h"
#include "py/stackctrl.h"
#include "mphalport.h"
#include "espidf.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "soc/cpu.h"


// ESP IDF has some functions that are declared but not implemented.
// To avoid linking errors, provide empty implementation

inline void gpio_pin_wakeup_disable(void){}
inline void gpio_pin_wakeup_enable(uint32_t i, GPIO_INT_TYPE intr_state){}
inline void gpio_intr_ack_high(uint32_t ack_mask){}
inline void gpio_intr_ack(uint32_t ack_mask){}
inline uint32_t gpio_intr_pending_high(void){return 0;}
inline uint32_t gpio_intr_pending(void){return 0;}
inline void gpio_intr_handler_register(gpio_intr_handler_fn_t fn, void *arg){}
inline void gpio_init(void){}

void task_delay_ms(int ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

// ISR callbacks handling
// Can't use mp_sched_schedule because lvgl won't yield to give micropython a chance to run
// Must run Micropython in ISR itself.

DEFINE_PTR_OBJ_TYPE(spi_transaction_ptr_type, MP_QSTR_spi_transaction_ptr_t);

typedef struct{
    mp_ptr_t spi_transaction_ptr;
    mp_obj_t pre_cb;
    mp_obj_t post_cb;
} mp_spi_device_callbacks_t;

void *spi_transaction_set_cb(mp_obj_t pre_cb, mp_obj_t post_cb)
{
    mp_spi_device_callbacks_t *callbacks = m_new_obj(mp_spi_device_callbacks_t);
    callbacks->spi_transaction_ptr.base.type = &spi_transaction_ptr_type;
    callbacks->pre_cb = pre_cb != mp_const_none? pre_cb: NULL;
    callbacks->post_cb = post_cb != mp_const_none? post_cb: NULL;
    return callbacks;
}

STATIC void isr_print_strn(void *env, const char *str, size_t len) {
    size_t i;
    (void)env;
    for (i=0; i<len; i++) ets_write_char_uart(str[i]);
}

static const mp_print_t mp_isr_print = {NULL, isr_print_strn};

// Called in ISR context!
//
static inline void cb_isr(mp_obj_t cb, mp_obj_t arg)
{
    if (cb != NULL && cb != mp_const_none) {

        volatile uint32_t sp = (uint32_t)get_sp();

        // Calling micropython from ISR
        // See: https://github.com/micropython/micropython/issues/4895

        void *old_state = mp_thread_get_state();

        mp_state_thread_t ts; // local thread state for the ISR
        mp_thread_set_state(&ts);
        mp_stack_set_top((void*)sp); // need to include in root-pointer scan
        mp_stack_set_limit(configIDLE_TASK_STACK_SIZE - 1024); // tune based on ISR thread stack size
        mp_locals_set(mp_state_ctx.thread.dict_locals); // use main thread's locals
        mp_globals_set(mp_state_ctx.thread.dict_globals); // use main thread's globals

        mp_sched_lock(); // prevent VM from switching to another MicroPython thread
        gc_lock(); // prevent memory allocation

        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {
            mp_call_function_1(cb, arg);
            nlr_pop();
        } else {
            ets_printf("Uncaught exception in IRQ callback handler!\n");
            mp_obj_print_exception(&mp_isr_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        }

        gc_unlock();
        mp_sched_unlock();

        mp_thread_set_state(old_state);

        mp_hal_wake_main_task_from_isr();
    }
}

// Called in ISR context!
void ex_spi_pre_cb_isr(spi_transaction_t *trans)
{
    mp_spi_device_callbacks_t *self = (mp_spi_device_callbacks_t*)trans->user;
    if (self) {
        self->spi_transaction_ptr.ptr = trans;
        cb_isr(self->pre_cb, MP_OBJ_FROM_PTR(self));
    }
}

// Called in ISR context!
void ex_spi_post_cb_isr(spi_transaction_t *trans)
{
    mp_spi_device_callbacks_t *self = (mp_spi_device_callbacks_t*)trans->user;
    if (self) {
        self->spi_transaction_ptr.ptr = trans;
        cb_isr(self->post_cb, MP_OBJ_FROM_PTR(self));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ILI9xxx flush and ISR implementation in C

#include "lvgl/lvgl.h"

DMA_ATTR static uint8_t dma_buf[4] = {0};
DMA_ATTR static spi_transaction_t spi_trans = {0};

static void spi_send_value(const uint8_t *value, uint8_t size, spi_device_handle_t spi)
{
    spi_trans.length = size*8;
    spi_trans.tx_buffer = value;
    spi_trans.user = NULL;
    spi_device_polling_transmit(spi, &spi_trans);
}

static inline void ili9xxx_send_cmd(uint8_t cmd, int dc, spi_device_handle_t spi)
{
   dma_buf[0] = cmd;
   gpio_set_level(dc, 0);
   spi_send_value(dma_buf, 1, spi);
}

static inline void ili9xxx_send_data(const uint8_t *value, int dc, spi_device_handle_t spi)
{
   gpio_set_level(dc, 1);
   spi_send_value(value, 4, spi);
}

static void ili9xxx_send_data_dma(void *disp_drv, void *data, size_t size, int dc, spi_device_handle_t spi)
{
    gpio_set_level(dc, 1);
    spi_trans.length = size*8;
    spi_trans.tx_buffer = data;
    spi_trans.user = disp_drv;
    spi_device_queue_trans(spi, &spi_trans, -1);
}

void ili9xxx_post_cb_isr(spi_transaction_t *trans)
{
    if (trans->user)
        lv_display_flush_ready(trans->user);
}


typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} color24_t;

#define DISPLAY_TYPE_ILI9341 1
#define DISPLAY_TYPE_ILI9488 2
#define DISPLAY_TYPE_GC9A01  3
#define DISPLAY_TYPE_ST7789  4
#define DISPLAY_TYPE_ST7735  5

void ili9xxx_flush(void *_disp_drv, const void *_area, void *_color_p)
{
    lv_display_t *disp_drv = _disp_drv;
    const lv_area_t *area = _area;
    lv_color_t *color_p = _color_p;
    int start_x = 0;
    int start_y = 0;

    void *driver_data = lv_display_get_driver_data(disp_drv);

    // We use disp_drv->driver_data to pass data from MP to C
    // The following lines extract dc and spi

    int dc = mp_obj_get_int(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_dc)));
    int dt = mp_obj_get_int(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_dt)));
    mp_buffer_info_t buffer_info;
    mp_get_buffer_raise(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_spi)), &buffer_info, MP_BUFFER_READ);
    spi_device_handle_t *spi_ptr = buffer_info.buf;

    // Some devices may need a start_x and start_y offset for displays with LCD screens smaller
    // than the devices built in frame buffer.

    start_x = mp_obj_get_int(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_start_x)));
    start_y = mp_obj_get_int(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_start_y)));

    int x1 = area->x1 + start_x;
    int x2 = area->x2 + start_x;
    int y1 = area->y1 + start_y;
    int y2 = area->y2 + start_y;

    // Column addresses

    ili9xxx_send_cmd(0x2A, dc, *spi_ptr);
    dma_buf[0] = (x1 >> 8) & 0xFF;
    dma_buf[1] = x1 & 0xFF;
    dma_buf[2] = (x2 >> 8) & 0xFF;
    dma_buf[3] = x2 & 0xFF;

    ili9xxx_send_data(dma_buf, dc, *spi_ptr);

    // Page addresses

    ili9xxx_send_cmd(0x2B, dc, *spi_ptr);
    dma_buf[0] = (y1 >> 8) & 0xFF;
    dma_buf[1] = y1 & 0xFF;
    dma_buf[2] = (y2 >> 8) & 0xFF;
    dma_buf[3] = y2 & 0xFF;

    ili9xxx_send_data(dma_buf, dc, *spi_ptr);

    // Memory write by DMA, disp_flush_ready when finished

    ili9xxx_send_cmd(0x2C, dc, *spi_ptr);

    size_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    uint8_t color_size = 2;

    bool swap_rgb565_bytes = mp_obj_get_int(mp_obj_dict_get(driver_data, MP_OBJ_NEW_QSTR(MP_QSTR_swap_rgb565_bytes)));
    if ( swap_rgb565_bytes == true ) {
        lv_draw_sw_rgb565_swap(color_p, size);
    }

    if ( dt == DISPLAY_TYPE_ILI9488 ) {
        color_size = 3;
        /*Convert ARGB to RGB is required (cut off A-byte)*/
        size_t i;
        lv_color32_t* tmp32 = (lv_color32_t*) color_p;
        color24_t* tmp24 = (color24_t*) color_p;

        for(i=0; i < size; i++) {
            tmp24[i].red = tmp32[i].red;
            tmp24[i].green = tmp32[i].green;
            tmp24[i].blue = tmp32[i].blue;
        }
    }

    ili9xxx_send_data_dma(disp_drv, color_p, size * color_size, dc, *spi_ptr);
}
