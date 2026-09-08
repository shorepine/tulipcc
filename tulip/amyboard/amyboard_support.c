#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "amy.h"

// Stuff just for amyboard -- cv in/out direct , i2c in/out

#ifdef ESP_PLATFORM
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_task.h"
#include "driver/i2c.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_rom_sys.h"   // esp_rom_delay_us for the sub-tick ADC conversion wait
#include "esp_timer.h"     // esp_timer_get_time for the CV pair skew measurement
#include "driver/i2s_std.h"


#include "pins.h"


TaskHandle_t i2c_check_for_data_handle;


#define AMY_I2C_CHECK_FOR_DATA_TASK_COREID (1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_NAME "amyboard_i2c"
#define ALLES_I2C_CHECK_FOR_DATA_TASK_STACK_SIZE (16 * 1024)


// i2c stuff
#define I2C_CLK_FREQ 400000
#define DATA_LENGTH MAX_MESSAGE_LEN
#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_FOLLOWER_NUM I2C_NUMBER(1) /*!< I2C port number for follower dev */
#define I2C_FOLLOWER_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C follower tx buffer size */
#define I2C_FOLLOWER_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C follower rx buffer size */

#define AMYCHIP_ADDR 0x3F
#define PCM9211_ADDR 0x40
#define ADS1015_ADDR 0x48
#define GP8413_ADDR  0x58

esp_err_t i2c_follower_init() {
    i2c_port_t i2c_follower_port = I2C_FOLLOWER_NUM;
    i2c_config_t conf_follower;
    conf_follower.sda_io_num = I2C_FOLLOWER_SDA;
    conf_follower.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_follower.scl_io_num = I2C_FOLLOWER_SCL;
    conf_follower.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_follower.mode = I2C_MODE_SLAVE;
    conf_follower.slave.addr_10bit_en = 0;
    conf_follower.slave.slave_addr = AMYCHIP_ADDR;
    conf_follower.slave.maximum_speed = I2C_CLK_FREQ; // expected maximum clock speed
    conf_follower.clk_flags =0;
    i2c_param_config(i2c_follower_port, &conf_follower);
    return i2c_driver_install(i2c_follower_port, conf_follower.mode,
                            I2C_FOLLOWER_RX_BUF_LEN, I2C_FOLLOWER_TX_BUF_LEN, 0);
}


uint8_t i2c_buffer[MAX_MESSAGE_LEN];

// AMY message framing for the I2C follower. The ESP-IDF slave driver hands us
// a byte ring with no transaction boundaries: an AMY message longer than one
// host write arrives split across reads, and back-to-back short messages can
// arrive coalesced in one read. So accumulate bytes and dispatch only complete
// messages. A message ends at 'Z' -- except a message carrying 'u' (patch
// string), which is itself a concatenation of Z-terminated messages and so
// ends at the doubled 'ZZ' (last inner terminator + the message's own; a host
// must always end a u-message with ZZ). Each complete message is dispatched
// on its own, so a leading 'H' (ticks scheduling) stays the first byte and a
// 'u' can't swallow a following message.
//
// Known limits of a byte scan (fine for real wire traffic, documented so
// nobody rediscovers them): a 'u' byte inside any argument triggers the ZZ
// rule, and a patch string with interior ZZs (mapping commands stored inside
// a patch, like the firmware drum kits in amy's patches.h -- those load by
// K number, they don't travel over the wire) ends at its first ZZ.
static uint8_t i2c_msg[MAX_MESSAGE_LEN];
static size_t i2c_msg_len = 0;
static uint8_t i2c_msg_has_u = 0;
static uint8_t i2c_msg_resync = 0;

