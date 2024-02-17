# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 418 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 24 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/lv_init.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/lv_init.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/_fake_defines.h" 1
# 2 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/_fake_typedefs.h" 1



typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int __s8;
typedef int __u8;
typedef int __s16;
typedef int __u16;
typedef int __s32;
typedef int __u32;
typedef int __s64;
typedef int __u64;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int char16_t;
typedef int char32_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sa_family_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;


typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;


typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;


typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;


typedef int intptr_t;
typedef int uintptr_t;


typedef int intmax_t;
typedef int uintmax_t;


typedef _Bool bool;


typedef void* MirEGLNativeWindowType;
typedef void* MirEGLNativeDisplayType;
typedef struct MirConnection MirConnection;
typedef struct MirSurface MirSurface;
typedef struct MirSurfaceSpec MirSurfaceSpec;
typedef struct MirScreencast MirScreencast;
typedef struct MirPromptSession MirPromptSession;
typedef struct MirBufferStream MirBufferStream;
typedef struct MirPersistentId MirPersistentId;
typedef struct MirBlob MirBlob;
typedef struct MirDisplayConfig MirDisplayConfig;


typedef struct xcb_connection_t xcb_connection_t;
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_visualid_t;


typedef _Atomic(_Bool) atomic_bool;
typedef _Atomic(char) atomic_char;
typedef _Atomic(signed char) atomic_schar;
typedef _Atomic(unsigned char) atomic_uchar;
typedef _Atomic(short) atomic_short;
typedef _Atomic(unsigned short) atomic_ushort;
typedef _Atomic(int) atomic_int;
typedef _Atomic(unsigned int) atomic_uint;
typedef _Atomic(long) atomic_long;
typedef _Atomic(unsigned long) atomic_ulong;
typedef _Atomic(long long) atomic_llong;
typedef _Atomic(unsigned long long) atomic_ullong;
typedef _Atomic(uint_least16_t) atomic_char16_t;
typedef _Atomic(uint_least32_t) atomic_char32_t;
typedef _Atomic(wchar_t) atomic_wchar_t;
typedef _Atomic(int_least8_t) atomic_int_least8_t;
typedef _Atomic(uint_least8_t) atomic_uint_least8_t;
typedef _Atomic(int_least16_t) atomic_int_least16_t;
typedef _Atomic(uint_least16_t) atomic_uint_least16_t;
typedef _Atomic(int_least32_t) atomic_int_least32_t;
typedef _Atomic(uint_least32_t) atomic_uint_least32_t;
typedef _Atomic(int_least64_t) atomic_int_least64_t;
typedef _Atomic(uint_least64_t) atomic_uint_least64_t;
typedef _Atomic(int_fast8_t) atomic_int_fast8_t;
typedef _Atomic(uint_fast8_t) atomic_uint_fast8_t;
typedef _Atomic(int_fast16_t) atomic_int_fast16_t;
typedef _Atomic(uint_fast16_t) atomic_uint_fast16_t;
typedef _Atomic(int_fast32_t) atomic_int_fast32_t;
typedef _Atomic(uint_fast32_t) atomic_uint_fast32_t;
typedef _Atomic(int_fast64_t) atomic_int_fast64_t;
typedef _Atomic(uint_fast64_t) atomic_uint_fast64_t;
typedef _Atomic(intptr_t) atomic_intptr_t;
typedef _Atomic(uintptr_t) atomic_uintptr_t;
typedef _Atomic(size_t) atomic_size_t;
typedef _Atomic(ptrdiff_t) atomic_ptrdiff_t;
typedef _Atomic(intmax_t) atomic_intmax_t;
typedef _Atomic(uintmax_t) atomic_uintmax_t;
typedef struct atomic_flag { atomic_bool _Value; } atomic_flag;
typedef enum memory_order {
  memory_order_relaxed,
  memory_order_consume,
  memory_order_acquire,
  memory_order_release,
  memory_order_acq_rel,
  memory_order_seq_cst
} memory_order;
# 3 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 2
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/lv_init.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h" 1
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_kconfig.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h" 2
# 57 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h"
# 1 "../../micropython/../lv_binding_micropython/lv_conf.h" 1
# 58 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h" 2
# 3119 "../../micropython/../lv_binding_micropython/lvgl/src/lv_conf_internal.h"
enum {ENUM_LV_DPI_DEF = 130};
enum {ENUM_LV_DRAW_BUF_STRIDE_ALIGN = 1};
enum {ENUM_LV_DRAW_BUF_ALIGN = 4};
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/lv_init.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/lv_init.h"
void lv_init(void);




void lv_deinit(void);




bool lv_is_initialized(void);
# 25 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/string.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_types.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_types.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_types.h" 2
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_types.h"
enum _lv_result_t {
    LV_RESULT_INVALID = 0,

    LV_RESULT_OK,
};




typedef uint8_t lv_result_t;




typedef uintptr_t lv_uintptr_t;
typedef intptr_t lv_intptr_t;
# 82 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_types.h"
typedef int32_t lv_value_precise_t;







struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;






typedef uint16_t lv_state_t;
typedef uint32_t lv_part_t;
typedef uint32_t lv_obj_flag_t;


struct _lv_obj_class_t;
typedef struct _lv_obj_class_t lv_obj_class_t;

struct _lv_group_t;
typedef struct _lv_group_t lv_group_t;




typedef uint8_t lv_key_t;


struct _lv_display_t;
typedef struct _lv_display_t lv_display_t;

struct _lv_layer_t;
typedef struct _lv_layer_t lv_layer_t;
struct _lv_draw_unit_t;
typedef struct _lv_draw_unit_t lv_draw_unit_t;
struct _lv_draw_task_t;
typedef struct _lv_draw_task_t lv_draw_task_t;

struct _lv_indev_t;
typedef struct _lv_indev_t lv_indev_t;

struct _lv_event_t;
typedef struct _lv_event_t lv_event_t;

struct _lv_timer_t;
typedef struct _lv_timer_t lv_timer_t;

struct _lv_theme_t;
typedef struct _lv_theme_t lv_theme_t;

struct _lv_anim_t;
typedef struct _lv_anim_t lv_anim_t;

struct _lv_font_t;
typedef struct _lv_font_t lv_font_t;

struct _lv_image_decoder_t;
typedef struct _lv_image_decoder_t lv_image_decoder_t;
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h"
typedef void * lv_mem_pool_t;




typedef struct {
    uint32_t total_size;
    uint32_t free_cnt;
    uint32_t free_size;
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint32_t max_used;
    uint8_t used_pct;
    uint8_t frag_pct;
} lv_mem_monitor_t;
# 55 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h"
void lv_mem_init(void);




void lv_mem_deinit(void);

lv_mem_pool_t lv_mem_add_pool(void * mem, size_t bytes);

void lv_mem_remove_pool(lv_mem_pool_t pool);






void * lv_malloc(size_t size);






void * lv_malloc_zeroed(size_t size);





void lv_free(void * data);
# 93 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_mem.h"
void * lv_realloc(void * data_p, size_t new_size);





void * lv_malloc_core(size_t size);





void lv_free_core(void * p);






void * lv_realloc_core(void * p, size_t new_size);





void lv_mem_monitor_core(lv_mem_monitor_t * mon_p);

lv_result_t lv_mem_test_core(void);





lv_result_t lv_mem_test(void);






void lv_mem_monitor(lv_mem_monitor_t * mon_p);
# 27 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h" 2
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
void * lv_memcpy(void * dst, const void * src, size_t len);
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
void lv_memset(void * dst, uint8_t v, size_t len);
# 58 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
void * lv_memmove(void * dst, const void * src, size_t len);






static inline void lv_memzero(void * dst, size_t len)
{
    lv_memset(dst, 0x00, len);
}






size_t lv_strlen(const char * str);
# 84 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
char * lv_strncpy(char * dst, const char * src, size_t dest_size);
# 93 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_string.h"
char * lv_strcpy(char * dst, const char * src);







int32_t lv_strcmp(const char * s1, const char * s2);






char * lv_strdup(const char * src);






char * lv_strdup(const char * src);
# 28 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h" 1
# 11 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/inttypes.h" 1
# 12 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h" 2
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdarg.h" 1
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/lv_sprintf.h" 2





int lv_snprintf(char * buffer, size_t count, const char * format, ...);

int lv_vsnprintf(char * buffer, size_t count, const char * format, va_list va);
# 29 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_log.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_log.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_log.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_log.h"
enum {ENUM_LV_LOG_LEVEL_TRACE = 0};
enum {ENUM_LV_LOG_LEVEL_INFO = 1};
enum {ENUM_LV_LOG_LEVEL_WARN = 2};
enum {ENUM_LV_LOG_LEVEL_ERROR = 3};
enum {ENUM_LV_LOG_LEVEL_USER = 4};
enum {ENUM_LV_LOG_LEVEL_NONE = 5};

typedef int8_t lv_log_level_t;
# 31 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h" 2
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h"
typedef uint32_t (*lv_tick_get_cb_t)(void);

typedef void (*lv_delay_cb_t)(uint32_t ms);

typedef struct {
    uint32_t sys_time;
    volatile uint8_t sys_irq_flag;
    lv_tick_get_cb_t tick_get_cb;
    lv_delay_cb_t delay_cb;
} lv_tick_state_t;
# 50 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../tick/lv_tick.h"
                      void lv_tick_inc(uint32_t tick_period);





uint32_t lv_tick_get(void);






uint32_t lv_tick_elaps(uint32_t prev_tick);







void lv_delay_ms(uint32_t ms);





void lv_tick_set_cb(lv_tick_get_cb_t cb);





void lv_delay_set_cb(lv_delay_cb_t cb);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h" 2
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h"
typedef uint8_t lv_ll_node_t;


typedef struct {
    uint32_t n_size;
    lv_ll_node_t * head;
    lv_ll_node_t * tail;
} lv_ll_t;
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h"
void _lv_ll_init(lv_ll_t * ll_p, uint32_t node_size);






void * _lv_ll_ins_head(lv_ll_t * ll_p);







void * _lv_ll_ins_prev(lv_ll_t * ll_p, void * n_act);






void * _lv_ll_ins_tail(lv_ll_t * ll_p);







void _lv_ll_remove(lv_ll_t * ll_p, void * node_p);

void _lv_ll_clear_custom(lv_ll_t * ll_p, void(*cleanup)(void *));





static inline void _lv_ll_clear(lv_ll_t * ll_p)
{
    _lv_ll_clear_custom(ll_p, 0);
}
# 98 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h"
void _lv_ll_chg_list(lv_ll_t * ll_ori_p, lv_ll_t * ll_new_p, void * node, bool head);






void * _lv_ll_get_head(const lv_ll_t * ll_p);






void * _lv_ll_get_tail(const lv_ll_t * ll_p);







void * _lv_ll_get_next(const lv_ll_t * ll_p, const void * n_act);







void * _lv_ll_get_prev(const lv_ll_t * ll_p, const void * n_act);






uint32_t _lv_ll_get_len(const lv_ll_t * ll_p);
# 151 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_ll.h"
void _lv_ll_move_before(lv_ll_t * ll_p, void * n_act, void * n_after);






bool _lv_ll_is_empty(lv_ll_t * ll_p);
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h" 2

# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h" 2
# 39 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h"
typedef void (*lv_timer_cb_t)(lv_timer_t *);




typedef void (*lv_timer_handler_resume_cb_t)(void * data);




struct _lv_timer_t {
    uint32_t period;
    uint32_t last_run;
    lv_timer_cb_t timer_cb;
    void * user_data;
    int32_t repeat_count;
    uint32_t paused : 1;
    uint32_t auto_delete : 1;
};

typedef struct {
    lv_ll_t timer_ll;

    bool lv_timer_run;
    uint8_t idle_last;
    bool timer_deleted;
    bool timer_created;
    uint32_t timer_time_until_next;

    bool already_running;
    uint32_t periodic_last_tick;
    uint32_t busy_time;
    uint32_t idle_period_start;
    uint32_t run_cnt;

    lv_timer_handler_resume_cb_t resume_cb;
    void * resume_data;
} lv_timer_state_t;
# 85 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h"
void _lv_timer_core_init(void);




void _lv_timer_core_deinit(void);







                           uint32_t lv_timer_handler(void);
# 109 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h"
static inline uint32_t lv_timer_handler_run_in_period(uint32_t period)
{
    static uint32_t last_tick = 0;

    if(lv_tick_elaps(last_tick) >= period) {
        last_tick = lv_tick_get();
        return lv_timer_handler();
    }
    return 1;
}





                           void lv_timer_periodic_handler(void);






void lv_timer_handler_set_resume_cb(lv_timer_handler_resume_cb_t cb, void * data);






lv_timer_t * lv_timer_create_basic(void);
# 149 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_timer.h"
lv_timer_t * lv_timer_create(lv_timer_cb_t timer_xcb, uint32_t period, void * user_data);





void lv_timer_delete(lv_timer_t * timer);





void lv_timer_pause(lv_timer_t * timer);





void lv_timer_resume(lv_timer_t * timer);






void lv_timer_set_cb(lv_timer_t * timer, lv_timer_cb_t timer_cb);






void lv_timer_set_period(lv_timer_t * timer, uint32_t period);





void lv_timer_ready(lv_timer_t * timer);






void lv_timer_set_repeat_count(lv_timer_t * timer, int32_t repeat_count);






void lv_timer_set_auto_delete(lv_timer_t * timer, bool auto_delete);






void lv_timer_set_user_data(lv_timer_t * timer, void * user_data);






void lv_timer_reset(lv_timer_t * timer);





void lv_timer_enable(bool en);





uint32_t lv_timer_get_idle(void);





uint32_t lv_timer_get_time_until_next(void);






lv_timer_t * lv_timer_get_next(lv_timer_t * timer);






static inline void * lv_timer_get_user_data(lv_timer_t * timer)
{
    return timer->user_data;
}






static inline bool lv_timer_get_paused(lv_timer_t * timer)
{
    return timer->paused;
}
# 32 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h" 2
# 37 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
typedef struct {
    uint16_t i;
    uint16_t f;
} lv_sqrt_res_t;
# 52 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
                      int32_t lv_trigo_sin(int16_t angle);

