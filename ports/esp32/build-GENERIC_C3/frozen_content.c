//
// Content for MICROPY_MODULE_FROZEN_STR
//
#include <stdint.h>
#define MP_FROZEN_STR_NAMES \

const uint32_t mp_frozen_str_sizes[] = { 0 };
const char mp_frozen_str_content[] = {
"\0"
};

//
// Content for MICROPY_MODULE_FROZEN_MPY
//
#include "py/mpconfig.h"
#include "py/objint.h"
#include "py/objstr.h"
#include "py/emitglue.h"
#include "py/nativeglue.h"

#if MICROPY_LONGINT_IMPL != 2
#error "incompatible MICROPY_LONGINT_IMPL"
#endif

#if MPZ_DIG_SIZE != 16
#error "incompatible MPZ_DIG_SIZE"
#endif

#if MICROPY_PY_BUILTINS_FLOAT
typedef struct _mp_obj_float_t {
    mp_obj_base_t base;
    mp_float_t value;
} mp_obj_float_t;
#endif

#if MICROPY_PY_BUILTINS_COMPLEX
typedef struct _mp_obj_complex_t {
    mp_obj_base_t base;
    mp_float_t real;
    mp_float_t imag;
} mp_obj_complex_t;
#endif

enum {
    MP_QSTR_apa106_dot_py = MP_QSTRnumber_of,
    MP_QSTR_NeoPixel,
    MP_QSTR_neopixel,
    MP_QSTR_APA106,
    MP_QSTR_ORDER,
    MP_QSTR_flashbdev_dot_py,
    MP_QSTR_vfs,
    MP_QSTR_bdev,
    MP_QSTR_ffat,
    MP_QSTR_inisetup_dot_py,
    MP_QSTR_flashbdev,
    MP_QSTR_check_bootsec,
    MP_QSTR_fs_corrupted,
    MP_QSTR_setup,
    MP_QSTR_boot_dot_py,
    MP_QSTR_w,
    MP_QSTR_Performing_space_initial_space_setup,
    MP_QSTR__boot_dot_py,
    MP_QSTR_inisetup,
    MP_QSTR_upip_dot_py,
    MP_QSTR_os,
    MP_QSTR_json,
    MP_QSTR_upip_utarfile,
    MP_QSTR_tarfile,
    MP_QSTR_debug,
    MP_QSTR_index_urls,
    MP_QSTR_install_path,
    MP_QSTR_cleanup_files,
    MP_QSTR_gzdict_sz,
    MP_QSTR_file_buf,
    MP_QSTR_NotFoundError,
    MP_QSTR_op_split,
    MP_QSTR__makedirs,
    MP_QSTR_save_file,
    MP_QSTR_install_tar,
    MP_QSTR_expandhome,
    MP_QSTR_warn_ussl,
    MP_QSTR_url_open,
    MP_QSTR_get_pkg_metadata,
    MP_QSTR_fatal,
    MP_QSTR_install_pkg,
    MP_QSTR_install,
    MP_QSTR_get_install_path,
    MP_QSTR_cleanup,
    MP_QSTR_fname,
    MP_QSTR_subf,
    MP_QSTR_f,
    MP_QSTR_prefix,
    MP_QSTR__dot_egg_hyphen_info,
    MP_QSTR_extractfile,
    MP_QSTR_deps,
    MP_QSTR_Skipping,
    MP_QSTR_DIRTYPE,
    MP_QSTR_s,
    MP_QSTR__tilde__slash_,
    MP_QSTR_getenv,
    MP_QSTR_HOME,
    MP_QSTR_url,
    MP_QSTR__colon_,
    MP_QSTR_https_colon_,
    MP_QSTR__percent_s_slash__percent_s_slash_json,
    MP_QSTR_msg,
    MP_QSTR_exc,
    MP_QSTR_Error_colon_,
    MP_QSTR_pkg_spec,
    MP_QSTR__equals__equals_,
    MP_QSTR_releases,
    MP_QSTR_TarFile,
    MP_QSTR_fileobj,
    MP_QSTR_to_install,
    MP_QSTR_Queue_colon_,
    MP_QSTR__hyphen_h,
    MP_QSTR__hyphen__hyphen_help,
    MP_QSTR__hyphen_p,
    MP_QSTR__hyphen_r,
    MP_QSTR__hash_,
    MP_QSTR__hyphen_i,
    MP_QSTR__hyphen__hyphen_debug,
    MP_QSTR__hyphen_,
    MP_QSTR_https_colon__slash__slash_pypi_dot_org_slash_pypi,
    MP_QSTR_setup_dot_,
    MP_QSTR_PKG_hyphen_INFO,
    MP_QSTR_README,
    MP_QSTR__slash_requires_dot_txt,
    MP_QSTR_Extracting_space_,
    MP_QSTR_Package_space_not_space_found,
    MP_QSTR_Installing_space__percent_s_space__percent_s_space_from_space__percent_s,
    MP_QSTR_Installing_space_to_colon__space_,
    MP_QSTR_Warning_colon__space_Cannot_space_delete_space_,
    MP_QSTR_Default_space_install_space_path_colon_,
    MP_QSTR_upip_utarfile_dot_py,
    MP_QSTR_size,
    MP_QSTR_TAR_HEADER,
    MP_QSTR_REGTYPE,
    MP_QSTR_roundup,
    MP_QSTR_FileSection,
    MP_QSTR_TarInfo,
    MP_QSTR_val,
    MP_QSTR_align,
    MP_QSTR_skip,
    MP_QSTR_content_len,
    MP_QSTR_aligned_len,
    MP_QSTR_sz,
    MP_QSTR_buf,
    MP_QSTR__0x00_,
    MP_QSTR_tarinfo,
    MP_QSTR_TarInfo_paren_open__percent_r_comma__space__percent_s_comma__space__percent_d_paren_close_,
    MP_QSTR_ntptime_dot_py,
    MP_QSTR_NTP_DELTA,
    MP_QSTR_host,
    MP_QSTR_settime,
    MP_QSTR__bang_I,
    MP_QSTR_pool_dot_ntp_dot_org,
    MP_QSTR_dht_dot_py,
    MP_QSTR_mimxrt,
    MP_QSTR_rp2,
    MP_QSTR_pyboard,
    MP_QSTR_pyb,
    MP_QSTR_DHTBase,
    MP_QSTR_DHT11,
    MP_QSTR_DHT22,
    MP_QSTR_measure,
    MP_QSTR_humidity,
    MP_QSTR_temperature,
    MP_QSTR_checksum_space_error,
    MP_QSTR_onewire_dot_py,
    MP_QSTR__ow,
    MP_QSTR_OneWireError,
    MP_QSTR_OneWire,
    MP_QSTR_SEARCH_ROM,
    MP_QSTR_MATCH_ROM,
    MP_QSTR_SKIP_ROM,
    MP_QSTR_select_rom,
    MP_QSTR__search_rom,
    MP_QSTR_required,
    MP_QSTR_rom,
    MP_QSTR_l_rom,
    MP_QSTR_diff,
    MP_QSTR_ds18x20_dot_py,
    MP_QSTR_DS18X20,
    MP_QSTR_convert_temp,
    MP_QSTR_read_scratch,
    MP_QSTR_write_scratch,
    MP_QSTR_read_temp,
    MP_QSTR_ow,
    MP_QSTR_CRC_space_error,
    MP_QSTR_uasyncio_slash___init___dot_py,
    MP_QSTR_core,
    MP_QSTR___version__,
    MP_QSTR_funcs,
    MP_QSTR_wait_for,
    MP_QSTR_gather,
    MP_QSTR_event,
    MP_QSTR_Event,
    MP_QSTR_Lock,
    MP_QSTR_stream,
    MP_QSTR__attrs,
    MP_QSTR_attr,
    MP_QSTR_wait_for_ms,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_open_connection,
    MP_QSTR_start_server,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR_uasyncio_slash_core_dot_py,
    MP_QSTR_ticks,
    MP_QSTR_task,
    MP_QSTR_TimeoutError,
    MP_QSTR_message,
    MP_QSTR_exception,
    MP_QSTR_future,
    MP_QSTR__exc_context,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_IOQueue,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_run_until_complete,
    MP_QSTR_run,
    MP_QSTR__stopper,
    MP_QSTR__stop_task,
    MP_QSTR_Loop,
    MP_QSTR_get_event_loop,
    MP_QSTR_current_task,
    MP_QSTR_new_event_loop,
    MP_QSTR_t,
    MP_QSTR_sgen,
    MP_QSTR__enqueue,
    MP_QSTR__dequeue,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_wait_io_event,
    MP_QSTR_aw,
    MP_QSTR_main_task,
    MP_QSTR__io_queue,
    MP_QSTR_call_exception_handler,
    MP_QSTR__exc_handler,
    MP_QSTR_run_forever,
    MP_QSTR_set_exception_handler,
    MP_QSTR_get_exception_handler,
    MP_QSTR_default_exception_handler,
    MP_QSTR_runq_len,
    MP_QSTR_waitq_len,
    MP_QSTR_poller,
    MP_QSTR_idx,
    MP_QSTR_dt,
    MP_QSTR_context,
    MP_QSTR_future_colon_,
    MP_QSTR_coro_equals_,
    MP_QSTR_coroutine_space_expected,
    MP_QSTR_uasyncio_slash_event_dot_py,
    MP_QSTR_is_set,
    MP_QSTR_wait,
    MP_QSTR_waiting,
    MP_QSTR__flag,
    MP_QSTR_req,
    MP_QSTR_uasyncio_slash_funcs_dot_py,
    MP_QSTR__Remove,
    MP_QSTR_return_exceptions,
    MP_QSTR_runner,
    MP_QSTR_waiter,
    MP_QSTR_er,
    MP_QSTR_gather_task,
    MP_QSTR_can_squot_t_space_gather,
    MP_QSTR_uasyncio_slash_lock_dot_py,
    MP_QSTR_exc_type,
    MP_QSTR_tb,
    MP_QSTR_Lock_space_not_space_acquired,
    MP_QSTR_uasyncio_slash_stream_dot_py,
    MP_QSTR_Stream,
    MP_QSTR_Server,
    MP_QSTR_stream_awrite,
    MP_QSTR_wait_closed,
    MP_QSTR_aclose,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR_get_extra_info,
    MP_QSTR_readexactly,
    MP_QSTR_drain,
    MP_QSTR_port,
    MP_QSTR__serve,
    MP_QSTR_cb,
    MP_QSTR_backlog,
    MP_QSTR_out_buf,
    MP_QSTR_v,
    MP_QSTR_n,
    MP_QSTR_peername,
    MP_QSTR_webrepl_dot_py,
    MP_QSTR_websocket_helper,
    MP_QSTR_listen_s,
    MP_QSTR_client_s,
    MP_QSTR_setup_conn,
    MP_QSTR_accept_conn,
    MP_QSTR_start_foreground,
    MP_QSTR_accept_handler,
    MP_QSTR_0_dot_0_dot_0_dot_0,
    MP_QSTR_listen_sock,
    MP_QSTR_rejected,
    MP_QSTR_server_handshake,
    MP_QSTR_webrepl_cfg,
    MP_QSTR_PASS,
    MP_QSTR_webrepl_setup_dot_py,
    MP_QSTR__dot__slash_boot_dot_py,
    MP_QSTR_RC,
    MP_QSTR_CONFIG,
    MP_QSTR_input_choice,
    MP_QSTR_getpass,
    MP_QSTR_input_pass,
    MP_QSTR_exists,
    MP_QSTR_get_daemon_status,
    MP_QSTR_change_daemon,
    MP_QSTR_prompt,
    MP_QSTR_choices,
    MP_QSTR_webrepl,
    MP_QSTR_action,
    MP_QSTR__dot_tmp,
    MP_QSTR_enabled,
    MP_QSTR_disabled,
    MP_QSTR__gt__space_,
    MP_QSTR_E,
    MP_QSTR_y,
    MP_QSTR_PASS_space__equals__space__percent_r_0x0a_,
    MP_QSTR_D,
    MP_QSTR__dot__slash_webrepl_cfg_dot_py,
    MP_QSTR_Invalid_space_password_space_length,
    MP_QSTR_Confirm_space_password_colon__space_,
    MP_QSTR_Passwords_space_do_space_not_space_match,
    MP_QSTR_import_space_webrepl,
    MP_QSTR_webrepl_dot_start_paren_open__paren_close_,
    MP_QSTR__paren_open_Empty_space_line_space_to_space_quit_paren_close_,
    MP_QSTR_websocket_helper_dot_py,
    MP_QSTR_binascii,
    MP_QSTR_hashlib,
    MP_QSTR_DEBUG,
    MP_QSTR_client_handshake,
    MP_QSTR_sock,
    MP_QSTR_rwb,
    MP_QSTR_respkey_colon_,
    MP_QSTR__0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_EOF_space_in_space_headers,
    MP_QSTR_Not_space_a_space_websocket_space_request,
    MP_QSTR_Sec_hyphen_WebSocket_hyphen_Key_colon_,
    MP_QSTR_neopixel_dot_py,
    MP_QSTR_bpp,
    MP_QSTR_timing,
    MP_QSTR_i,
    MP_QSTR_offset,
};

const qstr_hash_t mp_qstr_frozen_const_hashes[] = {
    57253,
    33641,
    41065,
    20130,
    26955,
    59367,
    31366,
    12400,
    1904,
    7083,
    62912,
    28927,
    25555,
    706,
    47476,
    46546,
    7181,
    15787,
    46092,
    41630,
    28537,
    53757,
    32818,
    49188,
    21972,
    31100,
    2646,
    8079,
    5172,
    5613,
    6814,
    47575,
    43444,
    41149,
    52092,
    18252,
    44617,
    64197,
    30882,
    53467,
    47271,
    56068,
    63711,
    11973,
    21988,
    12583,
    46531,
    16021,
    24877,
    40366,
    60551,
    53300,
    5954,
    46550,
    28948,
    64430,
    30314,
    25486,
    46495,
    63796,
    15581,
    18044,
    9435,
    19783,
    39491,
    26789,
    36127,
    64868,
    19460,
    63616,
    26894,
    26336,
    28212,
    26360,
    26362,
    46470,
    26337,
    39732,
    46472,
    3759,
    23340,
    43770,
    60255,
    36962,
    56712,
    48284,
    7236,
    1253,
    17877,
    5528,
    34645,
    42272,
    17794,
    53709,
    14466,
    21770,
    23628,
    31486,
    64424,
    44804,
    30036,
    17841,
    28908,
    18804,
    46501,
    24588,
    62189,
    16733,
    64488,
    13605,
    35858,
    26189,
    33769,
    56666,
    63250,
    35221,
    38486,
    29166,
    28456,
    32861,
    33021,
    44573,
    44924,
    57065,
    53040,
    55311,
    8066,
    23888,
    55400,
    28580,
    20921,
    4555,
    20450,
    32507,
    19500,
    35765,
    41766,
    64712,
    26790,
    45825,
    15425,
    36436,
    8251,
    12004,
    28541,
    53071,
    50626,
    56254,
    38975,
    27400,
    40746,
    2184,
    27081,
    35561,
    59534,
    54361,
    24346,
    48310,
    25035,
    9558,
    25560,
    63679,
    39580,
    35638,
    22019,
    2115,
    40488,
    19302,
    48238,
    60370,
    14592,
    13092,
    12601,
    26002,
    24914,
    21139,
    37871,
    35180,
    12837,
    5648,
    59673,
    19971,
    8734,
    3209,
    46545,
    32154,
    26816,
    49770,
    52601,
    8726,
    6948,
    28211,
    31740,
    43474,
    19688,
    52355,
    25036,
    5288,
    60604,
    45889,
    52773,
    21895,
    42221,
    21584,
    28053,
    55546,
    22842,
    21001,
    50473,
    58580,
    58562,
    21902,
    31214,
    4246,
    35683,
    24501,
    37244,
    47572,
    23541,
    60665,
    28082,
    21626,
    4535,
    10131,
    52220,
    28563,
    43943,
    56964,
    55353,
    2976,
    59482,
    29411,
    24658,
    49657,
    2796,
    16583,
    23753,
    18805,
    55388,
    55469,
    28388,
    43370,
    30085,
    46547,
    46539,
    8320,
    20377,
    5289,
    59904,
    18128,
    59505,
    48726,
    31393,
    36290,
    64203,
    32199,
    8523,
    50376,
    803,
    61364,
    59393,
    27925,
    29844,
    48783,
    17127,
    58434,
    59261,
    25861,
    49387,
    54096,
    40417,
    39069,
    25086,
    60251,
    31778,
    40672,
    18645,
    26843,
    46560,
    46556,
    64660,
    46561,
    32613,
    22679,
    25106,
    36388,
    42115,
    30641,
    39613,
    48622,
    15505,
    27920,
    27956,
    38292,
    40913,
    34978,
    47804,
    40133,
    54338,
    60085,
    35752,
    49582,
    18887,
    9013,
    46540,
    15432,
};

const qstr_len_t mp_qstr_frozen_const_lengths[] = {
    9,
    8,
    8,
    6,
    5,
    12,
    3,
    4,
    4,
    11,
    9,
    13,
    12,
    5,
    7,
    1,
    24,
    8,
    8,
    7,
    2,
    4,
    13,
    7,
    5,
    10,
    12,
    13,
    9,
    8,
    13,
    8,
    9,
    9,
    11,
    10,
    9,
    8,
    16,
    5,
    11,
    7,
    16,
    7,
    5,
    4,
    1,
    6,
    9,
    11,
    4,
    8,
    7,
    1,
    2,
    6,
    4,
    3,
    1,
    6,
    10,
    3,
    3,
    6,
    8,
    2,
    8,
    7,
    7,
    10,
    6,
    2,
    6,
    2,
    2,
    1,
    2,
    7,
    1,
    21,
    6,
    8,
    6,
    13,
    11,
    17,
    24,
    15,
    23,
    21,
    16,
    4,
    10,
    7,
    7,
    11,
    7,
    3,
    5,
    4,
    11,
    11,
    2,
    3,
    1,
    7,
    19,
    10,
    9,
    4,
    7,
    2,
    12,
    6,
    6,
    3,
    7,
    3,
    7,
    5,
    5,
    7,
    8,
    11,
    14,
    10,
    3,
    12,
    7,
    10,
    9,
    8,
    10,
    11,
    8,
    3,
    5,
    4,
    10,
    7,
    12,
    12,
    13,
    9,
    2,
    9,
    20,
    4,
    11,
    5,
    8,
    6,
    5,
    5,
    4,
    6,
    6,
    4,
    11,
    14,
    15,
    12,
    12,
    12,
    16,
    5,
    4,
    12,
    7,
    9,
    6,
    12,
    18,
    7,
    16,
    11,
    18,
    3,
    8,
    10,
    4,
    14,
    12,
    14,
    1,
    4,
    8,
    8,
    10,
    11,
    13,
    2,
    9,
    9,
    22,
    12,
    11,
    21,
    21,
    25,
    8,
    9,
    6,
    3,
    2,
    7,
    7,
    5,
    18,
    17,
    6,
    4,
    7,
    5,
    3,
    17,
    7,
    17,
    6,
    6,
    2,
    11,
    12,
    16,
    8,
    2,
    17,
    18,
    6,
    6,
    13,
    11,
    6,
    6,
    9,
    14,
    11,
    5,
    4,
    6,
    2,
    7,
    7,
    1,
    1,
    8,
    10,
    16,
    8,
    8,
    10,
    11,
    16,
    14,
    7,
    11,
    8,
    16,
    11,
    4,
    16,
    9,
    2,
    6,
    12,
    7,
    10,
    6,
    17,
    13,
    6,
    7,
    7,
    6,
    4,
    7,
    8,
    2,
    1,
    1,
    10,
    1,
    16,
    23,
    18,
    22,
    14,
    15,
    20,
    19,
    8,
    7,
    5,
    16,
    4,
    3,
    8,
    4,
    14,
    23,
    18,
    11,
    3,
    6,
    1,
    6,
};

extern const qstr_pool_t mp_qstr_const_pool;
const qstr_pool_t mp_qstr_frozen_const_pool = {
    &mp_qstr_const_pool, // previous pool
    MP_QSTRnumber_of, // previous pool size
    10, // allocated entries
    306, // used entries
    (qstr_hash_t *)mp_qstr_frozen_const_hashes,
    (qstr_len_t *)mp_qstr_frozen_const_lengths,
    {
        "apa106.py",
        "NeoPixel",
        "neopixel",
        "APA106",
        "ORDER",
        "flashbdev.py",
        "vfs",
        "bdev",
        "ffat",
        "inisetup.py",
        "flashbdev",
        "check_bootsec",
        "fs_corrupted",
        "setup",
        "boot.py",
        "w",
        "Performing initial setup",
        "_boot.py",
        "inisetup",
        "upip.py",
        "os",
        "json",
        "upip_utarfile",
        "tarfile",
        "debug",
        "index_urls",
        "install_path",
        "cleanup_files",
        "gzdict_sz",
        "file_buf",
        "NotFoundError",
        "op_split",
        "_makedirs",
        "save_file",
        "install_tar",
        "expandhome",
        "warn_ussl",
        "url_open",
        "get_pkg_metadata",
        "fatal",
        "install_pkg",
        "install",
        "get_install_path",
        "cleanup",
        "fname",
        "subf",
        "f",
        "prefix",
        ".egg-info",
        "extractfile",
        "deps",
        "Skipping",
        "DIRTYPE",
        "s",
        "~/",
        "getenv",
        "HOME",
        "url",
        ":",
        "https:",
        "%s/%s/json",
        "msg",
        "exc",
        "Error:",
        "pkg_spec",
        "==",
        "releases",
        "TarFile",
        "fileobj",
        "to_install",
        "Queue:",
        "-h",
        "--help",
        "-p",
        "-r",
        "#",
        "-i",
        "--debug",
        "-",
        "https://pypi.org/pypi",
        "setup.",
        "PKG-INFO",
        "README",
        "/requires.txt",
        "Extracting ",
        "Package not found",
        "Installing %s %s from %s",
        "Installing to: ",
        "Warning: Cannot delete ",
        "Default install path:",
        "upip_utarfile.py",
        "size",
        "TAR_HEADER",
        "REGTYPE",
        "roundup",
        "FileSection",
        "TarInfo",
        "val",
        "align",
        "skip",
        "content_len",
        "aligned_len",
        "sz",
        "buf",
        "\x00",
        "tarinfo",
        "TarInfo(%r, %s, %d)",
        "ntptime.py",
        "NTP_DELTA",
        "host",
        "settime",
        "!I",
        "pool.ntp.org",
        "dht.py",
        "mimxrt",
        "rp2",
        "pyboard",
        "pyb",
        "DHTBase",
        "DHT11",
        "DHT22",
        "measure",
        "humidity",
        "temperature",
        "checksum error",
        "onewire.py",
        "_ow",
        "OneWireError",
        "OneWire",
        "SEARCH_ROM",
        "MATCH_ROM",
        "SKIP_ROM",
        "select_rom",
        "_search_rom",
        "required",
        "rom",
        "l_rom",
        "diff",
        "ds18x20.py",
        "DS18X20",
        "convert_temp",
        "read_scratch",
        "write_scratch",
        "read_temp",
        "ow",
        "CRC error",
        "uasyncio/__init__.py",
        "core",
        "__version__",
        "funcs",
        "wait_for",
        "gather",
        "event",
        "Event",
        "Lock",
        "stream",
        "_attrs",
        "attr",
        "wait_for_ms",
        "ThreadSafeFlag",
        "open_connection",
        "start_server",
        "StreamReader",
        "StreamWriter",
        "uasyncio/core.py",
        "ticks",
        "task",
        "TimeoutError",
        "message",
        "exception",
        "future",
        "_exc_context",
        "SingletonGenerator",
        "IOQueue",
        "_promote_to_task",
        "create_task",
        "run_until_complete",
        "run",
        "_stopper",
        "_stop_task",
        "Loop",
        "get_event_loop",
        "current_task",
        "new_event_loop",
        "t",
        "sgen",
        "_enqueue",
        "_dequeue",
        "queue_read",
        "queue_write",
        "wait_io_event",
        "aw",
        "main_task",
        "_io_queue",
        "call_exception_handler",
        "_exc_handler",
        "run_forever",
        "set_exception_handler",
        "get_exception_handler",
        "default_exception_handler",
        "runq_len",
        "waitq_len",
        "poller",
        "idx",
        "dt",
        "context",
        "future:",
        "coro=",
        "coroutine expected",
        "uasyncio/event.py",
        "is_set",
        "wait",
        "waiting",
        "_flag",
        "req",
        "uasyncio/funcs.py",
        "_Remove",
        "return_exceptions",
        "runner",
        "waiter",
        "er",
        "gather_task",
        "can't gather",
        "uasyncio/lock.py",
        "exc_type",
        "tb",
        "Lock not acquired",
        "uasyncio/stream.py",
        "Stream",
        "Server",
        "stream_awrite",
        "wait_closed",
        "aclose",
        "awrite",
        "awritestr",
        "get_extra_info",
        "readexactly",
        "drain",
        "port",
        "_serve",
        "cb",
        "backlog",
        "out_buf",
        "v",
        "n",
        "peername",
        "webrepl.py",
        "websocket_helper",
        "listen_s",
        "client_s",
        "setup_conn",
        "accept_conn",
        "start_foreground",
        "accept_handler",
        "0.0.0.0",
        "listen_sock",
        "rejected",
        "server_handshake",
        "webrepl_cfg",
        "PASS",
        "webrepl_setup.py",
        "./boot.py",
        "RC",
        "CONFIG",
        "input_choice",
        "getpass",
        "input_pass",
        "exists",
        "get_daemon_status",
        "change_daemon",
        "prompt",
        "choices",
        "webrepl",
        "action",
        ".tmp",
        "enabled",
        "disabled",
        "> ",
        "E",
        "y",
        "\x50\x41\x53\x53\x20\x3d\x20\x25\x72\x0a",
        "D",
        "./webrepl_cfg.py",
        "Invalid password length",
        "Confirm password: ",
        "Passwords do not match",
        "import webrepl",
        "webrepl.start()",
        "(Empty line to quit)",
        "websocket_helper.py",
        "binascii",
        "hashlib",
        "DEBUG",
        "client_handshake",
        "sock",
        "rwb",
        "respkey:",
        "\x0d\x0a\x0d\x0a",
        "EOF in headers",
        "Not a websocket request",
        "Sec-WebSocket-Key:",
        "neopixel.py",
        "bpp",
        "timing",
        "i",
        "offset",
    },
};