static void i2c_accumulate_byte(uint8_t b) {
    if(i2c_msg_resync) {
        // Recovering from an oversized message: discard up to and including
        // the next 'Z', then start clean.
        if(b == 'Z') i2c_msg_resync = 0;
        return;
    }
    if(i2c_msg_len >= MAX_MESSAGE_LEN - 1) {
        // Longer than AMY's own message limit, so it could never parse (or a
        // terminator was lost to a dropped byte). Drop it and resync at the
        // next 'Z' rather than wedging the stream waiting for a terminator.
        fprintf(stderr, "amyboard i2c: message exceeds %d bytes, dropping\n", MAX_MESSAGE_LEN - 1);
        i2c_msg_len = 0;
        i2c_msg_has_u = 0;
        i2c_msg_resync = 1;
        return;
    }
    i2c_msg[i2c_msg_len++] = b;
    if(b == 'u') i2c_msg_has_u = 1;
    if(b == 'Z' && (!i2c_msg_has_u || (i2c_msg_len >= 2 && i2c_msg[i2c_msg_len - 2] == 'Z'))) {
        i2c_msg[i2c_msg_len] = 0;
        amy_add_message((char*)i2c_msg);
        i2c_msg_len = 0;
        i2c_msg_has_u = 0;
    }
}

void i2c_check_for_data() {
    while(1) {
        size_t size = i2c_slave_read_buffer(I2C_FOLLOWER_NUM, i2c_buffer, MAX_MESSAGE_LEN, 10 / portTICK_PERIOD_MS);
        for(size_t i = 0; i < size; i++) i2c_accumulate_byte(i2c_buffer[i]);
    }
}



// ---------------------------------------------------------------------------
// Background I2C write queue.  The OLED framebuffer flush takes ~200ms of bus
// time at 400kHz; done synchronously from MicroPython it froze Python (and
// starved the CV tasks' port mutex) for that long.  Python instead enqueues
// each transaction here and returns immediately; this low-priority task plays
// them out in order on I2C_NUM_0.  The legacy IDF driver's per-port mutex
// serializes us against machine.I2C and the CV read/write hooks, and because
// Python chunks large payloads into small transactions, those users interleave
// between chunks instead of timing out.  Single producer (the MicroPython
// task) / single consumer (this task), which is all a MessageBuffer allows.

#define I2C_BG_QUEUE_BYTES 16384      // > one full 8KB OLED frame incl. overhead
#define I2C_BG_MAX_PAYLOAD 1024       // per-transaction cap (Python sends ~256B)
#define I2C_BG_TASK_STACK_SIZE 4096
#define I2C_BG_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)  // same as cv_read_task
#define I2C_BG_TASK_COREID (0)                        // MicroPython runs on core 1

static MessageBufferHandle_t i2c_bg_mb = NULL;
static volatile uint32_t i2c_bg_errors_count = 0;
static volatile uint8_t i2c_bg_in_flight = 0;

static void i2c_bg_task(void *pvParameter) {
    static uint8_t msg[I2C_BG_MAX_PAYLOAD + 1];  // [addr][payload...]
    for(;;) {
        size_t n = xMessageBufferReceive(i2c_bg_mb, msg, sizeof(msg), portMAX_DELAY);
        if(n < 2) continue;
        i2c_bg_in_flight = 1;
        esp_err_t ret = i2c_master_write_to_device(I2C_NUM_0, msg[0], msg + 1, n - 1, pdMS_TO_TICKS(100));
        i2c_bg_in_flight = 0;
        if(ret != ESP_OK) i2c_bg_errors_count++;
    }
}