static inline int32_t lv_trigo_cos(int16_t angle)
{
    return lv_trigo_sin(angle + 90);
}
# 70 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
int32_t lv_cubic_bezier(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
# 81 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
static inline int32_t lv_bezier3(int32_t t, int32_t u0, uint32_t u1, int32_t u2, int32_t u3)
{
    ((void)u0);
    ((void)u3);
    return lv_cubic_bezier(t, 341, u1, 683, u2);
}







uint16_t lv_atan2(int x, int y);
# 108 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
                      void lv_sqrt(uint32_t x, lv_sqrt_res_t * q, uint32_t mask);
# 118 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
int64_t lv_pow(int64_t base, int8_t exp);
# 129 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_math.h"
int32_t lv_map(int32_t x, int32_t min_in, int32_t max_in, int32_t min_out, int32_t max_out);





void lv_rand_set_seed(uint32_t seed);







uint32_t lv_rand(uint32_t min, uint32_t max);
# 33 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
typedef struct {
    uint8_t * data;
    uint32_t size;
    uint32_t capacity;
    uint32_t element_size;
} lv_array_t;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
void lv_array_init(lv_array_t * array, uint32_t capacity, uint32_t element_size);







void lv_array_resize(lv_array_t * array, uint32_t new_capacity);





void lv_array_deinit(lv_array_t * array);






static inline uint32_t lv_array_size(const lv_array_t * array)
{
    return array->size;
}






static inline uint32_t lv_array_capacity(const lv_array_t * array)
{
    return array->capacity;
}






static inline bool lv_array_is_empty(const lv_array_t * array)
{
    return array->size == 0;
}






static inline bool lv_array_is_full(const lv_array_t * array)
{
    return array->size == array->capacity;
}







void lv_array_copy(lv_array_t * target, const lv_array_t * source);





static inline void lv_array_clear(lv_array_t * array)
{
    array->size = 0;
}







lv_result_t lv_array_remove(lv_array_t * array, uint32_t index);
# 139 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
lv_result_t lv_array_erase(lv_array_t * array, uint32_t start, uint32_t end);
# 148 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
lv_result_t lv_array_concat(lv_array_t * array, const lv_array_t * other);
# 157 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
lv_result_t lv_array_push_back(lv_array_t * array, const void * element);
# 166 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_array.h"
lv_result_t lv_array_assign(lv_array_t * array, uint32_t index, const void * value);







void * lv_array_at(const lv_array_t * array, uint32_t index);






static inline void * lv_array_front(const lv_array_t * array)
{
    return lv_array_at(array, 0);
}





static inline void * lv_array_back(const lv_array_t * array)
{
    return lv_array_at(array, lv_array_size(array) - 1);
}
# 34 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_async.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_async.h"
typedef void (*lv_async_cb_t)(void *);
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_async.h"
lv_result_t lv_async_call(lv_async_cb_t async_xcb, void * user_data);






lv_result_t lv_async_call_cancel(lv_async_cb_t async_xcb, void * user_data);
# 35 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_anim_timeline.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_anim_timeline.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 24 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 25 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h" 2
# 79 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
enum {ENUM_LV_ANIM_REPEAT_INFINITE = 0xFFFF};
enum {ENUM_LV_ANIM_PLAYTIME_INFINITE = 0xFFFFFFFF};






typedef enum {
    LV_ANIM_OFF,
    LV_ANIM_ON,
} lv_anim_enable_t;

typedef struct {
    bool anim_list_changed;
    bool anim_run_round;
    lv_timer_t * timer;
    lv_ll_t anim_ll;
} lv_anim_state_t;


typedef int32_t (*lv_anim_path_cb_t)(const lv_anim_t *);







typedef void (*lv_anim_exec_xcb_t)(void *, int32_t);



typedef void (*lv_anim_custom_exec_cb_t)(lv_anim_t *, int32_t);


typedef void (*lv_anim_completed_cb_t)(lv_anim_t *);


typedef void (*lv_anim_start_cb_t)(lv_anim_t *);


typedef int32_t (*lv_anim_get_value_cb_t)(lv_anim_t *);


typedef void (*lv_anim_deleted_cb_t)(lv_anim_t *);

typedef struct _lv_anim_bezier3_para_t {
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} lv_anim_bezier3_para_t;


struct _lv_anim_t {
    void * var;
    lv_anim_exec_xcb_t exec_cb;
    lv_anim_custom_exec_cb_t custom_exec_cb;

    lv_anim_start_cb_t start_cb;
    lv_anim_completed_cb_t completed_cb;
    lv_anim_deleted_cb_t deleted_cb;
    lv_anim_get_value_cb_t get_value_cb;
    void * user_data;
    lv_anim_path_cb_t path_cb;
    int32_t start_value;
    int32_t current_value;
    int32_t end_value;
    int32_t duration;
    int32_t act_time;
    uint32_t playback_delay;
    uint32_t playback_duration;
    uint32_t repeat_delay;
    uint16_t repeat_cnt;
    union _lv_anim_path_para_t {
        lv_anim_bezier3_para_t bezier3;
    } parameter;

    uint8_t early_apply : 1;


    uint32_t last_timer_run;
    uint8_t playback_now : 1;
    uint8_t run_round : 1;
    uint8_t start_cb_called : 1;
};
# 174 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
void _lv_anim_core_init(void);




void _lv_anim_core_deinit(void);
# 190 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
void lv_anim_init(lv_anim_t * a);






static inline void lv_anim_set_var(lv_anim_t * a, void * var)
{
    a->var = var;
}
# 209 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
static inline void lv_anim_set_exec_cb(lv_anim_t * a, lv_anim_exec_xcb_t exec_cb)
{
    a->exec_cb = exec_cb;
}






static inline void lv_anim_set_duration(lv_anim_t * a, uint32_t duration)
{
    a->duration = duration;
}




static inline void lv_anim_set_time(lv_anim_t * a, uint32_t duration)
{
    lv_anim_set_duration(a, duration);
}






static inline void lv_anim_set_delay(lv_anim_t * a, uint32_t delay)
{
    a->act_time = -(int32_t)(delay);
}







static inline void lv_anim_set_values(lv_anim_t * a, int32_t start, int32_t end)
{
    a->start_value = start;
    a->current_value = INT32_MIN;
    a->end_value = end;
}
# 263 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
static inline void lv_anim_set_custom_exec_cb(lv_anim_t * a, lv_anim_custom_exec_cb_t exec_cb)
{
    a->custom_exec_cb = exec_cb;
}






static inline void lv_anim_set_path_cb(lv_anim_t * a, lv_anim_path_cb_t path_cb)
{
    a->path_cb = path_cb;
}






static inline void lv_anim_set_start_cb(lv_anim_t * a, lv_anim_start_cb_t start_cb)
{
    a->start_cb = start_cb;
}







static inline void lv_anim_set_get_value_cb(lv_anim_t * a, lv_anim_get_value_cb_t get_value_cb)
{
    a->get_value_cb = get_value_cb;
}






static inline void lv_anim_set_completed_cb(lv_anim_t * a, lv_anim_completed_cb_t completed_cb)
{
    a->completed_cb = completed_cb;
}






static inline void lv_anim_set_deleted_cb(lv_anim_t * a, lv_anim_deleted_cb_t deleted_cb)
{
    a->deleted_cb = deleted_cb;
}






static inline void lv_anim_set_playback_duration(lv_anim_t * a, uint32_t duration)
{
    a->playback_duration = duration;
}




static inline void lv_anim_set_playback_time(lv_anim_t * a, uint32_t duration)
{
    lv_anim_set_playback_duration(a, duration);
}






static inline void lv_anim_set_playback_delay(lv_anim_t * a, uint32_t delay)
{
    a->playback_delay = delay;
}






static inline void lv_anim_set_repeat_count(lv_anim_t * a, uint16_t cnt)
{
    a->repeat_cnt = cnt;
}






static inline void lv_anim_set_repeat_delay(lv_anim_t * a, uint32_t delay)
{
    a->repeat_delay = delay;
}







static inline void lv_anim_set_early_apply(lv_anim_t * a, bool en)
{
    a->early_apply = en;
}






static inline void lv_anim_set_user_data(lv_anim_t * a, void * user_data)
{
    a->user_data = user_data;
}
# 395 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
static inline void lv_anim_set_bezier3_param(lv_anim_t * a, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    struct _lv_anim_bezier3_para_t * para = &a->parameter.bezier3;

    para->x1 = x1;
    para->x2 = x2;
    para->y1 = y1;
    para->y2 = y2;
}






lv_anim_t * lv_anim_start(const lv_anim_t * a);






static inline uint32_t lv_anim_get_delay(const lv_anim_t * a)
{
    return -a->act_time;
}






uint32_t lv_anim_get_playtime(const lv_anim_t * a);






static inline uint32_t lv_anim_get_time(const lv_anim_t * a)
{
    return a->duration;
}






static inline uint16_t lv_anim_get_repeat_count(const lv_anim_t * a)
{
    return a->repeat_cnt;
}






static inline void * lv_anim_get_user_data(const lv_anim_t * a)
{
    return a->user_data;
}
# 466 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
bool lv_anim_delete(void * var, lv_anim_exec_xcb_t exec_cb);




void lv_anim_delete_all(void);







lv_anim_t * lv_anim_get(void * var, lv_anim_exec_xcb_t exec_cb);





lv_timer_t * lv_anim_get_timer(void);
# 498 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
static inline bool lv_anim_custom_delete(lv_anim_t * a, lv_anim_custom_exec_cb_t exec_cb)
{
    return lv_anim_delete(a ? a->var : 0, (lv_anim_exec_xcb_t)exec_cb);
}
# 512 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
static inline lv_anim_t * lv_anim_custom_get(lv_anim_t * a, lv_anim_custom_exec_cb_t exec_cb)
{
    return lv_anim_get(a ? a->var : 0, (lv_anim_exec_xcb_t)exec_cb);
}





uint16_t lv_anim_count_running(void);







uint32_t lv_anim_speed(uint32_t speed);
# 542 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_anim.h"
uint32_t lv_anim_speed_clamped(uint32_t speed, uint32_t min_time, uint32_t max_time);







void lv_anim_refr_now(void);






int32_t lv_anim_path_linear(const lv_anim_t * a);






int32_t lv_anim_path_ease_in(const lv_anim_t * a);






int32_t lv_anim_path_ease_out(const lv_anim_t * a);






int32_t lv_anim_path_ease_in_out(const lv_anim_t * a);






int32_t lv_anim_path_overshoot(const lv_anim_t * a);






int32_t lv_anim_path_bounce(const lv_anim_t * a);







int32_t lv_anim_path_step(const lv_anim_t * a);






int32_t lv_anim_path_custom_bezier3(const lv_anim_t * a);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_anim_timeline.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_anim_timeline.h"
typedef struct _lv_anim_timeline_t lv_anim_timeline_t;
# 38 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_anim_timeline.h"
lv_anim_timeline_t * lv_anim_timeline_create(void);





void lv_anim_timeline_delete(lv_anim_timeline_t * at);







void lv_anim_timeline_add(lv_anim_timeline_t * at, uint32_t start_time, const lv_anim_t * a);






uint32_t lv_anim_timeline_start(lv_anim_timeline_t * at);





void lv_anim_timeline_pause(lv_anim_timeline_t * at);






void lv_anim_timeline_set_reverse(lv_anim_timeline_t * at, bool reverse);






void lv_anim_timeline_set_progress(lv_anim_timeline_t * at, uint16_t progress);






uint32_t lv_anim_timeline_get_playtime(lv_anim_timeline_t * at);






bool lv_anim_timeline_get_reverse(lv_anim_timeline_t * at);






uint16_t lv_anim_timeline_get_progress(lv_anim_timeline_t * at);
# 36 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_profiler_builtin.h" 1
# 37 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_rb.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_rb.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_rb.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_assert.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_assert.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_assert.h" 2
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_rb.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/misc/lv_rb.h"
typedef enum {
    LV_RB_COLOR_RED,
    LV_RB_COLOR_BLACK
} lv_rb_color_t;

typedef struct lv_rb_node_t {
    struct lv_rb_node_t * parent;
    struct lv_rb_node_t * left;
    struct lv_rb_node_t * right;
    lv_rb_color_t color;
    void * data;
} lv_rb_node_t;

typedef int8_t lv_rb_compare_res_t;

typedef lv_rb_compare_res_t (*lv_rb_compare_t)(const void * a, const void * b);

typedef struct {
    lv_rb_node_t * root;
    lv_rb_compare_t compare;
    size_t size;
} lv_rb_t;





bool lv_rb_init(lv_rb_t * tree, lv_rb_compare_t compare, size_t node_size);
lv_rb_node_t * lv_rb_insert(lv_rb_t * tree, void * key);
lv_rb_node_t * lv_rb_find(lv_rb_t * tree, const void * key);
void * lv_rb_remove_node(lv_rb_t * tree, lv_rb_node_t * node);
void * lv_rb_remove(lv_rb_t * tree, const void * key);
bool lv_rb_drop_node(lv_rb_t * tree, lv_rb_node_t * node);
bool lv_rb_drop(lv_rb_t * tree, const void * key);
lv_rb_node_t * lv_rb_minimum(lv_rb_t * node);
lv_rb_node_t * lv_rb_maximum(lv_rb_t * node);
lv_rb_node_t * lv_rb_minimum_from(lv_rb_node_t * node);
lv_rb_node_t * lv_rb_maximum_from(lv_rb_node_t * node);
void lv_rb_destroy(lv_rb_t * tree);
# 38 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2



# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_symbol_def.h" 1
# 283 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_symbol_def.h"
enum {
    _LV_STR_SYMBOL_BULLET,
    _LV_STR_SYMBOL_AUDIO,
    _LV_STR_SYMBOL_VIDEO,
    _LV_STR_SYMBOL_LIST,
    _LV_STR_SYMBOL_OK,
    _LV_STR_SYMBOL_CLOSE,
    _LV_STR_SYMBOL_POWER,
    _LV_STR_SYMBOL_SETTINGS,
    _LV_STR_SYMBOL_HOME,
    _LV_STR_SYMBOL_DOWNLOAD,
    _LV_STR_SYMBOL_DRIVE,
    _LV_STR_SYMBOL_REFRESH,
    _LV_STR_SYMBOL_MUTE,
    _LV_STR_SYMBOL_VOLUME_MID,
    _LV_STR_SYMBOL_VOLUME_MAX,
    _LV_STR_SYMBOL_IMAGE,
    _LV_STR_SYMBOL_TINT,
    _LV_STR_SYMBOL_PREV,
    _LV_STR_SYMBOL_PLAY,
    _LV_STR_SYMBOL_PAUSE,
    _LV_STR_SYMBOL_STOP,
    _LV_STR_SYMBOL_NEXT,
    _LV_STR_SYMBOL_EJECT,
    _LV_STR_SYMBOL_LEFT,
    _LV_STR_SYMBOL_RIGHT,
    _LV_STR_SYMBOL_PLUS,
    _LV_STR_SYMBOL_MINUS,
    _LV_STR_SYMBOL_EYE_OPEN,
    _LV_STR_SYMBOL_EYE_CLOSE,
    _LV_STR_SYMBOL_WARNING,
    _LV_STR_SYMBOL_SHUFFLE,
    _LV_STR_SYMBOL_UP,
    _LV_STR_SYMBOL_DOWN,
    _LV_STR_SYMBOL_LOOP,
    _LV_STR_SYMBOL_DIRECTORY,
    _LV_STR_SYMBOL_UPLOAD,
    _LV_STR_SYMBOL_CALL,
    _LV_STR_SYMBOL_CUT,
    _LV_STR_SYMBOL_COPY,
    _LV_STR_SYMBOL_SAVE,
    _LV_STR_SYMBOL_BARS,
    _LV_STR_SYMBOL_ENVELOPE,
    _LV_STR_SYMBOL_CHARGE,
    _LV_STR_SYMBOL_PASTE,
    _LV_STR_SYMBOL_BELL,
    _LV_STR_SYMBOL_KEYBOARD,
    _LV_STR_SYMBOL_GPS,
    _LV_STR_SYMBOL_FILE,
    _LV_STR_SYMBOL_WIFI,
    _LV_STR_SYMBOL_BATTERY_FULL,
    _LV_STR_SYMBOL_BATTERY_3,
    _LV_STR_SYMBOL_BATTERY_2,
    _LV_STR_SYMBOL_BATTERY_1,
    _LV_STR_SYMBOL_BATTERY_EMPTY,
    _LV_STR_SYMBOL_USB,
    _LV_STR_SYMBOL_BLUETOOTH,
    _LV_STR_SYMBOL_TRASH,
    _LV_STR_SYMBOL_EDIT,
    _LV_STR_SYMBOL_BACKSPACE,
    _LV_STR_SYMBOL_SD_CARD,
    _LV_STR_SYMBOL_NEW_LINE,
    _LV_STR_SYMBOL_DUMMY,
};
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
typedef struct {
    int32_t x;
    int32_t y;
} lv_point_t;

typedef struct {
    lv_value_precise_t x;
    lv_value_precise_t y;
} lv_point_precise_t;


typedef struct {
    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;
} lv_area_t;



enum _lv_align_t {
    LV_ALIGN_DEFAULT = 0,
    LV_ALIGN_TOP_LEFT,
    LV_ALIGN_TOP_MID,
    LV_ALIGN_TOP_RIGHT,
    LV_ALIGN_BOTTOM_LEFT,
    LV_ALIGN_BOTTOM_MID,
    LV_ALIGN_BOTTOM_RIGHT,
    LV_ALIGN_LEFT_MID,
    LV_ALIGN_RIGHT_MID,
    LV_ALIGN_CENTER,

    LV_ALIGN_OUT_TOP_LEFT,
    LV_ALIGN_OUT_TOP_MID,
    LV_ALIGN_OUT_TOP_RIGHT,
    LV_ALIGN_OUT_BOTTOM_LEFT,
    LV_ALIGN_OUT_BOTTOM_MID,
    LV_ALIGN_OUT_BOTTOM_RIGHT,
    LV_ALIGN_OUT_LEFT_TOP,
    LV_ALIGN_OUT_LEFT_MID,
    LV_ALIGN_OUT_LEFT_BOTTOM,
    LV_ALIGN_OUT_RIGHT_TOP,
    LV_ALIGN_OUT_RIGHT_MID,
    LV_ALIGN_OUT_RIGHT_BOTTOM,
};




typedef uint8_t lv_align_t;


enum _lv_dir_t {
    LV_DIR_NONE = 0x00,
    LV_DIR_LEFT = (1 << 0),
    LV_DIR_RIGHT = (1 << 1),
    LV_DIR_TOP = (1 << 2),
    LV_DIR_BOTTOM = (1 << 3),
    LV_DIR_HOR = LV_DIR_LEFT | LV_DIR_RIGHT,
    LV_DIR_VER = LV_DIR_TOP | LV_DIR_BOTTOM,
    LV_DIR_ALL = LV_DIR_HOR | LV_DIR_VER,
};




typedef uint8_t lv_dir_t;


typedef struct {
    int32_t angle_prev;
    int32_t sinma;
    int32_t cosma;
} lv_area_transform_cache_t;
# 119 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
void lv_area_set(lv_area_t * area_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2);






inline static void lv_area_copy(lv_area_t * dest, const lv_area_t * src)
{
    dest->x1 = src->x1;
    dest->y1 = src->y1;
    dest->x2 = src->x2;
    dest->y2 = src->y2;
}






static inline int32_t lv_area_get_width(const lv_area_t * area_p)
{
    return (int32_t)(area_p->x2 - area_p->x1 + 1);
}






static inline int32_t lv_area_get_height(const lv_area_t * area_p)
{
    return (int32_t)(area_p->y2 - area_p->y1 + 1);
}






void lv_area_set_width(lv_area_t * area_p, int32_t w);






void lv_area_set_height(lv_area_t * area_p, int32_t h);







void _lv_area_set_pos(lv_area_t * area_p, int32_t x, int32_t y);






uint32_t lv_area_get_size(const lv_area_t * area_p);

void lv_area_increase(lv_area_t * area, int32_t w_extra, int32_t h_extra);

void lv_area_move(lv_area_t * area, int32_t x_ofs, int32_t y_ofs);
# 194 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
bool _lv_area_intersect(lv_area_t * res_p, const lv_area_t * a1_p, const lv_area_t * a2_p);
# 203 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
int8_t _lv_area_diff(lv_area_t res_p[], const lv_area_t * a1_p, const lv_area_t * a2_p);







void _lv_area_join(lv_area_t * a_res_p, const lv_area_t * a1_p, const lv_area_t * a2_p);
# 220 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
bool _lv_area_is_point_on(const lv_area_t * a_p, const lv_point_t * p_p, int32_t radius);







bool _lv_area_is_on(const lv_area_t * a1_p, const lv_area_t * a2_p);
# 237 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
bool _lv_area_is_in(const lv_area_t * ain_p, const lv_area_t * aholder_p, int32_t radius);
# 246 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
bool _lv_area_is_out(const lv_area_t * aout_p, const lv_area_t * aholder_p, int32_t radius);






bool _lv_area_is_equal(const lv_area_t * a, const lv_area_t * b);
# 263 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
void lv_area_align(const lv_area_t * base, lv_area_t * to_align, lv_align_t align, int32_t ofs_x, int32_t ofs_y);

void lv_point_transform(lv_point_t * p, int32_t angle, int32_t scale_x, int32_t scale_y, const lv_point_t * pivot,
                        bool zoom_first);

static inline lv_point_t lv_point_from_precise(const lv_point_precise_t * p)
{
    lv_point_t point = {
        (int32_t)p->x, (int32_t)p->y
    };

    return point;
}

static inline lv_point_precise_t lv_point_to_precise(const lv_point_t * p)
{
    lv_point_precise_t point = {
        (lv_value_precise_t)p->x, (lv_value_precise_t)p->y
    };

    return point;
}

static inline void lv_point_set(lv_point_t * p, int32_t x, int32_t y)
{
    p->x = x;
    p->y = y;
}

static inline void lv_point_precise_set(lv_point_precise_t * p, lv_value_precise_t x, lv_value_precise_t y)
{
    p->x = x;
    p->y = y;
}

static inline void lv_point_swap(lv_point_t * p1, lv_point_t * p2)
{
    lv_point_t tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

static inline void lv_point_precise_swap(lv_point_precise_t * p1, lv_point_precise_t * p2)
{
    lv_point_precise_t tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
# 337 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_area.h"
enum {ENUM_LV_SIZE_CONTENT = ((2001) | (1 << (29U)))};





enum {ENUM_LV_COORD_MAX = ((1 << (29U)) - 1)};
enum {ENUM_LV_COORD_MIN = (-((1 << (29U)) - 1))};







static inline int32_t lv_pct(int32_t x)
{
    return (x < 0 ? ((1000 - (x)) | (1 << (29U))) : ((x) | (1 << (29U))));
}

static inline int32_t lv_pct_to_px(int32_t v, int32_t base)
{
    if((((((v) & (3 << (29U))) == (1 << (29U))) && ((v) & ~(3 << (29U))) <= 2000))) {
        return ((((v) & ~(3 << (29U))) > 1000 ? 1000 - ((v) & ~(3 << (29U))) : ((v) & ~(3 << (29U)))) * base) / 100;
    }

    return v;
}
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 2




enum {ENUM_LV_COLOR_DEPTH = 16};
# 42 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
enum _lv_opa_t {
    LV_OPA_TRANSP = 0,
    LV_OPA_0 = 0,
    LV_OPA_10 = 25,
    LV_OPA_20 = 51,
    LV_OPA_30 = 76,
    LV_OPA_40 = 102,
    LV_OPA_50 = 127,
    LV_OPA_60 = 153,
    LV_OPA_70 = 178,
    LV_OPA_80 = 204,
    LV_OPA_90 = 229,
    LV_OPA_100 = 255,
    LV_OPA_COVER = 255,
};




typedef uint8_t lv_opa_t;
# 89 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} lv_color_t;

typedef struct {
    uint16_t blue : 5;
    uint16_t green : 6;
    uint16_t red : 5;
} lv_color16_t;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} lv_color32_t;

typedef struct {
    uint16_t h;
    uint8_t s;
    uint8_t v;
} lv_color_hsv_t;

enum _lv_color_format_t {
    LV_COLOR_FORMAT_UNKNOWN = 0,

    LV_COLOR_FORMAT_RAW = 0x01,
    LV_COLOR_FORMAT_RAW_ALPHA = 0x02,


    LV_COLOR_FORMAT_L8 = 0x06,
    LV_COLOR_FORMAT_I1 = 0x07,
    LV_COLOR_FORMAT_I2 = 0x08,
    LV_COLOR_FORMAT_I4 = 0x09,
    LV_COLOR_FORMAT_I8 = 0x0A,
    LV_COLOR_FORMAT_A8 = 0x0E,


    LV_COLOR_FORMAT_RGB565 = 0x12,
    LV_COLOR_FORMAT_RGB565A8 = 0x14 ,


    LV_COLOR_FORMAT_RGB888 = 0x0F,
    LV_COLOR_FORMAT_ARGB8888 = 0x10,
    LV_COLOR_FORMAT_XRGB8888 = 0x11,


    LV_COLOR_FORMAT_A1 = 0x0B,
    LV_COLOR_FORMAT_A2 = 0x0C,
    LV_COLOR_FORMAT_A4 = 0x0D,



    LV_COLOR_FORMAT_YUV_START = 0x20,
    LV_COLOR_FORMAT_I420 = LV_COLOR_FORMAT_YUV_START,
    LV_COLOR_FORMAT_I422 = 0x21,
    LV_COLOR_FORMAT_I444 = 0x22,
    LV_COLOR_FORMAT_I400 = 0x23,
    LV_COLOR_FORMAT_NV21 = 0x24,
    LV_COLOR_FORMAT_NV12 = 0x25,


    LV_COLOR_FORMAT_YUY2 = 0x26,
    LV_COLOR_FORMAT_UYVY = 0x27,

    LV_COLOR_FORMAT_YUV_END = LV_COLOR_FORMAT_UYVY,





    LV_COLOR_FORMAT_NATIVE = LV_COLOR_FORMAT_RGB565,
    LV_COLOR_FORMAT_NATIVE_WITH_ALPHA = LV_COLOR_FORMAT_RGB565A8,







};




typedef uint8_t lv_color_format_t;
# 205 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
uint8_t lv_color_format_get_bpp(lv_color_format_t cf);






static inline uint8_t lv_color_format_get_size(lv_color_format_t cf)
{
    return (lv_color_format_get_bpp(cf) + 7) >> 3;
}






bool lv_color_format_has_alpha(lv_color_format_t src_cf);







lv_color32_t lv_color_to_32(lv_color_t color, lv_opa_t opa);






static inline uint32_t lv_color_to_int(lv_color_t c)
{
    uint8_t * tmp = (uint8_t *) &c;
    return tmp[0] + (tmp[1] << 8) + (tmp[2] << 16);
}







static inline bool lv_color_eq(lv_color_t c1, lv_color_t c2)
{
    return lv_color_to_int(c1) == lv_color_to_int(c2);
}







static inline bool lv_color32_eq(lv_color32_t c1, lv_color32_t c2)
{
    return *((uint32_t *)&c1) == *((uint32_t *)&c2);
}






static inline lv_color_t lv_color_hex(uint32_t c)
{
    lv_color_t ret;
    ret.red = (c >> 16) & 0xff;
    ret.green = (c >> 8) & 0xff;
    ret.blue = (c >> 0) & 0xff;
    return ret;
}
# 286 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
static inline lv_color_t lv_color_make(uint8_t r, uint8_t g, uint8_t b)
{
    lv_color_t ret;
    ret.red = r;
    ret.green = g;
    ret.blue = b;
    return ret;
}
# 303 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
static inline lv_color32_t lv_color32_make(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    lv_color32_t ret;
    ret.red = r;
    ret.green = g;
    ret.blue = b;
    ret.alpha = a;
    return ret;
}






static inline lv_color_t lv_color_hex3(uint32_t c)
{
    return lv_color_make((uint8_t)(((c >> 4) & 0xF0) | ((c >> 8) & 0xF)), (uint8_t)((c & 0xF0) | ((c & 0xF0) >> 4)),
                         (uint8_t)((c & 0xF) | ((c & 0xF) << 4)));
}






uint16_t lv_color_to_u16(lv_color_t color);






uint32_t lv_color_to_u32(lv_color_t color);
# 347 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
                      static inline uint16_t lv_color_16_16_mix(uint16_t c1, uint16_t c2, uint8_t mix)
{
    if(mix == 255) return c1;
    if(mix == 0) return c2;

    uint16_t ret;


    mix = (uint32_t)((uint32_t)mix + 4) >> 3;


    uint32_t bg = (uint32_t)(c2 | ((uint32_t)c2 << 16)) & 0x7E0F81F;
    uint32_t fg = (uint32_t)(c1 | ((uint32_t)c1 << 16)) & 0x7E0F81F;
    uint32_t result = ((((fg - bg) * mix) >> 5) + bg) & 0x7E0F81F;
    ret = (uint16_t)(result >> 16) | result;

    return ret;
}







lv_color_t lv_color_lighten(lv_color_t c, lv_opa_t lvl);







lv_color_t lv_color_darken(lv_color_t c, lv_opa_t lvl);
# 389 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
lv_color_t lv_color_hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v);
# 398 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h"
lv_color_hsv_t lv_color_rgb_to_hsv(uint8_t r8, uint8_t g8, uint8_t b8);






lv_color_hsv_t lv_color_to_hsv(lv_color_t color);







static inline lv_color_t lv_color_white(void)
{
    return lv_color_make(0xff, 0xff, 0xff);
}





static inline lv_color_t lv_color_black(void)
{
    return lv_color_make(0x00, 0x00, 0x00);
}

static inline void lv_color_premultiply(lv_color32_t * c)
{
    c->red = (((int32_t)(c->red) * (c->alpha)) >> 8);
    c->green = (((int32_t)(c->green) * (c->alpha)) >> 8);
    c->blue = (((int32_t)(c->blue) * (c->alpha)) >> 8);
}

static inline void lv_color16_premultiply(lv_color16_t * c, lv_opa_t a)
{
    c->red = (((int32_t)(c->red) * (a)) >> 8);
    c->green = (((int32_t)(c->green) * (a)) >> 8);
    c->blue = (((int32_t)(c->blue) * (a)) >> 8);
}





# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_palette.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_palette.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_palette.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_palette.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_palette.h" 2








typedef enum {
    LV_PALETTE_RED,
    LV_PALETTE_PINK,
    LV_PALETTE_PURPLE,
    LV_PALETTE_DEEP_PURPLE,
    LV_PALETTE_INDIGO,
    LV_PALETTE_BLUE,
    LV_PALETTE_LIGHT_BLUE,
    LV_PALETTE_CYAN,
    LV_PALETTE_TEAL,
    LV_PALETTE_GREEN,
    LV_PALETTE_LIGHT_GREEN,
    LV_PALETTE_LIME,
    LV_PALETTE_YELLOW,
    LV_PALETTE_AMBER,
    LV_PALETTE_ORANGE,
    LV_PALETTE_DEEP_ORANGE,
    LV_PALETTE_BROWN,
    LV_PALETTE_BLUE_GREY,
    LV_PALETTE_GREY,
    _LV_PALETTE_LAST,
    LV_PALETTE_NONE = 0xff,
} lv_palette_t;







lv_color_t lv_palette_main(lv_palette_t p);
lv_color_t lv_palette_lighten(lv_palette_t p, uint8_t lvl);
lv_color_t lv_palette_darken(lv_palette_t p, uint8_t lvl);
# 446 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h" 2
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h"
struct _lv_color_filter_dsc_t;

typedef lv_color_t (*lv_color_filter_cb_t)(const struct _lv_color_filter_dsc_t *, lv_color_t, lv_opa_t);

typedef struct _lv_color_filter_dsc_t {
    lv_color_filter_cb_t filter_cb;
    void * user_data;
} lv_color_filter_dsc_t;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h"
                      static inline lv_color_t lv_color_mix(lv_color_t c1, lv_color_t c2, uint8_t mix)
{
    lv_color_t ret;

    ret.red = ((((uint16_t)c1.red * mix + c2.red * (255 - mix) + 0) * 0x8081U) >> 0x17);
    ret.green = ((((uint16_t)c1.green * mix + c2.green * (255 - mix) + 0) * 0x8081U) >> 0x17);
    ret.blue = ((((uint16_t)c1.blue * mix + c2.blue * (255 - mix) + 0) * 0x8081U) >> 0x17);
    return ret;
}
# 67 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h"
static inline lv_color32_t lv_color_mix32(lv_color32_t fg, lv_color32_t bg)
{
    if(fg.alpha >= 253) {
        fg.alpha = bg.alpha;
        return fg;
    }
    if(fg.alpha <= 2) {
        return bg;
    }
    bg.red = (uint32_t)((uint32_t)fg.red * fg.alpha + (uint32_t)bg.red * (255 - fg.alpha)) >> 8;
    bg.green = (uint32_t)((uint32_t)fg.green * fg.alpha + (uint32_t)bg.green * (255 - fg.alpha)) >> 8;
    bg.blue = (uint32_t)((uint32_t)fg.blue * fg.alpha + (uint32_t)bg.blue * (255 - fg.alpha)) >> 8;
    return bg;
}
# 89 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_color_op.h"
static inline uint8_t lv_color_brightness(lv_color_t c)
{
    uint16_t bright = (uint16_t)(3u * c.red + c.green + 4u * c.blue);
    return (uint8_t)(bright >> 3);
}

static inline void lv_color_filter_dsc_init(lv_color_filter_dsc_t * dsc, lv_color_filter_cb_t cb)
{
    dsc->filter_cb = cb;
}
# 447 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_color.h" 2

                         extern const lv_color_filter_dsc_t lv_color_filter_shade;
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h" 2
# 36 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
typedef enum _lv_image_flags_t {




    LV_IMAGE_FLAGS_PREMULTIPLIED = (1 << 0),





    LV_IMAGE_FLAGS_MODIFIABLE = (1 << 1),





    LV_IMAGE_FLAGS_VECTORS = (1 << 2),






    LV_IMAGE_FLAGS_COMPRESSED = (1 << 3),




    LV_IMAGE_FLAGS_ALLOCATED = (1 << 4),




    LV_IMAGE_FLAGS_USER1 = 0x1000,
    LV_IMAGE_FLAGS_USER2 = 0x2000,
    LV_IMAGE_FLAGS_USER3 = 0x4000,
    LV_IMAGE_FLAGS_USER4 = 0x8000,
    LV_IMAGE_FLAGS_USER5 = 0x0100,
    LV_IMAGE_FLAGS_USER6 = 0x0200,
    LV_IMAGE_FLAGS_USER7 = 0x0400,
    LV_IMAGE_FLAGS_USER8 = 0x0800,
} lv_image_flags_t;

typedef enum {
    LV_IMAGE_COMPRESS_NONE = 0,
    LV_IMAGE_COMPRESS_RLE,
    LV_IMAGE_COMPRESS_LZ4,
} lv_image_compress_t;
# 97 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
typedef struct {
    uint32_t magic: 8;
    uint32_t cf : 8;
    uint32_t flags: 16;

    uint32_t w: 16;
    uint32_t h: 16;
    uint32_t stride: 16;
    uint32_t reserved_2: 16;
} lv_image_header_t;


typedef struct {
    void * buf;
    uint32_t stride;
} lv_yuv_plane_t;

typedef union {
    lv_yuv_plane_t yuv;
    struct {
        lv_yuv_plane_t y;
        lv_yuv_plane_t u;
        lv_yuv_plane_t v;
    } planar;
    struct {
        lv_yuv_plane_t y;
        lv_yuv_plane_t uv;
    } semi_planar;
} lv_yuv_buf_t;
# 134 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
typedef struct {
    lv_image_header_t header;
    uint32_t data_size;
    const uint8_t * data;
} lv_image_dsc_t;
# 154 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
void lv_image_buf_set_palette(lv_image_dsc_t * dsc, uint8_t id, lv_color32_t c);





void lv_image_buf_free(lv_image_dsc_t * dsc);
# 172 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_buf.h"
void _lv_image_buf_get_transformed_area(lv_area_t * res, int32_t w, int32_t h, int32_t angle, uint16_t scale_x,
                                        uint16_t scale_y,
                                        const lv_point_t * pivot);

static inline void lv_image_header_init(lv_image_header_t * header, uint32_t w, uint32_t h, lv_color_format_t cf,
                                        uint32_t stride, lv_image_flags_t flags)
{
    do { if(!(header)) { do {}while(0); while(1); } } while(0);
    lv_memzero(header, sizeof(*header));
    header->w = w;
    header->h = h;
    header->cf = cf;
    header->stride = stride;
    header->flags = flags;
}
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h" 2







enum {ENUM_LV_STRIDE_AUTO = 0};





typedef struct {
    lv_image_header_t header;
    uint32_t data_size;
    void * data;
    void * unaligned_data;
} lv_draw_buf_t;
# 70 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
typedef void * (*lv_draw_buf_malloc_cb)(size_t size, lv_color_format_t color_format);

typedef void (*lv_draw_buf_free_cb)(void * draw_buf);

typedef void * (*lv_draw_buf_align_cb)(void * buf, lv_color_format_t color_format);

typedef void (*lv_draw_buf_invalidate_cache_cb)(void * buf, uint32_t stride, lv_color_format_t color_format,
                                                const lv_area_t * area);

typedef uint32_t (*lv_draw_buf_width_to_stride_cb)(uint32_t w, lv_color_format_t color_format);

typedef struct {
    lv_draw_buf_malloc_cb buf_malloc_cb;
    lv_draw_buf_free_cb buf_free_cb;
    lv_draw_buf_align_cb align_pointer_cb;
    lv_draw_buf_invalidate_cache_cb invalidate_cache_cb;
    lv_draw_buf_width_to_stride_cb width_to_stride_cb;
} lv_draw_buf_handlers_t;
# 96 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
void _lv_draw_buf_init_handlers(void);






lv_draw_buf_handlers_t * lv_draw_buf_get_handlers(void);







void * lv_draw_buf_align(void * buf, lv_color_format_t color_format);
# 120 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
void lv_draw_buf_invalidate_cache(void * buf, uint32_t stride, lv_color_format_t color_format, const lv_area_t * area);







uint32_t lv_draw_buf_width_to_stride(uint32_t w, lv_color_format_t color_format);






void lv_draw_buf_clear(lv_draw_buf_t * draw_buf, const lv_area_t * a);
# 146 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
void lv_draw_buf_copy(lv_draw_buf_t * dest, const lv_area_t * dest_area,
                      const lv_draw_buf_t * src, const lv_area_t * src_area);
# 162 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
lv_draw_buf_t * lv_draw_buf_create(uint32_t w, uint32_t h, lv_color_format_t cf, uint32_t stride);
# 175 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
lv_result_t lv_draw_buf_init(lv_draw_buf_t * draw_buf, uint32_t w, uint32_t h, lv_color_format_t cf, uint32_t stride,
                             void * data, uint32_t data_size);






lv_draw_buf_t * lv_draw_buf_dup(const lv_draw_buf_t * draw_buf);
# 194 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
lv_draw_buf_t * lv_draw_buf_reshape(lv_draw_buf_t * draw_buf, lv_color_format_t cf, uint32_t w, uint32_t h,
                                    uint32_t stride);





void lv_draw_buf_destroy(lv_draw_buf_t * buf);




void * lv_draw_buf_goto_xy(const lv_draw_buf_t * buf, uint32_t x, uint32_t y);




lv_draw_buf_t * lv_draw_buf_adjust_stride(const lv_draw_buf_t * src, uint32_t stride);
# 220 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_buf.h"
lv_result_t lv_draw_buf_premultiply(lv_draw_buf_t * draw_buf);

static inline bool lv_draw_buf_has_flag(lv_draw_buf_t * draw_buf, lv_image_flags_t flag)
{
    return draw_buf->header.flags & flag;
}






static inline void lv_draw_buf_from_image(lv_draw_buf_t * buf, const lv_image_dsc_t * img)
{
    lv_memcpy(buf, img, sizeof(lv_image_dsc_t));
    buf->unaligned_data = buf->data;
}

static inline void lv_draw_buf_to_image(const lv_draw_buf_t * buf, lv_image_dsc_t * img)
{
    lv_memcpy(img, buf, sizeof(lv_image_dsc_t));
}
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h" 1
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 24 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os_none.h" 1
# 25 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os_none.h"
typedef int lv_mutex_t;
typedef int lv_thread_t;
typedef int lv_thread_sync_t;
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h" 2
# 48 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h"
typedef enum {
    LV_THREAD_PRIO_LOWEST,
    LV_THREAD_PRIO_LOW,
    LV_THREAD_PRIO_MID,
    LV_THREAD_PRIO_HIGH,
    LV_THREAD_PRIO_HIGHEST,
} lv_thread_prio_t;
# 74 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/../../osal/lv_os.h"
lv_result_t lv_thread_init(lv_thread_t * thread, lv_thread_prio_t prio, void (*callback)(void *), size_t stack_size,
                           void * user_data);






lv_result_t lv_thread_delete(lv_thread_t * thread);






lv_result_t lv_mutex_init(lv_mutex_t * mutex);






lv_result_t lv_mutex_lock(lv_mutex_t * mutex);






lv_result_t lv_mutex_lock_isr(lv_mutex_t * mutex);






lv_result_t lv_mutex_unlock(lv_mutex_t * mutex);






lv_result_t lv_mutex_delete(lv_mutex_t * mutex);






lv_result_t lv_thread_sync_init(lv_thread_sync_t * sync);






lv_result_t lv_thread_sync_wait(lv_thread_sync_t * sync);






lv_result_t lv_thread_sync_signal(lv_thread_sync_t * sync);






lv_result_t lv_thread_sync_delete(lv_thread_sync_t * sync);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_private.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_private.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_private.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdlib.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_private.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_private.h"
struct _lv_cache_ops_t;
struct _lv_cache_t;
struct _lv_cache_class_t;
struct _lv_cache_entry_t;

typedef struct _lv_cache_ops_t lv_cache_ops_t;
typedef struct _lv_cache_t lv_cache_t;
typedef struct _lv_cache_class_t lv_cache_class_t;
typedef struct _lv_cache_entry_t lv_cache_entry_t;

typedef int8_t lv_cache_compare_res_t;
typedef bool (*lv_cache_create_cb_t)(void * node, void * user_data);
typedef void (*lv_cache_free_cb_t)(void * node, void * user_data);
typedef lv_cache_compare_res_t (*lv_cache_compare_cb_t)(const void * a, const void * b);

typedef void * (*lv_cache_alloc_cb_t)(void);
typedef bool (*lv_cache_init_cb_t)(lv_cache_t * cache);
typedef void (*lv_cache_destroy_cb_t)(lv_cache_t * cache, void * user_data);
typedef lv_cache_entry_t * (*lv_cache_get_cb_t)(lv_cache_t * cache, const void * key, void * user_data);
typedef lv_cache_entry_t * (*lv_cache_add_cb_t)(lv_cache_t * cache, const void * key, void * user_data);
typedef void (*lv_cache_remove_cb_t)(lv_cache_t * cache, lv_cache_entry_t * entry, void * user_data);
typedef void (*lv_cache_drop_cb_t)(lv_cache_t * cache, const void * key, void * user_data);
typedef void (*lv_cache_clear_cb_t)(lv_cache_t * cache, void * user_data);

struct _lv_cache_ops_t {
    lv_cache_compare_cb_t compare_cb;
    lv_cache_create_cb_t create_cb;
    lv_cache_free_cb_t free_cb;
};

struct _lv_cache_t {
    const lv_cache_class_t * clz;

    size_t node_size;

    size_t max_size;
    size_t size;

    lv_cache_ops_t ops;

    lv_mutex_t lock;
};

struct _lv_cache_class_t {
    lv_cache_alloc_cb_t alloc_cb;
    lv_cache_init_cb_t init_cb;
    lv_cache_destroy_cb_t destroy_cb;

    lv_cache_get_cb_t get_cb;
    lv_cache_add_cb_t add_cb;
    lv_cache_remove_cb_t remove_cb;
    lv_cache_drop_cb_t drop_cb;
    lv_cache_clear_cb_t drop_all_cb;
};





struct _lv_cache_slot_size_t;

typedef struct _lv_cache_slot_size_t lv_cache_slot_size_t;

struct _lv_cache_slot_size_t {
    size_t size;
};
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdlib.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache_entry.h"
uint32_t lv_cache_entry_get_size(const uint32_t node_size);
int32_t lv_cache_entry_get_ref(lv_cache_entry_t * entry);
uint32_t lv_cache_entry_get_node_size(lv_cache_entry_t * entry);
bool lv_cache_entry_is_invalid(lv_cache_entry_t * entry);
void * lv_cache_entry_get_data(lv_cache_entry_t * entry);
const lv_cache_t * lv_cache_entry_get_cache(const lv_cache_entry_t * entry);
lv_cache_entry_t * lv_cache_entry_get_entry(void * data, const uint32_t node_size);

lv_cache_entry_t * lv_cache_entry_alloc(const uint32_t node_size, const lv_cache_t * cache);
void lv_cache_entry_init(lv_cache_entry_t * entry, const lv_cache_t * cache, const uint32_t node_size);
void lv_cache_entry_delete(lv_cache_entry_t * entry);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 2

# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdlib.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/_lv_cache_lru_rb.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/_lv_cache_lru_rb.h"
                         extern const lv_cache_class_t lv_cache_class_lru_rb_count;
                         extern const lv_cache_class_t lv_cache_class_lru_rb_size;
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_image_cache.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_image_cache.h"
void lv_image_cache_drop(const void * src);
# 24 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../misc/cache/lv_cache.h"
lv_cache_t * lv_cache_create(const lv_cache_class_t * cache_class,
                             size_t node_size, size_t max_size,
                             lv_cache_ops_t ops);
void lv_cache_destroy(lv_cache_t * cache, void * user_data);

lv_cache_entry_t * lv_cache_acquire(lv_cache_t * cache, const void * key, void * user_data);
lv_cache_entry_t * lv_cache_acquire_or_create(lv_cache_t * cache, const void * key, void * user_data);
lv_cache_entry_t * lv_cache_add(lv_cache_t * cache, const void * key, void * user_data);
void lv_cache_release(lv_cache_t * cache, lv_cache_entry_t * entry, void * user_data);
void lv_cache_drop(lv_cache_t * cache, const void * key, void * user_data);
void lv_cache_drop_all(lv_cache_t * cache, void * user_data);

void lv_cache_set_max_size(lv_cache_t * cache, size_t max_size, void * user_data);
size_t lv_cache_get_max_size(lv_cache_t * cache, void * user_data);
size_t lv_cache_get_size(lv_cache_t * cache, void * user_data);
size_t lv_cache_get_free_size(lv_cache_t * cache, void * user_data);

void lv_cache_set_compare_cb(lv_cache_t * cache, lv_cache_compare_cb_t compare_cb, void * user_data);
void lv_cache_set_create_cb(lv_cache_t * cache, lv_cache_create_cb_t alloc_cb, void * user_data);
void lv_cache_set_free_cb(lv_cache_t * cache, lv_cache_free_cb_t free_cb, void * user_data);
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h" 2
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
typedef struct {
    const lv_font_t *
    resolved_font;
    uint16_t adv_w;
    uint16_t box_w;
    uint16_t box_h;
    int16_t ofs_x;
    int16_t ofs_y;
    uint8_t bpp: 4;
    uint8_t is_placeholder: 1;

    uint32_t glyph_index;
    lv_cache_entry_t * entry;
} lv_font_glyph_dsc_t;


enum _lv_font_subpx_t {
    LV_FONT_SUBPX_NONE,
    LV_FONT_SUBPX_HOR,
    LV_FONT_SUBPX_VER,
    LV_FONT_SUBPX_BOTH,
};




typedef uint8_t lv_font_subpx_t;



enum _lv_font_kerning_t {
    LV_FONT_KERNING_NORMAL,
    LV_FONT_KERNING_NONE,
};




typedef uint8_t lv_font_kerning_t;



struct _lv_font_t {

    bool (*get_glyph_dsc)(const lv_font_t *, lv_font_glyph_dsc_t *, uint32_t letter, uint32_t letter_next);


    const void * (*get_glyph_bitmap)(lv_font_glyph_dsc_t *, uint32_t, lv_draw_buf_t *);


    void (*release_glyph)(const lv_font_t *, lv_font_glyph_dsc_t *);


    int32_t line_height;
    int32_t base_line;
    uint8_t subpx : 2;
    uint8_t kerning : 1;

    int8_t underline_position;
    int8_t underline_thickness;

    const void * dsc;
    const lv_font_t * fallback;
    void * user_data;
};
# 121 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
const void * lv_font_get_glyph_bitmap(lv_font_glyph_dsc_t * g_dsc, uint32_t letter,
                                      lv_draw_buf_t * draw_buf);
# 133 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
bool lv_font_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t letter,
                           uint32_t letter_next);
# 143 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
uint16_t lv_font_get_glyph_width(const lv_font_t * font, uint32_t letter, uint32_t letter_next);






static inline int32_t lv_font_get_line_height(const lv_font_t * font)
{
    return font->line_height;
}






void lv_font_set_kerning(lv_font_t * font, lv_font_kerning_t kerning);
# 181 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
 extern const lv_font_t lv_font_montserrat_14;



 extern const lv_font_t lv_font_montserrat_16;
# 201 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
 extern const lv_font_t lv_font_montserrat_24;
# 281 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/lv_font.h"
static inline const lv_font_t * lv_font_default(void)
{
    return &lv_font_montserrat_14;
}
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2



# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdarg.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h" 2
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
enum _lv_text_flag_t {
    LV_TEXT_FLAG_NONE = 0x00,
    LV_TEXT_FLAG_EXPAND = 0x01,
    LV_TEXT_FLAG_FIT = 0x02,
};




typedef uint8_t lv_text_flag_t;



enum _lv_text_align_t {
    LV_TEXT_ALIGN_AUTO,
    LV_TEXT_ALIGN_LEFT,
    LV_TEXT_ALIGN_CENTER,
    LV_TEXT_ALIGN_RIGHT,
};




typedef uint8_t lv_text_align_t;
# 82 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
void lv_text_get_size(lv_point_t * size_res, const char * text, const lv_font_t * font, int32_t letter_space,
                      int32_t line_space, int32_t max_width, lv_text_flag_t flag);
# 98 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
uint32_t _lv_text_get_next_line(const char * txt, const lv_font_t * font, int32_t letter_space,
                                int32_t max_width, int32_t * used_width, lv_text_flag_t flag);
# 110 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
int32_t lv_text_get_width(const char * txt, uint32_t length, const lv_font_t * font, int32_t letter_space);







void _lv_text_ins(char * txt_buf, uint32_t pos, const char * ins_txt);
# 127 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
void _lv_text_cut(char * txt, uint32_t pos, uint32_t len);
# 136 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
char * _lv_text_set_text_vfmt(const char * fmt, va_list ap) ;
# 147 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
void _lv_text_encoded_letter_next_2(const char * txt, uint32_t * letter, uint32_t * letter_next, uint32_t * ofs);






static inline bool _lv_text_is_break_char(uint32_t letter)
{
    uint8_t i;
    bool ret = 0;


    for(i = 0; " ,.;:-_)]}"[i] != '\0'; i++) {
        if(letter == (uint32_t)" ,.;:-_)]}"[i]) {
            ret = 1;
            break;
        }
    }

    return ret;
}






static inline bool _lv_text_is_a_word(uint32_t letter)
{

    if(letter == 0) return 0;


    if(letter >= 0x4E00 && letter <= 0x9FFF) {
        return 1;
    }


    if(letter >= 0xFF01 && letter <= 0xFF5E) {
        return 1;
    }


    if(letter >= 0x3000 && letter <= 0x303F) {
        return 1;
    }


    if(letter >= 0x2E80 && letter <= 0x2EFF) {
        return 1;
    }


    if(letter >= 0x31C0 && letter <= 0x31EF) {
        return 1;
    }


    if(letter >= 0x3040 && letter <= 0x30FF) {
        return 1;
    }


    if(letter >= 0xFE10 && letter <= 0xFE1F) {
        return 1;
    }


    if(letter >= 0xFE30 && letter <= 0xFE4F) {
        return 1;
    }

    return 0;
}
# 230 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
static inline bool _lv_text_is_marker(uint32_t letter)
{
    if(letter < 0x20) return 1;


    if(letter == 0x061C) return 1;


    if(letter == 0x115F) return 1;

    if(letter == 0x1160) return 1;


    if(letter >= 0x180B && letter <= 0x180E) return 1;


    if(letter >= 0x200B && letter <= 0x200F) return 1;
    if(letter >= 0x2028 && letter <= 0x202F) return 1;
    if(letter >= 0x205F && letter <= 0x206F) return 1;


    if(letter == 0xFEFF) return 1;

    if(letter == 0xF8FF) return 1;

    return 0;
}
# 267 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
extern uint8_t (*_lv_text_encoded_size)(const char *);






extern uint32_t (*_lv_text_unicode_to_encoded)(uint32_t);






extern uint32_t (*_lv_text_encoded_conv_wc)(uint32_t c);
# 291 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
extern uint32_t (*_lv_text_encoded_next)(const char *, uint32_t *);
# 300 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
extern uint32_t (*_lv_text_encoded_prev)(const char *, uint32_t *);
# 309 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
extern uint32_t (*_lv_text_encoded_get_byte_id)(const char *, uint32_t);
# 318 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_text.h"
extern uint32_t (*_lv_text_encoded_get_char_id)(const char *, uint32_t);