////////////////////////////////////////////////////////////////////////////////
// frozen module apa106
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/apa106.mpy
// - frozen file name: apa106.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file apa106.py, scope apa106__lt_module_gt_
static const byte fun_data_apa106__lt_module_gt_[30] = {
    0x18,0x06, // prelude
    0x01, // names: <module>
    0x60,0x6c, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'NeoPixel'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'neopixel'
    0x1c,0x02, // IMPORT_FROM 'NeoPixel'
    0x16,0x02, // STORE_NAME 'NeoPixel'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'APA106'
    0x11,0x02, // LOAD_NAME 'NeoPixel'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'APA106'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of apa106__lt_module_gt_
// frozen bytecode for file apa106.py, scope apa106_APA106
static const byte fun_data_apa106_APA106[19] = {
    0x00,0x06, // prelude
    0x04, // names: APA106
    0x88,0x07, // code info
    0x11,0x05, // LOAD_NAME '__name__'
    0x16,0x06, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'APA106'
    0x16,0x07, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x08, // STORE_NAME 'ORDER'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_apa106_APA106 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_apa106_APA106,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_apa106_APA106 + 3,
        .opcodes = fun_data_apa106_APA106 + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_apa106__lt_module_gt_[] = {
    &raw_code_apa106_APA106,
};

static const mp_raw_code_t raw_code_apa106__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_apa106__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 30,
    #endif
    .children = (void *)&children_apa106__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_apa106__lt_module_gt_ + 3,
        .opcodes = fun_data_apa106__lt_module_gt_ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_apa106[9] = {
    MP_QSTR_apa106_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_NeoPixel,
    MP_QSTR_neopixel,
    MP_QSTR_APA106,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_ORDER,
};

// constants
static const mp_rom_obj_tuple_t const_obj_apa106_0 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(0),
    MP_ROM_INT(1),
    MP_ROM_INT(2),
    MP_ROM_INT(3),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_apa106[1] = {
    MP_ROM_PTR(&const_obj_apa106_0),
};

static const mp_frozen_module_t frozen_module_apa106 = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_apa106,
        .obj_table = (mp_obj_t *)&const_obj_table_data_apa106,
    },
    .rc = &raw_code_apa106__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module flashbdev
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/flashbdev.mpy
// - frozen file name: flashbdev.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file flashbdev.py, scope flashbdev__lt_module_gt_
static const byte fun_data_flashbdev__lt_module_gt_[77] = {
    0x30,0x0a, // prelude
    0x01, // names: <module>
    0x6c,0x31,0x24,0x36, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'Partition'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'esp32'
    0x1c,0x02, // IMPORT_FROM 'Partition'
    0x16,0x02, // STORE_NAME 'Partition'
    0x59, // POP_TOP
    0x11,0x02, // LOAD_NAME 'Partition'
    0x14,0x04, // LOAD_METHOD 'find'
    0x11,0x02, // LOAD_NAME 'Partition'
    0x13,0x05, // LOAD_ATTR 'TYPE_DATA'
    0x10,0x06, // LOAD_CONST_STRING 'label'
    0x10,0x07, // LOAD_CONST_STRING 'vfs'
    0x36,0x82,0x01, // CALL_METHOD 257
    0x16,0x08, // STORE_NAME 'bdev'
    0x11,0x08, // LOAD_NAME 'bdev'
    0x43,0x56, // POP_JUMP_IF_TRUE 22
    0x11,0x02, // LOAD_NAME 'Partition'
    0x14,0x04, // LOAD_METHOD 'find'
    0x11,0x02, // LOAD_NAME 'Partition'
    0x13,0x05, // LOAD_ATTR 'TYPE_DATA'
    0x10,0x06, // LOAD_CONST_STRING 'label'
    0x10,0x09, // LOAD_CONST_STRING 'ffat'
    0x10,0x0a, // LOAD_CONST_STRING 'block_size'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x36,0x84,0x01, // CALL_METHOD 513
    0x16,0x08, // STORE_NAME 'bdev'
    0x11,0x08, // LOAD_NAME 'bdev'
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x11,0x08, // LOAD_NAME 'bdev'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x42,0x41, // JUMP 1
    0x51, // LOAD_CONST_NONE
    0x16,0x08, // STORE_NAME 'bdev'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_flashbdev__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_flashbdev__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 77,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_flashbdev__lt_module_gt_ + 3,
        .opcodes = fun_data_flashbdev__lt_module_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_flashbdev[11] = {
    MP_QSTR_flashbdev_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_Partition,
    MP_QSTR_esp32,
    MP_QSTR_find,
    MP_QSTR_TYPE_DATA,
    MP_QSTR_label,
    MP_QSTR_vfs,
    MP_QSTR_bdev,
    MP_QSTR_ffat,
    MP_QSTR_block_size,
};

static const mp_frozen_module_t frozen_module_flashbdev = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_flashbdev,
        .obj_table = NULL,
    },
    .rc = &raw_code_flashbdev__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module inisetup
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/inisetup.mpy
// - frozen file name: inisetup.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file inisetup.py, scope inisetup__lt_module_gt_
static const byte fun_data_inisetup__lt_module_gt_[41] = {
    0x08,0x0e, // prelude
    0x01, // names: <module>
    0x26,0x6c,0x84,0x0d,0x84,0x0f, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'uos'
    0x16,0x02, // STORE_NAME 'uos'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING 'bdev'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'flashbdev'
    0x1c,0x03, // IMPORT_FROM 'bdev'
    0x16,0x03, // STORE_NAME 'bdev'
    0x59, // POP_TOP
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x05, // STORE_NAME 'check_bootsec'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x06, // STORE_NAME 'fs_corrupted'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x07, // STORE_NAME 'setup'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_check_bootsec
static const byte fun_data_inisetup_check_bootsec[72] = {
    0x40,0x18, // prelude
    0x05, // names: check_bootsec
    0x60,0x40,0x2d,0x29,0x22,0x25,0x27,0x22,0x28,0x23,0x22, // code info
    0x12,0x08, // LOAD_GLOBAL 'bytearray'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x09, // LOAD_METHOD 'ioctl'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x0a, // LOAD_METHOD 'readblocks'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x12, // FOR_ITER 18
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x50, // LOAD_CONST_FALSE
    0xc1, // STORE_FAST 1
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x2c, // JUMP -20
    0xb1, // LOAD_FAST 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x12,0x06, // LOAD_GLOBAL 'fs_corrupted'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_inisetup_check_bootsec = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_inisetup_check_bootsec,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 72,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_inisetup_check_bootsec + 3,
        .opcodes = fun_data_inisetup_check_bootsec + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_fs_corrupted
static const byte fun_data_inisetup_fs_corrupted[33] = {
    0x18,0x10, // prelude
    0x06, // names: fs_corrupted
    0x80,0x12,0x45,0x20,0x22,0x85,0x07, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0b, // IMPORT_NAME 'time'
    0xc0, // STORE_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'print'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x0d, // LOAD_METHOD 'sleep'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x30, // JUMP -16
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_inisetup_fs_corrupted = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_inisetup_fs_corrupted,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_inisetup_fs_corrupted + 3,
        .opcodes = fun_data_inisetup_fs_corrupted + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_setup
static const byte fun_data_inisetup_setup[80] = {
    0x34,0x18, // prelude
    0x07, // names: setup
    0x80,0x21,0x25,0x27,0x2b,0x29,0x2a,0x2b,0x23,0x88,0x08, // code info
    0x12,0x05, // LOAD_GLOBAL 'check_bootsec'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x0c, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'uos'
    0x13,0x0e, // LOAD_ATTR 'VfsLfs2'
    0x14,0x0f, // LOAD_METHOD 'mkfs'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'uos'
    0x14,0x0e, // LOAD_METHOD 'VfsLfs2'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0x12,0x02, // LOAD_GLOBAL 'uos'
    0x14,0x10, // LOAD_METHOD 'mount'
    0xb0, // LOAD_FAST 0
    0x10,0x11, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x12,0x12, // LOAD_GLOBAL 'open'
    0x10,0x13, // LOAD_CONST_STRING 'boot.py'
    0x10,0x14, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0a, // SETUP_WITH 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x15, // LOAD_METHOD 'write'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_inisetup_setup = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_inisetup_setup,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 80,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_inisetup_setup + 3,
        .opcodes = fun_data_inisetup_setup + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_inisetup__lt_module_gt_[] = {
    &raw_code_inisetup_check_bootsec,
    &raw_code_inisetup_fs_corrupted,
    &raw_code_inisetup_setup,
};

static const mp_raw_code_t raw_code_inisetup__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_inisetup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = (void *)&children_inisetup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 3,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_inisetup__lt_module_gt_ + 3,
        .opcodes = fun_data_inisetup__lt_module_gt_ + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_inisetup[22] = {
    MP_QSTR_inisetup_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_uos,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_check_bootsec,
    MP_QSTR_fs_corrupted,
    MP_QSTR_setup,
    MP_QSTR_bytearray,
    MP_QSTR_ioctl,
    MP_QSTR_readblocks,
    MP_QSTR_time,
    MP_QSTR_print,
    MP_QSTR_sleep,
    MP_QSTR_VfsLfs2,
    MP_QSTR_mkfs,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_open,
    MP_QSTR_boot_dot_py,
    MP_QSTR_w,
    MP_QSTR_write,
};

// constants
static const mp_obj_str_t const_obj_inisetup_0 = {{&mp_type_str}, 55993, 258, (const byte*)"\x54\x68\x65\x20\x66\x69\x6c\x65\x73\x79\x73\x74\x65\x6d\x20\x61\x70\x70\x65\x61\x72\x73\x20\x74\x6f\x20\x62\x65\x20\x63\x6f\x72\x72\x75\x70\x74\x65\x64\x2e\x20\x49\x66\x20\x79\x6f\x75\x20\x68\x61\x64\x20\x69\x6d\x70\x6f\x72\x74\x61\x6e\x74\x20\x64\x61\x74\x61\x20\x74\x68\x65\x72\x65\x2c\x20\x79\x6f\x75\x0a\x6d\x61\x79\x20\x77\x61\x6e\x74\x20\x74\x6f\x20\x6d\x61\x6b\x65\x20\x61\x20\x66\x6c\x61\x73\x68\x20\x73\x6e\x61\x70\x73\x68\x6f\x74\x20\x74\x6f\x20\x74\x72\x79\x20\x74\x6f\x20\x72\x65\x63\x6f\x76\x65\x72\x20\x69\x74\x2e\x20\x4f\x74\x68\x65\x72\x77\x69\x73\x65\x2c\x20\x70\x65\x72\x66\x6f\x72\x6d\x0a\x66\x61\x63\x74\x6f\x72\x79\x20\x72\x65\x70\x72\x6f\x67\x72\x61\x6d\x6d\x69\x6e\x67\x20\x6f\x66\x20\x4d\x69\x63\x72\x6f\x50\x79\x74\x68\x6f\x6e\x20\x66\x69\x72\x6d\x77\x61\x72\x65\x20\x28\x63\x6f\x6d\x70\x6c\x65\x74\x65\x6c\x79\x20\x65\x72\x61\x73\x65\x20\x66\x6c\x61\x73\x68\x2c\x20\x66\x6f\x6c\x6c\x6f\x77\x65\x64\x0a\x62\x79\x20\x66\x69\x72\x6d\x77\x61\x72\x65\x20\x70\x72\x6f\x67\x72\x61\x6d\x6d\x69\x6e\x67\x29\x2e\x0a"};
static const mp_obj_str_t const_obj_inisetup_2 = {{&mp_type_str}, 18582, 139, (const byte*)"\x23\x20\x54\x68\x69\x73\x20\x66\x69\x6c\x65\x20\x69\x73\x20\x65\x78\x65\x63\x75\x74\x65\x64\x20\x6f\x6e\x20\x65\x76\x65\x72\x79\x20\x62\x6f\x6f\x74\x20\x28\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x61\x6b\x65\x2d\x62\x6f\x6f\x74\x20\x66\x72\x6f\x6d\x20\x64\x65\x65\x70\x73\x6c\x65\x65\x70\x29\x0a\x23\x69\x6d\x70\x6f\x72\x74\x20\x65\x73\x70\x0a\x23\x65\x73\x70\x2e\x6f\x73\x64\x65\x62\x75\x67\x28\x4e\x6f\x6e\x65\x29\x0a\x23\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x0a\x23\x77\x65\x62\x72\x65\x70\x6c\x2e\x73\x74\x61\x72\x74\x28\x29\x0a"};

// constant table
static const mp_rom_obj_t const_obj_table_data_inisetup[3] = {
    MP_ROM_PTR(&const_obj_inisetup_0),
    MP_ROM_QSTR(MP_QSTR_Performing_space_initial_space_setup),
    MP_ROM_PTR(&const_obj_inisetup_2),
};

static const mp_frozen_module_t frozen_module_inisetup = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_inisetup,
        .obj_table = (mp_obj_t *)&const_obj_table_data_inisetup,
    },
    .rc = &raw_code_inisetup__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module _boot
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/_boot.mpy
// - frozen file name: _boot.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file _boot.py, scope _boot__lt_module_gt_
static const byte fun_data__boot__lt_module_gt_[87] = {
    0x2c,0x12, // prelude
    0x01, // names: <module>
    0x26,0x26,0x4c,0x22,0x24,0x54,0x46,0x4b, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'gc'
    0x16,0x02, // STORE_NAME 'gc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'uos'
    0x16,0x03, // STORE_NAME 'uos'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'bdev'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'flashbdev'
    0x1c,0x04, // IMPORT_FROM 'bdev'
    0x16,0x04, // STORE_NAME 'bdev'
    0x59, // POP_TOP
    0x48,0x11, // SETUP_EXCEPT 17
    0x11,0x04, // LOAD_NAME 'bdev'
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x06, // LOAD_METHOD 'mount'
    0x11,0x04, // LOAD_NAME 'bdev'
    0x10,0x07, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x4a,0x18, // POP_EXCEPT_JUMP 24
    0x57, // DUP_TOP
    0x11,0x08, // LOAD_NAME 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x09, // IMPORT_NAME 'inisetup'
    0x16,0x09, // STORE_NAME 'inisetup'
    0x11,0x09, // LOAD_NAME 'inisetup'
    0x14,0x0a, // LOAD_METHOD 'setup'
    0x36,0x00, // CALL_METHOD 0
    0x16,0x0b, // STORE_NAME 'vfs'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x11,0x02, // LOAD_NAME 'gc'
    0x14,0x0c, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code__boot__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data__boot__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 87,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data__boot__lt_module_gt_ + 3,
        .opcodes = fun_data__boot__lt_module_gt_ + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data__boot[13] = {
    MP_QSTR__boot_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_gc,
    MP_QSTR_uos,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_OSError,
    MP_QSTR_inisetup,
    MP_QSTR_setup,
    MP_QSTR_vfs,
    MP_QSTR_collect,
};

static const mp_frozen_module_t frozen_module__boot = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data__boot,
        .obj_table = NULL,
    },
    .rc = &raw_code__boot__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module upip
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/upip.mpy
// - frozen file name: upip.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file upip.py, scope upip__lt_module_gt_
static const byte fun_data_upip__lt_module_gt_[242] = {
    0x18,0x66, // prelude
    0x01, // names: <module>
    0x80,0x07,0x26,0x26,0x26,0x26,0x26,0x26,0x46,0x67,0x23,0x28,0x23,0x24,0x43,0x69,0x6b,0x20,0x84,0x0d,0x8a,0x14,0x84,0x0a,0x84,0x20,0x84,0x07,0x26,0x46,0x63,0x84,0x32,0x84,0x0d,0x88,0x07,0x84,0x1b,0x88,0x26,0x84,0x0b,0x84,0x08,0x84,0x15,0x84,0x37,0x27, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'gc'
    0x16,0x03, // STORE_NAME 'gc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'uos'
    0x16,0x05, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'uerrno'
    0x16,0x07, // STORE_NAME 'errno'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'ujson'
    0x16,0x09, // STORE_NAME 'json'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0a, // IMPORT_NAME 'uzlib'
    0x16,0x0a, // STORE_NAME 'uzlib'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0b, // IMPORT_NAME 'upip_utarfile'
    0x16,0x0c, // STORE_NAME 'tarfile'
    0x11,0x03, // LOAD_NAME 'gc'
    0x14,0x0d, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x17,0x0e, // STORE_GLOBAL 'debug'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2b,0x02, // BUILD_LIST 2
    0x17,0x0f, // STORE_GLOBAL 'index_urls'
    0x51, // LOAD_CONST_NONE
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0x2b,0x00, // BUILD_LIST 0
    0x16,0x11, // STORE_NAME 'cleanup_files'
    0x9f, // LOAD_CONST_SMALL_INT 31
    0x17,0x12, // STORE_GLOBAL 'gzdict_sz'
    0x11,0x13, // LOAD_NAME 'bytearray'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x34,0x01, // CALL_FUNCTION 1
    0x17,0x14, // STORE_GLOBAL 'file_buf'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x15, // LOAD_CONST_STRING 'NotFoundError'
    0x11,0x16, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x15, // STORE_NAME 'NotFoundError'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x17, // STORE_NAME 'op_split'
    0x22,0x83,0x7f, // LOAD_CONST_SMALL_INT 511
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x18, // STORE_NAME '_makedirs'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x19, // STORE_NAME 'save_file'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x1a, // STORE_NAME 'install_tar'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x1b, // STORE_NAME 'expandhome'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x1c, // IMPORT_NAME 'ussl'
    0x16,0x1c, // STORE_NAME 'ussl'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x1d, // IMPORT_NAME 'usocket'
    0x16,0x1d, // STORE_NAME 'usocket'
    0x52, // LOAD_CONST_TRUE
    0x17,0x1e, // STORE_GLOBAL 'warn_ussl'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1f, // STORE_NAME 'url_open'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x20, // STORE_NAME 'get_pkg_metadata'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x21, // STORE_NAME 'fatal'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x22, // STORE_NAME 'install_pkg'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0a, // MAKE_FUNCTION_DEFARGS 10
    0x16,0x23, // STORE_NAME 'install'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x24, // STORE_NAME 'get_install_path'
    0x32,0x0c, // MAKE_FUNCTION 12
    0x16,0x25, // STORE_NAME 'cleanup'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x26, // STORE_NAME 'help'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x27, // STORE_NAME 'main'
    0x11,0x28, // LOAD_NAME '__name__'
    0x10,0x29, // LOAD_CONST_STRING '__main__'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x11,0x27, // LOAD_NAME 'main'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_NotFoundError
static const byte fun_data_upip_NotFoundError[15] = {
    0x00,0x06, // prelude
    0x15, // names: NotFoundError
    0x88,0x1c, // code info
    0x11,0x28, // LOAD_NAME '__name__'
    0x16,0x2a, // STORE_NAME '__module__'
    0x10,0x15, // LOAD_CONST_STRING 'NotFoundError'
    0x16,0x2b, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_NotFoundError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_NotFoundError,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_upip_NotFoundError + 3,
        .opcodes = fun_data_upip_NotFoundError + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_op_split
static const byte fun_data_upip_op_split[64] = {
    0x31,0x18, // prelude
    0x17,0x2c, // names: op_split, path
    0x80,0x20,0x26,0x23,0x29,0x29,0x26,0x24,0x23,0x23, // code info
    0xb0, // LOAD_FAST 0
    0x10,0x2d, // LOAD_CONST_STRING ''
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x2e, // LOAD_METHOD 'rsplit'
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0x12,0x30, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x10,0x2d, // LOAD_CONST_STRING ''
    0xb0, // LOAD_FAST 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_op_split = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_op_split,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 64,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_upip_op_split + 4,
        .opcodes = fun_data_upip_op_split + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip__makedirs
static const byte fun_data_upip__makedirs[144] = {
    0x82,0x13,0x28, // prelude
    0x18,0x31,0x32, // names: _makedirs, name, mode
    0x80,0x2d,0x22,0x23,0x33,0x28,0x23,0x25,0x2b,0x25,0x24,0x22,0x28,0x4d,0x34,0x22,0x2e, // code info
    0x50, // LOAD_CONST_FALSE
    0xc2, // STORE_FAST 2
    0x10,0x2d, // LOAD_CONST_STRING ''
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x14,0x33, // LOAD_METHOD 'rstrip'
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x14,0x34, // LOAD_METHOD 'split'
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x2d, // LOAD_CONST_STRING ''
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xc3, // STORE_FAST 3
    0xb4, // LOAD_FAST 4
    0x5f, // GET_ITER_STACK
    0x4b,0x50, // FOR_ITER 80
    0xc5, // STORE_FAST 5
    0xb3, // LOAD_FAST 3
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb3, // LOAD_FAST 3
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xe5, // BINARY_OP 14 __iadd__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0xe5, // BINARY_OP 14 __iadd__
    0xc3, // STORE_FAST 3
    0x48,0x0c, // SETUP_EXCEPT 12
    0x12,0x05, // LOAD_GLOBAL 'os'
    0x14,0x35, // LOAD_METHOD 'mkdir'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc2, // STORE_FAST 2
    0x4a,0x2a, // POP_EXCEPT_JUMP 42
    0x57, // DUP_TOP
    0x12,0x36, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x63, // POP_JUMP_IF_FALSE 35
    0xc6, // STORE_FAST 6
    0x49,0x19, // SETUP_FINALLY 25
    0xb6, // LOAD_FAST 6
    0x13,0x07, // LOAD_ATTR 'errno'
    0x12,0x07, // LOAD_GLOBAL 'errno'
    0x13,0x37, // LOAD_ATTR 'EEXIST'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb6, // LOAD_FAST 6
    0x13,0x07, // LOAD_ATTR 'errno'
    0x12,0x07, // LOAD_GLOBAL 'errno'
    0x13,0x38, // LOAD_ATTR 'EISDIR'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb6, // LOAD_FAST 6
    0x65, // RAISE_OBJ
    0x50, // LOAD_CONST_FALSE
    0xc2, // STORE_FAST 2
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0xae,0x7f, // JUMP -82
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip__makedirs = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip__makedirs,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 144,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_upip__makedirs + 6,
        .opcodes = fun_data_upip__makedirs + 23,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_save_file
static const byte fun_data_upip_save_file[52] = {
    0x4e,0x16, // prelude
    0x19,0x39,0x3a, // names: save_file, fname, subf
    0x80,0x41,0x20,0x2a,0x20,0x28,0x23,0x22, // code info
    0x12,0x3b, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x3c, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x1a, // SETUP_WITH 26
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x14,0x3d, // LOAD_METHOD 'readinto'
    0x12,0x14, // LOAD_GLOBAL 'file_buf'
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x4b, // JUMP 11
    0xb2, // LOAD_FAST 2
    0x14,0x3e, // LOAD_METHOD 'write'
    0x12,0x14, // LOAD_GLOBAL 'file_buf'
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x28, // JUMP -24
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_save_file = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_save_file,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_upip_save_file + 5,
        .opcodes = fun_data_upip_save_file + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_install_tar
static const byte fun_data_upip_install_tar[198] = {
    0x9e,0x10,0x38, // prelude
    0x1a,0x3f,0x40, // names: install_tar, f, prefix
    0x80,0x4b,0x23,0x46,0x24,0x22,0x58,0x46,0x22,0x46,0x2e,0x29,0x2e,0x22,0x24,0x28,0x48,0x23,0x24,0x2a,0x24,0x29,0x26,0x27,0x2a, // code info
    0x2c,0x00, // BUILD_MAP 0
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x9d,0x01, // FOR_ITER 157
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x13,0x31, // LOAD_ATTR 'name'
    0xc4, // STORE_FAST 4
    0x48,0x11, // SETUP_EXCEPT 17
    0xb4, // LOAD_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x41, // LOAD_METHOD 'index'
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x4a,0x0d, // POP_EXCEPT_JUMP 13
    0x57, // DUP_TOP
    0x12,0x42, // LOAD_GLOBAL 'ValueError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x59, // POP_TOP
    0x10,0x2d, // LOAD_CONST_STRING ''
    0xc4, // STORE_FAST 4
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x52, // LOAD_CONST_TRUE
    0xc5, // STORE_FAST 5
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x5f, // GET_ITER_STACK
    0x4b,0x3c, // FOR_ITER 60
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0x14,0x43, // LOAD_METHOD 'startswith'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0x10,0x44, // LOAD_CONST_STRING '.egg-info'
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0x44,0x6b, // POP_JUMP_IF_FALSE 43
    0xb4, // LOAD_FAST 4
    0x14,0x45, // LOAD_METHOD 'endswith'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x36,0x01, // CALL_METHOD 1
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x14,0x46, // LOAD_METHOD 'extractfile'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x14,0x47, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xb2, // LOAD_FAST 2
    0x10,0x48, // LOAD_CONST_STRING 'deps'
    0x56, // STORE_SUBSCR
    0x50, // LOAD_CONST_FALSE
    0xc5, // STORE_FAST 5
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x10,0x4a, // LOAD_CONST_STRING 'Skipping'
    0xb4, // LOAD_FAST 4
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x02, // JUMP -62
    0xb5, // LOAD_FAST 5
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0xb1, // LOAD_FAST 1
    0xb4, // LOAD_FAST 4
    0xf2, // BINARY_OP 27 __add__
    0xc7, // STORE_FAST 7
    0xb3, // LOAD_FAST 3
    0x13,0x4b, // LOAD_ATTR 'type'
    0x12,0x0c, // LOAD_GLOBAL 'tarfile'
    0x13,0x4c, // LOAD_ATTR 'DIRTYPE'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x61, // POP_JUMP_IF_FALSE 33
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xb7, // LOAD_FAST 7
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x18, // LOAD_GLOBAL '_makedirs'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x46, // LOAD_METHOD 'extractfile'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc8, // STORE_FAST 8
    0x12,0x19, // LOAD_GLOBAL 'save_file'
    0xb7, // LOAD_FAST 7
    0xb8, // LOAD_FAST 8
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0xe0,0x7e, // JUMP -160
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_install_tar = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_install_tar,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 198,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 20,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_upip_install_tar + 6,
        .opcodes = fun_data_upip_install_tar + 31,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_expandhome
static const byte fun_data_upip_expandhome[38] = {
    0x31,0x0e, // prelude
    0x1b,0x4d, // names: expandhome, s
    0x80,0x6b,0x26,0x29,0x2c, // code info
    0x10,0x4e, // LOAD_CONST_STRING '~/'
    0xb0, // LOAD_FAST 0
    0xdd, // BINARY_OP 6 <in>
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0x12,0x05, // LOAD_GLOBAL 'os'
    0x14,0x4f, // LOAD_METHOD 'getenv'
    0x10,0x50, // LOAD_CONST_STRING 'HOME'
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x51, // LOAD_METHOD 'replace'
    0x10,0x4e, // LOAD_CONST_STRING '~/'
    0xb1, // LOAD_FAST 1
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x02, // CALL_METHOD 2
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_expandhome = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_expandhome,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 38,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_upip_expandhome + 4,
        .opcodes = fun_data_upip_expandhome + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_url_open
static const byte fun_data_upip_url_open[324] = {
    0x91,0x12,0x4e, // prelude
    0x1f,0x52, // names: url_open, url
    0x80,0x78,0x40,0x24,0x46,0x2e,0x22,0x24,0x26,0x2b,0x26,0x59,0x53,0x44,0x30,0x43,0x49,0x26,0x2c,0x24,0x29,0x63,0x2e,0x26,0x2c,0x26,0x2c,0x27,0x26,0x20,0x26,0x23,0x27,0x26,0x4f,0x26,0x4b, // code info
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x49, // LOAD_GLOBAL 'print'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x34, // LOAD_METHOD 'split'
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x02, // CALL_METHOD 2
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc1, // STORE_FAST 1
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0x48,0x2b, // SETUP_EXCEPT 43
    0x22,0x83,0x3b, // LOAD_CONST_SMALL_INT 443
    0xc5, // STORE_FAST 5
    0x10,0x53, // LOAD_CONST_STRING ':'
    0xb3, // LOAD_FAST 3
    0xdd, // BINARY_OP 6 <in>
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0xb3, // LOAD_FAST 3
    0x14,0x34, // LOAD_METHOD 'split'
    0x10,0x53, // LOAD_CONST_STRING ':'
    0x36,0x01, // CALL_METHOD 1
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc5, // STORE_FAST 5
    0x12,0x54, // LOAD_GLOBAL 'int'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0xc5, // STORE_FAST 5
    0x12,0x1d, // LOAD_GLOBAL 'usocket'
    0x14,0x55, // LOAD_METHOD 'getaddrinfo'
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x1d, // LOAD_GLOBAL 'usocket'
    0x13,0x56, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0xc6, // STORE_FAST 6
    0x4a,0x1c, // POP_EXCEPT_JUMP 28
    0x57, // DUP_TOP
    0x12,0x36, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0xc7, // STORE_FAST 7
    0x49,0x0b, // SETUP_FINALLY 11
    0x12,0x21, // LOAD_GLOBAL 'fatal'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xb3, // LOAD_FAST 3
    0xf8, // BINARY_OP 33 __mod__
    0xb7, // LOAD_FAST 7
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x12,0x1d, // LOAD_GLOBAL 'usocket'
    0x14,0x57, // LOAD_METHOD 'socket'
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb6, // LOAD_FAST 6
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x36,0x03, // CALL_METHOD 3
    0xc8, // STORE_FAST 8
    0x48,0x86,0x01, // SETUP_EXCEPT 134
    0xb8, // LOAD_FAST 8
    0x14,0x58, // LOAD_METHOD 'connect'
    0xb6, // LOAD_FAST 6
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x10,0x59, // LOAD_CONST_STRING 'https:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5c, // POP_JUMP_IF_FALSE 28
    0x12,0x1c, // LOAD_GLOBAL 'ussl'
    0x14,0x5a, // LOAD_METHOD 'wrap_socket'
    0xb8, // LOAD_FAST 8
    0x10,0x5b, // LOAD_CONST_STRING 'server_hostname'
    0xb3, // LOAD_FAST 3
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc8, // STORE_FAST 8
    0x12,0x1e, // LOAD_GLOBAL 'warn_ussl'
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb3, // LOAD_FAST 3
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x17,0x1e, // STORE_GLOBAL 'warn_ussl'
    0xb8, // LOAD_FAST 8
    0x14,0x3e, // LOAD_METHOD 'write'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0xb4, // LOAD_FAST 4
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb8, // LOAD_FAST 8
    0x14,0x5c, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x14,0x34, // LOAD_METHOD 'split'
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xca, // STORE_FAST 10
    0xcb, // STORE_FAST 11
    0xcc, // STORE_FAST 12
    0xbb, // LOAD_FAST 11
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xbb, // LOAD_FAST 11
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0xbb, // LOAD_FAST 11
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x15, // LOAD_GLOBAL 'NotFoundError'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x42, // LOAD_GLOBAL 'ValueError'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb8, // LOAD_FAST 8
    0x14,0x5c, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x42, // LOAD_GLOBAL 'ValueError'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb9, // LOAD_FAST 9
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x42, // JUMP 2
    0x42,0x26, // JUMP -26
    0x4a,0x1a, // POP_EXCEPT_JUMP 26
    0x57, // DUP_TOP
    0x12,0x16, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0xc7, // STORE_FAST 7
    0x49,0x09, // SETUP_FINALLY 9
    0xb8, // LOAD_FAST 8
    0x14,0x5d, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb8, // LOAD_FAST 8
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_url_open = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_url_open,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 324,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 19,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_upip_url_open + 5,
        .opcodes = fun_data_upip_url_open + 42,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_get_pkg_metadata
static const byte fun_data_upip_get_pkg_metadata[77] = {
    0x65,0x16, // prelude
    0x20,0x31, // names: get_pkg_metadata, name
    0x80,0xaa,0x26,0x22,0x55,0x26,0x22,0x49,0x29, // code info
    0x12,0x0f, // LOAD_GLOBAL 'index_urls'
    0x5f, // GET_ITER_STACK
    0x4b,0x32, // FOR_ITER 50
    0xc1, // STORE_FAST 1
    0x48,0x0e, // SETUP_EXCEPT 14
    0x12,0x1f, // LOAD_GLOBAL 'url_open'
    0x10,0x5e, // LOAD_CONST_STRING '%s/%s/json'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x4a,0x0d, // POP_EXCEPT_JUMP 13
    0x57, // DUP_TOP
    0x12,0x15, // LOAD_GLOBAL 'NotFoundError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x59, // POP_TOP
    0x40,0x24,0x01, // UNWIND_JUMP -28
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x49,0x09, // SETUP_FINALLY 9
    0x12,0x09, // LOAD_GLOBAL 'json'
    0x14,0x5f, // LOAD_METHOD 'load'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xb2, // LOAD_FAST 2
    0x14,0x5d, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0x42,0x0c, // JUMP -52
    0x12,0x15, // LOAD_GLOBAL 'NotFoundError'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_get_pkg_metadata = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_get_pkg_metadata,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 77,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 32,
        .line_info = fun_data_upip_get_pkg_metadata + 4,
        .opcodes = fun_data_upip_get_pkg_metadata + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_fatal
static const byte fun_data_upip_fatal[38] = {
    0xa2,0x01,0x10, // prelude
    0x21,0x60,0x61, // names: fatal, msg, exc
    0x80,0xb7,0x28,0x27,0x22, // code info
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x10,0x62, // LOAD_CONST_STRING 'Error:'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x65, // RAISE_OBJ
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x14,0x63, // LOAD_METHOD 'exit'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_fatal = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_fatal,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 38,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 33,
        .line_info = fun_data_upip_fatal + 6,
        .opcodes = fun_data_upip_fatal + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_install_pkg
static const byte fun_data_upip_install_pkg[172] = {
    0x8e,0x10,0x32, // prelude
    0x22,0x64,0x10, // names: install_pkg, pkg_spec, install_path
    0x80,0xbe,0x28,0x48,0x29,0x4a,0x24,0x27,0x22,0x27,0x2c,0x27,0x2d,0x26,0x22,0x2a,0x2b,0x48,0x27,0x22,0x22,0x27, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x34, // LOAD_METHOD 'split'
    0x10,0x65, // LOAD_CONST_STRING '=='
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x20, // LOAD_GLOBAL 'get_pkg_metadata'
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x30, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb3, // LOAD_FAST 3
    0x10,0x66, // LOAD_CONST_STRING 'info'
    0x55, // LOAD_SUBSCR
    0x10,0x67, // LOAD_CONST_STRING 'version'
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x42,0x44, // JUMP 4
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x10,0x68, // LOAD_CONST_STRING 'releases'
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0x28,0x03, // DELETE_FAST 3
    0x12,0x03, // LOAD_GLOBAL 'gc'
    0x14,0x0d, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x30, // LOAD_GLOBAL 'len'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x69, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x52, // LOAD_CONST_STRING 'url'
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0xb0, // LOAD_FAST 0
    0xb4, // LOAD_FAST 4
    0xb6, // LOAD_FAST 6
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'url_open'
    0xb6, // LOAD_FAST 6
    0x34,0x01, // CALL_FUNCTION 1
    0xc7, // STORE_FAST 7
    0x49,0x1d, // SETUP_FINALLY 29
    0x12,0x0a, // LOAD_GLOBAL 'uzlib'
    0x14,0x6a, // LOAD_METHOD 'DecompIO'
    0xb7, // LOAD_FAST 7
    0x12,0x12, // LOAD_GLOBAL 'gzdict_sz'
    0x36,0x02, // CALL_METHOD 2
    0xc8, // STORE_FAST 8
    0x12,0x0c, // LOAD_GLOBAL 'tarfile'
    0x14,0x6b, // LOAD_METHOD 'TarFile'
    0x10,0x6c, // LOAD_CONST_STRING 'fileobj'
    0xb8, // LOAD_FAST 8
    0x36,0x82,0x00, // CALL_METHOD 256
    0xc9, // STORE_FAST 9
    0x12,0x1a, // LOAD_GLOBAL 'install_tar'
    0xb9, // LOAD_FAST 9
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0xca, // STORE_FAST 10
    0x51, // LOAD_CONST_NONE
    0xb7, // LOAD_FAST 7
    0x14,0x5d, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0x28,0x09, // DELETE_FAST 9
    0x28,0x08, // DELETE_FAST 8
    0x12,0x03, // LOAD_GLOBAL 'gc'
    0x14,0x0d, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xba, // LOAD_FAST 10
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_install_pkg = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_install_pkg,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 172,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upip_install_pkg + 6,
        .opcodes = fun_data_upip_install_pkg + 28,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_install
static const byte fun_data_upip_install[249] = {
    0xfa,0x03,0x44, // prelude
    0x23,0x6d,0x10, // names: install, to_install, install_path
    0x80,0xda,0x20,0x2e,0x28,0x43,0x25,0x25,0x28,0x25,0x29,0x24,0x49,0x23,0x22,0x23,0x24,0x28,0x27,0x25,0x22,0x27,0x27,0x24,0x26,0x2e,0x23,0x2e,0x56,0x22,0x2a, // code info
    0x12,0x03, // LOAD_GLOBAL 'gc'
    0x14,0x6e, // LOAD_METHOD 'mem_free'
    0x36,0x00, // CALL_METHOD 0
    0x12,0x03, // LOAD_GLOBAL 'gc'
    0x14,0x6f, // LOAD_METHOD 'mem_alloc'
    0x36,0x00, // CALL_METHOD 0
    0xf2, // BINARY_OP 27 __add__
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x22,0x84,0x80,0x00, // LOAD_CONST_SMALL_INT 65536
    0xda, // BINARY_OP 3 __le__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x9c, // LOAD_CONST_SMALL_INT 28
    0x17,0x12, // STORE_GLOBAL 'gzdict_sz'
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x12,0x24, // LOAD_GLOBAL 'get_install_path'
    0x34,0x00, // CALL_FUNCTION 0
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb1, // LOAD_FAST 1
    0x10,0x2f, // LOAD_CONST_STRING '/'
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0x12,0x70, // LOAD_GLOBAL 'isinstance'
    0xb0, // LOAD_FAST 0
    0x12,0x71, // LOAD_GLOBAL 'list'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x44, // POP_JUMP_IF_TRUE 4
    0xb0, // LOAD_FAST 0
    0x2b,0x01, // BUILD_LIST 1
    0xc0, // STORE_FAST 0
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x2b,0x00, // BUILD_LIST 0
    0xc3, // STORE_FAST 3
    0x48,0x61, // SETUP_EXCEPT 97
    0x42,0xd8,0x80, // JUMP 88
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x10,0x72, // LOAD_CONST_STRING 'Queue:'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x73, // LOAD_METHOD 'pop'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0xb3, // LOAD_FAST 3
    0xdd, // BINARY_OP 6 <in>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x7e, // JUMP 62
    0x12,0x22, // LOAD_GLOBAL 'install_pkg'
    0xb4, // LOAD_FAST 4
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0xb3, // LOAD_FAST 3
    0x14,0x74, // LOAD_METHOD 'append'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x49, // LOAD_GLOBAL 'print'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x75, // LOAD_METHOD 'get'
    0x10,0x48, // LOAD_CONST_STRING 'deps'
    0x10,0x2d, // LOAD_CONST_STRING ''
    0x36,0x02, // CALL_METHOD 2
    0x14,0x33, // LOAD_METHOD 'rstrip'
    0x36,0x00, // CALL_METHOD 0
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0xb6, // LOAD_FAST 6
    0x14,0x76, // LOAD_METHOD 'decode'
    0x10,0x77, // LOAD_CONST_STRING 'utf-8'
    0x36,0x01, // CALL_METHOD 1
    0x14,0x34, // LOAD_METHOD 'split'
    0x10,0x78, // LOAD_CONST_STRING '\n'
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb0, // LOAD_FAST 0
    0x14,0x79, // LOAD_METHOD 'extend'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x43,0xa4,0x7f, // POP_JUMP_IF_TRUE -92
    0x4a,0x26, // POP_EXCEPT_JUMP 38
    0x57, // DUP_TOP
    0x12,0x16, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5f, // POP_JUMP_IF_FALSE 31
    0xc7, // STORE_FAST 7
    0x49,0x15, // SETUP_FINALLY 21
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x14,0x7a, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0xb7, // LOAD_FAST 7
    0x36,0x02, // CALL_METHOD 2
    0x10,0x7b, // LOAD_CONST_STRING 'file'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7c, // LOAD_ATTR 'stderr'
    0x34,0x82,0x01, // CALL_FUNCTION 257
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_install = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_install,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 249,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upip_install + 6,
        .opcodes = fun_data_upip_install + 37,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_get_install_path
static const byte fun_data_upip_get_install_path[51] = {
    0x08,0x12, // prelude
    0x24, // names: get_install_path
    0x80,0xff,0x20,0x46,0x28,0x27,0x28,0x28, // code info
    0x12,0x10, // LOAD_GLOBAL 'install_path'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x2c, // LOAD_ATTR 'path'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0x12,0x10, // LOAD_GLOBAL 'install_path'
    0x10,0x7d, // LOAD_CONST_STRING '.frozen'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x2c, // LOAD_ATTR 'path'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0x12,0x1b, // LOAD_GLOBAL 'expandhome'
    0x12,0x10, // LOAD_GLOBAL 'install_path'
    0x34,0x01, // CALL_FUNCTION 1
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0x12,0x10, // LOAD_GLOBAL 'install_path'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_get_install_path = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_get_install_path,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 51,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 36,
        .line_info = fun_data_upip_get_install_path + 3,
        .opcodes = fun_data_upip_get_install_path + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_cleanup
static const byte fun_data_upip_cleanup[49] = {
    0x54,0x0c, // prelude
    0x25, // names: cleanup
    0x90,0x0a,0x26,0x22,0x51, // code info
    0x12,0x11, // LOAD_GLOBAL 'cleanup_files'
    0x5f, // GET_ITER_STACK
    0x4b,0x22, // FOR_ITER 34
    0xc0, // STORE_FAST 0
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x05, // LOAD_GLOBAL 'os'
    0x14,0x7e, // LOAD_METHOD 'unlink'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x13, // POP_EXCEPT_JUMP 19
    0x57, // DUP_TOP
    0x12,0x36, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x13, // LOAD_CONST_OBJ 19
    0xb0, // LOAD_FAST 0
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x1c, // JUMP -36
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_cleanup = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_cleanup,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 49,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 37,
        .line_info = fun_data_upip_cleanup + 3,
        .opcodes = fun_data_upip_cleanup + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_help
static const byte fun_data_upip_help[37] = {
    0x10,0x10, // prelude
    0x26, // names: help
    0x90,0x12,0x22,0x85,0x09,0x2b,0x22, // code info
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x14, // LOAD_CONST_OBJ 20
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x12,0x24, // LOAD_GLOBAL 'get_install_path'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'print'
    0x23,0x16, // LOAD_CONST_OBJ 22
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_help = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_help,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_upip_help + 3,
        .opcodes = fun_data_upip_help + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip__lt_module_gt_
// frozen bytecode for file upip.py, scope upip_main
static const byte fun_data_upip_main[355] = {
    0x5c,0x60, // prelude
    0x27, // names: main
    0x90,0x27,0x20,0x20,0x20,0x43,0x1f,0x25,0x25,0x42,0x2b,0x47,0x43,0x22,0x23,0x27,0x24,0x2e,0x25,0x22,0x27,0x28,0x27,0x27,0x27,0x24,0x28,0x20,0x26,0x23,0x22,0x29,0x22,0x32,0x27,0x2a,0x26,0x27,0x45,0x1f,0x45,0x2f,0x23,0x25,0x42,0x46,0x24, // code info
    0x51, // LOAD_CONST_NONE
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0x12,0x30, // LOAD_GLOBAL 'len'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x10,0x81,0x00, // LOAD_CONST_STRING '-h'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x10,0x81,0x01, // LOAD_CONST_STRING '--help'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x26, // LOAD_GLOBAL 'help'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x10,0x23, // LOAD_CONST_STRING 'install'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x21, // LOAD_GLOBAL 'fatal'
    0x23,0x17, // LOAD_CONST_OBJ 23
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x2b,0x00, // BUILD_LIST 0
    0xc0, // STORE_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0xc1, // STORE_FAST 1
    0x42,0xa4,0x81, // JUMP 164
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x00, // LOAD_CONST_STRING '-h'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x01, // LOAD_CONST_STRING '--help'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x26, // LOAD_GLOBAL 'help'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x02, // LOAD_CONST_STRING '-p'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x17,0x10, // STORE_GLOBAL 'install_path'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0x42,0xee,0x80, // JUMP 110
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x03, // LOAD_CONST_STRING '-r'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x7b, // POP_JUMP_IF_FALSE 59
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0x12,0x3b, // LOAD_GLOBAL 'open'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x25, // SETUP_WITH 37
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x5c, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x58, // JUMP 24
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x81,0x04, // LOAD_CONST_STRING '#'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x4b, // JUMP 11
    0xb0, // LOAD_FAST 0
    0x14,0x74, // LOAD_METHOD 'append'
    0xb5, // LOAD_FAST 5
    0x14,0x33, // LOAD_METHOD 'rstrip'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x1d, // JUMP -35
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x42,0x6c, // JUMP 44
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x05, // LOAD_CONST_STRING '-i'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x2b,0x01, // BUILD_LIST 1
    0x17,0x0f, // STORE_GLOBAL 'index_urls'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0x42,0x55, // JUMP 21
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x06, // LOAD_CONST_STRING '--debug'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x52, // LOAD_CONST_TRUE
    0x17,0x0e, // STORE_GLOBAL 'debug'
    0x42,0x49, // JUMP 9
    0x12,0x21, // LOAD_GLOBAL 'fatal'
    0x23,0x18, // LOAD_CONST_OBJ 24
    0xb2, // LOAD_FAST 2
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x12,0x30, // LOAD_GLOBAL 'len'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0x34,0x01, // CALL_FUNCTION 1
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x81,0x07, // LOAD_CONST_STRING '-'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0xc1,0x7e, // POP_JUMP_IF_TRUE -191
    0xb0, // LOAD_FAST 0
    0x14,0x79, // LOAD_METHOD 'extend'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x7f, // LOAD_ATTR 'argv'
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x26, // LOAD_GLOBAL 'help'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x23, // LOAD_GLOBAL 'install'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x0e, // LOAD_GLOBAL 'debug'
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0x12,0x25, // LOAD_GLOBAL 'cleanup'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_main = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_main,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 355,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_upip_main + 3,
        .opcodes = fun_data_upip_main + 50,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upip__lt_module_gt_[] = {
    &raw_code_upip_NotFoundError,
    &raw_code_upip_op_split,
    &raw_code_upip__makedirs,
    &raw_code_upip_save_file,
    &raw_code_upip_install_tar,
    &raw_code_upip_expandhome,
    &raw_code_upip_url_open,
    &raw_code_upip_get_pkg_metadata,
    &raw_code_upip_fatal,
    &raw_code_upip_install_pkg,
    &raw_code_upip_install,
    &raw_code_upip_get_install_path,
    &raw_code_upip_cleanup,
    &raw_code_upip_help,
    &raw_code_upip_main,
};

static const mp_raw_code_t raw_code_upip__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 242,
    #endif
    .children = (void *)&children_upip__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 15,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_upip__lt_module_gt_ + 3,
        .opcodes = fun_data_upip__lt_module_gt_ + 53,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_upip[136] = {
    MP_QSTR_upip_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_gc,
    MP_QSTR_uos,
    MP_QSTR_os,
    MP_QSTR_uerrno,
    MP_QSTR_errno,
    MP_QSTR_ujson,
    MP_QSTR_json,
    MP_QSTR_uzlib,
    MP_QSTR_upip_utarfile,
    MP_QSTR_tarfile,
    MP_QSTR_collect,
    MP_QSTR_debug,
    MP_QSTR_index_urls,
    MP_QSTR_install_path,
    MP_QSTR_cleanup_files,
    MP_QSTR_gzdict_sz,
    MP_QSTR_bytearray,
    MP_QSTR_file_buf,
    MP_QSTR_NotFoundError,
    MP_QSTR_Exception,
    MP_QSTR_op_split,
    MP_QSTR__makedirs,
    MP_QSTR_save_file,
    MP_QSTR_install_tar,
    MP_QSTR_expandhome,
    MP_QSTR_ussl,
    MP_QSTR_usocket,
    MP_QSTR_warn_ussl,
    MP_QSTR_url_open,
    MP_QSTR_get_pkg_metadata,
    MP_QSTR_fatal,
    MP_QSTR_install_pkg,
    MP_QSTR_install,
    MP_QSTR_get_install_path,
    MP_QSTR_cleanup,
    MP_QSTR_help,
    MP_QSTR_main,
    MP_QSTR___name__,
    MP_QSTR___main__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_path,
    MP_QSTR_,
    MP_QSTR_rsplit,
    MP_QSTR__slash_,
    MP_QSTR_len,
    MP_QSTR_name,
    MP_QSTR_mode,
    MP_QSTR_rstrip,
    MP_QSTR_split,
    MP_QSTR_mkdir,
    MP_QSTR_OSError,
    MP_QSTR_EEXIST,
    MP_QSTR_EISDIR,
    MP_QSTR_fname,
    MP_QSTR_subf,
    MP_QSTR_open,
    MP_QSTR_wb,
    MP_QSTR_readinto,
    MP_QSTR_write,
    MP_QSTR_f,
    MP_QSTR_prefix,
    MP_QSTR_index,
    MP_QSTR_ValueError,
    MP_QSTR_startswith,
    MP_QSTR__dot_egg_hyphen_info,
    MP_QSTR_endswith,
    MP_QSTR_extractfile,
    MP_QSTR_read,
    MP_QSTR_deps,
    MP_QSTR_print,
    MP_QSTR_Skipping,
    MP_QSTR_type,
    MP_QSTR_DIRTYPE,
    MP_QSTR_s,
    MP_QSTR__tilde__slash_,
    MP_QSTR_getenv,
    MP_QSTR_HOME,
    MP_QSTR_replace,
    MP_QSTR_url,
    MP_QSTR__colon_,
    MP_QSTR_int,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_socket,
    MP_QSTR_connect,
    MP_QSTR_https_colon_,
    MP_QSTR_wrap_socket,
    MP_QSTR_server_hostname,
    MP_QSTR_readline,
    MP_QSTR_close,
    MP_QSTR__percent_s_slash__percent_s_slash_json,
    MP_QSTR_load,
    MP_QSTR_msg,
    MP_QSTR_exc,
    MP_QSTR_Error_colon_,
    MP_QSTR_exit,
    MP_QSTR_pkg_spec,
    MP_QSTR__equals__equals_,
    MP_QSTR_info,
    MP_QSTR_version,
    MP_QSTR_releases,
    MP_QSTR_AssertionError,
    MP_QSTR_DecompIO,
    MP_QSTR_TarFile,
    MP_QSTR_fileobj,
    MP_QSTR_to_install,
    MP_QSTR_mem_free,
    MP_QSTR_mem_alloc,
    MP_QSTR_isinstance,
    MP_QSTR_list,
    MP_QSTR_Queue_colon_,
    MP_QSTR_pop,
    MP_QSTR_append,
    MP_QSTR_get,
    MP_QSTR_decode,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR__0x0a_,
    MP_QSTR_extend,
    MP_QSTR_format,
    MP_QSTR_file,
    MP_QSTR_stderr,
    MP_QSTR__dot_frozen,
    MP_QSTR_unlink,
    MP_QSTR_argv,
    MP_QSTR__hyphen_h,
    MP_QSTR__hyphen__hyphen_help,
    MP_QSTR__hyphen_p,
    MP_QSTR__hyphen_r,
    MP_QSTR__hash_,
    MP_QSTR__hyphen_i,
    MP_QSTR__hyphen__hyphen_debug,
    MP_QSTR__hyphen_,
};

// constants
static const mp_obj_str_t const_obj_upip_0 = {{&mp_type_str}, 58328, 26, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x6d\x69\x63\x72\x6f\x70\x79\x74\x68\x6f\x6e\x2e\x6f\x72\x67\x2f\x70\x69"};
static const mp_rom_obj_tuple_t const_obj_upip_2 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_),
    MP_ROM_QSTR(MP_QSTR_),
}};
static const mp_rom_obj_tuple_t const_obj_upip_3 = {{&mp_type_tuple}, 3, {
    MP_ROM_QSTR(MP_QSTR_setup_dot_),
    MP_ROM_QSTR(MP_QSTR_PKG_hyphen_INFO),
    MP_ROM_QSTR(MP_QSTR_README),
}};
static const mp_obj_str_t const_obj_upip_6 = {{&mp_type_str}, 10889, 35, (const byte*)"\x55\x6e\x61\x62\x6c\x65\x20\x74\x6f\x20\x72\x65\x73\x6f\x6c\x76\x65\x20\x25\x73\x20\x28\x6e\x6f\x20\x49\x6e\x74\x65\x72\x6e\x65\x74\x3f\x29"};
static const mp_obj_str_t const_obj_upip_7 = {{&mp_type_str}, 51159, 44, (const byte*)"\x57\x61\x72\x6e\x69\x6e\x67\x3a\x20\x25\x73\x20\x53\x53\x4c\x20\x63\x65\x72\x74\x69\x66\x69\x63\x61\x74\x65\x20\x69\x73\x20\x6e\x6f\x74\x20\x76\x61\x6c\x69\x64\x61\x74\x65\x64"};
static const mp_obj_str_t const_obj_upip_8 = {{&mp_type_str}, 65109, 33, (const byte*)"\x47\x45\x54\x20\x2f\x25\x73\x20\x48\x54\x54\x50\x2f\x31\x2e\x30\x0d\x0a\x48\x6f\x73\x74\x3a\x20\x25\x73\x3a\x25\x73\x0d\x0a\x0d\x0a"};
static const mp_obj_str_t const_obj_upip_9 = {{&mp_type_bytes}, 28951, 3, (const byte*)"\x32\x30\x30"};
static const mp_obj_str_t const_obj_upip_10 = {{&mp_type_bytes}, 22421, 3, (const byte*)"\x34\x30\x34"};
static const mp_obj_str_t const_obj_upip_11 = {{&mp_type_bytes}, 29975, 3, (const byte*)"\x33\x30\x31"};
static const mp_obj_str_t const_obj_upip_13 = {{&mp_type_str}, 39199, 30, (const byte*)"\x55\x6e\x65\x78\x70\x65\x63\x74\x65\x64\x20\x45\x4f\x46\x20\x69\x6e\x20\x48\x54\x54\x50\x20\x68\x65\x61\x64\x65\x72\x73"};
static const mp_obj_str_t const_obj_upip_14 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};
static const mp_obj_str_t const_obj_upip_18 = {{&mp_type_str}, 31671, 62, (const byte*)"\x45\x72\x72\x6f\x72\x20\x69\x6e\x73\x74\x61\x6c\x6c\x69\x6e\x67\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d\x2c\x20\x70\x61\x63\x6b\x61\x67\x65\x73\x20\x6d\x61\x79\x20\x62\x65\x20\x70\x61\x72\x74\x69\x61\x6c\x6c\x79\x20\x69\x6e\x73\x74\x61\x6c\x6c\x65\x64"};
static const mp_obj_str_t const_obj_upip_20 = {{&mp_type_str}, 25874, 364, (const byte*)"\x75\x70\x69\x70\x20\x2d\x20\x53\x69\x6d\x70\x6c\x65\x20\x50\x79\x50\x49\x20\x70\x61\x63\x6b\x61\x67\x65\x20\x6d\x61\x6e\x61\x67\x65\x72\x20\x66\x6f\x72\x20\x4d\x69\x63\x72\x6f\x50\x79\x74\x68\x6f\x6e\x0a\x55\x73\x61\x67\x65\x3a\x20\x6d\x69\x63\x72\x6f\x70\x79\x74\x68\x6f\x6e\x20\x2d\x6d\x20\x75\x70\x69\x70\x20\x69\x6e\x73\x74\x61\x6c\x6c\x20\x5b\x2d\x70\x20\x3c\x70\x61\x74\x68\x3e\x5d\x20\x3c\x70\x61\x63\x6b\x61\x67\x65\x3e\x2e\x2e\x2e\x20\x7c\x20\x2d\x72\x20\x3c\x72\x65\x71\x75\x69\x72\x65\x6d\x65\x6e\x74\x73\x2e\x74\x78\x74\x3e\x0a\x69\x6d\x70\x6f\x72\x74\x20\x75\x70\x69\x70\x3b\x20\x75\x70\x69\x70\x2e\x69\x6e\x73\x74\x61\x6c\x6c\x28\x70\x61\x63\x6b\x61\x67\x65\x5f\x6f\x72\x5f\x6c\x69\x73\x74\x2c\x20\x5b\x3c\x70\x61\x74\x68\x3e\x5d\x29\x0a\x0a\x49\x66\x20\x3c\x70\x61\x74\x68\x3e\x20\x69\x73\x6e\x27\x74\x20\x67\x69\x76\x65\x6e\x2c\x20\x70\x61\x63\x6b\x61\x67\x65\x73\x20\x77\x69\x6c\x6c\x20\x62\x65\x20\x69\x6e\x73\x74\x61\x6c\x6c\x65\x64\x20\x74\x6f\x20\x73\x79\x73\x2e\x70\x61\x74\x68\x5b\x31\x5d\x2c\x20\x6f\x72\x0a\x73\x79\x73\x2e\x70\x61\x74\x68\x5b\x32\x5d\x20\x69\x66\x20\x74\x68\x65\x20\x66\x6f\x72\x6d\x65\x72\x20\x69\x73\x20\x2e\x66\x72\x6f\x7a\x65\x6e\x20\x28\x70\x61\x74\x68\x20\x63\x61\x6e\x20\x62\x65\x20\x73\x65\x74\x20\x66\x72\x6f\x6d\x20\x4d\x49\x43\x52\x4f\x50\x59\x50\x41\x54\x48\x0a\x65\x6e\x76\x69\x72\x6f\x6e\x6d\x65\x6e\x74\x20\x76\x61\x72\x69\x61\x62\x6c\x65\x20\x69\x66\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x29\x2e"};
static const mp_obj_str_t const_obj_upip_22 = {{&mp_type_str}, 34882, 146, (const byte*)"\x0a\x4e\x6f\x74\x65\x3a\x20\x6f\x6e\x6c\x79\x20\x4d\x69\x63\x72\x6f\x50\x79\x74\x68\x6f\x6e\x20\x70\x61\x63\x6b\x61\x67\x65\x73\x20\x28\x75\x73\x75\x61\x6c\x6c\x79\x2c\x20\x6e\x61\x6d\x65\x64\x20\x6d\x69\x63\x72\x6f\x70\x79\x74\x68\x6f\x6e\x2d\x2a\x29\x20\x61\x72\x65\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x0a\x66\x6f\x72\x20\x69\x6e\x73\x74\x61\x6c\x6c\x61\x74\x69\x6f\x6e\x2c\x20\x75\x70\x69\x70\x20\x64\x6f\x65\x73\x20\x6e\x6f\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x20\x61\x72\x62\x69\x74\x72\x61\x72\x79\x20\x63\x6f\x64\x65\x20\x69\x6e\x20\x73\x65\x74\x75\x70\x2e\x70\x79\x2e\x0a"};
static const mp_obj_str_t const_obj_upip_23 = {{&mp_type_str}, 50937, 32, (const byte*)"\x4f\x6e\x6c\x79\x20\x27\x69\x6e\x73\x74\x61\x6c\x6c\x27\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64"};
static const mp_obj_str_t const_obj_upip_24 = {{&mp_type_str}, 40302, 28, (const byte*)"\x55\x6e\x6b\x6e\x6f\x77\x6e\x2f\x75\x6e\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x20\x6f\x70\x74\x69\x6f\x6e\x3a\x20"};

// constant table
static const mp_rom_obj_t const_obj_table_data_upip[25] = {
    MP_ROM_PTR(&const_obj_upip_0),
    MP_ROM_QSTR(MP_QSTR_https_colon__slash__slash_pypi_dot_org_slash_pypi),
    MP_ROM_PTR(&const_obj_upip_2),
    MP_ROM_PTR(&const_obj_upip_3),
    MP_ROM_QSTR(MP_QSTR__slash_requires_dot_txt),
    MP_ROM_QSTR(MP_QSTR_Extracting_space_),
    MP_ROM_PTR(&const_obj_upip_6),
    MP_ROM_PTR(&const_obj_upip_7),
    MP_ROM_PTR(&const_obj_upip_8),
    MP_ROM_PTR(&const_obj_upip_9),
    MP_ROM_PTR(&const_obj_upip_10),
    MP_ROM_PTR(&const_obj_upip_11),
    MP_ROM_QSTR(MP_QSTR_Package_space_not_space_found),
    MP_ROM_PTR(&const_obj_upip_13),
    MP_ROM_PTR(&const_obj_upip_14),
    MP_ROM_QSTR(MP_QSTR_Package_space_not_space_found),
    MP_ROM_QSTR(MP_QSTR_Installing_space__percent_s_space__percent_s_space_from_space__percent_s),
    MP_ROM_QSTR(MP_QSTR_Installing_space_to_colon__space_),
    MP_ROM_PTR(&const_obj_upip_18),
    MP_ROM_QSTR(MP_QSTR_Warning_colon__space_Cannot_space_delete_space_),
    MP_ROM_PTR(&const_obj_upip_20),
    MP_ROM_QSTR(MP_QSTR_Default_space_install_space_path_colon_),
    MP_ROM_PTR(&const_obj_upip_22),
    MP_ROM_PTR(&const_obj_upip_23),
    MP_ROM_PTR(&const_obj_upip_24),
};

static const mp_frozen_module_t frozen_module_upip = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_upip,
        .obj_table = (mp_obj_t *)&const_obj_table_data_upip,
    },
    .rc = &raw_code_upip__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module upip_utarfile
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/upip_utarfile.mpy
// - frozen file name: upip_utarfile.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file upip_utarfile.py, scope upip_utarfile__lt_module_gt_
static const byte fun_data_upip_utarfile__lt_module_gt_[104] = {
    0x18,0x1a, // prelude
    0x01, // names: <module>
    0x66,0x22,0x33,0x75,0x24,0x64,0x64,0x20,0x89,0x23,0x69,0x40, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'uctypes'
    0x16,0x02, // STORE_NAME 'uctypes'
    0x2c,0x02, // BUILD_MAP 2
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x03, // LOAD_ATTR 'ARRAY'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'UINT8'
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x05, // LOAD_CONST_STRING 'name'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x03, // LOAD_ATTR 'ARRAY'
    0x22,0x80,0x7c, // LOAD_CONST_SMALL_INT 124
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'UINT8'
    0x8b, // LOAD_CONST_SMALL_INT 11
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x06, // LOAD_CONST_STRING 'size'
    0x62, // STORE_MAP
    0x16,0x07, // STORE_NAME 'TAR_HEADER'
    0x10,0x08, // LOAD_CONST_STRING 'dir'
    0x16,0x09, // STORE_NAME 'DIRTYPE'
    0x10,0x0a, // LOAD_CONST_STRING 'file'
    0x16,0x0b, // STORE_NAME 'REGTYPE'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0c, // STORE_NAME 'roundup'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0d, // LOAD_CONST_STRING 'FileSection'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'FileSection'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x0e, // LOAD_CONST_STRING 'TarInfo'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0e, // STORE_NAME 'TarInfo'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x0f, // LOAD_CONST_STRING 'TarFile'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0f, // STORE_NAME 'TarFile'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upip_utarfile__lt_module_gt_
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_roundup
static const byte fun_data_upip_utarfile_roundup[18] = {
    0x22,0x0a, // prelude
    0x0c,0x10,0x11, // names: roundup, val, align
    0x80,0x0d, // code info
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0xd2, // UNARY_OP 2 __invert__
    0xef, // BINARY_OP 24 __and__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_roundup = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_utarfile_roundup,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_upip_utarfile_roundup + 5,
        .opcodes = fun_data_upip_utarfile_roundup + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile__lt_module_gt_
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_FileSection
static const byte fun_data_upip_utarfile_FileSection[44] = {
    0x08,0x12, // prelude
    0x0d, // names: FileSection
    0x88,0x11,0x64,0x40,0x8b,0x0a,0x84,0x09, // code info
    0x11,0x12, // LOAD_NAME '__name__'
    0x16,0x13, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'FileSection'
    0x16,0x14, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x15, // STORE_NAME '__init__'
    0x22,0x84,0x80,0x00, // LOAD_CONST_SMALL_INT 65536
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x16, // STORE_NAME 'read'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x17, // STORE_NAME 'readinto'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x18, // STORE_NAME 'skip'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upip_utarfile_FileSection
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile___init__
static const byte fun_data_upip_utarfile___init__[28] = {
    0xa8,0x04,0x12, // prelude
    0x15,0x1e,0x1f,0x20,0x21, // names: __init__, self, f, content_len, aligned_len
    0x80,0x12,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x1f, // STORE_ATTR 'f'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x20, // STORE_ATTR 'content_len'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'align'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_upip_utarfile___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_upip_utarfile___init__ + 8,
        .opcodes = fun_data_upip_utarfile___init__ + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_FileSection
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_read
static const byte fun_data_upip_utarfile_read[62] = {
    0xaa,0x01,0x18, // prelude
    0x16,0x1e,0x22, // names: read, self, sz
    0x80,0x17,0x27,0x23,0x27,0x24,0x29,0x26,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x1f, // LOAD_ATTR 'f'
    0x14,0x16, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xb1, // LOAD_FAST 1
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x20, // STORE_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_utarfile_read,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 62,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_upip_utarfile_read + 6,
        .opcodes = fun_data_upip_utarfile_read + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_FileSection
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_readinto
static const byte fun_data_upip_utarfile_readinto[66] = {
    0x2a,0x16, // prelude
    0x17,0x1e,0x24, // names: readinto, self, buf
    0x80,0x21,0x27,0x22,0x2b,0x2d,0x29,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x63, // RETURN_VALUE
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x25, // LOAD_GLOBAL 'memoryview'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x1f, // LOAD_ATTR 'f'
    0x14,0x17, // LOAD_METHOD 'readinto'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x20, // STORE_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_readinto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_utarfile_readinto,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 66,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_upip_utarfile_readinto + 5,
        .opcodes = fun_data_upip_utarfile_readinto + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_FileSection
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_skip
static const byte fun_data_upip_utarfile_skip[57] = {
    0x39,0x14, // prelude
    0x18,0x1e, // names: skip, self
    0x80,0x2a,0x28,0x23,0x26,0x22,0x27,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR 'content_len'
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'align'
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0x12,0x26, // LOAD_GLOBAL 'bytearray'
    0x90, // LOAD_CONST_SMALL_INT 16
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x42,0x55, // JUMP 21
    0x12,0x27, // LOAD_GLOBAL 'min'
    0xb1, // LOAD_FAST 1
    0x90, // LOAD_CONST_SMALL_INT 16
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1f, // LOAD_ATTR 'f'
    0x14,0x17, // LOAD_METHOD 'readinto'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0xe6, // BINARY_OP 15 __isub__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x28, // POP_JUMP_IF_TRUE -24
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_skip = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_utarfile_skip,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 57,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_upip_utarfile_skip + 4,
        .opcodes = fun_data_upip_utarfile_skip + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upip_utarfile_FileSection[] = {
    &raw_code_upip_utarfile___init__,
    &raw_code_upip_utarfile_read,
    &raw_code_upip_utarfile_readinto,
    &raw_code_upip_utarfile_skip,
};

static const mp_raw_code_t raw_code_upip_utarfile_FileSection = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_utarfile_FileSection,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 44,
    #endif
    .children = (void *)&children_upip_utarfile_FileSection,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 4,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_upip_utarfile_FileSection + 3,
        .opcodes = fun_data_upip_utarfile_FileSection + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile__lt_module_gt_
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_TarInfo
static const byte fun_data_upip_utarfile_TarInfo[19] = {
    0x00,0x06, // prelude
    0x0e, // names: TarInfo
    0x88,0x34, // code info
    0x11,0x12, // LOAD_NAME '__name__'
    0x16,0x13, // STORE_NAME '__module__'
    0x10,0x0e, // LOAD_CONST_STRING 'TarInfo'
    0x16,0x14, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x19, // STORE_NAME '__str__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upip_utarfile_TarInfo
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile___str__
static const byte fun_data_upip_utarfile___str__[21] = {
    0x21,0x08, // prelude
    0x19,0x1e, // names: __str__, self
    0x80,0x35, // code info
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xb0, // LOAD_FAST 0
    0x13,0x05, // LOAD_ATTR 'name'
    0xb0, // LOAD_FAST 0
    0x13,0x28, // LOAD_ATTR 'type'
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'size'
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile___str__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_utarfile___str__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 21,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_upip_utarfile___str__ + 4,
        .opcodes = fun_data_upip_utarfile___str__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upip_utarfile_TarInfo[] = {
    &raw_code_upip_utarfile___str__,
};

static const mp_raw_code_t raw_code_upip_utarfile_TarInfo = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_utarfile_TarInfo,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = (void *)&children_upip_utarfile_TarInfo,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_upip_utarfile_TarInfo + 3,
        .opcodes = fun_data_upip_utarfile_TarInfo + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile__lt_module_gt_
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_TarFile
static const byte fun_data_upip_utarfile_TarFile[47] = {
    0x08,0x14, // prelude
    0x0f, // names: TarFile
    0x88,0x39,0x89,0x07,0x84,0x14,0x64,0x64,0x60, // code info
    0x11,0x12, // LOAD_NAME '__name__'
    0x16,0x13, // STORE_NAME '__module__'
    0x10,0x0f, // LOAD_CONST_STRING 'TarFile'
    0x16,0x14, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x15, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x1a, // STORE_NAME 'next'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1b, // STORE_NAME '__iter__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x1c, // STORE_NAME '__next__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x1d, // STORE_NAME 'extractfile'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upip_utarfile_TarFile
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile___init__2
static const byte fun_data_upip_utarfile___init__2[38] = {
    0xab,0x80,0x01,0x12, // prelude
    0x15,0x1e,0x05,0x29, // names: __init__, self, name, fileobj
    0x80,0x3a,0x23,0x46,0x2a, // code info
    0xb2, // LOAD_FAST 2
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x1f, // STORE_ATTR 'f'
    0x42,0x4a, // JUMP 10
    0x12,0x2a, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x2b, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0xb0, // LOAD_FAST 0
    0x18,0x1f, // STORE_ATTR 'f'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'subf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile___init__2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_upip_utarfile___init__2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 38,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_upip_utarfile___init__2 + 8,
        .opcodes = fun_data_upip_utarfile___init__2 + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_TarFile
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_next
static const byte fun_data_upip_utarfile_next[164] = {
    0x49,0x22, // prelude
    0x1a,0x1e, // names: next, self
    0x80,0x41,0x25,0x28,0x2b,0x23,0x42,0x74,0x29,0x42,0x25,0x32,0x2f,0x32,0x3b, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'subf'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'subf'
    0x14,0x18, // LOAD_METHOD 'skip'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x1f, // LOAD_ATTR 'f'
    0x14,0x16, // LOAD_METHOD 'read'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x2d, // LOAD_METHOD 'struct'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x2e, // LOAD_METHOD 'addressof'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x12,0x07, // LOAD_GLOBAL 'TAR_HEADER'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x13,0x2f, // LOAD_ATTR 'LITTLE_ENDIAN'
    0x36,0x03, // CALL_METHOD 3
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x13,0x05, // LOAD_ATTR 'name'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x0e, // LOAD_GLOBAL 'TarInfo'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0x12,0x30, // LOAD_GLOBAL 'str'
    0xb2, // LOAD_FAST 2
    0x13,0x05, // LOAD_ATTR 'name'
    0x10,0x31, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0x14,0x32, // LOAD_METHOD 'rstrip'
    0x10,0x33, // LOAD_CONST_STRING '\x00'
    0x36,0x01, // CALL_METHOD 1
    0xb3, // LOAD_FAST 3
    0x18,0x05, // STORE_ATTR 'name'
    0x12,0x34, // LOAD_GLOBAL 'int'
    0x12,0x35, // LOAD_GLOBAL 'bytes'
    0xb2, // LOAD_FAST 2
    0x13,0x06, // LOAD_ATTR 'size'
    0x34,0x01, // CALL_FUNCTION 1
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x02, // CALL_FUNCTION 2
    0xb3, // LOAD_FAST 3
    0x18,0x06, // STORE_ATTR 'size'
    0x12,0x0b, // LOAD_GLOBAL 'REGTYPE'
    0x12,0x09, // LOAD_GLOBAL 'DIRTYPE'
    0x2b,0x02, // BUILD_LIST 2
    0xb3, // LOAD_FAST 3
    0x13,0x05, // LOAD_ATTR 'name'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x10,0x36, // LOAD_CONST_STRING '/'
    0xd9, // BINARY_OP 2 __eq__
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x18,0x28, // STORE_ATTR 'type'
    0x12,0x0d, // LOAD_GLOBAL 'FileSection'
    0xb0, // LOAD_FAST 0
    0x13,0x1f, // LOAD_ATTR 'f'
    0xb3, // LOAD_FAST 3
    0x13,0x06, // LOAD_ATTR 'size'
    0x12,0x0c, // LOAD_GLOBAL 'roundup'
    0xb3, // LOAD_FAST 3
    0x13,0x06, // LOAD_ATTR 'size'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x03, // CALL_FUNCTION 3
    0x57, // DUP_TOP
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'subf'
    0xb3, // LOAD_FAST 3
    0x18,0x2c, // STORE_ATTR 'subf'
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_next = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_utarfile_next,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 164,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_upip_utarfile_next + 4,
        .opcodes = fun_data_upip_utarfile_next + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_TarFile
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile___iter__
static const byte fun_data_upip_utarfile___iter__[8] = {
    0x09,0x08, // prelude
    0x1b,0x1e, // names: __iter__, self
    0x80,0x55, // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile___iter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_utarfile___iter__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 8,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_upip_utarfile___iter__ + 4,
        .opcodes = fun_data_upip_utarfile___iter__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_TarFile
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile___next__
static const byte fun_data_upip_utarfile___next__[25] = {
    0x19,0x0e, // prelude
    0x1c,0x1e, // names: __next__, self
    0x80,0x58,0x26,0x25,0x23, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x1a, // LOAD_METHOD 'next'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x12,0x37, // LOAD_GLOBAL 'StopIteration'
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile___next__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upip_utarfile___next__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_upip_utarfile___next__ + 4,
        .opcodes = fun_data_upip_utarfile___next__ + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upip_utarfile_TarFile
// frozen bytecode for file upip_utarfile.py, scope upip_utarfile_extractfile
static const byte fun_data_upip_utarfile_extractfile[11] = {
    0x12,0x0a, // prelude
    0x1d,0x1e,0x38, // names: extractfile, self, tarinfo
    0x80,0x5e, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x2c, // LOAD_ATTR 'subf'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upip_utarfile_extractfile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upip_utarfile_extractfile,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 11,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_upip_utarfile_extractfile + 5,
        .opcodes = fun_data_upip_utarfile_extractfile + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upip_utarfile_TarFile[] = {
    &raw_code_upip_utarfile___init__2,
    &raw_code_upip_utarfile_next,
    &raw_code_upip_utarfile___iter__,
    &raw_code_upip_utarfile___next__,
    &raw_code_upip_utarfile_extractfile,
};

static const mp_raw_code_t raw_code_upip_utarfile_TarFile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_utarfile_TarFile,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 47,
    #endif
    .children = (void *)&children_upip_utarfile_TarFile,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_upip_utarfile_TarFile + 3,
        .opcodes = fun_data_upip_utarfile_TarFile + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upip_utarfile__lt_module_gt_[] = {
    &raw_code_upip_utarfile_roundup,
    &raw_code_upip_utarfile_FileSection,
    &raw_code_upip_utarfile_TarInfo,
    &raw_code_upip_utarfile_TarFile,
};

static const mp_raw_code_t raw_code_upip_utarfile__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upip_utarfile__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 104,
    #endif
    .children = (void *)&children_upip_utarfile__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 4,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_upip_utarfile__lt_module_gt_ + 3,
        .opcodes = fun_data_upip_utarfile__lt_module_gt_ + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_upip_utarfile[57] = {
    MP_QSTR_upip_utarfile_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_uctypes,
    MP_QSTR_ARRAY,
    MP_QSTR_UINT8,
    MP_QSTR_name,
    MP_QSTR_size,
    MP_QSTR_TAR_HEADER,
    MP_QSTR_dir,
    MP_QSTR_DIRTYPE,
    MP_QSTR_file,
    MP_QSTR_REGTYPE,
    MP_QSTR_roundup,
    MP_QSTR_FileSection,
    MP_QSTR_TarInfo,
    MP_QSTR_TarFile,
    MP_QSTR_val,
    MP_QSTR_align,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_read,
    MP_QSTR_readinto,
    MP_QSTR_skip,
    MP_QSTR___str__,
    MP_QSTR_next,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR_extractfile,
    MP_QSTR_self,
    MP_QSTR_f,
    MP_QSTR_content_len,
    MP_QSTR_aligned_len,
    MP_QSTR_sz,
    MP_QSTR_len,
    MP_QSTR_buf,
    MP_QSTR_memoryview,
    MP_QSTR_bytearray,
    MP_QSTR_min,
    MP_QSTR_type,
    MP_QSTR_fileobj,
    MP_QSTR_open,
    MP_QSTR_rb,
    MP_QSTR_subf,
    MP_QSTR_struct,
    MP_QSTR_addressof,
    MP_QSTR_LITTLE_ENDIAN,
    MP_QSTR_str,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR_rstrip,
    MP_QSTR__0x00_,
    MP_QSTR_int,
    MP_QSTR_bytes,
    MP_QSTR__slash_,
    MP_QSTR_StopIteration,
    MP_QSTR_tarinfo,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_upip_utarfile[2] = {
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_QSTR(MP_QSTR_TarInfo_paren_open__percent_r_comma__space__percent_s_comma__space__percent_d_paren_close_),
};

static const mp_frozen_module_t frozen_module_upip_utarfile = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_upip_utarfile,
        .obj_table = (mp_obj_t *)&const_obj_table_data_upip_utarfile,
    },
    .rc = &raw_code_upip_utarfile__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ntptime
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/ntptime.mpy
// - frozen file name: ntptime.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file ntptime.py, scope ntptime__lt_module_gt_
static const byte fun_data_ntptime__lt_module_gt_[71] = {
    0x24,0x16, // prelude
    0x01, // names: <module>
    0x22,0x49,0x29,0x22,0x49,0x69,0x64,0x64,0x84,0x10, // code info
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'usocket'
    0x16,0x03, // STORE_NAME 'socket'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'socket'
    0x16,0x03, // STORE_NAME 'socket'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'ustruct'
    0x16,0x05, // STORE_NAME 'struct'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'struct'
    0x16,0x05, // STORE_NAME 'struct'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x06, // STORE_NAME 'NTP_DELTA'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x16,0x07, // STORE_NAME 'host'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME 'time'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'settime'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ntptime__lt_module_gt_
// frozen bytecode for file ntptime.py, scope ntptime_time
static const byte fun_data_ntptime_time[112] = {
    0x64,0x1a, // prelude
    0x08, // names: time
    0x80,0x11,0x27,0x24,0x30,0x2f,0x22,0x27,0x28,0x49,0x27,0x31, // code info
    0x12,0x0a, // LOAD_GLOBAL 'bytearray'
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x9b, // LOAD_CONST_SMALL_INT 27
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0x12,0x03, // LOAD_GLOBAL 'socket'
    0x14,0x0b, // LOAD_METHOD 'getaddrinfo'
    0x12,0x07, // LOAD_GLOBAL 'host'
    0x22,0x80,0x7b, // LOAD_CONST_SMALL_INT 123
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0x12,0x03, // LOAD_GLOBAL 'socket'
    0x14,0x03, // LOAD_METHOD 'socket'
    0x12,0x03, // LOAD_GLOBAL 'socket'
    0x13,0x0c, // LOAD_ATTR 'AF_INET'
    0x12,0x03, // LOAD_GLOBAL 'socket'
    0x13,0x0d, // LOAD_ATTR 'SOCK_DGRAM'
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0x49,0x18, // SETUP_FINALLY 24
    0xb2, // LOAD_FAST 2
    0x14,0x0e, // LOAD_METHOD 'settimeout'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x0f, // LOAD_METHOD 'sendto'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0xc3, // STORE_FAST 3
    0xb2, // LOAD_FAST 2
    0x14,0x10, // LOAD_METHOD 'recv'
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0x51, // LOAD_CONST_NONE
    0xb2, // LOAD_FAST 2
    0x14,0x11, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0x12,0x05, // LOAD_GLOBAL 'struct'
    0x14,0x12, // LOAD_METHOD 'unpack'
    0x10,0x13, // LOAD_CONST_STRING '!I'
    0xb4, // LOAD_FAST 4
    0xa8, // LOAD_CONST_SMALL_INT 40
    0xac, // LOAD_CONST_SMALL_INT 44
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x12,0x06, // LOAD_GLOBAL 'NTP_DELTA'
    0xf3, // BINARY_OP 28 __sub__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ntptime_time = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_ntptime_time,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 112,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ntptime_time + 3,
        .opcodes = fun_data_ntptime_time + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ntptime__lt_module_gt_
// frozen bytecode for file ntptime.py, scope ntptime_settime
static const byte fun_data_ntptime_settime[69] = {
    0x68,0x0e, // prelude
    0x09, // names: settime
    0x80,0x21,0x25,0x25,0x45,0x27, // code info
    0x12,0x08, // LOAD_GLOBAL 'time'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x14, // IMPORT_NAME 'machine'
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x15, // IMPORT_NAME 'utime'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x16, // LOAD_METHOD 'gmtime'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0x14,0x17, // LOAD_METHOD 'RTC'
    0x36,0x00, // CALL_METHOD 0
    0x14,0x18, // LOAD_METHOD 'datetime'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x85, // LOAD_CONST_SMALL_INT 5
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x08, // BUILD_TUPLE 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ntptime_settime = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_ntptime_settime,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 69,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ntptime_settime + 3,
        .opcodes = fun_data_ntptime_settime + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_ntptime__lt_module_gt_[] = {
    &raw_code_ntptime_time,
    &raw_code_ntptime_settime,
};

static const mp_raw_code_t raw_code_ntptime__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_ntptime__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 71,
    #endif
    .children = (void *)&children_ntptime__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ntptime__lt_module_gt_ + 3,
        .opcodes = fun_data_ntptime__lt_module_gt_ + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_ntptime[25] = {
    MP_QSTR_ntptime_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_usocket,
    MP_QSTR_socket,
    MP_QSTR_ustruct,
    MP_QSTR_struct,
    MP_QSTR_NTP_DELTA,
    MP_QSTR_host,
    MP_QSTR_time,
    MP_QSTR_settime,
    MP_QSTR_bytearray,
    MP_QSTR_getaddrinfo,
    MP_QSTR_AF_INET,
    MP_QSTR_SOCK_DGRAM,
    MP_QSTR_settimeout,
    MP_QSTR_sendto,
    MP_QSTR_recv,
    MP_QSTR_close,
    MP_QSTR_unpack,
    MP_QSTR__bang_I,
    MP_QSTR_machine,
    MP_QSTR_utime,
    MP_QSTR_gmtime,
    MP_QSTR_RTC,
    MP_QSTR_datetime,
};

// constants
static const mp_obj_int_t const_obj_ntptime_0 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0xc200,0xbc17}}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ntptime[2] = {
    MP_ROM_PTR(&const_obj_ntptime_0),
    MP_ROM_QSTR(MP_QSTR_pool_dot_ntp_dot_org),
};

static const mp_frozen_module_t frozen_module_ntptime = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ntptime,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ntptime,
    },
    .rc = &raw_code_ntptime__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module dht
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/dht.mpy
// - frozen file name: dht.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file dht.py, scope dht__lt_module_gt_
static const byte fun_data_dht__lt_module_gt_[165] = {
    0x18,0x20, // prelude
    0x01, // names: <module>
    0x60,0x46,0x2c,0x2f,0x29,0x2e,0x29,0x2e,0x29,0x4e,0x6c,0x89,0x0c,0x8b,0x08, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x03, // LOAD_ATTR 'platform'
    0x14,0x04, // LOAD_METHOD 'startswith'
    0x10,0x05, // LOAD_CONST_STRING 'esp'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'esp'
    0x1c,0x06, // IMPORT_FROM 'dht_readinto'
    0x16,0x06, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0xd1,0x80, // JUMP 81
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x03, // LOAD_ATTR 'platform'
    0x10,0x07, // LOAD_CONST_STRING 'mimxrt'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'mimxrt'
    0x1c,0x06, // IMPORT_FROM 'dht_readinto'
    0x16,0x06, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x7a, // JUMP 58
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x03, // LOAD_ATTR 'platform'
    0x10,0x08, // LOAD_CONST_STRING 'rp2'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x08, // IMPORT_NAME 'rp2'
    0x1c,0x06, // IMPORT_FROM 'dht_readinto'
    0x16,0x06, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x63, // JUMP 35
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x03, // LOAD_ATTR 'platform'
    0x10,0x09, // LOAD_CONST_STRING 'pyboard'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x0a, // IMPORT_NAME 'pyb'
    0x1c,0x06, // IMPORT_FROM 'dht_readinto'
    0x16,0x06, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x4c, // JUMP 12
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x0b, // IMPORT_NAME 'machine'
    0x1c,0x06, // IMPORT_FROM 'dht_readinto'
    0x16,0x06, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0c, // LOAD_CONST_STRING 'DHTBase'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'DHTBase'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0d, // LOAD_CONST_STRING 'DHT11'
    0x11,0x0c, // LOAD_NAME 'DHTBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0d, // STORE_NAME 'DHT11'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x0e, // LOAD_CONST_STRING 'DHT22'
    0x11,0x0c, // LOAD_NAME 'DHTBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0e, // STORE_NAME 'DHT22'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHTBase
static const byte fun_data_dht_DHTBase[25] = {
    0x00,0x0a, // prelude
    0x0c, // names: DHTBase
    0x88,0x12,0x64,0x20, // code info
    0x11,0x0f, // LOAD_NAME '__name__'
    0x16,0x10, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'DHTBase'
    0x16,0x11, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x12, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x13, // STORE_NAME 'measure'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHTBase
// frozen bytecode for file dht.py, scope dht___init__
static const byte fun_data_dht___init__[22] = {
    0x1a,0x0c, // prelude
    0x12,0x16,0x17, // names: __init__, self, pin
    0x80,0x13,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x17, // STORE_ATTR 'pin'
    0x12,0x18, // LOAD_GLOBAL 'bytearray'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x19, // STORE_ATTR 'buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_dht___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 22,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_dht___init__ + 5,
        .opcodes = fun_data_dht___init__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of dht_DHTBase
// frozen bytecode for file dht.py, scope dht_measure
static const byte fun_data_dht_measure[56] = {
    0x21,0x0e, // prelude
    0x13,0x16, // names: measure, self
    0x80,0x17,0x24,0x29,0x39, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0xc1, // STORE_FAST 1
    0x12,0x06, // LOAD_GLOBAL 'dht_readinto'
    0xb0, // LOAD_FAST 0
    0x13,0x17, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0xb1, // LOAD_FAST 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x1a, // LOAD_GLOBAL 'Exception'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht_measure = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_dht_measure,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 56,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_dht_measure + 4,
        .opcodes = fun_data_dht_measure + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_dht_DHTBase[] = {
    &raw_code_dht___init__,
    &raw_code_dht_measure,
};

static const mp_raw_code_t raw_code_dht_DHTBase = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_dht_DHTBase,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = (void *)&children_dht_DHTBase,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_dht_DHTBase + 3,
        .opcodes = fun_data_dht_DHTBase + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHT11
static const byte fun_data_dht_DHT11[24] = {
    0x00,0x08, // prelude
    0x0d, // names: DHT11
    0x88,0x1e,0x64, // code info
    0x11,0x0f, // LOAD_NAME '__name__'
    0x16,0x10, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'DHT11'
    0x16,0x11, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x14, // STORE_NAME 'humidity'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x15, // STORE_NAME 'temperature'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHT11
// frozen bytecode for file dht.py, scope dht_humidity
static const byte fun_data_dht_humidity[12] = {
    0x11,0x08, // prelude
    0x14,0x16, // names: humidity, self
    0x80,0x1f, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht_humidity = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_dht_humidity,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 12,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_dht_humidity + 4,
        .opcodes = fun_data_dht_humidity + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of dht_DHT11
// frozen bytecode for file dht.py, scope dht_temperature
static const byte fun_data_dht_temperature[12] = {
    0x11,0x08, // prelude
    0x15,0x16, // names: temperature, self
    0x80,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht_temperature = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_dht_temperature,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 12,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_dht_temperature + 4,
        .opcodes = fun_data_dht_temperature + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_dht_DHT11[] = {
    &raw_code_dht_humidity,
    &raw_code_dht_temperature,
};

static const mp_raw_code_t raw_code_dht_DHT11 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_dht_DHT11,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
    #endif
    .children = (void *)&children_dht_DHT11,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_dht_DHT11 + 3,
        .opcodes = fun_data_dht_DHT11 + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHT22
static const byte fun_data_dht_DHT22[24] = {
    0x00,0x08, // prelude
    0x0e, // names: DHT22
    0x88,0x26,0x64, // code info
    0x11,0x0f, // LOAD_NAME '__name__'
    0x16,0x10, // STORE_NAME '__module__'
    0x10,0x0e, // LOAD_CONST_STRING 'DHT22'
    0x16,0x11, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x14, // STORE_NAME 'humidity'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x15, // STORE_NAME 'temperature'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHT22
// frozen bytecode for file dht.py, scope dht_humidity2
static const byte fun_data_dht_humidity2[23] = {
    0x19,0x08, // prelude
    0x14,0x16, // names: humidity, self
    0x80,0x27, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht_humidity2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_dht_humidity2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 23,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_dht_humidity2 + 4,
        .opcodes = fun_data_dht_humidity2 + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of dht_DHT22
// frozen bytecode for file dht.py, scope dht_temperature2
static const byte fun_data_dht_temperature2[46] = {
    0x21,0x0e, // prelude
    0x15,0x16, // names: temperature, self
    0x80,0x2a,0x35,0x2b,0x23, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xef, // BINARY_OP 24 __and__
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf4, // BINARY_OP 29 __mul__
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xef, // BINARY_OP 24 __and__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb1, // LOAD_FAST 1
    0xd1, // UNARY_OP 1 __neg__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_dht_temperature2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_dht_temperature2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 46,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_dht_temperature2 + 4,
        .opcodes = fun_data_dht_temperature2 + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_dht_DHT22[] = {
    &raw_code_dht_humidity2,
    &raw_code_dht_temperature2,
};

static const mp_raw_code_t raw_code_dht_DHT22 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_dht_DHT22,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
    #endif
    .children = (void *)&children_dht_DHT22,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_dht_DHT22 + 3,
        .opcodes = fun_data_dht_DHT22 + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_dht__lt_module_gt_[] = {
    &raw_code_dht_DHTBase,
    &raw_code_dht_DHT11,
    &raw_code_dht_DHT22,
};

static const mp_raw_code_t raw_code_dht__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_dht__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 165,
    #endif
    .children = (void *)&children_dht__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 3,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_dht__lt_module_gt_ + 3,
        .opcodes = fun_data_dht__lt_module_gt_ + 18,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_dht[27] = {
    MP_QSTR_dht_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_platform,
    MP_QSTR_startswith,
    MP_QSTR_esp,
    MP_QSTR_dht_readinto,
    MP_QSTR_mimxrt,
    MP_QSTR_rp2,
    MP_QSTR_pyboard,
    MP_QSTR_pyb,
    MP_QSTR_machine,
    MP_QSTR_DHTBase,
    MP_QSTR_DHT11,
    MP_QSTR_DHT22,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_measure,
    MP_QSTR_humidity,
    MP_QSTR_temperature,
    MP_QSTR_self,
    MP_QSTR_pin,
    MP_QSTR_bytearray,
    MP_QSTR_buf,
    MP_QSTR_Exception,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_dht_1 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_dht_1_macro MP_ROM_PTR(&const_obj_dht_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_dht_1_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_dht_1_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_dht_2 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_dht_2_macro MP_ROM_PTR(&const_obj_dht_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_dht_2_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_dht_2_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_dht[3] = {
    MP_ROM_QSTR(MP_QSTR_checksum_space_error),
    const_obj_dht_1_macro,
    const_obj_dht_2_macro,
};

static const mp_frozen_module_t frozen_module_dht = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_dht,
        .obj_table = (mp_obj_t *)&const_obj_table_data_dht,
    },
    .rc = &raw_code_dht__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module onewire
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/onewire.mpy
// - frozen file name: onewire.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file onewire.py, scope onewire__lt_module_gt_
static const byte fun_data_onewire__lt_module_gt_[35] = {
    0x18,0x0a, // prelude
    0x01, // names: <module>
    0x60,0x66,0x6b,0x20, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME '_onewire'
    0x16,0x03, // STORE_NAME '_ow'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'OneWireError'
    0x11,0x05, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'OneWireError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x06, // LOAD_CONST_STRING 'OneWire'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x06, // STORE_NAME 'OneWire'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of onewire__lt_module_gt_
// frozen bytecode for file onewire.py, scope onewire_OneWireError
static const byte fun_data_onewire_OneWireError[15] = {
    0x00,0x06, // prelude
    0x04, // names: OneWireError
    0x88,0x07, // code info
    0x11,0x07, // LOAD_NAME '__name__'
    0x16,0x08, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'OneWireError'
    0x16,0x09, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_OneWireError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_onewire_OneWireError,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_onewire_OneWireError + 3,
        .opcodes = fun_data_onewire_OneWireError + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire__lt_module_gt_
// frozen bytecode for file onewire.py, scope onewire_OneWire
static const byte fun_data_onewire_OneWire[103] = {
    0x08,0x30, // prelude
    0x06, // names: OneWire
    0x88,0x0b,0x25,0x25,0x45,0x64,0x20,0x68,0x60,0x64,0x64,0x64,0x20,0x64,0x64,0x64,0x20,0x64,0x40,0x84,0x0c,0x84,0x1c, // code info
    0x11,0x07, // LOAD_NAME '__name__'
    0x16,0x08, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'OneWire'
    0x16,0x09, // STORE_NAME '__qualname__'
    0x22,0x81,0x70, // LOAD_CONST_SMALL_INT 240
    0x16,0x0a, // STORE_NAME 'SEARCH_ROM'
    0x22,0x80,0x55, // LOAD_CONST_SMALL_INT 85
    0x16,0x0b, // STORE_NAME 'MATCH_ROM'
    0x22,0x81,0x4c, // LOAD_CONST_SMALL_INT 204
    0x16,0x0c, // STORE_NAME 'SKIP_ROM'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0d, // STORE_NAME '__init__'
    0x50, // LOAD_CONST_FALSE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x0e, // STORE_NAME 'reset'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0f, // STORE_NAME 'readbit'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x10, // STORE_NAME 'readbyte'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x11, // STORE_NAME 'readinto'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x12, // STORE_NAME 'writebit'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x13, // STORE_NAME 'writebyte'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x14, // STORE_NAME 'write'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x15, // STORE_NAME 'select_rom'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x16, // STORE_NAME 'scan'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x17, // STORE_NAME '_search_rom'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x18, // STORE_NAME 'crc8'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire___init__
static const byte fun_data_onewire___init__[28] = {
    0x2a,0x0c, // prelude
    0x0d,0x19,0x1a, // names: __init__, self, pin
    0x80,0x10,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x1a, // STORE_ATTR 'pin'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0x14,0x1b, // LOAD_METHOD 'init'
    0xb1, // LOAD_FAST 1
    0x13,0x1c, // LOAD_ATTR 'OPEN_DRAIN'
    0xb1, // LOAD_FAST 1
    0x13,0x1d, // LOAD_ATTR 'PULL_UP'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_onewire___init__ + 5,
        .opcodes = fun_data_onewire___init__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_reset
static const byte fun_data_onewire_reset[32] = {
    0xaa,0x01,0x10, // prelude
    0x0e,0x19,0x1e, // names: reset, self, required
    0x80,0x14,0x2a,0x26,0x23, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x0e, // LOAD_METHOD 'reset'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb2, // LOAD_FAST 2
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x04, // LOAD_GLOBAL 'OneWireError'
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_reset = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_reset,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 32,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_onewire_reset + 6,
        .opcodes = fun_data_onewire_reset + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_readbit
static const byte fun_data_onewire_readbit[16] = {
    0x19,0x08, // prelude
    0x0f,0x19, // names: readbit, self
    0x80,0x1a, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x0f, // LOAD_METHOD 'readbit'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_readbit = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_onewire_readbit,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_onewire_readbit + 4,
        .opcodes = fun_data_onewire_readbit + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_readbyte
static const byte fun_data_onewire_readbyte[16] = {
    0x19,0x08, // prelude
    0x10,0x19, // names: readbyte, self
    0x80,0x1d, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x10, // LOAD_METHOD 'readbyte'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_readbyte = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_onewire_readbyte,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_onewire_readbyte + 4,
        .opcodes = fun_data_onewire_readbyte + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_readinto
static const byte fun_data_onewire_readinto[41] = {
    0x3a,0x0c, // prelude
    0x11,0x19,0x1f, // names: readinto, self, buf
    0x80,0x20,0x2a, // code info
    0x12,0x20, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x50, // JUMP 16
    0x57, // DUP_TOP
    0xc2, // STORE_FAST 2
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x10, // LOAD_METHOD 'readbyte'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x2b, // POP_JUMP_IF_TRUE -21
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_readinto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_readinto,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_onewire_readinto + 5,
        .opcodes = fun_data_onewire_readinto + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_writebit
static const byte fun_data_onewire_writebit[18] = {
    0x2a,0x0a, // prelude
    0x12,0x19,0x21, // names: writebit, self, value
    0x80,0x24, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x12, // LOAD_METHOD 'writebit'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_writebit = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_writebit,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_onewire_writebit + 5,
        .opcodes = fun_data_onewire_writebit + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_writebyte
static const byte fun_data_onewire_writebyte[18] = {
    0x2a,0x0a, // prelude
    0x13,0x19,0x21, // names: writebyte, self, value
    0x80,0x27, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x13, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_writebyte = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_writebyte,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_onewire_writebyte + 5,
        .opcodes = fun_data_onewire_writebyte + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_write
static const byte fun_data_onewire_write[28] = {
    0x52,0x0c, // prelude
    0x14,0x19,0x1f, // names: write, self, buf
    0x80,0x2a,0x25, // code info
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0e, // FOR_ITER 14
    0xc2, // STORE_FAST 2
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x13, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x1a, // LOAD_ATTR 'pin'
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x30, // JUMP -16
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_write,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_onewire_write + 5,
        .opcodes = fun_data_onewire_write + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_select_rom
static const byte fun_data_onewire_select_rom[33] = {
    0x22,0x0e, // prelude
    0x15,0x19,0x22, // names: select_rom, self, rom
    0x80,0x2e,0x26,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x13, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'MATCH_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x14, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_select_rom = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_select_rom,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_onewire_select_rom + 5,
        .opcodes = fun_data_onewire_select_rom + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_scan
static const byte fun_data_onewire_scan[68] = {
    0x49,0x1a, // prelude
    0x16,0x19, // names: scan, self
    0x80,0x33,0x23,0x24,0x22,0x25,0x2b,0x23,0x26,0x25,0x2c, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xc1, // STORE_FAST 1
    0x22,0x80,0x41, // LOAD_CONST_SMALL_INT 65
    0xc2, // STORE_FAST 2
    0x50, // LOAD_CONST_FALSE
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5f, // JUMP 31
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x14,0x17, // LOAD_METHOD '_search_rom'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc2, // STORE_FAST 2
    0xb3, // LOAD_FAST 3
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0x2b,0x01, // BUILD_LIST 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x49, // JUMP 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1a, // POP_JUMP_IF_TRUE -38
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_scan = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_onewire_scan,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 68,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_onewire_scan + 4,
        .opcodes = fun_data_onewire_scan + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire__search_rom
static const byte fun_data_onewire__search_rom[180] = {
    0x73,0x3c, // prelude
    0x17,0x19,0x23,0x24, // names: _search_rom, self, l_rom, diff
    0x80,0x3f,0x27,0x23,0x29,0x23,0x26,0x26,0x22,0x24,0x26,0x22,0x26,0x26,0x27,0x23,0x45,0x23,0x33,0x22,0x22,0x27,0x23,0x26,0x2d,0x2d, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x13, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'SEARCH_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0x12,0x25, // LOAD_GLOBAL 'bytearray'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0x12,0x25, // LOAD_GLOBAL 'bytearray'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc4, // STORE_FAST 4
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0xc5, // STORE_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xdc,0x80, // JUMP 92
    0x57, // DUP_TOP
    0xc6, // STORE_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc7, // STORE_FAST 7
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc7,0x80, // JUMP 71
    0x57, // DUP_TOP
    0xc8, // STORE_FAST 8
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'readbit'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'readbit'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb9, // LOAD_FAST 9
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x63, // RETURN_VALUE
    0x42,0x5a, // JUMP 26
    0xb9, // LOAD_FAST 9
    0x43,0x57, // POP_JUMP_IF_TRUE 23
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x4e, // POP_JUMP_IF_TRUE 14
    0xb1, // LOAD_FAST 1
    0xb6, // LOAD_FAST 6
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb8, // LOAD_FAST 8
    0xf0, // BINARY_OP 25 __lshift__
    0xef, // BINARY_OP 24 __and__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc9, // STORE_FAST 9
    0xb5, // LOAD_FAST 5
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x14,0x12, // LOAD_METHOD 'writebit'
    0xb9, // LOAD_FAST 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb7, // LOAD_FAST 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb8, // LOAD_FAST 8
    0xf0, // BINARY_OP 25 __lshift__
    0xe0, // BINARY_OP 9 __ior__
    0xc7, // STORE_FAST 7
    0xb5, // LOAD_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0xc5, // STORE_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x88, // LOAD_CONST_SMALL_INT 8
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb3,0x7f, // POP_JUMP_IF_TRUE -77
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0xb3, // LOAD_FAST 3
    0xb6, // LOAD_FAST 6
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x88, // LOAD_CONST_SMALL_INT 8
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x9e,0x7f, // POP_JUMP_IF_TRUE -98
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire__search_rom = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_onewire__search_rom,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 180,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_onewire__search_rom + 6,
        .opcodes = fun_data_onewire__search_rom + 32,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_crc8
static const byte fun_data_onewire_crc8[15] = {
    0x22,0x0a, // prelude
    0x18,0x19,0x26, // names: crc8, self, data
    0x80,0x5b, // code info
    0x12,0x03, // LOAD_GLOBAL '_ow'
    0x14,0x18, // LOAD_METHOD 'crc8'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_onewire_crc8 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_onewire_crc8,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_onewire_crc8 + 5,
        .opcodes = fun_data_onewire_crc8 + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_onewire_OneWire[] = {
    &raw_code_onewire___init__,
    &raw_code_onewire_reset,
    &raw_code_onewire_readbit,
    &raw_code_onewire_readbyte,
    &raw_code_onewire_readinto,
    &raw_code_onewire_writebit,
    &raw_code_onewire_writebyte,
    &raw_code_onewire_write,
    &raw_code_onewire_select_rom,
    &raw_code_onewire_scan,
    &raw_code_onewire__search_rom,
    &raw_code_onewire_crc8,
};

static const mp_raw_code_t raw_code_onewire_OneWire = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_onewire_OneWire,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 103,
    #endif
    .children = (void *)&children_onewire_OneWire,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 12,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_onewire_OneWire + 3,
        .opcodes = fun_data_onewire_OneWire + 26,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_onewire__lt_module_gt_[] = {
    &raw_code_onewire_OneWireError,
    &raw_code_onewire_OneWire,
};

static const mp_raw_code_t raw_code_onewire__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_onewire__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 35,
    #endif
    .children = (void *)&children_onewire__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_onewire__lt_module_gt_ + 3,
        .opcodes = fun_data_onewire__lt_module_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_onewire[39] = {
    MP_QSTR_onewire_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__onewire,
    MP_QSTR__ow,
    MP_QSTR_OneWireError,
    MP_QSTR_Exception,
    MP_QSTR_OneWire,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_SEARCH_ROM,
    MP_QSTR_MATCH_ROM,
    MP_QSTR_SKIP_ROM,
    MP_QSTR___init__,
    MP_QSTR_reset,
    MP_QSTR_readbit,
    MP_QSTR_readbyte,
    MP_QSTR_readinto,
    MP_QSTR_writebit,
    MP_QSTR_writebyte,
    MP_QSTR_write,
    MP_QSTR_select_rom,
    MP_QSTR_scan,
    MP_QSTR__search_rom,
    MP_QSTR_crc8,
    MP_QSTR_self,
    MP_QSTR_pin,
    MP_QSTR_init,
    MP_QSTR_OPEN_DRAIN,
    MP_QSTR_PULL_UP,
    MP_QSTR_required,
    MP_QSTR_buf,
    MP_QSTR_len,
    MP_QSTR_value,
    MP_QSTR_rom,
    MP_QSTR_l_rom,
    MP_QSTR_diff,
    MP_QSTR_bytearray,
    MP_QSTR_data,
};

// constants
static const mp_rom_obj_tuple_t const_obj_onewire_0 = {{&mp_type_tuple}, 2, {
    MP_ROM_NONE,
    MP_ROM_INT(0),
}};
static const mp_rom_obj_tuple_t const_obj_onewire_1 = {{&mp_type_tuple}, 2, {
    MP_ROM_NONE,
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_onewire[2] = {
    MP_ROM_PTR(&const_obj_onewire_0),
    MP_ROM_PTR(&const_obj_onewire_1),
};

static const mp_frozen_module_t frozen_module_onewire = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_onewire,
        .obj_table = (mp_obj_t *)&const_obj_table_data_onewire,
    },
    .rc = &raw_code_onewire__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ds18x20
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/ds18x20.mpy
// - frozen file name: ds18x20.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file ds18x20.py, scope ds18x20__lt_module_gt_
static const byte fun_data_ds18x20__lt_module_gt_[29] = {
    0x10,0x08, // prelude
    0x01, // names: <module>
    0x60,0x8c,0x07, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'micropython'
    0x1c,0x02, // IMPORT_FROM 'const'
    0x16,0x02, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'DS18X20'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'DS18X20'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ds18x20__lt_module_gt_
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20
static const byte fun_data_ds18x20_DS18X20[48] = {
    0x00,0x18, // prelude
    0x04, // names: DS18X20
    0x88,0x0b,0x64,0x20,0x64,0x64,0x40,0x84,0x09,0x64,0x60, // code info
    0x11,0x05, // LOAD_NAME '__name__'
    0x16,0x06, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'DS18X20'
    0x16,0x07, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'scan'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'convert_temp'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'read_scratch'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0c, // STORE_NAME 'write_scratch'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0d, // STORE_NAME 'read_temp'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20___init__
static const byte fun_data_ds18x20___init__[22] = {
    0x1a,0x0c, // prelude
    0x08,0x0e,0x0f, // names: __init__, self, onewire
    0x80,0x0c,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'ow'
    0x12,0x11, // LOAD_GLOBAL 'bytearray'
    0x89, // LOAD_CONST_SMALL_INT 9
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x12, // STORE_ATTR 'buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_ds18x20___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 22,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ds18x20___init__ + 5,
        .opcodes = fun_data_ds18x20___init__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_scan
static const byte fun_data_ds18x20_scan[18] = {
    0x19,0x08, // prelude
    0x09,0x0e, // names: scan, self
    0x80,0x10, // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x09, // LOAD_METHOD 'scan'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of ds18x20_scan
// frozen bytecode for file ds18x20.py, scope ds18x20__lt_listcomp_gt_
static const byte fun_data_ds18x20__lt_listcomp_gt_[27] = {
    0x41,0x08, // prelude
    0x1d,0x1e, // names: <listcomp>, *
    0x80,0x10, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0e, // FOR_ITER 14
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x35, // POP_JUMP_IF_FALSE -11
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x30, // JUMP -16
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_ds18x20__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_ds18x20__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_ds18x20__lt_listcomp_gt_ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_ds18x20_scan[] = {
    &raw_code_ds18x20__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_ds18x20_scan = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_ds18x20_scan,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = (void *)&children_ds18x20_scan,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ds18x20_scan + 4,
        .opcodes = fun_data_ds18x20_scan + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_convert_temp
static const byte fun_data_ds18x20_convert_temp[43] = {
    0x19,0x0c, // prelude
    0x0a,0x0e, // names: convert_temp, self
    0x80,0x13,0x29,0x2d, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x13, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x14, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x13,0x15, // LOAD_ATTR 'SKIP_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x14, // LOAD_METHOD 'writebyte'
    0x22,0x80,0x44, // LOAD_CONST_SMALL_INT 68
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20_convert_temp = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_ds18x20_convert_temp,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 43,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ds18x20_convert_temp + 4,
        .opcodes = fun_data_ds18x20_convert_temp + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_read_scratch
static const byte fun_data_ds18x20_read_scratch[76] = {
    0x22,0x16, // prelude
    0x0b,0x0e,0x16, // names: read_scratch, self, rom
    0x80,0x18,0x29,0x29,0x2b,0x2b,0x2c,0x27, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x13, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x17, // LOAD_METHOD 'select_rom'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x14, // LOAD_METHOD 'writebyte'
    0x22,0x81,0x3e, // LOAD_CONST_SMALL_INT 190
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x18, // LOAD_METHOD 'readinto'
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'buf'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x19, // LOAD_METHOD 'crc8'
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'buf'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x1a, // LOAD_GLOBAL 'Exception'
    0x10,0x1b, // LOAD_CONST_STRING 'CRC error'
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'buf'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20_read_scratch = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_ds18x20_read_scratch,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 76,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_ds18x20_read_scratch + 5,
        .opcodes = fun_data_ds18x20_read_scratch + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_write_scratch
static const byte fun_data_ds18x20_write_scratch[51] = {
    0x2b,0x12, // prelude
    0x0c,0x0e,0x16,0x12, // names: write_scratch, self, rom, buf
    0x80,0x21,0x29,0x29,0x2b, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x13, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x17, // LOAD_METHOD 'select_rom'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x14, // LOAD_METHOD 'writebyte'
    0x22,0x80,0x4e, // LOAD_CONST_SMALL_INT 78
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ow'
    0x14,0x1c, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20_write_scratch = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_ds18x20_write_scratch,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 51,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_ds18x20_write_scratch + 6,
        .opcodes = fun_data_ds18x20_write_scratch + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_read_temp
static const byte fun_data_ds18x20_read_temp[115] = {
    0x3a,0x1e, // prelude
    0x0d,0x0e,0x16, // names: read_temp, self, rom
    0x80,0x27,0x27,0x27,0x25,0x2a,0x4c,0x26,0x51,0x2a,0x28,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'read_scratch'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x90, // LOAD_CONST_SMALL_INT 16
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x72, // POP_JUMP_IF_FALSE 50
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf1, // BINARY_OP 26 __rshift__
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xed, // BINARY_OP 22 __or__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xd2, // UNARY_OP 2 __invert__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0xd1, // UNARY_OP 1 __neg__
    0xc3, // STORE_FAST 3
    0x42,0x46, // JUMP 6
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf1, // BINARY_OP 26 __rshift__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xf3, // BINARY_OP 28 __sub__
    0xb2, // LOAD_FAST 2
    0x87, // LOAD_CONST_SMALL_INT 7
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0xf3, // BINARY_OP 28 __sub__
    0xb2, // LOAD_FAST 2
    0x87, // LOAD_CONST_SMALL_INT 7
    0x55, // LOAD_SUBSCR
    0xf7, // BINARY_OP 32 __truediv__
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x22,0x82,0x80,0x00, // LOAD_CONST_SMALL_INT 32768
    0xef, // BINARY_OP 24 __and__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb3, // LOAD_FAST 3
    0x22,0x83,0xff,0x7f, // LOAD_CONST_SMALL_INT 65535
    0xee, // BINARY_OP 23 __xor__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xd1, // UNARY_OP 1 __neg__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x90, // LOAD_CONST_SMALL_INT 16
    0xf7, // BINARY_OP 32 __truediv__
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_ds18x20_read_temp = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_ds18x20_read_temp,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 115,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_ds18x20_read_temp + 5,
        .opcodes = fun_data_ds18x20_read_temp + 17,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_ds18x20_DS18X20[] = {
    &raw_code_ds18x20___init__,
    &raw_code_ds18x20_scan,
    &raw_code_ds18x20_convert_temp,
    &raw_code_ds18x20_read_scratch,
    &raw_code_ds18x20_write_scratch,
    &raw_code_ds18x20_read_temp,
};

static const mp_raw_code_t raw_code_ds18x20_DS18X20 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_ds18x20_DS18X20,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 48,
    #endif
    .children = (void *)&children_ds18x20_DS18X20,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 6,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_ds18x20_DS18X20 + 3,
        .opcodes = fun_data_ds18x20_DS18X20 + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_ds18x20__lt_module_gt_[] = {
    &raw_code_ds18x20_DS18X20,
};

static const mp_raw_code_t raw_code_ds18x20__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_ds18x20__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
    #endif
    .children = (void *)&children_ds18x20__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ds18x20__lt_module_gt_ + 3,
        .opcodes = fun_data_ds18x20__lt_module_gt_ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_ds18x20[31] = {
    MP_QSTR_ds18x20_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_DS18X20,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_scan,
    MP_QSTR_convert_temp,
    MP_QSTR_read_scratch,
    MP_QSTR_write_scratch,
    MP_QSTR_read_temp,
    MP_QSTR_self,
    MP_QSTR_onewire,
    MP_QSTR_ow,
    MP_QSTR_bytearray,
    MP_QSTR_buf,
    MP_QSTR_reset,
    MP_QSTR_writebyte,
    MP_QSTR_SKIP_ROM,
    MP_QSTR_rom,
    MP_QSTR_select_rom,
    MP_QSTR_readinto,
    MP_QSTR_crc8,
    MP_QSTR_Exception,
    MP_QSTR_CRC_space_error,
    MP_QSTR_write,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR__star_,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ds18x20_0 = {{&mp_type_float}, (mp_float_t)0.25};
#define const_obj_ds18x20_0_macro MP_ROM_PTR(&const_obj_ds18x20_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ds18x20_0_macro ((mp_rom_obj_t)(0xbf000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ds18x20_0_macro ((mp_rom_obj_t)(0xbfd4000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ds18x20_1 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(16),
    MP_ROM_INT(34),
    MP_ROM_INT(40),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ds18x20[2] = {
    const_obj_ds18x20_0_macro,
    MP_ROM_PTR(&const_obj_ds18x20_1),
};

static const mp_frozen_module_t frozen_module_ds18x20 = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ds18x20,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ds18x20,
    },
    .rc = &raw_code_ds18x20__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio___init__
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/__init__.mpy
// - frozen file name: uasyncio/__init__.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/__init__.py, scope uasyncio___init____lt_module_gt_
static const byte fun_data_uasyncio___init____lt_module_gt_[75] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'core'
    0x69, // IMPORT_STAR
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x04, // STORE_NAME '__version__'
    0x2c,0x0a, // BUILD_MAP 10
    0x10,0x05, // LOAD_CONST_STRING 'funcs'
    0x10,0x06, // LOAD_CONST_STRING 'wait_for'
    0x62, // STORE_MAP
    0x10,0x05, // LOAD_CONST_STRING 'funcs'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x62, // STORE_MAP
    0x10,0x05, // LOAD_CONST_STRING 'funcs'
    0x10,0x07, // LOAD_CONST_STRING 'gather'
    0x62, // STORE_MAP
    0x10,0x08, // LOAD_CONST_STRING 'event'
    0x10,0x09, // LOAD_CONST_STRING 'Event'
    0x62, // STORE_MAP
    0x10,0x08, // LOAD_CONST_STRING 'event'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x62, // STORE_MAP
    0x10,0x0a, // LOAD_CONST_STRING 'lock'
    0x10,0x0b, // LOAD_CONST_STRING 'Lock'
    0x62, // STORE_MAP
    0x10,0x0c, // LOAD_CONST_STRING 'stream'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x62, // STORE_MAP
    0x10,0x0c, // LOAD_CONST_STRING 'stream'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x62, // STORE_MAP
    0x10,0x0c, // LOAD_CONST_STRING 'stream'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x62, // STORE_MAP
    0x10,0x0c, // LOAD_CONST_STRING 'stream'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0x16,0x0d, // STORE_NAME '_attrs'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0e, // STORE_NAME '__getattr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio___init____lt_module_gt_
// frozen bytecode for file uasyncio/__init__.py, scope uasyncio___init_____getattr__
static const byte fun_data_uasyncio___init_____getattr__[48] = {
    0x49,0x04, // prelude
    0x0e,0x0f, // names: __getattr__, attr
     // code info
    0x12,0x0d, // LOAD_GLOBAL '_attrs'
    0x14,0x10, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x11, // LOAD_GLOBAL 'AttributeError'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x12, // LOAD_GLOBAL 'getattr'
    0x12,0x13, // LOAD_GLOBAL '__import__'
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x05, // CALL_FUNCTION 5
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x14, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x56, // STORE_SUBSCR
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio___init_____getattr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio___init_____getattr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 48,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_uasyncio___init_____getattr__ + 4,
        .opcodes = fun_data_uasyncio___init_____getattr__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio___init____lt_module_gt_[] = {
    &raw_code_uasyncio___init_____getattr__,
};

static const mp_raw_code_t raw_code_uasyncio___init____lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 75,
    #endif
    .children = (void *)&children_uasyncio___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio___init____lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio___init____lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio___init__[21] = {
    MP_QSTR_uasyncio_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_core,
    MP_QSTR___version__,
    MP_QSTR_funcs,
    MP_QSTR_wait_for,
    MP_QSTR_gather,
    MP_QSTR_event,
    MP_QSTR_Event,
    MP_QSTR_lock,
    MP_QSTR_Lock,
    MP_QSTR_stream,
    MP_QSTR__attrs,
    MP_QSTR___getattr__,
    MP_QSTR_attr,
    MP_QSTR_get,
    MP_QSTR_AttributeError,
    MP_QSTR_getattr,
    MP_QSTR___import__,
    MP_QSTR_globals,
};

// constants
static const mp_rom_obj_tuple_t const_obj_uasyncio___init___0 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(3),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_uasyncio___init__[7] = {
    MP_ROM_PTR(&const_obj_uasyncio___init___0),
    MP_ROM_QSTR(MP_QSTR_wait_for_ms),
    MP_ROM_QSTR(MP_QSTR_ThreadSafeFlag),
    MP_ROM_QSTR(MP_QSTR_open_connection),
    MP_ROM_QSTR(MP_QSTR_start_server),
    MP_ROM_QSTR(MP_QSTR_StreamReader),
    MP_ROM_QSTR(MP_QSTR_StreamWriter),
};

static const mp_frozen_module_t frozen_module_uasyncio___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio___init__,
        .obj_table = (mp_obj_t *)&const_obj_table_data_uasyncio___init__,
    },
    .rc = &raw_code_uasyncio___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio_core
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/core.mpy
// - frozen file name: uasyncio/core.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/core.py, scope uasyncio_core__lt_module_gt_
static const byte fun_data_uasyncio_core__lt_module_gt_[215] = {
    0x2c,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'ticks_ms'
    0x10,0x03, // LOAD_CONST_STRING 'ticks_diff'
    0x10,0x04, // LOAD_CONST_STRING 'ticks_add'
    0x2a,0x03, // BUILD_TUPLE 3
    0x1b,0x05, // IMPORT_NAME 'time'
    0x1c,0x02, // IMPORT_FROM 'ticks_ms'
    0x16,0x06, // STORE_NAME 'ticks'
    0x1c,0x03, // IMPORT_FROM 'ticks_diff'
    0x16,0x03, // STORE_NAME 'ticks_diff'
    0x1c,0x04, // IMPORT_FROM 'ticks_add'
    0x16,0x04, // STORE_NAME 'ticks_add'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'sys'
    0x16,0x07, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'select'
    0x16,0x08, // STORE_NAME 'select'
    0x48,0x14, // SETUP_EXCEPT 20
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x09, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x0a, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0b, // IMPORT_NAME '_uasyncio'
    0x1c,0x09, // IMPORT_FROM 'TaskQueue'
    0x16,0x09, // STORE_NAME 'TaskQueue'
    0x1c,0x0a, // IMPORT_FROM 'Task'
    0x16,0x0a, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x16, // POP_EXCEPT_JUMP 22
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x09, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x0a, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0c, // IMPORT_NAME 'task'
    0x1c,0x09, // IMPORT_FROM 'TaskQueue'
    0x16,0x09, // STORE_NAME 'TaskQueue'
    0x1c,0x0a, // IMPORT_FROM 'Task'
    0x16,0x0a, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0d, // LOAD_CONST_STRING 'CancelledError'
    0x11,0x0e, // LOAD_NAME 'BaseException'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0d, // STORE_NAME 'CancelledError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0f, // LOAD_CONST_STRING 'TimeoutError'
    0x11,0x10, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0f, // STORE_NAME 'TimeoutError'
    0x2c,0x03, // BUILD_MAP 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x11, // LOAD_CONST_STRING 'message'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x12, // LOAD_CONST_STRING 'exception'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x13, // LOAD_CONST_STRING 'future'
    0x62, // STORE_MAP
    0x16,0x14, // STORE_NAME '_exc_context'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x15, // LOAD_CONST_STRING 'SingletonGenerator'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x15, // STORE_NAME 'SingletonGenerator'
    0x11,0x15, // LOAD_NAME 'SingletonGenerator'
    0x34,0x00, // CALL_FUNCTION 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x16, // STORE_NAME 'sleep_ms'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x17, // STORE_NAME 'sleep'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x18, // LOAD_CONST_STRING 'IOQueue'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x18, // STORE_NAME 'IOQueue'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x19, // STORE_NAME '_promote_to_task'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x1a, // STORE_NAME 'create_task'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x1c, // STORE_NAME 'run'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x1d, // STORE_NAME '_stopper'
    0x51, // LOAD_CONST_NONE
    0x17,0x1e, // STORE_GLOBAL '_stop_task'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x0b, // MAKE_FUNCTION 11
    0x10,0x1f, // LOAD_CONST_STRING 'Loop'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x1f, // STORE_NAME 'Loop'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0c, // MAKE_FUNCTION_DEFARGS 12
    0x16,0x20, // STORE_NAME 'get_event_loop'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x21, // STORE_NAME 'current_task'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x22, // STORE_NAME 'new_event_loop'
    0x11,0x22, // LOAD_NAME 'new_event_loop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_CancelledError
static const byte fun_data_uasyncio_core_CancelledError[13] = {
    0x00,0x02, // prelude
    0x0d, // names: CancelledError
     // code info
    0x11,0x23, // LOAD_NAME '__name__'
    0x16,0x24, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'CancelledError'
    0x16,0x25, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_CancelledError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_CancelledError,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 13,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_uasyncio_core_CancelledError + 3,
        .opcodes = fun_data_uasyncio_core_CancelledError + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_TimeoutError
static const byte fun_data_uasyncio_core_TimeoutError[13] = {
    0x00,0x02, // prelude
    0x0f, // names: TimeoutError
     // code info
    0x11,0x23, // LOAD_NAME '__name__'
    0x16,0x24, // STORE_NAME '__module__'
    0x10,0x0f, // LOAD_CONST_STRING 'TimeoutError'
    0x16,0x25, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_TimeoutError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_TimeoutError,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 13,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_uasyncio_core_TimeoutError + 3,
        .opcodes = fun_data_uasyncio_core_TimeoutError + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_SingletonGenerator
static const byte fun_data_uasyncio_core_SingletonGenerator[25] = {
    0x00,0x02, // prelude
    0x15, // names: SingletonGenerator
     // code info
    0x11,0x23, // LOAD_NAME '__name__'
    0x16,0x24, // STORE_NAME '__module__'
    0x10,0x15, // LOAD_CONST_STRING 'SingletonGenerator'
    0x16,0x25, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x26, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x27, // STORE_NAME '__iter__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x28, // STORE_NAME '__next__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_core_SingletonGenerator
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core___init__
static const byte fun_data_uasyncio_core___init__[17] = {
    0x11,0x04, // prelude
    0x26,0x53, // names: __init__, self
     // code info
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'state'
    0x12,0x3e, // LOAD_GLOBAL 'StopIteration'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x18,0x54, // STORE_ATTR 'exc'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 17,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_uasyncio_core___init__ + 4,
        .opcodes = fun_data_uasyncio_core___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_SingletonGenerator
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core___iter__
static const byte fun_data_uasyncio_core___iter__[6] = {
    0x09,0x04, // prelude
    0x27,0x53, // names: __iter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core___iter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core___iter__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 6,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_uasyncio_core___iter__ + 4,
        .opcodes = fun_data_uasyncio_core___iter__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_SingletonGenerator
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core___next__
static const byte fun_data_uasyncio_core___next__[42] = {
    0x21,0x04, // prelude
    0x28,0x53, // names: __next__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0x12,0x44, // LOAD_GLOBAL 'cur_task'
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'state'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x54, // LOAD_ATTR 'exc'
    0x18,0x55, // STORE_ATTR '__traceback__'
    0xb0, // LOAD_FAST 0
    0x13,0x54, // LOAD_ATTR 'exc'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core___next__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core___next__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 42,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_uasyncio_core___next__ + 4,
        .opcodes = fun_data_uasyncio_core___next__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_core_SingletonGenerator[] = {
    &raw_code_uasyncio_core___init__,
    &raw_code_uasyncio_core___iter__,
    &raw_code_uasyncio_core___next__,
};

static const mp_raw_code_t raw_code_uasyncio_core_SingletonGenerator = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_SingletonGenerator,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = (void *)&children_uasyncio_core_SingletonGenerator,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 3,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_uasyncio_core_SingletonGenerator + 3,
        .opcodes = fun_data_uasyncio_core_SingletonGenerator + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_sleep_ms
static const byte fun_data_uasyncio_core_sleep_ms[25] = {
    0xb2,0x01,0x06, // prelude
    0x16,0x29,0x2a, // names: sleep_ms, t, sgen
     // code info
    0x12,0x04, // LOAD_GLOBAL 'ticks_add'
    0x12,0x06, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x2b, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xb1, // LOAD_FAST 1
    0x18,0x2c, // STORE_ATTR 'state'
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_sleep_ms = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_sleep_ms,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_uasyncio_core_sleep_ms + 6,
        .opcodes = fun_data_uasyncio_core_sleep_ms + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_sleep
static const byte fun_data_uasyncio_core_sleep[18] = {
    0x21,0x04, // prelude
    0x17,0x29, // names: sleep, t
     // code info
    0x12,0x16, // LOAD_GLOBAL 'sleep_ms'
    0x12,0x2d, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_sleep = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_sleep,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_uasyncio_core_sleep + 4,
        .opcodes = fun_data_uasyncio_core_sleep + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_IOQueue
static const byte fun_data_uasyncio_core_IOQueue[41] = {
    0x00,0x02, // prelude
    0x18, // names: IOQueue
     // code info
    0x11,0x23, // LOAD_NAME '__name__'
    0x16,0x24, // STORE_NAME '__module__'
    0x10,0x18, // LOAD_CONST_STRING 'IOQueue'
    0x16,0x25, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x26, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x2e, // STORE_NAME '_enqueue'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2f, // STORE_NAME '_dequeue'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x30, // STORE_NAME 'queue_read'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x31, // STORE_NAME 'queue_write'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x32, // STORE_NAME 'remove'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x33, // STORE_NAME 'wait_io_event'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core___init__2
static const byte fun_data_uasyncio_core___init__2[20] = {
    0x11,0x04, // prelude
    0x26,0x53, // names: __init__, self
     // code info
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x14,0x56, // LOAD_METHOD 'poll'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x57, // STORE_ATTR 'poller'
    0x2c,0x00, // BUILD_MAP 0
    0xb0, // LOAD_FAST 0
    0x18,0x42, // STORE_ATTR 'map'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core___init__2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core___init__2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_uasyncio_core___init__2 + 4,
        .opcodes = fun_data_uasyncio_core___init__2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core__enqueue
static const byte fun_data_uasyncio_core__enqueue[105] = {
    0x4b,0x08, // prelude
    0x2e,0x53,0x58,0x59, // names: _enqueue, self, s, idx
     // code info
    0x12,0x5a, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xb1, // LOAD_FAST 1
    0x2b,0x03, // BUILD_LIST 3
    0xc3, // STORE_FAST 3
    0x12,0x44, // LOAD_GLOBAL 'cur_task'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0x12,0x5a, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x5b, // LOAD_METHOD 'register'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5c, // LOAD_ATTR 'POLLIN'
    0x42,0x44, // JUMP 4
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5d, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x61, // JUMP 33
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0x12,0x5a, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x12,0x44, // LOAD_GLOBAL 'cur_task'
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x5e, // LOAD_METHOD 'modify'
    0xb1, // LOAD_FAST 1
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5c, // LOAD_ATTR 'POLLIN'
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5d, // LOAD_ATTR 'POLLOUT'
    0xed, // BINARY_OP 22 __or__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x12,0x44, // LOAD_GLOBAL 'cur_task'
    0x18,0x45, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core__enqueue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_uasyncio_core__enqueue,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 105,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 46,
        .line_info = fun_data_uasyncio_core__enqueue + 6,
        .opcodes = fun_data_uasyncio_core__enqueue + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core__dequeue
static const byte fun_data_uasyncio_core__dequeue[27] = {
    0x22,0x06, // prelude
    0x2f,0x53,0x58, // names: _dequeue, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0x12,0x5a, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x53, // LOAD_NULL
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x5f, // LOAD_METHOD 'unregister'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core__dequeue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core__dequeue,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 47,
        .line_info = fun_data_uasyncio_core__dequeue + 5,
        .opcodes = fun_data_uasyncio_core__dequeue + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_queue_read
static const byte fun_data_uasyncio_core_queue_read[15] = {
    0x2a,0x06, // prelude
    0x30,0x53,0x58, // names: queue_read, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x2e, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_queue_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_queue_read,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 48,
        .line_info = fun_data_uasyncio_core_queue_read + 5,
        .opcodes = fun_data_uasyncio_core_queue_read + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_queue_write
static const byte fun_data_uasyncio_core_queue_write[15] = {
    0x2a,0x06, // prelude
    0x31,0x53,0x58, // names: queue_write, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x2e, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_queue_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_queue_write,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 49,
        .line_info = fun_data_uasyncio_core_queue_write + 5,
        .opcodes = fun_data_uasyncio_core_queue_write + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_remove
static const byte fun_data_uasyncio_core_remove[65] = {
    0x6a,0x06, // prelude
    0x32,0x53,0x0c, // names: remove, self, task
     // code info
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0x5f, // GET_ITER_STACK
    0x4b,0x1f, // FOR_ITER 31
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0xb3, // LOAD_FAST 3
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0xb1, // LOAD_FAST 1
    0xde, // BINARY_OP 7 <is>
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb5, // LOAD_FAST 5
    0xb1, // LOAD_FAST 1
    0xde, // BINARY_OP 7 <is>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb6, // LOAD_FAST 6
    0xc2, // STORE_FAST 2
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x1f, // JUMP -33
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb0, // LOAD_FAST 0
    0x14,0x2f, // LOAD_METHOD '_dequeue'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x42, // JUMP 2
    0x42,0x06, // JUMP -58
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_remove,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 65,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 50,
        .line_info = fun_data_uasyncio_core_remove + 5,
        .opcodes = fun_data_uasyncio_core_remove + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_IOQueue
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_wait_io_event
static const byte fun_data_uasyncio_core_wait_io_event[156] = {
    0x62,0x06, // prelude
    0x33,0x53,0x60, // names: wait_io_event, self, dt
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x61, // LOAD_METHOD 'ipoll'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x89,0x01, // FOR_ITER 137
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR 'map'
    0x12,0x5a, // LOAD_GLOBAL 'id'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5d, // LOAD_ATTR 'POLLOUT'
    0xd2, // UNARY_OP 2 __invert__
    0xef, // BINARY_OP 24 __and__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5c, // LOAD_ATTR 'POLLIN'
    0xd2, // UNARY_OP 2 __invert__
    0xef, // BINARY_OP 24 __and__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb0, // LOAD_FAST 0
    0x14,0x2f, // LOAD_METHOD '_dequeue'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x63, // JUMP 35
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x5e, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5d, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'poller'
    0x14,0x5e, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x08, // LOAD_GLOBAL 'select'
    0x13,0x5c, // LOAD_ATTR 'POLLIN'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0xf4,0x7e, // JUMP -140
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_wait_io_event = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_wait_io_event,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 156,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_uasyncio_core_wait_io_event + 5,
        .opcodes = fun_data_uasyncio_core_wait_io_event + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_core_IOQueue[] = {
    &raw_code_uasyncio_core___init__2,
    &raw_code_uasyncio_core__enqueue,
    &raw_code_uasyncio_core__dequeue,
    &raw_code_uasyncio_core_queue_read,
    &raw_code_uasyncio_core_queue_write,
    &raw_code_uasyncio_core_remove,
    &raw_code_uasyncio_core_wait_io_event,
};

static const mp_raw_code_t raw_code_uasyncio_core_IOQueue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_IOQueue,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = (void *)&children_uasyncio_core_IOQueue,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 7,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_uasyncio_core_IOQueue + 3,
        .opcodes = fun_data_uasyncio_core_IOQueue + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core__promote_to_task
static const byte fun_data_uasyncio_core__promote_to_task[21] = {
    0x19,0x04, // prelude
    0x19,0x34, // names: _promote_to_task, aw
     // code info
    0x12,0x35, // LOAD_GLOBAL 'isinstance'
    0xb0, // LOAD_FAST 0
    0x12,0x0a, // LOAD_GLOBAL 'Task'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x12,0x1a, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core__promote_to_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core__promote_to_task,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 21,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_uasyncio_core__promote_to_task + 4,
        .opcodes = fun_data_uasyncio_core__promote_to_task + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_create_task
static const byte fun_data_uasyncio_core_create_task[40] = {
    0x21,0x04, // prelude
    0x1a,0x36, // names: create_task, coro
     // code info
    0x12,0x37, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x38, // LOAD_CONST_STRING 'send'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x39, // LOAD_GLOBAL 'TypeError'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x0a, // LOAD_GLOBAL 'Task'
    0xb0, // LOAD_FAST 0
    0x12,0x3a, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_create_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_create_task,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 40,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_uasyncio_core_create_task + 4,
        .opcodes = fun_data_uasyncio_core_create_task + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_run_until_complete
static const byte fun_data_uasyncio_core_run_until_complete[297] = {
    0xf1,0x03,0x04, // prelude
    0x1b,0x3d, // names: run_until_complete, main_task
     // code info
    0x12,0x0d, // LOAD_GLOBAL 'CancelledError'
    0x12,0x10, // LOAD_GLOBAL 'Exception'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc1, // STORE_FAST 1
    0x12,0x0d, // LOAD_GLOBAL 'CancelledError'
    0x12,0x3e, // LOAD_GLOBAL 'StopIteration'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc2, // STORE_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc3, // STORE_FAST 3
    0x42,0x6f, // JUMP 47
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xc3, // STORE_FAST 3
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3f, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x2b, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x03, // LOAD_GLOBAL 'ticks_diff'
    0xb4, // LOAD_FAST 4
    0x13,0x40, // LOAD_ATTR 'ph_key'
    0x12,0x06, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0x42,0x48, // JUMP 8
    0x12,0x41, // LOAD_GLOBAL '_io_queue'
    0x13,0x42, // LOAD_ATTR 'map'
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x41, // LOAD_GLOBAL '_io_queue'
    0x14,0x33, // LOAD_METHOD 'wait_io_event'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x0c, // POP_JUMP_IF_TRUE -52
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x43, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x17,0x44, // STORE_GLOBAL 'cur_task'
    0x48,0x22, // SETUP_EXCEPT 34
    0xb4, // LOAD_FAST 4
    0x13,0x45, // LOAD_ATTR 'data'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0xb4, // LOAD_FAST 4
    0x13,0x36, // LOAD_ATTR 'coro'
    0x14,0x38, // LOAD_METHOD 'send'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x45, // STORE_ATTR 'data'
    0xb4, // LOAD_FAST 4
    0x13,0x36, // LOAD_ATTR 'coro'
    0x14,0x46, // LOAD_METHOD 'throw'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0xab,0x01, // POP_EXCEPT_JUMP 171
    0x57, // DUP_TOP
    0xb1, // LOAD_FAST 1
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0xa4,0x81, // POP_JUMP_IF_FALSE 164
    0xc6, // STORE_FAST 6
    0x49,0x99,0x01, // SETUP_FINALLY 153
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0xde, // BINARY_OP 7 <is>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x35, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0x12,0x3e, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb6, // LOAD_FAST 6
    0x13,0x47, // LOAD_ATTR 'value'
    0x63, // RETURN_VALUE
    0xb6, // LOAD_FAST 6
    0x65, // RAISE_OBJ
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x44,0xe0,0x80, // POP_JUMP_IF_FALSE 96
    0x50, // LOAD_CONST_FALSE
    0xc7, // STORE_FAST 7
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x2c, // STORE_ATTR 'state'
    0x42,0x78, // JUMP 56
    0x12,0x48, // LOAD_GLOBAL 'callable'
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb4, // LOAD_FAST 4
    0x14,0x2c, // LOAD_METHOD 'state'
    0xb4, // LOAD_FAST 4
    0xb6, // LOAD_FAST 6
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x2c, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0x42,0x5f, // JUMP 31
    0x42,0x50, // JUMP 16
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x14,0x43, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x14,0x3f, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x2c, // STORE_ATTR 'state'
    0xb7, // LOAD_FAST 7
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0x12,0x35, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0xb4, // LOAD_FAST 4
    0x18,0x45, // STORE_ATTR 'data'
    0x42,0x5e, // JUMP 30
    0xb4, // LOAD_FAST 4
    0x13,0x2c, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0xb5, // LOAD_FAST 5
    0x12,0x14, // LOAD_GLOBAL '_exc_context'
    0x10,0x12, // LOAD_CONST_STRING 'exception'
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x12,0x14, // LOAD_GLOBAL '_exc_context'
    0x10,0x13, // LOAD_CONST_STRING 'future'
    0x56, // STORE_SUBSCR
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x14,0x49, // LOAD_METHOD 'call_exception_handler'
    0x12,0x14, // LOAD_GLOBAL '_exc_context'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x40, // JUMP 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0xec,0x7d, // JUMP -276
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_run_until_complete = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_run_until_complete,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 297,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_uasyncio_core_run_until_complete + 5,
        .opcodes = fun_data_uasyncio_core_run_until_complete + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_run
static const byte fun_data_uasyncio_core_run[14] = {
    0x19,0x04, // prelude
    0x1c,0x36, // names: run, coro
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x1a, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_run = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_run,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_uasyncio_core_run + 4,
        .opcodes = fun_data_uasyncio_core_run + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core__stopper
static const byte fun_data_uasyncio_core__stopper[6] = {
    0x80,0x40,0x02, // prelude
    0x1d, // names: _stopper
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core__stopper = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core__stopper,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 6,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_uasyncio_core__stopper + 4,
        .opcodes = fun_data_uasyncio_core__stopper + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_Loop
static const byte fun_data_uasyncio_core_Loop[52] = {
    0x00,0x02, // prelude
    0x1f, // names: Loop
     // code info
    0x11,0x23, // LOAD_NAME '__name__'
    0x16,0x24, // STORE_NAME '__module__'
    0x10,0x1f, // LOAD_CONST_STRING 'Loop'
    0x16,0x25, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x16,0x4a, // STORE_NAME '_exc_handler'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x1a, // STORE_NAME 'create_task'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x4b, // STORE_NAME 'run_forever'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x4c, // STORE_NAME 'stop'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x4d, // STORE_NAME 'close'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x4e, // STORE_NAME 'set_exception_handler'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x4f, // STORE_NAME 'get_exception_handler'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x50, // STORE_NAME 'default_exception_handler'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x49, // STORE_NAME 'call_exception_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_create_task2
static const byte fun_data_uasyncio_core_create_task2[10] = {
    0x11,0x04, // prelude
    0x1a,0x36, // names: create_task, coro
     // code info
    0x12,0x1a, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_create_task2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_create_task2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_uasyncio_core_create_task2 + 4,
        .opcodes = fun_data_uasyncio_core_create_task2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_run_forever
static const byte fun_data_uasyncio_core_run_forever[26] = {
    0x10,0x02, // prelude
    0x4b, // names: run_forever
     // code info
    0x12,0x0a, // LOAD_GLOBAL 'Task'
    0x12,0x1d, // LOAD_GLOBAL '_stopper'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x3a, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x17,0x1e, // STORE_GLOBAL '_stop_task'
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x1e, // LOAD_GLOBAL '_stop_task'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_run_forever = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_run_forever,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 26,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 75,
        .line_info = fun_data_uasyncio_core_run_forever + 3,
        .opcodes = fun_data_uasyncio_core_run_forever + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_run_until_complete2
static const byte fun_data_uasyncio_core_run_until_complete2[14] = {
    0x19,0x04, // prelude
    0x1b,0x34, // names: run_until_complete, aw
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x19, // LOAD_GLOBAL '_promote_to_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_run_until_complete2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_run_until_complete2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_uasyncio_core_run_until_complete2 + 4,
        .opcodes = fun_data_uasyncio_core_run_until_complete2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_stop
static const byte fun_data_uasyncio_core_stop[24] = {
    0x10,0x02, // prelude
    0x4c, // names: stop
     // code info
    0x12,0x1e, // LOAD_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x3b, // LOAD_GLOBAL '_task_queue'
    0x14,0x3c, // LOAD_METHOD 'push'
    0x12,0x1e, // LOAD_GLOBAL '_stop_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x17,0x1e, // STORE_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_stop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_stop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 76,
        .line_info = fun_data_uasyncio_core_stop + 3,
        .opcodes = fun_data_uasyncio_core_stop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_close
static const byte fun_data_uasyncio_core_close[5] = {
    0x00,0x02, // prelude
    0x4d, // names: close
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_close,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 5,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 77,
        .line_info = fun_data_uasyncio_core_close + 3,
        .opcodes = fun_data_uasyncio_core_close + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_set_exception_handler
static const byte fun_data_uasyncio_core_set_exception_handler[11] = {
    0x11,0x04, // prelude
    0x4e,0x62, // names: set_exception_handler, handler
     // code info
    0xb0, // LOAD_FAST 0
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x18,0x4a, // STORE_ATTR '_exc_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_set_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_set_exception_handler,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 11,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 78,
        .line_info = fun_data_uasyncio_core_set_exception_handler + 4,
        .opcodes = fun_data_uasyncio_core_set_exception_handler + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_get_exception_handler
static const byte fun_data_uasyncio_core_get_exception_handler[8] = {
    0x00,0x02, // prelude
    0x4f, // names: get_exception_handler
     // code info
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x13,0x4a, // LOAD_ATTR '_exc_handler'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_get_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_get_exception_handler,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 8,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 79,
        .line_info = fun_data_uasyncio_core_get_exception_handler + 3,
        .opcodes = fun_data_uasyncio_core_get_exception_handler + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_default_exception_handler
static const byte fun_data_uasyncio_core_default_exception_handler[46] = {
    0x3a,0x06, // prelude
    0x50,0x63,0x64, // names: default_exception_handler, loop, context
     // code info
    0x12,0x65, // LOAD_GLOBAL 'print'
    0xb1, // LOAD_FAST 1
    0x10,0x11, // LOAD_CONST_STRING 'message'
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x65, // LOAD_GLOBAL 'print'
    0x10,0x66, // LOAD_CONST_STRING 'future:'
    0xb1, // LOAD_FAST 1
    0x10,0x13, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x10,0x67, // LOAD_CONST_STRING 'coro='
    0xb1, // LOAD_FAST 1
    0x10,0x13, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x13,0x36, // LOAD_ATTR 'coro'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'sys'
    0x14,0x68, // LOAD_METHOD 'print_exception'
    0xb1, // LOAD_FAST 1
    0x10,0x12, // LOAD_CONST_STRING 'exception'
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_default_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_default_exception_handler,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 46,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 80,
        .line_info = fun_data_uasyncio_core_default_exception_handler + 5,
        .opcodes = fun_data_uasyncio_core_default_exception_handler + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core_Loop
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_call_exception_handler
static const byte fun_data_uasyncio_core_call_exception_handler[22] = {
    0x19,0x04, // prelude
    0x49,0x64, // names: call_exception_handler, context
     // code info
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x13,0x4a, // LOAD_ATTR '_exc_handler'
    0x45,0x04, // JUMP_IF_TRUE_OR_POP 4
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x13,0x50, // LOAD_ATTR 'default_exception_handler'
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_call_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_core_call_exception_handler,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 22,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 73,
        .line_info = fun_data_uasyncio_core_call_exception_handler + 4,
        .opcodes = fun_data_uasyncio_core_call_exception_handler + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_core_Loop[] = {
    &raw_code_uasyncio_core_create_task2,
    &raw_code_uasyncio_core_run_forever,
    &raw_code_uasyncio_core_run_until_complete2,
    &raw_code_uasyncio_core_stop,
    &raw_code_uasyncio_core_close,
    &raw_code_uasyncio_core_set_exception_handler,
    &raw_code_uasyncio_core_get_exception_handler,
    &raw_code_uasyncio_core_default_exception_handler,
    &raw_code_uasyncio_core_call_exception_handler,
};

static const mp_raw_code_t raw_code_uasyncio_core_Loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_Loop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = (void *)&children_uasyncio_core_Loop,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 9,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_uasyncio_core_Loop + 3,
        .opcodes = fun_data_uasyncio_core_Loop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_get_event_loop
static const byte fun_data_uasyncio_core_get_event_loop[10] = {
    0x92,0x80,0x01,0x06, // prelude
    0x20,0x51,0x52, // names: get_event_loop, runq_len, waitq_len
     // code info
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_get_event_loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_core_get_event_loop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 32,
        .line_info = fun_data_uasyncio_core_get_event_loop + 7,
        .opcodes = fun_data_uasyncio_core_get_event_loop + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_current_task
static const byte fun_data_uasyncio_core_current_task[6] = {
    0x00,0x02, // prelude
    0x21, // names: current_task
     // code info
    0x12,0x44, // LOAD_GLOBAL 'cur_task'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_current_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_current_task,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 6,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 33,
        .line_info = fun_data_uasyncio_core_current_task + 3,
        .opcodes = fun_data_uasyncio_core_current_task + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_core__lt_module_gt_
// frozen bytecode for file uasyncio/core.py, scope uasyncio_core_new_event_loop
static const byte fun_data_uasyncio_core_new_event_loop[18] = {
    0x00,0x02, // prelude
    0x22, // names: new_event_loop
     // code info
    0x12,0x09, // LOAD_GLOBAL 'TaskQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x3b, // STORE_GLOBAL '_task_queue'
    0x12,0x18, // LOAD_GLOBAL 'IOQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x41, // STORE_GLOBAL '_io_queue'
    0x12,0x1f, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_core_new_event_loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core_new_event_loop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_uasyncio_core_new_event_loop + 3,
        .opcodes = fun_data_uasyncio_core_new_event_loop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_core__lt_module_gt_[] = {
    &raw_code_uasyncio_core_CancelledError,
    &raw_code_uasyncio_core_TimeoutError,
    &raw_code_uasyncio_core_SingletonGenerator,
    &raw_code_uasyncio_core_sleep_ms,
    &raw_code_uasyncio_core_sleep,
    &raw_code_uasyncio_core_IOQueue,
    &raw_code_uasyncio_core__promote_to_task,
    &raw_code_uasyncio_core_create_task,
    &raw_code_uasyncio_core_run_until_complete,
    &raw_code_uasyncio_core_run,
    &raw_code_uasyncio_core__stopper,
    &raw_code_uasyncio_core_Loop,
    &raw_code_uasyncio_core_get_event_loop,
    &raw_code_uasyncio_core_current_task,
    &raw_code_uasyncio_core_new_event_loop,
};

static const mp_raw_code_t raw_code_uasyncio_core__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_core__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 215,
    #endif
    .children = (void *)&children_uasyncio_core__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 15,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio_core__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio_core__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio_core[105] = {
    MP_QSTR_uasyncio_slash_core_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_ticks_ms,
    MP_QSTR_ticks_diff,
    MP_QSTR_ticks_add,
    MP_QSTR_time,
    MP_QSTR_ticks,
    MP_QSTR_sys,
    MP_QSTR_select,
    MP_QSTR_TaskQueue,
    MP_QSTR_Task,
    MP_QSTR__uasyncio,
    MP_QSTR_task,
    MP_QSTR_CancelledError,
    MP_QSTR_BaseException,
    MP_QSTR_TimeoutError,
    MP_QSTR_Exception,
    MP_QSTR_message,
    MP_QSTR_exception,
    MP_QSTR_future,
    MP_QSTR__exc_context,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_sleep_ms,
    MP_QSTR_sleep,
    MP_QSTR_IOQueue,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_run_until_complete,
    MP_QSTR_run,
    MP_QSTR__stopper,
    MP_QSTR__stop_task,
    MP_QSTR_Loop,
    MP_QSTR_get_event_loop,
    MP_QSTR_current_task,
    MP_QSTR_new_event_loop,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR_t,
    MP_QSTR_sgen,
    MP_QSTR_max,
    MP_QSTR_state,
    MP_QSTR_int,
    MP_QSTR__enqueue,
    MP_QSTR__dequeue,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_remove,
    MP_QSTR_wait_io_event,
    MP_QSTR_aw,
    MP_QSTR_isinstance,
    MP_QSTR_coro,
    MP_QSTR_hasattr,
    MP_QSTR_send,
    MP_QSTR_TypeError,
    MP_QSTR_globals,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_main_task,
    MP_QSTR_StopIteration,
    MP_QSTR_peek,
    MP_QSTR_ph_key,
    MP_QSTR__io_queue,
    MP_QSTR_map,
    MP_QSTR_pop,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_throw,
    MP_QSTR_value,
    MP_QSTR_callable,
    MP_QSTR_call_exception_handler,
    MP_QSTR__exc_handler,
    MP_QSTR_run_forever,
    MP_QSTR_stop,
    MP_QSTR_close,
    MP_QSTR_set_exception_handler,
    MP_QSTR_get_exception_handler,
    MP_QSTR_default_exception_handler,
    MP_QSTR_runq_len,
    MP_QSTR_waitq_len,
    MP_QSTR_self,
    MP_QSTR_exc,
    MP_QSTR___traceback__,
    MP_QSTR_poll,
    MP_QSTR_poller,
    MP_QSTR_s,
    MP_QSTR_idx,
    MP_QSTR_id,
    MP_QSTR_register,
    MP_QSTR_POLLIN,
    MP_QSTR_POLLOUT,
    MP_QSTR_modify,
    MP_QSTR_unregister,
    MP_QSTR_dt,
    MP_QSTR_ipoll,
    MP_QSTR_handler,
    MP_QSTR_loop,
    MP_QSTR_context,
    MP_QSTR_print,
    MP_QSTR_future_colon_,
    MP_QSTR_coro_equals_,
    MP_QSTR_print_exception,
};

// constants
static const mp_obj_str_t const_obj_uasyncio_core_0 = {{&mp_type_str}, 64973, 31, (const byte*)"\x54\x61\x73\x6b\x20\x65\x78\x63\x65\x70\x74\x69\x6f\x6e\x20\x77\x61\x73\x6e\x27\x74\x20\x72\x65\x74\x72\x69\x65\x76\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_uasyncio_core[2] = {
    MP_ROM_PTR(&const_obj_uasyncio_core_0),
    MP_ROM_QSTR(MP_QSTR_coroutine_space_expected),
};

static const mp_frozen_module_t frozen_module_uasyncio_core = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio_core,
        .obj_table = (mp_obj_t *)&const_obj_table_data_uasyncio_core,
    },
    .rc = &raw_code_uasyncio_core__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio_event
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/event.mpy
// - frozen file name: uasyncio/event.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/event.py, scope uasyncio_event__lt_module_gt_
static const byte fun_data_uasyncio_event__lt_module_gt_[59] = {
    0x2c,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Event'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Event'
    0x48,0x15, // SETUP_EXCEPT 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'uio'
    0x16,0x05, // STORE_NAME 'uio'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x11,0x05, // LOAD_NAME 'uio'
    0x13,0x07, // LOAD_ATTR 'IOBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x06, // STORE_NAME 'ThreadSafeFlag'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x11,0x08, // LOAD_NAME 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_event__lt_module_gt_
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_Event
static const byte fun_data_uasyncio_event_Event[33] = {
    0x00,0x02, // prelude
    0x04, // names: Event
     // code info
    0x11,0x09, // LOAD_NAME '__name__'
    0x16,0x0a, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Event'
    0x16,0x0b, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0c, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0d, // STORE_NAME 'is_set'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0e, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0f, // STORE_NAME 'clear'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x10, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_event_Event
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event___init__
static const byte fun_data_uasyncio_event___init__[19] = {
    0x11,0x04, // prelude
    0x0c,0x12, // names: __init__, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x13, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x14, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_uasyncio_event___init__ + 4,
        .opcodes = fun_data_uasyncio_event___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_Event
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_is_set
static const byte fun_data_uasyncio_event_is_set[8] = {
    0x09,0x04, // prelude
    0x0d,0x12, // names: is_set, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'state'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_is_set = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_is_set,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 8,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_uasyncio_event_is_set + 4,
        .opcodes = fun_data_uasyncio_event_is_set + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_Event
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_set
static const byte fun_data_uasyncio_event_set[37] = {
    0x21,0x04, // prelude
    0x0e,0x12, // names: set, self
     // code info
    0x42,0x50, // JUMP 16
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x16, // LOAD_ATTR '_task_queue'
    0x14,0x17, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'waiting'
    0x14,0x18, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'waiting'
    0x14,0x19, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x13, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_set = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_set,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_uasyncio_event_set + 4,
        .opcodes = fun_data_uasyncio_event_set + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_Event
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_clear
static const byte fun_data_uasyncio_event_clear[10] = {
    0x11,0x04, // prelude
    0x0f,0x12, // names: clear, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x13, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_clear = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_clear,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_uasyncio_event_clear + 4,
        .opcodes = fun_data_uasyncio_event_clear + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_Event
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_wait
static const byte fun_data_uasyncio_event_wait[36] = {
    0x99,0x40,0x04, // prelude
    0x10,0x12, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'state'
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'waiting'
    0x14,0x17, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x1a, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x1a, // LOAD_ATTR 'cur_task'
    0x18,0x1b, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_wait = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_wait,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 36,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_uasyncio_event_wait + 5,
        .opcodes = fun_data_uasyncio_event_wait + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_event_Event[] = {
    &raw_code_uasyncio_event___init__,
    &raw_code_uasyncio_event_is_set,
    &raw_code_uasyncio_event_set,
    &raw_code_uasyncio_event_clear,
    &raw_code_uasyncio_event_wait,
};

static const mp_raw_code_t raw_code_uasyncio_event_Event = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_event_Event,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_uasyncio_event_Event,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_uasyncio_event_Event + 3,
        .opcodes = fun_data_uasyncio_event_Event + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event__lt_module_gt_
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_ThreadSafeFlag
static const byte fun_data_uasyncio_event_ThreadSafeFlag[29] = {
    0x00,0x02, // prelude
    0x06, // names: ThreadSafeFlag
     // code info
    0x11,0x09, // LOAD_NAME '__name__'
    0x16,0x0a, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x16,0x0b, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0c, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x11, // STORE_NAME 'ioctl'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0e, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x10, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_event_ThreadSafeFlag
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event___init__2
static const byte fun_data_uasyncio_event___init__2[10] = {
    0x11,0x04, // prelude
    0x0c,0x12, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x1c, // STORE_ATTR '_flag'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event___init__2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event___init__2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_uasyncio_event___init__2 + 4,
        .opcodes = fun_data_uasyncio_event___init__2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_ThreadSafeFlag
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_ioctl
static const byte fun_data_uasyncio_event_ioctl[19] = {
    0x23,0x08, // prelude
    0x11,0x12,0x1d,0x1e, // names: ioctl, self, req, flags
     // code info
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR '_flag'
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_ioctl = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_uasyncio_event_ioctl,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_uasyncio_event_ioctl + 6,
        .opcodes = fun_data_uasyncio_event_ioctl + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_ThreadSafeFlag
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_set2
static const byte fun_data_uasyncio_event_set2[10] = {
    0x11,0x04, // prelude
    0x0e,0x12, // names: set, self
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x1c, // STORE_ATTR '_flag'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_set2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_set2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_uasyncio_event_set2 + 4,
        .opcodes = fun_data_uasyncio_event_set2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_event_ThreadSafeFlag
// frozen bytecode for file uasyncio/event.py, scope uasyncio_event_wait2
static const byte fun_data_uasyncio_event_wait2[27] = {
    0x99,0x40,0x04, // prelude
    0x10,0x12, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR '_flag'
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x1f, // LOAD_ATTR '_io_queue'
    0x14,0x20, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x1c, // STORE_ATTR '_flag'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_event_wait2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_event_wait2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_uasyncio_event_wait2 + 5,
        .opcodes = fun_data_uasyncio_event_wait2 + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_event_ThreadSafeFlag[] = {
    &raw_code_uasyncio_event___init__2,
    &raw_code_uasyncio_event_ioctl,
    &raw_code_uasyncio_event_set2,
    &raw_code_uasyncio_event_wait2,
};

static const mp_raw_code_t raw_code_uasyncio_event_ThreadSafeFlag = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_event_ThreadSafeFlag,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
    #endif
    .children = (void *)&children_uasyncio_event_ThreadSafeFlag,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 4,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_uasyncio_event_ThreadSafeFlag + 3,
        .opcodes = fun_data_uasyncio_event_ThreadSafeFlag + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_event__lt_module_gt_[] = {
    &raw_code_uasyncio_event_Event,
    &raw_code_uasyncio_event_ThreadSafeFlag,
};

static const mp_raw_code_t raw_code_uasyncio_event__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_event__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 59,
    #endif
    .children = (void *)&children_uasyncio_event__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio_event__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio_event__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio_event[33] = {
    MP_QSTR_uasyncio_slash_event_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Event,
    MP_QSTR_uio,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_IOBase,
    MP_QSTR_ImportError,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_is_set,
    MP_QSTR_set,
    MP_QSTR_clear,
    MP_QSTR_wait,
    MP_QSTR_ioctl,
    MP_QSTR_self,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_pop,
    MP_QSTR_peek,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR__flag,
    MP_QSTR_req,
    MP_QSTR_flags,
    MP_QSTR__io_queue,
    MP_QSTR_queue_read,
};

static const mp_frozen_module_t frozen_module_uasyncio_event = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio_event,
        .obj_table = NULL,
    },
    .rc = &raw_code_uasyncio_event__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio_funcs
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/funcs.mpy
// - frozen file name: uasyncio/funcs.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs__lt_module_gt_
static const byte fun_data_uasyncio_funcs__lt_module_gt_[52] = {
    0x18,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x11,0x02, // LOAD_NAME 'core'
    0x13,0x04, // LOAD_ATTR 'sleep'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x05, // STORE_NAME 'wait_for'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x06, // STORE_NAME 'wait_for_ms'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x07, // LOAD_CONST_STRING '_Remove'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x07, // STORE_NAME '_Remove'
    0x53, // LOAD_NULL
    0x2c,0x00, // BUILD_MAP 0
    0x50, // LOAD_CONST_FALSE
    0x10,0x08, // LOAD_CONST_STRING 'return_exceptions'
    0x62, // STORE_MAP
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x09, // STORE_NAME 'gather'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_funcs__lt_module_gt_
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_wait_for
static const byte fun_data_uasyncio_funcs_wait_for[139] = {
    0xeb,0x43,0x88,0x01, // prelude
    0x05,0x0a,0x0b,0x04, // names: wait_for, aw, timeout, sleep
    0x06,0x07, // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0c, // LOAD_METHOD '_promote_to_task'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0x20,0x00,0x02, // MAKE_CLOSURE 0
    0xc3, // STORE_FAST 3
    0x51, // LOAD_CONST_NONE
    0x27,0x06, // STORE_DEREF 6
    0x51, // LOAD_CONST_NONE
    0x27,0x07, // STORE_DEREF 7
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0d, // LOAD_METHOD 'create_task'
    0xb3, // LOAD_FAST 3
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR 'cur_task'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0x48,0x0a, // SETUP_EXCEPT 10
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x4a,0x33, // POP_EXCEPT_JUMP 51
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0f, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x6a, // POP_JUMP_IF_FALSE 42
    0xc5, // STORE_FAST 5
    0x49,0x20, // SETUP_FINALLY 32
    0x25,0x06, // LOAD_DEREF 6
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x25,0x07, // LOAD_DEREF 7
    0x63, // RETURN_VALUE
    0x25,0x06, // LOAD_DEREF 6
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x52, // LOAD_CONST_TRUE
    0x27,0x06, // STORE_DEREF 6
    0xb4, // LOAD_FAST 4
    0x14,0x10, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x65, // RAISE_OBJ
    0x42,0x43, // JUMP 3
    0x25,0x06, // LOAD_DEREF 6
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc5, // STORE_FAST 5
    0x28,0x05, // DELETE_FAST 5
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x52, // LOAD_CONST_TRUE
    0x27,0x06, // STORE_DEREF 6
    0xb4, // LOAD_FAST 4
    0x14,0x10, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x11, // LOAD_ATTR 'TimeoutError'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_funcs_wait_for
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_runner
static const byte fun_data_uasyncio_funcs_runner[57] = {
    0xd8,0x46,0x0a, // prelude
    0x1f,0x29,0x29,0x20,0x0a, // names: runner, *, *, waiter, aw
     // code info
    0x48,0x0a, // SETUP_EXCEPT 10
    0xb3, // LOAD_FAST 3
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x27,0x01, // STORE_DEREF 1
    0x52, // LOAD_CONST_TRUE
    0xc4, // STORE_FAST 4
    0x4a,0x14, // POP_EXCEPT_JUMP 20
    0x57, // DUP_TOP
    0x12,0x21, // LOAD_GLOBAL 'BaseException'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xc5, // STORE_FAST 5
    0x49,0x03, // SETUP_FINALLY 3
    0xb5, // LOAD_FAST 5
    0xc4, // STORE_FAST 4
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc5, // STORE_FAST 5
    0x28,0x05, // DELETE_FAST 5
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x25,0x00, // LOAD_DEREF 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb4, // LOAD_FAST 4
    0x27,0x00, // STORE_DEREF 0
    0xb2, // LOAD_FAST 2
    0x14,0x10, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_funcs_runner = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_funcs_runner,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 57,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_uasyncio_funcs_runner + 8,
        .opcodes = fun_data_uasyncio_funcs_runner + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_funcs_wait_for[] = {
    &raw_code_uasyncio_funcs_runner,
};

static const mp_raw_code_t raw_code_uasyncio_funcs_wait_for = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 3,
    .fun_data = fun_data_uasyncio_funcs_wait_for,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 139,
    #endif
    .children = (void *)&children_uasyncio_funcs_wait_for,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_uasyncio_funcs_wait_for + 8,
        .opcodes = fun_data_uasyncio_funcs_wait_for + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_funcs__lt_module_gt_
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_wait_for_ms
static const byte fun_data_uasyncio_funcs_wait_for_ms[16] = {
    0x2a,0x06, // prelude
    0x06,0x0a,0x0b, // names: wait_for_ms, aw, timeout
     // code info
    0x12,0x05, // LOAD_GLOBAL 'wait_for'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR 'sleep_ms'
    0x34,0x03, // CALL_FUNCTION 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_funcs_wait_for_ms = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_funcs_wait_for_ms,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_uasyncio_funcs_wait_for_ms + 5,
        .opcodes = fun_data_uasyncio_funcs_wait_for_ms + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_funcs__lt_module_gt_
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs__Remove
static const byte fun_data_uasyncio_funcs__Remove[21] = {
    0x08,0x02, // prelude
    0x07, // names: _Remove
     // code info
    0x11,0x13, // LOAD_NAME '__name__'
    0x16,0x14, // STORE_NAME '__module__'
    0x10,0x07, // LOAD_CONST_STRING '_Remove'
    0x16,0x15, // STORE_NAME '__qualname__'
    0x11,0x16, // LOAD_NAME 'staticmethod'
    0x32,0x00, // MAKE_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x17, // STORE_NAME 'remove'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_funcs__Remove
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_remove
static const byte fun_data_uasyncio_funcs_remove[6] = {
    0x09,0x04, // prelude
    0x17,0x22, // names: remove, t
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_funcs_remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_funcs_remove,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 6,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_uasyncio_funcs_remove + 4,
        .opcodes = fun_data_uasyncio_funcs_remove + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_funcs__Remove[] = {
    &raw_code_uasyncio_funcs_remove,
};

static const mp_raw_code_t raw_code_uasyncio_funcs__Remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_funcs__Remove,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 21,
    #endif
    .children = (void *)&children_uasyncio_funcs__Remove,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_uasyncio_funcs__Remove + 3,
        .opcodes = fun_data_uasyncio_funcs__Remove + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_funcs__lt_module_gt_
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_gather
static const byte fun_data_uasyncio_funcs_gather[214] = {
    0xf0,0xca,0x80,0xc0,0x40,0x85,0x01, // prelude
    0x09,0x08, // names: gather, return_exceptions
    0x00,0x07,0x08, // code info
    0xb0, // LOAD_FAST 0
    0xb7, // LOAD_FAST 7
    0xb8, // LOAD_FAST 8
    0x20,0x00,0x03, // MAKE_CLOSURE 0
    0xc2, // STORE_FAST 2
    0x32,0x01, // MAKE_FUNCTION 1
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x18, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5b, // JUMP 27
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x19, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x1a, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x19, // STORE_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x20, // POP_JUMP_IF_TRUE -32
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR 'cur_task'
    0x27,0x07, // STORE_DEREF 7
    0x12,0x18, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x27,0x08, // STORE_DEREF 8
    0x50, // LOAD_CONST_FALSE
    0xc5, // STORE_FAST 5
    0x12,0x07, // LOAD_GLOBAL '_Remove'
    0x25,0x07, // LOAD_DEREF 7
    0x18,0x1b, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x19, // POP_EXCEPT_JUMP 25
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0f, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xc6, // STORE_FAST 6
    0x49,0x06, // SETUP_FINALLY 6
    0x52, // LOAD_CONST_TRUE
    0xc5, // STORE_FAST 5
    0xb6, // LOAD_FAST 6
    0x27,0x08, // STORE_DEREF 8
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x18, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc1,0x80, // JUMP 65
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x19, // LOAD_ATTR 'state'
    0xb2, // LOAD_FAST 2
    0xde, // BINARY_OP 7 <is>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x52, // LOAD_CONST_TRUE
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x19, // STORE_ATTR 'state'
    0xb5, // LOAD_FAST 5
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x14,0x10, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x61, // JUMP 33
    0x12,0x1c, // LOAD_GLOBAL 'isinstance'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x1b, // LOAD_ATTR 'data'
    0x12,0x1d, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x1b, // LOAD_ATTR 'data'
    0x13,0x1e, // LOAD_ATTR 'value'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x56, // STORE_SUBSCR
    0x42,0x48, // JUMP 8
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x1b, // LOAD_ATTR 'data'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb9,0x7f, // POP_JUMP_IF_TRUE -71
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x25,0x08, // LOAD_DEREF 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x25,0x08, // LOAD_DEREF 8
    0x65, // RAISE_OBJ
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
// child of uasyncio_funcs_gather
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs_done
static const byte fun_data_uasyncio_funcs_done[64] = {
    0xb9,0x04,0x0c, // prelude
    0x23,0x29,0x29,0x29,0x22,0x24, // names: done, *, *, *, t, er
     // code info
    0x25,0x01, // LOAD_DEREF 1
    0x13,0x1b, // LOAD_ATTR 'data'
    0x12,0x07, // LOAD_GLOBAL '_Remove'
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x25,0x00, // LOAD_DEREF 0
    0x43,0x4e, // POP_JUMP_IF_TRUE 14
    0x12,0x1c, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0x12,0x1d, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb4, // LOAD_FAST 4
    0x27,0x02, // STORE_DEREF 2
    0x42,0x4c, // JUMP 12
    0x25,0x02, // LOAD_DEREF 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0x27,0x02, // STORE_DEREF 2
    0x25,0x02, // LOAD_DEREF 2
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x26, // LOAD_ATTR '_task_queue'
    0x14,0x27, // LOAD_METHOD 'push'
    0x25,0x01, // LOAD_DEREF 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_funcs_done = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 5,
    .fun_data = fun_data_uasyncio_funcs_done,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 64,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_uasyncio_funcs_done + 9,
        .opcodes = fun_data_uasyncio_funcs_done + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_funcs_gather
// frozen bytecode for file uasyncio/funcs.py, scope uasyncio_funcs__lt_listcomp_gt_
static const byte fun_data_uasyncio_funcs__lt_listcomp_gt_[23] = {
    0x49,0x04, // prelude
    0x28,0x29, // names: <listcomp>, *
     // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0c, // FOR_ITER 12
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0c, // LOAD_METHOD '_promote_to_task'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x32, // JUMP -14
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_funcs__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_funcs__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 23,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_uasyncio_funcs__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_uasyncio_funcs__lt_listcomp_gt_ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_funcs_gather[] = {
    &raw_code_uasyncio_funcs_done,
    &raw_code_uasyncio_funcs__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_uasyncio_funcs_gather = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x0d,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_funcs_gather,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 214,
    #endif
    .children = (void *)&children_uasyncio_funcs_gather,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 13,
        .n_pos_args = 0,
        .n_kwonly_args = 1,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_uasyncio_funcs_gather + 9,
        .opcodes = fun_data_uasyncio_funcs_gather + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_funcs__lt_module_gt_[] = {
    &raw_code_uasyncio_funcs_wait_for,
    &raw_code_uasyncio_funcs_wait_for_ms,
    &raw_code_uasyncio_funcs__Remove,
    &raw_code_uasyncio_funcs_gather,
};

static const mp_raw_code_t raw_code_uasyncio_funcs__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_funcs__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = (void *)&children_uasyncio_funcs__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 4,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio_funcs__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio_funcs__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio_funcs[42] = {
    MP_QSTR_uasyncio_slash_funcs_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_sleep,
    MP_QSTR_wait_for,
    MP_QSTR_wait_for_ms,
    MP_QSTR__Remove,
    MP_QSTR_return_exceptions,
    MP_QSTR_gather,
    MP_QSTR_aw,
    MP_QSTR_timeout,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_cur_task,
    MP_QSTR_CancelledError,
    MP_QSTR_cancel,
    MP_QSTR_TimeoutError,
    MP_QSTR_sleep_ms,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_staticmethod,
    MP_QSTR_remove,
    MP_QSTR_len,
    MP_QSTR_state,
    MP_QSTR_RuntimeError,
    MP_QSTR_data,
    MP_QSTR_isinstance,
    MP_QSTR_StopIteration,
    MP_QSTR_value,
    MP_QSTR_runner,
    MP_QSTR_waiter,
    MP_QSTR_BaseException,
    MP_QSTR_t,
    MP_QSTR_done,
    MP_QSTR_er,
    MP_QSTR_gather_task,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR__star_,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_uasyncio_funcs[1] = {
    MP_ROM_QSTR(MP_QSTR_can_squot_t_space_gather),
};

static const mp_frozen_module_t frozen_module_uasyncio_funcs = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio_funcs,
        .obj_table = (mp_obj_t *)&const_obj_table_data_uasyncio_funcs,
    },
    .rc = &raw_code_uasyncio_funcs__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio_lock
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/lock.mpy
// - frozen file name: uasyncio/lock.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock__lt_module_gt_
static const byte fun_data_uasyncio_lock__lt_module_gt_[26] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Lock'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Lock'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_lock__lt_module_gt_
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock_Lock
static const byte fun_data_uasyncio_lock_Lock[37] = {
    0x00,0x02, // prelude
    0x04, // names: Lock
     // code info
    0x11,0x05, // LOAD_NAME '__name__'
    0x16,0x06, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Lock'
    0x16,0x07, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'locked'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'release'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'acquire'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0c, // STORE_NAME '__aenter__'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0d, // STORE_NAME '__aexit__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock___init__
static const byte fun_data_uasyncio_lock___init__[19] = {
    0x11,0x04, // prelude
    0x08,0x0e, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x10, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_lock___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_uasyncio_lock___init__ + 4,
        .opcodes = fun_data_uasyncio_lock___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock_locked
static const byte fun_data_uasyncio_lock_locked[10] = {
    0x11,0x04, // prelude
    0x09,0x0e, // names: locked, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock_locked = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_lock_locked,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 10,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_uasyncio_lock_locked + 4,
        .opcodes = fun_data_uasyncio_lock_locked + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock_release
static const byte fun_data_uasyncio_lock_release[57] = {
    0x19,0x04, // prelude
    0x0a,0x0e, // names: release, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x12, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'waiting'
    0x14,0x13, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'waiting'
    0x14,0x14, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x15, // LOAD_ATTR '_task_queue'
    0x14,0x16, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'state'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x44, // JUMP 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock_release = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_lock_release,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 57,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_uasyncio_lock_release + 4,
        .opcodes = fun_data_uasyncio_lock_release + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock_acquire
static const byte fun_data_uasyncio_lock_acquire[89] = {
    0xb9,0x42,0x04, // prelude
    0x0b,0x0e, // names: acquire, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'state'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x44,0xc6,0x80, // POP_JUMP_IF_FALSE 70
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'waiting'
    0x14,0x16, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x17, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x17, // LOAD_ATTR 'cur_task'
    0x18,0x18, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x2a, // POP_EXCEPT_JUMP 42
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x19, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x61, // POP_JUMP_IF_FALSE 33
    0xc1, // STORE_FAST 1
    0x49,0x17, // SETUP_FINALLY 23
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x17, // LOAD_ATTR 'cur_task'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'state'
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc1, // STORE_FAST 1
    0x28,0x01, // DELETE_FAST 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock_acquire = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_lock_acquire,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 89,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_uasyncio_lock_acquire + 5,
        .opcodes = fun_data_uasyncio_lock_acquire + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock___aenter__
static const byte fun_data_uasyncio_lock___aenter__[14] = {
    0x91,0x40,0x04, // prelude
    0x0c,0x0e, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'acquire'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock___aenter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_lock___aenter__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_uasyncio_lock___aenter__ + 5,
        .opcodes = fun_data_uasyncio_lock___aenter__ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_lock_Lock
// frozen bytecode for file uasyncio/lock.py, scope uasyncio_lock___aexit__
static const byte fun_data_uasyncio_lock___aexit__[14] = {
    0xa8,0x44,0x0a, // prelude
    0x0d,0x0e,0x1a,0x1b,0x1c, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_lock___aexit__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_lock___aexit__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_uasyncio_lock___aexit__ + 8,
        .opcodes = fun_data_uasyncio_lock___aexit__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_lock_Lock[] = {
    &raw_code_uasyncio_lock___init__,
    &raw_code_uasyncio_lock_locked,
    &raw_code_uasyncio_lock_release,
    &raw_code_uasyncio_lock_acquire,
    &raw_code_uasyncio_lock___aenter__,
    &raw_code_uasyncio_lock___aexit__,
};

static const mp_raw_code_t raw_code_uasyncio_lock_Lock = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_lock_Lock,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = (void *)&children_uasyncio_lock_Lock,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 6,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_uasyncio_lock_Lock + 3,
        .opcodes = fun_data_uasyncio_lock_Lock + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_lock__lt_module_gt_[] = {
    &raw_code_uasyncio_lock_Lock,
};

static const mp_raw_code_t raw_code_uasyncio_lock__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_lock__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 26,
    #endif
    .children = (void *)&children_uasyncio_lock__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio_lock__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio_lock__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio_lock[29] = {
    MP_QSTR_uasyncio_slash_lock_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Lock,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_locked,
    MP_QSTR_release,
    MP_QSTR_acquire,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR_self,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR_RuntimeError,
    MP_QSTR_peek,
    MP_QSTR_pop,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_CancelledError,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_uasyncio_lock[1] = {
    MP_ROM_QSTR(MP_QSTR_Lock_space_not_space_acquired),
};

static const mp_frozen_module_t frozen_module_uasyncio_lock = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio_lock,
        .obj_table = (mp_obj_t *)&const_obj_table_data_uasyncio_lock,
    },
    .rc = &raw_code_uasyncio_lock__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio_stream
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/uasyncio/stream.mpy
// - frozen file name: uasyncio/stream.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream__lt_module_gt_
static const byte fun_data_uasyncio_stream__lt_module_gt_[84] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Stream'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Stream'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x16,0x05, // STORE_NAME 'StreamReader'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x16,0x06, // STORE_NAME 'StreamWriter'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x07, // STORE_NAME 'open_connection'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x08, // LOAD_CONST_STRING 'Server'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'Server'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x09, // STORE_NAME 'start_server'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x0a, // STORE_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x13,0x0b, // LOAD_ATTR 'wait_closed'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x0c, // STORE_ATTR 'aclose'
    0x11,0x0a, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x0d, // STORE_ATTR 'awrite'
    0x11,0x0a, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x0e, // STORE_ATTR 'awritestr'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_stream__lt_module_gt_
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_Stream
static const byte fun_data_uasyncio_stream_Stream[66] = {
    0x08,0x02, // prelude
    0x04, // names: Stream
     // code info
    0x11,0x0f, // LOAD_NAME '__name__'
    0x16,0x10, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Stream'
    0x16,0x11, // STORE_NAME '__qualname__'
    0x2c,0x00, // BUILD_MAP 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x12, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x13, // STORE_NAME 'get_extra_info'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x14, // STORE_NAME '__aenter__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x15, // STORE_NAME '__aexit__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x16, // STORE_NAME 'close'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0b, // STORE_NAME 'wait_closed'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x17, // STORE_NAME 'read'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x18, // STORE_NAME 'readinto'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x19, // STORE_NAME 'readexactly'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x1a, // STORE_NAME 'readline'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x1b, // STORE_NAME 'write'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x1c, // STORE_NAME 'drain'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream___init__
static const byte fun_data_uasyncio_stream___init__[22] = {
    0xa3,0x01,0x08, // prelude
    0x12,0x35,0x3b,0x3c, // names: __init__, self, s, e
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x3b, // STORE_ATTR 's'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x3c, // STORE_ATTR 'e'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb0, // LOAD_FAST 0
    0x18,0x3d, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_uasyncio_stream___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 22,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_uasyncio_stream___init__ + 7,
        .opcodes = fun_data_uasyncio_stream___init__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_get_extra_info
static const byte fun_data_uasyncio_stream_get_extra_info[11] = {
    0x1a,0x06, // prelude
    0x13,0x35,0x3e, // names: get_extra_info, self, v
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x3c, // LOAD_ATTR 'e'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_get_extra_info = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_get_extra_info,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 11,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_uasyncio_stream_get_extra_info + 5,
        .opcodes = fun_data_uasyncio_stream_get_extra_info + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream___aenter__
static const byte fun_data_uasyncio_stream___aenter__[7] = {
    0x89,0x40,0x04, // prelude
    0x14,0x35, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream___aenter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream___aenter__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 7,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_uasyncio_stream___aenter__ + 5,
        .opcodes = fun_data_uasyncio_stream___aenter__ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream___aexit__
static const byte fun_data_uasyncio_stream___aexit__[19] = {
    0xa8,0x44,0x0a, // prelude
    0x15,0x35,0x3f,0x40,0x41, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x16, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream___aexit__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_stream___aexit__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_uasyncio_stream___aexit__ + 8,
        .opcodes = fun_data_uasyncio_stream___aexit__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_close
static const byte fun_data_uasyncio_stream_close[6] = {
    0x09,0x04, // prelude
    0x16,0x35, // names: close, self
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_close,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 6,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_uasyncio_stream_close + 4,
        .opcodes = fun_data_uasyncio_stream_close + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_wait_closed
static const byte fun_data_uasyncio_stream_wait_closed[15] = {
    0x91,0x40,0x04, // prelude
    0x0b,0x35, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x16, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_wait_closed = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_wait_closed,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_uasyncio_stream_wait_closed + 5,
        .opcodes = fun_data_uasyncio_stream_wait_closed + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_read
static const byte fun_data_uasyncio_stream_read[28] = {
    0xa2,0x40,0x06, // prelude
    0x17,0x35,0x42, // names: read, self, n
     // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x43, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x17, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_read,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_uasyncio_stream_read + 6,
        .opcodes = fun_data_uasyncio_stream_read + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_readinto
static const byte fun_data_uasyncio_stream_readinto[28] = {
    0xa2,0x40,0x06, // prelude
    0x18,0x35,0x36, // names: readinto, self, buf
     // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x43, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x18, // LOAD_METHOD 'readinto'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_readinto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_readinto,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_uasyncio_stream_readinto + 6,
        .opcodes = fun_data_uasyncio_stream_readinto + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_readexactly
static const byte fun_data_uasyncio_stream_readexactly[66] = {
    0xb2,0x40,0x06, // prelude
    0x19,0x35,0x42, // names: readexactly, self, n
     // code info
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xc2, // STORE_FAST 2
    0x42,0x72, // JUMP 50
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x43, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x17, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x3a, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x44, // LOAD_GLOBAL 'EOFError'
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x12,0x3a, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xe6, // BINARY_OP 15 __isub__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x0b, // POP_JUMP_IF_TRUE -53
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_readexactly = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_readexactly,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 66,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_uasyncio_stream_readexactly + 6,
        .opcodes = fun_data_uasyncio_stream_readexactly + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_readline
static const byte fun_data_uasyncio_stream_readline[49] = {
    0xa9,0x40,0x04, // prelude
    0x1a,0x35, // names: readline, self
     // code info
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x43, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x1a, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x8a, // LOAD_CONST_SMALL_INT 10
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0x42,0x19, // JUMP -39
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_readline = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_readline,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 49,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_uasyncio_stream_readline + 5,
        .opcodes = fun_data_uasyncio_stream_readline + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_write
static const byte fun_data_uasyncio_stream_write[16] = {
    0x22,0x06, // prelude
    0x1b,0x35,0x36, // names: write, self, buf
     // code info
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x3d, // LOAD_ATTR 'out_buf'
    0xb1, // LOAD_FAST 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x3d, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_write,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_uasyncio_stream_write + 5,
        .opcodes = fun_data_uasyncio_stream_write + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Stream
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_drain
static const byte fun_data_uasyncio_stream_drain[70] = {
    0xc1,0x40,0x04, // prelude
    0x1c,0x35, // names: drain, self
     // code info
    0x12,0x39, // LOAD_GLOBAL 'memoryview'
    0xb0, // LOAD_FAST 0
    0x13,0x3d, // LOAD_ATTR 'out_buf'
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x42,0x65, // JUMP 37
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x2a, // LOAD_METHOD 'queue_write'
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR 's'
    0x14,0x1b, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x3a, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x12, // POP_JUMP_IF_TRUE -46
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb0, // LOAD_FAST 0
    0x18,0x3d, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_drain = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_drain,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 70,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_uasyncio_stream_drain + 5,
        .opcodes = fun_data_uasyncio_stream_drain + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_stream_Stream[] = {
    &raw_code_uasyncio_stream___init__,
    &raw_code_uasyncio_stream_get_extra_info,
    &raw_code_uasyncio_stream___aenter__,
    &raw_code_uasyncio_stream___aexit__,
    &raw_code_uasyncio_stream_close,
    &raw_code_uasyncio_stream_wait_closed,
    &raw_code_uasyncio_stream_read,
    &raw_code_uasyncio_stream_readinto,
    &raw_code_uasyncio_stream_readexactly,
    &raw_code_uasyncio_stream_readline,
    &raw_code_uasyncio_stream_write,
    &raw_code_uasyncio_stream_drain,
};

static const mp_raw_code_t raw_code_uasyncio_stream_Stream = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_stream_Stream,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 66,
    #endif
    .children = (void *)&children_uasyncio_stream_Stream,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 12,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_uasyncio_stream_Stream + 3,
        .opcodes = fun_data_uasyncio_stream_Stream + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream__lt_module_gt_
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_open_connection
static const byte fun_data_uasyncio_stream_open_connection[120] = {
    0xea,0x42,0x06, // prelude
    0x07,0x1d,0x1e, // names: open_connection, host, port
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x1f, // LOAD_CONST_STRING 'EINPROGRESS'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x20, // IMPORT_NAME 'uerrno'
    0x1c,0x1f, // IMPORT_FROM 'EINPROGRESS'
    0xc2, // STORE_FAST 2
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x21, // IMPORT_NAME 'usocket'
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x22, // LOAD_METHOD 'getaddrinfo'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb3, // LOAD_FAST 3
    0x13,0x23, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x14,0x24, // LOAD_METHOD 'socket'
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x36,0x03, // CALL_METHOD 3
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x25, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0xc6, // STORE_FAST 6
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb5, // LOAD_FAST 5
    0x14,0x26, // LOAD_METHOD 'connect'
    0xb4, // LOAD_FAST 4
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x1b, // POP_EXCEPT_JUMP 27
    0x57, // DUP_TOP
    0x12,0x27, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xc7, // STORE_FAST 7
    0x49,0x0a, // SETUP_FINALLY 10
    0xb7, // LOAD_FAST 7
    0x13,0x28, // LOAD_ATTR 'errno'
    0xb2, // LOAD_FAST 2
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb7, // LOAD_FAST 7
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x2a, // LOAD_METHOD 'queue_write'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_open_connection = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_uasyncio_stream_open_connection,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 120,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_uasyncio_stream_open_connection + 6,
        .opcodes = fun_data_uasyncio_stream_open_connection + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream__lt_module_gt_
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_Server
static const byte fun_data_uasyncio_stream_Server[33] = {
    0x00,0x02, // prelude
    0x08, // names: Server
     // code info
    0x11,0x0f, // LOAD_NAME '__name__'
    0x16,0x10, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'Server'
    0x16,0x11, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x14, // STORE_NAME '__aenter__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x15, // STORE_NAME '__aexit__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x16, // STORE_NAME 'close'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'wait_closed'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x2b, // STORE_NAME '_serve'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio_stream_Server
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream___aenter__2
static const byte fun_data_uasyncio_stream___aenter__2[7] = {
    0x89,0x40,0x04, // prelude
    0x14,0x35, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream___aenter__2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream___aenter__2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 7,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_uasyncio_stream___aenter__2 + 5,
        .opcodes = fun_data_uasyncio_stream___aenter__2 + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Server
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream___aexit__2
static const byte fun_data_uasyncio_stream___aexit__2[25] = {
    0xa8,0x44,0x0a, // prelude
    0x15,0x35,0x3f,0x40,0x41, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x16, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'wait_closed'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream___aexit__2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_stream___aexit__2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_uasyncio_stream___aexit__2 + 8,
        .opcodes = fun_data_uasyncio_stream___aexit__2 + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Server
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_close2
static const byte fun_data_uasyncio_stream_close2[14] = {
    0x11,0x04, // prelude
    0x16,0x35, // names: close, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x34, // LOAD_ATTR 'task'
    0x14,0x45, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_close2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_close2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_uasyncio_stream_close2 + 4,
        .opcodes = fun_data_uasyncio_stream_close2 + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Server
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_wait_closed2
static const byte fun_data_uasyncio_stream_wait_closed2[14] = {
    0x91,0x40,0x04, // prelude
    0x0b,0x35, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x34, // LOAD_ATTR 'task'
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_wait_closed2 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio_stream_wait_closed2,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 14,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_uasyncio_stream_wait_closed2 + 5,
        .opcodes = fun_data_uasyncio_stream_wait_closed2 + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream_Server
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream__serve
static const byte fun_data_uasyncio_stream__serve[98] = {
    0xdf,0x40,0x08, // prelude
    0x2b,0x35,0x3b,0x2c, // names: _serve, self, s, cb
     // code info
    0x48,0x0d, // SETUP_EXCEPT 13
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x29, // LOAD_ATTR '_io_queue'
    0x14,0x43, // LOAD_METHOD 'queue_read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x14, // POP_EXCEPT_JUMP 20
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x46, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x16, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb1, // LOAD_FAST 1
    0x14,0x47, // LOAD_METHOD 'accept'
    0x36,0x00, // CALL_METHOD 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0x4a,0x07, // POP_EXCEPT_JUMP 7
    0x59, // POP_TOP
    0x40,0x63,0x01, // UNWIND_JUMP 35
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x14,0x25, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb3, // LOAD_FAST 3
    0x2c,0x01, // BUILD_MAP 1
    0xb4, // LOAD_FAST 4
    0x10,0x48, // LOAD_CONST_STRING 'peername'
    0x62, // STORE_MAP
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x33, // LOAD_METHOD 'create_task'
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xb5, // LOAD_FAST 5
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xa7,0x7f, // JUMP -89
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream__serve = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 3,
    .fun_data = fun_data_uasyncio_stream__serve,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 98,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 1,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_uasyncio_stream__serve + 7,
        .opcodes = fun_data_uasyncio_stream__serve + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_stream_Server[] = {
    &raw_code_uasyncio_stream___aenter__2,
    &raw_code_uasyncio_stream___aexit__2,
    &raw_code_uasyncio_stream_close2,
    &raw_code_uasyncio_stream_wait_closed2,
    &raw_code_uasyncio_stream__serve,
};

static const mp_raw_code_t raw_code_uasyncio_stream_Server = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_stream_Server,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_uasyncio_stream_Server,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_uasyncio_stream_Server + 3,
        .opcodes = fun_data_uasyncio_stream_Server + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream__lt_module_gt_
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_start_server
static const byte fun_data_uasyncio_stream_start_server[88] = {
    0xe0,0x45,0x0a, // prelude
    0x09,0x2c,0x1d,0x1e,0x2d, // names: start_server, cb, host, port, backlog
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x21, // IMPORT_NAME 'usocket'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x22, // LOAD_METHOD 'getaddrinfo'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb4, // LOAD_FAST 4
    0x14,0x24, // LOAD_METHOD 'socket'
    0x36,0x00, // CALL_METHOD 0
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x25, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x2e, // LOAD_METHOD 'setsockopt'
    0xb4, // LOAD_FAST 4
    0x13,0x2f, // LOAD_ATTR 'SOL_SOCKET'
    0xb4, // LOAD_FAST 4
    0x13,0x30, // LOAD_ATTR 'SO_REUSEADDR'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x31, // LOAD_METHOD 'bind'
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x32, // LOAD_METHOD 'listen'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'Server'
    0x34,0x00, // CALL_FUNCTION 0
    0xc6, // STORE_FAST 6
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x33, // LOAD_METHOD 'create_task'
    0xb6, // LOAD_FAST 6
    0x14,0x2b, // LOAD_METHOD '_serve'
    0xb5, // LOAD_FAST 5
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0x36,0x01, // CALL_METHOD 1
    0xb6, // LOAD_FAST 6
    0x18,0x34, // STORE_ATTR 'task'
    0xb6, // LOAD_FAST 6
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_start_server = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_stream_start_server,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 88,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_uasyncio_stream_start_server + 8,
        .opcodes = fun_data_uasyncio_stream_start_server + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of uasyncio_stream__lt_module_gt_
// frozen bytecode for file uasyncio/stream.py, scope uasyncio_stream_stream_awrite
static const byte fun_data_uasyncio_stream_stream_awrite[63] = {
    0xb8,0xc4,0x01,0x0a, // prelude
    0x0a,0x35,0x36,0x37,0x38, // names: stream_awrite, self, buf, off, sz
     // code info
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x12,0x39, // LOAD_GLOBAL 'memoryview'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x3a, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x1b, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x1c, // LOAD_METHOD 'drain'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio_stream_stream_awrite = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_uasyncio_stream_stream_awrite,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 63,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_uasyncio_stream_stream_awrite + 9,
        .opcodes = fun_data_uasyncio_stream_stream_awrite + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio_stream__lt_module_gt_[] = {
    &raw_code_uasyncio_stream_Stream,
    &raw_code_uasyncio_stream_open_connection,
    &raw_code_uasyncio_stream_Server,
    &raw_code_uasyncio_stream_start_server,
    &raw_code_uasyncio_stream_stream_awrite,
};

static const mp_raw_code_t raw_code_uasyncio_stream__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio_stream__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 84,
    #endif
    .children = (void *)&children_uasyncio_stream__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio_stream__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio_stream__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio_stream[73] = {
    MP_QSTR_uasyncio_slash_stream_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Stream,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR_open_connection,
    MP_QSTR_Server,
    MP_QSTR_start_server,
    MP_QSTR_stream_awrite,
    MP_QSTR_wait_closed,
    MP_QSTR_aclose,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR___init__,
    MP_QSTR_get_extra_info,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR_close,
    MP_QSTR_read,
    MP_QSTR_readinto,
    MP_QSTR_readexactly,
    MP_QSTR_readline,
    MP_QSTR_write,
    MP_QSTR_drain,
    MP_QSTR_host,
    MP_QSTR_port,
    MP_QSTR_EINPROGRESS,
    MP_QSTR_uerrno,
    MP_QSTR_usocket,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_socket,
    MP_QSTR_setblocking,
    MP_QSTR_connect,
    MP_QSTR_OSError,
    MP_QSTR_errno,
    MP_QSTR__io_queue,
    MP_QSTR_queue_write,
    MP_QSTR__serve,
    MP_QSTR_cb,
    MP_QSTR_backlog,
    MP_QSTR_setsockopt,
    MP_QSTR_SOL_SOCKET,
    MP_QSTR_SO_REUSEADDR,
    MP_QSTR_bind,
    MP_QSTR_listen,
    MP_QSTR_create_task,
    MP_QSTR_task,
    MP_QSTR_self,
    MP_QSTR_buf,
    MP_QSTR_off,
    MP_QSTR_sz,
    MP_QSTR_memoryview,
    MP_QSTR_len,
    MP_QSTR_s,
    MP_QSTR_e,
    MP_QSTR_out_buf,
    MP_QSTR_v,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
    MP_QSTR_n,
    MP_QSTR_queue_read,
    MP_QSTR_EOFError,
    MP_QSTR_cancel,
    MP_QSTR_CancelledError,
    MP_QSTR_accept,
    MP_QSTR_peername,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_uasyncio_stream[4] = {
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
};

static const mp_frozen_module_t frozen_module_uasyncio_stream = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio_stream,
        .obj_table = (mp_obj_t *)&const_obj_table_data_uasyncio_stream,
    },
    .rc = &raw_code_uasyncio_stream__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module webrepl
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/webrepl.mpy
// - frozen file name: webrepl.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file webrepl.py, scope webrepl__lt_module_gt_
static const byte fun_data_webrepl__lt_module_gt_[99] = {
    0x08,0x24, // prelude
    0x01, // names: <module>
    0x20,0x26,0x26,0x26,0x26,0x26,0x46,0x23,0x63,0x84,0x13,0x84,0x15,0x84,0x09,0x8c,0x11, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'socket'
    0x16,0x02, // STORE_NAME 'socket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'uos'
    0x16,0x03, // STORE_NAME 'uos'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'network'
    0x16,0x04, // STORE_NAME 'network'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'uwebsocket'
    0x16,0x05, // STORE_NAME 'uwebsocket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'websocket_helper'
    0x16,0x06, // STORE_NAME 'websocket_helper'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME '_webrepl'
    0x16,0x07, // STORE_NAME '_webrepl'
    0x51, // LOAD_CONST_NONE
    0x17,0x08, // STORE_GLOBAL 'listen_s'
    0x51, // LOAD_CONST_NONE
    0x17,0x09, // STORE_GLOBAL 'client_s'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0a, // STORE_NAME 'setup_conn'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0b, // STORE_NAME 'accept_conn'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'stop'
    0x22,0x80,0xc0,0x4a, // LOAD_CONST_SMALL_INT 8266
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x0d, // STORE_NAME 'start'
    0x22,0x80,0xc0,0x4a, // LOAD_CONST_SMALL_INT 8266
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x0e, // STORE_NAME 'start_foreground'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_setup_conn
static const byte fun_data_webrepl_setup_conn[144] = {
    0x6a,0x24, // prelude
    0x0a,0x0f,0x10, // names: setup_conn, port, accept_handler
    0x80,0x0d,0x20,0x28,0x50,0x2a,0x46,0x28,0x28,0x23,0x2d,0x2e,0x28,0x27,0x34, // code info
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x02, // LOAD_METHOD 'socket'
    0x36,0x00, // CALL_METHOD 0
    0x17,0x08, // STORE_GLOBAL 'listen_s'
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x14,0x11, // LOAD_METHOD 'setsockopt'
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x12, // LOAD_ATTR 'SOL_SOCKET'
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x13, // LOAD_ATTR 'SO_REUSEADDR'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x14, // LOAD_METHOD 'getaddrinfo'
    0x10,0x15, // LOAD_CONST_STRING '0.0.0.0'
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x14,0x16, // LOAD_METHOD 'bind'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x14,0x17, // LOAD_METHOD 'listen'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x14,0x11, // LOAD_METHOD 'setsockopt'
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x12, // LOAD_ATTR 'SOL_SOCKET'
    0x94, // LOAD_CONST_SMALL_INT 20
    0xb1, // LOAD_FAST 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'network'
    0x13,0x18, // LOAD_ATTR 'AP_IF'
    0x12,0x04, // LOAD_GLOBAL 'network'
    0x13,0x19, // LOAD_ATTR 'STA_IF'
    0x2a,0x02, // BUILD_TUPLE 2
    0x5f, // GET_ITER_STACK
    0x4b,0x24, // FOR_ITER 36
    0xc4, // STORE_FAST 4
    0x12,0x04, // LOAD_GLOBAL 'network'
    0x14,0x1a, // LOAD_METHOD 'WLAN'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x1b, // LOAD_METHOD 'active'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb5, // LOAD_FAST 5
    0x14,0x1d, // LOAD_METHOD 'ifconfig'
    0x36,0x00, // CALL_METHOD 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x1a, // JUMP -38
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_conn = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_webrepl_setup_conn,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 144,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_webrepl_setup_conn + 5,
        .opcodes = fun_data_webrepl_setup_conn + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_accept_conn
static const byte fun_data_webrepl_accept_conn[146] = {
    0x49,0x28, // prelude
    0x0b,0x1e, // names: accept_conn, listen_sock
    0x80,0x20,0x20,0x29,0x28,0x28,0x23,0x2a,0x26,0x22,0x28,0x23,0x28,0x29,0x28,0x47,0x2a,0x2f, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x1f, // LOAD_METHOD 'accept'
    0x36,0x00, // CALL_METHOD 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc1, // STORE_FAST 1
    0xc2, // STORE_FAST 2
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x14,0x20, // LOAD_METHOD 'dupterm'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x14,0x20, // LOAD_METHOD 'dupterm'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xb2, // LOAD_FAST 2
    0x10,0x21, // LOAD_CONST_STRING 'rejected'
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x22, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x17,0x09, // STORE_GLOBAL 'client_s'
    0x12,0x06, // LOAD_GLOBAL 'websocket_helper'
    0x14,0x23, // LOAD_METHOD 'server_handshake'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x05, // LOAD_GLOBAL 'uwebsocket'
    0x14,0x24, // LOAD_METHOD 'websocket'
    0xb1, // LOAD_FAST 1
    0x52, // LOAD_CONST_TRUE
    0x36,0x02, // CALL_METHOD 2
    0xc4, // STORE_FAST 4
    0x12,0x07, // LOAD_GLOBAL '_webrepl'
    0x14,0x07, // LOAD_METHOD '_webrepl'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x14,0x25, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x26, // LOAD_GLOBAL 'hasattr'
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb1, // LOAD_FAST 1
    0x14,0x11, // LOAD_METHOD 'setsockopt'
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x12, // LOAD_ATTR 'SOL_SOCKET'
    0x94, // LOAD_CONST_SMALL_INT 20
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x13,0x27, // LOAD_ATTR 'dupterm_notify'
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x14,0x20, // LOAD_METHOD 'dupterm'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_accept_conn = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_webrepl_accept_conn,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 146,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_webrepl_accept_conn + 4,
        .opcodes = fun_data_webrepl_accept_conn + 22,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_stop
static const byte fun_data_webrepl_stop[42] = {
    0x10,0x10, // prelude
    0x0c, // names: stop
    0x80,0x35,0x20,0x28,0x24,0x27,0x24, // code info
    0x12,0x03, // LOAD_GLOBAL 'uos'
    0x14,0x20, // LOAD_METHOD 'dupterm'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x09, // LOAD_GLOBAL 'client_s'
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x09, // LOAD_GLOBAL 'client_s'
    0x14,0x22, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x08, // LOAD_GLOBAL 'listen_s'
    0x14,0x22, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_stop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl_stop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 42,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_webrepl_stop + 3,
        .opcodes = fun_data_webrepl_stop + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_start
static const byte fun_data_webrepl_start[101] = {
    0xbe,0x80,0x01,0x1e, // prelude
    0x0d,0x0f,0x28, // names: start, port, password
    0x80,0x3e,0x25,0x25,0x22,0x45,0x2a,0x28,0x4a,0x4c,0x28,0x28, // code info
    0x12,0x0c, // LOAD_GLOBAL 'stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0x48,0x20, // SETUP_EXCEPT 32
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x29, // IMPORT_NAME 'webrepl_cfg'
    0xc2, // STORE_FAST 2
    0x12,0x07, // LOAD_GLOBAL '_webrepl'
    0x14,0x28, // LOAD_METHOD 'password'
    0xb2, // LOAD_FAST 2
    0x13,0x2a, // LOAD_ATTR 'PASS'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x0a, // LOAD_GLOBAL 'setup_conn'
    0xb0, // LOAD_FAST 0
    0x12,0x0b, // LOAD_GLOBAL 'accept_conn'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x59, // POP_TOP
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x57, // JUMP 23
    0x12,0x07, // LOAD_GLOBAL '_webrepl'
    0x14,0x28, // LOAD_METHOD 'password'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x0a, // LOAD_GLOBAL 'setup_conn'
    0xb0, // LOAD_FAST 0
    0x12,0x0b, // LOAD_GLOBAL 'accept_conn'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_start = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_webrepl_start,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 101,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_webrepl_start + 7,
        .opcodes = fun_data_webrepl_start + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_start_foreground
static const byte fun_data_webrepl_start_foreground[29] = {
    0xa1,0x01,0x0c, // prelude
    0x0e,0x0f, // names: start_foreground, port
    0x80,0x4f,0x25,0x27, // code info
    0x12,0x0c, // LOAD_GLOBAL 'stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x0a, // LOAD_GLOBAL 'setup_conn'
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0x12,0x0b, // LOAD_GLOBAL 'accept_conn'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_start_foreground = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_webrepl_start_foreground,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_webrepl_start_foreground + 5,
        .opcodes = fun_data_webrepl_start_foreground + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_webrepl__lt_module_gt_[] = {
    &raw_code_webrepl_setup_conn,
    &raw_code_webrepl_accept_conn,
    &raw_code_webrepl_stop,
    &raw_code_webrepl_start,
    &raw_code_webrepl_start_foreground,
};

static const mp_raw_code_t raw_code_webrepl__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 99,
    #endif
    .children = (void *)&children_webrepl__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_webrepl__lt_module_gt_ + 3,
        .opcodes = fun_data_webrepl__lt_module_gt_ + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_webrepl[43] = {
    MP_QSTR_webrepl_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_socket,
    MP_QSTR_uos,
    MP_QSTR_network,
    MP_QSTR_uwebsocket,
    MP_QSTR_websocket_helper,
    MP_QSTR__webrepl,
    MP_QSTR_listen_s,
    MP_QSTR_client_s,
    MP_QSTR_setup_conn,
    MP_QSTR_accept_conn,
    MP_QSTR_stop,
    MP_QSTR_start,
    MP_QSTR_start_foreground,
    MP_QSTR_port,
    MP_QSTR_accept_handler,
    MP_QSTR_setsockopt,
    MP_QSTR_SOL_SOCKET,
    MP_QSTR_SO_REUSEADDR,
    MP_QSTR_getaddrinfo,
    MP_QSTR_0_dot_0_dot_0_dot_0,
    MP_QSTR_bind,
    MP_QSTR_listen,
    MP_QSTR_AP_IF,
    MP_QSTR_STA_IF,
    MP_QSTR_WLAN,
    MP_QSTR_active,
    MP_QSTR_print,
    MP_QSTR_ifconfig,
    MP_QSTR_listen_sock,
    MP_QSTR_accept,
    MP_QSTR_dupterm,
    MP_QSTR_rejected,
    MP_QSTR_close,
    MP_QSTR_server_handshake,
    MP_QSTR_websocket,
    MP_QSTR_setblocking,
    MP_QSTR_hasattr,
    MP_QSTR_dupterm_notify,
    MP_QSTR_password,
    MP_QSTR_webrepl_cfg,
    MP_QSTR_PASS,
};

// constants
static const mp_obj_str_t const_obj_webrepl_0 = {{&mp_type_str}, 6721, 36, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x64\x61\x65\x6d\x6f\x6e\x20\x73\x74\x61\x72\x74\x65\x64\x20\x6f\x6e\x20\x77\x73\x3a\x2f\x2f\x25\x73\x3a\x25\x64"};
static const mp_obj_str_t const_obj_webrepl_1 = {{&mp_type_str}, 16607, 35, (const byte*)"\x0a\x43\x6f\x6e\x63\x75\x72\x72\x65\x6e\x74\x20\x57\x65\x62\x52\x45\x50\x4c\x20\x63\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x20\x66\x72\x6f\x6d"};
static const mp_obj_str_t const_obj_webrepl_2 = {{&mp_type_str}, 13454, 25, (const byte*)"\x0a\x57\x65\x62\x52\x45\x50\x4c\x20\x63\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x20\x66\x72\x6f\x6d\x3a"};
static const mp_obj_str_t const_obj_webrepl_4 = {{&mp_type_str}, 16616, 30, (const byte*)"\x53\x74\x61\x72\x74\x65\x64\x20\x77\x65\x62\x72\x65\x70\x6c\x20\x69\x6e\x20\x6e\x6f\x72\x6d\x61\x6c\x20\x6d\x6f\x64\x65"};
static const mp_obj_str_t const_obj_webrepl_5 = {{&mp_type_str}, 39750, 53, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x69\x73\x20\x6e\x6f\x74\x20\x63\x6f\x6e\x66\x69\x67\x75\x72\x65\x64\x2c\x20\x72\x75\x6e\x20\x27\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x5f\x73\x65\x74\x75\x70\x27"};
static const mp_obj_str_t const_obj_webrepl_6 = {{&mp_type_str}, 57813, 39, (const byte*)"\x53\x74\x61\x72\x74\x65\x64\x20\x77\x65\x62\x72\x65\x70\x6c\x20\x69\x6e\x20\x6d\x61\x6e\x75\x61\x6c\x20\x6f\x76\x65\x72\x72\x69\x64\x65\x20\x6d\x6f\x64\x65"};

// constant table
static const mp_rom_obj_t const_obj_table_data_webrepl[7] = {
    MP_ROM_PTR(&const_obj_webrepl_0),
    MP_ROM_PTR(&const_obj_webrepl_1),
    MP_ROM_PTR(&const_obj_webrepl_2),
    MP_ROM_QSTR(MP_QSTR_dupterm_notify),
    MP_ROM_PTR(&const_obj_webrepl_4),
    MP_ROM_PTR(&const_obj_webrepl_5),
    MP_ROM_PTR(&const_obj_webrepl_6),
};

static const mp_frozen_module_t frozen_module_webrepl = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_webrepl,
        .obj_table = (mp_obj_t *)&const_obj_table_data_webrepl,
    },
    .rc = &raw_code_webrepl__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module webrepl_setup
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/webrepl_setup.mpy
// - frozen file name: webrepl_setup.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file webrepl_setup.py, scope webrepl_setup__lt_module_gt_
static const byte fun_data_webrepl_setup__lt_module_gt_[83] = {
    0x08,0x28, // prelude
    0x01, // names: <module>
    0x66,0x26,0x46,0x24,0x64,0x84,0x07,0x64,0x20,0x84,0x0c,0x84,0x09,0x84,0x0a,0x84,0x15,0x84,0x22, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'machine'
    0x16,0x04, // STORE_NAME 'machine'
    0x10,0x05, // LOAD_CONST_STRING './boot.py'
    0x16,0x06, // STORE_NAME 'RC'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x07, // STORE_NAME 'CONFIG'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME 'input_choice'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'getpass'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'input_pass'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'exists'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0c, // STORE_NAME 'get_daemon_status'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0d, // STORE_NAME 'change_daemon'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x0e, // STORE_NAME 'main'
    0x11,0x0e, // LOAD_NAME 'main'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_input_choice
static const byte fun_data_webrepl_setup_input_choice[27] = {
    0x22,0x10, // prelude
    0x08,0x0f,0x10, // names: input_choice, prompt, choices
    0x80,0x0b,0x20,0x26,0x25, // code info
    0x12,0x11, // LOAD_GLOBAL 'input'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
    0x42,0x31, // JUMP -15
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_input_choice = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_webrepl_setup_input_choice,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_webrepl_setup_input_choice + 5,
        .opcodes = fun_data_webrepl_setup_input_choice + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_getpass
static const byte fun_data_webrepl_setup_getpass[12] = {
    0x11,0x08, // prelude
    0x09,0x0f, // names: getpass, prompt
    0x80,0x12, // code info
    0x12,0x11, // LOAD_GLOBAL 'input'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_getpass = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_webrepl_setup_getpass,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 12,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_webrepl_setup_getpass + 4,
        .opcodes = fun_data_webrepl_setup_getpass + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_input_pass
static const byte fun_data_webrepl_setup_input_pass[72] = {
    0x18,0x16, // prelude
    0x0a, // names: input_pass
    0x80,0x16,0x20,0x27,0x32,0x27,0x22,0x27,0x25,0x22, // code info
    0x12,0x09, // LOAD_GLOBAL 'getpass'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x12,0x12, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x49, // POP_JUMP_IF_TRUE 9
    0x12,0x12, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x89, // LOAD_CONST_SMALL_INT 9
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x55, // JUMP 21
    0x12,0x09, // LOAD_GLOBAL 'getpass'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x07, // JUMP -57
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_input_pass = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl_setup_input_pass,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 72,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_webrepl_setup_input_pass + 3,
        .opcodes = fun_data_webrepl_setup_input_pass + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_exists
static const byte fun_data_webrepl_setup_exists[42] = {
    0xb1,0x02,0x10, // prelude
    0x0b,0x14, // names: exists, fname
    0x80,0x22,0x22,0x28,0x23,0x4b, // code info
    0x48,0x0f, // SETUP_EXCEPT 15
    0x12,0x15, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x02, // SETUP_WITH 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x4a,0x0c, // POP_EXCEPT_JUMP 12
    0x57, // DUP_TOP
    0x12,0x16, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_exists = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_webrepl_setup_exists,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 42,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_webrepl_setup_exists + 5,
        .opcodes = fun_data_webrepl_setup_exists + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_get_daemon_status
static const byte fun_data_webrepl_setup_get_daemon_status[53] = {
    0x54,0x12, // prelude
    0x0c, // names: get_daemon_status
    0x80,0x2b,0x29,0x25,0x26,0x29,0x22,0x24, // code info
    0x12,0x15, // LOAD_GLOBAL 'open'
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x1e, // SETUP_WITH 30
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x16, // FOR_ITER 22
    0xc1, // STORE_FAST 1
    0x10,0x17, // LOAD_CONST_STRING 'webrepl'
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb1, // LOAD_FAST 1
    0x14,0x18, // LOAD_METHOD 'startswith'
    0x10,0x19, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x42,0x28, // JUMP -24
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_get_daemon_status = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl_setup_get_daemon_status,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 53,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_webrepl_setup_get_daemon_status + 3,
        .opcodes = fun_data_webrepl_setup_get_daemon_status + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_change_daemon
static const byte fun_data_webrepl_setup_change_daemon[161] = {
    0xa9,0x12,0x26, // prelude
    0x0d,0x1a, // names: change_daemon, action
    0x80,0x35,0x23,0x37,0x22,0x25,0x25,0x25,0x22,0x2c,0x2c,0x29,0x29,0x2a,0x23,0x6e,0x29, // code info
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xc1, // STORE_FAST 1
    0x12,0x15, // LOAD_GLOBAL 'open'
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x65, // SETUP_WITH 101
    0xc2, // STORE_FAST 2
    0x12,0x15, // LOAD_GLOBAL 'open'
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x10,0x1b, // LOAD_CONST_STRING '.tmp'
    0xf2, // BINARY_OP 27 __add__
    0x10,0x1c, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x54, // SETUP_WITH 84
    0xc3, // STORE_FAST 3
    0x50, // LOAD_CONST_FALSE
    0xc4, // STORE_FAST 4
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x41, // FOR_ITER 65
    0xc5, // STORE_FAST 5
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x32, // FOR_ITER 50
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xdd, // BINARY_OP 6 <in>
    0x44,0x6a, // POP_JUMP_IF_FALSE 42
    0x52, // LOAD_CONST_TRUE
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb5, // LOAD_FAST 5
    0x14,0x18, // LOAD_METHOD 'startswith'
    0x10,0x19, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb5, // LOAD_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0x42,0x53, // JUMP 19
    0xb0, // LOAD_FAST 0
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0xb5, // LOAD_FAST 5
    0x14,0x18, // LOAD_METHOD 'startswith'
    0x10,0x19, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x10,0x19, // LOAD_CONST_STRING '#'
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0xc5, // STORE_FAST 5
    0x42,0x40, // JUMP 0
    0x42,0x0c, // JUMP -52
    0xb3, // LOAD_FAST 3
    0x14,0x1d, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xbd,0x7f, // JUMP -67
    0xb4, // LOAD_FAST 4
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0xb3, // LOAD_FAST 3
    0x14,0x1d, // LOAD_METHOD 'write'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'remove'
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1f, // LOAD_METHOD 'rename'
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x10,0x1b, // LOAD_CONST_STRING '.tmp'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x06, // LOAD_GLOBAL 'RC'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_change_daemon = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_webrepl_setup_change_daemon,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 161,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 22,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_webrepl_setup_change_daemon + 5,
        .opcodes = fun_data_webrepl_setup_change_daemon + 22,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_main
static const byte fun_data_webrepl_setup_main[223] = {
    0x54,0x32, // prelude
    0x0e, // names: main
    0x80,0x4a,0x45,0x30,0x27,0x27,0x4b,0x27,0x28,0x22,0x69,0x27,0x43,0x26,0x25,0x2b,0x4d,0x39,0x27,0x47,0x49,0x27,0x29,0x26, // code info
    0x12,0x0c, // LOAD_GLOBAL 'get_daemon_status'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb0, // LOAD_FAST 0
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x20, // LOAD_CONST_STRING 'enabled'
    0x42,0x42, // JUMP 2
    0x10,0x21, // LOAD_CONST_STRING 'disabled'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x11, // LOAD_GLOBAL 'input'
    0x10,0x22, // LOAD_CONST_STRING '> '
    0x34,0x01, // CALL_FUNCTION 1
    0x14,0x23, // LOAD_METHOD 'upper'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x24, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xc0,0x80, // POP_JUMP_IF_FALSE 64
    0x12,0x0b, // LOAD_GLOBAL 'exists'
    0x12,0x07, // LOAD_GLOBAL 'CONFIG'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x08, // LOAD_GLOBAL 'input_choice'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0x42,0x4a, // JUMP 10
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x10,0x25, // LOAD_CONST_STRING 'y'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x10,0x25, // LOAD_CONST_STRING 'y'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5d, // POP_JUMP_IF_FALSE 29
    0x12,0x0a, // LOAD_GLOBAL 'input_pass'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0x12,0x15, // LOAD_GLOBAL 'open'
    0x12,0x07, // LOAD_GLOBAL 'CONFIG'
    0x10,0x1c, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0c, // SETUP_WITH 12
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x1d, // LOAD_METHOD 'write'
    0x10,0x26, // LOAD_CONST_STRING 'PASS = %r\n'
    0xb3, // LOAD_FAST 3
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x43,0x52, // POP_JUMP_IF_TRUE 18
    0xb1, // LOAD_FAST 1
    0x10,0x27, // LOAD_CONST_STRING 'D'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb0, // LOAD_FAST 0
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb1, // LOAD_FAST 1
    0x10,0x24, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0xb0, // LOAD_FAST 0
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x14,0x28, // LOAD_METHOD 'exit'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x0d, // LOAD_GLOBAL 'change_daemon'
    0xb1, // LOAD_FAST 1
    0x10,0x24, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x13, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'input_choice'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x25, // LOAD_CONST_STRING 'y'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x04, // LOAD_GLOBAL 'machine'
    0x14,0x29, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_webrepl_setup_main = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl_setup_main,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 223,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_webrepl_setup_main + 3,
        .opcodes = fun_data_webrepl_setup_main + 27,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_webrepl_setup__lt_module_gt_[] = {
    &raw_code_webrepl_setup_input_choice,
    &raw_code_webrepl_setup_getpass,
    &raw_code_webrepl_setup_input_pass,
    &raw_code_webrepl_setup_exists,
    &raw_code_webrepl_setup_get_daemon_status,
    &raw_code_webrepl_setup_change_daemon,
    &raw_code_webrepl_setup_main,
};

static const mp_raw_code_t raw_code_webrepl_setup__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_webrepl_setup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 83,
    #endif
    .children = (void *)&children_webrepl_setup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 7,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_webrepl_setup__lt_module_gt_ + 3,
        .opcodes = fun_data_webrepl_setup__lt_module_gt_ + 22,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_webrepl_setup[42] = {
    MP_QSTR_webrepl_setup_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_os,
    MP_QSTR_machine,
    MP_QSTR__dot__slash_boot_dot_py,
    MP_QSTR_RC,
    MP_QSTR_CONFIG,
    MP_QSTR_input_choice,
    MP_QSTR_getpass,
    MP_QSTR_input_pass,
    MP_QSTR_exists,
    MP_QSTR_get_daemon_status,
    MP_QSTR_change_daemon,
    MP_QSTR_main,
    MP_QSTR_prompt,
    MP_QSTR_choices,
    MP_QSTR_input,
    MP_QSTR_len,
    MP_QSTR_print,
    MP_QSTR_fname,
    MP_QSTR_open,
    MP_QSTR_OSError,
    MP_QSTR_webrepl,
    MP_QSTR_startswith,
    MP_QSTR__hash_,
    MP_QSTR_action,
    MP_QSTR__dot_tmp,
    MP_QSTR_w,
    MP_QSTR_write,
    MP_QSTR_remove,
    MP_QSTR_rename,
    MP_QSTR_enabled,
    MP_QSTR_disabled,
    MP_QSTR__gt__space_,
    MP_QSTR_upper,
    MP_QSTR_E,
    MP_QSTR_y,
    MP_QSTR_PASS_space__equals__space__percent_r_0x0a_,
    MP_QSTR_D,
    MP_QSTR_exit,
    MP_QSTR_reset,
};

// constants
static const mp_obj_str_t const_obj_webrepl_setup_1 = {{&mp_type_str}, 12822, 26, (const byte*)"\x4e\x65\x77\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x20\x28\x34\x2d\x39\x20\x63\x68\x61\x72\x73\x29\x3a\x20"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_5 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_import_space_webrepl),
    MP_ROM_QSTR(MP_QSTR_webrepl_dot_start_paren_open__paren_close_),
}};
static const mp_obj_str_t const_obj_webrepl_setup_6 = {{&mp_type_str}, 34423, 31, (const byte*)"\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x0a\x77\x65\x62\x72\x65\x70\x6c\x2e\x73\x74\x61\x72\x74\x28\x29\x0a"};
static const mp_obj_str_t const_obj_webrepl_setup_7 = {{&mp_type_str}, 24126, 33, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x64\x61\x65\x6d\x6f\x6e\x20\x61\x75\x74\x6f\x2d\x73\x74\x61\x72\x74\x20\x73\x74\x61\x74\x75\x73\x3a"};
static const mp_obj_str_t const_obj_webrepl_setup_8 = {{&mp_type_str}, 9123, 60, (const byte*)"\x0a\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x28\x45\x29\x6e\x61\x62\x6c\x65\x20\x6f\x72\x20\x28\x44\x29\x69\x73\x61\x62\x6c\x65\x20\x69\x74\x20\x72\x75\x6e\x6e\x69\x6e\x67\x20\x6f\x6e\x20\x62\x6f\x6f\x74\x3f"};
static const mp_obj_str_t const_obj_webrepl_setup_10 = {{&mp_type_str}, 49111, 49, (const byte*)"\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x63\x68\x61\x6e\x67\x65\x20\x57\x65\x62\x52\x45\x50\x4c\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x3f\x20\x28\x79\x2f\x6e\x29\x20"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_11 = {{&mp_type_tuple}, 3, {
    MP_ROM_QSTR(MP_QSTR_y),
    MP_ROM_QSTR(MP_QSTR_n),
    MP_ROM_QSTR(MP_QSTR_),
}};
static const mp_obj_str_t const_obj_webrepl_setup_12 = {{&mp_type_str}, 7023, 47, (const byte*)"\x54\x6f\x20\x65\x6e\x61\x62\x6c\x65\x20\x57\x65\x62\x52\x45\x50\x4c\x2c\x20\x79\x6f\x75\x20\x6d\x75\x73\x74\x20\x73\x65\x74\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x20\x66\x6f\x72\x20\x69\x74"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_13 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_D),
    MP_ROM_QSTR(MP_QSTR_E),
}};
static const mp_obj_str_t const_obj_webrepl_setup_14 = {{&mp_type_str}, 24025, 26, (const byte*)"\x4e\x6f\x20\x66\x75\x72\x74\x68\x65\x72\x20\x61\x63\x74\x69\x6f\x6e\x20\x72\x65\x71\x75\x69\x72\x65\x64"};
static const mp_obj_str_t const_obj_webrepl_setup_15 = {{&mp_type_str}, 15057, 38, (const byte*)"\x43\x68\x61\x6e\x67\x65\x73\x20\x77\x69\x6c\x6c\x20\x62\x65\x20\x61\x63\x74\x69\x76\x61\x74\x65\x64\x20\x61\x66\x74\x65\x72\x20\x72\x65\x62\x6f\x6f\x74"};
static const mp_obj_str_t const_obj_webrepl_setup_16 = {{&mp_type_str}, 7554, 36, (const byte*)"\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x72\x65\x62\x6f\x6f\x74\x20\x6e\x6f\x77\x3f\x20\x28\x79\x2f\x6e\x29\x20"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_17 = {{&mp_type_tuple}, 3, {
    MP_ROM_QSTR(MP_QSTR_y),
    MP_ROM_QSTR(MP_QSTR_n),
    MP_ROM_QSTR(MP_QSTR_),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_webrepl_setup[18] = {
    MP_ROM_QSTR(MP_QSTR__dot__slash_webrepl_cfg_dot_py),
    MP_ROM_PTR(&const_obj_webrepl_setup_1),
    MP_ROM_QSTR(MP_QSTR_Invalid_space_password_space_length),
    MP_ROM_QSTR(MP_QSTR_Confirm_space_password_colon__space_),
    MP_ROM_QSTR(MP_QSTR_Passwords_space_do_space_not_space_match),
    MP_ROM_PTR(&const_obj_webrepl_setup_5),
    MP_ROM_PTR(&const_obj_webrepl_setup_6),
    MP_ROM_PTR(&const_obj_webrepl_setup_7),
    MP_ROM_PTR(&const_obj_webrepl_setup_8),
    MP_ROM_QSTR(MP_QSTR__paren_open_Empty_space_line_space_to_space_quit_paren_close_),
    MP_ROM_PTR(&const_obj_webrepl_setup_10),
    MP_ROM_PTR(&const_obj_webrepl_setup_11),
    MP_ROM_PTR(&const_obj_webrepl_setup_12),
    MP_ROM_PTR(&const_obj_webrepl_setup_13),
    MP_ROM_PTR(&const_obj_webrepl_setup_14),
    MP_ROM_PTR(&const_obj_webrepl_setup_15),
    MP_ROM_PTR(&const_obj_webrepl_setup_16),
    MP_ROM_PTR(&const_obj_webrepl_setup_17),
};

static const mp_frozen_module_t frozen_module_webrepl_setup = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_webrepl_setup,
        .obj_table = (mp_obj_t *)&const_obj_table_data_webrepl_setup,
    },
    .rc = &raw_code_webrepl_setup__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module websocket_helper
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/websocket_helper.mpy
// - frozen file name: websocket_helper.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file websocket_helper.py, scope websocket_helper__lt_module_gt_
static const byte fun_data_websocket_helper__lt_module_gt_[94] = {
    0x2c,0x1a, // prelude
    0x01, // names: <module>
    0x22,0x4f,0x49,0x22,0x49,0x29,0x22,0x49,0x49,0x63,0x84,0x2f, // code info
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'usys'
    0x16,0x03, // STORE_NAME 'sys'
    0x4a,0x10, // POP_EXCEPT_JUMP 16
    0x57, // DUP_TOP
    0x11,0x04, // LOAD_NAME 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'sys'
    0x16,0x03, // STORE_NAME 'sys'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'ubinascii'
    0x16,0x06, // STORE_NAME 'binascii'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'binascii'
    0x16,0x06, // STORE_NAME 'binascii'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'uhashlib'
    0x16,0x08, // STORE_NAME 'hashlib'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'hashlib'
    0x16,0x08, // STORE_NAME 'hashlib'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x09, // STORE_NAME 'DEBUG'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0a, // STORE_NAME 'server_handshake'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0b, // STORE_NAME 'client_handshake'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of websocket_helper__lt_module_gt_
// frozen bytecode for file websocket_helper.py, scope websocket_helper_server_handshake
static const byte fun_data_websocket_helper_server_handshake[213] = {
    0x61,0x40, // prelude
    0x0a,0x0c, // names: server_handshake, sock
    0x80,0x12,0x29,0x66,0x42,0x20,0x26,0x23,0x27,0x26,0x42,0x30,0x24,0x29,0x26,0x44,0x23,0x47,0x24,0x4d,0x28,0x28,0x26,0x2d,0x24,0x48,0x23,0x65,0x60,0x27, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0d, // LOAD_METHOD 'makefile'
    0x10,0x0e, // LOAD_CONST_STRING 'rwb'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x0f, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0x14,0x0f, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x10, // LOAD_GLOBAL 'OSError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x67, // JUMP 39
    0x32,0x00, // MAKE_FUNCTION 0
    0xb2, // LOAD_FAST 2
    0x14,0x11, // LOAD_METHOD 'split'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0x12,0x09, // LOAD_GLOBAL 'DEBUG'
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x12, // LOAD_GLOBAL 'print'
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x2a,0x02, // BUILD_TUPLE 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb5, // LOAD_FAST 5
    0xc3, // STORE_FAST 3
    0x42,0x01, // JUMP -63
    0xb3, // LOAD_FAST 3
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x10, // LOAD_GLOBAL 'OSError'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x09, // LOAD_GLOBAL 'DEBUG'
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x12, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xb3, // LOAD_FAST 3
    0x12,0x13, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'hashlib'
    0x14,0x14, // LOAD_METHOD 'sha1'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x14,0x15, // LOAD_METHOD 'update'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0x14,0x16, // LOAD_METHOD 'digest'
    0x36,0x00, // CALL_METHOD 0
    0xc7, // STORE_FAST 7
    0x12,0x06, // LOAD_GLOBAL 'binascii'
    0x14,0x17, // LOAD_METHOD 'b2a_base64'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x12,0x09, // LOAD_GLOBAL 'DEBUG'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x12, // LOAD_GLOBAL 'print'
    0x10,0x18, // LOAD_CONST_STRING 'respkey:'
    0xb7, // LOAD_FAST 7
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x10,0x1a, // LOAD_CONST_STRING '\r\n\r\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of websocket_helper_server_handshake
// frozen bytecode for file websocket_helper.py, scope websocket_helper__lt_listcomp_gt_
static const byte fun_data_websocket_helper__lt_listcomp_gt_[23] = {
    0x41,0x08, // prelude
    0x1c,0x1d, // names: <listcomp>, *
    0x80,0x1f, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x1e, // LOAD_METHOD 'strip'
    0x36,0x00, // CALL_METHOD 0
    0x2f,0x14, // STORE_COMP 20
    0x42,0x34, // JUMP -12
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_websocket_helper__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_websocket_helper__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 23,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_websocket_helper__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_websocket_helper__lt_listcomp_gt_ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_websocket_helper_server_handshake[] = {
    &raw_code_websocket_helper__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_websocket_helper_server_handshake = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_websocket_helper_server_handshake,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 213,
    #endif
    .children = (void *)&children_websocket_helper_server_handshake,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_websocket_helper_server_handshake + 4,
        .opcodes = fun_data_websocket_helper_server_handshake + 34,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of websocket_helper__lt_module_gt_
// frozen bytecode for file websocket_helper.py, scope websocket_helper_client_handshake
static const byte fun_data_websocket_helper_client_handshake[55] = {
    0x31,0x18, // prelude
    0x0b,0x0c, // names: client_handshake, sock
    0x80,0x41,0x29,0x23,0x85,0x09,0x46,0x20,0x26,0x26, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0d, // LOAD_METHOD 'makefile'
    0x10,0x0e, // LOAD_CONST_STRING 'rwb'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x0f, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x14,0x0f, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x42, // JUMP 2
    0x42,0x30, // JUMP -16
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_websocket_helper_client_handshake = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_websocket_helper_client_handshake,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 55,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_websocket_helper_client_handshake + 4,
        .opcodes = fun_data_websocket_helper_client_handshake + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_websocket_helper__lt_module_gt_[] = {
    &raw_code_websocket_helper_server_handshake,
    &raw_code_websocket_helper_client_handshake,
};

static const mp_raw_code_t raw_code_websocket_helper__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_websocket_helper__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 94,
    #endif
    .children = (void *)&children_websocket_helper__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_websocket_helper__lt_module_gt_ + 3,
        .opcodes = fun_data_websocket_helper__lt_module_gt_ + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_websocket_helper[31] = {
    MP_QSTR_websocket_helper_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_usys,
    MP_QSTR_sys,
    MP_QSTR_ImportError,
    MP_QSTR_ubinascii,
    MP_QSTR_binascii,
    MP_QSTR_uhashlib,
    MP_QSTR_hashlib,
    MP_QSTR_DEBUG,
    MP_QSTR_server_handshake,
    MP_QSTR_client_handshake,
    MP_QSTR_sock,
    MP_QSTR_makefile,
    MP_QSTR_rwb,
    MP_QSTR_readline,
    MP_QSTR_OSError,
    MP_QSTR_split,
    MP_QSTR_print,
    MP_QSTR_len,
    MP_QSTR_sha1,
    MP_QSTR_update,
    MP_QSTR_digest,
    MP_QSTR_b2a_base64,
    MP_QSTR_respkey_colon_,
    MP_QSTR_send,
    MP_QSTR__0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_write,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR__star_,
    MP_QSTR_strip,
};

// constants
static const mp_obj_str_t const_obj_websocket_helper_1 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};
static const mp_obj_str_t const_obj_websocket_helper_2 = {{&mp_type_bytes}, 46495, 1, (const byte*)"\x3a"};
static const mp_obj_str_t const_obj_websocket_helper_3 = {{&mp_type_bytes}, 24914, 17, (const byte*)"\x53\x65\x63\x2d\x57\x65\x62\x53\x6f\x63\x6b\x65\x74\x2d\x4b\x65\x79"};
static const mp_obj_str_t const_obj_websocket_helper_6 = {{&mp_type_bytes}, 43888, 36, (const byte*)"\x32\x35\x38\x45\x41\x46\x41\x35\x2d\x45\x39\x31\x34\x2d\x34\x37\x44\x41\x2d\x39\x35\x43\x41\x2d\x43\x35\x41\x42\x30\x44\x43\x38\x35\x42\x31\x31"};
static const mp_obj_str_t const_obj_websocket_helper_7 = {{&mp_type_bytes}, 65413, 97, (const byte*)"\x48\x54\x54\x50\x2f\x31\x2e\x31\x20\x31\x30\x31\x20\x53\x77\x69\x74\x63\x68\x69\x6e\x67\x20\x50\x72\x6f\x74\x6f\x63\x6f\x6c\x73\x0d\x0a\x55\x70\x67\x72\x61\x64\x65\x3a\x20\x77\x65\x62\x73\x6f\x63\x6b\x65\x74\x0d\x0a\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x3a\x20\x55\x70\x67\x72\x61\x64\x65\x0d\x0a\x53\x65\x63\x2d\x57\x65\x62\x53\x6f\x63\x6b\x65\x74\x2d\x41\x63\x63\x65\x70\x74\x3a\x20"};
static const mp_obj_str_t const_obj_websocket_helper_8 = {{&mp_type_bytes}, 52281, 109, (const byte*)"\x47\x45\x54\x20\x2f\x20\x48\x54\x54\x50\x2f\x31\x2e\x31\x0d\x0a\x48\x6f\x73\x74\x3a\x20\x65\x63\x68\x6f\x2e\x77\x65\x62\x73\x6f\x63\x6b\x65\x74\x2e\x6f\x72\x67\x0d\x0a\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x3a\x20\x55\x70\x67\x72\x61\x64\x65\x0d\x0a\x55\x70\x67\x72\x61\x64\x65\x3a\x20\x77\x65\x62\x73\x6f\x63\x6b\x65\x74\x0d\x0a\x53\x65\x63\x2d\x57\x65\x62\x53\x6f\x63\x6b\x65\x74\x2d\x4b\x65\x79\x3a\x20\x66\x6f\x6f\x0d\x0a\x0d\x0a"};
static const mp_obj_str_t const_obj_websocket_helper_9 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};

// constant table
static const mp_rom_obj_t const_obj_table_data_websocket_helper[10] = {
    MP_ROM_QSTR(MP_QSTR_EOF_space_in_space_headers),
    MP_ROM_PTR(&const_obj_websocket_helper_1),
    MP_ROM_PTR(&const_obj_websocket_helper_2),
    MP_ROM_PTR(&const_obj_websocket_helper_3),
    MP_ROM_QSTR(MP_QSTR_Not_space_a_space_websocket_space_request),
    MP_ROM_QSTR(MP_QSTR_Sec_hyphen_WebSocket_hyphen_Key_colon_),
    MP_ROM_PTR(&const_obj_websocket_helper_6),
    MP_ROM_PTR(&const_obj_websocket_helper_7),
    MP_ROM_PTR(&const_obj_websocket_helper_8),
    MP_ROM_PTR(&const_obj_websocket_helper_9),
};

static const mp_frozen_module_t frozen_module_websocket_helper = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_websocket_helper,
        .obj_table = (mp_obj_t *)&const_obj_table_data_websocket_helper,
    },
    .rc = &raw_code_websocket_helper__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module neopixel
// - original source file: /Users/bwhitman/outside/micropython/ports/esp32/build-GENERIC_C3/frozen_mpy/neopixel.mpy
// - frozen file name: neopixel.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file neopixel.py, scope neopixel__lt_module_gt_
static const byte fun_data_neopixel__lt_module_gt_[26] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'bitstream'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'machine'
    0x1c,0x02, // IMPORT_FROM 'bitstream'
    0x16,0x02, // STORE_NAME 'bitstream'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'NeoPixel'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'NeoPixel'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of neopixel__lt_module_gt_
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel
static const byte fun_data_neopixel_NeoPixel[46] = {
    0x08,0x02, // prelude
    0x04, // names: NeoPixel
     // code info
    0x11,0x05, // LOAD_NAME '__name__'
    0x16,0x06, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'NeoPixel'
    0x16,0x07, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x08, // STORE_NAME 'ORDER'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x09, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0a, // STORE_NAME '__len__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0b, // STORE_NAME '__setitem__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0c, // STORE_NAME '__getitem__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0d, // STORE_NAME 'fill'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0e, // STORE_NAME 'write'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel___init__
static const byte fun_data_neopixel___init__[69] = {
    0xb9,0x84,0x01,0x0c, // prelude
    0x09,0x0f,0x10,0x11,0x12,0x13, // names: __init__, self, pin, n, bpp, timing
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'pin'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'n'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x12, // STORE_ATTR 'bpp'
    0x12,0x14, // LOAD_GLOBAL 'bytearray'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'buf'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'pin'
    0x14,0x16, // LOAD_METHOD 'init'
    0xb1, // LOAD_FAST 1
    0x13,0x17, // LOAD_ATTR 'OUT'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x18, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0x12,0x19, // LOAD_GLOBAL 'int'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb4, // LOAD_FAST 4
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x42,0x42, // JUMP 2
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x42,0x41, // JUMP 1
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x13, // STORE_ATTR 'timing'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 5,
    .fun_data = fun_data_neopixel___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 69,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_neopixel___init__ + 10,
        .opcodes = fun_data_neopixel___init__ + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel___len__
static const byte fun_data_neopixel___len__[8] = {
    0x09,0x04, // prelude
    0x0a,0x0f, // names: __len__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'n'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel___len__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_neopixel___len__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 8,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_neopixel___len__ + 4,
        .opcodes = fun_data_neopixel___len__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel___setitem__
static const byte fun_data_neopixel___setitem__[45] = {
    0x53,0x08, // prelude
    0x0b,0x0f,0x1a,0x1b, // names: __setitem__, self, i, v
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'bpp'
    0xf4, // BINARY_OP 29 __mul__
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'bpp'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x52, // JUMP 18
    0x57, // DUP_TOP
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'buf'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'ORDER'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x29, // POP_JUMP_IF_TRUE -23
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel___setitem__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_neopixel___setitem__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 45,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_neopixel___setitem__ + 6,
        .opcodes = fun_data_neopixel___setitem__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel___getitem__
static const byte fun_data_neopixel___getitem__[37] = {
    0x32,0x86,0x01, // prelude
    0x0c,0x0f,0x1a, // names: __getitem__, self, i
    0x00,0x02, // code info
    0xb1, // LOAD_FAST 1
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x12, // LOAD_ATTR 'bpp'
    0xf4, // BINARY_OP 29 __mul__
    0x27,0x02, // STORE_DEREF 2
    0x12,0x1c, // LOAD_GLOBAL 'tuple'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x20,0x00,0x02, // MAKE_CLOSURE 0
    0x12,0x1d, // LOAD_GLOBAL 'range'
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x12, // LOAD_ATTR 'bpp'
    0x34,0x01, // CALL_FUNCTION 1
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of neopixel___getitem__
// frozen bytecode for file neopixel.py, scope neopixel__lt_genexpr_gt_
static const byte fun_data_neopixel__lt_genexpr_gt_[34] = {
    0xdb,0x40,0x08, // prelude
    0x1f,0x20,0x20,0x20, // names: <genexpr>, *, *, *
     // code info
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x13, // FOR_ITER 19
    0xc3, // STORE_FAST 3
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x15, // LOAD_ATTR 'buf'
    0x25,0x01, // LOAD_DEREF 1
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x08, // LOAD_ATTR 'ORDER'
    0xb3, // LOAD_FAST 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x55, // LOAD_SUBSCR
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x2b, // JUMP -21
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel__lt_genexpr_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 3,
    .fun_data = fun_data_neopixel__lt_genexpr_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 34,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_neopixel__lt_genexpr_gt_ + 7,
        .opcodes = fun_data_neopixel__lt_genexpr_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_neopixel___getitem__[] = {
    &raw_code_neopixel__lt_genexpr_gt_,
};

static const mp_raw_code_t raw_code_neopixel___getitem__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_neopixel___getitem__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = (void *)&children_neopixel___getitem__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_neopixel___getitem__ + 6,
        .opcodes = fun_data_neopixel___getitem__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_fill
static const byte fun_data_neopixel_fill[63] = {
    0x62,0x06, // prelude
    0x0d,0x0f,0x1b, // names: fill, self, v
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'buf'
    0xc2, // STORE_FAST 2
    0x12,0x1e, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'buf'
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'bpp'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5d, // JUMP 29
    0x57, // DUP_TOP
    0xc5, // STORE_FAST 5
    0xb1, // LOAD_FAST 1
    0xb5, // LOAD_FAST 5
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'ORDER'
    0xb5, // LOAD_FAST 5
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x42,0x48, // JUMP 8
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0xb7, // LOAD_FAST 7
    0x56, // STORE_SUBSCR
    0xb7, // LOAD_FAST 7
    0xb4, // LOAD_FAST 4
    0xe5, // BINARY_OP 14 __iadd__
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0xb3, // LOAD_FAST 3
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x33, // POP_JUMP_IF_TRUE -13
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1e, // POP_JUMP_IF_TRUE -34
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel_fill = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_neopixel_fill,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 63,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_neopixel_fill + 5,
        .opcodes = fun_data_neopixel_fill + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_write
static const byte fun_data_neopixel_write[21] = {
    0x29,0x04, // prelude
    0x0e,0x0f, // names: write, self
     // code info
    0x12,0x02, // LOAD_GLOBAL 'bitstream'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'pin'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'timing'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'buf'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_neopixel_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_neopixel_write,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 21,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_neopixel_write + 4,
        .opcodes = fun_data_neopixel_write + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_neopixel_NeoPixel[] = {
    &raw_code_neopixel___init__,
    &raw_code_neopixel___len__,
    &raw_code_neopixel___setitem__,
    &raw_code_neopixel___getitem__,
    &raw_code_neopixel_fill,
    &raw_code_neopixel_write,
};

static const mp_raw_code_t raw_code_neopixel_NeoPixel = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_neopixel_NeoPixel,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 46,
    #endif
    .children = (void *)&children_neopixel_NeoPixel,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 6,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_neopixel_NeoPixel + 3,
        .opcodes = fun_data_neopixel_NeoPixel + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_neopixel__lt_module_gt_[] = {
    &raw_code_neopixel_NeoPixel,
};

static const mp_raw_code_t raw_code_neopixel__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_neopixel__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 26,
    #endif
    .children = (void *)&children_neopixel__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_neopixel__lt_module_gt_ + 3,
        .opcodes = fun_data_neopixel__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_neopixel[34] = {
    MP_QSTR_neopixel_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_bitstream,
    MP_QSTR_machine,
    MP_QSTR_NeoPixel,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_ORDER,
    MP_QSTR___init__,
    MP_QSTR___len__,
    MP_QSTR___setitem__,
    MP_QSTR___getitem__,
    MP_QSTR_fill,
    MP_QSTR_write,
    MP_QSTR_self,
    MP_QSTR_pin,
    MP_QSTR_n,
    MP_QSTR_bpp,
    MP_QSTR_timing,
    MP_QSTR_bytearray,
    MP_QSTR_buf,
    MP_QSTR_init,
    MP_QSTR_OUT,
    MP_QSTR_isinstance,
    MP_QSTR_int,
    MP_QSTR_i,
    MP_QSTR_v,
    MP_QSTR_tuple,
    MP_QSTR_range,
    MP_QSTR_len,
    MP_QSTR__lt_genexpr_gt_,
    MP_QSTR__star_,
    MP_QSTR_offset,
};

// constants
static const mp_rom_obj_tuple_t const_obj_neopixel_0 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(1),
    MP_ROM_INT(0),
    MP_ROM_INT(2),
    MP_ROM_INT(3),
}};
static const mp_rom_obj_tuple_t const_obj_neopixel_1 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(400),
    MP_ROM_INT(850),
    MP_ROM_INT(800),
    MP_ROM_INT(450),
}};
static const mp_rom_obj_tuple_t const_obj_neopixel_2 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(800),
    MP_ROM_INT(1700),
    MP_ROM_INT(1600),
    MP_ROM_INT(900),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_neopixel[3] = {
    MP_ROM_PTR(&const_obj_neopixel_0),
    MP_ROM_PTR(&const_obj_neopixel_1),
    MP_ROM_PTR(&const_obj_neopixel_2),
};

static const mp_frozen_module_t frozen_module_neopixel = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_neopixel,
        .obj_table = (mp_obj_t *)&const_obj_table_data_neopixel,
    },
    .rc = &raw_code_neopixel__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// collection of all frozen modules

const char mp_frozen_names[] = {
    #ifdef MP_FROZEN_STR_NAMES
    MP_FROZEN_STR_NAMES
    #endif
    "apa106.py\0"
    "flashbdev.py\0"
    "inisetup.py\0"
    "_boot.py\0"
    "upip.py\0"
    "upip_utarfile.py\0"
    "ntptime.py\0"
    "dht.py\0"
    "onewire.py\0"
    "ds18x20.py\0"
    "uasyncio/__init__.py\0"
    "uasyncio/core.py\0"
    "uasyncio/event.py\0"
    "uasyncio/funcs.py\0"
    "uasyncio/lock.py\0"
    "uasyncio/stream.py\0"
    "webrepl.py\0"
    "webrepl_setup.py\0"
    "websocket_helper.py\0"
    "neopixel.py\0"
    "\0"
};

const mp_frozen_module_t *const mp_frozen_mpy_content[] = {
    &frozen_module_apa106,
    &frozen_module_flashbdev,
    &frozen_module_inisetup,
    &frozen_module__boot,
    &frozen_module_upip,
    &frozen_module_upip_utarfile,
    &frozen_module_ntptime,
    &frozen_module_dht,
    &frozen_module_onewire,
    &frozen_module_ds18x20,
    &frozen_module_uasyncio___init__,
    &frozen_module_uasyncio_core,
    &frozen_module_uasyncio_event,
    &frozen_module_uasyncio_funcs,
    &frozen_module_uasyncio_lock,
    &frozen_module_uasyncio_stream,
    &frozen_module_webrepl,
    &frozen_module_webrepl_setup,
    &frozen_module_websocket_helper,
    &frozen_module_neopixel,
};

#ifdef MICROPY_FROZEN_LIST_ITEM
MICROPY_FROZEN_LIST_ITEM("apa106", "apa106.py")
MICROPY_FROZEN_LIST_ITEM("flashbdev", "flashbdev.py")
MICROPY_FROZEN_LIST_ITEM("inisetup", "inisetup.py")
MICROPY_FROZEN_LIST_ITEM("_boot", "_boot.py")
MICROPY_FROZEN_LIST_ITEM("upip", "upip.py")
MICROPY_FROZEN_LIST_ITEM("upip_utarfile", "upip_utarfile.py")
MICROPY_FROZEN_LIST_ITEM("ntptime", "ntptime.py")
MICROPY_FROZEN_LIST_ITEM("dht", "dht.py")
MICROPY_FROZEN_LIST_ITEM("onewire", "onewire.py")
MICROPY_FROZEN_LIST_ITEM("ds18x20", "ds18x20.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio", "uasyncio/__init__.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio/core", "uasyncio/core.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio/event", "uasyncio/event.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio/funcs", "uasyncio/funcs.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio/lock", "uasyncio/lock.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio/stream", "uasyncio/stream.py")
MICROPY_FROZEN_LIST_ITEM("webrepl", "webrepl.py")
MICROPY_FROZEN_LIST_ITEM("webrepl_setup", "webrepl_setup.py")
MICROPY_FROZEN_LIST_ITEM("websocket_helper", "websocket_helper.py")
MICROPY_FROZEN_LIST_ITEM("neopixel", "neopixel.py")
#endif

/*
byte sizes:
qstr content: 306 unique, 3888 bytes
bc content: 10635
const str content: 2067
const int content: 4
const obj content: 676
const table qstr content: 0 entries, 0 bytes
const table ptr content: 93 entries, 372 bytes
raw code content: 192 * 4 = 3072
mp_frozen_mpy_names_content: 280
mp_frozen_mpy_content_size: 80
total: 21074
*/