// Enqueue one I2C write transaction. Returns 0 on success, 1 if it had to be
// dropped (queue stayed full / payload too big) -- the failure also bumps
// i2c_bg_errors() so Python can schedule a full panel resync.
uint8_t amyboard_i2c_bg_write(uint8_t addr, const uint8_t *buf, uint32_t len) {
    // Only ever called from the MicroPython task (single producer).
    static uint8_t msg[I2C_BG_MAX_PAYLOAD + 1];
    if(len == 0 || len > I2C_BG_MAX_PAYLOAD) {
        i2c_bg_errors_count++;
        return 1;
    }
    if(i2c_bg_mb == NULL) {
        i2c_bg_mb = xMessageBufferCreate(I2C_BG_QUEUE_BYTES);
        xTaskCreatePinnedToCore(i2c_bg_task, "i2c_bg", I2C_BG_TASK_STACK_SIZE / sizeof(StackType_t),
                                NULL, I2C_BG_TASK_PRIORITY, NULL, I2C_BG_TASK_COREID);
    }
    msg[0] = addr;
    memcpy(msg + 1, buf, len);
    // Blocking here is backpressure: it only happens when more than a whole
    // queued frame is outstanding, and it bounds how far Python can run ahead.
    if(xMessageBufferSend(i2c_bg_mb, msg, len + 1, pdMS_TO_TICKS(500)) != len + 1) {
        i2c_bg_errors_count++;
        return 1;
    }
    return 0;
}

// Bytes still queued (plus any transaction currently on the wire); 0 == idle.
uint32_t amyboard_i2c_bg_pending(void) {
    if(i2c_bg_mb == NULL) return 0;
    uint32_t queued = I2C_BG_QUEUE_BYTES - (uint32_t)xMessageBufferSpacesAvailable(i2c_bg_mb);
    return queued + i2c_bg_in_flight;
}

uint32_t amyboard_i2c_bg_errors(void) {
    return i2c_bg_errors_count;
}

#define ADS1015_REGISTER_CONFIG (0x01)
#define ADS1015_CQUE_DISABLE (0x0003)

#define ADS1015_CLAT_NONLAT (0x0000)
#define ADS1015_CPOL_ACTVLOW (0x0000)
#define ADS1015_CMODE_TRAD (0x0000)
#define ADS1015_DR_3300SPS (0x00E0)  // 0x00E0 = 3300 SPS (max) on the ADS1015
#define ADS1015_MODE_SINGLE (0x0100)
#define ADS1015_MODE_CONT (0x0100)
#define ADS1015_OS_SINGLE (0x8000)  // initiate single conversion / check converter status
#define ADS1015_OS_READY (0x8000) // OS bit reads 1 when conversion is complete
#define ADS1015_PGA_2_048V (0x0400)
//#define ADS1015_PGA_4_096V (0x0200)
#define ADS1015_MUX_SINGLE_0 (0x4000)
//#define ADS1015_MUX_PER_CHAN (0x1000)
// To get the offset to ADS1015_MUX_SINGLE_0 for chan C, use (C << ADS1015_MUX_CHAN_SHIFTL)
#define ADS1015_MUX_CHAN_SHIFTL (12)
//#define ADS1015_MUX_SINGLE_1 (0x5000)
//#define ADS1015_MUX_SINGLE_2 (0x6000)
//#define ADS1015_MUX_SINGLE_3 (0x7000)
#define ADS1015_REGISTER_CONVERT (0x00)