extern uint32_t (*_lv_text_get_encoded_length)(const char *);
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h" 2
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h"
enum _lv_base_dir_t {
    LV_BASE_DIR_LTR = 0x00,
    LV_BASE_DIR_RTL = 0x01,
    LV_BASE_DIR_AUTO = 0x02,

    LV_BASE_DIR_NEUTRAL = 0x20,
    LV_BASE_DIR_WEAK = 0x21,
};




typedef uint8_t lv_base_dir_t;
# 132 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_bidi.h"
static inline void lv_bidi_calculate_align(lv_text_align_t * align, lv_base_dir_t * base_dir, const char * txt)
{
    ((void)txt);
    ((void)base_dir);
    if(*align == LV_TEXT_ALIGN_AUTO) * align = LV_TEXT_ALIGN_LEFT;
}
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/lv_layout.h" 1
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/lv_layout.h"
typedef void (*lv_layout_update_cb_t)(lv_obj_t *, void * user_data);
typedef struct {
    lv_layout_update_cb_t cb;
    void * user_data;
} lv_layout_dsc_t;

typedef enum {
    LV_LAYOUT_NONE = 0,


    LV_LAYOUT_FLEX,



    LV_LAYOUT_GRID,


    _LV_LAYOUT_LAST
} lv_layout_t;





void _lv_layout_init(void);

void _lv_layout_deinit(void);







uint32_t lv_layout_register(lv_layout_update_cb_t cb, void * user_data);





void _lv_layout_apply(lv_obj_t * obj);






# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/flex/lv_flex.h" 1
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/flex/lv_flex.h"
typedef enum {
    LV_FLEX_ALIGN_START,
    LV_FLEX_ALIGN_END,
    LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_SPACE_EVENLY,
    LV_FLEX_ALIGN_SPACE_AROUND,
    LV_FLEX_ALIGN_SPACE_BETWEEN,
} lv_flex_align_t;

typedef enum {
    LV_FLEX_FLOW_ROW = 0x00,
    LV_FLEX_FLOW_COLUMN = (1 << 0),
    LV_FLEX_FLOW_ROW_WRAP = LV_FLEX_FLOW_ROW | (1 << 2),
    LV_FLEX_FLOW_ROW_REVERSE = LV_FLEX_FLOW_ROW | (1 << 3),
    LV_FLEX_FLOW_ROW_WRAP_REVERSE = LV_FLEX_FLOW_ROW | (1 << 2) | (1 << 3),
    LV_FLEX_FLOW_COLUMN_WRAP = LV_FLEX_FLOW_COLUMN | (1 << 2),
    LV_FLEX_FLOW_COLUMN_REVERSE = LV_FLEX_FLOW_COLUMN | (1 << 3),
    LV_FLEX_FLOW_COLUMN_WRAP_REVERSE = LV_FLEX_FLOW_COLUMN | (1 << 2) | (1 << 3),
} lv_flex_flow_t;
# 67 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/flex/lv_flex.h"
void lv_flex_init(void);






void lv_obj_set_flex_flow(lv_obj_t * obj, lv_flex_flow_t flow);
# 83 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/flex/lv_flex.h"
void lv_obj_set_flex_align(lv_obj_t * obj, lv_flex_align_t main_place, lv_flex_align_t cross_place,
                           lv_flex_align_t track_cross_place);






void lv_obj_set_flex_grow(lv_obj_t * obj, uint8_t grow);
# 75 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/lv_layout.h" 2



# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/grid/lv_grid.h" 1
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/grid/lv_grid.h"
enum {ENUM_LV_GRID_CONTENT = (((1 << (29U)) - 1) - 101)};


enum {ENUM_LV_GRID_TEMPLATE_LAST = (((1 << (29U)) - 1))};







typedef enum {
    LV_GRID_ALIGN_START,
    LV_GRID_ALIGN_CENTER,
    LV_GRID_ALIGN_END,
    LV_GRID_ALIGN_STRETCH,
    LV_GRID_ALIGN_SPACE_EVENLY,
    LV_GRID_ALIGN_SPACE_AROUND,
    LV_GRID_ALIGN_SPACE_BETWEEN,
} lv_grid_align_t;
# 61 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/grid/lv_grid.h"
void lv_grid_init(void);

void lv_obj_set_grid_dsc_array(lv_obj_t * obj, const int32_t col_dsc[], const int32_t row_dsc[]);

void lv_obj_set_grid_align(lv_obj_t * obj, lv_grid_align_t column_align, lv_grid_align_t row_align);
# 77 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/grid/lv_grid.h"
void lv_obj_set_grid_cell(lv_obj_t * obj, lv_grid_align_t column_align, int32_t col_pos, int32_t col_span,
                          lv_grid_align_t row_align, int32_t row_pos, int32_t row_span);




static inline int32_t lv_grid_fr(uint8_t x)
{
    return (((1 << (29U)) - 1) - 100 + x);
}
# 79 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../layouts/lv_layout.h" 2
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2
# 52 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
enum {ENUM_LV_SCALE_NONE = 256};
# 82 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
enum _lv_blend_mode_t {
    LV_BLEND_MODE_NORMAL,
    LV_BLEND_MODE_ADDITIVE,
    LV_BLEND_MODE_SUBTRACTIVE,
    LV_BLEND_MODE_MULTIPLY,
};




typedef uint8_t lv_blend_mode_t;






enum _lv_text_decor_t {
    LV_TEXT_DECOR_NONE = 0x00,
    LV_TEXT_DECOR_UNDERLINE = 0x01,
    LV_TEXT_DECOR_STRIKETHROUGH = 0x02,
};




typedef uint8_t lv_text_decor_t;






enum _lv_border_side_t {
    LV_BORDER_SIDE_NONE = 0x00,
    LV_BORDER_SIDE_BOTTOM = 0x01,
    LV_BORDER_SIDE_TOP = 0x02,
    LV_BORDER_SIDE_LEFT = 0x04,
    LV_BORDER_SIDE_RIGHT = 0x08,
    LV_BORDER_SIDE_FULL = 0x0F,
    LV_BORDER_SIDE_INTERNAL = 0x10,
};




typedef uint8_t lv_border_side_t;





enum _lv_grad_dir_t {
    LV_GRAD_DIR_NONE,
    LV_GRAD_DIR_VER,
    LV_GRAD_DIR_HOR,
};




typedef uint8_t lv_grad_dir_t;





typedef struct {
    lv_color_t color;
    lv_opa_t opa;
    uint8_t frac;
} lv_gradient_stop_t;


typedef struct {
    lv_gradient_stop_t stops[2];
    uint8_t stops_count;
    lv_grad_dir_t dir : 3;

} lv_grad_dsc_t;




typedef union {
    int32_t num;
    const void * ptr;
    lv_color_t color;
} lv_style_value_t;






enum _lv_style_prop_t {
    LV_STYLE_PROP_INV = 0,


    LV_STYLE_WIDTH = 1,
    LV_STYLE_HEIGHT = 2,
    LV_STYLE_LENGTH = 3,

    LV_STYLE_MIN_WIDTH = 4,
    LV_STYLE_MAX_WIDTH = 5,
    LV_STYLE_MIN_HEIGHT = 6,
    LV_STYLE_MAX_HEIGHT = 7,

    LV_STYLE_X = 8,
    LV_STYLE_Y = 9,
    LV_STYLE_ALIGN = 10,

    LV_STYLE_RADIUS = 12,


    LV_STYLE_PAD_TOP = 16,
    LV_STYLE_PAD_BOTTOM = 17,
    LV_STYLE_PAD_LEFT = 18,
    LV_STYLE_PAD_RIGHT = 19,

    LV_STYLE_PAD_ROW = 20,
    LV_STYLE_PAD_COLUMN = 21,
    LV_STYLE_LAYOUT = 22,

    LV_STYLE_MARGIN_TOP = 24,
    LV_STYLE_MARGIN_BOTTOM = 25,
    LV_STYLE_MARGIN_LEFT = 26,
    LV_STYLE_MARGIN_RIGHT = 27,


    LV_STYLE_BG_COLOR = 28,
    LV_STYLE_BG_OPA = 29,

    LV_STYLE_BG_GRAD_DIR = 32,
    LV_STYLE_BG_MAIN_STOP = 33,
    LV_STYLE_BG_GRAD_STOP = 34,
    LV_STYLE_BG_GRAD_COLOR = 35,

    LV_STYLE_BG_MAIN_OPA = 36,
    LV_STYLE_BG_GRAD_OPA = 37,
    LV_STYLE_BG_GRAD = 38,
    LV_STYLE_BASE_DIR = 39,

    LV_STYLE_BG_IMAGE_SRC = 40,
    LV_STYLE_BG_IMAGE_OPA = 41,
    LV_STYLE_BG_IMAGE_RECOLOR = 42,
    LV_STYLE_BG_IMAGE_RECOLOR_OPA = 43,

    LV_STYLE_BG_IMAGE_TILED = 44,
    LV_STYLE_CLIP_CORNER = 45,


    LV_STYLE_BORDER_WIDTH = 48,
    LV_STYLE_BORDER_COLOR = 49,
    LV_STYLE_BORDER_OPA = 50,

    LV_STYLE_BORDER_SIDE = 52,
    LV_STYLE_BORDER_POST = 53,

    LV_STYLE_OUTLINE_WIDTH = 56,
    LV_STYLE_OUTLINE_COLOR = 57,
    LV_STYLE_OUTLINE_OPA = 58,
    LV_STYLE_OUTLINE_PAD = 59,


    LV_STYLE_SHADOW_WIDTH = 60,
    LV_STYLE_SHADOW_COLOR = 61,
    LV_STYLE_SHADOW_OPA = 62,

    LV_STYLE_SHADOW_OFFSET_X = 64,
    LV_STYLE_SHADOW_OFFSET_Y = 65,
    LV_STYLE_SHADOW_SPREAD = 66,

    LV_STYLE_IMAGE_OPA = 68,
    LV_STYLE_IMAGE_RECOLOR = 69,
    LV_STYLE_IMAGE_RECOLOR_OPA = 70,

    LV_STYLE_LINE_WIDTH = 72,
    LV_STYLE_LINE_DASH_WIDTH = 73,
    LV_STYLE_LINE_DASH_GAP = 74,
    LV_STYLE_LINE_ROUNDED = 75,
    LV_STYLE_LINE_COLOR = 76,
    LV_STYLE_LINE_OPA = 77,


    LV_STYLE_ARC_WIDTH = 80,
    LV_STYLE_ARC_ROUNDED = 81,
    LV_STYLE_ARC_COLOR = 82,
    LV_STYLE_ARC_OPA = 83,
    LV_STYLE_ARC_IMAGE_SRC = 84,

    LV_STYLE_TEXT_COLOR = 88,
    LV_STYLE_TEXT_OPA = 89,
    LV_STYLE_TEXT_FONT = 90,

    LV_STYLE_TEXT_LETTER_SPACE = 91,
    LV_STYLE_TEXT_LINE_SPACE = 92,
    LV_STYLE_TEXT_DECOR = 93,
    LV_STYLE_TEXT_ALIGN = 94,

    LV_STYLE_OPA = 95,
    LV_STYLE_OPA_LAYERED = 96,
    LV_STYLE_COLOR_FILTER_DSC = 97,
    LV_STYLE_COLOR_FILTER_OPA = 98,
    LV_STYLE_ANIM = 99,
    LV_STYLE_ANIM_DURATION = 100,
    LV_STYLE_TRANSITION = 102,
    LV_STYLE_BLEND_MODE = 103,
    LV_STYLE_TRANSFORM_WIDTH = 104,
    LV_STYLE_TRANSFORM_HEIGHT = 105,
    LV_STYLE_TRANSLATE_X = 106,
    LV_STYLE_TRANSLATE_Y = 107,
    LV_STYLE_TRANSFORM_SCALE_X = 108,
    LV_STYLE_TRANSFORM_SCALE_Y = 109,
    LV_STYLE_TRANSFORM_ROTATION = 110,
    LV_STYLE_TRANSFORM_PIVOT_X = 111,
    LV_STYLE_TRANSFORM_PIVOT_Y = 112,
    LV_STYLE_TRANSFORM_SKEW_X = 113,
    LV_STYLE_TRANSFORM_SKEW_Y = 114,


    LV_STYLE_FLEX_FLOW = 115,
    LV_STYLE_FLEX_MAIN_PLACE = 116,
    LV_STYLE_FLEX_CROSS_PLACE = 117,
    LV_STYLE_FLEX_TRACK_PLACE = 118,
    LV_STYLE_FLEX_GROW = 119,



    LV_STYLE_GRID_COLUMN_ALIGN = 120,
    LV_STYLE_GRID_ROW_ALIGN = 121,
    LV_STYLE_GRID_ROW_DSC_ARRAY = 122,
    LV_STYLE_GRID_COLUMN_DSC_ARRAY = 123,
    LV_STYLE_GRID_CELL_COLUMN_POS = 124,
    LV_STYLE_GRID_CELL_COLUMN_SPAN = 125,
    LV_STYLE_GRID_CELL_X_ALIGN = 126,
    LV_STYLE_GRID_CELL_ROW_POS = 127,
    LV_STYLE_GRID_CELL_ROW_SPAN = 128,
    LV_STYLE_GRID_CELL_Y_ALIGN = 129,


    _LV_STYLE_LAST_BUILT_IN_PROP = 130,

    _LV_STYLE_NUM_BUILT_IN_PROPS = _LV_STYLE_LAST_BUILT_IN_PROP + 1,

    LV_STYLE_PROP_ANY = 0xFF,
    _LV_STYLE_PROP_CONST = 0xFF
};




typedef uint8_t lv_style_prop_t;


enum _lv_style_res_t {
    LV_STYLE_RES_NOT_FOUND,
    LV_STYLE_RES_FOUND,
};




typedef uint8_t lv_style_res_t;





typedef struct {
    const lv_style_prop_t * props;
    void * user_data;
    lv_anim_path_cb_t path_xcb;
    uint32_t time;
    uint32_t delay;
} lv_style_transition_dsc_t;




typedef struct {
    const lv_style_prop_t * prop_ptr;
    lv_style_value_t value;
} lv_style_const_prop_t;




typedef struct {





    void * values_and_props;

    uint32_t has_group;
    uint8_t prop_cnt;
} lv_style_t;
# 393 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
void lv_style_init(lv_style_t * style);





void lv_style_reset(lv_style_t * style);






static inline bool lv_style_is_const(const lv_style_t * style)
{
    if(style->prop_cnt == 255) return 1;
    return 0;
}
# 425 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
lv_style_prop_t lv_style_register_prop(uint8_t flag);




lv_style_prop_t lv_style_get_num_custom_props(void);







bool lv_style_remove_prop(lv_style_t * style, lv_style_prop_t prop);
# 448 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
void lv_style_set_prop(lv_style_t * style, lv_style_prop_t prop, lv_style_value_t value);
# 459 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
lv_style_res_t lv_style_get_prop(const lv_style_t * style, lv_style_prop_t prop, lv_style_value_t * value);
# 474 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
void lv_style_transition_dsc_init(lv_style_transition_dsc_t * tr, const lv_style_prop_t props[],
                                  lv_anim_path_cb_t path_cb, uint32_t time, uint32_t delay, void * user_data);






lv_style_value_t lv_style_prop_get_default(lv_style_prop_t prop);
# 494 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
static inline lv_style_res_t lv_style_get_prop_inlined(const lv_style_t * style, lv_style_prop_t prop,
                                                       lv_style_value_t * value)
{
    if(lv_style_is_const(style)) {
        lv_style_const_prop_t * props = (lv_style_const_prop_t *)style->values_and_props;
        uint32_t i;
        for(i = 0; props[i].prop_ptr; i++) {
            if(*props[i].prop_ptr == prop) {
                *value = props[i].value;
                return LV_STYLE_RES_FOUND;
            }
        }
    }
    else {
        lv_style_prop_t * props = (lv_style_prop_t *)style->values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
        uint32_t i;
        for(i = 0; i < style->prop_cnt; i++) {
            if(props[i] == prop) {
                lv_style_value_t * values = (lv_style_value_t *)style->values_and_props;
                *value = values[i];
                return LV_STYLE_RES_FOUND;
            }
        }
    }
    return LV_STYLE_RES_NOT_FOUND;
}






bool lv_style_is_empty(const lv_style_t * style);







static inline uint32_t _lv_style_get_prop_group(lv_style_prop_t prop)
{
    uint32_t group = prop >> 2;
    if(group > 30) group = 31;
    return group;

}







uint8_t _lv_style_prop_lookup_flags(lv_style_prop_t prop);

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_style_gen.h" 1
# 12 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/lv_style_gen.h"
void lv_style_set_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_WIDTH;
void lv_style_set_min_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MIN_WIDTH;
void lv_style_set_max_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MAX_WIDTH;
void lv_style_set_height(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_HEIGHT;
void lv_style_set_min_height(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MIN_HEIGHT;
void lv_style_set_max_height(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MAX_HEIGHT;
void lv_style_set_length(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LENGTH;
void lv_style_set_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_X;
void lv_style_set_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_Y;
void lv_style_set_align(lv_style_t * style, lv_align_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ALIGN;
void lv_style_set_transform_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_WIDTH;
void lv_style_set_transform_height(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_HEIGHT;
void lv_style_set_translate_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSLATE_X;
void lv_style_set_translate_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSLATE_Y;
void lv_style_set_transform_scale_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_SCALE_X;
void lv_style_set_transform_scale_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_SCALE_Y;
void lv_style_set_transform_rotation(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_ROTATION;
void lv_style_set_transform_pivot_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_PIVOT_X;
void lv_style_set_transform_pivot_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_PIVOT_Y;
void lv_style_set_transform_skew_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_SKEW_X;
void lv_style_set_transform_skew_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSFORM_SKEW_Y;
void lv_style_set_pad_top(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_TOP;
void lv_style_set_pad_bottom(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_BOTTOM;
void lv_style_set_pad_left(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_LEFT;
void lv_style_set_pad_right(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_RIGHT;
void lv_style_set_pad_row(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_ROW;
void lv_style_set_pad_column(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_PAD_COLUMN;
void lv_style_set_margin_top(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MARGIN_TOP;
void lv_style_set_margin_bottom(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MARGIN_BOTTOM;
void lv_style_set_margin_left(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MARGIN_LEFT;
void lv_style_set_margin_right(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_MARGIN_RIGHT;
void lv_style_set_bg_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_COLOR;
void lv_style_set_bg_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_OPA;
void lv_style_set_bg_grad_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_GRAD_COLOR;
void lv_style_set_bg_grad_dir(lv_style_t * style, lv_grad_dir_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_GRAD_DIR;
void lv_style_set_bg_main_stop(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_MAIN_STOP;
void lv_style_set_bg_grad_stop(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_GRAD_STOP;
void lv_style_set_bg_main_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_MAIN_OPA;
void lv_style_set_bg_grad_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_GRAD_OPA;
void lv_style_set_bg_grad(lv_style_t * style, const lv_grad_dsc_t * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_GRAD;
void lv_style_set_bg_image_src(lv_style_t * style, const void * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_IMAGE_SRC;
void lv_style_set_bg_image_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_IMAGE_OPA;
void lv_style_set_bg_image_recolor(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_IMAGE_RECOLOR;
void lv_style_set_bg_image_recolor_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_IMAGE_RECOLOR_OPA;
void lv_style_set_bg_image_tiled(lv_style_t * style, bool value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BG_IMAGE_TILED;
void lv_style_set_border_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BORDER_COLOR;
void lv_style_set_border_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BORDER_OPA;
void lv_style_set_border_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BORDER_WIDTH;
void lv_style_set_border_side(lv_style_t * style, lv_border_side_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BORDER_SIDE;
void lv_style_set_border_post(lv_style_t * style, bool value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BORDER_POST;
void lv_style_set_outline_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OUTLINE_WIDTH;
void lv_style_set_outline_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OUTLINE_COLOR;
void lv_style_set_outline_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OUTLINE_OPA;
void lv_style_set_outline_pad(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OUTLINE_PAD;
void lv_style_set_shadow_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_WIDTH;
void lv_style_set_shadow_offset_x(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_OFFSET_X;
void lv_style_set_shadow_offset_y(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_OFFSET_Y;
void lv_style_set_shadow_spread(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_SPREAD;
void lv_style_set_shadow_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_COLOR;
void lv_style_set_shadow_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_SHADOW_OPA;
void lv_style_set_image_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_IMAGE_OPA;
void lv_style_set_image_recolor(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_IMAGE_RECOLOR;
void lv_style_set_image_recolor_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_IMAGE_RECOLOR_OPA;
void lv_style_set_line_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_WIDTH;
void lv_style_set_line_dash_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_DASH_WIDTH;
void lv_style_set_line_dash_gap(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_DASH_GAP;
void lv_style_set_line_rounded(lv_style_t * style, bool value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_ROUNDED;
void lv_style_set_line_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_COLOR;
void lv_style_set_line_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LINE_OPA;
void lv_style_set_arc_width(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ARC_WIDTH;
void lv_style_set_arc_rounded(lv_style_t * style, bool value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ARC_ROUNDED;
void lv_style_set_arc_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ARC_COLOR;
void lv_style_set_arc_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ARC_OPA;
void lv_style_set_arc_image_src(lv_style_t * style, const void * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ARC_IMAGE_SRC;
void lv_style_set_text_color(lv_style_t * style, lv_color_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_COLOR;
void lv_style_set_text_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_OPA;
void lv_style_set_text_font(lv_style_t * style, const lv_font_t * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_FONT;
void lv_style_set_text_letter_space(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_LETTER_SPACE;
void lv_style_set_text_line_space(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_LINE_SPACE;
void lv_style_set_text_decor(lv_style_t * style, lv_text_decor_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_DECOR;
void lv_style_set_text_align(lv_style_t * style, lv_text_align_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TEXT_ALIGN;
void lv_style_set_radius(lv_style_t * style, int32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_RADIUS;
void lv_style_set_clip_corner(lv_style_t * style, bool value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_CLIP_CORNER;
void lv_style_set_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OPA;
void lv_style_set_opa_layered(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_OPA_LAYERED;
void lv_style_set_color_filter_dsc(lv_style_t * style, const lv_color_filter_dsc_t * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_COLOR_FILTER_DSC;
void lv_style_set_color_filter_opa(lv_style_t * style, lv_opa_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_COLOR_FILTER_OPA;
void lv_style_set_anim(lv_style_t * style, const lv_anim_t * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ANIM;
void lv_style_set_anim_duration(lv_style_t * style, uint32_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_ANIM_DURATION;
void lv_style_set_transition(lv_style_t * style, const lv_style_transition_dsc_t * value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_TRANSITION;
void lv_style_set_blend_mode(lv_style_t * style, lv_blend_mode_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BLEND_MODE;
void lv_style_set_layout(lv_style_t * style, uint16_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_LAYOUT;
void lv_style_set_base_dir(lv_style_t * style, lv_base_dir_t value);
                         extern const lv_style_prop_t _lv_style_const_prop_id_BASE_DIR;


    void lv_style_set_flex_flow(lv_style_t * style, lv_flex_flow_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_FLEX_FLOW;
    void lv_style_set_flex_main_place(lv_style_t * style, lv_flex_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_FLEX_MAIN_PLACE;
    void lv_style_set_flex_cross_place(lv_style_t * style, lv_flex_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_FLEX_CROSS_PLACE;
    void lv_style_set_flex_track_place(lv_style_t * style, lv_flex_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_FLEX_TRACK_PLACE;
    void lv_style_set_flex_grow(lv_style_t * style, uint8_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_FLEX_GROW;




    void lv_style_set_grid_column_dsc_array(lv_style_t * style, const int32_t * value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_COLUMN_DSC_ARRAY;
    void lv_style_set_grid_column_align(lv_style_t * style, lv_grid_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_COLUMN_ALIGN;
    void lv_style_set_grid_row_dsc_array(lv_style_t * style, const int32_t * value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_ROW_DSC_ARRAY;
    void lv_style_set_grid_row_align(lv_style_t * style, lv_grid_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_ROW_ALIGN;
    void lv_style_set_grid_cell_column_pos(lv_style_t * style, int32_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_COLUMN_POS;
    void lv_style_set_grid_cell_x_align(lv_style_t * style, lv_grid_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_X_ALIGN;
    void lv_style_set_grid_cell_column_span(lv_style_t * style, int32_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_COLUMN_SPAN;
    void lv_style_set_grid_cell_row_pos(lv_style_t * style, int32_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_ROW_POS;
    void lv_style_set_grid_cell_y_align(lv_style_t * style, lv_grid_align_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_Y_ALIGN;
    void lv_style_set_grid_cell_row_span(lv_style_t * style, int32_t value);
                             extern const lv_style_prop_t _lv_style_const_prop_id_GRID_CELL_ROW_SPAN;
# 551 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h" 2

static inline void lv_style_set_size(lv_style_t * style, int32_t width, int32_t height)
{
    lv_style_set_width(style, width);
    lv_style_set_height(style, height);
}

static inline void lv_style_set_pad_all(lv_style_t * style, int32_t value)
{
    lv_style_set_pad_left(style, value);
    lv_style_set_pad_right(style, value);
    lv_style_set_pad_top(style, value);
    lv_style_set_pad_bottom(style, value);
}

static inline void lv_style_set_pad_hor(lv_style_t * style, int32_t value)
{
    lv_style_set_pad_left(style, value);
    lv_style_set_pad_right(style, value);
}

static inline void lv_style_set_pad_ver(lv_style_t * style, int32_t value)
{
    lv_style_set_pad_top(style, value);
    lv_style_set_pad_bottom(style, value);
}

static inline void lv_style_set_pad_gap(lv_style_t * style, int32_t value)
{
    lv_style_set_pad_row(style, value);
    lv_style_set_pad_column(style, value);
}

static inline void lv_style_set_transform_scale(lv_style_t * style, int32_t value)
{
    lv_style_set_transform_scale_x(style, value);
    lv_style_set_transform_scale_y(style, value);
}
# 600 "../../micropython/../lv_binding_micropython/lvgl/src/core/../misc/lv_style.h"
static inline bool lv_style_prop_has_flag(lv_style_prop_t prop, uint8_t flag)
{
    return _lv_style_prop_lookup_flags(prop) & flag;
}





                         extern const lv_style_prop_t lv_style_const_prop_id_inv;
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2




# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h" 2
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h"
typedef void (*lv_event_cb_t)(lv_event_t * e);

typedef struct {
    lv_event_cb_t cb;
    void * user_data;
    uint32_t filter;
} lv_event_dsc_t;




typedef enum {
    LV_EVENT_ALL = 0,


    LV_EVENT_PRESSED,
    LV_EVENT_PRESSING,
    LV_EVENT_PRESS_LOST,
    LV_EVENT_SHORT_CLICKED,
    LV_EVENT_LONG_PRESSED,
    LV_EVENT_LONG_PRESSED_REPEAT,
    LV_EVENT_CLICKED,
    LV_EVENT_RELEASED,
    LV_EVENT_SCROLL_BEGIN,
    LV_EVENT_SCROLL_THROW_BEGIN,
    LV_EVENT_SCROLL_END,
    LV_EVENT_SCROLL,
    LV_EVENT_GESTURE,
    LV_EVENT_KEY,
    LV_EVENT_FOCUSED,
    LV_EVENT_DEFOCUSED,
    LV_EVENT_LEAVE,
    LV_EVENT_HIT_TEST,
    LV_EVENT_INDEV_RESET,


    LV_EVENT_COVER_CHECK,
    LV_EVENT_REFR_EXT_DRAW_SIZE,
    LV_EVENT_DRAW_MAIN_BEGIN,
    LV_EVENT_DRAW_MAIN,
    LV_EVENT_DRAW_MAIN_END,
    LV_EVENT_DRAW_POST_BEGIN,
    LV_EVENT_DRAW_POST,
    LV_EVENT_DRAW_POST_END,
    LV_EVENT_DRAW_TASK_ADDED,


    LV_EVENT_VALUE_CHANGED,
    LV_EVENT_INSERT,
    LV_EVENT_REFRESH,
    LV_EVENT_READY,
    LV_EVENT_CANCEL,


    LV_EVENT_CREATE,
    LV_EVENT_DELETE,
    LV_EVENT_CHILD_CHANGED,
    LV_EVENT_CHILD_CREATED,
    LV_EVENT_CHILD_DELETED,
    LV_EVENT_SCREEN_UNLOAD_START,
    LV_EVENT_SCREEN_LOAD_START,
    LV_EVENT_SCREEN_LOADED,
    LV_EVENT_SCREEN_UNLOADED,
    LV_EVENT_SIZE_CHANGED,
    LV_EVENT_STYLE_CHANGED,
    LV_EVENT_LAYOUT_CHANGED,
    LV_EVENT_GET_SELF_SIZE,


    LV_EVENT_INVALIDATE_AREA,
    LV_EVENT_RESOLUTION_CHANGED,
    LV_EVENT_COLOR_FORMAT_CHANGED,
    LV_EVENT_REFR_REQUEST,
    LV_EVENT_REFR_START,
    LV_EVENT_REFR_READY,
    LV_EVENT_RENDER_START,
    LV_EVENT_RENDER_READY,
    LV_EVENT_FLUSH_START,
    LV_EVENT_FLUSH_FINISH,

    LV_EVENT_VSYNC,

    _LV_EVENT_LAST,

    LV_EVENT_PREPROCESS = 0x8000,

} lv_event_code_t;

typedef lv_array_t lv_event_list_t;

struct _lv_event_t {
    void * current_target;
    void * original_target;
    lv_event_code_t code;
    void * user_data;
    void * param;
    lv_event_t * prev;
    uint8_t deleted : 1;
    uint8_t stop_processing : 1;
    uint8_t stop_bubbling : 1;
};
# 143 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h"
void _lv_event_push(lv_event_t * e);

void _lv_event_pop(lv_event_t * e);

lv_result_t lv_event_send(lv_event_list_t * list, lv_event_t * e, bool preprocess);

void lv_event_add(lv_event_list_t * list, lv_event_cb_t cb, lv_event_code_t filter, void * user_data);

uint32_t lv_event_get_count(lv_event_list_t * list);

lv_event_dsc_t * lv_event_get_dsc(lv_event_list_t * list, uint32_t index);

lv_event_cb_t lv_event_dsc_get_cb(lv_event_dsc_t * dsc);

void * lv_event_dsc_get_user_data(lv_event_dsc_t * dsc);

bool lv_event_remove(lv_event_list_t * list, uint32_t index);

void lv_event_remove_all(lv_event_list_t * list);






void * lv_event_get_target(lv_event_t * e);







void * lv_event_get_current_target(lv_event_t * e);






lv_event_code_t lv_event_get_code(lv_event_t * e);






void * lv_event_get_param(lv_event_t * e);






void * lv_event_get_user_data(lv_event_t * e);






void lv_event_stop_bubbling(lv_event_t * e);






void lv_event_stop_processing(lv_event_t * e);
# 224 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../misc/lv_event.h"
uint32_t lv_event_register_id(void);






void _lv_event_mark_deleted(void * target);
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_profiler.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h" 2
# 36 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
enum _lv_fs_res_t {
    LV_FS_RES_OK = 0,
    LV_FS_RES_HW_ERR,
    LV_FS_RES_FS_ERR,
    LV_FS_RES_NOT_EX,
    LV_FS_RES_FULL,
    LV_FS_RES_LOCKED,
    LV_FS_RES_DENIED,
    LV_FS_RES_BUSY,
    LV_FS_RES_TOUT,
    LV_FS_RES_NOT_IMP,
    LV_FS_RES_OUT_OF_MEM,
    LV_FS_RES_INV_PARAM,
    LV_FS_RES_UNKNOWN,
};




typedef uint8_t lv_fs_res_t;





enum _lv_fs_mode_t {
    LV_FS_MODE_WR = 0x01,
    LV_FS_MODE_RD = 0x02,
};




typedef uint8_t lv_fs_mode_t;





typedef enum {
    LV_FS_SEEK_SET = 0x00,
    LV_FS_SEEK_CUR = 0x01,
    LV_FS_SEEK_END = 0x02,
} lv_fs_whence_t;

struct _lv_fs_drv_t;
typedef struct _lv_fs_drv_t lv_fs_drv_t;
struct _lv_fs_drv_t {
    char letter;
    uint32_t cache_size;
    bool (*ready_cb)(lv_fs_drv_t * drv);

    void * (*open_cb)(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
    lv_fs_res_t (*close_cb)(lv_fs_drv_t * drv, void * file_p);
    lv_fs_res_t (*read_cb)(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
    lv_fs_res_t (*write_cb)(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
    lv_fs_res_t (*seek_cb)(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
    lv_fs_res_t (*tell_cb)(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);

    void * (*dir_open_cb)(lv_fs_drv_t * drv, const char * path);
    lv_fs_res_t (*dir_read_cb)(lv_fs_drv_t * drv, void * rddir_p, char * fn, uint32_t fn_len);
    lv_fs_res_t (*dir_close_cb)(lv_fs_drv_t * drv, void * rddir_p);

    void * user_data;
};

typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t file_position;
    void * buffer;
} lv_fs_file_cache_t;

typedef struct {
    void * file_d;
    lv_fs_drv_t * drv;
    lv_fs_file_cache_t * cache;
} lv_fs_file_t;


typedef struct {
    char path[4];
    const void * buffer;
    uint32_t size;
} lv_fs_path_ex_t;

typedef struct {
    void * dir_d;
    lv_fs_drv_t * drv;
} lv_fs_dir_t;
# 134 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
void _lv_fs_init(void);




void _lv_fs_deinit(void);







void lv_fs_drv_init(lv_fs_drv_t * drv);







void lv_fs_drv_register(lv_fs_drv_t * drv);






lv_fs_drv_t * lv_fs_get_drv(char letter);







bool lv_fs_is_ready(char letter);
# 179 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
lv_fs_res_t lv_fs_open(lv_fs_file_t * file_p, const char * path, lv_fs_mode_t mode);
# 188 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
void lv_fs_make_path_from_buffer(lv_fs_path_ex_t * path, char letter, const void * buf, uint32_t size);






lv_fs_res_t lv_fs_close(lv_fs_file_t * file_p);
# 205 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
lv_fs_res_t lv_fs_read(lv_fs_file_t * file_p, void * buf, uint32_t btr, uint32_t * br);
# 215 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
lv_fs_res_t lv_fs_write(lv_fs_file_t * file_p, const void * buf, uint32_t btw, uint32_t * bw);
# 224 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
lv_fs_res_t lv_fs_seek(lv_fs_file_t * file_p, uint32_t pos, lv_fs_whence_t whence);







lv_fs_res_t lv_fs_tell(lv_fs_file_t * file_p, uint32_t * pos);







lv_fs_res_t lv_fs_dir_open(lv_fs_dir_t * rddir_p, const char * path);
# 250 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/../misc/lv_fs.h"
lv_fs_res_t lv_fs_dir_read(lv_fs_dir_t * rddir_p, char * fn, uint32_t fn_len);






lv_fs_res_t lv_fs_dir_close(lv_fs_dir_t * rddir_p);






char * lv_fs_get_letters(char * buf);






const char * lv_fs_get_ext(const char * fn);






char * lv_fs_up(char * path);






const char * lv_fs_get_last(const char * path);
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
enum _lv_image_src_t {
    LV_IMAGE_SRC_VARIABLE,
    LV_IMAGE_SRC_FILE,
    LV_IMAGE_SRC_SYMBOL,
    LV_IMAGE_SRC_UNKNOWN,
};




typedef uint8_t lv_image_src_t;



struct _lv_image_decoder_dsc_t;
typedef struct _lv_image_decoder_dsc_t lv_image_decoder_dsc_t;
# 61 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
typedef struct _lv_image_decoder_args_t {
    bool stride_align;
    bool premultiply;
    bool no_cache;
    bool use_indexed;
} lv_image_decoder_args_t;
# 75 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
typedef lv_result_t (*lv_image_decoder_info_f_t)(lv_image_decoder_t * decoder, const void * src,
                                                 lv_image_header_t * header);






typedef lv_result_t (*lv_image_decoder_open_f_t)(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);
# 96 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
typedef lv_result_t (*lv_image_decoder_get_area_cb_t)(lv_image_decoder_t * decoder,
                                                      lv_image_decoder_dsc_t * dsc,
                                                      const lv_area_t * full_area, lv_area_t * decoded_area);






typedef void (*lv_image_decoder_close_f_t)(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);

struct _lv_image_decoder_t {
    lv_image_decoder_info_f_t info_cb;
    lv_image_decoder_open_f_t open_cb;
    lv_image_decoder_get_area_cb_t get_area_cb;
    lv_image_decoder_close_f_t close_cb;

    lv_cache_free_cb_t cache_free_cb;
    void * user_data;
};

typedef struct _lv_image_decoder_cache_data_t {
    lv_cache_slot_size_t slot;

    const void * src;
    lv_image_src_t src_type;

    const lv_draw_buf_t * decoded;
    const lv_image_decoder_t * decoder;
    void * user_data;
} lv_image_cache_data_t;

typedef struct _lv_image_decoder_header_cache_data_t {
    const void * src;
    lv_image_src_t src_type;

    lv_image_header_t header;
    lv_image_decoder_t * decoder;
} lv_image_header_cache_data_t;


struct _lv_image_decoder_dsc_t {

    lv_image_decoder_t * decoder;


    lv_image_decoder_args_t args;


    const void * src;


    lv_image_src_t src_type;


    lv_image_header_t header;



    const lv_draw_buf_t * decoded;

    const lv_color32_t * palette;
    uint32_t palette_size;



    uint32_t time_to_open;



    const char * error_msg;

    lv_cache_t * cache;


    lv_cache_entry_t * cache_entry;


    void * user_data;
};
# 184 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
void _lv_image_decoder_init(void);




void _lv_image_decoder_deinit(void);
# 201 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
lv_result_t lv_image_decoder_get_info(const void * src, lv_image_header_t * header);
# 216 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
lv_result_t lv_image_decoder_open(lv_image_decoder_dsc_t * dsc, const void * src, const lv_image_decoder_args_t * args);
# 225 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
lv_result_t lv_image_decoder_get_area(lv_image_decoder_dsc_t * dsc, const lv_area_t * full_area,
                                      lv_area_t * decoded_area);





void lv_image_decoder_close(lv_image_decoder_dsc_t * dsc);





lv_image_decoder_t * lv_image_decoder_create(void);





void lv_image_decoder_delete(lv_image_decoder_t * decoder);






lv_image_decoder_t * lv_image_decoder_get_next(lv_image_decoder_t * decoder);






void lv_image_decoder_set_info_cb(lv_image_decoder_t * decoder, lv_image_decoder_info_f_t info_cb);






void lv_image_decoder_set_open_cb(lv_image_decoder_t * decoder, lv_image_decoder_open_f_t open_cb);






void lv_image_decoder_set_get_area_cb(lv_image_decoder_t * decoder, lv_image_decoder_get_area_cb_t read_line_cb);






void lv_image_decoder_set_close_cb(lv_image_decoder_t * decoder, lv_image_decoder_close_f_t close_cb);

void lv_image_decoder_set_cache_free_cb(lv_image_decoder_t * decoder, lv_cache_free_cb_t cache_free_cb);
# 296 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_image_decoder.h"
lv_draw_buf_t * lv_image_decoder_post_process(lv_image_decoder_dsc_t * dsc, lv_draw_buf_t * decoded);
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
typedef enum {
    LV_DRAW_TASK_TYPE_FILL,
    LV_DRAW_TASK_TYPE_BORDER,
    LV_DRAW_TASK_TYPE_BOX_SHADOW,
    LV_DRAW_TASK_TYPE_LABEL,
    LV_DRAW_TASK_TYPE_IMAGE,
    LV_DRAW_TASK_TYPE_LAYER,
    LV_DRAW_TASK_TYPE_LINE,
    LV_DRAW_TASK_TYPE_ARC,
    LV_DRAW_TASK_TYPE_TRIANGLE,
    LV_DRAW_TASK_TYPE_MASK_RECTANGLE,
    LV_DRAW_TASK_TYPE_MASK_BITMAP,
    LV_DRAW_TASK_TYPE_VECTOR,
} lv_draw_task_type_t;

typedef enum {
    LV_DRAW_TASK_STATE_WAITING,
    LV_DRAW_TASK_STATE_QUEUED,
    LV_DRAW_TASK_STATE_IN_PROGRESS,
    LV_DRAW_TASK_STATE_READY,
} lv_draw_task_state_t;

struct _lv_draw_task_t {
    lv_draw_task_t * next;

    lv_draw_task_type_t type;




    lv_area_t area;




    lv_area_t _real_area;


    lv_area_t clip_area_original;






    lv_area_t clip_area;

    volatile int state;

    void * draw_dsc;




    uint8_t preferred_draw_unit_id;







    uint8_t preference_score;

};

typedef struct {
    void * user_data;
} lv_draw_mask_t;

struct _lv_draw_unit_t {
    lv_draw_unit_t * next;




    lv_layer_t * target_layer;

    const lv_area_t * clip_area;
# 129 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
    int32_t (*dispatch_cb)(lv_draw_unit_t * draw_unit, lv_layer_t * layer);







    int32_t (*evaluate_cb)(lv_draw_unit_t * draw_unit, lv_draw_task_t * task);






    int32_t (*delete_cb)(lv_draw_unit_t * draw_unit);
};

struct _lv_layer_t {


    lv_draw_buf_t * draw_buf;


    lv_area_t buf_area;


    lv_color_format_t color_format;
# 166 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
    lv_area_t _clip_area;


    lv_draw_task_t * draw_task_head;

    lv_layer_t * parent;
    lv_layer_t * next;
    bool all_tasks_added;
    void * user_data;
};

typedef struct {
    lv_obj_t * obj;
    uint32_t part;
    uint32_t id1;
    uint32_t id2;
    lv_layer_t * layer;
    size_t dsc_size;
    void * user_data;
} lv_draw_dsc_base_t;

typedef struct {
    lv_draw_unit_t * unit_head;
    uint32_t used_memory_for_layers_kb;



    int dispatch_req;

    lv_mutex_t circle_cache_mutex;
    bool task_running;
} lv_draw_global_info_t;
# 206 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
void lv_draw_init(void);




void lv_draw_deinit(void);






void * lv_draw_create_unit(size_t size);
# 227 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
lv_draw_task_t * lv_draw_add_task(lv_layer_t * layer, const lv_area_t * coords);
# 236 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
void lv_draw_finalize_task_creation(lv_layer_t * layer, lv_draw_task_t * t);




void lv_draw_dispatch(void);







bool lv_draw_dispatch_layer(lv_display_t * disp, lv_layer_t * layer);





void lv_draw_dispatch_wait_for_request(void);





void lv_draw_dispatch_request(void);
# 270 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
lv_draw_task_t * lv_draw_get_next_available_task(lv_layer_t * layer, lv_draw_task_t * t_prev, uint8_t draw_unit_id);
# 280 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
uint32_t lv_draw_get_dependent_count(lv_draw_task_t * t_check);
# 289 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
lv_layer_t * lv_draw_layer_create(lv_layer_t * parent_layer, lv_color_format_t color_format, const lv_area_t * area);






void * lv_draw_layer_alloc_buf(lv_layer_t * layer);
# 305 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
void * lv_draw_layer_go_to_xy(lv_layer_t * layer, int32_t x, int32_t y);
# 318 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h" 2



# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/lv_draw_sw_gradient.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/lv_draw_sw_gradient.h"
typedef lv_color_t lv_grad_color_t;

typedef struct _lv_gradient_cache_t {
    lv_color_t * color_map;
    lv_opa_t * opa_map;
    uint32_t size;
} lv_grad_t;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/lv_draw_sw_gradient.h"
                      void lv_gradient_color_calculate(const lv_grad_dsc_t * dsc, int32_t range,
                                                       int32_t frac, lv_grad_color_t * color_out, lv_opa_t * opa_out);


lv_grad_t * lv_gradient_get(const lv_grad_dsc_t * gradient, int32_t w, int32_t h);





void lv_gradient_cleanup(lv_grad_t * grad);
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h" 2





enum {ENUM_LV_RADIUS_CIRCLE = 0x7FFF};





typedef struct {
    lv_draw_dsc_base_t base;

    int32_t radius;


    lv_opa_t bg_opa;
    lv_color_t bg_color;
    lv_grad_dsc_t bg_grad;


    const void * bg_image_src;
    const void * bg_image_symbol_font;
    lv_color_t bg_image_recolor;
    lv_opa_t bg_image_opa;
    lv_opa_t bg_image_recolor_opa;
    uint8_t bg_image_tiled;


    lv_color_t border_color;
    int32_t border_width;
    lv_opa_t border_opa;
    lv_border_side_t border_side : 5;
    uint8_t border_post : 1;


    lv_color_t outline_color;
    int32_t outline_width;
    int32_t outline_pad;
    lv_opa_t outline_opa;


    lv_color_t shadow_color;
    int32_t shadow_width;
    int32_t shadow_offset_x;
    int32_t shadow_offset_y;
    int32_t shadow_spread;
    lv_opa_t shadow_opa;
} lv_draw_rect_dsc_t;

typedef struct {
    lv_draw_dsc_base_t base;

    int32_t radius;

    lv_opa_t opa;
    lv_color_t color;
    lv_grad_dsc_t grad;
} lv_draw_fill_dsc_t;

typedef struct {
    lv_draw_dsc_base_t base;

    int32_t radius;

    lv_color_t color;
    int32_t width;
    lv_opa_t opa;
    lv_border_side_t side : 5;

} lv_draw_border_dsc_t;

typedef struct {
    lv_draw_dsc_base_t base;

    int32_t radius;

    lv_color_t color;
    int32_t width;
    int32_t spread;
    int32_t ofs_x;
    int32_t ofs_y;
    lv_opa_t opa;
    uint8_t bg_cover : 1;
} lv_draw_box_shadow_dsc_t;
# 116 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h"
                      void lv_draw_rect_dsc_init(lv_draw_rect_dsc_t * dsc);





void lv_draw_fill_dsc_init(lv_draw_fill_dsc_t * dsc);





void lv_draw_border_dsc_init(lv_draw_border_dsc_t * dsc);





void lv_draw_box_shadow_dsc_init(lv_draw_box_shadow_dsc_t * dsc);
# 143 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_rect.h"
void lv_draw_rect(lv_layer_t * layer, const lv_draw_rect_dsc_t * dsc, const lv_area_t * coords);
# 319 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h" 2
# 36 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
typedef struct _lv_draw_label_hint_t {

    int32_t line_start;


    int32_t y;



    int32_t coord_y;
} lv_draw_label_hint_t;

typedef struct {
    lv_draw_dsc_base_t base;

    const char * text;
    const lv_font_t * font;
    uint32_t sel_start;
    uint32_t sel_end;
    lv_color_t color;
    lv_color_t sel_color;
    lv_color_t sel_bg_color;
    int32_t line_space;
    int32_t letter_space;
    int32_t ofs_x;
    int32_t ofs_y;
    lv_opa_t opa;
    lv_base_dir_t bidi_dir;
    lv_text_align_t align;
    lv_text_flag_t flag;
    lv_text_decor_t decor : 3;
    lv_blend_mode_t blend_mode : 3;



    uint8_t text_local : 1;
    lv_draw_label_hint_t * hint;
} lv_draw_label_dsc_t;

typedef enum {
    LV_DRAW_LETTER_BITMAP_FORMAT_INVALID,
    LV_DRAW_LETTER_BITMAP_FORMAT_A8,
    LV_DRAW_LETTER_BITMAP_FORMAT_IMAGE,
    LV_DRAW_LETTER_VECTOR_FORMAT,
} lv_draw_glyph_bitmap_format_t;

typedef struct {
    void * glyph_data;
    lv_draw_glyph_bitmap_format_t format;
    const lv_area_t * letter_coords;
    const lv_area_t * bg_coords;
    const lv_font_glyph_dsc_t * g;
    lv_color_t color;
    lv_opa_t opa;
    lv_draw_buf_t * _draw_buf;
} lv_draw_glyph_dsc_t;
# 105 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
typedef void(*lv_draw_glyph_cb_t)(lv_draw_unit_t * draw_unit, lv_draw_glyph_dsc_t * dsc, lv_draw_fill_dsc_t * fill_dsc,
                                  const lv_area_t * fill_area);
# 116 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
                      void lv_draw_label_dsc_init(lv_draw_label_dsc_t * dsc);






void lv_draw_glyph_dsc_init(lv_draw_glyph_dsc_t * dsc);







                      void lv_draw_label(lv_layer_t * layer, const lv_draw_label_dsc_t * dsc,
                                         const lv_area_t * coords);
# 141 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
                      void lv_draw_character(lv_layer_t * layer, lv_draw_label_dsc_t * dsc,
                                             const lv_point_t * point, uint32_t unicode_letter);
# 152 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_label.h"
void lv_draw_label_iterate_characters(lv_draw_unit_t * draw_unit, const lv_draw_label_dsc_t * dsc,
                                      const lv_area_t * coords, lv_draw_glyph_cb_t cb);
# 320 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h" 2
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
typedef struct {
    lv_color_t alpha_color;
    const lv_color32_t * palette;
    uint32_t palette_size : 9;
} lv_draw_image_sup_t;

typedef struct _lv_draw_image_dsc_t {
    lv_draw_dsc_base_t base;

    const void * src;
    lv_image_header_t header;

    int32_t rotation;
    int32_t scale_x;
    int32_t scale_y;
    int32_t skew_x;
    int32_t skew_y;
    lv_point_t pivot;

    lv_color_t recolor;
    lv_opa_t recolor_opa;

    lv_opa_t opa;
    lv_blend_mode_t blend_mode : 4;

    uint16_t antialias : 1;
    uint16_t tile : 1;
    lv_draw_image_sup_t * sup;
} lv_draw_image_dsc_t;
# 72 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
typedef void (*lv_draw_image_core_cb)(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc,
                                      const lv_image_decoder_dsc_t * decoder_dsc, lv_draw_image_sup_t * sup,
                                      const lv_area_t * img_coords, const lv_area_t * clipped_img_area);
# 84 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
void lv_draw_image_dsc_init(lv_draw_image_dsc_t * dsc);







void lv_draw_image(lv_layer_t * layer, const lv_draw_image_dsc_t * dsc, const lv_area_t * coords);







void lv_draw_layer(lv_layer_t * layer, const lv_draw_image_dsc_t * dsc, const lv_area_t * coords);
# 110 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
lv_image_src_t lv_image_src_get_type(const void * src);
# 120 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
void _lv_draw_image_normal_helper(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc,
                                  const lv_area_t * coords, lv_draw_image_core_cb draw_core_cb);
# 131 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_image.h"
void _lv_draw_image_tiled_helper(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc,
                                 const lv_area_t * coords, lv_draw_image_core_cb draw_core_cb);
# 321 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_arc.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_arc.h"
typedef struct {
    lv_draw_dsc_base_t base;

    lv_color_t color;
    int32_t width;
    lv_value_precise_t start_angle;
    lv_value_precise_t end_angle;
    lv_point_t center;
    uint16_t radius;
    const void * img_src;
    lv_opa_t opa;
    uint8_t rounded : 1;
} lv_draw_arc_dsc_t;
# 51 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_arc.h"
void lv_draw_arc_dsc_init(lv_draw_arc_dsc_t * dsc);






void lv_draw_arc(lv_layer_t * layer, const lv_draw_arc_dsc_t * dsc);
# 71 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_arc.h"
void lv_draw_arc_get_area(int32_t x, int32_t y, uint16_t radius, lv_value_precise_t start_angle,
                          lv_value_precise_t end_angle,
                          int32_t w, bool rounded, lv_area_t * area);
# 322 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_line.h" 1
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_line.h"
typedef struct {
    lv_draw_dsc_base_t base;

    lv_point_precise_t p1;
    lv_point_precise_t p2;
    lv_color_t color;
    int32_t width;
    int32_t dash_width;
    int32_t dash_gap;
    lv_opa_t opa;
    lv_blend_mode_t blend_mode : 2;
    uint8_t round_start : 1;
    uint8_t round_end : 1;
    uint8_t raw_end : 1;
} lv_draw_line_dsc_t;
# 52 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_line.h"
void lv_draw_line_dsc_init(lv_draw_line_dsc_t * dsc);






void lv_draw_line(lv_layer_t * layer, const lv_draw_line_dsc_t * dsc);
# 323 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_triangle.h" 1
# 25 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_triangle.h"
typedef struct {
    lv_draw_dsc_base_t base;

    lv_opa_t bg_opa;
    lv_color_t bg_color;
    lv_grad_dsc_t bg_grad;

    lv_point_precise_t p[3];
} lv_draw_triangle_dsc_t;
# 43 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_triangle.h"
void lv_draw_triangle_dsc_init(lv_draw_triangle_dsc_t * draw_dsc);






void lv_draw_triangle(lv_layer_t * layer, const lv_draw_triangle_dsc_t * draw_dsc);
# 324 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_mask.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_mask.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_mask.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_mask.h"
typedef struct {
    lv_draw_dsc_base_t base;

    lv_area_t area;
    int32_t radius;
} lv_draw_mask_rect_dsc_t;
# 43 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/lv_draw_mask.h"
                      void lv_draw_mask_rect_dsc_init(lv_draw_mask_rect_dsc_t * dsc);






void lv_draw_mask_rect(lv_layer_t * layer, const lv_draw_mask_rect_dsc_t * dsc);
# 325 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/../draw/lv_draw.h" 2
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h" 2
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
typedef enum {
    LV_DISPLAY_ROTATION_0 = 0,
    LV_DISPLAY_ROTATION_90,
    LV_DISPLAY_ROTATION_180,
    LV_DISPLAY_ROTATION_270
} lv_display_rotation_t;

typedef enum {




    LV_DISPLAY_RENDER_MODE_PARTIAL,






    LV_DISPLAY_RENDER_MODE_DIRECT,





    LV_DISPLAY_RENDER_MODE_FULL,
} lv_display_render_mode_t;

typedef enum {
    LV_SCR_LOAD_ANIM_NONE,
    LV_SCR_LOAD_ANIM_OVER_LEFT,
    LV_SCR_LOAD_ANIM_OVER_RIGHT,
    LV_SCR_LOAD_ANIM_OVER_TOP,
    LV_SCR_LOAD_ANIM_OVER_BOTTOM,
    LV_SCR_LOAD_ANIM_MOVE_LEFT,
    LV_SCR_LOAD_ANIM_MOVE_RIGHT,
    LV_SCR_LOAD_ANIM_MOVE_TOP,
    LV_SCR_LOAD_ANIM_MOVE_BOTTOM,
    LV_SCR_LOAD_ANIM_FADE_IN,
    LV_SCR_LOAD_ANIM_FADE_ON = LV_SCR_LOAD_ANIM_FADE_IN,
    LV_SCR_LOAD_ANIM_FADE_OUT,
    LV_SCR_LOAD_ANIM_OUT_LEFT,
    LV_SCR_LOAD_ANIM_OUT_RIGHT,
    LV_SCR_LOAD_ANIM_OUT_TOP,
    LV_SCR_LOAD_ANIM_OUT_BOTTOM,
} lv_screen_load_anim_t;

typedef void (*lv_display_flush_cb_t)(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
typedef void (*lv_display_flush_wait_cb_t)(lv_display_t * disp);
# 94 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
lv_display_t * lv_display_create(int32_t hor_res, int32_t ver_res);





void lv_display_delete(lv_display_t * disp);





void lv_display_set_default(lv_display_t * disp);





lv_display_t * lv_display_get_default(void);






lv_display_t * lv_display_get_next(lv_display_t * disp);
# 133 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_resolution(lv_display_t * disp, int32_t hor_res, int32_t ver_res);
# 143 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_physical_resolution(lv_display_t * disp, int32_t hor_res, int32_t ver_res);
# 152 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_offset(lv_display_t * disp, int32_t x, int32_t y);






void lv_display_set_rotation(lv_display_t * disp, lv_display_rotation_t rotation);







void lv_display_set_dpi(lv_display_t * disp, int32_t dpi);






int32_t lv_display_get_horizontal_resolution(const lv_display_t * disp);






int32_t lv_display_get_vertical_resolution(const lv_display_t * disp);






int32_t lv_display_get_physical_horizontal_resolution(const lv_display_t * disp);






int32_t lv_display_get_physical_vertical_resolution(const lv_display_t * disp);






int32_t lv_display_get_offset_x(const lv_display_t * disp);






int32_t lv_display_get_offset_y(const lv_display_t * disp);






lv_display_rotation_t lv_display_get_rotation(lv_display_t * disp);






int32_t lv_display_get_dpi(const lv_display_t * disp);
# 237 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_buffers(lv_display_t * disp, void * buf1, void * buf2, uint32_t buf_size,
                            lv_display_render_mode_t render_mode);
# 248 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_draw_buffers(lv_display_t * disp, lv_draw_buf_t * buf1, lv_draw_buf_t * buf2);






void lv_display_set_render_mode(lv_display_t * disp, lv_display_render_mode_t render_mode);






void lv_display_set_flush_cb(lv_display_t * disp, lv_display_flush_cb_t flush_cb);
# 272 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_flush_wait_cb(lv_display_t * disp, lv_display_flush_wait_cb_t wait_cb);
# 285 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_set_color_format(lv_display_t * disp, lv_color_format_t color_format);






lv_color_format_t lv_display_get_color_format(lv_display_t * disp);






void lv_display_set_antialiasing(lv_display_t * disp, bool en);






bool lv_display_get_antialiasing(lv_display_t * disp);







                         void lv_display_flush_ready(lv_display_t * disp);
# 323 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
                         bool lv_display_flush_is_last(lv_display_t * disp);



bool lv_display_is_double_buffered(lv_display_t * disp);
# 339 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
lv_obj_t * lv_display_get_screen_active(lv_display_t * disp);







lv_obj_t * lv_display_get_screen_prev(lv_display_t * disp);






lv_obj_t * lv_display_get_layer_top(lv_display_t * disp);






lv_obj_t * lv_display_get_layer_sys(lv_display_t * disp);







lv_obj_t * lv_display_get_layer_bottom(lv_display_t * disp);





void lv_screen_load(struct _lv_obj_t * scr);
# 385 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_screen_load_anim(lv_obj_t * scr, lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay,
                         bool auto_del);





static inline lv_obj_t * lv_screen_active(void)
{
    return lv_display_get_screen_active(lv_display_get_default());
}





static inline lv_obj_t * lv_layer_top(void)
{
    return lv_display_get_layer_top(lv_display_get_default());
}





static inline lv_obj_t * lv_layer_sys(void)
{
    return lv_display_get_layer_sys(lv_display_get_default());
}





static inline lv_obj_t * lv_layer_bottom(void)
{
    return lv_display_get_layer_bottom(lv_display_get_default());
}
# 435 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
void lv_display_add_event_cb(lv_display_t * disp, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data);






uint32_t lv_display_get_event_count(lv_display_t * disp);







lv_event_dsc_t * lv_display_get_event_dsc(lv_display_t * disp, uint32_t index);







bool lv_display_delete_event(lv_display_t * disp, uint32_t index);
# 467 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
uint32_t lv_display_remove_event_cb_with_user_data(lv_display_t * disp, lv_event_cb_t event_cb, void * user_data);
# 476 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
lv_result_t lv_display_send_event(lv_display_t * disp, lv_event_code_t code, void * param);






void lv_display_set_theme(lv_display_t * disp, lv_theme_t * th);






lv_theme_t * lv_display_get_theme(lv_display_t * disp);






uint32_t lv_display_get_inactive_time(const lv_display_t * disp);





void lv_display_trigger_activity(lv_display_t * disp);






void lv_display_enable_invalidation(lv_display_t * disp, bool en);






bool lv_display_is_invalidation_enabled(lv_display_t * disp);







lv_timer_t * lv_display_get_refr_timer(lv_display_t * disp);





void lv_display_delete_refr_timer(lv_display_t * disp);

void lv_display_set_user_data(lv_display_t * disp, void * user_data);
void lv_display_set_driver_data(lv_display_t * disp, void * driver_data);
void * lv_display_get_user_data(lv_display_t * disp);
void * lv_display_get_driver_data(lv_display_t * disp);
# 578 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
static inline int32_t lv_dpx(int32_t n)
{
    return ((n) == 0 ? 0 :(((( (lv_display_get_dpi(0)) * (n) + 80) / 160)) > (1) ? ((( (lv_display_get_dpi(0)) * (n) + 80) / 160)) : (1)));
}
# 592 "../../micropython/../lv_binding_micropython/lvgl/src/core/../display/lv_display.h"
static inline int32_t lv_display_dpx(const lv_display_t * disp, int32_t n)
{
    return ((n) == 0 ? 0 :(((( (lv_display_get_dpi(disp)) * (n) + 80) / 160)) > (1) ? ((( (lv_display_get_dpi(disp)) * (n) + 80) / 160)) : (1)));
}
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
typedef enum {
    LV_OBJ_TREE_WALK_NEXT,
    LV_OBJ_TREE_WALK_SKIP_CHILDREN,
    LV_OBJ_TREE_WALK_END,
} lv_obj_tree_walk_res_t;

typedef lv_obj_tree_walk_res_t (*lv_obj_tree_walk_cb_t)(lv_obj_t *, void *);
# 48 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
void lv_obj_delete(lv_obj_t * obj);







void lv_obj_clean(lv_obj_t * obj);






void lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms);





void lv_obj_delete_anim_completed_cb(lv_anim_t * a);







void lv_obj_delete_async(lv_obj_t * obj);







void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);







void lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2);
# 103 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
void lv_obj_move_to_index(lv_obj_t * obj, int32_t index);






lv_obj_t * lv_obj_get_screen(const lv_obj_t * obj);






lv_display_t * lv_obj_get_display(const lv_obj_t * obj);






lv_obj_t * lv_obj_get_parent(const lv_obj_t * obj);
# 137 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
lv_obj_t * lv_obj_get_child(const lv_obj_t * obj, int32_t idx);
# 151 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
lv_obj_t * lv_obj_get_child_by_type(const lv_obj_t * obj, int32_t idx,
                                    const lv_obj_class_t * class_p);
# 165 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
lv_obj_t * lv_obj_get_sibling(const lv_obj_t * obj, int32_t idx);
# 179 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
lv_obj_t * lv_obj_get_sibling_by_type(const lv_obj_t * obj, int32_t idx,
                                      const lv_obj_class_t * class_p);






uint32_t lv_obj_get_child_count(const lv_obj_t * obj);
# 196 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
uint32_t lv_obj_get_child_count_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p);
# 205 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
int32_t lv_obj_get_index(const lv_obj_t * obj);
# 215 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_tree.h"
int32_t lv_obj_get_index_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p);







void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data);





void lv_obj_dump_tree(lv_obj_t * start_ob);
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h" 1
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_pos(lv_obj_t * obj, int32_t x, int32_t y);
# 51 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_x(lv_obj_t * obj, int32_t x);
# 62 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_y(lv_obj_t * obj, int32_t y);
# 75 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_size(lv_obj_t * obj, int32_t w, int32_t h);






bool lv_obj_refr_size(lv_obj_t * obj);
# 94 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_width(lv_obj_t * obj, int32_t w);
# 106 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_set_height(lv_obj_t * obj, int32_t h);






void lv_obj_set_content_width(lv_obj_t * obj, int32_t w);






void lv_obj_set_content_height(lv_obj_t * obj, int32_t h);






void lv_obj_set_layout(lv_obj_t * obj, uint32_t layout);






bool lv_obj_is_layout_positioned(const lv_obj_t * obj);





void lv_obj_mark_layout_as_dirty(lv_obj_t * obj);





void lv_obj_update_layout(const lv_obj_t * obj);






void lv_obj_set_align(lv_obj_t * obj, lv_align_t align);
# 165 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs);
# 176 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_align_to(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, int32_t x_ofs,
                     int32_t y_ofs);






static inline void lv_obj_center(lv_obj_t * obj)
{
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}






void lv_obj_get_coords(const lv_obj_t * obj, lv_area_t * coords);
# 206 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_x(const lv_obj_t * obj);
# 218 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_x2(const lv_obj_t * obj);
# 230 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_y(const lv_obj_t * obj);
# 242 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_y2(const lv_obj_t * obj);






int32_t lv_obj_get_x_aligned(const lv_obj_t * obj);






int32_t lv_obj_get_y_aligned(const lv_obj_t * obj);
# 265 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_width(const lv_obj_t * obj);
# 274 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_height(const lv_obj_t * obj);
# 283 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_content_width(const lv_obj_t * obj);
# 292 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_content_height(const lv_obj_t * obj);
# 301 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_get_content_coords(const lv_obj_t * obj, lv_area_t * area);
# 310 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_self_width(const lv_obj_t * obj);
# 319 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_obj_get_self_height(const lv_obj_t * obj);






bool lv_obj_refresh_self_size(lv_obj_t * obj);

void lv_obj_refr_pos(lv_obj_t * obj);

void lv_obj_move_to(lv_obj_t * obj, int32_t x, int32_t y);

void lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating);
# 341 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_transform_point(const lv_obj_t * obj, lv_point_t * p, bool recursive, bool inv);
# 350 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
void lv_obj_get_transformed_area(const lv_obj_t * obj, lv_area_t * area, bool recursive, bool inv);







void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area);





void lv_obj_invalidate(const lv_obj_t * obj);







bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area);






bool lv_obj_is_visible(const lv_obj_t * obj);






void lv_obj_set_ext_click_area(lv_obj_t * obj, int32_t size);







void lv_obj_get_click_area(const lv_obj_t * obj, lv_area_t * area);







bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point);
# 412 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_clamp_width(int32_t width, int32_t min_width, int32_t max_width, int32_t ref_width);
# 422 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_pos.h"
int32_t lv_clamp_height(int32_t height, int32_t min_height, int32_t max_height, int32_t ref_height);
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
enum _lv_scrollbar_mode_t {
    LV_SCROLLBAR_MODE_OFF,
    LV_SCROLLBAR_MODE_ON,
    LV_SCROLLBAR_MODE_ACTIVE,
    LV_SCROLLBAR_MODE_AUTO,
};




typedef uint8_t lv_scrollbar_mode_t;



enum _lv_scroll_snap_t {
    LV_SCROLL_SNAP_NONE,
    LV_SCROLL_SNAP_START,
    LV_SCROLL_SNAP_END,
    LV_SCROLL_SNAP_CENTER
};




typedef uint8_t lv_scroll_snap_t;
# 71 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode);






void lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir);






void lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align);






void lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align);
# 103 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(const lv_obj_t * obj);






lv_dir_t lv_obj_get_scroll_dir(const lv_obj_t * obj);






lv_scroll_snap_t lv_obj_get_scroll_snap_x(const lv_obj_t * obj);






lv_scroll_snap_t lv_obj_get_scroll_snap_y(const lv_obj_t * obj);
# 134 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_x(const lv_obj_t * obj);
# 144 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_y(const lv_obj_t * obj);
# 153 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_top(lv_obj_t * obj);
# 162 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_bottom(lv_obj_t * obj);
# 171 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_left(lv_obj_t * obj);
# 180 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
int32_t lv_obj_get_scroll_right(lv_obj_t * obj);







void lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end);
# 203 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_by(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en);
# 214 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en);
# 224 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en);
# 233 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en);
# 242 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en);






void lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en);
# 258 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
void lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en);
# 269 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_scroll.h"
lv_result_t _lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);






bool lv_obj_is_scrolling(const lv_obj_t * obj);






void lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en);







void lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor, lv_area_t * ver);





void lv_obj_scrollbar_invalidate(lv_obj_t * obj);






void lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en);
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h" 2
# 41 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
typedef enum {
    _LV_STYLE_STATE_CMP_SAME,
    _LV_STYLE_STATE_CMP_DIFF_REDRAW,
    _LV_STYLE_STATE_CMP_DIFF_DRAW_PAD,
    _LV_STYLE_STATE_CMP_DIFF_LAYOUT,
} _lv_style_state_cmp_t;

typedef uint32_t lv_style_selector_t;

typedef struct {
    const lv_style_t * style;
    uint32_t selector : 24;
    uint32_t is_local : 1;
    uint32_t is_trans : 1;
} _lv_obj_style_t;

typedef struct {
    uint16_t time;
    uint16_t delay;
    lv_style_selector_t selector;
    lv_style_prop_t prop;
    lv_anim_path_cb_t path_cb;
    void * user_data;
} _lv_obj_style_transition_dsc_t;
# 74 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void _lv_obj_style_init(void);





void _lv_obj_style_deinit(void);
# 90 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector);
# 101 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style,
                          lv_style_selector_t selector);
# 113 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector);





void lv_obj_remove_style_all(lv_obj_t * obj);






void lv_obj_report_style_change(lv_style_t * style);
# 136 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop);






void lv_obj_enable_style_refresh(bool en);
# 155 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
lv_style_value_t lv_obj_get_style_prop(const lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop);
# 164 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop);
# 173 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t value,
                                 lv_style_selector_t selector);

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t * value,
                                           lv_style_selector_t selector);
# 186 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector);




lv_style_value_t _lv_obj_style_apply_color_filter(const lv_obj_t * obj, uint32_t part, lv_style_value_t v);
# 201 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
void _lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state,
                                     lv_state_t new_state, const _lv_obj_style_transition_dsc_t * tr);
# 211 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h"
_lv_style_state_cmp_t _lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2);







void lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay);







void lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay);

static inline lv_state_t lv_obj_style_get_selector_state(lv_style_selector_t selector)
{
    return selector & 0xFFFF;
}

static inline lv_part_t lv_obj_style_get_selector_part(lv_style_selector_t selector)
{
    return selector & 0xFF0000;
}

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style_gen.h" 1
# 14 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style_gen.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h" 1
# 15 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style_gen.h" 2

static inline int32_t lv_obj_get_style_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_min_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MIN_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_max_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MAX_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_height(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_HEIGHT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_min_height(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MIN_HEIGHT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_max_height(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MAX_HEIGHT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_length(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LENGTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_Y);
    return (int32_t)v.num;
}

static inline lv_align_t lv_obj_get_style_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ALIGN);
    return (lv_align_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_height(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_HEIGHT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_translate_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSLATE_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_translate_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSLATE_Y);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_scale_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_SCALE_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_scale_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_SCALE_Y);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_rotation(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_ROTATION);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_pivot_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_PIVOT_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_pivot_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_PIVOT_Y);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_skew_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_SKEW_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_transform_skew_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSFORM_SKEW_Y);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_top(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_TOP);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_bottom(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_BOTTOM);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_left(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_LEFT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_right(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_RIGHT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_row(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_ROW);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_pad_column(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_PAD_COLUMN);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_margin_top(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MARGIN_TOP);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_margin_bottom(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MARGIN_BOTTOM);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_margin_left(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MARGIN_LEFT);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_margin_right(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_MARGIN_RIGHT);
    return (int32_t)v.num;
}

static inline lv_color_t lv_obj_get_style_bg_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_bg_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part, LV_STYLE_BG_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_bg_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_color_t lv_obj_get_style_bg_grad_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_GRAD_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_bg_grad_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_BG_GRAD_COLOR));
    return v.color;
}

static inline lv_grad_dir_t lv_obj_get_style_bg_grad_dir(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_GRAD_DIR);
    return (lv_grad_dir_t)v.num;
}

static inline int32_t lv_obj_get_style_bg_main_stop(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_MAIN_STOP);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_bg_grad_stop(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_GRAD_STOP);
    return (int32_t)v.num;
}

static inline lv_opa_t lv_obj_get_style_bg_main_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_MAIN_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_opa_t lv_obj_get_style_bg_grad_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_GRAD_OPA);
    return (lv_opa_t)v.num;
}

static inline const lv_grad_dsc_t * lv_obj_get_style_bg_grad(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_GRAD);
    return (const lv_grad_dsc_t *)v.ptr;
}

static inline const void * lv_obj_get_style_bg_image_src(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_IMAGE_SRC);
    return (const void *)v.ptr;
}

static inline lv_opa_t lv_obj_get_style_bg_image_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_IMAGE_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_color_t lv_obj_get_style_bg_image_recolor(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_IMAGE_RECOLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_bg_image_recolor_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_BG_IMAGE_RECOLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_bg_image_recolor_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_IMAGE_RECOLOR_OPA);
    return (lv_opa_t)v.num;
}

static inline bool lv_obj_get_style_bg_image_tiled(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BG_IMAGE_TILED);
    return (bool)v.num;
}

static inline lv_color_t lv_obj_get_style_border_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BORDER_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_border_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_BORDER_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_border_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BORDER_OPA);
    return (lv_opa_t)v.num;
}

static inline int32_t lv_obj_get_style_border_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BORDER_WIDTH);
    return (int32_t)v.num;
}

static inline lv_border_side_t lv_obj_get_style_border_side(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BORDER_SIDE);
    return (lv_border_side_t)v.num;
}

static inline bool lv_obj_get_style_border_post(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BORDER_POST);
    return (bool)v.num;
}

static inline int32_t lv_obj_get_style_outline_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OUTLINE_WIDTH);
    return (int32_t)v.num;
}

static inline lv_color_t lv_obj_get_style_outline_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OUTLINE_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_outline_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_OUTLINE_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_outline_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OUTLINE_OPA);
    return (lv_opa_t)v.num;
}

static inline int32_t lv_obj_get_style_outline_pad(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OUTLINE_PAD);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_shadow_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_shadow_offset_x(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_OFFSET_X);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_shadow_offset_y(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_OFFSET_Y);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_shadow_spread(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_SPREAD);
    return (int32_t)v.num;
}

static inline lv_color_t lv_obj_get_style_shadow_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_shadow_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_SHADOW_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_shadow_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_SHADOW_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_opa_t lv_obj_get_style_image_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_IMAGE_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_color_t lv_obj_get_style_image_recolor(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_IMAGE_RECOLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_image_recolor_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part,
                                                                                           LV_STYLE_IMAGE_RECOLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_image_recolor_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_IMAGE_RECOLOR_OPA);
    return (lv_opa_t)v.num;
}

static inline int32_t lv_obj_get_style_line_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_line_dash_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_DASH_WIDTH);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_line_dash_gap(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_DASH_GAP);
    return (int32_t)v.num;
}

static inline bool lv_obj_get_style_line_rounded(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_ROUNDED);
    return (bool)v.num;
}

static inline lv_color_t lv_obj_get_style_line_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_line_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_line_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LINE_OPA);
    return (lv_opa_t)v.num;
}

static inline int32_t lv_obj_get_style_arc_width(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_WIDTH);
    return (int32_t)v.num;
}

static inline bool lv_obj_get_style_arc_rounded(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_ROUNDED);
    return (bool)v.num;
}

static inline lv_color_t lv_obj_get_style_arc_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_arc_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_arc_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_OPA);
    return (lv_opa_t)v.num;
}

static inline const void * lv_obj_get_style_arc_image_src(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ARC_IMAGE_SRC);
    return (const void *)v.ptr;
}

static inline lv_color_t lv_obj_get_style_text_color(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_COLOR);
    return v.color;
}

static inline lv_color_t lv_obj_get_style_text_color_filtered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = _lv_obj_style_apply_color_filter(obj, part, lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_COLOR));
    return v.color;
}

static inline lv_opa_t lv_obj_get_style_text_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_OPA);
    return (lv_opa_t)v.num;
}

static inline const lv_font_t * lv_obj_get_style_text_font(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_FONT);
    return (const lv_font_t *)v.ptr;
}

static inline int32_t lv_obj_get_style_text_letter_space(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_LETTER_SPACE);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_text_line_space(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_LINE_SPACE);
    return (int32_t)v.num;
}