static esp_err_t ads1015_write_register(uint8_t reg, uint16_t data) {
    i2c_cmd_handle_t cmd;
    esp_err_t ret;
    uint8_t out[2];

    out[0] = data >> 8; // get 8 greater bits
    out[1] = data & 0xFF; // get 8 lower bits
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd); // generate a start command
    i2c_master_write_byte(cmd,(ADS1015_ADDR<<1) | I2C_MASTER_WRITE,1); // specify address and write command
    i2c_master_write_byte(cmd,reg,1); // specify register
    i2c_master_write(cmd,out,2,1); // write it
    i2c_master_stop(cmd); // generate a stop command
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10)); // send the i2c command
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t ads1015_read_register(uint8_t reg, uint8_t* data, uint8_t len) {
    // Standard I2C register read: write the register pointer, then a *repeated*
    // START to turn the bus around and read, all in one transaction.
    //
    // This used to be two separate transactions with a STOP between them, which
    // cost two acquisitions of the driver's per-port mutex (held for the whole
    // of i2c_master_cmd_begin, i2c.c:1554-1642) plus an extra address byte on
    // the wire, and gave any other user of I2C_NUM_0 -- the audio thread's CV
    // DAC writes, the OLED flush task, machine.I2C -- a window to slip in
    // between the pointer write and the read. It also meant the pointer write
    // could succeed while the read failed (or vice versa), which is how a read
    // could come back holding the wrong register's contents.
    //
    // The last byte of a master read is NACKed (I2C_MASTER_LAST_NACK) rather
    // than ACKed as before. ACKing it tells the device to put another byte on
    // the bus, so it can still be driving SDA when the STOP is issued -- a
    // classic source of intermittent corruption on the next transaction.
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,(ADS1015_ADDR<<1) | I2C_MASTER_WRITE,1); // address + write
    i2c_master_write_byte(cmd,reg,1);                                  // register pointer
    i2c_master_start(cmd);                                             // repeated START
    i2c_master_write_byte(cmd,(ADS1015_ADDR<<1) | I2C_MASTER_READ,1);  // address + read
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// One conversion at 3300 SPS takes 1/3300 = 303us, so this is the conversion
// time plus a little margin. Spun out directly for ch0 (inside the skew window)
// and used as the spin quantum for the rare early wake in
// ads1015_wait_ready_yielding(), which handles ch1.
#define ADS1015_CONVERSION_US (320)

static esp_err_t ads1015_start_conversion(uint8_t channel) {
    uint16_t channel_mux = ADS1015_MUX_SINGLE_0 + (channel << ADS1015_MUX_CHAN_SHIFTL);
    uint16_t data = (ADS1015_CQUE_DISABLE | ADS1015_CLAT_NONLAT |
                     ADS1015_CPOL_ACTVLOW | ADS1015_CMODE_TRAD | ADS1015_DR_3300SPS |
                     ADS1015_MODE_SINGLE | ADS1015_OS_SINGLE | ADS1015_PGA_2_048V |
                     channel_mux);
    return ads1015_write_register(ADS1015_REGISTER_CONFIG, data);
}

static esp_err_t ads1015_read_convert(uint16_t *out) {
    uint8_t buffer[2];
    esp_err_t ret = ads1015_read_register(ADS1015_REGISTER_CONVERT, buffer, 2);
    if(ret != ESP_OK) return ret;
    *out = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
    return ESP_OK;
}

// Confirm the ADC is present, responding, and idle (OS reads 1 when no
// conversion is running) before committing to the sequence below. This lives
// here, ahead of the pair, rather than between the two conversions: a CONFIG
// read is ~115us on the wire plus driver overhead, and inline it would be pure
// added skew.
static esp_err_t ads1015_check_idle(void) {
    uint8_t buffer[2];
    esp_err_t ret = ads1015_read_register(ADS1015_REGISTER_CONFIG, buffer, 2);
    if(ret != ESP_OK) return ret;
    if(!((((uint16_t)buffer[0] << 8) | buffer[1]) & ADS1015_OS_READY)) return ESP_ERR_INVALID_STATE;
    return ESP_OK;
}

// Wait out a conversion by yielding instead of spinning. vTaskDelay(1) wakes on
// the next tick boundary, so it returns anywhere between ~0 and one full tick
// depending on where in the tick period we started -- on its own it is NOT a
// guarantee that the 303us conversion finished, and reading CONVERT early would
// hand back the *previous* channel's sample. So confirm with the OS bit and
// spin out only the remainder on the rare early wake.
//
// Used for ch1 only, whose wait falls after the last timestamp feeding the skew
// measurement. ch0's wait is inside the skew window and must spin.
//
// vTaskDelay() returns when this task is next *scheduled*, not at the tick edge:
// at PRIO_MIN+2 on core 0 that means waiting out AMY's render task
// (PRIO_MAX-1, same core, every 5.8ms). Measured on an AMYboard with an 8-voice
// piano playing and Python scanning the bus, yielding both waits gave a median
// skew of 4922us (p90 5506us) -- no better than the ~5ms of the speculative
// scheme this replaced. Spinning ch0's wait alone brings the median to 735us
// (p90 4780us, the tail being render preemption landing between the two
// conversions) for 320us of busy-wait per 6ms period, ~5% of core 0.
static esp_err_t ads1015_wait_ready_yielding(void) {
    vTaskDelay(1);
    for(int i = 0; i < 5; i++) {
        esp_err_t ret = ads1015_check_idle();
        if(ret != ESP_ERR_INVALID_STATE) return ret;  // ESP_OK, or a real I2C failure
        esp_rom_delay_us(ADS1015_CONVERSION_US / 4);
    }
    return ESP_ERR_TIMEOUT;
}