static inline lv_text_decor_t lv_obj_get_style_text_decor(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_DECOR);
    return (lv_text_decor_t)v.num;
}

static inline lv_text_align_t lv_obj_get_style_text_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TEXT_ALIGN);
    return (lv_text_align_t)v.num;
}

static inline int32_t lv_obj_get_style_radius(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_RADIUS);
    return (int32_t)v.num;
}

static inline bool lv_obj_get_style_clip_corner(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_CLIP_CORNER);
    return (bool)v.num;
}

static inline lv_opa_t lv_obj_get_style_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OPA);
    return (lv_opa_t)v.num;
}

static inline lv_opa_t lv_obj_get_style_opa_layered(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_OPA_LAYERED);
    return (lv_opa_t)v.num;
}

static inline const lv_color_filter_dsc_t * lv_obj_get_style_color_filter_dsc(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_COLOR_FILTER_DSC);
    return (const lv_color_filter_dsc_t *)v.ptr;
}

static inline lv_opa_t lv_obj_get_style_color_filter_opa(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_COLOR_FILTER_OPA);
    return (lv_opa_t)v.num;
}

static inline const lv_anim_t * lv_obj_get_style_anim(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ANIM);
    return (const lv_anim_t *)v.ptr;
}

static inline uint32_t lv_obj_get_style_anim_duration(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_ANIM_DURATION);
    return (uint32_t)v.num;
}

static inline const lv_style_transition_dsc_t * lv_obj_get_style_transition(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_TRANSITION);
    return (const lv_style_transition_dsc_t *)v.ptr;
}

static inline lv_blend_mode_t lv_obj_get_style_blend_mode(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BLEND_MODE);
    return (lv_blend_mode_t)v.num;
}

static inline uint16_t lv_obj_get_style_layout(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_LAYOUT);
    return (uint16_t)v.num;
}

static inline lv_base_dir_t lv_obj_get_style_base_dir(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_BASE_DIR);
    return (lv_base_dir_t)v.num;
}



static inline lv_flex_flow_t lv_obj_get_style_flex_flow(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_FLEX_FLOW);
    return (lv_flex_flow_t)v.num;
}

static inline lv_flex_align_t lv_obj_get_style_flex_main_place(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_FLEX_MAIN_PLACE);
    return (lv_flex_align_t)v.num;
}

static inline lv_flex_align_t lv_obj_get_style_flex_cross_place(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_FLEX_CROSS_PLACE);
    return (lv_flex_align_t)v.num;
}

static inline lv_flex_align_t lv_obj_get_style_flex_track_place(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_FLEX_TRACK_PLACE);
    return (lv_flex_align_t)v.num;
}

static inline uint8_t lv_obj_get_style_flex_grow(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_FLEX_GROW);
    return (uint8_t)v.num;
}





static inline const int32_t * lv_obj_get_style_grid_column_dsc_array(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_COLUMN_DSC_ARRAY);
    return (const int32_t *)v.ptr;
}

static inline lv_grid_align_t lv_obj_get_style_grid_column_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_COLUMN_ALIGN);
    return (lv_grid_align_t)v.num;
}

static inline const int32_t * lv_obj_get_style_grid_row_dsc_array(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_ROW_DSC_ARRAY);
    return (const int32_t *)v.ptr;
}

static inline lv_grid_align_t lv_obj_get_style_grid_row_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_ROW_ALIGN);
    return (lv_grid_align_t)v.num;
}

static inline int32_t lv_obj_get_style_grid_cell_column_pos(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_COLUMN_POS);
    return (int32_t)v.num;
}

static inline lv_grid_align_t lv_obj_get_style_grid_cell_x_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_X_ALIGN);
    return (lv_grid_align_t)v.num;
}

static inline int32_t lv_obj_get_style_grid_cell_column_span(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_COLUMN_SPAN);
    return (int32_t)v.num;
}

static inline int32_t lv_obj_get_style_grid_cell_row_pos(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_ROW_POS);
    return (int32_t)v.num;
}

static inline lv_grid_align_t lv_obj_get_style_grid_cell_y_align(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_Y_ALIGN);
    return (lv_grid_align_t)v.num;
}

static inline int32_t lv_obj_get_style_grid_cell_row_span(const lv_obj_t * obj, uint32_t part)
{
    lv_style_value_t v = lv_obj_get_style_prop(obj, part, LV_STYLE_GRID_CELL_ROW_SPAN);
    return (int32_t)v.num;
}



void lv_obj_set_style_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_min_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_max_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_min_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_max_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_length(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_align(lv_obj_t * obj, lv_align_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_height(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_translate_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_translate_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_scale_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_scale_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_rotation(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_pivot_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_pivot_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_skew_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transform_skew_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_row(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_pad_column(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_margin_top(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_margin_bottom(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_margin_left(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_margin_right(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_grad_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_grad_dir(lv_obj_t * obj, lv_grad_dir_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_main_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_grad_stop(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_main_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_grad_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_grad(lv_obj_t * obj, const lv_grad_dsc_t * value, lv_style_selector_t selector);
void lv_obj_set_style_bg_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector);
void lv_obj_set_style_bg_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_bg_image_tiled(lv_obj_t * obj, bool value, lv_style_selector_t selector);
void lv_obj_set_style_border_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_border_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_border_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_border_side(lv_obj_t * obj, lv_border_side_t value, lv_style_selector_t selector);
void lv_obj_set_style_border_post(lv_obj_t * obj, bool value, lv_style_selector_t selector);
void lv_obj_set_style_outline_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_outline_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_outline_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_outline_pad(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_offset_x(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_offset_y(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_spread(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_shadow_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_image_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_image_recolor(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_image_recolor_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_line_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_line_dash_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_line_dash_gap(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_line_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector);
void lv_obj_set_style_line_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_line_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_arc_width(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_arc_rounded(lv_obj_t * obj, bool value, lv_style_selector_t selector);
void lv_obj_set_style_arc_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_arc_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_arc_image_src(lv_obj_t * obj, const void * value, lv_style_selector_t selector);
void lv_obj_set_style_text_color(lv_obj_t * obj, lv_color_t value, lv_style_selector_t selector);
void lv_obj_set_style_text_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_text_font(lv_obj_t * obj, const lv_font_t * value, lv_style_selector_t selector);
void lv_obj_set_style_text_letter_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_text_line_space(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_text_decor(lv_obj_t * obj, lv_text_decor_t value, lv_style_selector_t selector);
void lv_obj_set_style_text_align(lv_obj_t * obj, lv_text_align_t value, lv_style_selector_t selector);
void lv_obj_set_style_radius(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
void lv_obj_set_style_clip_corner(lv_obj_t * obj, bool value, lv_style_selector_t selector);
void lv_obj_set_style_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_opa_layered(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_color_filter_dsc(lv_obj_t * obj, const lv_color_filter_dsc_t * value,
                                       lv_style_selector_t selector);
void lv_obj_set_style_color_filter_opa(lv_obj_t * obj, lv_opa_t value, lv_style_selector_t selector);
void lv_obj_set_style_anim(lv_obj_t * obj, const lv_anim_t * value, lv_style_selector_t selector);
void lv_obj_set_style_anim_duration(lv_obj_t * obj, uint32_t value, lv_style_selector_t selector);
void lv_obj_set_style_transition(lv_obj_t * obj, const lv_style_transition_dsc_t * value, lv_style_selector_t selector);
void lv_obj_set_style_blend_mode(lv_obj_t * obj, lv_blend_mode_t value, lv_style_selector_t selector);
void lv_obj_set_style_layout(lv_obj_t * obj, uint16_t value, lv_style_selector_t selector);
void lv_obj_set_style_base_dir(lv_obj_t * obj, lv_base_dir_t value, lv_style_selector_t selector);


    void lv_obj_set_style_flex_flow(lv_obj_t * obj, lv_flex_flow_t value, lv_style_selector_t selector);
    void lv_obj_set_style_flex_main_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_flex_cross_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_flex_track_place(lv_obj_t * obj, lv_flex_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_flex_grow(lv_obj_t * obj, uint8_t value, lv_style_selector_t selector);




    void lv_obj_set_style_grid_column_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_column_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_row_dsc_array(lv_obj_t * obj, const int32_t * value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_row_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_column_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_x_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_column_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_row_pos(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_y_align(lv_obj_t * obj, lv_grid_align_t value, lv_style_selector_t selector);
    void lv_obj_set_style_grid_cell_row_span(lv_obj_t * obj, int32_t value, lv_style_selector_t selector);
# 240 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_style.h" 2

static inline void lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_pad_left(obj, value, selector);
    lv_obj_set_style_pad_right(obj, value, selector);
    lv_obj_set_style_pad_top(obj, value, selector);
    lv_obj_set_style_pad_bottom(obj, value, selector);
}

static inline void lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_pad_left(obj, value, selector);
    lv_obj_set_style_pad_right(obj, value, selector);
}

static inline void lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_pad_top(obj, value, selector);
    lv_obj_set_style_pad_bottom(obj, value, selector);
}

static inline void lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_margin_left(obj, value, selector);
    lv_obj_set_style_margin_right(obj, value, selector);
    lv_obj_set_style_margin_top(obj, value, selector);
    lv_obj_set_style_margin_bottom(obj, value, selector);
}

static inline void lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_margin_left(obj, value, selector);
    lv_obj_set_style_margin_right(obj, value, selector);
}

static inline void lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_margin_top(obj, value, selector);
    lv_obj_set_style_margin_bottom(obj, value, selector);
}

static inline void lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    lv_obj_set_style_pad_row(obj, value, selector);
    lv_obj_set_style_pad_column(obj, value, selector);
}

static inline void lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height,
                                         lv_style_selector_t selector)
{
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

static inline void lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t value,
                                                    lv_style_selector_t selector)
{
    lv_obj_set_style_transform_scale_x(obj, value, selector);
    lv_obj_set_style_transform_scale_y(obj, value, selector);
}

static inline int32_t lv_obj_get_style_space_left(const lv_obj_t * obj, uint32_t part)
{
    int32_t padding = lv_obj_get_style_pad_left(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    return (border_side & LV_BORDER_SIDE_LEFT) ? padding + border_width : padding;
}

static inline int32_t lv_obj_get_style_space_right(const lv_obj_t * obj, uint32_t part)
{
    int32_t padding = lv_obj_get_style_pad_right(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    return (border_side & LV_BORDER_SIDE_RIGHT) ? padding + border_width : padding;
}

static inline int32_t lv_obj_get_style_space_top(const lv_obj_t * obj, uint32_t part)
{
    int32_t padding = lv_obj_get_style_pad_top(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    return (border_side & LV_BORDER_SIDE_TOP) ? padding + border_width : padding;
}

static inline int32_t lv_obj_get_style_space_bottom(const lv_obj_t * obj, uint32_t part)
{
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    return (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt);

static inline int32_t lv_obj_get_style_transform_scale_x_safe(const lv_obj_t * obj, uint32_t part)
{
    int16_t zoom = lv_obj_get_style_transform_scale_x(obj, part);
    return zoom != 0 ? zoom : 1;
}

static inline int32_t lv_obj_get_style_transform_scale_y_safe(const lv_obj_t * obj, uint32_t part)
{
    int16_t zoom = lv_obj_get_style_transform_scale_y(obj, part);
    return zoom != 0 ? zoom : 1;
}







lv_opa_t lv_obj_get_style_opa_recursive(const lv_obj_t * obj, lv_part_t part);
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h" 1
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
typedef enum {
    LV_LAYER_TYPE_NONE,
    LV_LAYER_TYPE_SIMPLE,
    LV_LAYER_TYPE_TRANSFORM,
} lv_layer_type_t;
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint32_t part, lv_draw_rect_dsc_t * draw_dsc);
# 57 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint32_t part, lv_draw_label_dsc_t * draw_dsc);
# 66 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
void lv_obj_init_draw_image_dsc(lv_obj_t * obj, uint32_t part, lv_draw_image_dsc_t * draw_dsc);
# 75 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint32_t part, lv_draw_line_dsc_t * draw_dsc);
# 84 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_draw.h"
void lv_obj_init_draw_arc_dsc(lv_obj_t * obj, uint32_t part, lv_draw_arc_dsc_t * draw_dsc);







int32_t lv_obj_calculate_ext_draw_size(lv_obj_t * obj, uint32_t part);






void lv_obj_refresh_ext_draw_size(lv_obj_t * obj);






int32_t _lv_obj_get_ext_draw_size(const lv_obj_t * obj);

lv_layer_type_t _lv_obj_get_layer_type(const lv_obj_t * obj);
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_property.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h"
typedef enum {
    LV_OBJ_CLASS_EDITABLE_INHERIT,
    LV_OBJ_CLASS_EDITABLE_TRUE,
    LV_OBJ_CLASS_EDITABLE_FALSE,
} lv_obj_class_editable_t;

typedef enum {
    LV_OBJ_CLASS_GROUP_DEF_INHERIT,
    LV_OBJ_CLASS_GROUP_DEF_TRUE,
    LV_OBJ_CLASS_GROUP_DEF_FALSE,
} lv_obj_class_group_def_t;

typedef enum {
    LV_OBJ_CLASS_THEME_INHERITABLE_FALSE,
    LV_OBJ_CLASS_THEME_INHERITABLE_TRUE,
} lv_obj_class_theme_inheritable_t;

typedef void (*lv_obj_class_event_cb_t)(lv_obj_class_t * class_p, lv_event_t * e);




struct _lv_obj_class_t {
    const lv_obj_class_t * base_class;

    void (*constructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);
    void (*destructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);


    void (*event_cb)(const lv_obj_class_t * class_p, lv_event_t * e);
# 68 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h"
    void * user_data;
    const char * name;
    int32_t width_def;
    int32_t height_def;
    uint32_t editable : 2;
    uint32_t group_def : 2;
    uint32_t instance_size : 16;
    uint32_t theme_inheritable : 1;
};
# 88 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_class.h"
lv_obj_t * lv_obj_class_create_obj(const lv_obj_class_t * class_p, lv_obj_t * parent);

void lv_obj_class_init_obj(lv_obj_t * obj);

void _lv_obj_destruct(lv_obj_t * obj);

bool lv_obj_is_editable(lv_obj_t * obj);

bool lv_obj_is_group_def(lv_obj_t * obj);
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/../core/lv_group.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/../core/lv_group.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/../core/lv_group.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/../core/lv_group.h" 2







enum _lv_key_t {
    LV_KEY_UP = 17,
    LV_KEY_DOWN = 18,
    LV_KEY_RIGHT = 19,
    LV_KEY_LEFT = 20,
    LV_KEY_ESC = 27,
    LV_KEY_DEL = 127,
    LV_KEY_BACKSPACE = 8,
    LV_KEY_ENTER = 10,
    LV_KEY_NEXT = 9,
    LV_KEY_PREV = 11,
    LV_KEY_HOME = 2,
    LV_KEY_END = 3,
};





typedef void (*lv_group_focus_cb_t)(lv_group_t *);
typedef void (*lv_group_edge_cb_t)(lv_group_t *, bool);





struct _lv_group_t {
    lv_ll_t obj_ll;
    lv_obj_t ** obj_focus;

    lv_group_focus_cb_t focus_cb;
    lv_group_edge_cb_t edge_cb;



    void * user_data;

    uint8_t frozen : 1;
    uint8_t editing : 1;
    uint8_t refocus_policy : 1;

    uint8_t wrap : 1;

};

typedef enum {
    LV_GROUP_REFOCUS_POLICY_NEXT = 0,
    LV_GROUP_REFOCUS_POLICY_PREV = 1
} lv_group_refocus_policy_t;
# 86 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/../core/lv_group.h"
void _lv_group_init(void);





void _lv_group_deinit(void);





lv_group_t * lv_group_create(void);





void lv_group_delete(lv_group_t * group);





void lv_group_set_default(lv_group_t * group);





lv_group_t * lv_group_get_default(void);






void lv_group_add_obj(lv_group_t * group, lv_obj_t * obj);






void lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2);





void lv_group_remove_obj(lv_obj_t * obj);





void lv_group_remove_all_objs(lv_group_t * group);





void lv_group_focus_obj(lv_obj_t * obj);





void lv_group_focus_next(lv_group_t * group);





void lv_group_focus_prev(lv_group_t * group);






void lv_group_focus_freeze(lv_group_t * group, bool en);







lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c);






void lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb);






void lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb);







void lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy);






void lv_group_set_editing(lv_group_t * group, bool edit);






void lv_group_set_wrap(lv_group_t * group, bool en);






lv_obj_t * lv_group_get_focused(const lv_group_t * group);






lv_group_focus_cb_t lv_group_get_focus_cb(const lv_group_t * group);






lv_group_edge_cb_t lv_group_get_edge_cb(const lv_group_t * group);






bool lv_group_get_editing(const lv_group_t * group);






bool lv_group_get_wrap(lv_group_t * group);






uint32_t lv_group_get_obj_count(lv_group_t * group);





uint32_t lv_group_get_count(void);





lv_group_t * lv_group_by_index(uint32_t index);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
typedef enum {
    LV_INDEV_TYPE_NONE,
    LV_INDEV_TYPE_POINTER,
    LV_INDEV_TYPE_KEYPAD,
    LV_INDEV_TYPE_BUTTON,
    LV_INDEV_TYPE_ENCODER,
} lv_indev_type_t;


typedef enum {
    LV_INDEV_STATE_RELEASED = 0,
    LV_INDEV_STATE_PRESSED
} lv_indev_state_t;

typedef enum {
    LV_INDEV_MODE_NONE = 0,
    LV_INDEV_MODE_TIMER,
    LV_INDEV_MODE_EVENT,
} lv_indev_mode_t;


typedef struct {
    lv_point_t point;
    uint32_t key;
    uint32_t btn_id;
    int16_t enc_diff;

    lv_indev_state_t state;
    bool continue_reading;
} lv_indev_data_t;

typedef void (*lv_indev_read_cb_t)(lv_indev_t * indev, lv_indev_data_t * data);





lv_indev_t * lv_indev_create(void);





void lv_indev_delete(lv_indev_t * indev);







lv_indev_t * lv_indev_get_next(lv_indev_t * indev);





void lv_indev_read(lv_indev_t * indev);





void lv_indev_read_timer_cb(lv_timer_t * timer);






void lv_indev_enable(lv_indev_t * indev, bool en);






lv_indev_t * lv_indev_active(void);






void lv_indev_set_type(lv_indev_t * indev, lv_indev_type_t indev_type);

void lv_indev_set_read_cb(lv_indev_t * indev, lv_indev_read_cb_t read_cb);

void lv_indev_set_user_data(lv_indev_t * indev, void * user_data);

void lv_indev_set_driver_data(lv_indev_t * indev, void * driver_data);






lv_indev_type_t lv_indev_get_type(const lv_indev_t * indev);

lv_indev_read_cb_t lv_indev_get_read_cb(lv_indev_t * indev);

lv_indev_state_t lv_indev_get_state(const lv_indev_t * indev);

lv_group_t * lv_indev_get_group(const lv_indev_t * indev);

void lv_indev_set_display(lv_indev_t * indev, struct _lv_display_t * disp);

lv_display_t * lv_indev_get_display(const lv_indev_t * indev);

void * lv_indev_get_user_data(const lv_indev_t * indev);

void * lv_indev_get_driver_data(const lv_indev_t * indev);






void lv_indev_reset(lv_indev_t * indev, lv_obj_t * obj);





void lv_indev_reset_long_press(lv_indev_t * indev);






void lv_indev_set_cursor(lv_indev_t * indev, lv_obj_t * cur_obj);






void lv_indev_set_group(lv_indev_t * indev, lv_group_t * group);







void lv_indev_set_button_points(lv_indev_t * indev, const lv_point_t points[]);






void lv_indev_get_point(const lv_indev_t * indev, lv_point_t * point);






lv_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev);






uint32_t lv_indev_get_key(const lv_indev_t * indev);
# 206 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
lv_dir_t lv_indev_get_scroll_dir(const lv_indev_t * indev);







lv_obj_t * lv_indev_get_scroll_obj(const lv_indev_t * indev);







void lv_indev_get_vect(const lv_indev_t * indev, lv_point_t * point);





void lv_indev_wait_release(lv_indev_t * indev);





lv_obj_t * lv_indev_get_active_obj(void);







lv_timer_t * lv_indev_get_read_timer(lv_indev_t * indev);






void lv_indev_set_mode(lv_indev_t * indev, lv_indev_mode_t mode);






lv_indev_mode_t lv_indev_get_mode(lv_indev_t * indev);







lv_obj_t * lv_indev_search_obj(lv_obj_t * obj, lv_point_t * point);
# 273 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
void lv_indev_add_event_cb(lv_indev_t * indev, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data);






uint32_t lv_indev_get_event_count(lv_indev_t * indev);







lv_event_dsc_t * lv_indev_get_event_dsc(lv_indev_t * indev, uint32_t index);







bool lv_indev_remove_event(lv_indev_t * indev, uint32_t index);
# 305 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
uint32_t lv_indev_remove_event_cb_with_user_data(lv_indev_t * indev, lv_event_cb_t event_cb, void * user_data);
# 314 "../../micropython/../lv_binding_micropython/lvgl/src/core/../indev/lv_indev.h"
lv_result_t lv_indev_send_event(lv_indev_t * indev, lv_event_code_t code, void * param);
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h" 2
# 36 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h"
typedef struct {
    const lv_point_t * point;
    bool res;
} lv_hit_test_info_t;


typedef enum {
    LV_COVER_RES_COVER = 0,
    LV_COVER_RES_NOT_COVER = 1,
    LV_COVER_RES_MASKED = 2,
} lv_cover_res_t;






typedef struct {
    lv_cover_res_t res;
    const lv_area_t * area;
} lv_cover_check_info_t;
# 69 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h"
lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param);







lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e);







lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e);






lv_obj_t * lv_event_get_target_obj(lv_event_t * e);
# 103 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h"
void lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter,
                         void * user_data);

uint32_t lv_obj_get_event_count(lv_obj_t * obj);

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index);

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index);

bool lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb);
# 121 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj_event.h"
uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data);






lv_indev_t * lv_event_get_indev(lv_event_t * e);







lv_layer_t * lv_event_get_layer(lv_event_t * e);






const lv_area_t * lv_event_get_old_size(lv_event_t * e);






uint32_t lv_event_get_key(lv_event_t * e);






lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e);






void lv_event_set_ext_draw_size(lv_event_t * e, int32_t size);







lv_point_t * lv_event_get_self_size_info(lv_event_t * e);






lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e);







const lv_area_t * lv_event_get_cover_area(lv_event_t * e);






void lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res);







lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e);
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h" 2
# 48 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
enum _lv_state_t {
    LV_STATE_DEFAULT = 0x0000,
    LV_STATE_CHECKED = 0x0001,
    LV_STATE_FOCUSED = 0x0002,
    LV_STATE_FOCUS_KEY = 0x0004,
    LV_STATE_EDITED = 0x0008,
    LV_STATE_HOVERED = 0x0010,
    LV_STATE_PRESSED = 0x0020,
    LV_STATE_SCROLLED = 0x0040,
    LV_STATE_DISABLED = 0x0080,
    LV_STATE_USER_1 = 0x1000,
    LV_STATE_USER_2 = 0x2000,
    LV_STATE_USER_3 = 0x4000,
    LV_STATE_USER_4 = 0x8000,

    LV_STATE_ANY = 0xFFFF,
};
# 73 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
enum _lv_part_t {
    LV_PART_MAIN = 0x000000,
    LV_PART_SCROLLBAR = 0x010000,
    LV_PART_INDICATOR = 0x020000,
    LV_PART_KNOB = 0x030000,
    LV_PART_SELECTED = 0x040000,
    LV_PART_ITEMS = 0x050000,
    LV_PART_CURSOR = 0x060000,

    LV_PART_CUSTOM_FIRST = 0x080000,

    LV_PART_ANY = 0x0F0000,
};
# 94 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
typedef enum {
    LV_OBJ_FLAG_HIDDEN = (1L << 0),
    LV_OBJ_FLAG_CLICKABLE = (1L << 1),
    LV_OBJ_FLAG_CLICK_FOCUSABLE = (1L << 2),
    LV_OBJ_FLAG_CHECKABLE = (1L << 3),
    LV_OBJ_FLAG_SCROLLABLE = (1L << 4),
    LV_OBJ_FLAG_SCROLL_ELASTIC = (1L << 5),
    LV_OBJ_FLAG_SCROLL_MOMENTUM = (1L << 6),
    LV_OBJ_FLAG_SCROLL_ONE = (1L << 7),
    LV_OBJ_FLAG_SCROLL_CHAIN_HOR = (1L << 8),
    LV_OBJ_FLAG_SCROLL_CHAIN_VER = (1L << 9),
    LV_OBJ_FLAG_SCROLL_CHAIN = (LV_OBJ_FLAG_SCROLL_CHAIN_HOR | LV_OBJ_FLAG_SCROLL_CHAIN_VER),
    LV_OBJ_FLAG_SCROLL_ON_FOCUS = (1L << 10),
    LV_OBJ_FLAG_SCROLL_WITH_ARROW = (1L << 11),
    LV_OBJ_FLAG_SNAPPABLE = (1L << 12),
    LV_OBJ_FLAG_PRESS_LOCK = (1L << 13),
    LV_OBJ_FLAG_EVENT_BUBBLE = (1L << 14),
    LV_OBJ_FLAG_GESTURE_BUBBLE = (1L << 15),
    LV_OBJ_FLAG_ADV_HITTEST = (1L << 16),
    LV_OBJ_FLAG_IGNORE_LAYOUT = (1L << 17),
    LV_OBJ_FLAG_FLOATING = (1L << 18),
    LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS = (1L << 19),
    LV_OBJ_FLAG_OVERFLOW_VISIBLE = (1L << 20),

    LV_OBJ_FLAG_FLEX_IN_NEW_TRACK = (1L << 21),


    LV_OBJ_FLAG_LAYOUT_1 = (1L << 23),
    LV_OBJ_FLAG_LAYOUT_2 = (1L << 24),

    LV_OBJ_FLAG_WIDGET_1 = (1L << 25),
    LV_OBJ_FLAG_WIDGET_2 = (1L << 26),
    LV_OBJ_FLAG_USER_1 = (1L << 27),
    LV_OBJ_FLAG_USER_2 = (1L << 28),
    LV_OBJ_FLAG_USER_3 = (1L << 29),
    LV_OBJ_FLAG_USER_4 = (1L << 30),
} _lv_obj_flag_t;
# 195 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
                         extern const lv_obj_class_t lv_obj_class;