// CV read diagnostics, readable from Python as tulip.cv_stats().
static volatile uint32_t cv_read_errors_count = 0;
static volatile uint32_t cv_pair_skew_us = 0;
static volatile uint32_t cv_pair_skew_max_us = 0;

// Read both CV channels as a tightly-spaced pair.
//
// The ADS1015 has one mux and one delta-sigma modulator, so the two channels
// can never be sampled at the same instant -- and because it is delta-sigma,
// each reading integrates over its own ~303us conversion window, so
// "simultaneous" isn't meaningful below that scale anyway. What this buys is
// two *adjacent* conversion windows: the gap between the two conversion starts
// is one conversion plus one CONVERT read plus one CONFIG write, roughly
// 0.6-0.8ms.
//
// The scheme this replaces started ch0's conversion at the *end* of the
// previous iteration ("speculatively", to save a wait) and then let the result
// sit in the CONVERT register across the whole 6ms task delay, so ch0's sample
// was ~6ms old while ch1's was fresh -- about 5ms of skew, structural rather
// than jitter, and the worst arrangement available for reading a CV pair.
//
// It also drops the cached ads1015_pending_channel: every conversion is now
// started explicitly with its result checked, so there is no cached notion of
// which channel the mux is on that can drift out of step with the hardware.
//
// Measured on an AMYboard: median skew 721us idle, 735us under load (8-voice
// piano + Python bus scans), p90 4780us under load. Reported live as
// cv_pair_skew_us / cv_pair_skew_max_us via tulip.cv_stats().
static esp_err_t ads1015_read_pair(uint16_t *raw) {
    esp_err_t ret = ads1015_check_idle();
    if(ret != ESP_OK) return ret;

    int64_t t0 = esp_timer_get_time();
    ret = ads1015_start_conversion(0);
    if(ret != ESP_OK) return ret;
    // Spin, don't yield: this wait is inside the skew window. Measured on an
    // AMYboard, yielding here costs a median skew of 4922us under load vs
    // 735us spinning -- see ads1015_wait_ready_yielding().
    esp_rom_delay_us(ADS1015_CONVERSION_US);
    ret = ads1015_read_convert(&raw[0]);
    if(ret != ESP_OK) return ret;

    int64_t t1 = esp_timer_get_time();
    ret = ads1015_start_conversion(1);
    if(ret != ESP_OK) return ret;
    ret = ads1015_wait_ready_yielding();
    if(ret != ESP_OK) return ret;
    ret = ads1015_read_convert(&raw[1]);
    if(ret != ESP_OK) return ret;

    // Interval between the two conversion starts. Nothing pins this task to the
    // CPU across the pair, so preemption (the I2C follower task runs at
    // priority 20 on this core) shows up here as outliers well above the
    // ~0.6-0.8ms floor.
    uint32_t skew = (uint32_t)(t1 - t0);
    cv_pair_skew_us = skew;
    if(skew > cv_pair_skew_max_us) cv_pair_skew_max_us = skew;
    return ESP_OK;
}

uint32_t amyboard_cv_errors(void) { return cv_read_errors_count; }
uint32_t amyboard_cv_skew_us(void) { return cv_pair_skew_us; }
uint32_t amyboard_cv_skew_max_us(void) { return cv_pair_skew_max_us; }
void amyboard_cv_stats_reset(void) {
    cv_read_errors_count = 0;
    cv_pair_skew_max_us = 0;
}