typedef struct {
    lv_obj_t ** children;
    lv_group_t * group_p;
    lv_event_list_t event_list;

    lv_point_t scroll;

    int32_t ext_click_pad;
    int32_t ext_draw_size;

    uint16_t child_cnt;
    uint16_t scrollbar_mode : 2;
    uint16_t scroll_snap_x : 2;
    uint16_t scroll_snap_y : 2;
    uint16_t scroll_dir : 4;
    uint16_t layer_type : 2;
} _lv_obj_spec_attr_t;

struct _lv_obj_t {
    const lv_obj_class_t * class_p;
    lv_obj_t * parent;
    _lv_obj_spec_attr_t * spec_attr;
    _lv_obj_style_t * styles;

    uint32_t style_main_prop_is_set;
    uint32_t style_other_prop_is_set;

    void * user_data;



    lv_area_t coords;
    lv_obj_flag_t flags;
    lv_state_t state;
    uint16_t layout_inv : 1;
    uint16_t readjust_scroll_after_layout : 1;
    uint16_t scr_layout_inv : 1;
    uint16_t skip_trans : 1;
    uint16_t style_cnt : 6;
    uint16_t h_layout : 1;
    uint16_t w_layout : 1;
    uint16_t is_deleting : 1;
};
# 254 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
lv_obj_t * lv_obj_create(lv_obj_t * parent);
# 265 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
void lv_obj_add_flag(lv_obj_t * obj, lv_obj_flag_t f);






void lv_obj_remove_flag(lv_obj_t * obj, lv_obj_flag_t f);







void lv_obj_update_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);







void lv_obj_add_state(lv_obj_t * obj, lv_state_t state);







void lv_obj_remove_state(lv_obj_t * obj, lv_state_t state);







void lv_obj_set_state(lv_obj_t * obj, lv_state_t state, bool v);






static inline void lv_obj_set_user_data(lv_obj_t * obj, void * user_data)
{
    obj->user_data = user_data;
}
# 326 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
bool lv_obj_has_flag(const lv_obj_t * obj, lv_obj_flag_t f);







bool lv_obj_has_flag_any(const lv_obj_t * obj, lv_obj_flag_t f);






lv_state_t lv_obj_get_state(const lv_obj_t * obj);







bool lv_obj_has_state(const lv_obj_t * obj, lv_state_t state);






lv_group_t * lv_obj_get_group(const lv_obj_t * obj);






static inline void * lv_obj_get_user_data(lv_obj_t * obj)
{
    return obj->user_data;
}
# 376 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
void lv_obj_allocate_spec_attr(lv_obj_t * obj);







bool lv_obj_check_type(const lv_obj_t * obj, const lv_obj_class_t * class_p);
# 393 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_obj.h"
bool lv_obj_has_class(const lv_obj_t * obj, const lv_obj_class_t * class_p);






const lv_obj_class_t * lv_obj_get_class(const lv_obj_t * obj);






bool lv_obj_is_valid(const lv_obj_t * obj);
# 42 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_refr.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_refr.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_refr.h" 2
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_refr.h"
void _lv_refr_init(void);




void _lv_refr_deinit(void);
# 61 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_refr.h"
void lv_refr_now(lv_display_t * disp);






void lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj);







void _lv_inv_area(lv_display_t * disp, const lv_area_t * area_p);





lv_display_t * _lv_refr_get_disp_refreshing(void);





void _lv_refr_set_disp_refreshing(lv_display_t * disp);





void _lv_display_refr_timer(lv_timer_t * timer);
# 45 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2



# 1 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_binfont_loader.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_binfont_loader.h"
lv_font_t * lv_binfont_create(const char * font_name);
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_binfont_loader.h"
lv_font_t * lv_binfont_create_from_buffer(void * buffer, uint32_t size);






void lv_binfont_destroy(lv_font_t * font);
# 49 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
typedef struct {

    uint32_t bitmap_index : 20;
    uint32_t adv_w : 12;
    uint8_t box_w;
    uint8_t box_h;
    int8_t ofs_x;
    int8_t ofs_y;
# 46 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
} lv_font_fmt_txt_glyph_dsc_t;


enum _lv_font_fmt_txt_cmap_type_t {
    LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL,
    LV_FONT_FMT_TXT_CMAP_SPARSE_FULL,
    LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY,
    LV_FONT_FMT_TXT_CMAP_SPARSE_TINY,
};




typedef uint8_t lv_font_fmt_txt_cmap_type_t;







typedef struct {

    uint32_t range_start;



    uint16_t range_length;


    uint16_t glyph_id_start;
# 105 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
    const uint16_t * unicode_list;




    const void * glyph_id_ofs_list;


    uint16_t list_length;


    lv_font_fmt_txt_cmap_type_t type;
} lv_font_fmt_txt_cmap_t;


typedef struct {







    const void * glyph_ids;
    const int8_t * values;
    uint32_t pair_cnt : 30;
    uint32_t glyph_ids_size : 2;
} lv_font_fmt_txt_kern_pair_t;


typedef struct {
# 144 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
    const int8_t * class_pair_values;
    const uint8_t * left_class_mapping;
    const uint8_t * right_class_mapping;
    uint8_t left_class_cnt;
    uint8_t right_class_cnt;
} lv_font_fmt_txt_kern_classes_t;


typedef enum {
    LV_FONT_FMT_TXT_PLAIN = 0,
    LV_FONT_FMT_TXT_COMPRESSED = 1,
    LV_FONT_FMT_TXT_COMPRESSED_NO_PREFILTER = 1,
} lv_font_fmt_txt_bitmap_format_t;


typedef struct {

    const uint8_t * glyph_bitmap;


    const lv_font_fmt_txt_glyph_dsc_t * glyph_dsc;



    const lv_font_fmt_txt_cmap_t * cmaps;






    const void * kern_dsc;


    uint16_t kern_scale;


    uint16_t cmap_num : 9;


    uint16_t bpp : 4;


    uint16_t kern_classes : 1;





    uint16_t bitmap_format : 2;
} lv_font_fmt_txt_dsc_t;
# 224 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
const void * lv_font_get_bitmap_fmt_txt(lv_font_glyph_dsc_t * g_dsc, uint32_t unicode_letter,
                                        lv_draw_buf_t * draw_buf);
# 236 "../../micropython/../lv_binding_micropython/lvgl/src/font/lv_font_fmt_txt.h"
bool lv_font_get_glyph_dsc_fmt_txt(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter,
                                   uint32_t unicode_letter_next);
# 50 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h" 1
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
typedef struct {
    lv_obj_t obj;
    const void * src;
    lv_point_t offset;
    int32_t w;
    int32_t h;
    uint32_t rotation;
    uint32_t scale_x;
    uint32_t scale_y;
    lv_point_t pivot;
    uint32_t src_type : 2;
    uint32_t cf : 5;
    uint32_t antialias : 1;
    uint32_t align: 4;
    uint32_t blend_mode: 4;
} lv_image_t;

                         extern const lv_obj_class_t lv_image_class;




enum _lv_image_align_t {
    LV_IMAGE_ALIGN_DEFAULT = 0,
    LV_IMAGE_ALIGN_TOP_LEFT,
    LV_IMAGE_ALIGN_TOP_MID,
    LV_IMAGE_ALIGN_TOP_RIGHT,
    LV_IMAGE_ALIGN_BOTTOM_LEFT,
    LV_IMAGE_ALIGN_BOTTOM_MID,
    LV_IMAGE_ALIGN_BOTTOM_RIGHT,
    LV_IMAGE_ALIGN_LEFT_MID,
    LV_IMAGE_ALIGN_RIGHT_MID,
    LV_IMAGE_ALIGN_CENTER,
    _LV_IMAGE_ALIGN_AUTO_TRANSFORM,
    LV_IMAGE_ALIGN_STRETCH,
    LV_IMAGE_ALIGN_TILE,
};




typedef uint8_t lv_image_align_t;
# 110 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
lv_obj_t * lv_image_create(lv_obj_t * parent);
# 123 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_src(lv_obj_t * obj, const void * src);






void lv_image_set_offset_x(lv_obj_t * obj, int32_t x);







void lv_image_set_offset_y(lv_obj_t * obj, int32_t y);
# 150 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_rotation(lv_obj_t * obj, int32_t angle);
# 160 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_pivot(lv_obj_t * obj, int32_t x, int32_t y);




static inline void _lv_image_set_pivot(lv_obj_t * obj, lv_point_t * pivot)
{
    lv_image_set_pivot(obj, pivot->x, pivot->y);
}
# 181 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_scale(lv_obj_t * obj, uint32_t zoom);
# 194 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_scale_x(lv_obj_t * obj, uint32_t zoom);
# 207 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_scale_y(lv_obj_t * obj, uint32_t zoom);






void lv_image_set_blend_mode(lv_obj_t * obj, lv_blend_mode_t blend_mode);







void lv_image_set_antialias(lv_obj_t * obj, bool antialias);
# 231 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
void lv_image_set_align(lv_obj_t * obj, lv_image_align_t align);
# 242 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
const void * lv_image_get_src(lv_obj_t * obj);






int32_t lv_image_get_offset_x(lv_obj_t * obj);






int32_t lv_image_get_offset_y(lv_obj_t * obj);
# 265 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/../image/lv_image.h"
int32_t lv_image_get_rotation(lv_obj_t * obj);







void lv_image_get_pivot(lv_obj_t * obj, lv_point_t * pivot);






int32_t lv_image_get_scale(lv_obj_t * obj);






int32_t lv_image_get_scale_x(lv_obj_t * obj);






int32_t lv_image_get_scale_y(lv_obj_t * obj);






lv_blend_mode_t lv_image_get_blend_mode(lv_obj_t * obj);






bool lv_image_get_antialias(lv_obj_t * obj);






lv_image_align_t lv_image_get_align(lv_obj_t * obj);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h" 2
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h"
                         extern const lv_obj_class_t lv_animimg_class;


typedef struct {
    lv_image_t img;
    lv_anim_t anim;

    const void ** dsc;
    int8_t pic_count;
} lv_animimg_t;


enum _lv_animimg_part_t {
    LV_ANIM_IMAGE_PART_MAIN,
};




typedef uint8_t lv_animimg_part_t;
# 64 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h"
lv_obj_t * lv_animimg_create(lv_obj_t * parent);
# 76 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h"
void lv_animimg_set_src(lv_obj_t * img, const void * dsc[], size_t num);





void lv_animimg_start(lv_obj_t * obj);






void lv_animimg_set_duration(lv_obj_t * img, uint32_t duration);






void lv_animimg_set_repeat_count(lv_obj_t * img, uint32_t count);
# 107 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/animimage/lv_animimage.h"
const void ** lv_animimg_get_src(lv_obj_t * img);






uint8_t lv_animimg_get_src_count(lv_obj_t * img);






uint32_t lv_animimg_get_duration(lv_obj_t * img);






uint32_t lv_animimg_get_repeat_count(lv_obj_t * img);
# 52 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h"
enum _lv_arc_mode_t {
    LV_ARC_MODE_NORMAL,
    LV_ARC_MODE_SYMMETRICAL,
    LV_ARC_MODE_REVERSE
};




typedef uint8_t lv_arc_mode_t;


typedef struct {
    lv_obj_t obj;
    int32_t rotation;
    lv_value_precise_t indic_angle_start;
    lv_value_precise_t indic_angle_end;
    lv_value_precise_t bg_angle_start;
    lv_value_precise_t bg_angle_end;
    int32_t value;
    int32_t min_value;
    int32_t max_value;
    uint32_t dragging : 1;
    uint32_t type : 2;
    uint32_t min_close : 1;
    uint32_t in_out : 1;
    uint32_t chg_rate;
    uint32_t last_tick;
    lv_value_precise_t last_angle;
    int16_t knob_offset;
} lv_arc_t;

                         extern const lv_obj_class_t lv_arc_class;
# 72 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h"
lv_obj_t * lv_arc_create(lv_obj_t * parent);
# 87 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h"
void lv_arc_set_start_angle(lv_obj_t * obj, lv_value_precise_t start);






void lv_arc_set_end_angle(lv_obj_t * obj, lv_value_precise_t end);







void lv_arc_set_angles(lv_obj_t * obj, lv_value_precise_t start, lv_value_precise_t end);






void lv_arc_set_bg_start_angle(lv_obj_t * obj, lv_value_precise_t start);






void lv_arc_set_bg_end_angle(lv_obj_t * obj, lv_value_precise_t end);







void lv_arc_set_bg_angles(lv_obj_t * obj, lv_value_precise_t start, lv_value_precise_t end);






void lv_arc_set_rotation(lv_obj_t * obj, int32_t rotation);






void lv_arc_set_mode(lv_obj_t * obj, lv_arc_mode_t type);






void lv_arc_set_value(lv_obj_t * obj, int32_t value);







void lv_arc_set_range(lv_obj_t * obj, int32_t min, int32_t max);






void lv_arc_set_change_rate(lv_obj_t * obj, uint32_t rate);






void lv_arc_set_knob_offset(lv_obj_t * obj, int32_t offset);
# 178 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h"
lv_value_precise_t lv_arc_get_angle_start(lv_obj_t * obj);






lv_value_precise_t lv_arc_get_angle_end(lv_obj_t * obj);






lv_value_precise_t lv_arc_get_bg_angle_start(lv_obj_t * obj);






lv_value_precise_t lv_arc_get_bg_angle_end(lv_obj_t * obj);






int32_t lv_arc_get_value(const lv_obj_t * obj);






int32_t lv_arc_get_min_value(const lv_obj_t * obj);






int32_t lv_arc_get_max_value(const lv_obj_t * obj);






lv_arc_mode_t lv_arc_get_mode(const lv_obj_t * obj);






int32_t lv_arc_get_rotation(const lv_obj_t * obj);






int32_t lv_arc_get_knob_offset(const lv_obj_t * obj);
# 253 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/arc/lv_arc.h"
void lv_arc_align_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_align, int32_t r_offset);







void lv_arc_rotate_obj_to_angle(const lv_obj_t * obj, lv_obj_t * obj_to_rotate, int32_t r_offset);
# 53 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdarg.h" 1
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h" 2
# 39 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
enum {ENUM_LV_LABEL_DOT_NUM = 3};
enum {ENUM_LV_LABEL_POS_LAST = 0xFFFF};
enum {ENUM_LV_LABEL_TEXT_SELECTION_OFF = (0xFFFF)};






enum _lv_label_long_mode_t {
    LV_LABEL_LONG_WRAP,
    LV_LABEL_LONG_DOT,
    LV_LABEL_LONG_SCROLL,
    LV_LABEL_LONG_SCROLL_CIRCULAR,
    LV_LABEL_LONG_CLIP,
};




typedef uint8_t lv_label_long_mode_t;


typedef struct {
    lv_obj_t obj;
    char * text;
    union {
        char * tmp_ptr;
        char tmp[3 + 1];
    } dot;
    uint32_t dot_end;


    lv_draw_label_hint_t hint;



    uint32_t sel_start;
    uint32_t sel_end;


    lv_point_t size_cache;
    lv_point_t offset;
    lv_label_long_mode_t long_mode : 3;
    uint8_t static_txt : 1;
    uint8_t expand : 1;
    uint8_t dot_tmp_alloc : 1;
    uint8_t invalid_size_cache : 1;
} lv_label_t;

                         extern const lv_obj_class_t lv_label_class;
# 100 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
lv_obj_t * lv_label_create(lv_obj_t * parent);
# 111 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
void lv_label_set_text(lv_obj_t * obj, const char * text);







void lv_label_set_text_fmt(lv_obj_t * obj, const char * fmt, ...) ;







void lv_label_set_text_static(lv_obj_t * obj, const char * text);







void lv_label_set_long_mode(lv_obj_t * obj, lv_label_long_mode_t long_mode);






void lv_label_set_text_selection_start(lv_obj_t * obj, uint32_t index);






void lv_label_set_text_selection_end(lv_obj_t * obj, uint32_t index);
# 160 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
char * lv_label_get_text(const lv_obj_t * obj);






lv_label_long_mode_t lv_label_get_long_mode(const lv_obj_t * obj);
# 176 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
void lv_label_get_letter_pos(const lv_obj_t * obj, uint32_t char_id, lv_point_t * pos);
# 186 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
uint32_t lv_label_get_letter_on(const lv_obj_t * obj, lv_point_t * pos_in, bool bidi);







bool lv_label_is_char_under_pos(const lv_obj_t * obj, lv_point_t * pos);






uint32_t lv_label_get_text_selection_start(const lv_obj_t * obj);






uint32_t lv_label_get_text_selection_end(const lv_obj_t * obj);
# 221 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
void lv_label_ins_text(lv_obj_t * obj, uint32_t pos, const char * txt);
# 230 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/../label/lv_label.h"
void lv_label_cut_text(lv_obj_t * obj, uint32_t pos, uint32_t cnt);
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h" 2








enum _lv_bar_mode_t {
    LV_BAR_MODE_NORMAL,
    LV_BAR_MODE_SYMMETRICAL,
    LV_BAR_MODE_RANGE
};




typedef uint8_t lv_bar_mode_t;


typedef struct {
    lv_obj_t * bar;
    int32_t anim_start;
    int32_t anim_end;
    int32_t anim_state;
} _lv_bar_anim_t;

typedef struct {
    lv_obj_t obj;
    int32_t cur_value;
    int32_t min_value;
    int32_t max_value;
    int32_t start_value;
    lv_area_t indic_area;
    bool val_reversed;
    _lv_bar_anim_t cur_value_anim;
    _lv_bar_anim_t start_value_anim;
    lv_bar_mode_t mode : 2;
} lv_bar_t;

                         extern const lv_obj_class_t lv_bar_class;
# 74 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h"
lv_obj_t * lv_bar_create(lv_obj_t * parent);
# 86 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h"
void lv_bar_set_value(lv_obj_t * obj, int32_t value, lv_anim_enable_t anim);







void lv_bar_set_start_value(lv_obj_t * obj, int32_t start_value, lv_anim_enable_t anim);
# 103 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h"
void lv_bar_set_range(lv_obj_t * obj, int32_t min, int32_t max);






void lv_bar_set_mode(lv_obj_t * obj, lv_bar_mode_t mode);
# 121 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/bar/lv_bar.h"
int32_t lv_bar_get_value(const lv_obj_t * obj);






int32_t lv_bar_get_start_value(const lv_obj_t * obj);






int32_t lv_bar_get_min_value(const lv_obj_t * obj);






int32_t lv_bar_get_max_value(const lv_obj_t * obj);






lv_bar_mode_t lv_bar_get_mode(lv_obj_t * obj);






bool lv_bar_is_symmetrical(lv_obj_t * obj);
# 54 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/button/lv_button.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/button/lv_button.h"
typedef struct {
    lv_obj_t obj;
} lv_button_t;

                         extern const lv_obj_class_t lv_button_class;
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/button/lv_button.h"
lv_obj_t * lv_button_create(lv_obj_t * parent);
# 55 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h" 1
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
enum {ENUM_LV_BUTTONMATRIX_BUTTON_NONE = 0xFFFF};







enum _lv_buttonmatrix_ctrl_t {
    _LV_BUTTONMATRIX_WIDTH = 0x000F,
    LV_BUTTONMATRIX_CTRL_HIDDEN = 0x0010,
    LV_BUTTONMATRIX_CTRL_NO_REPEAT = 0x0020,
    LV_BUTTONMATRIX_CTRL_DISABLED = 0x0040,
    LV_BUTTONMATRIX_CTRL_CHECKABLE = 0x0080,
    LV_BUTTONMATRIX_CTRL_CHECKED = 0x0100,
    LV_BUTTONMATRIX_CTRL_CLICK_TRIG = 0x0200,
    LV_BUTTONMATRIX_CTRL_POPOVER = 0x0400,
    _LV_BUTTONMATRIX_CTRL_RESERVED_1 = 0x0800,
    _LV_BUTTONMATRIX_CTRL_RESERVED_2 = 0x1000,
    _LV_BUTTONMATRIX_CTRL_RESERVED_3 = 0x2000,
    LV_BUTTONMATRIX_CTRL_CUSTOM_1 = 0x4000,
    LV_BUTTONMATRIX_CTRL_CUSTOM_2 = 0x8000,
};




typedef uint32_t lv_buttonmatrix_ctrl_t;


typedef bool (*lv_buttonmatrix_button_draw_cb_t)(lv_obj_t * btnm, uint32_t btn_id, const lv_area_t * draw_area,
                                                 const lv_area_t * clip_area);


typedef struct {
    lv_obj_t obj;
    const char ** map_p;
    lv_area_t * button_areas;
    lv_buttonmatrix_ctrl_t * ctrl_bits;
    uint32_t btn_cnt;
    uint32_t row_cnt;
    uint32_t btn_id_sel;
    uint32_t one_check : 1;
} lv_buttonmatrix_t;

                         extern const lv_obj_class_t lv_buttonmatrix_class;
# 82 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
lv_obj_t * lv_buttonmatrix_create(lv_obj_t * parent);
# 95 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
void lv_buttonmatrix_set_map(lv_obj_t * obj, const char * map[]);
# 109 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
void lv_buttonmatrix_set_ctrl_map(lv_obj_t * obj, const lv_buttonmatrix_ctrl_t ctrl_map[]);






void lv_buttonmatrix_set_selected_button(lv_obj_t * obj, uint32_t btn_id);







void lv_buttonmatrix_set_button_ctrl(lv_obj_t * obj, uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);







void lv_buttonmatrix_clear_button_ctrl(lv_obj_t * obj, uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);






void lv_buttonmatrix_set_button_ctrl_all(lv_obj_t * obj, lv_buttonmatrix_ctrl_t ctrl);







void lv_buttonmatrix_clear_button_ctrl_all(lv_obj_t * obj, lv_buttonmatrix_ctrl_t ctrl);
# 158 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
void lv_buttonmatrix_set_button_width(lv_obj_t * obj, uint32_t btn_id, uint32_t width);
# 167 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
void lv_buttonmatrix_set_one_checked(lv_obj_t * obj, bool en);
# 178 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
const char ** lv_buttonmatrix_get_map(const lv_obj_t * obj);







uint32_t lv_buttonmatrix_get_selected_button(const lv_obj_t * obj);







const char * lv_buttonmatrix_get_button_text(const lv_obj_t * obj, uint32_t btn_id);
# 203 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.h"
bool lv_buttonmatrix_has_button_ctrl(lv_obj_t * obj, uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);






bool lv_buttonmatrix_get_one_checked(const lv_obj_t * obj);
# 56 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
typedef struct {
    uint16_t year;
    int8_t month;
    int8_t day;
} lv_calendar_date_t;


typedef struct {
    lv_obj_t obj;

    lv_obj_t * btnm;
    lv_calendar_date_t today;
    lv_calendar_date_t showed_date;
    lv_calendar_date_t * highlighted_dates;
    size_t highlighted_dates_num;
    const char * map[8 * 7];
    char nums [7 * 6][4];
} lv_calendar_t;

                         extern const lv_obj_class_t lv_calendar_class;
# 61 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
lv_obj_t * lv_calendar_create(lv_obj_t * parent);
# 78 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
void lv_calendar_set_today_date(lv_obj_t * obj, uint32_t year, uint32_t month, uint32_t day);







void lv_calendar_set_showed_date(lv_obj_t * obj, uint32_t year, uint32_t month);
# 95 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
void lv_calendar_set_highlighted_dates(lv_obj_t * obj, lv_calendar_date_t highlighted[], size_t date_num);
# 104 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
void lv_calendar_set_day_names(lv_obj_t * obj, const char ** day_names);
# 116 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
lv_obj_t * lv_calendar_get_btnmatrix(const lv_obj_t * obj);






const lv_calendar_date_t * lv_calendar_get_today_date(const lv_obj_t * calendar);






const lv_calendar_date_t * lv_calendar_get_showed_date(const lv_obj_t * calendar);






lv_calendar_date_t * lv_calendar_get_highlighted_dates(const lv_obj_t * calendar);






size_t lv_calendar_get_highlighted_dates_num(const lv_obj_t * calendar);
# 153 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
lv_result_t lv_calendar_get_pressed_date(const lv_obj_t * calendar, lv_calendar_date_t * date);
# 163 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_arrow.h" 1
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_arrow.h"
                         extern const lv_obj_class_t lv_calendar_header_arrow_class;
# 37 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_arrow.h"
lv_obj_t * lv_calendar_header_arrow_create(lv_obj_t * parent);
# 164 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_dropdown.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_dropdown.h"
                         extern const lv_obj_class_t lv_calendar_header_dropdown_class;
# 41 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_dropdown.h"
lv_obj_t * lv_calendar_header_dropdown_create(lv_obj_t * parent);
# 50 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar_header_dropdown.h"
void lv_calendar_header_dropdown_set_year_list(lv_obj_t * parent, const char * years_list);
# 165 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/calendar/lv_calendar.h" 2
# 57 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
                         extern const lv_obj_class_t lv_canvas_class;


typedef struct {
    lv_image_t img;
    lv_draw_buf_t * draw_buf;
    lv_draw_buf_t static_buf;
} lv_canvas_t;
# 48 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
lv_obj_t * lv_canvas_create(lv_obj_t * parent);
# 67 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
void lv_canvas_set_buffer(lv_obj_t * obj, void * buf, int32_t w, int32_t h, lv_color_format_t cf);
# 76 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
void lv_canvas_set_draw_buf(lv_obj_t * obj, lv_draw_buf_t * draw_buf);
# 90 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
void lv_canvas_set_px(lv_obj_t * obj, int32_t x, int32_t y, lv_color_t color, lv_opa_t opa);
# 102 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
void lv_canvas_set_palette(lv_obj_t * canvas, uint8_t id, lv_color32_t c);





lv_draw_buf_t * lv_canvas_get_draw_buf(lv_obj_t * obj);
# 117 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
lv_color32_t lv_canvas_get_px(lv_obj_t * obj, int32_t x, int32_t y);






lv_image_dsc_t * lv_canvas_get_image(lv_obj_t * canvas);
# 133 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
const void * lv_canvas_get_buf(lv_obj_t * canvas);
# 146 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
void lv_canvas_copy_buf(lv_obj_t * obj, const lv_area_t * canvas_area, lv_draw_buf_t * dest_buf,
                        const lv_area_t * dest_area);







void lv_canvas_fill_bg(lv_obj_t * obj, lv_color_t color, lv_opa_t opa);







void lv_canvas_init_layer(lv_obj_t * canvas, lv_layer_t * layer);







void lv_canvas_finish_layer(lv_obj_t * canvas, lv_layer_t * layer);
# 182 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/canvas/lv_canvas.h"
static inline uint32_t lv_canvas_buf_size(int32_t w, int32_t h, uint8_t bpp, uint8_t stride)
{
    return (uint32_t)(((((w * bpp + 7) >> 3) + stride - 1) & ~(stride - 1)) * h + 4);
}
# 58 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/../../lvgl.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/../../lvgl.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/../../lvgl.h" 2
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h" 2
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
enum {ENUM_LV_CHART_POINT_NONE = (INT32_MAX)};
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
enum _lv_chart_type_t {
    LV_CHART_TYPE_NONE,
    LV_CHART_TYPE_LINE,
    LV_CHART_TYPE_BAR,
    LV_CHART_TYPE_SCATTER,
};




typedef uint8_t lv_chart_type_t;





enum _lv_chart_update_mode_t {
    LV_CHART_UPDATE_MODE_SHIFT,
    LV_CHART_UPDATE_MODE_CIRCULAR,
};




typedef uint8_t lv_chart_update_mode_t;





enum _lv_chart_axis_t {
    LV_CHART_AXIS_PRIMARY_Y = 0x00,
    LV_CHART_AXIS_SECONDARY_Y = 0x01,
    LV_CHART_AXIS_PRIMARY_X = 0x02,
    LV_CHART_AXIS_SECONDARY_X = 0x04,
    _LV_CHART_AXIS_LAST
};




typedef uint8_t lv_chart_axis_t;





typedef struct {
    int32_t * x_points;
    int32_t * y_points;
    lv_color_t color;
    uint32_t start_point;
    uint32_t hidden : 1;
    uint32_t x_ext_buf_assigned : 1;
    uint32_t y_ext_buf_assigned : 1;
    uint32_t x_axis_sec : 1;
    uint32_t y_axis_sec : 1;
} lv_chart_series_t;

typedef struct {
    lv_point_t pos;
    int32_t point_id;
    lv_color_t color;
    lv_chart_series_t * ser;
    lv_dir_t dir;
    uint32_t pos_set: 1;
} lv_chart_cursor_t;

typedef struct {
    lv_obj_t obj;
    lv_ll_t series_ll;
    lv_ll_t cursor_ll;
    int32_t ymin[2];
    int32_t ymax[2];
    int32_t xmin[2];
    int32_t xmax[2];
    int32_t pressed_point_id;
    uint32_t hdiv_cnt;
    uint32_t vdiv_cnt;
    uint32_t point_cnt;
    lv_chart_type_t type : 3;
    lv_chart_update_mode_t update_mode : 1;
} lv_chart_t;

                         extern const lv_obj_class_t lv_chart_class;
# 130 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
lv_obj_t * lv_chart_create(lv_obj_t * parent);






void lv_chart_set_type(lv_obj_t * obj, lv_chart_type_t type);





void lv_chart_set_point_count(lv_obj_t * obj, uint32_t cnt);
# 152 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_range(lv_obj_t * obj, lv_chart_axis_t axis, int32_t min, int32_t max);






void lv_chart_set_update_mode(lv_obj_t * obj, lv_chart_update_mode_t update_mode);







void lv_chart_set_div_line_count(lv_obj_t * obj, uint8_t hdiv, uint8_t vdiv);






lv_chart_type_t lv_chart_get_type(const lv_obj_t * obj);






uint32_t lv_chart_get_point_count(const lv_obj_t * obj);







uint32_t lv_chart_get_x_start_point(const lv_obj_t * obj, lv_chart_series_t * ser);
# 198 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_get_point_pos_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, lv_point_t * p_out);





void lv_chart_refresh(lv_obj_t * obj);
# 217 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
lv_chart_series_t * lv_chart_add_series(lv_obj_t * obj, lv_color_t color, lv_chart_axis_t axis);






void lv_chart_remove_series(lv_obj_t * obj, lv_chart_series_t * series);







void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide);







void lv_chart_set_series_color(lv_obj_t * chart, lv_chart_series_t * series, lv_color_t color);
# 249 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_x_start_point(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id);







lv_chart_series_t * lv_chart_get_series_next(const lv_obj_t * chart, const lv_chart_series_t * ser);
# 270 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
lv_chart_cursor_t * lv_chart_add_cursor(lv_obj_t * obj, lv_color_t color, lv_dir_t dir);







void lv_chart_set_cursor_pos(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_point_t * pos);
# 287 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_chart_series_t * ser,
                               uint32_t point_id);







lv_point_t lv_chart_get_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor);
# 308 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_all_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value);







void lv_chart_set_next_value(lv_obj_t * obj, lv_chart_series_t * ser, int32_t value);
# 325 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_next_value2(lv_obj_t * obj, lv_chart_series_t * ser, int32_t x_value, int32_t y_value);
# 334 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_value_by_id(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, int32_t value);
# 345 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_value_by_id2(lv_obj_t * obj, lv_chart_series_t * ser, uint32_t id, int32_t x_value,
                               int32_t y_value);
# 355 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_ext_y_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[]);
# 364 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/chart/lv_chart.h"
void lv_chart_set_ext_x_array(lv_obj_t * obj, lv_chart_series_t * ser, int32_t array[]);







int32_t * lv_chart_get_y_array(const lv_obj_t * obj, lv_chart_series_t * ser);







int32_t * lv_chart_get_x_array(const lv_obj_t * obj, lv_chart_series_t * ser);






uint32_t lv_chart_get_pressed_point(const lv_obj_t * obj);







int32_t lv_chart_get_first_point_center_offset(lv_obj_t * obj);
# 59 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/checkbox/lv_checkbox.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/checkbox/lv_checkbox.h"
typedef struct {
    lv_obj_t obj;
    char * txt;
    uint32_t static_txt : 1;
} lv_checkbox_t;

                         extern const lv_obj_class_t lv_checkbox_class;
# 46 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/checkbox/lv_checkbox.h"
lv_obj_t * lv_checkbox_create(lv_obj_t * parent);
# 58 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/checkbox/lv_checkbox.h"
void lv_checkbox_set_text(lv_obj_t * obj, const char * txt);







void lv_checkbox_set_text_static(lv_obj_t * obj, const char * txt);
# 77 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/checkbox/lv_checkbox.h"
const char * lv_checkbox_get_text(const lv_obj_t * obj);
# 60 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h" 1
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
enum {ENUM_LV_DROPDOWN_POS_LAST = 0xFFFF};





typedef struct {
    lv_obj_t obj;
    lv_obj_t * list;
    const char * text;
    const void * symbol;
    char * options;
    uint32_t option_cnt;
    uint32_t sel_opt_id;
    uint32_t sel_opt_id_orig;
    uint32_t pr_opt_id;
    lv_dir_t dir : 4;
    uint8_t static_txt : 1;
    uint8_t selected_highlight: 1;
} lv_dropdown_t;