#endif // ESP_PLATFORM

extern uint8_t * external_map;
float cv_local_value[2];
uint8_t cv_local_override[2];

// Cached CV input values -- updated by a dedicated FreeRTOS task so the audio
// render thread never blocks on I2C.
//
// Double-buffered: the task fills the back buffer and publishes it with a
// single index store, so any one read sees a coherent snapshot of both
// channels rather than a pair torn across an update. (Two separate reads can
// still straddle a publish, but they are microseconds apart within one audio
// block against a 6ms update period.)
static float cv_pair_buf[2][2] = {{0, 0}, {0, 0}};
static volatile uint8_t cv_pair_index = 0;

// Called from AMY's coef hook on the audio thread and from tulip.cv_in() on the
// MicroPython task. Both are pure reads of the published snapshot -- neither
// touches I2C, so they cannot contend with each other or with cv_read_task.
float cv_input_hook(uint16_t channel) {
    if(channel > 1) return 0;
    if(cv_local_override[channel]) {
        return cv_local_value[channel];
    }
    return cv_pair_buf[cv_pair_index][channel];
}

#ifdef ESP_PLATFORM
// FreeRTOS task: reads both ADS1015 channels as a tightly-spaced pair and
// publishes them together, once per cv_period.
void cv_read_task(void *pvParameter) {
    // Bench-calibrated (loopback vs multimeter, 2026-08-07): raw = 20080 + 2003*V.
    // The ADC saturates at raw 32752 / raw 0, so the readable window is about
    // -10V to +6.3V -- inputs above +6.3V clip (the jack itself is fine to ±10V).
    const int32_t min = 10064; // -5V
    const int32_t max = 30096; // +5V
    // Scan both CV channels once per AMY audio block (AMY_BLOCK_SIZE / AMY_SAMPLE_RATE,
    // ~5.8ms at 256/44100) so CV tracks the audio block cadence. Expressed in RTOS ticks,
    // rounded to nearest, so it follows the audio rate regardless of tick rate; clamped to
    // >=1 tick. (At configTICK_RATE_HZ=1000 this is 6 ticks; 5.8ms can't be hit exactly.)
    TickType_t cv_period = (AMY_BLOCK_SIZE * configTICK_RATE_HZ + AMY_SAMPLE_RATE / 2) / AMY_SAMPLE_RATE;
    if(cv_period == 0) cv_period = 1;
    // xTaskDelayUntil holds a fixed period regardless of how long the two ADS1015
    // conversions take, unlike vTaskDelay which would add the read time on top.
    TickType_t last_wake = xTaskGetTickCount();
    for(;;) {
        // Overriding both channels from Python (tulip.cv_local) takes the ADC
        // out of the loop entirely -- hwci's no_cv A/B run depends on this to
        // remove all CV I2C traffic while the task itself keeps running.
        if(!(cv_local_override[0] && cv_local_override[1])) {
            uint16_t raw[2];
            if(ads1015_read_pair(raw) == ESP_OK) {
                uint8_t back = 1 - cv_pair_index;
                for(uint8_t ch = 0; ch < 2; ch++) {
                    // Map [min, max] -> [-5v, +5v]
                    cv_pair_buf[back][ch] = (
                        (((float)((int32_t)raw[ch] - min))
                         / ((float)(max - min)))
                        * 10.0f
                    ) - 5.0f;
                }
                cv_pair_index = back;  // publish both channels at once
            } else {
                // Hold the last good pair rather than publishing a bogus one.
                // The old read path returned a bare uint16_t with no way to
                // report failure, so a timed-out or misdirected read published
                // whatever bytes happened to be in the buffer.
                cv_read_errors_count++;
            }
        }
        xTaskDelayUntil(&last_wake, cv_period);
    }
}
#endif