typedef struct {
    lv_obj_t obj;
    lv_obj_t * dropdown;
} lv_dropdown_list_t;

                         extern const lv_obj_class_t lv_dropdown_class;
                         extern const lv_obj_class_t lv_dropdownlist_class;
# 70 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
lv_obj_t * lv_dropdown_create(lv_obj_t * parent);
# 83 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
void lv_dropdown_set_text(lv_obj_t * obj, const char * txt);







void lv_dropdown_set_options(lv_obj_t * obj, const char * options);







void lv_dropdown_set_options_static(lv_obj_t * obj, const char * options);







void lv_dropdown_add_option(lv_obj_t * obj, const char * option, uint32_t pos);





void lv_dropdown_clear_options(lv_obj_t * obj);






void lv_dropdown_set_selected(lv_obj_t * obj, uint32_t sel_opt);






void lv_dropdown_set_dir(lv_obj_t * obj, lv_dir_t dir);
# 136 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
void lv_dropdown_set_symbol(lv_obj_t * obj, const void * symbol);






void lv_dropdown_set_selected_highlight(lv_obj_t * obj, bool en);
# 154 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
lv_obj_t * lv_dropdown_get_list(lv_obj_t * obj);






const char * lv_dropdown_get_text(lv_obj_t * obj);






const char * lv_dropdown_get_options(const lv_obj_t * obj);






uint32_t lv_dropdown_get_selected(const lv_obj_t * obj);






uint32_t lv_dropdown_get_option_count(const lv_obj_t * obj);







void lv_dropdown_get_selected_str(const lv_obj_t * obj, char * buf, uint32_t buf_size);







int32_t lv_dropdown_get_option_index(lv_obj_t * obj, const char * option);






const char * lv_dropdown_get_symbol(lv_obj_t * obj);






bool lv_dropdown_get_selected_highlight(lv_obj_t * obj);






lv_dir_t lv_dropdown_get_dir(const lv_obj_t * obj);
# 229 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/dropdown/lv_dropdown.h"
void lv_dropdown_open(lv_obj_t * dropdown_obj);





void lv_dropdown_close(lv_obj_t * obj);






bool lv_dropdown_is_open(lv_obj_t * obj);
# 61 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/imagebutton/lv_imagebutton.h" 1
# 23 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/imagebutton/lv_imagebutton.h"
typedef enum {
    LV_IMAGEBUTTON_STATE_RELEASED,
    LV_IMAGEBUTTON_STATE_PRESSED,
    LV_IMAGEBUTTON_STATE_DISABLED,
    LV_IMAGEBUTTON_STATE_CHECKED_RELEASED,
    LV_IMAGEBUTTON_STATE_CHECKED_PRESSED,
    LV_IMAGEBUTTON_STATE_CHECKED_DISABLED,
    _LV_IMAGEBUTTON_STATE_NUM,
} lv_imagebutton_state_t;

typedef struct {
    const void * img_src;
    lv_image_header_t header;
} lv_imagebutton_src_info_t;





typedef struct {
    lv_obj_t obj;
    lv_imagebutton_src_info_t src_mid[_LV_IMAGEBUTTON_STATE_NUM];
    lv_imagebutton_src_info_t src_left[_LV_IMAGEBUTTON_STATE_NUM];
    lv_imagebutton_src_info_t src_right[_LV_IMAGEBUTTON_STATE_NUM];
} lv_imagebutton_t;

                         extern const lv_obj_class_t lv_imagebutton_class;
# 60 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/imagebutton/lv_imagebutton.h"
lv_obj_t * lv_imagebutton_create(lv_obj_t * parent);
# 81 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/imagebutton/lv_imagebutton.h"
void lv_imagebutton_set_src(lv_obj_t * imagebutton, lv_imagebutton_state_t state, const void * src_left,
                            const void * src_mid,
                            const void * src_right);






void lv_imagebutton_set_state(lv_obj_t * imagebutton, lv_imagebutton_state_t state);
# 102 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/imagebutton/lv_imagebutton.h"
const void * lv_imagebutton_get_src_left(lv_obj_t * imagebutton, lv_imagebutton_state_t state);







const void * lv_imagebutton_get_src_middle(lv_obj_t * imagebutton, lv_imagebutton_state_t state);







const void * lv_imagebutton_get_src_right(lv_obj_t * imagebutton, lv_imagebutton_state_t state);
# 63 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h" 1
# 39 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
enum _lv_keyboard_mode_t {
    LV_KEYBOARD_MODE_TEXT_LOWER,
    LV_KEYBOARD_MODE_TEXT_UPPER,
    LV_KEYBOARD_MODE_SPECIAL,
    LV_KEYBOARD_MODE_NUMBER,
    LV_KEYBOARD_MODE_USER_1,
    LV_KEYBOARD_MODE_USER_2,
    LV_KEYBOARD_MODE_USER_3,
    LV_KEYBOARD_MODE_USER_4,



};




typedef uint8_t lv_keyboard_mode_t;



typedef struct {
    lv_buttonmatrix_t btnm;
    lv_obj_t * ta;
    lv_keyboard_mode_t mode;
    uint8_t popovers : 1;
} lv_keyboard_t;

                         extern const lv_obj_class_t lv_keyboard_class;
# 78 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
lv_obj_t * lv_keyboard_create(lv_obj_t * parent);
# 89 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
void lv_keyboard_set_textarea(lv_obj_t * kb, lv_obj_t * ta);






void lv_keyboard_set_mode(lv_obj_t * kb, lv_keyboard_mode_t mode);






void lv_keyboard_set_popovers(lv_obj_t * kb, bool en);
# 114 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
void lv_keyboard_set_map(lv_obj_t * kb, lv_keyboard_mode_t mode, const char * map[],
                         const lv_buttonmatrix_ctrl_t ctrl_map[]);
# 126 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
lv_obj_t * lv_keyboard_get_textarea(const lv_obj_t * kb);






lv_keyboard_mode_t lv_keyboard_get_mode(const lv_obj_t * kb);






bool lv_buttonmatrix_get_popovers(const lv_obj_t * obj);






static inline const char ** lv_keyboard_get_map_array(const lv_obj_t * kb)
{
    return lv_buttonmatrix_get_map(kb);
}







static inline uint32_t lv_keyboard_get_selected_button(const lv_obj_t * obj)
{
    return lv_buttonmatrix_get_selected_button(obj);
}







static inline const char * lv_keyboard_get_button_text(const lv_obj_t * obj, uint32_t btn_id)
{
    return lv_buttonmatrix_get_button_text(obj, btn_id);
}
# 184 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/keyboard/lv_keyboard.h"
void lv_keyboard_def_event_cb(lv_event_t * e);
# 64 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/led/lv_led.h" 1
# 38 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/led/lv_led.h"
typedef struct {
    lv_obj_t obj;
    lv_color_t color;
    uint8_t bright;
} lv_led_t;

                         extern const lv_obj_class_t lv_led_class;
# 55 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/led/lv_led.h"
lv_obj_t * lv_led_create(lv_obj_t * parent);






void lv_led_set_color(lv_obj_t * led, lv_color_t color);






void lv_led_set_brightness(lv_obj_t * led, uint8_t bright);





void lv_led_on(lv_obj_t * led);





void lv_led_off(lv_obj_t * led);





void lv_led_toggle(lv_obj_t * led);






uint8_t lv_led_get_brightness(const lv_obj_t * obj);
# 66 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h" 1
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h"
typedef struct {
    lv_obj_t obj;
    const lv_point_precise_t * point_array;
    uint32_t point_num;
    uint32_t y_inv : 1;
} lv_line_t;

                         extern const lv_obj_class_t lv_line_class;
# 46 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h"
lv_obj_t * lv_line_create(lv_obj_t * parent);
# 58 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h"
void lv_line_set_points(lv_obj_t * obj, const lv_point_precise_t points[], uint32_t point_num);
# 67 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h"
void lv_line_set_y_invert(lv_obj_t * obj, bool en);
# 78 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/line/lv_line.h"
bool lv_line_get_y_invert(const lv_obj_t * obj);
# 67 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/list/lv_list.h" 1
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/list/lv_list.h"
                         extern const lv_obj_class_t lv_list_class;
                         extern const lv_obj_class_t lv_list_text_class;
                         extern const lv_obj_class_t lv_list_button_class;
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/list/lv_list.h"
lv_obj_t * lv_list_create(lv_obj_t * parent);







lv_obj_t * lv_list_add_text(lv_obj_t * list, const char * txt);
# 57 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/list/lv_list.h"
lv_obj_t * lv_list_add_button(lv_obj_t * list, const void * icon, const char * txt);







const char * lv_list_get_button_text(lv_obj_t * list, lv_obj_t * btn);
# 74 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/list/lv_list.h"
void lv_list_set_button_text(lv_obj_t * list, lv_obj_t * btn, const char * txt);
# 68 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/menu/lv_menu.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/menu/lv_menu.h"
enum _lv_menu_mode_header_t {
    LV_MENU_HEADER_TOP_FIXED,
    LV_MENU_HEADER_TOP_UNFIXED,
    LV_MENU_HEADER_BOTTOM_FIXED
};




typedef uint8_t lv_menu_mode_header_t;


enum _lv_menu_mode_root_back_button_t {
    LV_MENU_ROOT_BACK_BUTTON_DISABLED,
    LV_MENU_ROOT_BACK_BUTTON_ENABLED
};




typedef uint8_t lv_menu_mode_root_back_button_t;


typedef struct



    lv_menu_load_page_event_data_t

{
    lv_obj_t * menu;
    lv_obj_t * page;
} lv_menu_load_page_event_data_t ;

typedef struct {
    lv_obj_t * page;
} lv_menu_history_t;

typedef struct {
    lv_obj_t obj;
    lv_obj_t * storage;
    lv_obj_t * main;
    lv_obj_t * main_page;
    lv_obj_t * main_header;
    lv_obj_t *
    main_header_back_btn;
    lv_obj_t * main_header_title;
    lv_obj_t * sidebar;
    lv_obj_t * sidebar_page;
    lv_obj_t * sidebar_header;
    lv_obj_t *
    sidebar_header_back_btn;
    lv_obj_t * sidebar_header_title;
    lv_obj_t * selected_tab;
    lv_ll_t history_ll;
    uint8_t cur_depth;
    uint8_t prev_depth;
    uint8_t sidebar_generated : 1;
    lv_menu_mode_header_t mode_header : 2;
    lv_menu_mode_root_back_button_t mode_root_back_btn : 1;
} lv_menu_t;

typedef struct {
    lv_obj_t obj;
    char * title;
    bool static_title;
} lv_menu_page_t;

                         extern const lv_obj_class_t lv_menu_class;
                         extern const lv_obj_class_t lv_menu_page_class;
                         extern const lv_obj_class_t lv_menu_cont_class;
                         extern const lv_obj_class_t lv_menu_section_class;
                         extern const lv_obj_class_t lv_menu_separator_class;
                         extern const lv_obj_class_t lv_menu_sidebar_cont_class;
                         extern const lv_obj_class_t lv_menu_main_cont_class;
                         extern const lv_obj_class_t lv_menu_sidebar_header_cont_class;
                         extern const lv_obj_class_t lv_menu_main_header_cont_class;
# 117 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/menu/lv_menu.h"
lv_obj_t * lv_menu_create(lv_obj_t * parent);







lv_obj_t * lv_menu_page_create(lv_obj_t * parent, char const * const title);






lv_obj_t * lv_menu_cont_create(lv_obj_t * parent);






lv_obj_t * lv_menu_section_create(lv_obj_t * parent);






lv_obj_t * lv_menu_separator_create(lv_obj_t * parent);
# 155 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/menu/lv_menu.h"
void lv_menu_set_page(lv_obj_t * obj, lv_obj_t * page);






void lv_menu_set_page_title(lv_obj_t * page, char const * const title);







void lv_menu_set_page_title_static(lv_obj_t * page, char const * const title);






void lv_menu_set_sidebar_page(lv_obj_t * obj, lv_obj_t * page);






void lv_menu_set_mode_header(lv_obj_t * obj, lv_menu_mode_header_t mode);






void lv_menu_set_mode_root_back_button(lv_obj_t * obj, lv_menu_mode_root_back_button_t mode);







void lv_menu_set_load_page_event(lv_obj_t * menu, lv_obj_t * obj, lv_obj_t * page);
# 209 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/menu/lv_menu.h"
lv_obj_t * lv_menu_get_cur_main_page(lv_obj_t * obj);






lv_obj_t * lv_menu_get_cur_sidebar_page(lv_obj_t * obj);






lv_obj_t * lv_menu_get_main_header(lv_obj_t * obj);






lv_obj_t * lv_menu_get_main_header_back_button(lv_obj_t * obj);






lv_obj_t * lv_menu_get_sidebar_header(lv_obj_t * obj);






lv_obj_t * lv_menu_get_sidebar_header_back_button(lv_obj_t * obj);







bool lv_menu_back_button_is_root(lv_obj_t * menu, lv_obj_t * obj);





void lv_menu_clear_history(lv_obj_t * obj);
# 69 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/msgbox/lv_msgbox.h" 1
# 37 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/msgbox/lv_msgbox.h"
typedef struct {
    lv_obj_t obj;
    lv_obj_t * header;
    lv_obj_t * content;
    lv_obj_t * footer;
    lv_obj_t * title;
} lv_msgbox_t;

                         extern const lv_obj_class_t lv_msgbox_class;
                         extern const lv_obj_class_t lv_msgbox_header_class;
                         extern const lv_obj_class_t lv_msgbox_content_class;
                         extern const lv_obj_class_t lv_msgbox_footer_class;
                         extern const lv_obj_class_t lv_msgbox_header_button_class;
                         extern const lv_obj_class_t lv_msgbox_footer_button_class;
                         extern const lv_obj_class_t lv_msgbox_backdrop_class;
# 62 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/msgbox/lv_msgbox.h"
lv_obj_t * lv_msgbox_create(lv_obj_t * parent);







lv_obj_t * lv_msgbox_add_title(lv_obj_t * obj, const char * title);







lv_obj_t * lv_msgbox_add_header_button(lv_obj_t * obj, const void * icon);







lv_obj_t * lv_msgbox_add_text(lv_obj_t * obj, const char * text);







lv_obj_t * lv_msgbox_add_footer_button(lv_obj_t * obj, const char * text);






lv_obj_t * lv_msgbox_add_close_button(lv_obj_t * obj);






lv_obj_t * lv_msgbox_get_header(lv_obj_t * obj);






lv_obj_t * lv_msgbox_get_footer(lv_obj_t * obj);






lv_obj_t * lv_msgbox_get_content(lv_obj_t * obj);






lv_obj_t * lv_msgbox_get_title(lv_obj_t * obj);





void lv_msgbox_close(lv_obj_t * mbox);





void lv_msgbox_close_async(lv_obj_t * mbox);
# 70 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/roller/lv_roller.h" 1
# 36 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/roller/lv_roller.h"
enum _lv_roller_mode_t {
    LV_ROLLER_MODE_NORMAL,
    LV_ROLLER_MODE_INFINITE,
};




typedef uint8_t lv_roller_mode_t;


typedef struct {
    lv_obj_t obj;
    uint32_t option_cnt;
    uint32_t sel_opt_id;
    uint32_t sel_opt_id_ori;
    uint32_t inf_page_cnt;
    lv_roller_mode_t mode : 1;
    uint32_t moved : 1;
} lv_roller_t;

                         extern const lv_obj_class_t lv_roller_class;
# 68 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/roller/lv_roller.h"
lv_obj_t * lv_roller_create(lv_obj_t * parent);
# 80 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/roller/lv_roller.h"
void lv_roller_set_options(lv_obj_t * obj, const char * options, lv_roller_mode_t mode);







void lv_roller_set_selected(lv_obj_t * obj, uint32_t sel_opt, lv_anim_enable_t anim);






void lv_roller_set_visible_row_count(lv_obj_t * obj, uint32_t row_cnt);
# 106 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/roller/lv_roller.h"
uint32_t lv_roller_get_selected(const lv_obj_t * obj);







void lv_roller_get_selected_str(const lv_obj_t * obj, char * buf, uint32_t buf_size);






const char * lv_roller_get_options(const lv_obj_t * obj);






uint32_t lv_roller_get_option_count(const lv_obj_t * obj);
# 71 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
enum {ENUM_LV_SCALE_TOTAL_TICK_COUNT_DEFAULT = (11U)};



enum {ENUM_LV_SCALE_MAJOR_TICK_EVERY_DEFAULT = (5U)};



enum {ENUM_LV_SCALE_LABEL_ENABLED_DEFAULT = (1U)};
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
enum {
    LV_SCALE_MODE_HORIZONTAL_TOP = 0x00U,
    LV_SCALE_MODE_HORIZONTAL_BOTTOM = 0x01U,
    LV_SCALE_MODE_VERTICAL_LEFT = 0x02U,
    LV_SCALE_MODE_VERTICAL_RIGHT = 0x04U,
    LV_SCALE_MODE_ROUND_INNER = 0x08U,
    LV_SCALE_MODE_ROUND_OUTER = 0x10U,
    _LV_SCALE_MODE_LAST
};
typedef uint32_t lv_scale_mode_t;

typedef struct {
    lv_style_t * main_style;
    lv_style_t * indicator_style;
    lv_style_t * items_style;
    int32_t minor_range;
    int32_t major_range;
    uint32_t first_tick_idx_in_section;
    uint32_t last_tick_idx_in_section;
    uint32_t first_tick_idx_is_major;
    uint32_t last_tick_idx_is_major;
    int32_t first_tick_in_section_width;
    int32_t last_tick_in_section_width;
    lv_point_t first_tick_in_section;
    lv_point_t last_tick_in_section;
} lv_scale_section_t;

typedef struct {
    lv_obj_t obj;
    lv_ll_t section_ll;
    const char ** txt_src;
    lv_scale_mode_t mode;
    int32_t range_min;
    int32_t range_max;
    uint32_t total_tick_count : 15;
    uint32_t major_tick_every : 15;
    uint32_t label_enabled : 1;
    uint32_t post_draw : 1;

    uint32_t angle_range;
    int32_t rotation;

    int32_t custom_label_cnt;
    int32_t last_tick_width;
    int32_t first_tick_width;
} lv_scale_t;

                         extern const lv_obj_class_t lv_scale_class;
# 105 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
lv_obj_t * lv_scale_create(lv_obj_t * parent);
# 120 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
void lv_scale_set_mode(lv_obj_t * obj, lv_scale_mode_t mode);






void lv_scale_set_total_tick_count(lv_obj_t * obj, uint32_t total_tick_count);






void lv_scale_set_major_tick_every(lv_obj_t * obj, uint32_t major_tick_every);






void lv_scale_set_label_show(lv_obj_t * obj, bool show_label);







void lv_scale_set_range(lv_obj_t * obj, int32_t min, int32_t max);






void lv_scale_set_angle_range(lv_obj_t * obj, uint32_t angle_range);






void lv_scale_set_rotation(lv_obj_t * obj, int32_t rotation);
# 174 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
void lv_scale_set_line_needle_value(lv_obj_t * obj, lv_obj_t * needle_line, int32_t needle_length,
                                    int32_t value);
# 184 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
void lv_scale_set_image_needle_value(lv_obj_t * obj, lv_obj_t * needle_img, int32_t value);






void lv_scale_set_text_src(lv_obj_t * obj, const char * txt_src[]);






void lv_scale_set_post_draw(lv_obj_t * obj, bool en);






lv_scale_section_t * lv_scale_add_section(lv_obj_t * obj);







void lv_scale_section_set_range(lv_scale_section_t * section, int32_t minor_range, int32_t major_range);







void lv_scale_section_set_style(lv_scale_section_t * section, uint32_t part, lv_style_t * section_part_style);
# 232 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/scale/lv_scale.h"
lv_scale_mode_t lv_scale_get_mode(lv_obj_t * obj);






int32_t lv_scale_get_total_tick_count(lv_obj_t * obj);






int32_t lv_scale_get_major_tick_every(lv_obj_t * obj);






bool lv_scale_get_label_show(lv_obj_t * obj);






uint32_t lv_scale_get_angle_range(lv_obj_t * obj);






int32_t lv_scale_get_range_min_value(lv_obj_t * obj);






int32_t lv_scale_get_range_max_value(lv_obj_t * obj);
# 72 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/slider/lv_slider.h" 1
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/slider/lv_slider.h"
enum _lv_slider_mode_t {
    LV_SLIDER_MODE_NORMAL = LV_BAR_MODE_NORMAL,
    LV_SLIDER_MODE_SYMMETRICAL = LV_BAR_MODE_SYMMETRICAL,
    LV_SLIDER_MODE_RANGE = LV_BAR_MODE_RANGE
};




typedef uint8_t lv_slider_mode_t;


typedef struct {
    lv_bar_t bar;
    lv_area_t left_knob_area;
    lv_area_t right_knob_area;
    lv_point_t pressed_point;
    int32_t * value_to_set;
    uint8_t dragging : 1;
    uint8_t left_knob_focus : 1;
} lv_slider_t;

                         extern const lv_obj_class_t lv_slider_class;
# 65 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/slider/lv_slider.h"
lv_obj_t * lv_slider_create(lv_obj_t * parent);
# 77 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/slider/lv_slider.h"
static inline void lv_slider_set_value(lv_obj_t * obj, int32_t value, lv_anim_enable_t anim)
{
    lv_bar_set_value(obj, value, anim);
}







static inline void lv_slider_set_left_value(lv_obj_t * obj, int32_t value, lv_anim_enable_t anim)
{
    lv_bar_set_start_value(obj, value, anim);
}







static inline void lv_slider_set_range(lv_obj_t * obj, int32_t min, int32_t max)
{
    lv_bar_set_range(obj, min, max);
}






static inline void lv_slider_set_mode(lv_obj_t * obj, lv_slider_mode_t mode)
{
    lv_bar_set_mode(obj, (lv_bar_mode_t)mode);
}
# 123 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/slider/lv_slider.h"
static inline int32_t lv_slider_get_value(const lv_obj_t * obj)
{
    return lv_bar_get_value(obj);
}






static inline int32_t lv_slider_get_left_value(const lv_obj_t * obj)
{
    return lv_bar_get_start_value(obj);
}






static inline int32_t lv_slider_get_min_value(const lv_obj_t * obj)
{
    return lv_bar_get_min_value(obj);
}






static inline int32_t lv_slider_get_max_value(const lv_obj_t * obj)
{
    return lv_bar_get_max_value(obj);
}






bool lv_slider_is_dragged(const lv_obj_t * obj);






static inline lv_slider_mode_t lv_slider_get_mode(lv_obj_t * slider)
{
    lv_bar_mode_t mode = lv_bar_get_mode(slider);
    if(mode == LV_BAR_MODE_SYMMETRICAL) return LV_SLIDER_MODE_SYMMETRICAL;
    else if(mode == LV_BAR_MODE_RANGE) return LV_SLIDER_MODE_RANGE;
    else return LV_SLIDER_MODE_NORMAL;
}






static inline bool lv_slider_is_symmetrical(lv_obj_t * obj)
{
    return lv_bar_is_symmetrical(obj);
}
# 73 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
enum _lv_span_overflow_t {
    LV_SPAN_OVERFLOW_CLIP,
    LV_SPAN_OVERFLOW_ELLIPSIS,
    _LV_SPAN_OVERFLOW_LAST,
};




typedef uint32_t lv_span_overflow_t;


enum _lv_span_mode_t {
    LV_SPAN_MODE_FIXED,
    LV_SPAN_MODE_EXPAND,
    LV_SPAN_MODE_BREAK,
    _LV_SPAN_MODE_LAST
};




typedef uint32_t lv_span_mode_t;


typedef struct {
    char * txt;
    lv_obj_t * spangroup;
    lv_style_t style;
    uint32_t static_flag : 1;
} lv_span_t;


typedef struct {
    lv_obj_t obj;
    int32_t lines;
    int32_t indent;
    int32_t cache_w;
    int32_t cache_h;
    lv_ll_t child_ll;
    uint32_t mode : 2;
    uint32_t overflow : 1;
    uint32_t refresh : 1;
} lv_spangroup_t;

                         extern const lv_obj_class_t lv_spangroup_class;





void lv_span_stack_init(void);
void lv_span_stack_deinit(void);






lv_obj_t * lv_spangroup_create(lv_obj_t * parent);






lv_span_t * lv_spangroup_new_span(lv_obj_t * obj);






void lv_spangroup_delete_span(lv_obj_t * obj, lv_span_t * span);
# 114 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
void lv_span_set_text(lv_span_t * span, const char * text);







void lv_span_set_text_static(lv_span_t * span, const char * text);






void lv_spangroup_set_align(lv_obj_t * obj, lv_text_align_t align);






void lv_spangroup_set_overflow(lv_obj_t * obj, lv_span_overflow_t overflow);






void lv_spangroup_set_indent(lv_obj_t * obj, int32_t indent);






void lv_spangroup_set_mode(lv_obj_t * obj, lv_span_mode_t mode);






void lv_spangroup_set_max_lines(lv_obj_t * obj, int32_t lines);
# 175 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
lv_span_t * lv_spangroup_get_child(const lv_obj_t * obj, int32_t id);






uint32_t lv_spangroup_get_span_count(const lv_obj_t * obj);






lv_text_align_t lv_spangroup_get_align(lv_obj_t * obj);






lv_span_overflow_t lv_spangroup_get_overflow(lv_obj_t * obj);






int32_t lv_spangroup_get_indent(lv_obj_t * obj);





lv_span_mode_t lv_spangroup_get_mode(lv_obj_t * obj);






int32_t lv_spangroup_get_max_lines(lv_obj_t * obj);





int32_t lv_spangroup_get_max_line_height(lv_obj_t * obj);
# 231 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
uint32_t lv_spangroup_get_expand_width(lv_obj_t * obj, uint32_t max_width);







int32_t lv_spangroup_get_expand_height(lv_obj_t * obj, int32_t width);
# 249 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/span/lv_span.h"
void lv_spangroup_refr_mode(lv_obj_t * obj);
# 74 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
enum {ENUM_LV_TEXTAREA_CURSOR_LAST = (0x7FFF)};






typedef struct {
    lv_obj_t obj;
    lv_obj_t * label;
    char * placeholder_txt;
    char * pwd_tmp;
    char * pwd_bullet;
    const char * accepted_chars;
    uint32_t max_length;
    uint32_t pwd_show_time;
    struct {
        int32_t valid_x;

        uint32_t pos;

        lv_area_t area;
        uint32_t txt_byte_pos;
        uint8_t show : 1;
        uint8_t click_pos : 1;
    } cursor;

    uint32_t sel_start;
    uint32_t sel_end;
    uint8_t text_sel_in_prog : 1;
    uint8_t text_sel_en : 1;

    uint8_t pwd_mode : 1;
    uint8_t one_line : 1;
} lv_textarea_t;

                         extern const lv_obj_class_t lv_textarea_class;

enum {
    LV_PART_TEXTAREA_PLACEHOLDER = LV_PART_CUSTOM_FIRST,
};
# 81 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
lv_obj_t * lv_textarea_create(lv_obj_t * parent);
# 93 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_add_char(lv_obj_t * obj, uint32_t c);






void lv_textarea_add_text(lv_obj_t * obj, const char * txt);





void lv_textarea_delete_char(lv_obj_t * obj);





void lv_textarea_delete_char_forward(lv_obj_t * obj);
# 123 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_set_text(lv_obj_t * obj, const char * txt);






void lv_textarea_set_placeholder_text(lv_obj_t * obj, const char * txt);
# 139 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_set_cursor_pos(lv_obj_t * obj, int32_t pos);






void lv_textarea_set_cursor_click_pos(lv_obj_t * obj, bool en);






void lv_textarea_set_password_mode(lv_obj_t * obj, bool en);






void lv_textarea_set_password_bullet(lv_obj_t * obj, const char * bullet);






void lv_textarea_set_one_line(lv_obj_t * obj, bool en);






void lv_textarea_set_accepted_chars(lv_obj_t * obj, const char * list);






void lv_textarea_set_max_length(lv_obj_t * obj, uint32_t num);
# 190 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_set_insert_replace(lv_obj_t * obj, const char * txt);






void lv_textarea_set_text_selection(lv_obj_t * obj, bool en);






void lv_textarea_set_password_show_time(lv_obj_t * obj, uint32_t time);
# 214 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_set_align(lv_obj_t * obj, lv_text_align_t align);
# 225 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
const char * lv_textarea_get_text(const lv_obj_t * obj);






const char * lv_textarea_get_placeholder_text(lv_obj_t * obj);






lv_obj_t * lv_textarea_get_label(const lv_obj_t * obj);






uint32_t lv_textarea_get_cursor_pos(const lv_obj_t * obj);






bool lv_textarea_get_cursor_click_pos(lv_obj_t * obj);






bool lv_textarea_get_password_mode(const lv_obj_t * obj);






const char * lv_textarea_get_password_bullet(lv_obj_t * obj);






bool lv_textarea_get_one_line(const lv_obj_t * obj);






const char * lv_textarea_get_accepted_chars(lv_obj_t * obj);






uint32_t lv_textarea_get_max_length(lv_obj_t * obj);






bool lv_textarea_text_is_selected(const lv_obj_t * obj);






bool lv_textarea_get_text_selection(lv_obj_t * obj);






uint32_t lv_textarea_get_password_show_time(lv_obj_t * obj);






uint32_t lv_textarea_get_current_char(lv_obj_t * obj);
# 326 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/../textarea/lv_textarea.h"
void lv_textarea_clear_selection(lv_obj_t * obj);





void lv_textarea_cursor_right(lv_obj_t * obj);





void lv_textarea_cursor_left(lv_obj_t * obj);





void lv_textarea_cursor_down(lv_obj_t * obj);





void lv_textarea_cursor_up(lv_obj_t * obj);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
typedef struct {
    lv_textarea_t ta;

    int32_t value;
    int32_t range_max;
    int32_t range_min;
    int32_t step;
    uint32_t digit_count : 4;
    uint32_t dec_point_pos : 4;
    uint32_t rollover : 1;
    uint32_t digit_step_dir : 2;
} lv_spinbox_t;

                         extern const lv_obj_class_t lv_spinbox_class;
# 59 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
lv_obj_t * lv_spinbox_create(lv_obj_t * parent);
# 70 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
void lv_spinbox_set_value(lv_obj_t * obj, int32_t v);






void lv_spinbox_set_rollover(lv_obj_t * obj, bool rollover);
# 86 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
void lv_spinbox_set_digit_format(lv_obj_t * obj, uint32_t digit_count, uint32_t sep_pos);






void lv_spinbox_set_step(lv_obj_t * obj, uint32_t step);







void lv_spinbox_set_range(lv_obj_t * obj, int32_t range_min, int32_t range_max);






void lv_spinbox_set_cursor_pos(lv_obj_t * obj, uint32_t pos);






void lv_spinbox_set_digit_step_direction(lv_obj_t * obj, lv_dir_t direction);
# 125 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
bool lv_spinbox_get_rollover(lv_obj_t * obj);






int32_t lv_spinbox_get_value(lv_obj_t * obj);






int32_t lv_spinbox_get_step(lv_obj_t * obj);
# 149 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinbox/lv_spinbox.h"
void lv_spinbox_step_next(lv_obj_t * obj);





void lv_spinbox_step_prev(lv_obj_t * obj);





void lv_spinbox_increment(lv_obj_t * obj);





void lv_spinbox_decrement(lv_obj_t * obj);
# 75 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinner/lv_spinner.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinner/lv_spinner.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinner/lv_spinner.h" 2
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinner/lv_spinner.h"
                         extern const lv_obj_class_t lv_spinner_class;
# 43 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/spinner/lv_spinner.h"
lv_obj_t * lv_spinner_create(lv_obj_t * parent);







void lv_spinner_set_anim_params(lv_obj_t * obj, uint32_t t, uint32_t angle);
# 76 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/switch/lv_switch.h" 1
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/switch/lv_switch.h"
typedef struct {
    lv_obj_t obj;
    int32_t anim_state;
} lv_switch_t;

                         extern const lv_obj_class_t lv_switch_class;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/switch/lv_switch.h"
lv_obj_t * lv_switch_create(lv_obj_t * parent);
# 77 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
enum {ENUM_LV_TABLE_CELL_NONE = 0XFFFF};





enum _lv_table_cell_ctrl_t {
    LV_TABLE_CELL_CTRL_MERGE_RIGHT = 1 << 0,
    LV_TABLE_CELL_CTRL_TEXT_CROP = 1 << 1,
    LV_TABLE_CELL_CTRL_CUSTOM_1 = 1 << 4,
    LV_TABLE_CELL_CTRL_CUSTOM_2 = 1 << 5,
    LV_TABLE_CELL_CTRL_CUSTOM_3 = 1 << 6,
    LV_TABLE_CELL_CTRL_CUSTOM_4 = 1 << 7,
};




typedef uint32_t lv_table_cell_ctrl_t;



typedef struct {
    lv_table_cell_ctrl_t ctrl;
    void * user_data;
    char txt[];
} lv_table_cell_t;


typedef struct {
    lv_obj_t obj;
    uint32_t col_cnt;
    uint32_t row_cnt;
    lv_table_cell_t ** cell_data;
    int32_t * row_h;
    int32_t * col_w;
    uint32_t col_act;
    uint32_t row_act;
} lv_table_t;

                         extern const lv_obj_class_t lv_table_class;
# 81 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
lv_obj_t * lv_table_create(lv_obj_t * parent);
# 95 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
void lv_table_set_cell_value(lv_obj_t * obj, uint32_t row, uint32_t col, const char * txt);
# 105 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
void lv_table_set_cell_value_fmt(lv_obj_t * obj, uint32_t row, uint32_t col, const char * fmt,
                                 ...) ;






void lv_table_set_row_count(lv_obj_t * obj, uint32_t row_cnt);






void lv_table_set_column_count(lv_obj_t * obj, uint32_t col_cnt);







void lv_table_set_column_width(lv_obj_t * obj, uint32_t col_id, int32_t w);
# 137 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
void lv_table_add_cell_ctrl(lv_obj_t * obj, uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
# 146 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
void lv_table_clear_cell_ctrl(lv_obj_t * obj, uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
# 158 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
void lv_table_set_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col, void * user_data);
# 171 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
const char * lv_table_get_cell_value(lv_obj_t * obj, uint32_t row, uint32_t col);






uint32_t lv_table_get_row_count(lv_obj_t * obj);






uint32_t lv_table_get_column_count(lv_obj_t * obj);







int32_t lv_table_get_column_width(lv_obj_t * obj, uint32_t col);
# 203 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/table/lv_table.h"
bool lv_table_has_cell_ctrl(lv_obj_t * obj, uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);







void lv_table_get_selected_cell(lv_obj_t * obj, uint32_t * row, uint32_t * col);







void * lv_table_get_cell_user_data(lv_obj_t * obj, uint16_t row, uint16_t col);
# 78 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tabview/lv_tabview.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tabview/lv_tabview.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tabview/lv_tabview.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tabview/lv_tabview.h"
typedef struct {
    lv_obj_t obj;
    uint32_t tab_cur;
    lv_dir_t tab_pos;
} lv_tabview_t;

                         extern const lv_obj_class_t lv_tabview_class;
# 45 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tabview/lv_tabview.h"
lv_obj_t * lv_tabview_create(lv_obj_t * parent);







lv_obj_t * lv_tabview_add_tab(lv_obj_t * obj, const char * name);







void lv_tabview_rename_tab(lv_obj_t * obj, uint32_t idx, const char * new_name);







void lv_tabview_set_active(lv_obj_t * obj, uint32_t idx, lv_anim_enable_t anim_en);






void lv_tabview_set_tab_bar_position(lv_obj_t * obj, lv_dir_t dir);






void lv_tabview_set_tab_bar_size(lv_obj_t * obj, int32_t size);






uint32_t lv_tabview_get_tab_count(lv_obj_t * obj);






uint32_t lv_tabview_get_tab_active(lv_obj_t * obj);






lv_obj_t * lv_tabview_get_content(lv_obj_t * obj);






lv_obj_t * lv_tabview_get_tab_bar(lv_obj_t * obj);
# 79 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tileview/lv_tileview.h" 1
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tileview/lv_tileview.h"
typedef struct {
    lv_obj_t obj;
    lv_obj_t * tile_act;
} lv_tileview_t;

typedef struct {
    lv_obj_t obj;
    lv_dir_t dir;
} lv_tileview_tile_t;

                         extern const lv_obj_class_t lv_tileview_class;
                         extern const lv_obj_class_t lv_tileview_tile_class;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/tileview/lv_tileview.h"
lv_obj_t * lv_tileview_create(lv_obj_t * parent);

lv_obj_t * lv_tileview_add_tile(lv_obj_t * tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir);

void lv_tileview_set_tile(lv_obj_t * tv, lv_obj_t * tile_obj, lv_anim_enable_t anim_en);
void lv_tileview_set_tile_by_index(lv_obj_t * tv, uint32_t col_id, uint32_t row_id, lv_anim_enable_t anim_en);

lv_obj_t * lv_tileview_get_tile_active(lv_obj_t * obj);
# 81 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/win/lv_win.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/win/lv_win.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/widgets/win/lv_win.h" 2








typedef struct {
    lv_obj_t obj;
} lv_win_t;

                         extern const lv_obj_class_t lv_win_class;





lv_obj_t * lv_win_create(lv_obj_t * parent);

lv_obj_t * lv_win_add_title(lv_obj_t * win, const char * txt);
lv_obj_t * lv_win_add_button(lv_obj_t * win, const void * icon, int32_t btn_w);

lv_obj_t * lv_win_get_header(lv_obj_t * win);
lv_obj_t * lv_win_get_content(lv_obj_t * win);
# 82 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h" 2
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h" 2
# 40 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h"
lv_image_dsc_t * lv_snapshot_take(lv_obj_t * obj, lv_color_format_t cf);






void lv_snapshot_free(lv_image_dsc_t * dsc);







uint32_t lv_snapshot_buf_size_needed(lv_obj_t * obj, lv_color_format_t cf);
# 66 "../../micropython/../lv_binding_micropython/lvgl/src/others/snapshot/lv_snapshot.h"
lv_result_t lv_snapshot_take_to_buf(lv_obj_t * obj, lv_color_format_t cf, lv_image_dsc_t * dsc, void * buf,
                                    uint32_t buf_size);
# 84 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/lv_sysmon.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/lv_sysmon.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h" 1
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
struct _lv_observer_t;
typedef struct _lv_observer_t lv_observer_t;

typedef enum {
    LV_SUBJECT_TYPE_INVALID = 0,
    LV_SUBJECT_TYPE_NONE = 1,
    LV_SUBJECT_TYPE_INT = 2,
    LV_SUBJECT_TYPE_POINTER = 3,
    LV_SUBJECT_TYPE_COLOR = 4,
    LV_SUBJECT_TYPE_GROUP = 5,
    LV_SUBJECT_TYPE_STRING = 6,
} lv_subject_type_t;




typedef union {
    int32_t num;
    const void * pointer;
    lv_color_t color;
} lv_subject_value_t;




typedef struct {
    lv_ll_t subs_ll;
    uint32_t type : 4;
    uint32_t size : 28;
    lv_subject_value_t value;
    lv_subject_value_t prev_value;
    uint32_t notify_restart_query : 1;
    void * user_data;
} lv_subject_t;






typedef void (*lv_observer_cb_t)(lv_observer_t * observer, lv_subject_t * subject);




struct _lv_observer_t {
    lv_subject_t * subject;
    lv_observer_cb_t cb;
    void * target;
    void * user_data;
    uint32_t auto_free_user_data : 1;
    uint32_t notified : 1;
};
# 91 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
void lv_subject_init_int(lv_subject_t * subject, int32_t value);






void lv_subject_set_int(lv_subject_t * subject, int32_t value);






int32_t lv_subject_get_int(lv_subject_t * subject);






int32_t lv_subject_get_previous_int(lv_subject_t * subject);
# 123 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
void lv_subject_init_string(lv_subject_t * subject, char * buf, char * prev_buf, size_t size, const char * value);






void lv_subject_copy_string(lv_subject_t * subject, const char * buf);






const char * lv_subject_get_string(lv_subject_t * subject);
# 146 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
const char * lv_subject_get_previous_string(lv_subject_t * subject);






void lv_subject_init_pointer(lv_subject_t * subject, void * value);






void lv_subject_set_pointer(lv_subject_t * subject, void * ptr);






const void * lv_subject_get_pointer(lv_subject_t * subject);






const void * lv_subject_get_previous_pointer(lv_subject_t * subject);






void lv_subject_init_color(lv_subject_t * subject, lv_color_t color);






void lv_subject_set_color(lv_subject_t * subject, lv_color_t color);






lv_color_t lv_subject_get_color(lv_subject_t * subject);






lv_color_t lv_subject_get_previous_color(lv_subject_t * subject);







void lv_subject_init_group(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len);







lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index);
# 227 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, void * user_data);
# 238 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            void * user_data);
# 249 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t cb, void * target,
                                                    void * user_data);





void lv_observer_remove(lv_observer_t * observer);






void lv_subject_remove_all_obj(lv_subject_t * subject, lv_obj_t * obj);






void * lv_observer_get_target(lv_observer_t * observer);
# 279 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
static inline lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    return (lv_obj_t *)lv_observer_get_target(observer);
}





void lv_subject_notify(lv_subject_t * subject);
# 298 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value);
# 308 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag,
                                           int32_t ref_value);
# 319 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value);
# 329 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state,
                                            int32_t ref_value);
# 339 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_button_bind_checked(lv_obj_t * obj, lv_subject_t * subject);
# 353 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_label_bind_text(lv_obj_t * obj, lv_subject_t * subject, const char * fmt);
# 363 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject);
# 373 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject);
# 383 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject);
# 393 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/../../others/observer/lv_observer.h"
lv_observer_t * lv_dropdown_bind_value(lv_obj_t * obj, lv_subject_t * subject);
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/others/sysmon/lv_sysmon.h" 2
# 85 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/monkey/lv_monkey.h" 1
# 15 "../../micropython/../lv_binding_micropython/lvgl/src/others/monkey/lv_monkey.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/others/monkey/lv_monkey.h" 2
# 86 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/gridnav/lv_gridnav.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/others/gridnav/lv_gridnav.h"
typedef int _keep_pedantic_happy;
# 87 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/fragment/lv_fragment.h" 1
# 88 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/imgfont/lv_imgfont.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/others/imgfont/lv_imgfont.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/others/imgfont/lv_imgfont.h" 2
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/others/imgfont/lv_imgfont.h"
typedef const void * (*lv_imgfont_get_path_cb_t)(const lv_font_t * font,
                                                 uint32_t unicode, uint32_t unicode_next,
                                                 int32_t * offset_y, void * user_data);
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/others/imgfont/lv_imgfont.h"
lv_font_t * lv_imgfont_create(uint16_t height, lv_imgfont_get_path_cb_t path_cb, void * user_data);





void lv_imgfont_destroy(lv_font_t * font);
# 89 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/ime/lv_ime_pinyin.h" 1
# 15 "../../micropython/../lv_binding_micropython/lvgl/src/others/ime/lv_ime_pinyin.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/others/ime/lv_ime_pinyin.h" 2
# 91 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/others/file_explorer/lv_file_explorer.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/others/file_explorer/lv_file_explorer.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/others/file_explorer/lv_file_explorer.h" 2
# 92 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/barcode/lv_barcode.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/barcode/lv_barcode.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/barcode/lv_barcode.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/libs/barcode/lv_barcode.h"
typedef struct {
    lv_canvas_t canvas;
    lv_color_t dark_color;
    lv_color_t light_color;
    uint16_t scale;
    lv_dir_t direction;
} lv_barcode_t;

                         extern const lv_obj_class_t lv_barcode_class;
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/libs/barcode/lv_barcode.h"
lv_obj_t * lv_barcode_create(lv_obj_t * parent);






void lv_barcode_set_dark_color(lv_obj_t * obj, lv_color_t color);






void lv_barcode_set_light_color(lv_obj_t * obj, lv_color_t color);






void lv_barcode_set_scale(lv_obj_t * obj, uint16_t scale);






void lv_barcode_set_direction(lv_obj_t * obj, lv_dir_t direction);







lv_result_t lv_barcode_update(lv_obj_t * obj, const char * data);






lv_color_t lv_barcode_get_dark_color(lv_obj_t * obj);






lv_color_t lv_barcode_get_light_color(lv_obj_t * obj);






uint16_t lv_barcode_get_scale(lv_obj_t * obj);
# 94 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/bin_decoder/lv_bin_decoder.h" 1
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/libs/bin_decoder/lv_bin_decoder.h"
void lv_bin_decoder_init(void);
# 42 "../../micropython/../lv_binding_micropython/lvgl/src/libs/bin_decoder/lv_bin_decoder.h"
lv_result_t lv_bin_decoder_info(lv_image_decoder_t * decoder, const void * src, lv_image_header_t * header);

lv_result_t lv_bin_decoder_get_area(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc,
                                    const lv_area_t * full_area, lv_area_t * decoded_area);







lv_result_t lv_bin_decoder_open(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);






void lv_bin_decoder_close(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);
# 95 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/bmp/lv_bmp.h" 1
# 96 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rle/lv_rle.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rle/lv_rle.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rle/lv_rle.h" 2
# 97 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/fsdrv/lv_fsdrv.h" 1
# 47 "../../micropython/../lv_binding_micropython/lvgl/src/libs/fsdrv/lv_fsdrv.h"
void lv_fs_memfs_init(void);
# 98 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/lodepng/lv_lodepng.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/libs/lodepng/lv_lodepng.h"
void lv_lodepng_init(void);

void lv_lodepng_deinit(void);
# 99 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/libpng/lv_libpng.h" 1
# 100 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h" 2


# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/gifdec.h" 1



# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdint.h" 1
# 5 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/gifdec.h" 2




typedef struct _gd_Palette {
    int size;
    uint8_t colors[0x100 * 3];
} gd_Palette;

typedef struct _gd_GCE {
    uint16_t delay;
    uint8_t tindex;
    uint8_t disposal;
    int input;
    int transparency;
} gd_GCE;



typedef struct _gd_GIF {
    lv_fs_file_t fd;
    const char * data;
    uint8_t is_file;
    uint32_t f_rw_p;
    int32_t anim_start;
    uint16_t width, height;
    uint16_t depth;
    int32_t loop_count;
    gd_GCE gce;
    gd_Palette * palette;
    gd_Palette lct, gct;
    void (*plain_text)(
        struct _gd_GIF * gif, uint16_t tx, uint16_t ty,
        uint16_t tw, uint16_t th, uint8_t cw, uint8_t ch,
        uint8_t fg, uint8_t bg
    );
    void (*comment)(struct _gd_GIF * gif);
    void (*application)(struct _gd_GIF * gif, char id[8], char auth[3]);
    uint16_t fx, fy, fw, fh;
    uint8_t bgindex;
    uint8_t * canvas, * frame;



} gd_GIF;

gd_GIF * gd_open_gif_file(const char * fname);

gd_GIF * gd_open_gif_data(const void * data);

void gd_render_frame(gd_GIF * gif, uint8_t * buffer);

int gd_get_frame(gd_GIF * gif);
void gd_rewind(gd_GIF * gif);
void gd_close_gif(gd_GIF * gif);
# 21 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h" 2
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h"
typedef struct {
    lv_image_t img;
    gd_GIF * gif;
    lv_timer_t * timer;
    lv_image_dsc_t imgdsc;
    uint32_t last_call;
} lv_gif_t;

                         extern const lv_obj_class_t lv_gif_class;
# 49 "../../micropython/../lv_binding_micropython/lvgl/src/libs/gif/lv_gif.h"
lv_obj_t * lv_gif_create(lv_obj_t * parent);







void lv_gif_set_src(lv_obj_t * obj, const void * src);





void lv_gif_restart(lv_obj_t * obj);





void lv_gif_pause(lv_obj_t * obj);





void lv_gif_resume(lv_obj_t * obj);
# 101 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h" 2
# 28 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h"
typedef struct {
    lv_canvas_t canvas;
    lv_color_t dark_color;
    lv_color_t light_color;
} lv_qrcode_t;

                         extern const lv_obj_class_t lv_qrcode_class;
# 45 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h"
lv_obj_t * lv_qrcode_create(lv_obj_t * parent);






void lv_qrcode_set_size(lv_obj_t * obj, int32_t size);






void lv_qrcode_set_dark_color(lv_obj_t * obj, lv_color_t color);






void lv_qrcode_set_light_color(lv_obj_t * obj, lv_color_t color);
# 75 "../../micropython/../lv_binding_micropython/lvgl/src/libs/qrcode/lv_qrcode.h"
lv_result_t lv_qrcode_update(lv_obj_t * obj, const void * data, uint32_t data_len);
# 102 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/tjpgd/lv_tjpgd.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/libs/tjpgd/lv_tjpgd.h"
void lv_tjpgd_init(void);

void lv_tjpgd_deinit(void);
# 103 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/libjpeg_turbo/lv_libjpeg_turbo.h" 1
# 104 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/freetype/lv_freetype.h" 1
# 15 "../../micropython/../lv_binding_micropython/lvgl/src/libs/freetype/lv_freetype.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/freetype/lv_freetype.h" 2
# 105 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rlottie/lv_rlottie.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rlottie/lv_rlottie.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/rlottie/lv_rlottie.h" 2
# 106 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/ffmpeg/lv_ffmpeg.h" 1
# 15 "../../micropython/../lv_binding_micropython/lvgl/src/libs/ffmpeg/lv_ffmpeg.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/ffmpeg/lv_ffmpeg.h" 2
# 107 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/libs/tiny_ttf/lv_tiny_ttf.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/libs/tiny_ttf/lv_tiny_ttf.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/libs/tiny_ttf/lv_tiny_ttf.h" 2
# 108 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2




# 1 "../../micropython/../lv_binding_micropython/lvgl/src/draw/lv_draw_vector.h" 1
# 113 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 1
# 26 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h"
typedef void (*lv_theme_apply_cb_t)(lv_theme_t *, lv_obj_t *);

struct _lv_theme_t {
    lv_theme_apply_cb_t apply_cb;
    lv_theme_t * parent;
    void * user_data;
    lv_display_t * disp;
    lv_color_t color_primary;
    lv_color_t color_secondary;
    const lv_font_t * font_small;
    const lv_font_t * font_normal;
    const lv_font_t * font_large;
    uint32_t flags;
};
# 50 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h"
lv_theme_t * lv_theme_get_from_obj(lv_obj_t * obj);





void lv_theme_apply(lv_obj_t * obj);
# 65 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h"
void lv_theme_set_parent(lv_theme_t * new_theme, lv_theme_t * parent);







void lv_theme_set_apply_cb(lv_theme_t * theme, lv_theme_apply_cb_t apply_cb);






const lv_font_t * lv_theme_get_font_small(lv_obj_t * obj);





const lv_font_t * lv_theme_get_font_normal(lv_obj_t * obj);






const lv_font_t * lv_theme_get_font_large(lv_obj_t * obj);






lv_color_t lv_theme_get_color_primary(lv_obj_t * obj);






lv_color_t lv_theme_get_color_secondary(lv_obj_t * obj);





# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/default/lv_theme_default.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/themes/default/lv_theme_default.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/themes/default/lv_theme_default.h" 2
# 41 "../../micropython/../lv_binding_micropython/lvgl/src/themes/default/lv_theme_default.h"
lv_theme_t * lv_theme_default_init(lv_display_t * disp, lv_color_t color_primary, lv_color_t color_secondary, bool dark,
                                   const lv_font_t * font);





lv_theme_t * lv_theme_default_get(void);





bool lv_theme_default_is_inited(void);




void lv_theme_default_deinit(void);
# 114 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/mono/lv_theme_mono.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/themes/mono/lv_theme_mono.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/themes/mono/lv_theme_mono.h" 2
# 115 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/simple/lv_theme_simple.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/themes/simple/lv_theme_simple.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/themes/simple/lv_theme_simple.h" 2
# 116 "../../micropython/../lv_binding_micropython/lvgl/src/themes/lv_theme.h" 2
# 115 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/sdl/lv_sdl_window.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/sdl/lv_sdl_mouse.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/sdl/lv_sdl_mousewheel.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/sdl/lv_sdl_keyboard.h" 1
# 20 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/x11/lv_x11.h" 1
# 22 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/display/drm/lv_linux_drm.h" 1
# 24 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/display/fb/lv_linux_fbdev.h" 1
# 25 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/display/tft_espi/lv_tft_espi.h" 1
# 27 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/nuttx/lv_nuttx_entry.h" 1
# 29 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/nuttx/lv_nuttx_fbdev.h" 1
# 30 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/nuttx/lv_nuttx_touchscreen.h" 1
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/nuttx/lv_nuttx_lcd.h" 1
# 32 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/nuttx/lv_nuttx_libuv.h" 1
# 33 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/evdev/lv_evdev.h" 1
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/windows/lv_windows_input.h" 1
# 37 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/windows/lv_windows_display.h" 1
# 38 "../../micropython/../lv_binding_micropython/lvgl/src/drivers/lv_drivers.h" 2
# 117 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/lv_api_map.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/lv_api_map.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/lv_api_map.h" 2
# 46 "../../micropython/../lv_binding_micropython/lvgl/src/lv_api_map.h"
typedef int32_t lv_coord_t;
typedef lv_result_t lv_res_t;
typedef lv_image_dsc_t lv_img_dsc_t;
typedef lv_display_t lv_disp_t;
typedef lv_display_rotation_t lv_disp_rotation_t;
typedef lv_display_render_mode_t lv_disp_render_t;
typedef lv_anim_completed_cb_t lv_anim_ready_cb_t;





static inline uint32_t lv_task_handler(void)
{
    return lv_timer_handler();
}







static inline void lv_obj_move_foreground(lv_obj_t * obj)
{
    lv_obj_t * parent = lv_obj_get_parent(obj);
    if(!parent) {
        do {}while(0);
        return;
    }

    lv_obj_move_to_index(obj, lv_obj_get_child_count(parent) - 1);
}







static inline void lv_obj_move_background(lv_obj_t * obj)
{
    lv_obj_move_to_index(obj, 0);
}
# 119 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2

# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h" 1
# 18 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 19 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h" 2




# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h" 1
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
typedef struct {
    lv_draw_unit_t base_unit;
    lv_draw_task_t * task_act;






    uint32_t idx;
} lv_draw_sw_unit_t;
# 62 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
void lv_draw_sw_init(void);




void lv_draw_sw_deinit(void);







void lv_draw_sw_fill(lv_draw_unit_t * draw_unit, const lv_draw_fill_dsc_t * dsc, const lv_area_t * coords);







void lv_draw_sw_border(lv_draw_unit_t * draw_unit, const lv_draw_border_dsc_t * dsc, const lv_area_t * coords);







void lv_draw_sw_box_shadow(lv_draw_unit_t * draw_unit, const lv_draw_box_shadow_dsc_t * dsc, const lv_area_t * coords);







void lv_draw_sw_image(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc,
                      const lv_area_t * coords);







void lv_draw_sw_label(lv_draw_unit_t * draw_unit, const lv_draw_label_dsc_t * dsc, const lv_area_t * coords);







void lv_draw_sw_arc(lv_draw_unit_t * draw_unit, const lv_draw_arc_dsc_t * dsc, const lv_area_t * coords);






void lv_draw_sw_line(lv_draw_unit_t * draw_unit, const lv_draw_line_dsc_t * dsc);







void lv_draw_sw_layer(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc, const lv_area_t * coords);






void lv_draw_sw_triangle(lv_draw_unit_t * draw_unit, const lv_draw_triangle_dsc_t * dsc);







void lv_draw_sw_mask_rect(lv_draw_unit_t * draw_unit, const lv_draw_mask_rect_dsc_t * dsc, const lv_area_t * coords);
# 161 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
void lv_draw_sw_transform(lv_draw_unit_t * draw_unit, const lv_area_t * dest_area, const void * src_buf,
                          int32_t src_w, int32_t src_h, int32_t src_stride,
                          const lv_draw_image_dsc_t * draw_dsc, const lv_draw_image_sup_t * sup, lv_color_format_t cf, void * dest_buf);
# 180 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
void lv_draw_sw_rgb565_swap(void * buf, uint32_t buf_size_px);
# 193 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
void lv_draw_sw_rotate(const void * src, void * dest, int32_t src_width, int32_t src_height, int32_t src_sride,
                       int32_t dest_stride, lv_display_rotation_t rotation, lv_color_format_t color_format);
# 204 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/lv_draw_sw_blend.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/lv_draw_sw_blend.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h" 1
# 16 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stdbool.h" 1
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h" 2
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
enum {
    LV_DRAW_SW_MASK_RES_TRANSP,
    LV_DRAW_SW_MASK_RES_FULL_COVER,
    LV_DRAW_SW_MASK_RES_CHANGED,
    LV_DRAW_SW_MASK_RES_UNKNOWN
};

typedef uint8_t lv_draw_sw_mask_res_t;



enum {
    LV_DRAW_SW_MASK_TYPE_LINE,
    LV_DRAW_SW_MASK_TYPE_ANGLE,
    LV_DRAW_SW_MASK_TYPE_RADIUS,
    LV_DRAW_SW_MASK_TYPE_FADE,
    LV_DRAW_SW_MASK_TYPE_MAP,
};

typedef uint8_t lv_draw_sw_mask_type_t;

enum {
    LV_DRAW_SW_MASK_LINE_SIDE_LEFT = 0,
    LV_DRAW_SW_MASK_LINE_SIDE_RIGHT,
    LV_DRAW_SW_MASK_LINE_SIDE_TOP,
    LV_DRAW_SW_MASK_LINE_SIDE_BOTTOM,
};





typedef lv_draw_sw_mask_res_t (*lv_draw_sw_mask_xcb_t)(lv_opa_t * mask_buf, int32_t abs_x, int32_t abs_y,
                                                       int32_t len,
                                                       void * p);

typedef uint8_t lv_draw_sw_mask_line_side_t;

typedef struct {
    lv_draw_sw_mask_xcb_t cb;
    lv_draw_sw_mask_type_t type;
} _lv_draw_sw_mask_common_dsc_t;

typedef struct {

    _lv_draw_sw_mask_common_dsc_t dsc;

    struct {

        lv_point_t p1;


        lv_point_t p2;


        lv_draw_sw_mask_line_side_t side : 2;
    } cfg;


    lv_point_t origo;


    int32_t xy_steep;


    int32_t yx_steep;


    int32_t steep;


    int32_t spx;


    uint8_t flat : 1;



    uint8_t inv: 1;
} lv_draw_sw_mask_line_param_t;

typedef struct {

    _lv_draw_sw_mask_common_dsc_t dsc;

    struct {
        lv_point_t vertex_p;
        int32_t start_angle;
        int32_t end_angle;
    } cfg;

    lv_draw_sw_mask_line_param_t start_line;
    lv_draw_sw_mask_line_param_t end_line;
    uint16_t delta_deg;
} lv_draw_sw_mask_angle_param_t;

typedef struct {
    uint8_t * buf;
    lv_opa_t * cir_opa;
    uint16_t * x_start_on_y;
    uint16_t * opa_start_on_y;
    int32_t life;
    uint32_t used_cnt;
    int32_t radius;
} _lv_draw_sw_mask_radius_circle_dsc_t;

typedef _lv_draw_sw_mask_radius_circle_dsc_t _lv_draw_sw_mask_radius_circle_dsc_arr_t[4];

typedef struct {

    _lv_draw_sw_mask_common_dsc_t dsc;

    struct {
        lv_area_t rect;
        int32_t radius;

        uint8_t outer: 1;
    } cfg;

    _lv_draw_sw_mask_radius_circle_dsc_t * circle;
} lv_draw_sw_mask_radius_param_t;

typedef struct {

    _lv_draw_sw_mask_common_dsc_t dsc;

    struct {
        lv_area_t coords;
        int32_t y_top;
        int32_t y_bottom;
        lv_opa_t opa_top;
        lv_opa_t opa_bottom;
    } cfg;

} lv_draw_sw_mask_fade_param_t;

typedef struct _lv_draw_sw_mask_map_param_t {

    _lv_draw_sw_mask_common_dsc_t dsc;

    struct {
        lv_area_t coords;
        const lv_opa_t * map;
    } cfg;
} lv_draw_sw_mask_map_param_t;





void lv_draw_sw_mask_init(void);

void lv_draw_sw_mask_deinit(void);
# 203 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
                      lv_draw_sw_mask_res_t lv_draw_sw_mask_apply(void * masks[], lv_opa_t * mask_buf, int32_t abs_x,
                                                                  int32_t abs_y,
                                                                  int32_t len);
# 216 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_free_param(void * p);





void _lv_draw_sw_mask_cleanup(void);
# 235 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_line_points_init(lv_draw_sw_mask_line_param_t * param, int32_t p1x, int32_t p1y,
                                      int32_t p2x,
                                      int32_t p2y, lv_draw_sw_mask_line_side_t side);
# 249 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_line_angle_init(lv_draw_sw_mask_line_param_t * param, int32_t p1x, int32_t py, int16_t angle,
                                     lv_draw_sw_mask_line_side_t side);
# 260 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_angle_init(lv_draw_sw_mask_angle_param_t * param, int32_t vertex_x, int32_t vertex_y,
                                int32_t start_angle, int32_t end_angle);
# 270 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_radius_init(lv_draw_sw_mask_radius_param_t * param, const lv_area_t * rect, int32_t radius,
                                 bool inv);
# 282 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/../lv_draw_sw_mask.h"
void lv_draw_sw_mask_fade_init(lv_draw_sw_mask_fade_param_t * param, const lv_area_t * coords, lv_opa_t opa_top,
                               int32_t y_top,
                               lv_opa_t opa_bottom, int32_t y_bottom);







void lv_draw_sw_mask_map_init(lv_draw_sw_mask_map_param_t * param, const lv_area_t * coords, const lv_opa_t * map);
# 17 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/lv_draw_sw_blend.h" 2
# 31 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/lv_draw_sw_blend.h"
typedef struct {
    const lv_area_t * blend_area;

    const void * src_buf;
    uint32_t src_stride;
    lv_color_format_t src_color_format;
    const lv_area_t * src_area;
    lv_opa_t opa;
    lv_color_t color;
    const lv_opa_t * mask_buf;
    lv_draw_sw_mask_res_t mask_res;
    const lv_area_t * mask_area;
    int32_t mask_stride;
    lv_blend_mode_t blend_mode;
} lv_draw_sw_blend_dsc_t;

typedef struct {
    void * dest_buf;
    int32_t dest_w;
    int32_t dest_h;
    int32_t dest_stride;
    const lv_opa_t * mask_buf;
    int32_t mask_stride;
    lv_color_t color;
    lv_opa_t opa;
} _lv_draw_sw_blend_fill_dsc_t;

typedef struct {
    void * dest_buf;
    int32_t dest_w;
    int32_t dest_h;
    int32_t dest_stride;
    const lv_opa_t * mask_buf;
    int32_t mask_stride;
    const void * src_buf;
    int32_t src_stride;
    lv_color_format_t src_color_format;
    lv_opa_t opa;
    lv_blend_mode_t blend_mode;
} _lv_draw_sw_blend_image_dsc_t;
# 81 "../../micropython/../lv_binding_micropython/lvgl/src/stdlib/../misc/../font/../draw/sw/blend/lv_draw_sw_blend.h"
void lv_draw_sw_blend(lv_draw_unit_t * draw_unit, const lv_draw_sw_blend_dsc_t * dsc);
# 205 "../../micropython/../lv_binding_micropython/lvgl/src/core/../draw/sw/lv_draw_sw.h" 2
# 24 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h" 2
# 34 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
# 1 "../../micropython/../lv_binding_micropython/lvgl/src/core/../stdlib/builtin/lv_tlsf.h" 1
# 44 "../../micropython/../lv_binding_micropython/lvgl/src/core/../stdlib/builtin/lv_tlsf.h"
# 1 "../../micropython/../lv_binding_micropython/pycparser/utils/fake_libc_include/stddef.h" 1
# 45 "../../micropython/../lv_binding_micropython/lvgl/src/core/../stdlib/builtin/lv_tlsf.h" 2
# 55 "../../micropython/../lv_binding_micropython/lvgl/src/core/../stdlib/builtin/lv_tlsf.h"
typedef void * lv_tlsf_t;
typedef void * lv_pool_t;

typedef struct {



    lv_tlsf_t tlsf;
    uint32_t cur_used;
    uint32_t max_used;
    lv_ll_t pool_ll;
} lv_tlsf_state_t;


lv_tlsf_t lv_tlsf_create(void * mem);
lv_tlsf_t lv_tlsf_create_with_pool(void * mem, size_t bytes);
void lv_tlsf_destroy(lv_tlsf_t tlsf);
lv_pool_t lv_tlsf_get_pool(lv_tlsf_t tlsf);


lv_pool_t lv_tlsf_add_pool(lv_tlsf_t tlsf, void * mem, size_t bytes);
void lv_tlsf_remove_pool(lv_tlsf_t tlsf, lv_pool_t pool);


void * lv_tlsf_malloc(lv_tlsf_t tlsf, size_t bytes);
void * lv_tlsf_memalign(lv_tlsf_t tlsf, size_t align, size_t bytes);
void * lv_tlsf_realloc(lv_tlsf_t tlsf, void * ptr, size_t size);
size_t lv_tlsf_free(lv_tlsf_t tlsf, const void * ptr);


size_t lv_tlsf_block_size(void * ptr);


size_t lv_tlsf_size(void);
size_t lv_tlsf_align_size(void);
size_t lv_tlsf_block_size_min(void);
size_t lv_tlsf_block_size_max(void);
size_t lv_tlsf_pool_overhead(void);
size_t lv_tlsf_alloc_overhead(void);


typedef void (*lv_tlsf_walker)(void * ptr, size_t size, int used, void * user);
void lv_tlsf_walk_pool(lv_pool_t pool, lv_tlsf_walker walker, void * user);

int lv_tlsf_check(lv_tlsf_t tlsf);
int lv_tlsf_check_pool(lv_pool_t pool);
# 35 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h" 2
# 53 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
struct _snippet_stack;






typedef struct _lv_global_t {
    bool inited;
    bool deinit_in_progress;

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    uint32_t style_custom_table_size;
    uint32_t style_last_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_table;

    lv_ll_t group_ll;
    lv_group_t * group_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_active;
    lv_obj_t * indev_obj_active;

    uint32_t layout_count;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t memory_zero;
    uint32_t math_rand_seed;
    lv_area_transform_cache_t area_trans_cache;

    lv_event_t * event_header;
    uint32_t event_last_register_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;

    lv_ll_t img_decoder_ll;






    lv_cache_t * img_header_cache;


    lv_draw_global_info_t draw_info;




    _lv_draw_sw_mask_radius_circle_dsc_arr_t sw_circle_cache;
# 129 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
    void * theme_default;







    lv_tlsf_state_t tlsf_state;


    lv_ll_t fsdrv_ll;
# 169 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
    struct _snippet_stack * span_snippet_stack;
# 196 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
    void * user_data;
} lv_global_t;
# 211 "../../micropython/../lv_binding_micropython/lvgl/src/core/lv_global.h"
                         extern lv_global_t lv_global;
# 121 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h" 2
# 164 "../../micropython/../lv_binding_micropython/lvgl/lvgl.h"
static inline int lv_version_major(void)
{
    return 9;
}

static inline int lv_version_minor(void)
{
    return 0;
}

static inline int lv_version_patch(void)
{
    return 0;
}

static inline const char * lv_version_info(void)
{
    return "";
}
