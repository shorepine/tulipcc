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
    MP_QSTR_asyncio_slash___init___dot_py = MP_QSTRnumber_of,
    MP_QSTR_core,
    MP_QSTR_funcs,
    MP_QSTR_wait_for,
    MP_QSTR_gather,
    MP_QSTR_event,
    MP_QSTR_Event,
    MP_QSTR_lock,
    MP_QSTR_Lock,
    MP_QSTR_stream,
    MP_QSTR___version__,
    MP_QSTR__attrs,
    MP_QSTR_attr,
    MP_QSTR_wait_for_ms,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_open_connection,
    MP_QSTR_start_server,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR_asyncio_slash_core_dot_py,
    MP_QSTR_select,
    MP_QSTR_task,
    MP_QSTR_TimeoutError,
    MP_QSTR_message,
    MP_QSTR_exception,
    MP_QSTR_future,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_IOQueue,
    MP_QSTR_Loop,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_run_until_complete,
    MP_QSTR_wait_io_event,
    MP_QSTR_call_exception_handler,
    MP_QSTR_run,
    MP_QSTR__stopper,
    MP_QSTR_get_event_loop,
    MP_QSTR_current_task,
    MP_QSTR_new_event_loop,
    MP_QSTR_exc,
    MP_QSTR_poll,
    MP_QSTR_poller,
    MP_QSTR__enqueue,
    MP_QSTR_register,
    MP_QSTR_POLLIN,
    MP_QSTR_POLLOUT,
    MP_QSTR_modify,
    MP_QSTR__dequeue,
    MP_QSTR_unregister,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_ipoll,
    MP_QSTR_run_forever,
    MP_QSTR_set_exception_handler,
    MP_QSTR__exc_handler,
    MP_QSTR_get_exception_handler,
    MP_QSTR_default_exception_handler,
    MP_QSTR_future_colon_,
    MP_QSTR_coro_equals_,
    MP_QSTR_ticks,
    MP_QSTR__exc_context,
    MP_QSTR__stop_task,
    MP_QSTR_t,
    MP_QSTR_sgen,
    MP_QSTR_aw,
    MP_QSTR_main_task,
    MP_QSTR__io_queue,
    MP_QSTR_runq_len,
    MP_QSTR_waitq_len,
    MP_QSTR_s,
    MP_QSTR_idx,
    MP_QSTR_dt,
    MP_QSTR_handler,
    MP_QSTR_loop,
    MP_QSTR_context,
    MP_QSTR_coroutine_space_expected,
    MP_QSTR_asyncio_slash_event_dot_py,
    MP_QSTR_waiting,
    MP_QSTR_is_set,
    MP_QSTR_wait,
    MP_QSTR_req,
    MP_QSTR_asyncio_slash_funcs_dot_py,
    MP_QSTR__Remove,
    MP_QSTR_return_exceptions,
    MP_QSTR__run,
    MP_QSTR_waiter,
    MP_QSTR_timeout,
    MP_QSTR_er,
    MP_QSTR_can_squot_t_space_gather,
    MP_QSTR_asyncio_slash_lock_dot_py,
    MP_QSTR_locked,
    MP_QSTR_release,
    MP_QSTR_acquire,
    MP_QSTR_exc_type,
    MP_QSTR_tb,
    MP_QSTR_Lock_space_not_space_acquired,
    MP_QSTR_asyncio_slash_stream_dot_py,
    MP_QSTR_Stream,
    MP_QSTR_Server,
    MP_QSTR_wait_closed,
    MP_QSTR_aclose,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR__serve,
    MP_QSTR_stream_awrite,
    MP_QSTR_drain,
    MP_QSTR_out_buf,
    MP_QSTR_get_extra_info,
    MP_QSTR_readexactly,
    MP_QSTR_peername,
    MP_QSTR_host,
    MP_QSTR_port,
    MP_QSTR_cb,
    MP_QSTR_backlog,
    MP_QSTR_buf,
    MP_QSTR_sz,
    MP_QSTR_v,
    MP_QSTR_n,
    MP_QSTR_uasyncio_dot_py,
    MP_QSTR_asyncio,
    MP_QSTR_world_dot_py,
    MP_QSTR_ubinascii,
    MP_QSTR_urequests,
    MP_QSTR_tulip,
    MP_QSTR_UUID,
    MP_QSTR_uuid4,
    MP_QSTR_nice_time,
    MP_QSTR__percent_ds,
    MP_QSTR__percent_dm,
    MP_QSTR__percent_dh,
    MP_QSTR__percent_dd,
    MP_QSTR__tilde_,
    MP_QSTR__space_ago,
    MP_QSTR__percent__space_8s,
    MP_QSTR_matrix_put,
    MP_QSTR_headers,
    MP_QSTR_Bearer_space__percent_s,
    MP_QSTR_matrix_get,
    MP_QSTR_matrix_post,
    MP_QSTR_post,
    MP_QSTR__isdir,
    MP_QSTR_read_in_chunks,
    MP_QSTR_upload,
    MP_QSTR_Packing_space__percent_s,
    MP_QSTR_tar_create,
    MP_QSTR__dot_tar,
    MP_QSTR_content_type,
    MP_QSTR_mimetype,
    MP_QSTR_info,
    MP_QSTR_m_dot_file,
    MP_QSTR_msgtype,
    MP_QSTR_body,
    MP_QSTR_url,
    MP_QSTR_files,
    MP_QSTR_messages,
    MP_QSTR_chunk,
    MP_QSTR_content,
    MP_QSTR_age,
    MP_QSTR_age_ms,
    MP_QSTR_filename,
    MP_QSTR_ls,
    MP_QSTR__lt__space__percent_s_space__gt_,
    MP_QSTR__0x09__percent__space_40s_space__percent_s,
    MP_QSTR_download,
    MP_QSTR__dot_,
    MP_QSTR_limit,
    MP_QSTR_save,
    MP_QSTR_chunk_size,
    MP_QSTR_tar_extract,
    MP_QSTR_show_progress,
    MP_QSTR_m_dot_text,
    MP_QSTR_check,
    MP_QSTR_age_s,
    MP_QSTR_put_message,
    MP_QSTR__space__space__space__space__space_,
    MP_QSTR__colon__space_,
    MP_QSTR_world_ui,
    MP_QSTR_Colors,
    MP_QSTR_INVERSE,
    MP_QSTR_DEFAULT,
    MP_QSTR_roundrect,
    MP_QSTR_world,
    MP_QSTR_ip,
    MP_QSTR_need_space_wifi_dot_,
    MP_QSTR__bytes,
    MP_QSTR__hyphen_,
    MP_QSTR__lt_UUID_colon__space__percent_s_gt_,
    MP_QSTR_ub,
    MP_QSTR_world_token,
    MP_QSTR_room_id,
    MP_QSTR_files_room_id,
    MP_QSTR_firmware_room_id,
    MP_QSTR_last_message,
    MP_QSTR_file_object,
    MP_QSTR_m,
    MP_QSTR_duraflame_dot_rosaline_dot_org,
    MP_QSTR__bang_rGPkdYQOECXDlTVoGe_colon__percent_s,
    MP_QSTR__bang_MuceoboBAfueEttdFw_colon__percent_s,
    MP_QSTR__bang_eMmMZLncsdKrMOFTMM_colon__percent_s,
    MP_QSTR_application_slash_octet_hyphen_stream,
    MP_QSTR_Authorization,
    MP_QSTR_Content_hyphen_Type,
    MP_QSTR_content_uri,
    MP_QSTR_m_dot_room_dot_message,
    MP_QSTR__space__space__space__space__space__space__space__space__space__space__space__space__space__space__space_,
    MP_QSTR_tulip_dot_py,
    MP_QSTR_cd,
    MP_QSTR_upysh,
    MP_QSTR_alles,
    MP_QSTR_Game,
    MP_QSTR_Sprite,
    MP_QSTR_Player,
    MP_QSTR_Joy,
    MP_QSTR_wb,
    MP_QSTR_User_hyphen_Agent,
    MP_QSTR_screen_size,
    MP_QSTR__0x1b__bracket_open_0_semicolon_30m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_31m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_32m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_33m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_34m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_35m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_36m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_37m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_30m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_31m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_32m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_33m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_34m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_35m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_36m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_37m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_40m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_41m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_42m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_43m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_44m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_45m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_46m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_47m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_40m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_41m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_42m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_43m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_44m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_45m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_46m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_47m,
    MP_QSTR__0x1b__bracket_open_1m,
    MP_QSTR__0x1b__bracket_open_2m,
    MP_QSTR__0x1b__bracket_open_3m,
    MP_QSTR__0x1b__bracket_open_4m,
    MP_QSTR__0x1b__bracket_open_5m,
    MP_QSTR__0x1b__bracket_open_7m,
    MP_QSTR__0x1b__bracket_open_9m,
    MP_QSTR__0x1b__bracket_open_0m,
    MP_QSTR__hyphen_dirty,
    MP_QSTR_free_disk_bytes,
    MP_QSTR_upgrade,
    MP_QSTR_esp32,
    MP_QSTR_Partition,
    MP_QSTR_RUNNING,
    MP_QSTR_get_next_update,
    MP_QSTR_ota,
    MP_QSTR_tulipcc_hyphen_,
    MP_QSTR_Y,
    MP_QSTR_y,
    MP_QSTR_generate,
    MP_QSTR_set_boot,
    MP_QSTR_reset,
    MP_QSTR_joyk,
    MP_QSTR_RIGHT,
    MP_QSTR_LEFT,
    MP_QSTR_UP,
    MP_QSTR_DOWN,
    MP_QSTR_B,
    MP_QSTR_A,
    MP_QSTR_X,
    MP_QSTR_START,
    MP_QSTR_SELECT,
    MP_QSTR_L1,
    MP_QSTR_R1,
    MP_QSTR_import_space__percent_s,
    MP_QSTR__dot__dot_,
    MP_QSTR_url_save,
    MP_QSTR_url_get,
    MP_QSTR_screenshot,
    MP_QSTR_rm,
    MP_QSTR_ascii,
    MP_QSTR_ansi_fg,
    MP_QSTR__0x1b__bracket_open_38_semicolon_5_semicolon__percent_dm,
    MP_QSTR_ansi_bg,
    MP_QSTR__0x1b__bracket_open_48_semicolon_5_semicolon__percent_dm,
    MP_QSTR_color,
    MP_QSTR_rgb,
    MP_QSTR_network,
    MP_QSTR_127_dot_0_dot_0_dot_1,
    MP_QSTR_WLAN,
    MP_QSTR_STA_IF,
    MP_QSTR_isconnected,
    MP_QSTR_ifconfig,
    MP_QSTR_set_time,
    MP_QSTR_ntptime,
    MP_QSTR_settime,
    MP_QSTR_wifi,
    MP_QSTR_active,
    MP_QSTR_utarfile,
    MP_QSTR_TarFile,
    MP_QSTR_w,
    MP_QSTR_extracting,
    MP_QSTR_DIRTYPE,
    MP_QSTR__dot__slash_,
    MP_QSTR_making_colon_,
    MP_QSTR__dot__,
    MP_QSTR_ignoring,
    MP_QSTR_extractfile,
    MP_QSTR_borked_space_on_colon_,
    MP_QSTR_debug,
    MP_QSTR_quit,
    MP_QSTR_mem_pointer,
    MP_QSTR_num_sprites,
    MP_QSTR_sprite_id,
    MP_QSTR_mem_pos,
    MP_QSTR_width,
    MP_QSTR_height,
    MP_QSTR_x,
    MP_QSTR_x_v,
    MP_QSTR_y_v,
    MP_QSTR_clamp,
    MP_QSTR_SCREEN_WIDTH,
    MP_QSTR_SCREEN_HEIGHT,
    MP_QSTR_SPRITES,
    MP_QSTR_SPRITE_RAM_BYTES,
    MP_QSTR_moveto,
    MP_QSTR_move,
    MP_QSTR_joy_move,
    MP_QSTR_BLACK,
    MP_QSTR_RED,
    MP_QSTR_GREEN,
    MP_QSTR_BROWN,
    MP_QSTR_BLUE,
    MP_QSTR_PURPLE,
    MP_QSTR_CYAN,
    MP_QSTR_LIGHT_GRAY,
    MP_QSTR_DARK_GRAY,
    MP_QSTR_LIGHT_RED,
    MP_QSTR_LIGHT_GREEN,
    MP_QSTR_YELLOW,
    MP_QSTR_LIGHT_BLUE,
    MP_QSTR_LIGHT_PURPLE,
    MP_QSTR_LIGHT_CYAN,
    MP_QSTR_LIGHT_WHITE,
    MP_QSTR_BG_BLACK,
    MP_QSTR_BG_RED,
    MP_QSTR_BG_GREEN,
    MP_QSTR_BG_BROWN,
    MP_QSTR_BG_BLUE,
    MP_QSTR_BG_PURPLE,
    MP_QSTR_BG_CYAN,
    MP_QSTR_BG_LIGHT_GRAY,
    MP_QSTR_BG_DARK_GRAY,
    MP_QSTR_BG_LIGHT_RED,
    MP_QSTR_BG_LIGHT_GREEN,
    MP_QSTR_BG_YELLOW,
    MP_QSTR_BG_LIGHT_BLUE,
    MP_QSTR_BG_LIGHT_PURPLE,
    MP_QSTR_BG_LIGHT_CYAN,
    MP_QSTR_BG_LIGHT_WHITE,
    MP_QSTR_BOLD,
    MP_QSTR_FAINT,
    MP_QSTR_ITALIC,
    MP_QSTR_UNDERLINE,
    MP_QSTR_FLASH,
    MP_QSTR_BLINK,
    MP_QSTR_REVERSE,
    MP_QSTR_NEGATIVE,
    MP_QSTR_STRIKE,
    MP_QSTR_CROSSED,
    MP_QSTR_RESET,
    MP_QSTR_END,
    MP_QSTR_SORTED_HUE,
    MP_QSTR_pal_idx,
    MP_QSTR_g,
    MP_QSTR_b,
    MP_QSTR_px0,
    MP_QSTR_ssid,
    MP_QSTR_passwd,
    MP_QSTR_wait_timeout,
    MP_QSTR_directory,
    MP_QSTR_file_name,
    MP_QSTR_copy_of,
    MP_QSTR_speed,
    MP_QSTR_TulipCC_slash_4_dot_0,
    MP_QSTR_You_space_have_space_version_colon__space__percent_s,
    MP_QSTR_Flashing_space__percent_s_dot__dot__dot__space__bracket_open__percent_s_bracket_close_,
    MP_QSTR_Rebooting_dot__dot__dot_,
    MP_QSTR_Error_space_running_space__percent_s_colon_,
    MP_QSTR_del_space_sys_dot_modules_bracket_open__quot__percent_s_quot__bracket_close_,
    MP_QSTR__temp_ss_dot_png,
    MP_QSTR_multipart_slash_form_hyphen_data_semicolon_,
    MP_QSTR_authorization,
    MP_QSTR_content_hyphen_type,
    MP_QSTR_Need_space_wi_hyphen_fi_space_on,
    MP_QSTR_Need_space_to_space_be_space_on_space_wifi,
    MP_QSTR_Warning_colon__space_directory,
    MP_QSTR_already_space_exists,
    MP_QSTR_extracting_colon_,
    MP_QSTR_No_space_more_space_sprite_space_handles_dot_,
    MP_QSTR_upysh_dot_py,
    MP_QSTR_LS,
    MP_QSTR_PWD,
    MP_QSTR_CLEAR,
    MP_QSTR_Man,
    MP_QSTR_head,
    MP_QSTR_cat,
    MP_QSTR_cp,
    MP_QSTR_newfile,
    MP_QSTR__percent__space_9d_space__percent_s,
    MP_QSTR__0x1b__bracket_open_2J_0x1b__bracket_open_H,
    MP_QSTR_man,
    MP_QSTR_pwd,
    MP_QSTR_mv,
    MP_QSTR_f,
    MP_QSTR_d,
    MP_QSTR_recursive,
    MP_QSTR_rm_space_of_space__squot__percent_s_squot__space_failed,
    MP_QSTR__space__space__space__space__lt_dir_gt__space__percent_s,
    MP_QSTR__space__space__space__space__space__space__space__space__space__space__percent_s,
    MP_QSTR_sequencer_dot_py,
    MP_QSTR_Timer,
    MP_QSTR_handleInterrupt,
    MP_QSTR_set_bpm,
    MP_QSTR_period,
    MP_QSTR_init,
    MP_QSTR_PERIODIC,
    MP_QSTR_callback,
    MP_QSTR_ppq,
    MP_QSTR_bpm,
    MP_QSTR_swing,
    MP_QSTR_tick,
    MP_QSTR_beat,
    MP_QSTR_playing,
    MP_QSTR_seq_id,
    MP_QSTR_loop_length,
    MP_QSTR_sequence,
    MP_QSTR_timer,
    MP_QSTR_in_bpm,
    MP_QSTR_us_per_tick,
    MP_QSTR_in_beat,
    MP_QSTR_in_tick,
    MP_QSTR_method,
    MP_QSTR_utarfile_dot_py,
    MP_QSTR_uid,
    MP_QSTR_gid,
    MP_QSTR_size,
    MP_QSTR_chksum,
    MP_QSTR_typeflag,
    MP_QSTR_FileSection,
    MP_QSTR_TarInfo,
    MP_QSTR_roundup,
    MP_QSTR__setstring,
    MP_QSTR__isreg,
    MP_QSTR_content_len,
    MP_QSTR_align,
    MP_QSTR_skip,
    MP_QSTR_subf,
    MP_QSTR_offset,
    MP_QSTR__0x00_,
    MP_QSTR_addfile,
    MP_QSTR_finfo,
    MP_QSTR__percent_06o_space_,
    MP_QSTR__percent_011o_space_,
    MP_QSTR_5,
    MP_QSTR_0,
    MP_QSTR__percent_06o_0x00_,
    MP_QSTR_TAR_HEADER,
    MP_QSTR_REGTYPE,
    MP_QSTR_NUL,
    MP_QSTR_BLOCKSIZE,
    MP_QSTR_RECORDSIZE,
    MP_QSTR__S_IFMT,
    MP_QSTR__S_IFREG,
    MP_QSTR__S_IFDIR,
    MP_QSTR_val,
    MP_QSTR_maxlen,
    MP_QSTR_aligned_len,
    MP_QSTR_fileobj,
    MP_QSTR_tarinfo,
    MP_QSTR_TarInfo_paren_open__percent_r_comma__space__percent_s_comma__space__percent_d_paren_close_,
    MP_QSTR_mode_space_must_space_be_space__squot_r_squot__space_or_space__squot_w_squot_,
    MP_QSTR_Cannot_space_stat,
    MP_QSTR__space__hyphen__space_skipping_dot_,
    MP_QSTR_music_dot_py,
    MP_QSTR_Progression,
    MP_QSTR_Key,
    MP_QSTR_Note,
    MP_QSTR_NoteClass,
    MP_QSTR_Chord,
    MP_QSTR_I,
    MP_QSTR_II,
    MP_QSTR_III,
    MP_QSTR_IV,
    MP_QSTR_V,
    MP_QSTR_VI,
    MP_QSTR_VII,
    MP_QSTR_C,
    MP_QSTR_C_hash_,
    MP_QSTR_Db,
    MP_QSTR_D,
    MP_QSTR_D_hash_,
    MP_QSTR_Eb,
    MP_QSTR_E,
    MP_QSTR_E_hash_,
    MP_QSTR_Fb,
    MP_QSTR_F,
    MP_QSTR_F_hash_,
    MP_QSTR_Gb,
    MP_QSTR_G,
    MP_QSTR_G_hash_,
    MP_QSTR_Ab,
    MP_QSTR_A_hash_,
    MP_QSTR_Bb,
    MP_QSTR_Cb,
    MP_QSTR_B_hash_,
    MP_QSTR_1,
    MP_QSTR_2,
    MP_QSTR_maj,
    MP_QSTR_7,
    MP_QSTR_7_paren_open__hash_9_paren_close_,
    MP_QSTR_7_paren_open_b9_paren_close_,
    MP_QSTR_min7,
    MP_QSTR_maj7,
    MP_QSTR_maj13,
    MP_QSTR_maj_slash_5,
    MP_QSTR_add9,
    MP_QSTR_maj_slash_3,
    MP_QSTR_maj_slash_13,
    MP_QSTR_sus4,
    MP_QSTR_maj_paren_open_9_paren_close_,
    MP_QSTR_sus4_paren_open_b7_comma_9_paren_close_,
    MP_QSTR_maj_slash_9,
    MP_QSTR_sus4_paren_open_b7_paren_close_,
    MP_QSTR_min9,
    MP_QSTR_maj_slash_b7,
    MP_QSTR_min_slash_3,
    MP_QSTR_maj6,
    MP_QSTR_maj6_paren_open_b7_paren_close_,
    MP_QSTR_maj_slash_scale,
    MP_QSTR_min_slash_scale,
    MP_QSTR_progression,
    MP_QSTR_chords,
    MP_QSTR_notes,
    MP_QSTR_roman_major,
    MP_QSTR__colon_maj,
    MP_QSTR_roman_minor,
    MP_QSTR__colon_min,
    MP_QSTR_key_string,
    MP_QSTR__slash_scale,
    MP_QSTR_chord,
    MP_QSTR_note_in_key,
    MP_QSTR_octave,
    MP_QSTR_note,
    MP_QSTR_midinote,
    MP_QSTR_frequency,
    MP_QSTR_valid,
    MP_QSTR_names,
    MP_QSTR_unknown,
    MP_QSTR__percent_s,
    MP_QSTR_freq,
    MP_QSTR_chord_string,
    MP_QSTR_annotation,
    MP_QSTR_annotation_name,
    MP_QSTR_none,
    MP_QSTR_root_note,
    MP_QSTR_parse_chord,
    MP_QSTR_N,
    MP_QSTR__amp_pause,
    MP_QSTR__colon_,
    MP_QSTR_annotations_map,
    MP_QSTR_annotations,
    MP_QSTR_frequencies,
    MP_QSTR_midinotes,
    MP_QSTR_basenames,
    MP_QSTR_root,
    MP_QSTR_minor_second,
    MP_QSTR_major_second,
    MP_QSTR_minor_third,
    MP_QSTR_major_third,
    MP_QSTR_perfect_fourth,
    MP_QSTR_diminished_fifth,
    MP_QSTR_augmented_fourth,
    MP_QSTR_perfect_fifth,
    MP_QSTR_minor_sixth,
    MP_QSTR_major_sixth,
    MP_QSTR_minor_seventh,
    MP_QSTR_major_seventh,
    MP_QSTR_perfect_octave,
    MP_QSTR_minor_ninth,
    MP_QSTR_major_ninth,
    MP_QSTR_perfect_fifth_octave,
    MP_QSTR_major_thirteenth,
    MP_QSTR_sus4_paren_open_b7_comma_9_comma_13_paren_close_,
    MP_QSTR__percent_s_space__hyphen__space__percent_s_space__percent_s_space__paren_open_from_space__percent_s_paren_close_,
    MP_QSTR__boot_dot_py,
    MP_QSTR_uos,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_inisetup,
    MP_QSTR_setup,
    MP_QSTR_mark_app_valid_cancel_rollback,
    MP_QSTR_getenv,
    MP_QSTR_HOME,
    MP_QSTR__slash_ex,
    MP_QSTR__slash_ex_slash_g,
    MP_QSTR_unix,
    MP_QSTR__dot_py,
    MP_QSTR__slash_ex_slash_,
    MP_QSTR__dot_png,
    MP_QSTR__slash_ex_slash_g_slash_,
    MP_QSTR_vfs,
    MP_QSTR_currentPartition,
    MP_QSTR_tulipcc,
    MP_QSTR_tulip_home,
    MP_QSTR__slash_Documents_slash_tulipcc,
    MP_QSTR__slash__dot__dot__slash__dot__dot__slash_tulip_home,
    MP_QSTR_alles_dot_py,
    MP_QSTR_preset,
    MP_QSTR_osc,
    MP_QSTR_wave,
    MP_QSTR_bp0,
    MP_QSTR_bp0_target,
    MP_QSTR_filter_freq,
    MP_QSTR_resonance,
    MP_QSTR_filter_type,
    MP_QSTR_vel,
    MP_QSTR_mod_target,
    MP_QSTR_mod_source,
    MP_QSTR_500_comma_0_comma_0_comma_0,
    MP_QSTR_250_comma_0_comma_0_comma_0,
    MP_QSTR_patch,
    MP_QSTR_millis,
    MP_QSTR__percent__dot_6f,
    MP_QSTR_p,
    MP_QSTR_P,
    MP_QSTR_Q,
    MP_QSTR_c,
    MP_QSTR_a,
    MP_QSTR_l,
    MP_QSTR_R,
    MP_QSTR_o,
    MP_QSTR_A_percent_s,
    MP_QSTR_B_percent_s,
    MP_QSTR_C_percent_s,
    MP_QSTR_O_percent_s,
    MP_QSTR_T,
    MP_QSTR_W,
    MP_QSTR_L,
    MP_QSTR_S,
    MP_QSTR_z,
    MP_QSTR_k,
    MP_QSTR_h,
    MP_QSTR_H,
    MP_QSTR_j,
    MP_QSTR_J,
    MP_QSTR_Z,
    MP_QSTR_volume,
    MP_QSTR_client,
    MP_QSTR_latency_ms,
    MP_QSTR_test,
    MP_QSTR_play_patches,
    MP_QSTR_polyphony,
    MP_QSTR_eq_test,
    MP_QSTR_eq_l,
    MP_QSTR_eq_m,
    MP_QSTR_eq_h,
    MP_QSTR_loops,
    MP_QSTR_sweep,
    MP_QSTR_duty,
    MP_QSTR_drums,
    MP_QSTR_pan,
    MP_QSTR_c_major,
    MP_QSTR_chorus,
    MP_QSTR_amp,
    MP_QSTR_chorus_level,
    MP_QSTR_chorus_delay,
    MP_QSTR_reverb,
    MP_QSTR_reverb_level,
    MP_QSTR_reverb_liveness,
    MP_QSTR_reverb_damping,
    MP_QSTR_reverb_xover,
    MP_QSTR_mesh,
    MP_QSTR_local,
    MP_QSTR_BLOCK_SIZE,
    MP_QSTR_SAMPLE_RATE,
    MP_QSTR_OSCS,
    MP_QSTR_MAX_QUEUE,
    MP_QSTR_SINE,
    MP_QSTR_PULSE,
    MP_QSTR_SAW_DOWN,
    MP_QSTR_SAW_UP,
    MP_QSTR_TRIANGLE,
    MP_QSTR_NOISE,
    MP_QSTR_KS,
    MP_QSTR_PCM,
    MP_QSTR_ALGO,
    MP_QSTR_PARTIAL,
    MP_QSTR_PARTIALS,
    MP_QSTR_OFF,
    MP_QSTR_TARGET_AMP,
    MP_QSTR_TARGET_DUTY,
    MP_QSTR_TARGET_FREQ,
    MP_QSTR_TARGET_FILTER_FREQ,
    MP_QSTR_TARGET_RESONANCE,
    MP_QSTR_TARGET_FEEDBACK,
    MP_QSTR_TARGET_LINEAR,
    MP_QSTR_TARGET_TRUE_EXPONENTIAL,
    MP_QSTR_TARGET_DX7_EXPONENTIAL,
    MP_QSTR_TARGET_PAN,
    MP_QSTR_FILTER_NONE,
    MP_QSTR_FILTER_LPF,
    MP_QSTR_FILTER_BPF,
    MP_QSTR_FILTER_HPF,
    MP_QSTR_mesh_flag,
    MP_QSTR_CHORUS_OSC,
    MP_QSTR_send_buffer,
    MP_QSTR_buffer_size,
    MP_QSTR_which,
    MP_QSTR_number,
    MP_QSTR_feedback,
    MP_QSTR_timestamp,
    MP_QSTR_retries,
    MP_QSTR_bp1,
    MP_QSTR_bp2,
    MP_QSTR_bp1_target,
    MP_QSTR_bp2_target,
    MP_QSTR_algorithm,
    MP_QSTR_ratio,
    MP_QSTR_algo_source,
    MP_QSTR_latency,
    MP_QSTR_patch_total,
    MP_QSTR_max_voices,
    MP_QSTR_res,
    MP_QSTR_level,
    MP_QSTR_max_delay,
    MP_QSTR_liveness,
    MP_QSTR_damping,
    MP_QSTR_xover_hz,
    MP_QSTR_local_ip,
    MP_QSTR_10_comma_1_comma_250_comma_0_dot_7_comma_500_comma_0,
    MP_QSTR_100_comma_0_dot_5_comma_25_comma_0,
    MP_QSTR_150_comma_1_comma_250_comma_0_dot_25_comma_250_comma_0,
    MP_QSTR_150_comma_1_comma_400_comma_0_comma_0_comma_0,
    MP_QSTR_25_comma_1_comma_75_comma_0_comma_0_comma_0,
    MP_QSTR_1_comma_1_comma_500_comma_0_comma_0_comma_0,
    MP_QSTR_urequests_dot_py,
    MP_QSTR_usocket,
    MP_QSTR_Response,
    MP_QSTR_request,
    MP_QSTR_Basic_space__brace_open__brace_close_,
    MP_QSTR_http_colon_,
    MP_QSTR_https_colon_,
    MP_QSTR_ussl,
    MP_QSTR_Host,
    MP_QSTR_ujson,
    MP_QSTR_0_0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_GET,
    MP_QSTR_status_code,
    MP_QSTR_reason,
    MP_QSTR_HEAD,
    MP_QSTR_POST,
    MP_QSTR_PUT,
    MP_QSTR_PATCH,
    MP_QSTR_delete,
    MP_QSTR_DELETE,
    MP_QSTR_raw,
    MP_QSTR__cached,
    MP_QSTR_auth,
    MP_QSTR_parse_headers,
    MP_QSTR_Unsupported_space_protocol_colon__space_,
    MP_QSTR_Unsupported_space_,
};

const qstr_hash_t mp_qstr_frozen_const_hashes[] = {
    63415,
    56254,
    27400,
    40746,
    2184,
    27081,
    35561,
    9134,
    59534,
    54361,
    38975,
    24346,
    48310,
    25035,
    9558,
    25560,
    63679,
    39580,
    35638,
    61558,
    16781,
    40488,
    19302,
    48238,
    60370,
    14592,
    12601,
    26002,
    59673,
    24914,
    21139,
    37871,
    6948,
    19688,
    35180,
    12837,
    19971,
    8734,
    3209,
    9435,
    55706,
    42221,
    26816,
    41388,
    24957,
    34164,
    26357,
    49770,
    54295,
    52601,
    8726,
    23891,
    25036,
    5288,
    52355,
    60604,
    45889,
    22842,
    21001,
    2115,
    13092,
    5648,
    46545,
    32154,
    28211,
    31740,
    43474,
    52773,
    21895,
    46550,
    21584,
    28053,
    24029,
    9273,
    55546,
    50473,
    25345,
    31214,
    58562,
    21902,
    35683,
    46624,
    37244,
    47572,
    58803,
    60665,
    21566,
    28082,
    4535,
    16486,
    47631,
    36844,
    54045,
    52220,
    28563,
    43943,
    35185,
    55353,
    2976,
    29411,
    24658,
    49657,
    2796,
    55469,
    59482,
    18805,
    30085,
    16583,
    23753,
    8320,
    13605,
    55388,
    28388,
    43370,
    18804,
    28908,
    46547,
    46539,
    55575,
    44357,
    23776,
    35012,
    62162,
    3409,
    59848,
    8956,
    7054,
    8727,
    8713,
    8716,
    8704,
    46555,
    876,
    19691,
    11440,
    43945,
    62832,
    63351,
    15993,
    55357,
    38303,
    46776,
    48710,
    39338,
    52537,
    32172,
    27883,
    977,
    46059,
    19744,
    20964,
    15701,
    25486,
    52560,
    19037,
    62110,
    55724,
    13030,
    32135,
    21732,
    28314,
    35697,
    53576,
    32145,
    46475,
    47376,
    33700,
    62500,
    44432,
    44740,
    15227,
    44835,
    36714,
    23808,
    55173,
    26975,
    228,
    33931,
    41653,
    55342,
    18887,
    21223,
    28476,
    55984,
    195,
    46472,
    3462,
    28594,
    12131,
    40072,
    42946,
    46118,
    41115,
    39017,
    46536,
    63423,
    53692,
    19185,
    28613,
    36904,
    40382,
    47385,
    62717,
    14396,
    3205,
    3414,
    28386,
    44962,
    4018,
    5355,
    18572,
    27574,
    59353,
    28784,
    59840,
    27219,
    27392,
    27425,
    27586,
    27619,
    27268,
    27301,
    27462,
    27495,
    21313,
    21280,
    21251,
    21474,
    21189,
    21156,
    21127,
    21350,
    26247,
    26470,
    26309,
    26276,
    26371,
    26594,
    26433,
    26400,
    18118,
    18151,
    17924,
    17957,
    18242,
    18275,
    18048,
    18081,
    20505,
    20666,
    20699,
    20604,
    20381,
    20575,
    20753,
    20728,
    22618,
    28093,
    38805,
    52930,
    17319,
    17730,
    23941,
    24415,
    49020,
    46588,
    46556,
    55150,
    50286,
    62480,
    24242,
    31173,
    28062,
    29600,
    18231,
    46567,
    46564,
    46589,
    61413,
    1165,
    29496,
    29926,
    64622,
    26309,
    58032,
    45223,
    59273,
    28890,
    22356,
    37582,
    5079,
    37706,
    61296,
    1752,
    35506,
    10331,
    4622,
    37905,
    7091,
    39296,
    16864,
    7789,
    64314,
    35858,
    30516,
    60009,
    4817,
    64868,
    46546,
    25544,
    5954,
    26308,
    57784,
    26292,
    26744,
    40366,
    35179,
    21972,
    63836,
    47588,
    7414,
    16286,
    43539,
    29987,
    13306,
    46557,
    38164,
    37141,
    36598,
    18448,
    39497,
    26175,
    63243,
    8431,
    49780,
    54071,
    19842,
    1686,
    39134,
    4803,
    15163,
    16395,
    9744,
    40553,
    33259,
    54999,
    32095,
    18753,
    64346,
    54570,
    1777,
    42851,
    52504,
    38284,
    8772,
    35801,
    60769,
    3665,
    4938,
    32563,
    24561,
    5453,
    30021,
    30619,
    19456,
    57840,
    37547,
    27641,
    15360,
    22417,
    60415,
    25027,
    20629,
    11623,
    23941,
    61926,
    59415,
    56378,
    45872,
    42794,
    27929,
    32370,
    46530,
    46535,
    29085,
    19208,
    36551,
    53418,
    40460,
    38235,
    19862,
    3938,
    47860,
    57450,
    60605,
    44938,
    15702,
    32934,
    10430,
    38852,
    7326,
    31385,
    29970,
    12483,
    38172,
    47267,
    56562,
    41039,
    50373,
    29530,
    3398,
    40860,
    50119,
    24045,
    19379,
    28406,
    41535,
    28331,
    21,
    17319,
    28710,
    28350,
    46531,
    46529,
    3711,
    26822,
    49998,
    47859,
    52175,
    8098,
    47944,
    42791,
    41120,
    46111,
    13578,
    61516,
    28884,
    18906,
    31393,
    49712,
    13239,
    53345,
    12624,
    35290,
    27482,
    65410,
    28834,
    64913,
    48591,
    31816,
    22682,
    8918,
    26301,
    15279,
    42272,
    31854,
    62705,
    21770,
    23628,
    14466,
    16909,
    49168,
    30036,
    64424,
    44804,
    12583,
    15432,
    46501,
    36866,
    46125,
    4457,
    48639,
    46480,
    46485,
    4425,
    17794,
    53709,
    54930,
    24361,
    11725,
    41856,
    233,
    15206,
    31486,
    6230,
    17841,
    19460,
    24588,
    62189,
    59430,
    60814,
    26167,
    50339,
    28976,
    60754,
    53685,
    9723,
    38839,
    46572,
    29477,
    55180,
    29498,
    46579,
    29722,
    63251,
    46566,
    29317,
    29283,
    46561,
    29218,
    29058,
    46560,
    29123,
    29217,
    46563,
    29280,
    29248,
    46562,
    29185,
    29190,
    29255,
    29349,
    29380,
    29412,
    46484,
    46487,
    17315,
    46482,
    35369,
    40616,
    39096,
    47156,
    48705,
    48537,
    31485,
    48543,
    28942,
    13060,
    29659,
    3717,
    48533,
    39536,
    39094,
    30649,
    44179,
    47157,
    23489,
    62132,
    16056,
    53264,
    18692,
    2054,
    40286,
    61945,
    35922,
    62197,
    1432,
    31250,
    30359,
    56293,
    7471,
    19925,
    13052,
    52897,
    13779,
    10641,
    9325,
    26099,
    15077,
    4445,
    54466,
    5370,
    19599,
    24012,
    39069,
    46571,
    7441,
    46495,
    38418,
    28017,
    15175,
    37391,
    43460,
    739,
    16541,
    24337,
    13582,
    22018,
    61115,
    30683,
    1256,
    54396,
    18611,
    42687,
    1498,
    29910,
    30115,
    22776,
    56564,
    25289,
    60992,
    20747,
    29375,
    15787,
    26604,
    12400,
    62912,
    46092,
    706,
    58487,
    64430,
    30314,
    15607,
    23199,
    18831,
    14722,
    56312,
    27730,
    44624,
    31366,
    6446,
    39217,
    5825,
    42157,
    3822,
    59093,
    27968,
    24314,
    21280,
    18823,
    15049,
    9402,
    38153,
    40450,
    31610,
    20397,
    16737,
    58380,
    45742,
    40587,
    8539,
    34366,
    46549,
    46581,
    46580,
    46534,
    46532,
    46537,
    46583,
    46538,
    47634,
    53073,
    49808,
    54428,
    46577,
    46578,
    46569,
    46582,
    46559,
    46542,
    46541,
    46573,
    46543,
    46575,
    46591,
    27501,
    10908,
    38764,
    44851,
    62278,
    62287,
    45432,
    38178,
    38179,
    38182,
    43818,
    27089,
    11289,
    5624,
    29434,
    57666,
    18229,
    13113,
    12220,
    34367,
    57937,
    41880,
    47891,
    1590,
    65240,
    52118,
    38984,
    54326,
    32446,
    15945,
    16703,
    42740,
    27258,
    6765,
    64890,
    33515,
    62651,
    29693,
    4059,
    42240,
    43958,
    8741,
    57514,
    41015,
    40119,
    49931,
    61076,
    62983,
    37730,
    65046,
    2645,
    40488,
    59572,
    52432,
    640,
    7054,
    61828,
    19045,
    3125,
    48038,
    54719,
    50232,
    63046,
    45708,
    48235,
    41291,
    18822,
    18821,
    46824,
    58251,
    10,
    4548,
    31394,
    6061,
    64150,
    43019,
    35681,
    19411,
    45499,
    13368,
    7485,
    65470,
    14126,
    44034,
    15556,
    11813,
    59240,
    39661,
    32524,
    36405,
    117,
    5830,
    45172,
    57753,
    3847,
    63796,
    61980,
    64005,
    12520,
    1365,
    48915,
    62819,
    59329,
    32237,
    6461,
    3220,
    55275,
    59292,
    11292,
    35809,
    6226,
    47277,
    44963,
    38204,
    18832,
};

const qstr_len_t mp_qstr_frozen_const_lengths[] = {
    19,
    4,
    5,
    8,
    6,
    5,
    5,
    4,
    4,
    6,
    11,
    6,
    4,
    11,
    14,
    15,
    12,
    12,
    12,
    15,
    6,
    4,
    12,
    7,
    9,
    6,
    18,
    7,
    4,
    16,
    11,
    18,
    13,
    22,
    3,
    8,
    14,
    12,
    14,
    3,
    4,
    6,
    8,
    8,
    6,
    7,
    6,
    8,
    10,
    10,
    11,
    5,
    11,
    21,
    12,
    21,
    25,
    7,
    5,
    5,
    12,
    10,
    1,
    4,
    2,
    9,
    9,
    8,
    9,
    1,
    3,
    2,
    7,
    4,
    7,
    18,
    16,
    7,
    6,
    4,
    3,
    16,
    7,
    17,
    4,
    6,
    7,
    2,
    12,
    15,
    6,
    7,
    7,
    8,
    2,
    17,
    17,
    6,
    6,
    11,
    6,
    6,
    9,
    6,
    13,
    5,
    7,
    14,
    11,
    8,
    4,
    4,
    2,
    7,
    3,
    2,
    1,
    1,
    11,
    7,
    8,
    9,
    9,
    5,
    4,
    5,
    9,
    3,
    3,
    3,
    3,
    1,
    4,
    4,
    10,
    7,
    9,
    10,
    11,
    4,
    6,
    14,
    6,
    10,
    10,
    4,
    12,
    8,
    4,
    6,
    7,
    4,
    3,
    5,
    8,
    5,
    7,
    3,
    6,
    8,
    2,
    6,
    9,
    8,
    1,
    5,
    4,
    10,
    11,
    13,
    6,
    5,
    5,
    11,
    5,
    2,
    8,
    6,
    7,
    7,
    9,
    5,
    2,
    10,
    6,
    1,
    10,
    2,
    11,
    7,
    13,
    16,
    12,
    11,
    1,
    22,
    22,
    22,
    22,
    24,
    13,
    12,
    11,
    14,
    15,
    8,
    2,
    5,
    5,
    4,
    6,
    6,
    3,
    2,
    10,
    11,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    6,
    15,
    7,
    5,
    9,
    7,
    15,
    3,
    8,
    1,
    1,
    8,
    8,
    5,
    4,
    5,
    4,
    2,
    4,
    1,
    1,
    1,
    5,
    6,
    2,
    2,
    9,
    2,
    8,
    7,
    10,
    2,
    5,
    7,
    10,
    7,
    10,
    5,
    3,
    7,
    9,
    4,
    6,
    11,
    8,
    8,
    7,
    7,
    4,
    6,
    8,
    7,
    1,
    10,
    7,
    2,
    7,
    2,
    8,
    11,
    10,
    5,
    4,
    11,
    11,
    9,
    7,
    5,
    6,
    1,
    3,
    3,
    5,
    12,
    13,
    7,
    16,
    6,
    4,
    8,
    5,
    3,
    5,
    5,
    4,
    6,
    4,
    10,
    9,
    9,
    11,
    6,
    10,
    12,
    10,
    11,
    8,
    6,
    8,
    8,
    7,
    9,
    7,
    13,
    12,
    12,
    14,
    9,
    13,
    15,
    13,
    14,
    4,
    5,
    6,
    9,
    5,
    5,
    7,
    8,
    6,
    7,
    5,
    3,
    10,
    7,
    1,
    1,
    3,
    4,
    6,
    12,
    9,
    9,
    7,
    5,
    11,
    20,
    19,
    12,
    17,
    21,
    12,
    20,
    13,
    12,
    13,
    18,
    18,
    14,
    11,
    23,
    8,
    2,
    3,
    5,
    3,
    4,
    3,
    2,
    7,
    7,
    7,
    3,
    3,
    2,
    1,
    1,
    9,
    17,
    12,
    12,
    12,
    5,
    15,
    7,
    6,
    4,
    8,
    8,
    3,
    3,
    5,
    4,
    4,
    7,
    6,
    11,
    8,
    5,
    6,
    11,
    7,
    7,
    6,
    11,
    3,
    3,
    4,
    6,
    8,
    11,
    7,
    7,
    10,
    6,
    11,
    5,
    4,
    4,
    6,
    1,
    7,
    5,
    5,
    6,
    1,
    1,
    5,
    10,
    7,
    3,
    9,
    10,
    7,
    8,
    8,
    3,
    6,
    11,
    7,
    7,
    19,
    23,
    11,
    12,
    8,
    11,
    3,
    4,
    9,
    5,
    1,
    2,
    3,
    2,
    1,
    2,
    3,
    1,
    2,
    2,
    1,
    2,
    2,
    1,
    2,
    2,
    1,
    2,
    2,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1,
    3,
    1,
    5,
    5,
    4,
    4,
    5,
    5,
    4,
    5,
    6,
    4,
    6,
    10,
    5,
    8,
    4,
    6,
    5,
    4,
    8,
    9,
    9,
    11,
    6,
    5,
    11,
    4,
    11,
    4,
    10,
    6,
    5,
    11,
    6,
    4,
    8,
    9,
    5,
    5,
    7,
    2,
    4,
    12,
    10,
    15,
    4,
    9,
    11,
    1,
    6,
    1,
    15,
    11,
    11,
    9,
    9,
    4,
    12,
    12,
    11,
    11,
    14,
    16,
    16,
    13,
    11,
    11,
    13,
    13,
    14,
    11,
    11,
    20,
    16,
    13,
    20,
    8,
    3,
    4,
    9,
    8,
    5,
    30,
    6,
    4,
    3,
    5,
    4,
    3,
    4,
    4,
    6,
    3,
    16,
    7,
    10,
    18,
    17,
    8,
    6,
    3,
    4,
    3,
    10,
    11,
    9,
    11,
    3,
    10,
    10,
    9,
    9,
    5,
    6,
    4,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    3,
    3,
    3,
    3,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    6,
    6,
    10,
    4,
    12,
    9,
    7,
    4,
    4,
    4,
    5,
    5,
    4,
    5,
    3,
    7,
    6,
    3,
    12,
    12,
    6,
    12,
    15,
    14,
    12,
    4,
    5,
    10,
    11,
    4,
    9,
    4,
    5,
    8,
    6,
    8,
    5,
    2,
    3,
    4,
    7,
    8,
    3,
    10,
    11,
    11,
    18,
    16,
    15,
    13,
    23,
    22,
    10,
    11,
    10,
    10,
    10,
    9,
    10,
    11,
    11,
    5,
    6,
    8,
    9,
    7,
    3,
    3,
    10,
    10,
    9,
    5,
    11,
    7,
    11,
    10,
    3,
    5,
    9,
    8,
    7,
    8,
    8,
    18,
    12,
    20,
    15,
    13,
    13,
    12,
    7,
    8,
    7,
    8,
    5,
    6,
    4,
    4,
    5,
    5,
    3,
    11,
    6,
    4,
    4,
    3,
    5,
    6,
    6,
    3,
    7,
    4,
    13,
    22,
    12,
};

extern const qstr_pool_t mp_qstr_const_pool;
const qstr_pool_t mp_qstr_frozen_const_pool = {
    &mp_qstr_const_pool, // previous pool
    MP_QSTRnumber_of, // previous pool size
    10, // allocated entries
    780, // used entries
    (qstr_hash_t *)mp_qstr_frozen_const_hashes,
    (qstr_len_t *)mp_qstr_frozen_const_lengths,
    {
        "asyncio/__init__.py",
        "core",
        "funcs",
        "wait_for",
        "gather",
        "event",
        "Event",
        "lock",
        "Lock",
        "stream",
        "__version__",
        "_attrs",
        "attr",
        "wait_for_ms",
        "ThreadSafeFlag",
        "open_connection",
        "start_server",
        "StreamReader",
        "StreamWriter",
        "asyncio/core.py",
        "select",
        "task",
        "TimeoutError",
        "message",
        "exception",
        "future",
        "SingletonGenerator",
        "IOQueue",
        "Loop",
        "_promote_to_task",
        "create_task",
        "run_until_complete",
        "wait_io_event",
        "call_exception_handler",
        "run",
        "_stopper",
        "get_event_loop",
        "current_task",
        "new_event_loop",
        "exc",
        "poll",
        "poller",
        "_enqueue",
        "register",
        "POLLIN",
        "POLLOUT",
        "modify",
        "_dequeue",
        "unregister",
        "queue_read",
        "queue_write",
        "ipoll",
        "run_forever",
        "set_exception_handler",
        "_exc_handler",
        "get_exception_handler",
        "default_exception_handler",
        "future:",
        "coro=",
        "ticks",
        "_exc_context",
        "_stop_task",
        "t",
        "sgen",
        "aw",
        "main_task",
        "_io_queue",
        "runq_len",
        "waitq_len",
        "s",
        "idx",
        "dt",
        "handler",
        "loop",
        "context",
        "coroutine expected",
        "asyncio/event.py",
        "waiting",
        "is_set",
        "wait",
        "req",
        "asyncio/funcs.py",
        "_Remove",
        "return_exceptions",
        "_run",
        "waiter",
        "timeout",
        "er",
        "can't gather",
        "asyncio/lock.py",
        "locked",
        "release",
        "acquire",
        "exc_type",
        "tb",
        "Lock not acquired",
        "asyncio/stream.py",
        "Stream",
        "Server",
        "wait_closed",
        "aclose",
        "awrite",
        "awritestr",
        "_serve",
        "stream_awrite",
        "drain",
        "out_buf",
        "get_extra_info",
        "readexactly",
        "peername",
        "host",
        "port",
        "cb",
        "backlog",
        "buf",
        "sz",
        "v",
        "n",
        "uasyncio.py",
        "asyncio",
        "world.py",
        "ubinascii",
        "urequests",
        "tulip",
        "UUID",
        "uuid4",
        "nice_time",
        "%ds",
        "%dm",
        "%dh",
        "%dd",
        "~",
        " ago",
        "% 8s",
        "matrix_put",
        "headers",
        "Bearer %s",
        "matrix_get",
        "matrix_post",
        "post",
        "_isdir",
        "read_in_chunks",
        "upload",
        "Packing %s",
        "tar_create",
        ".tar",
        "content_type",
        "mimetype",
        "info",
        "m.file",
        "msgtype",
        "body",
        "url",
        "files",
        "messages",
        "chunk",
        "content",
        "age",
        "age_ms",
        "filename",
        "ls",
        "< %s >",
        "\x09\x25\x20\x34\x30\x73\x20\x25\x73",
        "download",
        ".",
        "limit",
        "save",
        "chunk_size",
        "tar_extract",
        "show_progress",
        "m.text",
        "check",
        "age_s",
        "put_message",
        "     ",
        ": ",
        "world_ui",
        "Colors",
        "INVERSE",
        "DEFAULT",
        "roundrect",
        "world",
        "ip",
        "need wifi.",
        "_bytes",
        "-",
        "<UUID: %s>",
        "ub",
        "world_token",
        "room_id",
        "files_room_id",
        "firmware_room_id",
        "last_message",
        "file_object",
        "m",
        "duraflame.rosaline.org",
        "!rGPkdYQOECXDlTVoGe:%s",
        "!MuceoboBAfueEttdFw:%s",
        "!eMmMZLncsdKrMOFTMM:%s",
        "application/octet-stream",
        "Authorization",
        "Content-Type",
        "content_uri",
        "m.room.message",
        "               ",
        "tulip.py",
        "cd",
        "upysh",
        "alles",
        "Game",
        "Sprite",
        "Player",
        "Joy",
        "wb",
        "User-Agent",
        "screen_size",
        "\x1b\x5b\x30\x3b\x33\x30\x6d",
        "\x1b\x5b\x30\x3b\x33\x31\x6d",
        "\x1b\x5b\x30\x3b\x33\x32\x6d",
        "\x1b\x5b\x30\x3b\x33\x33\x6d",
        "\x1b\x5b\x30\x3b\x33\x34\x6d",
        "\x1b\x5b\x30\x3b\x33\x35\x6d",
        "\x1b\x5b\x30\x3b\x33\x36\x6d",
        "\x1b\x5b\x30\x3b\x33\x37\x6d",
        "\x1b\x5b\x31\x3b\x33\x30\x6d",
        "\x1b\x5b\x31\x3b\x33\x31\x6d",
        "\x1b\x5b\x31\x3b\x33\x32\x6d",
        "\x1b\x5b\x31\x3b\x33\x33\x6d",
        "\x1b\x5b\x31\x3b\x33\x34\x6d",
        "\x1b\x5b\x31\x3b\x33\x35\x6d",
        "\x1b\x5b\x31\x3b\x33\x36\x6d",
        "\x1b\x5b\x31\x3b\x33\x37\x6d",
        "\x1b\x5b\x30\x3b\x34\x30\x6d",
        "\x1b\x5b\x30\x3b\x34\x31\x6d",
        "\x1b\x5b\x30\x3b\x34\x32\x6d",
        "\x1b\x5b\x30\x3b\x34\x33\x6d",
        "\x1b\x5b\x30\x3b\x34\x34\x6d",
        "\x1b\x5b\x30\x3b\x34\x35\x6d",
        "\x1b\x5b\x30\x3b\x34\x36\x6d",
        "\x1b\x5b\x30\x3b\x34\x37\x6d",
        "\x1b\x5b\x31\x3b\x34\x30\x6d",
        "\x1b\x5b\x31\x3b\x34\x31\x6d",
        "\x1b\x5b\x31\x3b\x34\x32\x6d",
        "\x1b\x5b\x31\x3b\x34\x33\x6d",
        "\x1b\x5b\x31\x3b\x34\x34\x6d",
        "\x1b\x5b\x31\x3b\x34\x35\x6d",
        "\x1b\x5b\x31\x3b\x34\x36\x6d",
        "\x1b\x5b\x31\x3b\x34\x37\x6d",
        "\x1b\x5b\x31\x6d",
        "\x1b\x5b\x32\x6d",
        "\x1b\x5b\x33\x6d",
        "\x1b\x5b\x34\x6d",
        "\x1b\x5b\x35\x6d",
        "\x1b\x5b\x37\x6d",
        "\x1b\x5b\x39\x6d",
        "\x1b\x5b\x30\x6d",
        "-dirty",
        "free_disk_bytes",
        "upgrade",
        "esp32",
        "Partition",
        "RUNNING",
        "get_next_update",
        "ota",
        "tulipcc-",
        "Y",
        "y",
        "generate",
        "set_boot",
        "reset",
        "joyk",
        "RIGHT",
        "LEFT",
        "UP",
        "DOWN",
        "B",
        "A",
        "X",
        "START",
        "SELECT",
        "L1",
        "R1",
        "import %s",
        "..",
        "url_save",
        "url_get",
        "screenshot",
        "rm",
        "ascii",
        "ansi_fg",
        "\x1b\x5b\x33\x38\x3b\x35\x3b\x25\x64\x6d",
        "ansi_bg",
        "\x1b\x5b\x34\x38\x3b\x35\x3b\x25\x64\x6d",
        "color",
        "rgb",
        "network",
        "127.0.0.1",
        "WLAN",
        "STA_IF",
        "isconnected",
        "ifconfig",
        "set_time",
        "ntptime",
        "settime",
        "wifi",
        "active",
        "utarfile",
        "TarFile",
        "w",
        "extracting",
        "DIRTYPE",
        "./",
        "making:",
        "._",
        "ignoring",
        "extractfile",
        "borked on:",
        "debug",
        "quit",
        "mem_pointer",
        "num_sprites",
        "sprite_id",
        "mem_pos",
        "width",
        "height",
        "x",
        "x_v",
        "y_v",
        "clamp",
        "SCREEN_WIDTH",
        "SCREEN_HEIGHT",
        "SPRITES",
        "SPRITE_RAM_BYTES",
        "moveto",
        "move",
        "joy_move",
        "BLACK",
        "RED",
        "GREEN",
        "BROWN",
        "BLUE",
        "PURPLE",
        "CYAN",
        "LIGHT_GRAY",
        "DARK_GRAY",
        "LIGHT_RED",
        "LIGHT_GREEN",
        "YELLOW",
        "LIGHT_BLUE",
        "LIGHT_PURPLE",
        "LIGHT_CYAN",
        "LIGHT_WHITE",
        "BG_BLACK",
        "BG_RED",
        "BG_GREEN",
        "BG_BROWN",
        "BG_BLUE",
        "BG_PURPLE",
        "BG_CYAN",
        "BG_LIGHT_GRAY",
        "BG_DARK_GRAY",
        "BG_LIGHT_RED",
        "BG_LIGHT_GREEN",
        "BG_YELLOW",
        "BG_LIGHT_BLUE",
        "BG_LIGHT_PURPLE",
        "BG_LIGHT_CYAN",
        "BG_LIGHT_WHITE",
        "BOLD",
        "FAINT",
        "ITALIC",
        "UNDERLINE",
        "FLASH",
        "BLINK",
        "REVERSE",
        "NEGATIVE",
        "STRIKE",
        "CROSSED",
        "RESET",
        "END",
        "SORTED_HUE",
        "pal_idx",
        "g",
        "b",
        "px0",
        "ssid",
        "passwd",
        "wait_timeout",
        "directory",
        "file_name",
        "copy_of",
        "speed",
        "TulipCC/4.0",
        "You have version: %s",
        "Flashing %s... [%s]",
        "Rebooting...",
        "Error running %s:",
        "\x64\x65\x6c\x20\x73\x79\x73\x2e\x6d\x6f\x64\x75\x6c\x65\x73\x5b\x22\x25\x73\x22\x5d",
        "_temp_ss.png",
        "multipart/form-data;",
        "authorization",
        "content-type",
        "Need wi-fi on",
        "Need to be on wifi",
        "Warning: directory",
        "already exists",
        "extracting:",
        "No more sprite handles.",
        "upysh.py",
        "LS",
        "PWD",
        "CLEAR",
        "Man",
        "head",
        "cat",
        "cp",
        "newfile",
        "% 9d %s",
        "\x1b\x5b\x32\x4a\x1b\x5b\x48",
        "man",
        "pwd",
        "mv",
        "f",
        "d",
        "recursive",
        "rm of '%s' failed",
        "    <dir> %s",
        "          %s",
        "sequencer.py",
        "Timer",
        "handleInterrupt",
        "set_bpm",
        "period",
        "init",
        "PERIODIC",
        "callback",
        "ppq",
        "bpm",
        "swing",
        "tick",
        "beat",
        "playing",
        "seq_id",
        "loop_length",
        "sequence",
        "timer",
        "in_bpm",
        "us_per_tick",
        "in_beat",
        "in_tick",
        "method",
        "utarfile.py",
        "uid",
        "gid",
        "size",
        "chksum",
        "typeflag",
        "FileSection",
        "TarInfo",
        "roundup",
        "_setstring",
        "_isreg",
        "content_len",
        "align",
        "skip",
        "subf",
        "offset",
        "\x00",
        "addfile",
        "finfo",
        "%06o ",
        "%011o ",
        "5",
        "0",
        "\x25\x30\x36\x6f\x00",
        "TAR_HEADER",
        "REGTYPE",
        "NUL",
        "BLOCKSIZE",
        "RECORDSIZE",
        "_S_IFMT",
        "_S_IFREG",
        "_S_IFDIR",
        "val",
        "maxlen",
        "aligned_len",
        "fileobj",
        "tarinfo",
        "TarInfo(%r, %s, %d)",
        "mode must be 'r' or 'w'",
        "Cannot stat",
        " - skipping.",
        "music.py",
        "Progression",
        "Key",
        "Note",
        "NoteClass",
        "Chord",
        "I",
        "II",
        "III",
        "IV",
        "V",
        "VI",
        "VII",
        "C",
        "C#",
        "Db",
        "D",
        "D#",
        "Eb",
        "E",
        "E#",
        "Fb",
        "F",
        "F#",
        "Gb",
        "G",
        "G#",
        "Ab",
        "A#",
        "Bb",
        "Cb",
        "B#",
        "1",
        "2",
        "maj",
        "7",
        "7(#9)",
        "7(b9)",
        "min7",
        "maj7",
        "maj13",
        "maj/5",
        "add9",
        "maj/3",
        "maj/13",
        "sus4",
        "maj(9)",
        "sus4(b7,9)",
        "maj/9",
        "sus4(b7)",
        "min9",
        "maj/b7",
        "min/3",
        "maj6",
        "maj6(b7)",
        "maj/scale",
        "min/scale",
        "progression",
        "chords",
        "notes",
        "roman_major",
        ":maj",
        "roman_minor",
        ":min",
        "key_string",
        "/scale",
        "chord",
        "note_in_key",
        "octave",
        "note",
        "midinote",
        "frequency",
        "valid",
        "names",
        "unknown",
        "%s",
        "freq",
        "chord_string",
        "annotation",
        "annotation_name",
        "none",
        "root_note",
        "parse_chord",
        "N",
        "&pause",
        ":",
        "annotations_map",
        "annotations",
        "frequencies",
        "midinotes",
        "basenames",
        "root",
        "minor_second",
        "major_second",
        "minor_third",
        "major_third",
        "perfect_fourth",
        "diminished_fifth",
        "augmented_fourth",
        "perfect_fifth",
        "minor_sixth",
        "major_sixth",
        "minor_seventh",
        "major_seventh",
        "perfect_octave",
        "minor_ninth",
        "major_ninth",
        "perfect_fifth_octave",
        "major_thirteenth",
        "sus4(b7,9,13)",
        "%s - %s %s (from %s)",
        "_boot.py",
        "uos",
        "bdev",
        "flashbdev",
        "inisetup",
        "setup",
        "mark_app_valid_cancel_rollback",
        "getenv",
        "HOME",
        "/ex",
        "/ex/g",
        "unix",
        ".py",
        "/ex/",
        ".png",
        "/ex/g/",
        "vfs",
        "currentPartition",
        "tulipcc",
        "tulip_home",
        "/Documents/tulipcc",
        "/../../tulip_home",
        "alles.py",
        "preset",
        "osc",
        "wave",
        "bp0",
        "bp0_target",
        "filter_freq",
        "resonance",
        "filter_type",
        "vel",
        "mod_target",
        "mod_source",
        "500,0,0,0",
        "250,0,0,0",
        "patch",
        "millis",
        "%.6f",
        "p",
        "P",
        "Q",
        "c",
        "a",
        "l",
        "R",
        "o",
        "A%s",
        "B%s",
        "C%s",
        "O%s",
        "T",
        "W",
        "L",
        "S",
        "z",
        "k",
        "h",
        "H",
        "j",
        "J",
        "Z",
        "volume",
        "client",
        "latency_ms",
        "test",
        "play_patches",
        "polyphony",
        "eq_test",
        "eq_l",
        "eq_m",
        "eq_h",
        "loops",
        "sweep",
        "duty",
        "drums",
        "pan",
        "c_major",
        "chorus",
        "amp",
        "chorus_level",
        "chorus_delay",
        "reverb",
        "reverb_level",
        "reverb_liveness",
        "reverb_damping",
        "reverb_xover",
        "mesh",
        "local",
        "BLOCK_SIZE",
        "SAMPLE_RATE",
        "OSCS",
        "MAX_QUEUE",
        "SINE",
        "PULSE",
        "SAW_DOWN",
        "SAW_UP",
        "TRIANGLE",
        "NOISE",
        "KS",
        "PCM",
        "ALGO",
        "PARTIAL",
        "PARTIALS",
        "OFF",
        "TARGET_AMP",
        "TARGET_DUTY",
        "TARGET_FREQ",
        "TARGET_FILTER_FREQ",
        "TARGET_RESONANCE",
        "TARGET_FEEDBACK",
        "TARGET_LINEAR",
        "TARGET_TRUE_EXPONENTIAL",
        "TARGET_DX7_EXPONENTIAL",
        "TARGET_PAN",
        "FILTER_NONE",
        "FILTER_LPF",
        "FILTER_BPF",
        "FILTER_HPF",
        "mesh_flag",
        "CHORUS_OSC",
        "send_buffer",
        "buffer_size",
        "which",
        "number",
        "feedback",
        "timestamp",
        "retries",
        "bp1",
        "bp2",
        "bp1_target",
        "bp2_target",
        "algorithm",
        "ratio",
        "algo_source",
        "latency",
        "patch_total",
        "max_voices",
        "res",
        "level",
        "max_delay",
        "liveness",
        "damping",
        "xover_hz",
        "local_ip",
        "10,1,250,0.7,500,0",
        "100,0.5,25,0",
        "150,1,250,0.25,250,0",
        "150,1,400,0,0,0",
        "25,1,75,0,0,0",
        "1,1,500,0,0,0",
        "urequests.py",
        "usocket",
        "Response",
        "request",
        "Basic {}",
        "http:",
        "https:",
        "ussl",
        "Host",
        "ujson",
        "\x30\x0d\x0a\x0d\x0a",
        "GET",
        "status_code",
        "reason",
        "HEAD",
        "POST",
        "PUT",
        "PATCH",
        "delete",
        "DELETE",
        "raw",
        "_cached",
        "auth",
        "parse_headers",
        "Unsupported protocol: ",
        "Unsupported ",
    },
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio___init__
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/__init__.mpy
// - frozen file name: asyncio/__init__.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/__init__.py, scope asyncio___init____lt_module_gt_
static const byte fun_data_asyncio___init____lt_module_gt_[75] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'core'
    0x69, // IMPORT_STAR
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x0e, // STORE_NAME '__version__'
    0x2c,0x0a, // BUILD_MAP 10
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x10,0x05, // LOAD_CONST_STRING 'wait_for'
    0x62, // STORE_MAP
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x62, // STORE_MAP
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x10,0x06, // LOAD_CONST_STRING 'gather'
    0x62, // STORE_MAP
    0x10,0x07, // LOAD_CONST_STRING 'event'
    0x10,0x08, // LOAD_CONST_STRING 'Event'
    0x62, // STORE_MAP
    0x10,0x07, // LOAD_CONST_STRING 'event'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x62, // STORE_MAP
    0x10,0x09, // LOAD_CONST_STRING 'lock'
    0x10,0x0a, // LOAD_CONST_STRING 'Lock'
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0x16,0x0f, // STORE_NAME '_attrs'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0c, // STORE_NAME '__getattr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio___init____lt_module_gt_
// frozen bytecode for file asyncio/__init__.py, scope asyncio___init_____getattr__
static const byte fun_data_asyncio___init_____getattr__[48] = {
    0x49,0x04, // prelude
    0x0c,0x10, // names: __getattr__, attr
     // code info
    0x12,0x0f, // LOAD_GLOBAL '_attrs'
    0x14,0x0d, // LOAD_METHOD 'get'
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
static const mp_raw_code_t raw_code_asyncio___init_____getattr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio___init_____getattr__,
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
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio___init_____getattr__ + 4,
        .line_info_top = fun_data_asyncio___init_____getattr__ + 4,
        .opcodes = fun_data_asyncio___init_____getattr__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio___init____lt_module_gt_[] = {
    &raw_code_asyncio___init_____getattr__,
};

static const mp_raw_code_t raw_code_asyncio___init____lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 75,
    #endif
    .children = (void *)&children_asyncio___init____lt_module_gt_,
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
        .line_info = fun_data_asyncio___init____lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio___init____lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio___init____lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio___init__[21] = {
    MP_QSTR_asyncio_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_core,
    MP_QSTR_funcs,
    MP_QSTR_wait_for,
    MP_QSTR_gather,
    MP_QSTR_event,
    MP_QSTR_Event,
    MP_QSTR_lock,
    MP_QSTR_Lock,
    MP_QSTR_stream,
    MP_QSTR___getattr__,
    MP_QSTR_get,
    MP_QSTR___version__,
    MP_QSTR__attrs,
    MP_QSTR_attr,
    MP_QSTR_AttributeError,
    MP_QSTR_getattr,
    MP_QSTR___import__,
    MP_QSTR_globals,
};

// constants
static const mp_rom_obj_tuple_t const_obj_asyncio___init___0 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(3),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio___init__[7] = {
    MP_ROM_PTR(&const_obj_asyncio___init___0),
    MP_ROM_QSTR(MP_QSTR_wait_for_ms),
    MP_ROM_QSTR(MP_QSTR_ThreadSafeFlag),
    MP_ROM_QSTR(MP_QSTR_open_connection),
    MP_ROM_QSTR(MP_QSTR_start_server),
    MP_ROM_QSTR(MP_QSTR_StreamReader),
    MP_ROM_QSTR(MP_QSTR_StreamWriter),
};

static const mp_frozen_module_t frozen_module_asyncio___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio___init__,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio___init__,
    },
    .rc = &raw_code_asyncio___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_core
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/core.mpy
// - frozen file name: asyncio/core.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/core.py, scope asyncio_core__lt_module_gt_
static const byte fun_data_asyncio_core__lt_module_gt_[215] = {
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
    0x16,0x47, // STORE_NAME 'ticks'
    0x1c,0x03, // IMPORT_FROM 'ticks_diff'
    0x16,0x03, // STORE_NAME 'ticks_diff'
    0x1c,0x04, // IMPORT_FROM 'ticks_add'
    0x16,0x04, // STORE_NAME 'ticks_add'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'sys'
    0x16,0x06, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'select'
    0x16,0x07, // STORE_NAME 'select'
    0x48,0x14, // SETUP_EXCEPT 20
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x08, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x09, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0a, // IMPORT_NAME '_asyncio'
    0x1c,0x08, // IMPORT_FROM 'TaskQueue'
    0x16,0x08, // STORE_NAME 'TaskQueue'
    0x1c,0x09, // IMPORT_FROM 'Task'
    0x16,0x09, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x16, // POP_EXCEPT_JUMP 22
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x08, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x09, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0b, // IMPORT_NAME 'task'
    0x1c,0x08, // IMPORT_FROM 'TaskQueue'
    0x16,0x08, // STORE_NAME 'TaskQueue'
    0x1c,0x09, // IMPORT_FROM 'Task'
    0x16,0x09, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0c, // LOAD_CONST_STRING 'CancelledError'
    0x11,0x48, // LOAD_NAME 'BaseException'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0c, // STORE_NAME 'CancelledError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0d, // LOAD_CONST_STRING 'TimeoutError'
    0x11,0x49, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0d, // STORE_NAME 'TimeoutError'
    0x2c,0x03, // BUILD_MAP 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x0e, // LOAD_CONST_STRING 'message'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x62, // STORE_MAP
    0x16,0x4a, // STORE_NAME '_exc_context'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x11, // LOAD_CONST_STRING 'SingletonGenerator'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x11, // STORE_NAME 'SingletonGenerator'
    0x11,0x11, // LOAD_NAME 'SingletonGenerator'
    0x34,0x00, // CALL_FUNCTION 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x14, // STORE_NAME 'sleep_ms'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x16, // STORE_NAME 'sleep'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x12, // LOAD_CONST_STRING 'IOQueue'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x12, // STORE_NAME 'IOQueue'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x17, // STORE_NAME '_promote_to_task'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x18, // STORE_NAME 'create_task'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x26, // STORE_NAME 'run'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x27, // STORE_NAME '_stopper'
    0x51, // LOAD_CONST_NONE
    0x17,0x4b, // STORE_GLOBAL '_stop_task'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x0b, // MAKE_FUNCTION 11
    0x10,0x13, // LOAD_CONST_STRING 'Loop'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x13, // STORE_NAME 'Loop'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0c, // MAKE_FUNCTION_DEFARGS 12
    0x16,0x28, // STORE_NAME 'get_event_loop'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x29, // STORE_NAME 'current_task'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x2a, // STORE_NAME 'new_event_loop'
    0x11,0x2a, // LOAD_NAME 'new_event_loop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_CancelledError
static const byte fun_data_asyncio_core_CancelledError[13] = {
    0x00,0x02, // prelude
    0x0c, // names: CancelledError
     // code info
    0x11,0x4c, // LOAD_NAME '__name__'
    0x16,0x4d, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'CancelledError'
    0x16,0x4e, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_CancelledError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_CancelledError,
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
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio_core_CancelledError + 3,
        .line_info_top = fun_data_asyncio_core_CancelledError + 3,
        .opcodes = fun_data_asyncio_core_CancelledError + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_TimeoutError
static const byte fun_data_asyncio_core_TimeoutError[13] = {
    0x00,0x02, // prelude
    0x0d, // names: TimeoutError
     // code info
    0x11,0x4c, // LOAD_NAME '__name__'
    0x16,0x4d, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'TimeoutError'
    0x16,0x4e, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_TimeoutError = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_TimeoutError,
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
        .line_info = fun_data_asyncio_core_TimeoutError + 3,
        .line_info_top = fun_data_asyncio_core_TimeoutError + 3,
        .opcodes = fun_data_asyncio_core_TimeoutError + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator
static const byte fun_data_asyncio_core_SingletonGenerator[25] = {
    0x00,0x02, // prelude
    0x11, // names: SingletonGenerator
     // code info
    0x11,0x4c, // LOAD_NAME '__name__'
    0x16,0x4d, // STORE_NAME '__module__'
    0x10,0x11, // LOAD_CONST_STRING 'SingletonGenerator'
    0x16,0x4e, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x2b, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x2d, // STORE_NAME '__iter__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2e, // STORE_NAME '__next__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___init__
static const byte fun_data_asyncio_core_SingletonGenerator___init__[17] = {
    0x11,0x04, // prelude
    0x2b,0x60, // names: __init__, self
     // code info
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'state'
    0x12,0x5a, // LOAD_GLOBAL 'StopIteration'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'exc'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_SingletonGenerator___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___init__,
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
        .qstr_block_name_idx = 43,
        .line_info = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___iter__
static const byte fun_data_asyncio_core_SingletonGenerator___iter__[6] = {
    0x09,0x04, // prelude
    0x2d,0x60, // names: __iter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_SingletonGenerator___iter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___iter__,
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
        .qstr_block_name_idx = 45,
        .line_info = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___next__
static const byte fun_data_asyncio_core_SingletonGenerator___next__[42] = {
    0x21,0x04, // prelude
    0x2e,0x60, // names: __next__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0x12,0x5c, // LOAD_GLOBAL 'cur_task'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'state'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'exc'
    0x18,0x2f, // STORE_ATTR '__traceback__'
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'exc'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_SingletonGenerator___next__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___next__,
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
        .qstr_block_name_idx = 46,
        .line_info = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_core_SingletonGenerator[] = {
    &raw_code_asyncio_core_SingletonGenerator___init__,
    &raw_code_asyncio_core_SingletonGenerator___iter__,
    &raw_code_asyncio_core_SingletonGenerator___next__,
};

static const mp_raw_code_t raw_code_asyncio_core_SingletonGenerator = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_SingletonGenerator,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = (void *)&children_asyncio_core_SingletonGenerator,
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
        .qstr_block_name_idx = 17,
        .line_info = fun_data_asyncio_core_SingletonGenerator + 3,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator + 3,
        .opcodes = fun_data_asyncio_core_SingletonGenerator + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_sleep_ms
static const byte fun_data_asyncio_core_sleep_ms[25] = {
    0xb2,0x01,0x06, // prelude
    0x14,0x4f,0x50, // names: sleep_ms, t, sgen
     // code info
    0x12,0x04, // LOAD_GLOBAL 'ticks_add'
    0x12,0x47, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x51, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xb1, // LOAD_FAST 1
    0x18,0x15, // STORE_ATTR 'state'
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_sleep_ms = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_sleep_ms,
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
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_core_sleep_ms + 6,
        .line_info_top = fun_data_asyncio_core_sleep_ms + 6,
        .opcodes = fun_data_asyncio_core_sleep_ms + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_sleep
static const byte fun_data_asyncio_core_sleep[18] = {
    0x21,0x04, // prelude
    0x16,0x4f, // names: sleep, t
     // code info
    0x12,0x14, // LOAD_GLOBAL 'sleep_ms'
    0x12,0x52, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_sleep = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_sleep,
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
        .qstr_block_name_idx = 22,
        .line_info = fun_data_asyncio_core_sleep + 4,
        .line_info_top = fun_data_asyncio_core_sleep + 4,
        .opcodes = fun_data_asyncio_core_sleep + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue
static const byte fun_data_asyncio_core_IOQueue[41] = {
    0x00,0x02, // prelude
    0x12, // names: IOQueue
     // code info
    0x11,0x4c, // LOAD_NAME '__name__'
    0x16,0x4d, // STORE_NAME '__module__'
    0x10,0x12, // LOAD_CONST_STRING 'IOQueue'
    0x16,0x4e, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x2b, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x32, // STORE_NAME '_enqueue'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x37, // STORE_NAME '_dequeue'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x39, // STORE_NAME 'queue_read'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x3a, // STORE_NAME 'queue_write'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x3b, // STORE_NAME 'remove'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1f, // STORE_NAME 'wait_io_event'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue___init__
static const byte fun_data_asyncio_core_IOQueue___init__[20] = {
    0x11,0x04, // prelude
    0x2b,0x60, // names: __init__, self
     // code info
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x14,0x30, // LOAD_METHOD 'poll'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x31, // STORE_ATTR 'poller'
    0x2c,0x00, // BUILD_MAP 0
    0xb0, // LOAD_FAST 0
    0x18,0x1e, // STORE_ATTR 'map'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_IOQueue___init__,
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
        .qstr_block_name_idx = 43,
        .line_info = fun_data_asyncio_core_IOQueue___init__ + 4,
        .line_info_top = fun_data_asyncio_core_IOQueue___init__ + 4,
        .opcodes = fun_data_asyncio_core_IOQueue___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue__enqueue
static const byte fun_data_asyncio_core_IOQueue__enqueue[105] = {
    0x4b,0x08, // prelude
    0x32,0x60,0x61,0x62, // names: _enqueue, self, s, idx
     // code info
    0x12,0x63, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xb1, // LOAD_FAST 1
    0x2b,0x03, // BUILD_LIST 3
    0xc3, // STORE_FAST 3
    0x12,0x5c, // LOAD_GLOBAL 'cur_task'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x63, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x33, // LOAD_METHOD 'register'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x42,0x44, // JUMP 4
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x61, // JUMP 33
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x63, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x12,0x5c, // LOAD_GLOBAL 'cur_task'
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb1, // LOAD_FAST 1
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0xed, // BINARY_OP 22 __or__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x12,0x5c, // LOAD_GLOBAL 'cur_task'
    0x18,0x21, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue__enqueue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_asyncio_core_IOQueue__enqueue,
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
        .qstr_block_name_idx = 50,
        .line_info = fun_data_asyncio_core_IOQueue__enqueue + 6,
        .line_info_top = fun_data_asyncio_core_IOQueue__enqueue + 6,
        .opcodes = fun_data_asyncio_core_IOQueue__enqueue + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue__dequeue
static const byte fun_data_asyncio_core_IOQueue__dequeue[27] = {
    0x22,0x06, // prelude
    0x37,0x60,0x61, // names: _dequeue, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x63, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x53, // LOAD_NULL
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x38, // LOAD_METHOD 'unregister'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue__dequeue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_IOQueue__dequeue,
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
        .qstr_block_name_idx = 55,
        .line_info = fun_data_asyncio_core_IOQueue__dequeue + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue__dequeue + 5,
        .opcodes = fun_data_asyncio_core_IOQueue__dequeue + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_queue_read
static const byte fun_data_asyncio_core_IOQueue_queue_read[15] = {
    0x2a,0x06, // prelude
    0x39,0x60,0x61, // names: queue_read, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue_queue_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_IOQueue_queue_read,
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
        .qstr_block_name_idx = 57,
        .line_info = fun_data_asyncio_core_IOQueue_queue_read + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_queue_read + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_queue_read + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_queue_write
static const byte fun_data_asyncio_core_IOQueue_queue_write[15] = {
    0x2a,0x06, // prelude
    0x3a,0x60,0x61, // names: queue_write, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue_queue_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_IOQueue_queue_write,
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
        .qstr_block_name_idx = 58,
        .line_info = fun_data_asyncio_core_IOQueue_queue_write + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_queue_write + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_queue_write + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_remove
static const byte fun_data_asyncio_core_IOQueue_remove[65] = {
    0x6a,0x06, // prelude
    0x3b,0x60,0x0b, // names: remove, self, task
     // code info
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x5f, // GET_ITER_STACK
    0x4b,0x1f, // FOR_ITER 31
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
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
    0x14,0x37, // LOAD_METHOD '_dequeue'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x42, // JUMP 2
    0x42,0x06, // JUMP -58
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue_remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_IOQueue_remove,
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
        .qstr_block_name_idx = 59,
        .line_info = fun_data_asyncio_core_IOQueue_remove + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_remove + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_remove + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_wait_io_event
static const byte fun_data_asyncio_core_IOQueue_wait_io_event[156] = {
    0x62,0x06, // prelude
    0x1f,0x60,0x64, // names: wait_io_event, self, dt
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x3c, // LOAD_METHOD 'ipoll'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x89,0x01, // FOR_ITER 137
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x63, // LOAD_GLOBAL 'id'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
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
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
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
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
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
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
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
    0x14,0x37, // LOAD_METHOD '_dequeue'
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
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0xf4,0x7e, // JUMP -140
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_IOQueue_wait_io_event = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_IOQueue_wait_io_event,
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
        .qstr_block_name_idx = 31,
        .line_info = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_core_IOQueue[] = {
    &raw_code_asyncio_core_IOQueue___init__,
    &raw_code_asyncio_core_IOQueue__enqueue,
    &raw_code_asyncio_core_IOQueue__dequeue,
    &raw_code_asyncio_core_IOQueue_queue_read,
    &raw_code_asyncio_core_IOQueue_queue_write,
    &raw_code_asyncio_core_IOQueue_remove,
    &raw_code_asyncio_core_IOQueue_wait_io_event,
};

static const mp_raw_code_t raw_code_asyncio_core_IOQueue = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_IOQueue,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = (void *)&children_asyncio_core_IOQueue,
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
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_core_IOQueue + 3,
        .line_info_top = fun_data_asyncio_core_IOQueue + 3,
        .opcodes = fun_data_asyncio_core_IOQueue + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core__promote_to_task
static const byte fun_data_asyncio_core__promote_to_task[21] = {
    0x19,0x04, // prelude
    0x17,0x53, // names: _promote_to_task, aw
     // code info
    0x12,0x54, // LOAD_GLOBAL 'isinstance'
    0xb0, // LOAD_FAST 0
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core__promote_to_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core__promote_to_task,
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
        .qstr_block_name_idx = 23,
        .line_info = fun_data_asyncio_core__promote_to_task + 4,
        .line_info_top = fun_data_asyncio_core__promote_to_task + 4,
        .opcodes = fun_data_asyncio_core__promote_to_task + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_create_task
static const byte fun_data_asyncio_core_create_task[40] = {
    0x21,0x04, // prelude
    0x18,0x22, // names: create_task, coro
     // code info
    0x12,0x55, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x19, // LOAD_CONST_STRING 'send'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x56, // LOAD_GLOBAL 'TypeError'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0xb0, // LOAD_FAST 0
    0x12,0x57, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_create_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_create_task,
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
        .qstr_block_name_idx = 24,
        .line_info = fun_data_asyncio_core_create_task + 4,
        .line_info_top = fun_data_asyncio_core_create_task + 4,
        .opcodes = fun_data_asyncio_core_create_task + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_run_until_complete
static const byte fun_data_asyncio_core_run_until_complete[297] = {
    0xf1,0x03,0x04, // prelude
    0x1b,0x59, // names: run_until_complete, main_task
     // code info
    0x12,0x0c, // LOAD_GLOBAL 'CancelledError'
    0x12,0x49, // LOAD_GLOBAL 'Exception'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc1, // STORE_FAST 1
    0x12,0x0c, // LOAD_GLOBAL 'CancelledError'
    0x12,0x5a, // LOAD_GLOBAL 'StopIteration'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc2, // STORE_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc3, // STORE_FAST 3
    0x42,0x6f, // JUMP 47
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xc3, // STORE_FAST 3
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1c, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x51, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x03, // LOAD_GLOBAL 'ticks_diff'
    0xb4, // LOAD_FAST 4
    0x13,0x1d, // LOAD_ATTR 'ph_key'
    0x12,0x47, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0x42,0x48, // JUMP 8
    0x12,0x5b, // LOAD_GLOBAL '_io_queue'
    0x13,0x1e, // LOAD_ATTR 'map'
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x5b, // LOAD_GLOBAL '_io_queue'
    0x14,0x1f, // LOAD_METHOD 'wait_io_event'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x0c, // POP_JUMP_IF_TRUE -52
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x20, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x17,0x5c, // STORE_GLOBAL 'cur_task'
    0x48,0x22, // SETUP_EXCEPT 34
    0xb4, // LOAD_FAST 4
    0x13,0x21, // LOAD_ATTR 'data'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0xb4, // LOAD_FAST 4
    0x13,0x22, // LOAD_ATTR 'coro'
    0x14,0x19, // LOAD_METHOD 'send'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x21, // STORE_ATTR 'data'
    0xb4, // LOAD_FAST 4
    0x13,0x22, // LOAD_ATTR 'coro'
    0x14,0x23, // LOAD_METHOD 'throw'
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
    0x12,0x54, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0x12,0x5a, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb6, // LOAD_FAST 6
    0x13,0x24, // LOAD_ATTR 'value'
    0x63, // RETURN_VALUE
    0xb6, // LOAD_FAST 6
    0x65, // RAISE_OBJ
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x44,0xe0,0x80, // POP_JUMP_IF_FALSE 96
    0x50, // LOAD_CONST_FALSE
    0xc7, // STORE_FAST 7
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0x42,0x78, // JUMP 56
    0x12,0x5d, // LOAD_GLOBAL 'callable'
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb4, // LOAD_FAST 4
    0x14,0x15, // LOAD_METHOD 'state'
    0xb4, // LOAD_FAST 4
    0xb6, // LOAD_FAST 6
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0x42,0x5f, // JUMP 31
    0x42,0x50, // JUMP 16
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x14,0x20, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x14,0x1c, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0xb7, // LOAD_FAST 7
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0x12,0x54, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0xb4, // LOAD_FAST 4
    0x18,0x21, // STORE_ATTR 'data'
    0x42,0x5e, // JUMP 30
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0xb5, // LOAD_FAST 5
    0x12,0x4a, // LOAD_GLOBAL '_exc_context'
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x12,0x4a, // LOAD_GLOBAL '_exc_context'
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x56, // STORE_SUBSCR
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x14,0x25, // LOAD_METHOD 'call_exception_handler'
    0x12,0x4a, // LOAD_GLOBAL '_exc_context'
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
static const mp_raw_code_t raw_code_asyncio_core_run_until_complete = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_run_until_complete,
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
        .line_info = fun_data_asyncio_core_run_until_complete + 5,
        .line_info_top = fun_data_asyncio_core_run_until_complete + 5,
        .opcodes = fun_data_asyncio_core_run_until_complete + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_run
static const byte fun_data_asyncio_core_run[14] = {
    0x19,0x04, // prelude
    0x26,0x22, // names: run, coro
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_run = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_run,
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
        .qstr_block_name_idx = 38,
        .line_info = fun_data_asyncio_core_run + 4,
        .line_info_top = fun_data_asyncio_core_run + 4,
        .opcodes = fun_data_asyncio_core_run + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core__stopper
static const byte fun_data_asyncio_core__stopper[6] = {
    0x80,0x40,0x02, // prelude
    0x27, // names: _stopper
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core__stopper = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core__stopper,
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
        .qstr_block_name_idx = 39,
        .line_info = fun_data_asyncio_core__stopper + 4,
        .line_info_top = fun_data_asyncio_core__stopper + 4,
        .opcodes = fun_data_asyncio_core__stopper + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop
static const byte fun_data_asyncio_core_Loop[52] = {
    0x00,0x02, // prelude
    0x13, // names: Loop
     // code info
    0x11,0x4c, // LOAD_NAME '__name__'
    0x16,0x4d, // STORE_NAME '__module__'
    0x10,0x13, // LOAD_CONST_STRING 'Loop'
    0x16,0x4e, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x16,0x41, // STORE_NAME '_exc_handler'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x18, // STORE_NAME 'create_task'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x3d, // STORE_NAME 'run_forever'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x3e, // STORE_NAME 'stop'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x3f, // STORE_NAME 'close'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x40, // STORE_NAME 'set_exception_handler'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x42, // STORE_NAME 'get_exception_handler'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x43, // STORE_NAME 'default_exception_handler'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x25, // STORE_NAME 'call_exception_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_create_task
static const byte fun_data_asyncio_core_Loop_create_task[10] = {
    0x11,0x04, // prelude
    0x18,0x22, // names: create_task, coro
     // code info
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_create_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_Loop_create_task,
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
        .qstr_block_name_idx = 24,
        .line_info = fun_data_asyncio_core_Loop_create_task + 4,
        .line_info_top = fun_data_asyncio_core_Loop_create_task + 4,
        .opcodes = fun_data_asyncio_core_Loop_create_task + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_run_forever
static const byte fun_data_asyncio_core_Loop_run_forever[26] = {
    0x10,0x02, // prelude
    0x3d, // names: run_forever
     // code info
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0x12,0x27, // LOAD_GLOBAL '_stopper'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x57, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x17,0x4b, // STORE_GLOBAL '_stop_task'
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x4b, // LOAD_GLOBAL '_stop_task'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_run_forever = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_Loop_run_forever,
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
        .qstr_block_name_idx = 61,
        .line_info = fun_data_asyncio_core_Loop_run_forever + 3,
        .line_info_top = fun_data_asyncio_core_Loop_run_forever + 3,
        .opcodes = fun_data_asyncio_core_Loop_run_forever + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_run_until_complete
static const byte fun_data_asyncio_core_Loop_run_until_complete[14] = {
    0x19,0x04, // prelude
    0x1b,0x53, // names: run_until_complete, aw
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x17, // LOAD_GLOBAL '_promote_to_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_run_until_complete = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_Loop_run_until_complete,
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
        .line_info = fun_data_asyncio_core_Loop_run_until_complete + 4,
        .line_info_top = fun_data_asyncio_core_Loop_run_until_complete + 4,
        .opcodes = fun_data_asyncio_core_Loop_run_until_complete + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_stop
static const byte fun_data_asyncio_core_Loop_stop[24] = {
    0x10,0x02, // prelude
    0x3e, // names: stop
     // code info
    0x12,0x4b, // LOAD_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x58, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0x12,0x4b, // LOAD_GLOBAL '_stop_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x17,0x4b, // STORE_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_stop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_Loop_stop,
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
        .qstr_block_name_idx = 62,
        .line_info = fun_data_asyncio_core_Loop_stop + 3,
        .line_info_top = fun_data_asyncio_core_Loop_stop + 3,
        .opcodes = fun_data_asyncio_core_Loop_stop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_close
static const byte fun_data_asyncio_core_Loop_close[5] = {
    0x00,0x02, // prelude
    0x3f, // names: close
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_Loop_close,
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
        .qstr_block_name_idx = 63,
        .line_info = fun_data_asyncio_core_Loop_close + 3,
        .line_info_top = fun_data_asyncio_core_Loop_close + 3,
        .opcodes = fun_data_asyncio_core_Loop_close + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_set_exception_handler
static const byte fun_data_asyncio_core_Loop_set_exception_handler[11] = {
    0x11,0x04, // prelude
    0x40,0x65, // names: set_exception_handler, handler
     // code info
    0xb0, // LOAD_FAST 0
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x18,0x41, // STORE_ATTR '_exc_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_set_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_Loop_set_exception_handler,
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
        .qstr_block_name_idx = 64,
        .line_info = fun_data_asyncio_core_Loop_set_exception_handler + 4,
        .line_info_top = fun_data_asyncio_core_Loop_set_exception_handler + 4,
        .opcodes = fun_data_asyncio_core_Loop_set_exception_handler + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_get_exception_handler
static const byte fun_data_asyncio_core_Loop_get_exception_handler[8] = {
    0x00,0x02, // prelude
    0x42, // names: get_exception_handler
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x41, // LOAD_ATTR '_exc_handler'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_get_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_Loop_get_exception_handler,
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
        .qstr_block_name_idx = 66,
        .line_info = fun_data_asyncio_core_Loop_get_exception_handler + 3,
        .line_info_top = fun_data_asyncio_core_Loop_get_exception_handler + 3,
        .opcodes = fun_data_asyncio_core_Loop_get_exception_handler + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_default_exception_handler
static const byte fun_data_asyncio_core_Loop_default_exception_handler[46] = {
    0x3a,0x06, // prelude
    0x43,0x66,0x67, // names: default_exception_handler, loop, context
     // code info
    0x12,0x68, // LOAD_GLOBAL 'print'
    0xb1, // LOAD_FAST 1
    0x10,0x0e, // LOAD_CONST_STRING 'message'
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x68, // LOAD_GLOBAL 'print'
    0x10,0x44, // LOAD_CONST_STRING 'future:'
    0xb1, // LOAD_FAST 1
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x10,0x45, // LOAD_CONST_STRING 'coro='
    0xb1, // LOAD_FAST 1
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x13,0x22, // LOAD_ATTR 'coro'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'sys'
    0x14,0x46, // LOAD_METHOD 'print_exception'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_default_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_Loop_default_exception_handler,
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
        .qstr_block_name_idx = 67,
        .line_info = fun_data_asyncio_core_Loop_default_exception_handler + 5,
        .line_info_top = fun_data_asyncio_core_Loop_default_exception_handler + 5,
        .opcodes = fun_data_asyncio_core_Loop_default_exception_handler + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_call_exception_handler
static const byte fun_data_asyncio_core_Loop_call_exception_handler[22] = {
    0x19,0x04, // prelude
    0x25,0x67, // names: call_exception_handler, context
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x41, // LOAD_ATTR '_exc_handler'
    0x45,0x04, // JUMP_IF_TRUE_OR_POP 4
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x43, // LOAD_ATTR 'default_exception_handler'
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_Loop_call_exception_handler = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_core_Loop_call_exception_handler,
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
        .qstr_block_name_idx = 37,
        .line_info = fun_data_asyncio_core_Loop_call_exception_handler + 4,
        .line_info_top = fun_data_asyncio_core_Loop_call_exception_handler + 4,
        .opcodes = fun_data_asyncio_core_Loop_call_exception_handler + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_core_Loop[] = {
    &raw_code_asyncio_core_Loop_create_task,
    &raw_code_asyncio_core_Loop_run_forever,
    &raw_code_asyncio_core_Loop_run_until_complete,
    &raw_code_asyncio_core_Loop_stop,
    &raw_code_asyncio_core_Loop_close,
    &raw_code_asyncio_core_Loop_set_exception_handler,
    &raw_code_asyncio_core_Loop_get_exception_handler,
    &raw_code_asyncio_core_Loop_default_exception_handler,
    &raw_code_asyncio_core_Loop_call_exception_handler,
};

static const mp_raw_code_t raw_code_asyncio_core_Loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_Loop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = (void *)&children_asyncio_core_Loop,
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
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_core_Loop + 3,
        .line_info_top = fun_data_asyncio_core_Loop + 3,
        .opcodes = fun_data_asyncio_core_Loop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_get_event_loop
static const byte fun_data_asyncio_core_get_event_loop[10] = {
    0x92,0x80,0x01,0x06, // prelude
    0x28,0x5e,0x5f, // names: get_event_loop, runq_len, waitq_len
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_get_event_loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_core_get_event_loop,
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
        .qstr_block_name_idx = 40,
        .line_info = fun_data_asyncio_core_get_event_loop + 7,
        .line_info_top = fun_data_asyncio_core_get_event_loop + 7,
        .opcodes = fun_data_asyncio_core_get_event_loop + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_current_task
static const byte fun_data_asyncio_core_current_task[6] = {
    0x00,0x02, // prelude
    0x29, // names: current_task
     // code info
    0x12,0x5c, // LOAD_GLOBAL 'cur_task'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_current_task = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_current_task,
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
        .qstr_block_name_idx = 41,
        .line_info = fun_data_asyncio_core_current_task + 3,
        .line_info_top = fun_data_asyncio_core_current_task + 3,
        .opcodes = fun_data_asyncio_core_current_task + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_new_event_loop
static const byte fun_data_asyncio_core_new_event_loop[18] = {
    0x00,0x02, // prelude
    0x2a, // names: new_event_loop
     // code info
    0x12,0x08, // LOAD_GLOBAL 'TaskQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x58, // STORE_GLOBAL '_task_queue'
    0x12,0x12, // LOAD_GLOBAL 'IOQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x5b, // STORE_GLOBAL '_io_queue'
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_core_new_event_loop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core_new_event_loop,
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
        .qstr_block_name_idx = 42,
        .line_info = fun_data_asyncio_core_new_event_loop + 3,
        .line_info_top = fun_data_asyncio_core_new_event_loop + 3,
        .opcodes = fun_data_asyncio_core_new_event_loop + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_core__lt_module_gt_[] = {
    &raw_code_asyncio_core_CancelledError,
    &raw_code_asyncio_core_TimeoutError,
    &raw_code_asyncio_core_SingletonGenerator,
    &raw_code_asyncio_core_sleep_ms,
    &raw_code_asyncio_core_sleep,
    &raw_code_asyncio_core_IOQueue,
    &raw_code_asyncio_core__promote_to_task,
    &raw_code_asyncio_core_create_task,
    &raw_code_asyncio_core_run_until_complete,
    &raw_code_asyncio_core_run,
    &raw_code_asyncio_core__stopper,
    &raw_code_asyncio_core_Loop,
    &raw_code_asyncio_core_get_event_loop,
    &raw_code_asyncio_core_current_task,
    &raw_code_asyncio_core_new_event_loop,
};

static const mp_raw_code_t raw_code_asyncio_core__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_core__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 215,
    #endif
    .children = (void *)&children_asyncio_core__lt_module_gt_,
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
        .line_info = fun_data_asyncio_core__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_core__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_core__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_core[105] = {
    MP_QSTR_asyncio_slash_core_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_ticks_ms,
    MP_QSTR_ticks_diff,
    MP_QSTR_ticks_add,
    MP_QSTR_time,
    MP_QSTR_sys,
    MP_QSTR_select,
    MP_QSTR_TaskQueue,
    MP_QSTR_Task,
    MP_QSTR__asyncio,
    MP_QSTR_task,
    MP_QSTR_CancelledError,
    MP_QSTR_TimeoutError,
    MP_QSTR_message,
    MP_QSTR_exception,
    MP_QSTR_future,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_IOQueue,
    MP_QSTR_Loop,
    MP_QSTR_sleep_ms,
    MP_QSTR_state,
    MP_QSTR_sleep,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_send,
    MP_QSTR_push,
    MP_QSTR_run_until_complete,
    MP_QSTR_peek,
    MP_QSTR_ph_key,
    MP_QSTR_map,
    MP_QSTR_wait_io_event,
    MP_QSTR_pop,
    MP_QSTR_data,
    MP_QSTR_coro,
    MP_QSTR_throw,
    MP_QSTR_value,
    MP_QSTR_call_exception_handler,
    MP_QSTR_run,
    MP_QSTR__stopper,
    MP_QSTR_get_event_loop,
    MP_QSTR_current_task,
    MP_QSTR_new_event_loop,
    MP_QSTR___init__,
    MP_QSTR_exc,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR___traceback__,
    MP_QSTR_poll,
    MP_QSTR_poller,
    MP_QSTR__enqueue,
    MP_QSTR_register,
    MP_QSTR_POLLIN,
    MP_QSTR_POLLOUT,
    MP_QSTR_modify,
    MP_QSTR__dequeue,
    MP_QSTR_unregister,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_remove,
    MP_QSTR_ipoll,
    MP_QSTR_run_forever,
    MP_QSTR_stop,
    MP_QSTR_close,
    MP_QSTR_set_exception_handler,
    MP_QSTR__exc_handler,
    MP_QSTR_get_exception_handler,
    MP_QSTR_default_exception_handler,
    MP_QSTR_future_colon_,
    MP_QSTR_coro_equals_,
    MP_QSTR_print_exception,
    MP_QSTR_ticks,
    MP_QSTR_BaseException,
    MP_QSTR_Exception,
    MP_QSTR__exc_context,
    MP_QSTR__stop_task,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_t,
    MP_QSTR_sgen,
    MP_QSTR_max,
    MP_QSTR_int,
    MP_QSTR_aw,
    MP_QSTR_isinstance,
    MP_QSTR_hasattr,
    MP_QSTR_TypeError,
    MP_QSTR_globals,
    MP_QSTR__task_queue,
    MP_QSTR_main_task,
    MP_QSTR_StopIteration,
    MP_QSTR__io_queue,
    MP_QSTR_cur_task,
    MP_QSTR_callable,
    MP_QSTR_runq_len,
    MP_QSTR_waitq_len,
    MP_QSTR_self,
    MP_QSTR_s,
    MP_QSTR_idx,
    MP_QSTR_id,
    MP_QSTR_dt,
    MP_QSTR_handler,
    MP_QSTR_loop,
    MP_QSTR_context,
    MP_QSTR_print,
};

// constants
static const mp_obj_str_t const_obj_asyncio_core_0 = {{&mp_type_str}, 64973, 31, (const byte*)"\x54\x61\x73\x6b\x20\x65\x78\x63\x65\x70\x74\x69\x6f\x6e\x20\x77\x61\x73\x6e\x27\x74\x20\x72\x65\x74\x72\x69\x65\x76\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_core[2] = {
    MP_ROM_PTR(&const_obj_asyncio_core_0),
    MP_ROM_QSTR(MP_QSTR_coroutine_space_expected),
};

static const mp_frozen_module_t frozen_module_asyncio_core = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_core,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_core,
    },
    .rc = &raw_code_asyncio_core__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_event
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/event.mpy
// - frozen file name: asyncio/event.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/event.py, scope asyncio_event__lt_module_gt_
static const byte fun_data_asyncio_event__lt_module_gt_[59] = {
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
    0x1b,0x05, // IMPORT_NAME 'io'
    0x16,0x05, // STORE_NAME 'io'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x11,0x05, // LOAD_NAME 'io'
    0x13,0x07, // LOAD_ATTR 'IOBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x06, // STORE_NAME 'ThreadSafeFlag'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x11,0x19, // LOAD_NAME 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event__lt_module_gt_
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event
static const byte fun_data_asyncio_event_Event[33] = {
    0x00,0x02, // prelude
    0x04, // names: Event
     // code info
    0x11,0x1a, // LOAD_NAME '__name__'
    0x16,0x1b, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Event'
    0x16,0x1c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0c, // STORE_NAME 'is_set'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x12, // STORE_NAME 'clear'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event___init__
static const byte fun_data_asyncio_event_Event___init__[19] = {
    0x11,0x04, // prelude
    0x08,0x1d, // names: __init__, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0a, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_Event___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_Event___init__,
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
        .line_info = fun_data_asyncio_event_Event___init__ + 4,
        .line_info_top = fun_data_asyncio_event_Event___init__ + 4,
        .opcodes = fun_data_asyncio_event_Event___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_is_set
static const byte fun_data_asyncio_event_Event_is_set[8] = {
    0x09,0x04, // prelude
    0x0c,0x1d, // names: is_set, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_Event_is_set = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_Event_is_set,
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
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio_event_Event_is_set + 4,
        .line_info_top = fun_data_asyncio_event_Event_is_set + 4,
        .opcodes = fun_data_asyncio_event_Event_is_set + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_set
static const byte fun_data_asyncio_event_Event_set[37] = {
    0x21,0x04, // prelude
    0x0d,0x1d, // names: set, self
     // code info
    0x42,0x50, // JUMP 16
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR '_task_queue'
    0x14,0x0f, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x10, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x11, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_Event_set = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_Event_set,
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
        .qstr_block_name_idx = 13,
        .line_info = fun_data_asyncio_event_Event_set + 4,
        .line_info_top = fun_data_asyncio_event_Event_set + 4,
        .opcodes = fun_data_asyncio_event_Event_set + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_clear
static const byte fun_data_asyncio_event_Event_clear[10] = {
    0x11,0x04, // prelude
    0x12,0x1d, // names: clear, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_Event_clear = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_Event_clear,
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
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_event_Event_clear + 4,
        .line_info_top = fun_data_asyncio_event_Event_clear + 4,
        .opcodes = fun_data_asyncio_event_Event_clear + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_wait
static const byte fun_data_asyncio_event_Event_wait[36] = {
    0x99,0x40,0x04, // prelude
    0x13,0x1d, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x0f, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x14, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x14, // LOAD_ATTR 'cur_task'
    0x18,0x15, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_Event_wait = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_Event_wait,
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
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_event_Event_wait + 5,
        .line_info_top = fun_data_asyncio_event_Event_wait + 5,
        .opcodes = fun_data_asyncio_event_Event_wait + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_event_Event[] = {
    &raw_code_asyncio_event_Event___init__,
    &raw_code_asyncio_event_Event_is_set,
    &raw_code_asyncio_event_Event_set,
    &raw_code_asyncio_event_Event_clear,
    &raw_code_asyncio_event_Event_wait,
};

static const mp_raw_code_t raw_code_asyncio_event_Event = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_event_Event,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_asyncio_event_Event,
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
        .line_info = fun_data_asyncio_event_Event + 3,
        .line_info_top = fun_data_asyncio_event_Event + 3,
        .opcodes = fun_data_asyncio_event_Event + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event__lt_module_gt_
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag
static const byte fun_data_asyncio_event_ThreadSafeFlag[33] = {
    0x00,0x02, // prelude
    0x06, // names: ThreadSafeFlag
     // code info
    0x11,0x1a, // LOAD_NAME '__name__'
    0x16,0x1b, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x16,0x1c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME 'ioctl'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x12, // STORE_NAME 'clear'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag___init__
static const byte fun_data_asyncio_event_ThreadSafeFlag___init__[10] = {
    0x11,0x04, // prelude
    0x08,0x1d, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag___init__,
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
        .qstr_block_name_idx = 8,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_ioctl
static const byte fun_data_asyncio_event_ThreadSafeFlag_ioctl[19] = {
    0x23,0x08, // prelude
    0x16,0x1d,0x1e,0x1f, // names: ioctl, self, req, flags
     // code info
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag_ioctl = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_ioctl,
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
        .qstr_block_name_idx = 22,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_set
static const byte fun_data_asyncio_event_ThreadSafeFlag_set[10] = {
    0x11,0x04, // prelude
    0x0d,0x1d, // names: set, self
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag_set = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_set,
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
        .qstr_block_name_idx = 13,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_clear
static const byte fun_data_asyncio_event_ThreadSafeFlag_clear[10] = {
    0x11,0x04, // prelude
    0x12,0x1d, // names: clear, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag_clear = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_clear,
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
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_wait
static const byte fun_data_asyncio_event_ThreadSafeFlag_wait[27] = {
    0x99,0x40,0x04, // prelude
    0x13,0x1d, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x17, // LOAD_ATTR '_io_queue'
    0x14,0x18, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag_wait = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_wait,
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
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_event_ThreadSafeFlag[] = {
    &raw_code_asyncio_event_ThreadSafeFlag___init__,
    &raw_code_asyncio_event_ThreadSafeFlag_ioctl,
    &raw_code_asyncio_event_ThreadSafeFlag_set,
    &raw_code_asyncio_event_ThreadSafeFlag_clear,
    &raw_code_asyncio_event_ThreadSafeFlag_wait,
};

static const mp_raw_code_t raw_code_asyncio_event_ThreadSafeFlag = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_asyncio_event_ThreadSafeFlag,
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
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag + 3,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag + 3,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_event__lt_module_gt_[] = {
    &raw_code_asyncio_event_Event,
    &raw_code_asyncio_event_ThreadSafeFlag,
};

static const mp_raw_code_t raw_code_asyncio_event__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_event__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 59,
    #endif
    .children = (void *)&children_asyncio_event__lt_module_gt_,
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
        .line_info = fun_data_asyncio_event__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_event__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_event__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_event[32] = {
    MP_QSTR_asyncio_slash_event_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Event,
    MP_QSTR_io,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_IOBase,
    MP_QSTR___init__,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR_is_set,
    MP_QSTR_set,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_pop,
    MP_QSTR_peek,
    MP_QSTR_clear,
    MP_QSTR_wait,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_ioctl,
    MP_QSTR__io_queue,
    MP_QSTR_queue_read,
    MP_QSTR_ImportError,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_req,
    MP_QSTR_flags,
};

static const mp_frozen_module_t frozen_module_asyncio_event = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_event,
        .obj_table = NULL,
    },
    .rc = &raw_code_asyncio_event__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_funcs
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/funcs.mpy
// - frozen file name: asyncio/funcs.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__lt_module_gt_
static const byte fun_data_asyncio_funcs__lt_module_gt_[56] = {
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
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME '_run'
    0x11,0x02, // LOAD_NAME 'core'
    0x13,0x04, // LOAD_ATTR 'sleep'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x0b, // STORE_NAME 'wait_for'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x12, // STORE_NAME 'wait_for_ms'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x05, // LOAD_CONST_STRING '_Remove'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME '_Remove'
    0x53, // LOAD_NULL
    0x2c,0x00, // BUILD_MAP 0
    0x50, // LOAD_CONST_FALSE
    0x10,0x06, // LOAD_CONST_STRING 'return_exceptions'
    0x62, // STORE_MAP
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x14, // STORE_NAME 'gather'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__run
static const byte fun_data_asyncio_funcs__run[66] = {
    0xd2,0x42,0x06, // prelude
    0x07,0x1b,0x1c, // names: _run, waiter, aw
     // code info
    0x48,0x09, // SETUP_EXCEPT 9
    0xb1, // LOAD_FAST 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0xc2, // STORE_FAST 2
    0x52, // LOAD_CONST_TRUE
    0xc3, // STORE_FAST 3
    0x4a,0x16, // POP_EXCEPT_JUMP 22
    0x57, // DUP_TOP
    0x12,0x1d, // LOAD_GLOBAL 'BaseException'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xc4, // STORE_FAST 4
    0x49,0x05, // SETUP_FINALLY 5
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0xb4, // LOAD_FAST 4
    0xc3, // STORE_FAST 3
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb0, // LOAD_FAST 0
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0a, // LOAD_METHOD 'CancelledError'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_funcs__run = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_funcs__run,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 66,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_asyncio_funcs__run + 6,
        .line_info_top = fun_data_asyncio_funcs__run + 6,
        .opcodes = fun_data_asyncio_funcs__run + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_wait_for
static const byte fun_data_asyncio_funcs_wait_for[119] = {
    0xdb,0x43,0x08, // prelude
    0x0b,0x1c,0x1e,0x04, // names: wait_for, aw, timeout, sleep
     // code info
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
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0d, // LOAD_METHOD 'create_task'
    0x12,0x07, // LOAD_GLOBAL '_run'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR 'cur_task'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x48,0x0a, // SETUP_EXCEPT 10
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x4a,0x32, // POP_EXCEPT_JUMP 50
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0a, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x69, // POP_JUMP_IF_FALSE 41
    0xc4, // STORE_FAST 4
    0x49,0x1f, // SETUP_FINALLY 31
    0xb4, // LOAD_FAST 4
    0x13,0x0f, // LOAD_ATTR 'value'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb3, // LOAD_FAST 3
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x65, // RAISE_OBJ
    0xb5, // LOAD_FAST 5
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb4, // LOAD_FAST 4
    0x13,0x10, // LOAD_ATTR 'args'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0xb5, // LOAD_FAST 5
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x11, // LOAD_ATTR 'TimeoutError'
    0x65, // RAISE_OBJ
};
static const mp_raw_code_t raw_code_asyncio_funcs_wait_for = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 3,
    .fun_data = fun_data_asyncio_funcs_wait_for,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 119,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_asyncio_funcs_wait_for + 7,
        .line_info_top = fun_data_asyncio_funcs_wait_for + 7,
        .opcodes = fun_data_asyncio_funcs_wait_for + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_wait_for_ms
static const byte fun_data_asyncio_funcs_wait_for_ms[16] = {
    0x2a,0x06, // prelude
    0x12,0x1c,0x1e, // names: wait_for_ms, aw, timeout
     // code info
    0x12,0x0b, // LOAD_GLOBAL 'wait_for'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x13, // LOAD_ATTR 'sleep_ms'
    0x34,0x03, // CALL_FUNCTION 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_funcs_wait_for_ms = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_funcs_wait_for_ms,
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
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_funcs_wait_for_ms + 5,
        .line_info_top = fun_data_asyncio_funcs_wait_for_ms + 5,
        .opcodes = fun_data_asyncio_funcs_wait_for_ms + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__Remove
static const byte fun_data_asyncio_funcs__Remove[21] = {
    0x08,0x02, // prelude
    0x05, // names: _Remove
     // code info
    0x11,0x1f, // LOAD_NAME '__name__'
    0x16,0x20, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING '_Remove'
    0x16,0x21, // STORE_NAME '__qualname__'
    0x11,0x22, // LOAD_NAME 'staticmethod'
    0x32,0x00, // MAKE_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x16, // STORE_NAME 'remove'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs__Remove
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__Remove_remove
static const byte fun_data_asyncio_funcs__Remove_remove[6] = {
    0x09,0x04, // prelude
    0x16,0x27, // names: remove, t
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_funcs__Remove_remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_funcs__Remove_remove,
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
        .line_info = fun_data_asyncio_funcs__Remove_remove + 4,
        .line_info_top = fun_data_asyncio_funcs__Remove_remove + 4,
        .opcodes = fun_data_asyncio_funcs__Remove_remove + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_funcs__Remove[] = {
    &raw_code_asyncio_funcs__Remove_remove,
};

static const mp_raw_code_t raw_code_asyncio_funcs__Remove = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_funcs__Remove,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 21,
    #endif
    .children = (void *)&children_asyncio_funcs__Remove,
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
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_funcs__Remove + 3,
        .line_info_top = fun_data_asyncio_funcs__Remove + 3,
        .opcodes = fun_data_asyncio_funcs__Remove + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather
static const byte fun_data_asyncio_funcs_gather[217] = {
    0xf0,0xca,0x80,0xc0,0x40,0x85,0x01, // prelude
    0x14,0x06, // names: gather, return_exceptions
    0x00,0x07,0x08, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x2b,0x00, // BUILD_LIST 0
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0xb7, // LOAD_FAST 7
    0xb8, // LOAD_FAST 8
    0x20,0x00,0x03, // MAKE_CLOSURE 0
    0xc2, // STORE_FAST 2
    0x32,0x01, // MAKE_FUNCTION 1
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5b, // JUMP 27
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x15, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x24, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x15, // STORE_ATTR 'state'
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
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x27,0x08, // STORE_DEREF 8
    0x50, // LOAD_CONST_FALSE
    0xc5, // STORE_FAST 5
    0x12,0x05, // LOAD_GLOBAL '_Remove'
    0x25,0x07, // LOAD_DEREF 7
    0x18,0x08, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x19, // POP_EXCEPT_JUMP 25
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0a, // LOAD_ATTR 'CancelledError'
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
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc1,0x80, // JUMP 65
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x15, // LOAD_ATTR 'state'
    0xb2, // LOAD_FAST 2
    0xde, // BINARY_OP 7 <is>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x52, // LOAD_CONST_TRUE
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x15, // STORE_ATTR 'state'
    0xb5, // LOAD_FAST 5
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x61, // JUMP 33
    0x12,0x25, // LOAD_GLOBAL 'isinstance'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x12,0x26, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x13,0x0f, // LOAD_ATTR 'value'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x56, // STORE_SUBSCR
    0x42,0x48, // JUMP 8
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
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
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x25,0x08, // LOAD_DEREF 8
    0x65, // RAISE_OBJ
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs_gather
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather_done
static const byte fun_data_asyncio_funcs_gather_done[64] = {
    0xb9,0x04,0x0c, // prelude
    0x17,0x28,0x28,0x28,0x27,0x29, // names: done, *, *, *, t, er
     // code info
    0x25,0x01, // LOAD_DEREF 1
    0x13,0x08, // LOAD_ATTR 'data'
    0x12,0x05, // LOAD_GLOBAL '_Remove'
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x25,0x00, // LOAD_DEREF 0
    0x43,0x4e, // POP_JUMP_IF_TRUE 14
    0x12,0x25, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0x12,0x26, // LOAD_GLOBAL 'StopIteration'
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
    0x13,0x18, // LOAD_ATTR '_task_queue'
    0x14,0x19, // LOAD_METHOD 'push'
    0x25,0x01, // LOAD_DEREF 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_funcs_gather_done = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 5,
    .fun_data = fun_data_asyncio_funcs_gather_done,
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
        .qstr_block_name_idx = 23,
        .line_info = fun_data_asyncio_funcs_gather_done + 9,
        .line_info_top = fun_data_asyncio_funcs_gather_done + 9,
        .opcodes = fun_data_asyncio_funcs_gather_done + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_funcs_gather
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather__lt_listcomp_gt_
static const byte fun_data_asyncio_funcs_gather__lt_listcomp_gt_[23] = {
    0x49,0x04, // prelude
    0x1a,0x28, // names: <listcomp>, *
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
static const mp_raw_code_t raw_code_asyncio_funcs_gather__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_funcs_gather__lt_listcomp_gt_,
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
        .qstr_block_name_idx = 26,
        .line_info = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_funcs_gather[] = {
    &raw_code_asyncio_funcs_gather_done,
    &raw_code_asyncio_funcs_gather__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_asyncio_funcs_gather = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x0d,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_funcs_gather,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 217,
    #endif
    .children = (void *)&children_asyncio_funcs_gather,
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
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_funcs_gather + 9,
        .line_info_top = fun_data_asyncio_funcs_gather + 9,
        .opcodes = fun_data_asyncio_funcs_gather + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_funcs__lt_module_gt_[] = {
    &raw_code_asyncio_funcs__run,
    &raw_code_asyncio_funcs_wait_for,
    &raw_code_asyncio_funcs_wait_for_ms,
    &raw_code_asyncio_funcs__Remove,
    &raw_code_asyncio_funcs_gather,
};

static const mp_raw_code_t raw_code_asyncio_funcs__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_funcs__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 56,
    #endif
    .children = (void *)&children_asyncio_funcs__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_funcs__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_funcs__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_funcs__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_funcs[42] = {
    MP_QSTR_asyncio_slash_funcs_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_sleep,
    MP_QSTR__Remove,
    MP_QSTR_return_exceptions,
    MP_QSTR__run,
    MP_QSTR_data,
    MP_QSTR_cancel,
    MP_QSTR_CancelledError,
    MP_QSTR_wait_for,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_cur_task,
    MP_QSTR_value,
    MP_QSTR_args,
    MP_QSTR_TimeoutError,
    MP_QSTR_wait_for_ms,
    MP_QSTR_sleep_ms,
    MP_QSTR_gather,
    MP_QSTR_state,
    MP_QSTR_remove,
    MP_QSTR_done,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_waiter,
    MP_QSTR_aw,
    MP_QSTR_BaseException,
    MP_QSTR_timeout,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_staticmethod,
    MP_QSTR_len,
    MP_QSTR_RuntimeError,
    MP_QSTR_isinstance,
    MP_QSTR_StopIteration,
    MP_QSTR_t,
    MP_QSTR__star_,
    MP_QSTR_er,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_funcs[1] = {
    MP_ROM_QSTR(MP_QSTR_can_squot_t_space_gather),
};

static const mp_frozen_module_t frozen_module_asyncio_funcs = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_funcs,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_funcs,
    },
    .rc = &raw_code_asyncio_funcs__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_lock
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/lock.mpy
// - frozen file name: asyncio/lock.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/lock.py, scope asyncio_lock__lt_module_gt_
static const byte fun_data_asyncio_lock__lt_module_gt_[26] = {
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
// child of asyncio_lock__lt_module_gt_
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock
static const byte fun_data_asyncio_lock_Lock[37] = {
    0x00,0x02, // prelude
    0x04, // names: Lock
     // code info
    0x11,0x15, // LOAD_NAME '__name__'
    0x16,0x16, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Lock'
    0x16,0x17, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x05, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'locked'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'release'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0f, // STORE_NAME 'acquire'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME '__aenter__'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x14, // STORE_NAME '__aexit__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___init__
static const byte fun_data_asyncio_lock_Lock___init__[19] = {
    0x11,0x04, // prelude
    0x05,0x18, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x07, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_lock_Lock___init__,
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
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_lock_Lock___init__ + 4,
        .line_info_top = fun_data_asyncio_lock_Lock___init__ + 4,
        .opcodes = fun_data_asyncio_lock_Lock___init__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_locked
static const byte fun_data_asyncio_lock_Lock_locked[10] = {
    0x11,0x04, // prelude
    0x09,0x18, // names: locked, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock_locked = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_lock_Lock_locked,
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
        .line_info = fun_data_asyncio_lock_Lock_locked + 4,
        .line_info_top = fun_data_asyncio_lock_Lock_locked + 4,
        .opcodes = fun_data_asyncio_lock_Lock_locked + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_release
static const byte fun_data_asyncio_lock_Lock_release[57] = {
    0x19,0x04, // prelude
    0x0a,0x18, // names: release, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x19, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0b, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0c, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0d, // LOAD_ATTR '_task_queue'
    0x14,0x0e, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x44, // JUMP 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock_release = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_lock_Lock_release,
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
        .line_info = fun_data_asyncio_lock_Lock_release + 4,
        .line_info_top = fun_data_asyncio_lock_Lock_release + 4,
        .opcodes = fun_data_asyncio_lock_Lock_release + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_acquire
static const byte fun_data_asyncio_lock_Lock_acquire[88] = {
    0xb9,0x42,0x04, // prelude
    0x0f,0x18, // names: acquire, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x44,0xc5,0x80, // POP_JUMP_IF_FALSE 69
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0e, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0x18,0x11, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x29, // POP_EXCEPT_JUMP 41
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0xc1, // STORE_FAST 1
    0x49,0x16, // SETUP_FINALLY 22
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0xc1, // STORE_FAST 1
    0x28,0x01, // DELETE_FAST 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock_acquire = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_lock_Lock_acquire,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 88,
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
        .qstr_block_name_idx = 15,
        .line_info = fun_data_asyncio_lock_Lock_acquire + 5,
        .line_info_top = fun_data_asyncio_lock_Lock_acquire + 5,
        .opcodes = fun_data_asyncio_lock_Lock_acquire + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___aenter__
static const byte fun_data_asyncio_lock_Lock___aenter__[14] = {
    0x91,0x40,0x04, // prelude
    0x13,0x18, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'acquire'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock___aenter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_lock_Lock___aenter__,
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
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_lock_Lock___aenter__ + 5,
        .line_info_top = fun_data_asyncio_lock_Lock___aenter__ + 5,
        .opcodes = fun_data_asyncio_lock_Lock___aenter__ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___aexit__
static const byte fun_data_asyncio_lock_Lock___aexit__[14] = {
    0xa8,0x44,0x0a, // prelude
    0x14,0x18,0x1a,0x1b,0x1c, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_lock_Lock___aexit__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_asyncio_lock_Lock___aexit__,
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
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_lock_Lock___aexit__ + 8,
        .line_info_top = fun_data_asyncio_lock_Lock___aexit__ + 8,
        .opcodes = fun_data_asyncio_lock_Lock___aexit__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_lock_Lock[] = {
    &raw_code_asyncio_lock_Lock___init__,
    &raw_code_asyncio_lock_Lock_locked,
    &raw_code_asyncio_lock_Lock_release,
    &raw_code_asyncio_lock_Lock_acquire,
    &raw_code_asyncio_lock_Lock___aenter__,
    &raw_code_asyncio_lock_Lock___aexit__,
};

static const mp_raw_code_t raw_code_asyncio_lock_Lock = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_lock_Lock,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = (void *)&children_asyncio_lock_Lock,
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
        .line_info = fun_data_asyncio_lock_Lock + 3,
        .line_info_top = fun_data_asyncio_lock_Lock + 3,
        .opcodes = fun_data_asyncio_lock_Lock + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_lock__lt_module_gt_[] = {
    &raw_code_asyncio_lock_Lock,
};

static const mp_raw_code_t raw_code_asyncio_lock__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_lock__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 26,
    #endif
    .children = (void *)&children_asyncio_lock__lt_module_gt_,
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
        .line_info = fun_data_asyncio_lock__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_lock__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_lock__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_lock[29] = {
    MP_QSTR_asyncio_slash_lock_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Lock,
    MP_QSTR___init__,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR_locked,
    MP_QSTR_release,
    MP_QSTR_peek,
    MP_QSTR_pop,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_acquire,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_CancelledError,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_RuntimeError,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_lock[1] = {
    MP_ROM_QSTR(MP_QSTR_Lock_space_not_space_acquired),
};

static const mp_frozen_module_t frozen_module_asyncio_lock = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_lock,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_lock,
    },
    .rc = &raw_code_asyncio_lock__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_stream
// - original source file: build-standard/tulip/obj/frozen_mpy/asyncio/stream.mpy
// - frozen file name: asyncio/stream.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/stream.py, scope asyncio_stream__lt_module_gt_
static const byte fun_data_asyncio_stream__lt_module_gt_[84] = {
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
    0x16,0x32, // STORE_NAME 'StreamReader'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x16,0x33, // STORE_NAME 'StreamWriter'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0a, // STORE_NAME 'open_connection'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x05, // LOAD_CONST_STRING 'Server'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'Server'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x14, // STORE_NAME 'start_server'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x1d, // STORE_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x13,0x06, // LOAD_ATTR 'wait_closed'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x07, // STORE_ATTR 'aclose'
    0x11,0x1d, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x08, // STORE_ATTR 'awrite'
    0x11,0x1d, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x09, // STORE_ATTR 'awritestr'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream
static const byte fun_data_asyncio_stream_Stream[70] = {
    0x08,0x02, // prelude
    0x04, // names: Stream
     // code info
    0x11,0x34, // LOAD_NAME '__name__'
    0x16,0x35, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Stream'
    0x16,0x36, // STORE_NAME '__qualname__'
    0x2c,0x00, // BUILD_MAP 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x20, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x24, // STORE_NAME 'get_extra_info'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x25, // STORE_NAME '__aenter__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x26, // STORE_NAME '__aexit__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x27, // STORE_NAME 'close'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x06, // STORE_NAME 'wait_closed'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x06, // MAKE_FUNCTION_DEFARGS 6
    0x16,0x28, // STORE_NAME 'read'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x2a, // STORE_NAME 'readinto'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x2b, // STORE_NAME 'readexactly'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x2c, // STORE_NAME 'readline'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x1e, // STORE_NAME 'write'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x1f, // STORE_NAME 'drain'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream___init__
static const byte fun_data_asyncio_stream_Stream___init__[22] = {
    0xa3,0x01,0x08, // prelude
    0x20,0x3c,0x21,0x22, // names: __init__, self, s, e
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x21, // STORE_ATTR 's'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x22, // STORE_ATTR 'e'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb0, // LOAD_FAST 0
    0x18,0x23, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_asyncio_stream_Stream___init__,
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
        .qstr_block_name_idx = 32,
        .line_info = fun_data_asyncio_stream_Stream___init__ + 7,
        .line_info_top = fun_data_asyncio_stream_Stream___init__ + 7,
        .opcodes = fun_data_asyncio_stream_Stream___init__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_get_extra_info
static const byte fun_data_asyncio_stream_Stream_get_extra_info[11] = {
    0x1a,0x06, // prelude
    0x24,0x3c,0x42, // names: get_extra_info, self, v
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x22, // LOAD_ATTR 'e'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_get_extra_info = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_Stream_get_extra_info,
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
        .qstr_block_name_idx = 36,
        .line_info = fun_data_asyncio_stream_Stream_get_extra_info + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_get_extra_info + 5,
        .opcodes = fun_data_asyncio_stream_Stream_get_extra_info + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream___aenter__
static const byte fun_data_asyncio_stream_Stream___aenter__[7] = {
    0x89,0x40,0x04, // prelude
    0x25,0x3c, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream___aenter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Stream___aenter__,
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
        .qstr_block_name_idx = 37,
        .line_info = fun_data_asyncio_stream_Stream___aenter__ + 5,
        .line_info_top = fun_data_asyncio_stream_Stream___aenter__ + 5,
        .opcodes = fun_data_asyncio_stream_Stream___aenter__ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream___aexit__
static const byte fun_data_asyncio_stream_Stream___aexit__[19] = {
    0xa8,0x44,0x0a, // prelude
    0x26,0x3c,0x43,0x44,0x45, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x27, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream___aexit__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_asyncio_stream_Stream___aexit__,
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
        .qstr_block_name_idx = 38,
        .line_info = fun_data_asyncio_stream_Stream___aexit__ + 8,
        .line_info_top = fun_data_asyncio_stream_Stream___aexit__ + 8,
        .opcodes = fun_data_asyncio_stream_Stream___aexit__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_close
static const byte fun_data_asyncio_stream_Stream_close[6] = {
    0x09,0x04, // prelude
    0x27,0x3c, // names: close, self
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Stream_close,
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
        .line_info = fun_data_asyncio_stream_Stream_close + 4,
        .line_info_top = fun_data_asyncio_stream_Stream_close + 4,
        .opcodes = fun_data_asyncio_stream_Stream_close + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_wait_closed
static const byte fun_data_asyncio_stream_Stream_wait_closed[15] = {
    0x91,0x40,0x04, // prelude
    0x06,0x3c, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x27, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_wait_closed = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Stream_wait_closed,
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
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_stream_Stream_wait_closed + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_wait_closed + 5,
        .opcodes = fun_data_asyncio_stream_Stream_wait_closed + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_read
static const byte fun_data_asyncio_stream_Stream_read[61] = {
    0xb2,0x41,0x06, // prelude
    0x28,0x3c,0x46, // names: read, self, n
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc2, // STORE_FAST 2
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x29, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x28, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0x42,0x0e, // JUMP -50
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_Stream_read,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 61,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_asyncio_stream_Stream_read + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_read + 6,
        .opcodes = fun_data_asyncio_stream_Stream_read + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readinto
static const byte fun_data_asyncio_stream_Stream_readinto[28] = {
    0xa2,0x40,0x06, // prelude
    0x2a,0x3c,0x3d, // names: readinto, self, buf
     // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x29, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x2a, // LOAD_METHOD 'readinto'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_readinto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_Stream_readinto,
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
        .qstr_block_name_idx = 42,
        .line_info = fun_data_asyncio_stream_Stream_readinto + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_readinto + 6,
        .opcodes = fun_data_asyncio_stream_Stream_readinto + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readexactly
static const byte fun_data_asyncio_stream_Stream_readexactly[66] = {
    0xb2,0x40,0x06, // prelude
    0x2b,0x3c,0x46, // names: readexactly, self, n
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc2, // STORE_FAST 2
    0x42,0x72, // JUMP 50
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x29, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x28, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x47, // LOAD_GLOBAL 'EOFError'
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xe6, // BINARY_OP 15 __isub__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x0b, // POP_JUMP_IF_TRUE -53
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_readexactly = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_Stream_readexactly,
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
        .qstr_block_name_idx = 43,
        .line_info = fun_data_asyncio_stream_Stream_readexactly + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_readexactly + 6,
        .opcodes = fun_data_asyncio_stream_Stream_readexactly + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readline
static const byte fun_data_asyncio_stream_Stream_readline[49] = {
    0xa9,0x40,0x04, // prelude
    0x2c,0x3c, // names: readline, self
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x29, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x2c, // LOAD_METHOD 'readline'
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
static const mp_raw_code_t raw_code_asyncio_stream_Stream_readline = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Stream_readline,
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
        .qstr_block_name_idx = 44,
        .line_info = fun_data_asyncio_stream_Stream_readline + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_readline + 5,
        .opcodes = fun_data_asyncio_stream_Stream_readline + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_write
static const byte fun_data_asyncio_stream_Stream_write[54] = {
    0x2a,0x06, // prelude
    0x1e,0x3c,0x3d, // names: write, self, buf
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x23, // LOAD_ATTR 'out_buf'
    0x43,0x61, // POP_JUMP_IF_TRUE 33
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x1e, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x23, // LOAD_ATTR 'out_buf'
    0xb1, // LOAD_FAST 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x23, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_write = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_Stream_write,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 54,
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
        .qstr_block_name_idx = 30,
        .line_info = fun_data_asyncio_stream_Stream_write + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_write + 5,
        .opcodes = fun_data_asyncio_stream_Stream_write + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_drain
static const byte fun_data_asyncio_stream_Stream_drain[86] = {
    0xc1,0x40,0x04, // prelude
    0x1f,0x3c, // names: drain, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x23, // LOAD_ATTR 'out_buf'
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x2d, // LOAD_METHOD 'sleep_ms'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
    0x12,0x40, // LOAD_GLOBAL 'memoryview'
    0xb0, // LOAD_FAST 0
    0x13,0x23, // LOAD_ATTR 'out_buf'
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x42,0x65, // JUMP 37
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x13, // LOAD_METHOD 'queue_write'
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x21, // LOAD_ATTR 's'
    0x14,0x1e, // LOAD_METHOD 'write'
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
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x12, // POP_JUMP_IF_TRUE -46
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb0, // LOAD_FAST 0
    0x18,0x23, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Stream_drain = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Stream_drain,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 86,
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
        .qstr_block_name_idx = 31,
        .line_info = fun_data_asyncio_stream_Stream_drain + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_drain + 5,
        .opcodes = fun_data_asyncio_stream_Stream_drain + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_stream_Stream[] = {
    &raw_code_asyncio_stream_Stream___init__,
    &raw_code_asyncio_stream_Stream_get_extra_info,
    &raw_code_asyncio_stream_Stream___aenter__,
    &raw_code_asyncio_stream_Stream___aexit__,
    &raw_code_asyncio_stream_Stream_close,
    &raw_code_asyncio_stream_Stream_wait_closed,
    &raw_code_asyncio_stream_Stream_read,
    &raw_code_asyncio_stream_Stream_readinto,
    &raw_code_asyncio_stream_Stream_readexactly,
    &raw_code_asyncio_stream_Stream_readline,
    &raw_code_asyncio_stream_Stream_write,
    &raw_code_asyncio_stream_Stream_drain,
};

static const mp_raw_code_t raw_code_asyncio_stream_Stream = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_stream_Stream,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 70,
    #endif
    .children = (void *)&children_asyncio_stream_Stream,
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
        .line_info = fun_data_asyncio_stream_Stream + 3,
        .line_info_top = fun_data_asyncio_stream_Stream + 3,
        .opcodes = fun_data_asyncio_stream_Stream + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_open_connection
static const byte fun_data_asyncio_stream_open_connection[120] = {
    0xea,0x42,0x06, // prelude
    0x0a,0x37,0x38, // names: open_connection, host, port
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0b, // LOAD_CONST_STRING 'EINPROGRESS'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x0c, // IMPORT_NAME 'errno'
    0x1c,0x0b, // IMPORT_FROM 'EINPROGRESS'
    0xc2, // STORE_FAST 2
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0d, // IMPORT_NAME 'socket'
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x0e, // LOAD_METHOD 'getaddrinfo'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb3, // LOAD_FAST 3
    0x13,0x0f, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x14,0x0d, // LOAD_METHOD 'socket'
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
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0xc6, // STORE_FAST 6
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb5, // LOAD_FAST 5
    0x14,0x11, // LOAD_METHOD 'connect'
    0xb4, // LOAD_FAST 4
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x1b, // POP_EXCEPT_JUMP 27
    0x57, // DUP_TOP
    0x12,0x39, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xc7, // STORE_FAST 7
    0x49,0x0a, // SETUP_FINALLY 10
    0xb7, // LOAD_FAST 7
    0x13,0x0c, // LOAD_ATTR 'errno'
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
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x13, // LOAD_METHOD 'queue_write'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_open_connection = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_asyncio_stream_open_connection,
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
        .qstr_block_name_idx = 10,
        .line_info = fun_data_asyncio_stream_open_connection + 6,
        .line_info_top = fun_data_asyncio_stream_open_connection + 6,
        .opcodes = fun_data_asyncio_stream_open_connection + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server
static const byte fun_data_asyncio_stream_Server[33] = {
    0x00,0x02, // prelude
    0x05, // names: Server
     // code info
    0x11,0x34, // LOAD_NAME '__name__'
    0x16,0x35, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'Server'
    0x16,0x36, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x25, // STORE_NAME '__aenter__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x26, // STORE_NAME '__aexit__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x27, // STORE_NAME 'close'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x06, // STORE_NAME 'wait_closed'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x1b, // STORE_NAME '_serve'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server___aenter__
static const byte fun_data_asyncio_stream_Server___aenter__[7] = {
    0x89,0x40,0x04, // prelude
    0x25,0x3c, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Server___aenter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Server___aenter__,
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
        .qstr_block_name_idx = 37,
        .line_info = fun_data_asyncio_stream_Server___aenter__ + 5,
        .line_info_top = fun_data_asyncio_stream_Server___aenter__ + 5,
        .opcodes = fun_data_asyncio_stream_Server___aenter__ + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server___aexit__
static const byte fun_data_asyncio_stream_Server___aexit__[25] = {
    0xa8,0x44,0x0a, // prelude
    0x26,0x3c,0x43,0x44,0x45, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x27, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x06, // LOAD_METHOD 'wait_closed'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Server___aexit__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_asyncio_stream_Server___aexit__,
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
        .qstr_block_name_idx = 38,
        .line_info = fun_data_asyncio_stream_Server___aexit__ + 8,
        .line_info_top = fun_data_asyncio_stream_Server___aexit__ + 8,
        .opcodes = fun_data_asyncio_stream_Server___aexit__ + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server_close
static const byte fun_data_asyncio_stream_Server_close[14] = {
    0x11,0x04, // prelude
    0x27,0x3c, // names: close, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'task'
    0x14,0x2e, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Server_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Server_close,
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
        .qstr_block_name_idx = 39,
        .line_info = fun_data_asyncio_stream_Server_close + 4,
        .line_info_top = fun_data_asyncio_stream_Server_close + 4,
        .opcodes = fun_data_asyncio_stream_Server_close + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server_wait_closed
static const byte fun_data_asyncio_stream_Server_wait_closed[14] = {
    0x91,0x40,0x04, // prelude
    0x06,0x3c, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'task'
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Server_wait_closed = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 1,
    .fun_data = fun_data_asyncio_stream_Server_wait_closed,
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
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_stream_Server_wait_closed + 5,
        .line_info_top = fun_data_asyncio_stream_Server_wait_closed + 5,
        .opcodes = fun_data_asyncio_stream_Server_wait_closed + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server__serve
static const byte fun_data_asyncio_stream_Server__serve[94] = {
    0xdf,0x40,0x08, // prelude
    0x1b,0x3c,0x21,0x3a, // names: _serve, self, s, cb
     // code info
    0x48,0x0d, // SETUP_EXCEPT 13
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR '_io_queue'
    0x14,0x29, // LOAD_METHOD 'queue_read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x12, // POP_EXCEPT_JUMP 18
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x2f, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x27, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb1, // LOAD_FAST 1
    0x14,0x30, // LOAD_METHOD 'accept'
    0x36,0x00, // CALL_METHOD 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0x4a,0x05, // POP_EXCEPT_JUMP 5
    0x59, // POP_TOP
    0x40,0x61,0x01, // UNWIND_JUMP 33
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb3, // LOAD_FAST 3
    0x2c,0x01, // BUILD_MAP 1
    0xb4, // LOAD_FAST 4
    0x10,0x31, // LOAD_CONST_STRING 'peername'
    0x62, // STORE_MAP
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x1a, // LOAD_METHOD 'create_task'
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xb5, // LOAD_FAST 5
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xab,0x7f, // JUMP -85
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_Server__serve = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 3,
    .fun_data = fun_data_asyncio_stream_Server__serve,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 94,
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
        .qstr_block_name_idx = 27,
        .line_info = fun_data_asyncio_stream_Server__serve + 7,
        .line_info_top = fun_data_asyncio_stream_Server__serve + 7,
        .opcodes = fun_data_asyncio_stream_Server__serve + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_stream_Server[] = {
    &raw_code_asyncio_stream_Server___aenter__,
    &raw_code_asyncio_stream_Server___aexit__,
    &raw_code_asyncio_stream_Server_close,
    &raw_code_asyncio_stream_Server_wait_closed,
    &raw_code_asyncio_stream_Server__serve,
};

static const mp_raw_code_t raw_code_asyncio_stream_Server = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_stream_Server,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_asyncio_stream_Server,
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
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_stream_Server + 3,
        .line_info_top = fun_data_asyncio_stream_Server + 3,
        .opcodes = fun_data_asyncio_stream_Server + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_start_server
static const byte fun_data_asyncio_stream_start_server[88] = {
    0xe0,0x45,0x0a, // prelude
    0x14,0x3a,0x37,0x38,0x3b, // names: start_server, cb, host, port, backlog
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0d, // IMPORT_NAME 'socket'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x0e, // LOAD_METHOD 'getaddrinfo'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb4, // LOAD_FAST 4
    0x14,0x0d, // LOAD_METHOD 'socket'
    0x36,0x00, // CALL_METHOD 0
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x15, // LOAD_METHOD 'setsockopt'
    0xb4, // LOAD_FAST 4
    0x13,0x16, // LOAD_ATTR 'SOL_SOCKET'
    0xb4, // LOAD_FAST 4
    0x13,0x17, // LOAD_ATTR 'SO_REUSEADDR'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x18, // LOAD_METHOD 'bind'
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x19, // LOAD_METHOD 'listen'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x05, // LOAD_GLOBAL 'Server'
    0x34,0x00, // CALL_FUNCTION 0
    0xc6, // STORE_FAST 6
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x1a, // LOAD_METHOD 'create_task'
    0xb6, // LOAD_FAST 6
    0x14,0x1b, // LOAD_METHOD '_serve'
    0xb5, // LOAD_FAST 5
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0x36,0x01, // CALL_METHOD 1
    0xb6, // LOAD_FAST 6
    0x18,0x1c, // STORE_ATTR 'task'
    0xb6, // LOAD_FAST 6
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_start_server = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_asyncio_stream_start_server,
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
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_stream_start_server + 8,
        .line_info_top = fun_data_asyncio_stream_start_server + 8,
        .opcodes = fun_data_asyncio_stream_start_server + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_stream_awrite
static const byte fun_data_asyncio_stream_stream_awrite[63] = {
    0xb8,0xc4,0x01,0x0a, // prelude
    0x1d,0x3c,0x3d,0x3e,0x3f, // names: stream_awrite, self, buf, off, sz
     // code info
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x12,0x40, // LOAD_GLOBAL 'memoryview'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x41, // LOAD_GLOBAL 'len'
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
    0x14,0x1e, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x1f, // LOAD_METHOD 'drain'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_asyncio_stream_stream_awrite = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 4,
    .fun_data = fun_data_asyncio_stream_stream_awrite,
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
        .qstr_block_name_idx = 29,
        .line_info = fun_data_asyncio_stream_stream_awrite + 9,
        .line_info_top = fun_data_asyncio_stream_stream_awrite + 9,
        .opcodes = fun_data_asyncio_stream_stream_awrite + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_asyncio_stream__lt_module_gt_[] = {
    &raw_code_asyncio_stream_Stream,
    &raw_code_asyncio_stream_open_connection,
    &raw_code_asyncio_stream_Server,
    &raw_code_asyncio_stream_start_server,
    &raw_code_asyncio_stream_stream_awrite,
};

static const mp_raw_code_t raw_code_asyncio_stream__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_asyncio_stream__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 84,
    #endif
    .children = (void *)&children_asyncio_stream__lt_module_gt_,
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
        .line_info = fun_data_asyncio_stream__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_stream__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_stream__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_stream[72] = {
    MP_QSTR_asyncio_slash_stream_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Stream,
    MP_QSTR_Server,
    MP_QSTR_wait_closed,
    MP_QSTR_aclose,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR_open_connection,
    MP_QSTR_EINPROGRESS,
    MP_QSTR_errno,
    MP_QSTR_socket,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_setblocking,
    MP_QSTR_connect,
    MP_QSTR__io_queue,
    MP_QSTR_queue_write,
    MP_QSTR_start_server,
    MP_QSTR_setsockopt,
    MP_QSTR_SOL_SOCKET,
    MP_QSTR_SO_REUSEADDR,
    MP_QSTR_bind,
    MP_QSTR_listen,
    MP_QSTR_create_task,
    MP_QSTR__serve,
    MP_QSTR_task,
    MP_QSTR_stream_awrite,
    MP_QSTR_write,
    MP_QSTR_drain,
    MP_QSTR___init__,
    MP_QSTR_s,
    MP_QSTR_e,
    MP_QSTR_out_buf,
    MP_QSTR_get_extra_info,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR_close,
    MP_QSTR_read,
    MP_QSTR_queue_read,
    MP_QSTR_readinto,
    MP_QSTR_readexactly,
    MP_QSTR_readline,
    MP_QSTR_sleep_ms,
    MP_QSTR_cancel,
    MP_QSTR_CancelledError,
    MP_QSTR_accept,
    MP_QSTR_peername,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_host,
    MP_QSTR_port,
    MP_QSTR_OSError,
    MP_QSTR_cb,
    MP_QSTR_backlog,
    MP_QSTR_self,
    MP_QSTR_buf,
    MP_QSTR_off,
    MP_QSTR_sz,
    MP_QSTR_memoryview,
    MP_QSTR_len,
    MP_QSTR_v,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
    MP_QSTR_n,
    MP_QSTR_EOFError,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_stream[1] = {
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
};

static const mp_frozen_module_t frozen_module_asyncio_stream = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_stream,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_stream,
    },
    .rc = &raw_code_asyncio_stream__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio
// - original source file: build-standard/tulip/obj/frozen_mpy/uasyncio.mpy
// - frozen file name: uasyncio.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file uasyncio.py, scope uasyncio__lt_module_gt_
static const byte fun_data_uasyncio__lt_module_gt_[9] = {
    0x00,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x02, // STORE_NAME '__getattr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio__lt_module_gt_
// frozen bytecode for file uasyncio.py, scope uasyncio___getattr__
static const byte fun_data_uasyncio___getattr__[16] = {
    0x21,0x04, // prelude
    0x02,0x04, // names: __getattr__, attr
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'asyncio'
    0xc1, // STORE_FAST 1
    0x12,0x05, // LOAD_GLOBAL 'getattr'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_uasyncio___getattr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_uasyncio___getattr__,
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
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 2,
        .line_info = fun_data_uasyncio___getattr__ + 4,
        .line_info_top = fun_data_uasyncio___getattr__ + 4,
        .opcodes = fun_data_uasyncio___getattr__ + 4,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_uasyncio__lt_module_gt_[] = {
    &raw_code_uasyncio___getattr__,
};

static const mp_raw_code_t raw_code_uasyncio__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_uasyncio__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 9,
    #endif
    .children = (void *)&children_uasyncio__lt_module_gt_,
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
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio__lt_module_gt_ + 3,
        .line_info_top = fun_data_uasyncio__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio__lt_module_gt_ + 3,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio[6] = {
    MP_QSTR_uasyncio_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR___getattr__,
    MP_QSTR_asyncio,
    MP_QSTR_attr,
    MP_QSTR_getattr,
};

static const mp_frozen_module_t frozen_module_uasyncio = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio,
        .obj_table = NULL,
    },
    .rc = &raw_code_uasyncio__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module world
// - original source file: build-standard/tulip/obj/frozen_mpy/world.mpy
// - frozen file name: world.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file world.py, scope world__lt_module_gt_
static const byte fun_data_world__lt_module_gt_[233] = {
    0x10,0x56, // prelude
    0x01, // names: <module>
    0x60,0x26,0x26,0x26,0x26,0x26,0x66,0x24,0x24,0x27,0x27,0x27,0x63,0x89,0x11,0x84,0x08,0x84,0x0f,0x64,0x20,0x64,0x69,0x64,0x8a,0x08,0x8b,0x19,0x8c,0x0b,0x88,0x11,0x8d,0x22,0x64,0x60,0x8a,0x12,0x84,0x09,0x84,0x08, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'os'
    0x16,0x02, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'ubinascii'
    0x16,0x64, // STORE_NAME 'ub'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'urequests'
    0x16,0x04, // STORE_NAME 'urequests'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'json'
    0x16,0x05, // STORE_NAME 'json'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'tulip'
    0x16,0x06, // STORE_NAME 'tulip'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'time'
    0x16,0x07, // STORE_NAME 'time'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x65, // STORE_NAME 'world_token'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x16,0x66, // STORE_NAME 'host'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x11,0x66, // LOAD_NAME 'host'
    0xf8, // BINARY_OP 33 __mod__
    0x16,0x67, // STORE_NAME 'room_id'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x11,0x66, // LOAD_NAME 'host'
    0xf8, // BINARY_OP 33 __mod__
    0x16,0x68, // STORE_NAME 'files_room_id'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x11,0x66, // LOAD_NAME 'host'
    0xf8, // BINARY_OP 33 __mod__
    0x16,0x69, // STORE_NAME 'firmware_room_id'
    0x51, // LOAD_CONST_NONE
    0x17,0x6a, // STORE_GLOBAL 'last_message'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x08, // LOAD_CONST_STRING 'UUID'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'UUID'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'uuid4'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'nice_time'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x14, // STORE_NAME 'matrix_put'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x1c, // STORE_NAME 'matrix_get'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x1e, // STORE_NAME 'matrix_post'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x20, // STORE_NAME '_isdir'
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x22, // STORE_NAME 'read_in_chunks'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x11,0x68, // LOAD_NAME 'files_room_id'
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x24, // STORE_NAME 'upload'
    0x22,0xa7,0x08, // LOAD_CONST_SMALL_INT 5000
    0x11,0x68, // LOAD_NAME 'files_room_id'
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x09, // MAKE_FUNCTION_DEFARGS 9
    0x16,0x33, // STORE_NAME 'files'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0a, // MAKE_FUNCTION_DEFARGS 10
    0x16,0x3c, // STORE_NAME 'ls'
    0x22,0xa7,0x08, // LOAD_CONST_SMALL_INT 5000
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0b, // MAKE_FUNCTION_DEFARGS 11
    0x16,0x41, // STORE_NAME 'download'
    0x32,0x0c, // MAKE_FUNCTION 12
    0x16,0x49, // STORE_NAME 'send'
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0d, // MAKE_FUNCTION_DEFARGS 13
    0x16,0x4b, // STORE_NAME 'check'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x4e, // STORE_NAME 'put_message'
    0x32,0x0f, // MAKE_FUNCTION 15
    0x16,0x51, // STORE_NAME 'world_ui'
    0x32,0x10, // MAKE_FUNCTION 16
    0x16,0x57, // STORE_NAME 'world'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_UUID
static const byte fun_data_world_UUID[41] = {
    0x08,0x12, // prelude
    0x08, // names: UUID
    0x88,0x14,0x64,0x40,0x68,0x20,0x64,0x20, // code info
    0x11,0x6b, // LOAD_NAME '__name__'
    0x16,0x6c, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'UUID'
    0x16,0x6d, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x5a, // STORE_NAME '__init__'
    0x11,0x6e, // LOAD_NAME 'property'
    0x32,0x01, // MAKE_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x5c, // STORE_NAME 'hex'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x5f, // STORE_NAME '__str__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x62, // STORE_NAME '__repr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of world_UUID
// frozen bytecode for file world.py, scope world_UUID___init__
static const byte fun_data_world_UUID___init__[31] = {
    0x1a,0x0e, // prelude
    0x5a,0x7a,0x0b, // names: __init__, self, bytes
    0x80,0x15,0x29,0x27, // code info
    0x12,0x79, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x90, // LOAD_CONST_SMALL_INT 16
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x7b, // LOAD_GLOBAL 'ValueError'
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x5b, // STORE_ATTR '_bytes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_UUID___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_world_UUID___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 31,
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
        .qstr_block_name_idx = 90,
        .line_info = fun_data_world_UUID___init__ + 5,
        .line_info_top = fun_data_world_UUID___init__ + 9,
        .opcodes = fun_data_world_UUID___init__ + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world_UUID
// frozen bytecode for file world.py, scope world_UUID_hex
static const byte fun_data_world_UUID_hex[20] = {
    0x19,0x08, // prelude
    0x5c,0x7a, // names: hex, self
    0x80,0x1b, // code info
    0x12,0x64, // LOAD_GLOBAL 'ub'
    0x14,0x5d, // LOAD_METHOD 'hexlify'
    0xb0, // LOAD_FAST 0
    0x13,0x5b, // LOAD_ATTR '_bytes'
    0x36,0x01, // CALL_METHOD 1
    0x14,0x5e, // LOAD_METHOD 'decode'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_UUID_hex = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_UUID_hex,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
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
        .qstr_block_name_idx = 92,
        .line_info = fun_data_world_UUID_hex + 4,
        .line_info_top = fun_data_world_UUID_hex + 6,
        .opcodes = fun_data_world_UUID_hex + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world_UUID
// frozen bytecode for file world.py, scope world_UUID___str__
static const byte fun_data_world_UUID___str__[50] = {
    0x51,0x0a, // prelude
    0x5f,0x7a, // names: __str__, self
    0x80,0x1e,0x24, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x5c, // LOAD_ATTR 'hex'
    0xc1, // STORE_FAST 1
    0x10,0x60, // LOAD_CONST_STRING '-'
    0x14,0x61, // LOAD_METHOD 'join'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x88, // LOAD_CONST_SMALL_INT 8
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x88, // LOAD_CONST_SMALL_INT 8
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x90, // LOAD_CONST_SMALL_INT 16
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x90, // LOAD_CONST_SMALL_INT 16
    0x94, // LOAD_CONST_SMALL_INT 20
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x94, // LOAD_CONST_SMALL_INT 20
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x2a,0x05, // BUILD_TUPLE 5
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_UUID___str__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_UUID___str__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 50,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 95,
        .line_info = fun_data_world_UUID___str__ + 4,
        .line_info_top = fun_data_world_UUID___str__ + 7,
        .opcodes = fun_data_world_UUID___str__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world_UUID
// frozen bytecode for file world.py, scope world_UUID___repr__
static const byte fun_data_world_UUID___repr__[15] = {
    0x19,0x08, // prelude
    0x62,0x7a, // names: __repr__, self
    0x80,0x22, // code info
    0x10,0x63, // LOAD_CONST_STRING '<UUID: %s>'
    0x12,0x73, // LOAD_GLOBAL 'str'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_UUID___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_UUID___repr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
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
        .qstr_block_name_idx = 98,
        .line_info = fun_data_world_UUID___repr__ + 4,
        .line_info_top = fun_data_world_UUID___repr__ + 6,
        .opcodes = fun_data_world_UUID___repr__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_world_UUID[] = {
    &raw_code_world_UUID___init__,
    &raw_code_world_UUID_hex,
    &raw_code_world_UUID___str__,
    &raw_code_world_UUID___repr__,
};

static const mp_raw_code_t raw_code_world_UUID = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_world_UUID,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = (void *)&children_world_UUID,
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
        .qstr_block_name_idx = 8,
        .line_info = fun_data_world_UUID + 3,
        .line_info_top = fun_data_world_UUID + 11,
        .opcodes = fun_data_world_UUID + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_uuid4
static const byte fun_data_world_uuid4[55] = {
    0x20,0x0e, // prelude
    0x09, // names: uuid4
    0x80,0x25,0x20,0x2c,0x2c,0x2d, // code info
    0x12,0x6f, // LOAD_GLOBAL 'bytearray'
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x0a, // LOAD_METHOD 'urandom'
    0x90, // LOAD_CONST_SMALL_INT 16
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x8f, // LOAD_CONST_SMALL_INT 15
    0xef, // BINARY_OP 24 __and__
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0xed, // BINARY_OP 22 __or__
    0xb0, // LOAD_FAST 0
    0x86, // LOAD_CONST_SMALL_INT 6
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x88, // LOAD_CONST_SMALL_INT 8
    0x55, // LOAD_SUBSCR
    0x22,0x3f, // LOAD_CONST_SMALL_INT 63
    0xef, // BINARY_OP 24 __and__
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xed, // BINARY_OP 22 __or__
    0xb0, // LOAD_FAST 0
    0x88, // LOAD_CONST_SMALL_INT 8
    0x56, // STORE_SUBSCR
    0x12,0x08, // LOAD_GLOBAL 'UUID'
    0x10,0x0b, // LOAD_CONST_STRING 'bytes'
    0xb0, // LOAD_FAST 0
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_uuid4 = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_world_uuid4,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 55,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_world_uuid4 + 3,
        .line_info_top = fun_data_world_uuid4 + 9,
        .opcodes = fun_data_world_uuid4 + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_nice_time
static const byte fun_data_world_nice_time[105] = {
    0x21,0x1c, // prelude
    0x0c,0x4d, // names: nice_time, age_s
    0x80,0x2d,0x26,0x28,0x27,0x2a,0x28,0x2d,0x29,0x4f,0x23,0x25, // code info
    0xb0, // LOAD_FAST 0
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x10,0x0d, // LOAD_CONST_STRING '%ds'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x42,0xc1,0x80, // JUMP 65
    0xb0, // LOAD_FAST 0
    0x22,0x9c,0x10, // LOAD_CONST_SMALL_INT 3600
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x10,0x0e, // LOAD_CONST_STRING '%dm'
    0xb0, // LOAD_FAST 0
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x42,0x70, // JUMP 48
    0xb0, // LOAD_FAST 0
    0x22,0x85,0xa3,0x00, // LOAD_CONST_SMALL_INT 86400
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x10,0x0f, // LOAD_CONST_STRING '%dh'
    0xb0, // LOAD_FAST 0
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x42,0x5b, // JUMP 27
    0xb0, // LOAD_FAST 0
    0x22,0x8f,0x84,0xe7,0x00, // LOAD_CONST_SMALL_INT 31536000
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x10,0x10, // LOAD_CONST_STRING '%dd'
    0xb0, // LOAD_FAST 0
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0x98, // LOAD_CONST_SMALL_INT 24
    0xf7, // BINARY_OP 32 __truediv__
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x42,0x43, // JUMP 3
    0x10,0x11, // LOAD_CONST_STRING '~'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x12, // LOAD_CONST_STRING ' ago'
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0x10,0x13, // LOAD_CONST_STRING '% 8s'
    0xb1, // LOAD_FAST 1
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_nice_time = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_nice_time,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 105,
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
        .qstr_block_name_idx = 12,
        .line_info = fun_data_world_nice_time + 4,
        .line_info_top = fun_data_world_nice_time + 16,
        .opcodes = fun_data_world_nice_time + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_matrix_put
static const byte fun_data_world_matrix_put[47] = {
    0x52,0x0a, // prelude
    0x14,0x31,0x16, // names: matrix_put, url, data
    0x80,0x3c, // code info
    0x12,0x04, // LOAD_GLOBAL 'urequests'
    0x14,0x15, // LOAD_METHOD 'put'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING 'data'
    0x12,0x0b, // LOAD_GLOBAL 'bytes'
    0x12,0x05, // LOAD_GLOBAL 'json'
    0x14,0x17, // LOAD_METHOD 'dumps'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x14,0x18, // LOAD_METHOD 'encode'
    0x10,0x19, // LOAD_CONST_STRING 'utf-8'
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x1a, // LOAD_CONST_STRING 'headers'
    0x2c,0x01, // BUILD_MAP 1
    0x10,0x1b, // LOAD_CONST_STRING 'Bearer %s'
    0x12,0x65, // LOAD_GLOBAL 'world_token'
    0xf8, // BINARY_OP 33 __mod__
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0x36,0x84,0x01, // CALL_METHOD 513
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_matrix_put = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_world_matrix_put,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 47,
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
        .line_info = fun_data_world_matrix_put + 5,
        .line_info_top = fun_data_world_matrix_put + 7,
        .opcodes = fun_data_world_matrix_put + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_matrix_get
static const byte fun_data_world_matrix_get[27] = {
    0x39,0x08, // prelude
    0x1c,0x31, // names: matrix_get, url
    0x80,0x40, // code info
    0x12,0x04, // LOAD_GLOBAL 'urequests'
    0x14,0x1d, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x10,0x1a, // LOAD_CONST_STRING 'headers'
    0x2c,0x01, // BUILD_MAP 1
    0x10,0x1b, // LOAD_CONST_STRING 'Bearer %s'
    0x12,0x65, // LOAD_GLOBAL 'world_token'
    0xf8, // BINARY_OP 33 __mod__
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0x36,0x82,0x01, // CALL_METHOD 257
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_matrix_get = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_matrix_get,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
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
        .qstr_block_name_idx = 28,
        .line_info = fun_data_world_matrix_get + 4,
        .line_info_top = fun_data_world_matrix_get + 6,
        .opcodes = fun_data_world_matrix_get + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_matrix_post
static const byte fun_data_world_matrix_post[37] = {
    0xdb,0x01,0x0c, // prelude
    0x1e,0x31,0x16,0x2a, // names: matrix_post, url, data, content_type
    0x80,0x43, // code info
    0x12,0x04, // LOAD_GLOBAL 'urequests'
    0x14,0x1f, // LOAD_METHOD 'post'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING 'data'
    0xb1, // LOAD_FAST 1
    0x10,0x1a, // LOAD_CONST_STRING 'headers'
    0x2c,0x02, // BUILD_MAP 2
    0x10,0x1b, // LOAD_CONST_STRING 'Bearer %s'
    0x12,0x65, // LOAD_GLOBAL 'world_token'
    0xf8, // BINARY_OP 33 __mod__
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0xb2, // LOAD_FAST 2
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x62, // STORE_MAP
    0x36,0x84,0x01, // CALL_METHOD 513
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_matrix_post = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_world_matrix_post,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 30,
        .line_info = fun_data_world_matrix_post + 7,
        .line_info_top = fun_data_world_matrix_post + 9,
        .opcodes = fun_data_world_matrix_post + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world__isdir
static const byte fun_data_world__isdir[23] = {
    0x19,0x08, // prelude
    0x20,0x3b, // names: _isdir, filename
    0x80,0x46, // code info
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x21, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world__isdir = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world__isdir,
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
        .qstr_block_name_idx = 32,
        .line_info = fun_data_world__isdir + 4,
        .line_info_top = fun_data_world__isdir + 6,
        .opcodes = fun_data_world__isdir + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_read_in_chunks
static const byte fun_data_world_read_in_chunks[31] = {
    0xaa,0x41,0x12, // prelude
    0x22,0x70,0x45, // names: read_in_chunks, file_object, chunk_size
    0x80,0x49,0x20,0x27,0x23,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x23, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x45, // JUMP 5
    0xb2, // LOAD_FAST 2
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x2f, // JUMP -17
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_read_in_chunks = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_world_read_in_chunks,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 31,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_world_read_in_chunks + 6,
        .line_info_top = fun_data_world_read_in_chunks + 12,
        .opcodes = fun_data_world_read_in_chunks + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_upload
static const byte fun_data_world_upload[186] = {
    0xeb,0x80,0x01,0x2e, // prelude
    0x24,0x3b,0x2a,0x67, // names: upload, filename, content_type, room_id
    0x80,0x51,0x22,0x29,0x27,0x27,0x22,0x29,0x28,0x65,0x20,0x28,0x56,0x47,0x39,0x33,0x27,0x29,0x23, // code info
    0x50, // LOAD_CONST_FALSE
    0xc3, // STORE_FAST 3
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x12,0x66, // LOAD_GLOBAL 'host'
    0xb0, // LOAD_FAST 0
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0xc4, // STORE_FAST 4
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x25, // LOAD_METHOD 'display_stop'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x20, // LOAD_GLOBAL '_isdir'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0x52, // LOAD_CONST_TRUE
    0xc3, // STORE_FAST 3
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x10,0x26, // LOAD_CONST_STRING 'Packing %s'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x27, // LOAD_METHOD 'tar_create'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x10,0x28, // LOAD_CONST_STRING '.tar'
    0xe5, // BINARY_OP 14 __iadd__
    0xc0, // STORE_FAST 0
    0x12,0x72, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x29, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0x12,0x1e, // LOAD_GLOBAL 'matrix_post'
    0xb4, // LOAD_FAST 4
    0x12,0x22, // LOAD_GLOBAL 'read_in_chunks'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x2a, // LOAD_CONST_STRING 'content_type'
    0xb1, // LOAD_FAST 1
    0x34,0x82,0x02, // CALL_FUNCTION 258
    0x14,0x05, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x2b, // LOAD_METHOD 'display_start'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x2c,0x04, // BUILD_MAP 4
    0x2c,0x01, // BUILD_MAP 1
    0xb1, // LOAD_FAST 1
    0x10,0x2c, // LOAD_CONST_STRING 'mimetype'
    0x62, // STORE_MAP
    0x10,0x2d, // LOAD_CONST_STRING 'info'
    0x62, // STORE_MAP
    0x10,0x2e, // LOAD_CONST_STRING 'm.file'
    0x10,0x2f, // LOAD_CONST_STRING 'msgtype'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x62, // STORE_MAP
    0xb6, // LOAD_FAST 6
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0x62, // STORE_MAP
    0xc7, // STORE_FAST 7
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x12,0x66, // LOAD_GLOBAL 'host'
    0xb2, // LOAD_FAST 2
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x12,0x73, // LOAD_GLOBAL 'str'
    0x12,0x09, // LOAD_GLOBAL 'uuid4'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x04, // BUILD_TUPLE 4
    0xf8, // BINARY_OP 33 __mod__
    0xc4, // STORE_FAST 4
    0x12,0x14, // LOAD_GLOBAL 'matrix_put'
    0xb4, // LOAD_FAST 4
    0xb7, // LOAD_FAST 7
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x32, // LOAD_METHOD 'remove'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_upload = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_world_upload,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 186,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 36,
        .line_info = fun_data_world_upload + 8,
        .line_info_top = fun_data_world_upload + 27,
        .opcodes = fun_data_world_upload + 27,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_files
static const byte fun_data_world_files[111] = {
    0xfa,0x80,0x01,0x1c, // prelude
    0x33,0x43,0x67, // names: files, limit, room_id
    0x80,0x6a,0x23,0x2a,0x26,0x22,0x2f,0x29,0x29,0x1f,0x26, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xc2, // STORE_FAST 2
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x12,0x66, // LOAD_GLOBAL 'host'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0xc3, // STORE_FAST 3
    0x12,0x1c, // LOAD_GLOBAL 'matrix_get'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xc4, // STORE_FAST 4
    0x51, // LOAD_CONST_NONE
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0x14,0x05, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0x10,0x34, // LOAD_CONST_STRING 'messages'
    0x55, // LOAD_SUBSCR
    0x10,0x35, // LOAD_CONST_STRING 'chunk'
    0x55, // LOAD_SUBSCR
    0x5f, // GET_ITER_STACK
    0x4b,0x38, // FOR_ITER 56
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x10,0x36, // LOAD_CONST_STRING 'type'
    0x55, // LOAD_SUBSCR
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x6c, // POP_JUMP_IF_FALSE 44
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0xb6, // LOAD_FAST 6
    0x10,0x37, // LOAD_CONST_STRING 'content'
    0x55, // LOAD_SUBSCR
    0xdd, // BINARY_OP 6 <in>
    0x44,0x63, // POP_JUMP_IF_FALSE 35
    0xb2, // LOAD_FAST 2
    0x14,0x38, // LOAD_METHOD 'append'
    0x2c,0x03, // BUILD_MAP 3
    0xb6, // LOAD_FAST 6
    0x10,0x37, // LOAD_CONST_STRING 'content'
    0x55, // LOAD_SUBSCR
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0x55, // LOAD_SUBSCR
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0x62, // STORE_MAP
    0xb6, // LOAD_FAST 6
    0x10,0x39, // LOAD_CONST_STRING 'age'
    0x55, // LOAD_SUBSCR
    0x10,0x3a, // LOAD_CONST_STRING 'age_ms'
    0x62, // STORE_MAP
    0xb6, // LOAD_FAST 6
    0x10,0x37, // LOAD_CONST_STRING 'content'
    0x55, // LOAD_SUBSCR
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x55, // LOAD_SUBSCR
    0x10,0x3b, // LOAD_CONST_STRING 'filename'
    0x62, // STORE_MAP
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x06, // JUMP -58
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_files = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_world_files,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 111,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_world_files + 7,
        .line_info_top = fun_data_world_files + 18,
        .opcodes = fun_data_world_files + 18,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_ls
static const byte fun_data_world_ls[116] = {
    0xf9,0x01,0x20, // prelude
    0x3c,0x74, // names: ls, count
    0x80,0x75,0x23,0x22,0x25,0x26,0x25,0x25,0x25,0x24,0x29,0x2a,0x37,0x24, // code info
    0x2c,0x00, // BUILD_MAP 0
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x12,0x33, // LOAD_GLOBAL 'files'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x3d, // LOAD_METHOD 'reverse'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x5f, // GET_ITER_STACK
    0x4b,0x4b, // FOR_ITER 75
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x10,0x3b, // LOAD_CONST_STRING 'filename'
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x43,0x72, // POP_JUMP_IF_TRUE 50
    0x52, // LOAD_CONST_TRUE
    0xb1, // LOAD_FAST 1
    0xb5, // LOAD_FAST 5
    0x56, // STORE_SUBSCR
    0xb5, // LOAD_FAST 5
    0x14,0x3e, // LOAD_METHOD 'endswith'
    0x10,0x28, // LOAD_CONST_STRING '.tar'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x10,0x3f, // LOAD_CONST_STRING '< %s >'
    0xb5, // LOAD_FAST 5
    0x51, // LOAD_CONST_NONE
    0x7c, // LOAD_CONST_SMALL_INT -4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0xc5, // STORE_FAST 5
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x10,0x40, // LOAD_CONST_STRING '\t% 40s %s'
    0xb5, // LOAD_FAST 5
    0x12,0x0c, // LOAD_GLOBAL 'nice_time'
    0xb4, // LOAD_FAST 4
    0x10,0x3a, // LOAD_CONST_STRING 'age_ms'
    0x55, // LOAD_SUBSCR
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x43, // JUMP 3
    0x42,0xb3,0x7f, // JUMP -77
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_ls = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_ls,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 116,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 60,
        .line_info = fun_data_world_ls + 5,
        .line_info_top = fun_data_world_ls + 19,
        .opcodes = fun_data_world_ls + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_download
static const byte fun_data_world_download[257] = {
    0x83,0x90,0x01,0x3a, // prelude
    0x41,0x3b,0x43,0x45, // names: download, filename, limit, chunk_size
    0x80,0x86,0x22,0x2a,0x25,0x2d,0x67,0x2c,0x25,0x2c,0x28,0x2d,0x47,0x27,0x27,0x49,0x27,0x26,0x2b,0x47,0x31,0x29,0x31,0x2c,0x6a, // code info
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x12,0x36, // LOAD_GLOBAL 'type'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x75, // LOAD_GLOBAL 'dict'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xb0, // LOAD_FAST 0
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0x12,0x0c, // LOAD_GLOBAL 'nice_time'
    0xb0, // LOAD_FAST 0
    0x10,0x3a, // LOAD_CONST_STRING 'age_ms'
    0x55, // LOAD_SUBSCR
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x10,0x3b, // LOAD_CONST_STRING 'filename'
    0x55, // LOAD_SUBSCR
    0xc0, // STORE_FAST 0
    0x42,0x79, // JUMP 57
    0x10,0x42, // LOAD_CONST_STRING '.'
    0xb0, // LOAD_FAST 0
    0x7b, // LOAD_CONST_SMALL_INT -5
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0x10,0x28, // LOAD_CONST_STRING '.tar'
    0xf2, // BINARY_OP 27 __add__
    0xc0, // STORE_FAST 0
    0x12,0x33, // LOAD_GLOBAL 'files'
    0x10,0x43, // LOAD_CONST_STRING 'limit'
    0xb1, // LOAD_FAST 1
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x5f, // GET_ITER_STACK
    0x4b,0x1d, // FOR_ITER 29
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x10,0x3b, // LOAD_CONST_STRING 'filename'
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x0c, // LOAD_GLOBAL 'nice_time'
    0xb5, // LOAD_FAST 5
    0x10,0x3a, // LOAD_CONST_STRING 'age_ms'
    0x55, // LOAD_SUBSCR
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0xc4, // STORE_FAST 4
    0xb5, // LOAD_FAST 5
    0x10,0x31, // LOAD_CONST_STRING 'url'
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0x42,0x21, // JUMP -31
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0xf0,0x80, // POP_JUMP_IF_FALSE 112
    0xb3, // LOAD_FAST 3
    0x86, // LOAD_CONST_SMALL_INT 6
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x12,0x66, // LOAD_GLOBAL 'host'
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0xc7, // STORE_FAST 7
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x25, // LOAD_METHOD 'display_stop'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x1c, // LOAD_GLOBAL 'matrix_get'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0xb8, // LOAD_FAST 8
    0x14,0x44, // LOAD_METHOD 'save'
    0xb0, // LOAD_FAST 0
    0x10,0x45, // LOAD_CONST_STRING 'chunk_size'
    0xb2, // LOAD_FAST 2
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc9, // STORE_FAST 9
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x2b, // LOAD_METHOD 'display_start'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0xb0, // LOAD_FAST 0
    0xb9, // LOAD_FAST 9
    0xb4, // LOAD_FAST 4
    0x14,0x46, // LOAD_METHOD 'lstrip'
    0x36,0x00, // CALL_METHOD 0
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x3e, // LOAD_METHOD 'endswith'
    0x10,0x28, // LOAD_CONST_STRING '.tar'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x65, // POP_JUMP_IF_FALSE 37
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0xb0, // LOAD_FAST 0
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0x7c, // LOAD_CONST_SMALL_INT -4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x47, // LOAD_METHOD 'tar_extract'
    0xb0, // LOAD_FAST 0
    0x10,0x48, // LOAD_CONST_STRING 'show_progress'
    0x50, // LOAD_CONST_FALSE
    0x36,0x82,0x01, // CALL_METHOD 257
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x32, // LOAD_METHOD 'remove'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x49, // JUMP 9
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_download = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_world_download,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 257,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 65,
        .line_info = fun_data_world_download + 8,
        .line_info_top = fun_data_world_download + 33,
        .opcodes = fun_data_world_download + 33,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_send
static const byte fun_data_world_send[49] = {
    0x41,0x0c, // prelude
    0x49,0x76, // names: send, message
    0x80,0xa8,0x2c,0x34, // code info
    0x2c,0x02, // BUILD_MAP 2
    0x10,0x4a, // LOAD_CONST_STRING 'm.text'
    0x10,0x2f, // LOAD_CONST_STRING 'msgtype'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x62, // STORE_MAP
    0xc1, // STORE_FAST 1
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x12,0x66, // LOAD_GLOBAL 'host'
    0x12,0x67, // LOAD_GLOBAL 'room_id'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x12,0x73, // LOAD_GLOBAL 'str'
    0x12,0x09, // LOAD_GLOBAL 'uuid4'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x04, // BUILD_TUPLE 4
    0xf8, // BINARY_OP 33 __mod__
    0xc2, // STORE_FAST 2
    0x12,0x14, // LOAD_GLOBAL 'matrix_put'
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_send = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_send,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 49,
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
        .qstr_block_name_idx = 73,
        .line_info = fun_data_world_send + 4,
        .line_info_top = fun_data_world_send + 8,
        .opcodes = fun_data_world_send + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_check
static const byte fun_data_world_check[155] = {
    0xf1,0x01,0x22, // prelude
    0x4b,0x43, // names: check, limit
    0x80,0xae,0x20,0x26,0x4d,0x2d,0x26,0x23,0x2b,0x2d,0x2f,0x29,0x29,0x1f,0x24, // code info
    0x12,0x6a, // LOAD_GLOBAL 'last_message'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x12,0x66, // LOAD_GLOBAL 'host'
    0x12,0x67, // LOAD_GLOBAL 'room_id'
    0xb0, // LOAD_FAST 0
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x42,0x4d, // JUMP 13
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x12,0x66, // LOAD_GLOBAL 'host'
    0x12,0x67, // LOAD_GLOBAL 'room_id'
    0x12,0x6a, // LOAD_GLOBAL 'last_message'
    0xb0, // LOAD_FAST 0
    0x2a,0x04, // BUILD_TUPLE 4
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0x12,0x1c, // LOAD_GLOBAL 'matrix_get'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x2b,0x00, // BUILD_LIST 0
    0xc3, // STORE_FAST 3
    0x10,0x34, // LOAD_CONST_STRING 'messages'
    0xb2, // LOAD_FAST 2
    0x14,0x05, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0xdd, // BINARY_OP 6 <in>
    0x44,0xd1,0x80, // POP_JUMP_IF_FALSE 81
    0xb2, // LOAD_FAST 2
    0x14,0x05, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0x10,0x34, // LOAD_CONST_STRING 'messages'
    0x55, // LOAD_SUBSCR
    0x10,0x4c, // LOAD_CONST_STRING 'end'
    0x55, // LOAD_SUBSCR
    0x17,0x6a, // STORE_GLOBAL 'last_message'
    0xb2, // LOAD_FAST 2
    0x14,0x05, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0x10,0x34, // LOAD_CONST_STRING 'messages'
    0x55, // LOAD_SUBSCR
    0x10,0x35, // LOAD_CONST_STRING 'chunk'
    0x55, // LOAD_SUBSCR
    0x5f, // GET_ITER_STACK
    0x4b,0x36, // FOR_ITER 54
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x10,0x36, // LOAD_CONST_STRING 'type'
    0x55, // LOAD_SUBSCR
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x6a, // POP_JUMP_IF_FALSE 42
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0xb4, // LOAD_FAST 4
    0x10,0x37, // LOAD_CONST_STRING 'content'
    0x55, // LOAD_SUBSCR
    0xdd, // BINARY_OP 6 <in>
    0x44,0x61, // POP_JUMP_IF_FALSE 33
    0xb3, // LOAD_FAST 3
    0x14,0x38, // LOAD_METHOD 'append'
    0x2c,0x02, // BUILD_MAP 2
    0xb4, // LOAD_FAST 4
    0x10,0x37, // LOAD_CONST_STRING 'content'
    0x55, // LOAD_SUBSCR
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x55, // LOAD_SUBSCR
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x62, // STORE_MAP
    0x12,0x77, // LOAD_GLOBAL 'int'
    0xb4, // LOAD_FAST 4
    0x10,0x39, // LOAD_CONST_STRING 'age'
    0x55, // LOAD_SUBSCR
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x4d, // LOAD_CONST_STRING 'age_s'
    0x62, // STORE_MAP
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x08, // JUMP -56
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_check = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_check,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 155,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 75,
        .line_info = fun_data_world_check + 5,
        .line_info_top = fun_data_world_check + 20,
        .opcodes = fun_data_world_check + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_put_message
static const byte fun_data_world_put_message[111] = {
    0x51,0x10, // prelude
    0x4e,0x78, // names: put_message, m
    0x80,0xc0,0x34,0x2d,0x4b,0x34, // code info
    0x10,0x4f, // LOAD_CONST_STRING '     '
    0x12,0x0c, // LOAD_GLOBAL 'nice_time'
    0xb0, // LOAD_FAST 0
    0x10,0x4d, // LOAD_CONST_STRING 'age_s'
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x50, // LOAD_CONST_STRING ': '
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x10,0x30, // LOAD_CONST_STRING 'body'
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0x12,0x71, // LOAD_GLOBAL 'print'
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x79, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x77, // POP_JUMP_IF_FALSE 55
    0x12,0x77, // LOAD_GLOBAL 'int'
    0x12,0x79, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5e, // JUMP 30
    0x57, // DUP_TOP
    0xc2, // STORE_FAST 2
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x23,0x13, // LOAD_CONST_OBJ 19
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xf4, // BINARY_OP 29 __mul__
    0xb2, // LOAD_FAST 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xf4, // BINARY_OP 29 __mul__
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1d, // POP_JUMP_IF_TRUE -35
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_put_message = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_world_put_message,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 111,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 78,
        .line_info = fun_data_world_put_message + 4,
        .line_info_top = fun_data_world_put_message + 10,
        .opcodes = fun_data_world_put_message + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_world_ui
static const byte fun_data_world_world_ui[83] = {
    0x38,0x0e, // prelude
    0x51, // names: world_ui
    0x80,0xc9,0x27,0x2e,0x2b,0x33, // code info
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x52, // LOAD_METHOD 'gpu_reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x13,0x53, // LOAD_ATTR 'Colors'
    0x13,0x54, // LOAD_ATTR 'INVERSE'
    0x23,0x14, // LOAD_CONST_OBJ 20
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x13,0x53, // LOAD_ATTR 'Colors'
    0x13,0x55, // LOAD_ATTR 'DEFAULT'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x56, // LOAD_METHOD 'roundrect'
    0x94, // LOAD_CONST_SMALL_INT 20
    0x94, // LOAD_CONST_SMALL_INT 20
    0x22,0x87,0x36, // LOAD_CONST_SMALL_INT 950
    0x22,0x83,0x74, // LOAD_CONST_SMALL_INT 500
    0x94, // LOAD_CONST_SMALL_INT 20
    0x22,0x81,0x11, // LOAD_CONST_SMALL_INT 145
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x56, // LOAD_METHOD 'roundrect'
    0x94, // LOAD_CONST_SMALL_INT 20
    0x22,0x84,0x08, // LOAD_CONST_SMALL_INT 520
    0x22,0x87,0x36, // LOAD_CONST_SMALL_INT 950
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0x94, // LOAD_CONST_SMALL_INT 20
    0x22,0x81,0x11, // LOAD_CONST_SMALL_INT 145
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_world_ui = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_world_world_ui,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 83,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 81,
        .line_info = fun_data_world_world_ui + 3,
        .line_info_top = fun_data_world_world_ui + 9,
        .opcodes = fun_data_world_world_ui + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of world__lt_module_gt_
// frozen bytecode for file world.py, scope world_world
static const byte fun_data_world_world[55] = {
    0x38,0x12, // prelude
    0x57, // names: world
    0x80,0xd1,0x2a,0x27,0x42,0x65,0x25,0x25, // code info
    0x12,0x06, // LOAD_GLOBAL 'tulip'
    0x14,0x58, // LOAD_METHOD 'ip'
    0x36,0x00, // CALL_METHOD 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x71, // LOAD_GLOBAL 'print'
    0x10,0x59, // LOAD_CONST_STRING 'need wifi.'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x51, // LOAD_GLOBAL 'world_ui'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x4b, // LOAD_GLOBAL 'check'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x09, // FOR_ITER 9
    0xc1, // STORE_FAST 1
    0x12,0x4e, // LOAD_GLOBAL 'put_message'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x35, // JUMP -11
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_world_world = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_world_world,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 55,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 87,
        .line_info = fun_data_world_world + 3,
        .line_info_top = fun_data_world_world + 11,
        .opcodes = fun_data_world_world + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_world__lt_module_gt_[] = {
    &raw_code_world_UUID,
    &raw_code_world_uuid4,
    &raw_code_world_nice_time,
    &raw_code_world_matrix_put,
    &raw_code_world_matrix_get,
    &raw_code_world_matrix_post,
    &raw_code_world__isdir,
    &raw_code_world_read_in_chunks,
    &raw_code_world_upload,
    &raw_code_world_files,
    &raw_code_world_ls,
    &raw_code_world_download,
    &raw_code_world_send,
    &raw_code_world_check,
    &raw_code_world_put_message,
    &raw_code_world_world_ui,
    &raw_code_world_world,
};

static const mp_raw_code_t raw_code_world__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_world__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 233,
    #endif
    .children = (void *)&children_world__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 17,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_world__lt_module_gt_ + 3,
        .line_info_top = fun_data_world__lt_module_gt_ + 45,
        .opcodes = fun_data_world__lt_module_gt_ + 45,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_world[124] = {
    MP_QSTR_world_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_os,
    MP_QSTR_ubinascii,
    MP_QSTR_urequests,
    MP_QSTR_json,
    MP_QSTR_tulip,
    MP_QSTR_time,
    MP_QSTR_UUID,
    MP_QSTR_uuid4,
    MP_QSTR_urandom,
    MP_QSTR_bytes,
    MP_QSTR_nice_time,
    MP_QSTR__percent_ds,
    MP_QSTR__percent_dm,
    MP_QSTR__percent_dh,
    MP_QSTR__percent_dd,
    MP_QSTR__tilde_,
    MP_QSTR__space_ago,
    MP_QSTR__percent__space_8s,
    MP_QSTR_matrix_put,
    MP_QSTR_put,
    MP_QSTR_data,
    MP_QSTR_dumps,
    MP_QSTR_encode,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR_headers,
    MP_QSTR_Bearer_space__percent_s,
    MP_QSTR_matrix_get,
    MP_QSTR_get,
    MP_QSTR_matrix_post,
    MP_QSTR_post,
    MP_QSTR__isdir,
    MP_QSTR_stat,
    MP_QSTR_read_in_chunks,
    MP_QSTR_read,
    MP_QSTR_upload,
    MP_QSTR_display_stop,
    MP_QSTR_Packing_space__percent_s,
    MP_QSTR_tar_create,
    MP_QSTR__dot_tar,
    MP_QSTR_rb,
    MP_QSTR_content_type,
    MP_QSTR_display_start,
    MP_QSTR_mimetype,
    MP_QSTR_info,
    MP_QSTR_m_dot_file,
    MP_QSTR_msgtype,
    MP_QSTR_body,
    MP_QSTR_url,
    MP_QSTR_remove,
    MP_QSTR_files,
    MP_QSTR_messages,
    MP_QSTR_chunk,
    MP_QSTR_type,
    MP_QSTR_content,
    MP_QSTR_append,
    MP_QSTR_age,
    MP_QSTR_age_ms,
    MP_QSTR_filename,
    MP_QSTR_ls,
    MP_QSTR_reverse,
    MP_QSTR_endswith,
    MP_QSTR__lt__space__percent_s_space__gt_,
    MP_QSTR__0x09__percent__space_40s_space__percent_s,
    MP_QSTR_download,
    MP_QSTR__dot_,
    MP_QSTR_limit,
    MP_QSTR_save,
    MP_QSTR_chunk_size,
    MP_QSTR_lstrip,
    MP_QSTR_tar_extract,
    MP_QSTR_show_progress,
    MP_QSTR_send,
    MP_QSTR_m_dot_text,
    MP_QSTR_check,
    MP_QSTR_end,
    MP_QSTR_age_s,
    MP_QSTR_put_message,
    MP_QSTR__space__space__space__space__space_,
    MP_QSTR__colon__space_,
    MP_QSTR_world_ui,
    MP_QSTR_gpu_reset,
    MP_QSTR_Colors,
    MP_QSTR_INVERSE,
    MP_QSTR_DEFAULT,
    MP_QSTR_roundrect,
    MP_QSTR_world,
    MP_QSTR_ip,
    MP_QSTR_need_space_wifi_dot_,
    MP_QSTR___init__,
    MP_QSTR__bytes,
    MP_QSTR_hex,
    MP_QSTR_hexlify,
    MP_QSTR_decode,
    MP_QSTR___str__,
    MP_QSTR__hyphen_,
    MP_QSTR_join,
    MP_QSTR___repr__,
    MP_QSTR__lt_UUID_colon__space__percent_s_gt_,
    MP_QSTR_ub,
    MP_QSTR_world_token,
    MP_QSTR_host,
    MP_QSTR_room_id,
    MP_QSTR_files_room_id,
    MP_QSTR_firmware_room_id,
    MP_QSTR_last_message,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_property,
    MP_QSTR_bytearray,
    MP_QSTR_file_object,
    MP_QSTR_print,
    MP_QSTR_open,
    MP_QSTR_str,
    MP_QSTR_count,
    MP_QSTR_dict,
    MP_QSTR_message,
    MP_QSTR_int,
    MP_QSTR_m,
    MP_QSTR_len,
    MP_QSTR_self,
    MP_QSTR_ValueError,
};

// constants
static const mp_obj_str_t const_obj_world_0 = {{&mp_type_str}, 56504, 39, (const byte*)"\x73\x79\x74\x5f\x64\x48\x56\x73\x61\x58\x41\x5f\x6c\x50\x41\x44\x69\x58\x43\x77\x4b\x64\x43\x4a\x76\x72\x65\x41\x4c\x53\x75\x6c\x5f\x30\x6f\x64\x79\x39\x4a"};
static const mp_obj_str_t const_obj_world_8 = {{&mp_type_str}, 17041, 46, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x6d\x65\x64\x69\x61\x2f\x76\x33\x2f\x75\x70\x6c\x6f\x61\x64\x3f\x66\x69\x6c\x65\x6e\x61\x6d\x65\x3d\x25\x73"};
static const mp_obj_str_t const_obj_world_10 = {{&mp_type_str}, 30620, 48, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x63\x6c\x69\x65\x6e\x74\x2f\x76\x33\x2f\x72\x6f\x6f\x6d\x73\x2f\x25\x73\x2f\x73\x65\x6e\x64\x2f\x25\x73\x2f\x25\x73"};
static const mp_obj_str_t const_obj_world_12 = {{&mp_type_str}, 58866, 27, (const byte*)"\x55\x70\x6c\x6f\x61\x64\x65\x64\x20\x25\x73\x20\x74\x6f\x20\x54\x75\x6c\x69\x70\x20\x57\x6f\x72\x6c\x64\x2e"};
static const mp_obj_str_t const_obj_world_13 = {{&mp_type_str}, 46920, 58, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x63\x6c\x69\x65\x6e\x74\x2f\x72\x30\x2f\x72\x6f\x6f\x6d\x73\x2f\x25\x73\x2f\x69\x6e\x69\x74\x69\x61\x6c\x53\x79\x6e\x63\x3f\x6c\x69\x6d\x69\x74\x3d\x25\x64"};
static const mp_obj_str_t const_obj_world_14 = {{&mp_type_str}, 37613, 39, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x6d\x65\x64\x69\x61\x2f\x72\x30\x2f\x64\x6f\x77\x6e\x6c\x6f\x61\x64\x2f\x25\x73"};
static const mp_obj_str_t const_obj_world_15 = {{&mp_type_str}, 59379, 59, (const byte*)"\x44\x6f\x77\x6e\x6c\x6f\x61\x64\x65\x64\x20\x25\x73\x20\x5b\x25\x64\x20\x62\x79\x74\x65\x73\x2c\x20\x6c\x61\x73\x74\x20\x75\x70\x64\x61\x74\x65\x64\x20\x25\x73\x5d\x20\x66\x72\x6f\x6d\x20\x54\x75\x6c\x69\x70\x20\x57\x6f\x72\x6c\x64\x2e"};
static const mp_obj_str_t const_obj_world_16 = {{&mp_type_str}, 58071, 35, (const byte*)"\x55\x6e\x70\x61\x63\x6b\x69\x6e\x67\x20\x25\x73\x2e\x20\x52\x75\x6e\x20\x69\x74\x20\x77\x69\x74\x68\x20\x72\x75\x6e\x28\x27\x25\x73\x27\x29"};
static const mp_obj_str_t const_obj_world_17 = {{&mp_type_str}, 21381, 32, (const byte*)"\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x66\x69\x6e\x64\x20\x25\x73\x20\x6f\x6e\x20\x54\x75\x6c\x69\x70\x20\x57\x6f\x72\x6c\x64"};
static const mp_obj_str_t const_obj_world_18 = {{&mp_type_str}, 41200, 69, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x63\x6c\x69\x65\x6e\x74\x2f\x72\x30\x2f\x72\x6f\x6f\x6d\x73\x2f\x25\x73\x2f\x6d\x65\x73\x73\x61\x67\x65\x73\x3f\x66\x72\x6f\x6d\x3d\x25\x73\x26\x64\x69\x72\x3d\x66\x26\x6c\x69\x6d\x69\x74\x3d\x25\x64"};
static const mp_obj_str_t const_obj_world_20 = {{&mp_type_str}, 23372, 32, (const byte*)"\x57\x65\x6c\x63\x6f\x6d\x65\x20\x74\x6f\x20\x54\x20\x55\x20\x4c\x20\x49\x20\x50\x20\x7e\x20\x57\x20\x4f\x20\x52\x20\x4c\x20\x44"};
static const mp_obj_str_t const_obj_world_21 = {{&mp_type_str}, 57860, 31, (const byte*)"\x62\x79\x74\x65\x73\x20\x61\x72\x67\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x31\x36\x20\x62\x79\x74\x65\x73\x20\x6c\x6f\x6e\x67"};

// constant table
static const mp_rom_obj_t const_obj_table_data_world[22] = {
    MP_ROM_PTR(&const_obj_world_0),
    MP_ROM_QSTR(MP_QSTR_duraflame_dot_rosaline_dot_org),
    MP_ROM_QSTR(MP_QSTR__bang_rGPkdYQOECXDlTVoGe_colon__percent_s),
    MP_ROM_QSTR(MP_QSTR__bang_MuceoboBAfueEttdFw_colon__percent_s),
    MP_ROM_QSTR(MP_QSTR__bang_eMmMZLncsdKrMOFTMM_colon__percent_s),
    MP_ROM_QSTR(MP_QSTR_application_slash_octet_hyphen_stream),
    MP_ROM_QSTR(MP_QSTR_Authorization),
    MP_ROM_QSTR(MP_QSTR_Content_hyphen_Type),
    MP_ROM_PTR(&const_obj_world_8),
    MP_ROM_QSTR(MP_QSTR_content_uri),
    MP_ROM_PTR(&const_obj_world_10),
    MP_ROM_QSTR(MP_QSTR_m_dot_room_dot_message),
    MP_ROM_PTR(&const_obj_world_12),
    MP_ROM_PTR(&const_obj_world_13),
    MP_ROM_PTR(&const_obj_world_14),
    MP_ROM_PTR(&const_obj_world_15),
    MP_ROM_PTR(&const_obj_world_16),
    MP_ROM_PTR(&const_obj_world_17),
    MP_ROM_PTR(&const_obj_world_18),
    MP_ROM_QSTR(MP_QSTR__space__space__space__space__space__space__space__space__space__space__space__space__space__space__space_),
    MP_ROM_PTR(&const_obj_world_20),
    MP_ROM_PTR(&const_obj_world_21),
};

static const mp_frozen_module_t frozen_module_world = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_world,
        .obj_table = (mp_obj_t *)&const_obj_table_data_world,
    },
    .rc = &raw_code_world__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module tulip
// - original source file: build-standard/tulip/obj/frozen_mpy/tulip.mpy
// - frozen file name: tulip.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file tulip.py, scope tulip__lt_module_gt_
static const byte fun_data_tulip__lt_module_gt_[256] = {
    0x18,0x68, // prelude
    0x01, // names: <module>
    0x60,0x20,0x28,0x2c,0x2c,0x46,0x64,0x40,0x89,0x18,0x89,0x4f,0x8b,0x12,0x89,0x2c,0x89,0x11,0x64,0x40,0x84,0x07,0x84,0x3e,0x84,0x14,0x84,0x0f,0x70,0x60,0x20,0x6e,0x40,0x88,0x12,0x65,0x20,0x65,0x20,0x85,0x08,0x65,0x60,0x85,0x0b,0x85,0x08,0x89,0x0c,0x65,0x60, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME '_tulip'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'world'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'world'
    0x1c,0x04, // IMPORT_FROM 'world'
    0x16,0x04, // STORE_NAME 'world'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x05, // LOAD_CONST_STRING 'cd'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x06, // IMPORT_NAME 'upysh'
    0x1c,0x05, // IMPORT_FROM 'cd'
    0x16,0x05, // STORE_NAME 'cd'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'alles'
    0x16,0x07, // STORE_NAME 'alles'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0f, // STORE_NAME 'screen_size'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x08, // LOAD_CONST_STRING 'Game'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'Game'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x09, // LOAD_CONST_STRING 'Sprite'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x09, // STORE_NAME 'Sprite'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x0a, // LOAD_CONST_STRING 'Player'
    0x11,0x09, // LOAD_NAME 'Sprite'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0a, // STORE_NAME 'Player'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x04, // MAKE_FUNCTION 4
    0x10,0x0b, // LOAD_CONST_STRING 'Colors'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0b, // STORE_NAME 'Colors'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x0c, // LOAD_CONST_STRING 'Joy'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'Joy'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x38, // STORE_NAME 'version'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x3d, // STORE_NAME 'free_disk_bytes'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x41, // STORE_NAME 'upgrade'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x62, // STORE_NAME 'joyk'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x6e, // STORE_NAME 'run'
    0x10,0x0d, // LOAD_CONST_STRING 'wb'
    0x2c,0x01, // BUILD_MAP 1
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x0e, // LOAD_CONST_STRING 'User-Agent'
    0x62, // STORE_MAP
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0b, // MAKE_FUNCTION_DEFARGS 11
    0x16,0x73, // STORE_NAME 'url_save'
    0x2c,0x01, // BUILD_MAP 1
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x0e, // LOAD_CONST_STRING 'User-Agent'
    0x62, // STORE_MAP
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0c, // MAKE_FUNCTION_DEFARGS 12
    0x16,0x78, // STORE_NAME 'url_get'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0d, // MAKE_FUNCTION_DEFARGS 13
    0x16,0x79, // STORE_NAME 'screenshot'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x81,0x02, // STORE_NAME 'ansi_fg'
    0x32,0x0f, // MAKE_FUNCTION 15
    0x16,0x81,0x04, // STORE_NAME 'ansi_bg'
    0x32,0x10, // MAKE_FUNCTION 16
    0x16,0x81,0x06, // STORE_NAME 'color'
    0x32,0x11, // MAKE_FUNCTION 17
    0x16,0x81,0x07, // STORE_NAME 'rgb'
    0x32,0x12, // MAKE_FUNCTION 18
    0x16,0x81,0x08, // STORE_NAME 'ip'
    0x32,0x13, // MAKE_FUNCTION 19
    0x16,0x81,0x0f, // STORE_NAME 'set_time'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x14, // MAKE_FUNCTION_DEFARGS 20
    0x16,0x81,0x12, // STORE_NAME 'wifi'
    0x32,0x15, // MAKE_FUNCTION 21
    0x16,0x81,0x15, // STORE_NAME 'tar_create'
    0x52, // LOAD_CONST_TRUE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x16, // MAKE_FUNCTION_DEFARGS 22
    0x16,0x81,0x1c, // STORE_NAME 'tar_extract'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_screen_size
static const byte fun_data_tulip_screen_size[21] = {
    0x18,0x08, // prelude
    0x0f, // names: screen_size
    0x80,0x0a,0x26, // code info
    0x12,0x81,0x43, // LOAD_GLOBAL 'timing'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_screen_size = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_screen_size,
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
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_tulip_screen_size + 3,
        .line_info_top = fun_data_tulip_screen_size + 6,
        .opcodes = fun_data_tulip_screen_size + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_Game
static const byte fun_data_tulip_Game[34] = {
    0x08,0x0a, // prelude
    0x08, // names: Game
    0x8b,0x0f,0x89,0x0a, // code info
    0x11,0x81,0x44, // LOAD_NAME '__name__'
    0x16,0x81,0x45, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'Game'
    0x16,0x81,0x46, // STORE_NAME '__qualname__'
    0x50, // LOAD_CONST_FALSE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x81,0x2c, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x81,0x2e, // STORE_NAME 'quit'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of tulip_Game
// frozen bytecode for file tulip.py, scope tulip_Game___init__
static const byte fun_data_tulip_Game___init__[65] = {
    0x9a,0x01,0x1e, // prelude
    0x81,0x2c,0x82,0x14,0x81,0x2d, // names: __init__, self, debug
    0x80,0x10,0x25,0x23,0x26,0x27,0x27,0x26,0x27, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x2d, // STORE_ATTR 'debug'
    0xb1, // LOAD_FAST 1
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0x12,0x82,0x15, // LOAD_GLOBAL 'tfb_save'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x82,0x16, // LOAD_GLOBAL 'key_scan'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x14,0x60, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x82,0x17, // LOAD_GLOBAL 'collisions'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'alles'
    0x14,0x60, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x6e, // STORE_ATTR 'run'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Game___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_tulip_Game___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 65,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 172,
        .line_info = fun_data_tulip_Game___init__ + 9,
        .line_info_top = fun_data_tulip_Game___init__ + 18,
        .opcodes = fun_data_tulip_Game___init__ + 18,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Game
// frozen bytecode for file tulip.py, scope tulip_Game_quit
static const byte fun_data_tulip_Game_quit[72] = {
    0x11,0x1c, // prelude
    0x81,0x2e,0x82,0x14, // names: quit, self
    0x80,0x1a,0x44,0x26,0x26,0x26,0x27,0x26,0x26,0x27, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x6e, // STORE_ATTR 'run'
    0x12,0x82,0x18, // LOAD_GLOBAL 'frame_callback'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x82,0x17, // LOAD_GLOBAL 'collisions'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x2d, // LOAD_ATTR 'debug'
    0x43,0x53, // POP_JUMP_IF_TRUE 19
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x14,0x60, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x82,0x19, // LOAD_GLOBAL 'gpu_reset'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x82,0x1a, // LOAD_GLOBAL 'tfb_restore'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x82,0x16, // LOAD_GLOBAL 'key_scan'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x82,0x1b, // LOAD_GLOBAL 'display_restart'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Game_quit = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Game_quit,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 72,
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
        .qstr_block_name_idx = 174,
        .line_info = fun_data_tulip_Game_quit + 6,
        .line_info_top = fun_data_tulip_Game_quit + 16,
        .opcodes = fun_data_tulip_Game_quit + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_tulip_Game[] = {
    &raw_code_tulip_Game___init__,
    &raw_code_tulip_Game_quit,
};

static const mp_raw_code_t raw_code_tulip_Game = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Game,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 34,
    #endif
    .children = (void *)&children_tulip_Game,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_tulip_Game + 3,
        .line_info_top = fun_data_tulip_Game + 7,
        .opcodes = fun_data_tulip_Game + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_Sprite
static const byte fun_data_tulip_Sprite[110] = {
    0x08,0x28, // prelude
    0x09, // names: Sprite
    0x8b,0x27,0x24,0x24,0x27,0x24,0x4c,0x64,0x40,0x8a,0x15,0x85,0x0c,0x85,0x11,0x65,0x65,0x65,0x60, // code info
    0x11,0x81,0x44, // LOAD_NAME '__name__'
    0x16,0x81,0x45, // STORE_NAME '__module__'
    0x10,0x09, // LOAD_CONST_STRING 'Sprite'
    0x16,0x81,0x46, // STORE_NAME '__qualname__'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x81,0x2f, // STORE_NAME 'mem_pointer'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x81,0x30, // STORE_NAME 'num_sprites'
    0x22,0x82,0x80,0x00, // LOAD_CONST_SMALL_INT 32768
    0x16,0x81,0x3d, // STORE_NAME 'SPRITE_RAM_BYTES'
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x16,0x81,0x3c, // STORE_NAME 'SPRITES'
    0x11,0x0f, // LOAD_NAME 'screen_size'
    0x34,0x00, // CALL_FUNCTION 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0x16,0x81,0x39, // STORE_NAME 'SCREEN_WIDTH'
    0x16,0x81,0x3a, // STORE_NAME 'SCREEN_HEIGHT'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x60, // STORE_NAME 'reset'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x81,0x2c, // STORE_NAME '__init__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x81,0x38, // STORE_NAME 'clamp'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x81,0x3b, // STORE_NAME 'load'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x81,0x3e, // STORE_NAME 'off'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x81,0x3f, // STORE_NAME 'on'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x81,0x40, // STORE_NAME 'moveto'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x81,0x41, // STORE_NAME 'move'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_reset
static const byte fun_data_tulip_Sprite_reset[27] = {
    0x08,0x0a, // prelude
    0x60, // names: reset
    0x80,0x2e,0x26,0x26, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x18,0x81,0x2f, // STORE_ATTR 'mem_pointer'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x18,0x81,0x30, // STORE_ATTR 'num_sprites'
    0x12,0x82,0x1c, // LOAD_GLOBAL 'sprite_clear'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_reset = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Sprite_reset,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 27,
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
        .qstr_block_name_idx = 96,
        .line_info = fun_data_tulip_Sprite_reset + 3,
        .line_info_top = fun_data_tulip_Sprite_reset + 7,
        .opcodes = fun_data_tulip_Sprite_reset + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite___init__
static const byte fun_data_tulip_Sprite___init__[133] = {
    0xab,0x80,0x01,0x30, // prelude
    0x81,0x2c,0x82,0x14,0x81,0x31,0x82,0x1d, // names: __init__, self, sprite_id, copy_of
    0x80,0x33,0x25,0x29,0x4e,0x65,0x26,0x28,0x28,0x4a,0x25,0x25,0x25,0x26,0x25,0x26, // code info
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x30, // LOAD_ATTR 'num_sprites'
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x31, // STORE_ATTR 'sprite_id'
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x57, // DUP_TOP
    0x13,0x81,0x30, // LOAD_ATTR 'num_sprites'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x81,0x30, // STORE_ATTR 'num_sprites'
    0x42,0x45, // JUMP 5
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x31, // STORE_ATTR 'sprite_id'
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0xb2, // LOAD_FAST 2
    0x13,0x81,0x32, // LOAD_ATTR 'mem_pos'
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x32, // STORE_ATTR 'mem_pos'
    0xb2, // LOAD_FAST 2
    0x13,0x81,0x33, // LOAD_ATTR 'width'
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x33, // STORE_ATTR 'width'
    0xb2, // LOAD_FAST 2
    0x13,0x81,0x34, // LOAD_ATTR 'height'
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x34, // STORE_ATTR 'height'
    0x42,0x4f, // JUMP 15
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x32, // STORE_ATTR 'mem_pos'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x33, // STORE_ATTR 'width'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x34, // STORE_ATTR 'height'
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0xb0, // LOAD_FAST 0
    0x18,0x57, // STORE_ATTR 'y'
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x36, // STORE_ATTR 'x_v'
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x37, // STORE_ATTR 'y_v'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_tulip_Sprite___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 133,
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
        .qstr_block_name_idx = 172,
        .line_info = fun_data_tulip_Sprite___init__ + 12,
        .line_info_top = fun_data_tulip_Sprite___init__ + 28,
        .opcodes = fun_data_tulip_Sprite___init__ + 28,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_clamp
static const byte fun_data_tulip_Sprite_clamp[105] = {
    0x19,0x1a, // prelude
    0x81,0x38,0x82,0x14, // names: clamp, self
    0x80,0x49,0x28,0x25,0x31,0x30,0x27,0x24,0x30, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x35, // LOAD_ATTR 'x'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x35, // LOAD_ATTR 'x'
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x39, // LOAD_ATTR 'SCREEN_WIDTH'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x33, // LOAD_ATTR 'width'
    0xf3, // BINARY_OP 28 __sub__
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x39, // LOAD_ATTR 'SCREEN_WIDTH'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x33, // LOAD_ATTR 'width'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'y'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x57, // STORE_ATTR 'y'
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'y'
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x3a, // LOAD_ATTR 'SCREEN_HEIGHT'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x34, // LOAD_ATTR 'height'
    0xf3, // BINARY_OP 28 __sub__
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x3a, // LOAD_ATTR 'SCREEN_HEIGHT'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x34, // LOAD_ATTR 'height'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x18,0x57, // STORE_ATTR 'y'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_clamp = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Sprite_clamp,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 105,
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
        .qstr_block_name_idx = 184,
        .line_info = fun_data_tulip_Sprite_clamp + 6,
        .line_info_top = fun_data_tulip_Sprite_clamp + 15,
        .opcodes = fun_data_tulip_Sprite_clamp + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_load
static const byte fun_data_tulip_Sprite_load[184] = {
    0xc0,0x04,0x2c, // prelude
    0x81,0x3b,0x82,0x14,0x52,0x81,0x33,0x81,0x34, // names: load, self, filename, width, height
    0x80,0x54,0x2c,0x48,0x29,0x59,0x25,0x25,0x31,0x53,0x29,0x2c,0x36, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x3c, // LOAD_ATTR 'SPRITES'
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x81,0x7e, // LOAD_GLOBAL 'Exception'
    0x23,0x1e, // LOAD_CONST_OBJ 30
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x32, // LOAD_ATTR 'mem_pos'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x82,0x1e, // LOAD_GLOBAL 'sprite_register'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x32, // LOAD_ATTR 'mem_pos'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x33, // LOAD_ATTR 'width'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x34, // LOAD_ATTR 'height'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x42,0xe7,0x80, // JUMP 103
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x33, // STORE_ATTR 'width'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x34, // STORE_ATTR 'height'
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x2f, // LOAD_ATTR 'mem_pointer'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0xf2, // BINARY_OP 27 __add__
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x3d, // LOAD_ATTR 'SPRITE_RAM_BYTES'
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x81,0x7e, // LOAD_GLOBAL 'Exception'
    0x23,0x1f, // LOAD_CONST_OBJ 31
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x2f, // LOAD_ATTR 'mem_pointer'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x2f, // LOAD_ATTR 'mem_pointer'
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x32, // STORE_ATTR 'mem_pos'
    0x12,0x82,0x1f, // LOAD_GLOBAL 'sprite_png'
    0xb1, // LOAD_FAST 1
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x13,0x81,0x2f, // LOAD_ATTR 'mem_pointer'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x82,0x1e, // LOAD_GLOBAL 'sprite_register'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x32, // LOAD_ATTR 'mem_pos'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x33, // LOAD_ATTR 'width'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x34, // LOAD_ATTR 'height'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x12,0x09, // LOAD_GLOBAL 'Sprite'
    0x57, // DUP_TOP
    0x13,0x81,0x2f, // LOAD_ATTR 'mem_pointer'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xf4, // BINARY_OP 29 __mul__
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x81,0x2f, // STORE_ATTR 'mem_pointer'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_load = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_tulip_Sprite_load,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 184,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 187,
        .line_info = fun_data_tulip_Sprite_load + 12,
        .line_info_top = fun_data_tulip_Sprite_load + 25,
        .opcodes = fun_data_tulip_Sprite_load + 25,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_off
static const byte fun_data_tulip_Sprite_off[20] = {
    0x11,0x0c, // prelude
    0x81,0x3e,0x82,0x14, // names: off, self
    0x80,0x65, // code info
    0x12,0x82,0x20, // LOAD_GLOBAL 'sprite_off'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_off = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Sprite_off,
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
        .qstr_block_name_idx = 190,
        .line_info = fun_data_tulip_Sprite_off + 6,
        .line_info_top = fun_data_tulip_Sprite_off + 8,
        .opcodes = fun_data_tulip_Sprite_off + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_on
static const byte fun_data_tulip_Sprite_on[20] = {
    0x11,0x0c, // prelude
    0x81,0x3f,0x82,0x14, // names: on, self
    0x80,0x68, // code info
    0x12,0x82,0x21, // LOAD_GLOBAL 'sprite_on'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_on = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Sprite_on,
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
        .qstr_block_name_idx = 191,
        .line_info = fun_data_tulip_Sprite_on + 6,
        .line_info_top = fun_data_tulip_Sprite_on + 8,
        .opcodes = fun_data_tulip_Sprite_on + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_moveto
static const byte fun_data_tulip_Sprite_moveto[31] = {
    0x23,0x16, // prelude
    0x81,0x40,0x82,0x14,0x81,0x35,0x57, // names: moveto, self, x, y
    0x80,0x6c,0x25,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x57, // STORE_ATTR 'y'
    0xb0, // LOAD_FAST 0
    0x14,0x81,0x41, // LOAD_METHOD 'move'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_moveto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_tulip_Sprite_moveto,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 31,
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
        .qstr_block_name_idx = 192,
        .line_info = fun_data_tulip_Sprite_moveto + 9,
        .line_info_top = fun_data_tulip_Sprite_moveto + 13,
        .opcodes = fun_data_tulip_Sprite_moveto + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Sprite
// frozen bytecode for file tulip.py, scope tulip_Sprite_move
static const byte fun_data_tulip_Sprite_move[37] = {
    0x29,0x0c, // prelude
    0x81,0x41,0x82,0x14, // names: move, self
    0x80,0x71, // code info
    0x12,0x82,0x22, // LOAD_GLOBAL 'sprite_move'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x31, // LOAD_ATTR 'sprite_id'
    0x12,0x82,0x23, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x35, // LOAD_ATTR 'x'
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x82,0x23, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x13,0x57, // LOAD_ATTR 'y'
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Sprite_move = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Sprite_move,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
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
        .qstr_block_name_idx = 193,
        .line_info = fun_data_tulip_Sprite_move + 6,
        .line_info_top = fun_data_tulip_Sprite_move + 8,
        .opcodes = fun_data_tulip_Sprite_move + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_tulip_Sprite[] = {
    &raw_code_tulip_Sprite_reset,
    &raw_code_tulip_Sprite___init__,
    &raw_code_tulip_Sprite_clamp,
    &raw_code_tulip_Sprite_load,
    &raw_code_tulip_Sprite_off,
    &raw_code_tulip_Sprite_on,
    &raw_code_tulip_Sprite_moveto,
    &raw_code_tulip_Sprite_move,
};

static const mp_raw_code_t raw_code_tulip_Sprite = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Sprite,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 110,
    #endif
    .children = (void *)&children_tulip_Sprite,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 8,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_tulip_Sprite + 3,
        .line_info_top = fun_data_tulip_Sprite + 22,
        .opcodes = fun_data_tulip_Sprite + 22,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_Player
static const byte fun_data_tulip_Player[37] = {
    0x18,0x0b, // prelude
    0x0a, // names: Player
    0x8b,0x76,0x6b,0x40,0x00, // code info
    0x11,0x81,0x44, // LOAD_NAME '__name__'
    0x16,0x81,0x45, // STORE_NAME '__module__'
    0x10,0x0a, // LOAD_CONST_STRING 'Player'
    0x16,0x81,0x46, // STORE_NAME '__qualname__'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x00,0x01, // MAKE_CLOSURE_DEFARGS 0
    0x16,0x81,0x2c, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x81,0x42, // STORE_NAME 'joy_move'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of tulip_Player
// frozen bytecode for file tulip.py, scope tulip_Player___init__
static const byte fun_data_tulip_Player___init__[38] = {
    0xab,0x01,0x16, // prelude
    0x81,0x2c,0x02,0x82,0x14,0x82,0x24, // names: __init__, *, self, speed
    0x80,0x77,0x2c,0x25, // code info
    0x12,0x82,0x25, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x81,0x2c, // LOAD_SUPER_METHOD '__init__'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x18,0x81,0x36, // STORE_ATTR 'x_v'
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x18,0x81,0x37, // STORE_ATTR 'y_v'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Player___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_tulip_Player___init__,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 172,
        .line_info = fun_data_tulip_Player___init__ + 10,
        .line_info_top = fun_data_tulip_Player___init__ + 14,
        .opcodes = fun_data_tulip_Player___init__ + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip_Player
// frozen bytecode for file tulip.py, scope tulip_Player_joy_move
static const byte fun_data_tulip_Player_joy_move[121] = {
    0x19,0x1c, // prelude
    0x81,0x42,0x82,0x14, // names: joy_move, self
    0x80,0x7d,0x2b,0x2e,0x2b,0x2e,0x2b,0x2c,0x2b,0x2c, // code info
    0x12,0x62, // LOAD_GLOBAL 'joyk'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x63, // LOAD_ATTR 'RIGHT'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x81,0x35, // LOAD_ATTR 'x'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x36, // LOAD_ATTR 'x_v'
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0x12,0x62, // LOAD_GLOBAL 'joyk'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x64, // LOAD_ATTR 'LEFT'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x81,0x35, // LOAD_ATTR 'x'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x36, // LOAD_ATTR 'x_v'
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x81,0x35, // STORE_ATTR 'x'
    0x12,0x62, // LOAD_GLOBAL 'joyk'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x65, // LOAD_ATTR 'UP'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x57, // LOAD_ATTR 'y'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x37, // LOAD_ATTR 'y_v'
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x57, // STORE_ATTR 'y'
    0x12,0x62, // LOAD_GLOBAL 'joyk'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x66, // LOAD_ATTR 'DOWN'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x57, // LOAD_ATTR 'y'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x37, // LOAD_ATTR 'y_v'
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x57, // STORE_ATTR 'y'
    0xb0, // LOAD_FAST 0
    0x14,0x81,0x38, // LOAD_METHOD 'clamp'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Player_joy_move = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_Player_joy_move,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 121,
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
        .qstr_block_name_idx = 194,
        .line_info = fun_data_tulip_Player_joy_move + 6,
        .line_info_top = fun_data_tulip_Player_joy_move + 16,
        .opcodes = fun_data_tulip_Player_joy_move + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_tulip_Player[] = {
    &raw_code_tulip_Player___init__,
    &raw_code_tulip_Player_joy_move,
};

static const mp_raw_code_t raw_code_tulip_Player = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Player,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 37,
    #endif
    .children = (void *)&children_tulip_Player,
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
        .qstr_block_name_idx = 10,
        .line_info = fun_data_tulip_Player + 3,
        .line_info_top = fun_data_tulip_Player + 7,
        .opcodes = fun_data_tulip_Player + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_Colors
static const byte fun_data_tulip_Colors[947] = {
    0xf8,0xb0,0x30,0x58, // prelude
    0x0b, // names: Colors
    0x8b,0x88,0x20,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x2d,0x29,0x2d, // code info
    0x11,0x81,0x44, // LOAD_NAME '__name__'
    0x16,0x81,0x45, // STORE_NAME '__module__'
    0x10,0x0b, // LOAD_CONST_STRING 'Colors'
    0x16,0x81,0x46, // STORE_NAME '__qualname__'
    0x10,0x10, // LOAD_CONST_STRING '\x1b[0;30m'
    0x16,0x81,0x47, // STORE_NAME 'BLACK'
    0x10,0x11, // LOAD_CONST_STRING '\x1b[0;31m'
    0x16,0x81,0x48, // STORE_NAME 'RED'
    0x10,0x12, // LOAD_CONST_STRING '\x1b[0;32m'
    0x16,0x81,0x49, // STORE_NAME 'GREEN'
    0x10,0x13, // LOAD_CONST_STRING '\x1b[0;33m'
    0x16,0x81,0x4a, // STORE_NAME 'BROWN'
    0x10,0x14, // LOAD_CONST_STRING '\x1b[0;34m'
    0x16,0x81,0x4b, // STORE_NAME 'BLUE'
    0x10,0x15, // LOAD_CONST_STRING '\x1b[0;35m'
    0x16,0x81,0x4c, // STORE_NAME 'PURPLE'
    0x10,0x16, // LOAD_CONST_STRING '\x1b[0;36m'
    0x16,0x81,0x4d, // STORE_NAME 'CYAN'
    0x10,0x17, // LOAD_CONST_STRING '\x1b[0;37m'
    0x16,0x81,0x4e, // STORE_NAME 'LIGHT_GRAY'
    0x10,0x18, // LOAD_CONST_STRING '\x1b[1;30m'
    0x16,0x81,0x4f, // STORE_NAME 'DARK_GRAY'
    0x10,0x19, // LOAD_CONST_STRING '\x1b[1;31m'
    0x16,0x81,0x50, // STORE_NAME 'LIGHT_RED'
    0x10,0x1a, // LOAD_CONST_STRING '\x1b[1;32m'
    0x16,0x81,0x51, // STORE_NAME 'LIGHT_GREEN'
    0x10,0x1b, // LOAD_CONST_STRING '\x1b[1;33m'
    0x16,0x81,0x52, // STORE_NAME 'YELLOW'
    0x10,0x1c, // LOAD_CONST_STRING '\x1b[1;34m'
    0x16,0x81,0x53, // STORE_NAME 'LIGHT_BLUE'
    0x10,0x1d, // LOAD_CONST_STRING '\x1b[1;35m'
    0x16,0x81,0x54, // STORE_NAME 'LIGHT_PURPLE'
    0x10,0x1e, // LOAD_CONST_STRING '\x1b[1;36m'
    0x16,0x81,0x55, // STORE_NAME 'LIGHT_CYAN'
    0x10,0x1f, // LOAD_CONST_STRING '\x1b[1;37m'
    0x16,0x81,0x56, // STORE_NAME 'LIGHT_WHITE'
    0x10,0x20, // LOAD_CONST_STRING '\x1b[0;40m'
    0x16,0x81,0x57, // STORE_NAME 'BG_BLACK'
    0x10,0x21, // LOAD_CONST_STRING '\x1b[0;41m'
    0x16,0x81,0x58, // STORE_NAME 'BG_RED'
    0x10,0x22, // LOAD_CONST_STRING '\x1b[0;42m'
    0x16,0x81,0x59, // STORE_NAME 'BG_GREEN'
    0x10,0x23, // LOAD_CONST_STRING '\x1b[0;43m'
    0x16,0x81,0x5a, // STORE_NAME 'BG_BROWN'
    0x10,0x24, // LOAD_CONST_STRING '\x1b[0;44m'
    0x16,0x81,0x5b, // STORE_NAME 'BG_BLUE'
    0x10,0x25, // LOAD_CONST_STRING '\x1b[0;45m'
    0x16,0x81,0x5c, // STORE_NAME 'BG_PURPLE'
    0x10,0x26, // LOAD_CONST_STRING '\x1b[0;46m'
    0x16,0x81,0x5d, // STORE_NAME 'BG_CYAN'
    0x10,0x27, // LOAD_CONST_STRING '\x1b[0;47m'
    0x16,0x81,0x5e, // STORE_NAME 'BG_LIGHT_GRAY'
    0x10,0x28, // LOAD_CONST_STRING '\x1b[1;40m'
    0x16,0x81,0x5f, // STORE_NAME 'BG_DARK_GRAY'
    0x10,0x29, // LOAD_CONST_STRING '\x1b[1;41m'
    0x16,0x81,0x60, // STORE_NAME 'BG_LIGHT_RED'
    0x10,0x2a, // LOAD_CONST_STRING '\x1b[1;42m'
    0x16,0x81,0x61, // STORE_NAME 'BG_LIGHT_GREEN'
    0x10,0x2b, // LOAD_CONST_STRING '\x1b[1;43m'
    0x16,0x81,0x62, // STORE_NAME 'BG_YELLOW'
    0x10,0x2c, // LOAD_CONST_STRING '\x1b[1;44m'
    0x16,0x81,0x63, // STORE_NAME 'BG_LIGHT_BLUE'
    0x10,0x2d, // LOAD_CONST_STRING '\x1b[1;45m'
    0x16,0x81,0x64, // STORE_NAME 'BG_LIGHT_PURPLE'
    0x10,0x2e, // LOAD_CONST_STRING '\x1b[1;46m'
    0x16,0x81,0x65, // STORE_NAME 'BG_LIGHT_CYAN'
    0x10,0x2f, // LOAD_CONST_STRING '\x1b[1;47m'
    0x16,0x81,0x66, // STORE_NAME 'BG_LIGHT_WHITE'
    0x10,0x30, // LOAD_CONST_STRING '\x1b[1m'
    0x16,0x81,0x67, // STORE_NAME 'BOLD'
    0x10,0x31, // LOAD_CONST_STRING '\x1b[2m'
    0x16,0x81,0x68, // STORE_NAME 'FAINT'
    0x10,0x32, // LOAD_CONST_STRING '\x1b[3m'
    0x16,0x81,0x69, // STORE_NAME 'ITALIC'
    0x10,0x33, // LOAD_CONST_STRING '\x1b[4m'
    0x16,0x81,0x6a, // STORE_NAME 'UNDERLINE'
    0x10,0x34, // LOAD_CONST_STRING '\x1b[5m'
    0x57, // DUP_TOP
    0x16,0x81,0x6b, // STORE_NAME 'FLASH'
    0x16,0x81,0x6c, // STORE_NAME 'BLINK'
    0x10,0x35, // LOAD_CONST_STRING '\x1b[7m'
    0x57, // DUP_TOP
    0x16,0x81,0x6d, // STORE_NAME 'REVERSE'
    0x57, // DUP_TOP
    0x16,0x81,0x6e, // STORE_NAME 'INVERSE'
    0x16,0x81,0x6f, // STORE_NAME 'NEGATIVE'
    0x10,0x36, // LOAD_CONST_STRING '\x1b[9m'
    0x57, // DUP_TOP
    0x16,0x81,0x70, // STORE_NAME 'STRIKE'
    0x16,0x81,0x71, // STORE_NAME 'CROSSED'
    0x10,0x37, // LOAD_CONST_STRING '\x1b[0m'
    0x57, // DUP_TOP
    0x16,0x81,0x72, // STORE_NAME 'RESET'
    0x57, // DUP_TOP
    0x16,0x81,0x73, // STORE_NAME 'DEFAULT'
    0x16,0x81,0x74, // STORE_NAME 'END'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x22,0x80,0x49, // LOAD_CONST_SMALL_INT 73
    0x22,0x81,0x12, // LOAD_CONST_SMALL_INT 146
    0x22,0x81,0x5b, // LOAD_CONST_SMALL_INT 219
    0x22,0x81,0x7b, // LOAD_CONST_SMALL_INT 251
    0x22,0x81,0x32, // LOAD_CONST_SMALL_INT 178
    0x22,0x80,0x69, // LOAD_CONST_SMALL_INT 105
    0x22,0x81,0x52, // LOAD_CONST_SMALL_INT 210
    0x22,0x81,0x72, // LOAD_CONST_SMALL_INT 242
    0x22,0x81,0x09, // LOAD_CONST_SMALL_INT 137
    0x22,0x81,0x29, // LOAD_CONST_SMALL_INT 169
    0x22,0x81,0x49, // LOAD_CONST_SMALL_INT 201
    0x22,0x81,0x69, // LOAD_CONST_SMALL_INT 233
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0x22,0x80,0x60, // LOAD_CONST_SMALL_INT 96
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0x22,0x81,0x20, // LOAD_CONST_SMALL_INT 160
    0x22,0x81,0x40, // LOAD_CONST_SMALL_INT 192
    0x22,0x81,0x60, // LOAD_CONST_SMALL_INT 224
    0x22,0x81,0x64, // LOAD_CONST_SMALL_INT 228
    0x22,0x81,0x44, // LOAD_CONST_SMALL_INT 196
    0x22,0x81,0x6d, // LOAD_CONST_SMALL_INT 237
    0x22,0x81,0x24, // LOAD_CONST_SMALL_INT 164
    0x22,0x81,0x4d, // LOAD_CONST_SMALL_INT 205
    0x22,0x81,0x04, // LOAD_CONST_SMALL_INT 132
    0x22,0x81,0x68, // LOAD_CONST_SMALL_INT 232
    0x22,0x81,0x76, // LOAD_CONST_SMALL_INT 246
    0x22,0x81,0x2d, // LOAD_CONST_SMALL_INT 173
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0x22,0x81,0x71, // LOAD_CONST_SMALL_INT 241
    0x22,0x81,0x28, // LOAD_CONST_SMALL_INT 168
    0x22,0x81,0x6c, // LOAD_CONST_SMALL_INT 236
    0x22,0x81,0x56, // LOAD_CONST_SMALL_INT 214
    0x22,0x81,0x0d, // LOAD_CONST_SMALL_INT 141
    0x22,0x81,0x51, // LOAD_CONST_SMALL_INT 209
    0x22,0x80,0x44, // LOAD_CONST_SMALL_INT 68
    0x22,0x81,0x08, // LOAD_CONST_SMALL_INT 136
    0x22,0x81,0x4c, // LOAD_CONST_SMALL_INT 204
    0x22,0x81,0x70, // LOAD_CONST_SMALL_INT 240
    0x22,0x81,0x75, // LOAD_CONST_SMALL_INT 245
    0x22,0x81,0x2c, // LOAD_CONST_SMALL_INT 172
    0x22,0x81,0x7a, // LOAD_CONST_SMALL_INT 250
    0x22,0x81,0x31, // LOAD_CONST_SMALL_INT 177
    0x22,0x80,0x68, // LOAD_CONST_SMALL_INT 104
    0x22,0x81,0x50, // LOAD_CONST_SMALL_INT 208
    0x22,0x81,0x74, // LOAD_CONST_SMALL_INT 244
    0x22,0x81,0x55, // LOAD_CONST_SMALL_INT 213
    0x22,0x81,0x0c, // LOAD_CONST_SMALL_INT 140
    0x22,0x81,0x79, // LOAD_CONST_SMALL_INT 249
    0x22,0x81,0x30, // LOAD_CONST_SMALL_INT 176
    0x22,0x81,0x54, // LOAD_CONST_SMALL_INT 212
    0x22,0x81,0x78, // LOAD_CONST_SMALL_INT 248
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0x22,0x81,0x36, // LOAD_CONST_SMALL_INT 182
    0x22,0x80,0x6d, // LOAD_CONST_SMALL_INT 109
    0x22,0x81,0x5a, // LOAD_CONST_SMALL_INT 218
    0x22,0x81,0x7e, // LOAD_CONST_SMALL_INT 254
    0x22,0x81,0x11, // LOAD_CONST_SMALL_INT 145
    0x22,0x81,0x35, // LOAD_CONST_SMALL_INT 181
    0x22,0x81,0x59, // LOAD_CONST_SMALL_INT 217
    0x22,0x81,0x7d, // LOAD_CONST_SMALL_INT 253
    0xa4, // LOAD_CONST_SMALL_INT 36
    0x22,0x80,0x48, // LOAD_CONST_SMALL_INT 72
    0x22,0x80,0x6c, // LOAD_CONST_SMALL_INT 108
    0x22,0x81,0x10, // LOAD_CONST_SMALL_INT 144
    0x22,0x81,0x34, // LOAD_CONST_SMALL_INT 180
    0x22,0x81,0x58, // LOAD_CONST_SMALL_INT 216
    0x22,0x81,0x7c, // LOAD_CONST_SMALL_INT 252
    0x22,0x81,0x5c, // LOAD_CONST_SMALL_INT 220
    0x22,0x81,0x38, // LOAD_CONST_SMALL_INT 184
    0x22,0x81,0x5d, // LOAD_CONST_SMALL_INT 221
    0x22,0x81,0x14, // LOAD_CONST_SMALL_INT 148
    0x22,0x81,0x39, // LOAD_CONST_SMALL_INT 185
    0x22,0x80,0x70, // LOAD_CONST_SMALL_INT 112
    0x22,0x81,0x3c, // LOAD_CONST_SMALL_INT 188
    0x22,0x81,0x5e, // LOAD_CONST_SMALL_INT 222
    0x22,0x81,0x15, // LOAD_CONST_SMALL_INT 149
    0x22,0x80,0x4c, // LOAD_CONST_SMALL_INT 76
    0x22,0x81,0x18, // LOAD_CONST_SMALL_INT 152
    0x22,0x81,0x3d, // LOAD_CONST_SMALL_INT 189
    0x22,0x80,0x74, // LOAD_CONST_SMALL_INT 116
    0x22,0x81,0x1c, // LOAD_CONST_SMALL_INT 156
    0x22,0x81,0x3a, // LOAD_CONST_SMALL_INT 186
    0x22,0x80,0x71, // LOAD_CONST_SMALL_INT 113
    0x22,0x81,0x19, // LOAD_CONST_SMALL_INT 153
    0xa8, // LOAD_CONST_SMALL_INT 40
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0x22,0x80,0x78, // LOAD_CONST_SMALL_INT 120
    0x22,0x80,0x7c, // LOAD_CONST_SMALL_INT 124
    0x22,0x81,0x1d, // LOAD_CONST_SMALL_INT 157
    0x22,0x80,0x54, // LOAD_CONST_SMALL_INT 84
    0x22,0x81,0x3e, // LOAD_CONST_SMALL_INT 190
    0x22,0x80,0x75, // LOAD_CONST_SMALL_INT 117
    0xac, // LOAD_CONST_SMALL_INT 44
    0x22,0x80,0x58, // LOAD_CONST_SMALL_INT 88
    0x22,0x80,0x5c, // LOAD_CONST_SMALL_INT 92
    0x22,0x80,0x79, // LOAD_CONST_SMALL_INT 121
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x22,0x80,0x7d, // LOAD_CONST_SMALL_INT 125
    0x22,0x34, // LOAD_CONST_SMALL_INT 52
    0x22,0x38, // LOAD_CONST_SMALL_INT 56
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0x22,0x81,0x5f, // LOAD_CONST_SMALL_INT 223
    0x22,0x81,0x16, // LOAD_CONST_SMALL_INT 150
    0x22,0x80,0x4d, // LOAD_CONST_SMALL_INT 77
    0x22,0x81,0x1a, // LOAD_CONST_SMALL_INT 154
    0x22,0x81,0x1e, // LOAD_CONST_SMALL_INT 158
    0x22,0x80,0x51, // LOAD_CONST_SMALL_INT 81
    0x22,0x80,0x55, // LOAD_CONST_SMALL_INT 85
    0x22,0x80,0x59, // LOAD_CONST_SMALL_INT 89
    0x22,0x80,0x5d, // LOAD_CONST_SMALL_INT 93
    0x84, // LOAD_CONST_SMALL_INT 4
    0x88, // LOAD_CONST_SMALL_INT 8
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x90, // LOAD_CONST_SMALL_INT 16
    0x94, // LOAD_CONST_SMALL_INT 20
    0x98, // LOAD_CONST_SMALL_INT 24
    0x9c, // LOAD_CONST_SMALL_INT 28
    0x22,0x3d, // LOAD_CONST_SMALL_INT 61
    0x22,0x39, // LOAD_CONST_SMALL_INT 57
    0x22,0x80,0x7e, // LOAD_CONST_SMALL_INT 126
    0x22,0x35, // LOAD_CONST_SMALL_INT 53
    0x9d, // LOAD_CONST_SMALL_INT 29
    0x22,0x80,0x7a, // LOAD_CONST_SMALL_INT 122
    0x22,0x31, // LOAD_CONST_SMALL_INT 49
    0x99, // LOAD_CONST_SMALL_INT 25
    0x22,0x80,0x5e, // LOAD_CONST_SMALL_INT 94
    0x95, // LOAD_CONST_SMALL_INT 21
    0x22,0x81,0x3f, // LOAD_CONST_SMALL_INT 191
    0x22,0x80,0x76, // LOAD_CONST_SMALL_INT 118
    0xad, // LOAD_CONST_SMALL_INT 45
    0x22,0x80,0x5a, // LOAD_CONST_SMALL_INT 90
    0x22,0x3e, // LOAD_CONST_SMALL_INT 62
    0x91, // LOAD_CONST_SMALL_INT 17
    0x9e, // LOAD_CONST_SMALL_INT 30
    0x22,0x3a, // LOAD_CONST_SMALL_INT 58
    0x22,0x81,0x1f, // LOAD_CONST_SMALL_INT 159
    0x22,0x80,0x56, // LOAD_CONST_SMALL_INT 86
    0x8d, // LOAD_CONST_SMALL_INT 13
    0x9a, // LOAD_CONST_SMALL_INT 26
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0x22,0x36, // LOAD_CONST_SMALL_INT 54
    0x22,0x80,0x5f, // LOAD_CONST_SMALL_INT 95
    0x96, // LOAD_CONST_SMALL_INT 22
    0x22,0x3f, // LOAD_CONST_SMALL_INT 63
    0x9f, // LOAD_CONST_SMALL_INT 31
    0x22,0x81,0x3b, // LOAD_CONST_SMALL_INT 187
    0x22,0x80,0x72, // LOAD_CONST_SMALL_INT 114
    0x22,0x81,0x1b, // LOAD_CONST_SMALL_INT 155
    0xa9, // LOAD_CONST_SMALL_INT 41
    0x22,0x80,0x52, // LOAD_CONST_SMALL_INT 82
    0x22,0x80,0x7b, // LOAD_CONST_SMALL_INT 123
    0x22,0x80,0x5b, // LOAD_CONST_SMALL_INT 91
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0x22,0x3b, // LOAD_CONST_SMALL_INT 59
    0x89, // LOAD_CONST_SMALL_INT 9
    0x92, // LOAD_CONST_SMALL_INT 18
    0x9b, // LOAD_CONST_SMALL_INT 27
    0x97, // LOAD_CONST_SMALL_INT 23
    0x22,0x37, // LOAD_CONST_SMALL_INT 55
    0x22,0x80,0x57, // LOAD_CONST_SMALL_INT 87
    0x8e, // LOAD_CONST_SMALL_INT 14
    0x22,0x80,0x77, // LOAD_CONST_SMALL_INT 119
    0xae, // LOAD_CONST_SMALL_INT 46
    0x93, // LOAD_CONST_SMALL_INT 19
    0x22,0x33, // LOAD_CONST_SMALL_INT 51
    0x22,0x81,0x17, // LOAD_CONST_SMALL_INT 151
    0x22,0x80,0x4e, // LOAD_CONST_SMALL_INT 78
    0x22,0x80,0x53, // LOAD_CONST_SMALL_INT 83
    0x85, // LOAD_CONST_SMALL_INT 5
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x8f, // LOAD_CONST_SMALL_INT 15
    0xaf, // LOAD_CONST_SMALL_INT 47
    0x22,0x80,0x73, // LOAD_CONST_SMALL_INT 115
    0xaa, // LOAD_CONST_SMALL_INT 42
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x22,0x80,0x4f, // LOAD_CONST_SMALL_INT 79
    0x86, // LOAD_CONST_SMALL_INT 6
    0xab, // LOAD_CONST_SMALL_INT 43
    0x87, // LOAD_CONST_SMALL_INT 7
    0x22,0x81,0x37, // LOAD_CONST_SMALL_INT 183
    0x22,0x80,0x6e, // LOAD_CONST_SMALL_INT 110
    0x22,0x81,0x13, // LOAD_CONST_SMALL_INT 147
    0xa5, // LOAD_CONST_SMALL_INT 37
    0x22,0x80,0x4a, // LOAD_CONST_SMALL_INT 74
    0x22,0x80,0x6f, // LOAD_CONST_SMALL_INT 111
    0x22,0x80,0x4b, // LOAD_CONST_SMALL_INT 75
    0xa6, // LOAD_CONST_SMALL_INT 38
    0xa7, // LOAD_CONST_SMALL_INT 39
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0xa3, // LOAD_CONST_SMALL_INT 35
    0x22,0x80,0x47, // LOAD_CONST_SMALL_INT 71
    0x22,0x80,0x6b, // LOAD_CONST_SMALL_INT 107
    0xa2, // LOAD_CONST_SMALL_INT 34
    0x22,0x81,0x0f, // LOAD_CONST_SMALL_INT 143
    0x22,0x80,0x46, // LOAD_CONST_SMALL_INT 70
    0x22,0x80,0x43, // LOAD_CONST_SMALL_INT 67
    0x22,0x80,0x67, // LOAD_CONST_SMALL_INT 103
    0x22,0x81,0x33, // LOAD_CONST_SMALL_INT 179
    0x22,0x80,0x6a, // LOAD_CONST_SMALL_INT 106
    0x22,0x81,0x0b, // LOAD_CONST_SMALL_INT 139
    0xa1, // LOAD_CONST_SMALL_INT 33
    0x22,0x80,0x42, // LOAD_CONST_SMALL_INT 66
    0x22,0x80,0x63, // LOAD_CONST_SMALL_INT 99
    0x22,0x81,0x07, // LOAD_CONST_SMALL_INT 135
    0x22,0x81,0x2f, // LOAD_CONST_SMALL_INT 175
    0x22,0x80,0x66, // LOAD_CONST_SMALL_INT 102
    0x22,0x81,0x03, // LOAD_CONST_SMALL_INT 131
    0x22,0x81,0x2b, // LOAD_CONST_SMALL_INT 171
    0x22,0x80,0x62, // LOAD_CONST_SMALL_INT 98
    0x22,0x81,0x27, // LOAD_CONST_SMALL_INT 167
    0x22,0x81,0x23, // LOAD_CONST_SMALL_INT 163
    0x22,0x81,0x57, // LOAD_CONST_SMALL_INT 215
    0x22,0x81,0x0e, // LOAD_CONST_SMALL_INT 142
    0x22,0x81,0x53, // LOAD_CONST_SMALL_INT 211
    0x22,0x80,0x45, // LOAD_CONST_SMALL_INT 69
    0x22,0x81,0x0a, // LOAD_CONST_SMALL_INT 138
    0x22,0x81,0x4f, // LOAD_CONST_SMALL_INT 207
    0x22,0x81,0x4b, // LOAD_CONST_SMALL_INT 203
    0x22,0x81,0x06, // LOAD_CONST_SMALL_INT 134
    0x22,0x81,0x47, // LOAD_CONST_SMALL_INT 199
    0x22,0x80,0x41, // LOAD_CONST_SMALL_INT 65
    0x22,0x81,0x02, // LOAD_CONST_SMALL_INT 130
    0x22,0x81,0x43, // LOAD_CONST_SMALL_INT 195
    0x22,0x81,0x63, // LOAD_CONST_SMALL_INT 227
    0x22,0x81,0x67, // LOAD_CONST_SMALL_INT 231
    0x22,0x81,0x6b, // LOAD_CONST_SMALL_INT 235
    0x22,0x81,0x22, // LOAD_CONST_SMALL_INT 162
    0x22,0x81,0x6f, // LOAD_CONST_SMALL_INT 239
    0x22,0x81,0x26, // LOAD_CONST_SMALL_INT 166
    0x22,0x81,0x73, // LOAD_CONST_SMALL_INT 243
    0x22,0x81,0x2a, // LOAD_CONST_SMALL_INT 170
    0x22,0x80,0x61, // LOAD_CONST_SMALL_INT 97
    0x22,0x81,0x42, // LOAD_CONST_SMALL_INT 194
    0x22,0x81,0x46, // LOAD_CONST_SMALL_INT 198
    0x22,0x81,0x62, // LOAD_CONST_SMALL_INT 226
    0x22,0x81,0x77, // LOAD_CONST_SMALL_INT 247
    0x22,0x81,0x2e, // LOAD_CONST_SMALL_INT 174
    0x22,0x80,0x65, // LOAD_CONST_SMALL_INT 101
    0x22,0x81,0x4a, // LOAD_CONST_SMALL_INT 202
    0x22,0x81,0x66, // LOAD_CONST_SMALL_INT 230
    0x22,0x81,0x01, // LOAD_CONST_SMALL_INT 129
    0x22,0x81,0x6a, // LOAD_CONST_SMALL_INT 234
    0x22,0x81,0x21, // LOAD_CONST_SMALL_INT 161
    0x22,0x81,0x4e, // LOAD_CONST_SMALL_INT 206
    0x22,0x81,0x05, // LOAD_CONST_SMALL_INT 133
    0x22,0x81,0x41, // LOAD_CONST_SMALL_INT 193
    0x22,0x81,0x61, // LOAD_CONST_SMALL_INT 225
    0x22,0x81,0x6e, // LOAD_CONST_SMALL_INT 238
    0x22,0x81,0x25, // LOAD_CONST_SMALL_INT 165
    0x22,0x81,0x45, // LOAD_CONST_SMALL_INT 197
    0x22,0x81,0x65, // LOAD_CONST_SMALL_INT 229
    0x2b,0x82,0x00, // BUILD_LIST 256
    0x16,0x81,0x75, // STORE_NAME 'SORTED_HUE'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Colors = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Colors,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 947,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 256,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_tulip_Colors + 5,
        .line_info_top = fun_data_tulip_Colors + 48,
        .opcodes = fun_data_tulip_Colors + 48,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_Joy
static const byte fun_data_tulip_Joy[79] = {
    0x00,0x1c, // prelude
    0x0c, // names: Joy
    0x8b,0xb7,0x23,0x23,0x23,0x23,0x23,0x25,0x25,0x25,0x25,0x25,0x25, // code info
    0x11,0x81,0x44, // LOAD_NAME '__name__'
    0x16,0x81,0x45, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'Joy'
    0x16,0x81,0x46, // STORE_NAME '__qualname__'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x16,0x6d, // STORE_NAME 'R1'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x16,0x6c, // STORE_NAME 'L1'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x16,0x69, // STORE_NAME 'X'
    0x90, // LOAD_CONST_SMALL_INT 16
    0x16,0x68, // STORE_NAME 'A'
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x16,0x63, // STORE_NAME 'RIGHT'
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0x16,0x64, // STORE_NAME 'LEFT'
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0x16,0x66, // STORE_NAME 'DOWN'
    0x22,0x82,0x00, // LOAD_CONST_SMALL_INT 256
    0x16,0x65, // STORE_NAME 'UP'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x16,0x6a, // STORE_NAME 'START'
    0x22,0x88,0x00, // LOAD_CONST_SMALL_INT 1024
    0x16,0x6b, // STORE_NAME 'SELECT'
    0x22,0x90,0x00, // LOAD_CONST_SMALL_INT 2048
    0x16,0x56, // STORE_NAME 'Y'
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0x16,0x67, // STORE_NAME 'B'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_Joy = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_Joy,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 79,
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
        .qstr_block_name_idx = 12,
        .line_info = fun_data_tulip_Joy + 3,
        .line_info_top = fun_data_tulip_Joy + 16,
        .opcodes = fun_data_tulip_Joy + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_version
static const byte fun_data_tulip_version[39] = {
    0x28,0x08, // prelude
    0x38, // names: version
    0x80,0xc6,0x26, // code info
    0x12,0x81,0x76, // LOAD_GLOBAL 'build_strings'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x14,0x39, // LOAD_METHOD 'replace'
    0x10,0x3a, // LOAD_CONST_STRING '-'
    0x10,0x3b, // LOAD_CONST_STRING ''
    0x36,0x02, // CALL_METHOD 2
    0x10,0x3a, // LOAD_CONST_STRING '-'
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x14,0x39, // LOAD_METHOD 'replace'
    0x10,0x3c, // LOAD_CONST_STRING '-dirty'
    0x10,0x3b, // LOAD_CONST_STRING ''
    0x36,0x02, // CALL_METHOD 2
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_version = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_version,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 39,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 56,
        .line_info = fun_data_tulip_version + 3,
        .line_info_top = fun_data_tulip_version + 6,
        .opcodes = fun_data_tulip_version + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_free_disk_bytes
static const byte fun_data_tulip_free_disk_bytes[28] = {
    0x20,0x0a, // prelude
    0x3d, // names: free_disk_bytes
    0x80,0xca,0x25,0x28, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x3e, // IMPORT_NAME 'os'
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x14,0x3f, // LOAD_METHOD 'statvfs'
    0x10,0x40, // LOAD_CONST_STRING '.'
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_free_disk_bytes = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_free_disk_bytes,
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
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 61,
        .line_info = fun_data_tulip_free_disk_bytes + 3,
        .line_info_top = fun_data_tulip_free_disk_bytes + 7,
        .opcodes = fun_data_tulip_free_disk_bytes + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_upgrade
static const byte fun_data_tulip_upgrade[551] = {
    0xe0,0x12,0x62, // prelude
    0x41, // names: upgrade
    0x80,0xd1,0x34,0x22,0x54,0x28,0x63,0x2f,0x34,0x28,0x62,0x24,0x23,0x23,0x31,0x26,0x29,0x24,0x2f,0x39,0x2d,0x26,0x2d,0x26,0x2d,0x26,0x30,0x27,0x26,0x23,0x30,0x52,0x27,0x23,0x2e,0x2b,0x28,0x36,0x2a,0x2a,0x31,0x29,0x30,0x26,0x48,0x53,0x26,0x32, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'world'
    0xc0, // STORE_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x42, // IMPORT_NAME 'time'
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x43, // IMPORT_NAME 'sys'
    0xc2, // STORE_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x3e, // IMPORT_NAME 'os'
    0xc3, // STORE_FAST 3
    0x48,0x0c, // SETUP_EXCEPT 12
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x44, // IMPORT_NAME 'esp32'
    0xc4, // STORE_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x45, // IMPORT_NAME 'machine'
    0xc5, // STORE_FAST 5
    0x4a,0x13, // POP_EXCEPT_JUMP 19
    0x57, // DUP_TOP
    0x12,0x81,0x77, // LOAD_GLOBAL 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0xb4, // LOAD_FAST 4
    0x14,0x46, // LOAD_METHOD 'Partition'
    0xb4, // LOAD_FAST 4
    0x13,0x46, // LOAD_ATTR 'Partition'
    0x13,0x47, // LOAD_ATTR 'RUNNING'
    0x36,0x01, // CALL_METHOD 1
    0x14,0x48, // LOAD_METHOD 'get_next_update'
    0x36,0x00, // CALL_METHOD 0
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0xb6, // LOAD_FAST 6
    0x14,0x49, // LOAD_METHOD 'info'
    0x36,0x00, // CALL_METHOD 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x14,0x4a, // LOAD_METHOD 'startswith'
    0x10,0x4b, // LOAD_CONST_STRING 'ota'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0xc7, // STORE_FAST 7
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xc8, // STORE_FAST 8
    0x10,0x4c, // LOAD_CONST_STRING 'tulipcc-'
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x4d, // LOAD_METHOD 'files'
    0x10,0x4e, // LOAD_CONST_STRING 'limit'
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0x10,0x4f, // LOAD_CONST_STRING 'room_id'
    0xb0, // LOAD_FAST 0
    0x13,0x50, // LOAD_ATTR 'firmware_room_id'
    0x36,0x84,0x00, // CALL_METHOD 512
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x14,0x51, // LOAD_METHOD 'reverse'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x81,0x79, // LOAD_GLOBAL 'len'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0xe5,0x82, // POP_JUMP_IF_FALSE 357
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0xbb, // LOAD_FAST 11
    0x10,0x52, // LOAD_CONST_STRING 'filename'
    0x55, // LOAD_SUBSCR
    0x12,0x81,0x79, // LOAD_GLOBAL 'len'
    0xb9, // LOAD_FAST 9
    0x34,0x01, // CALL_FUNCTION 1
    0x7c, // LOAD_CONST_SMALL_INT -4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xcc, // STORE_FAST 12
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xbc, // LOAD_FAST 12
    0xb0, // LOAD_FAST 0
    0x14,0x53, // LOAD_METHOD 'nice_time'
    0xbb, // LOAD_FAST 11
    0x10,0x54, // LOAD_CONST_STRING 'age_ms'
    0x55, // LOAD_SUBSCR
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x01, // CALL_METHOD 1
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x12,0x38, // LOAD_GLOBAL 'version'
    0x34,0x00, // CALL_FUNCTION 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x10,0x55, // LOAD_CONST_STRING 'end'
    0x10,0x3b, // LOAD_CONST_STRING ''
    0x34,0x82,0x01, // CALL_FUNCTION 257
    0x59, // POP_TOP
    0x12,0x81,0x7a, // LOAD_GLOBAL 'input'
    0x34,0x00, // CALL_FUNCTION 0
    0xcd, // STORE_FAST 13
    0xbd, // LOAD_FAST 13
    0x10,0x56, // LOAD_CONST_STRING 'Y'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0xbd, // LOAD_FAST 13
    0x10,0x57, // LOAD_CONST_STRING 'y'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x86,0x82, // POP_JUMP_IF_FALSE 262
    0x12,0x81,0x7b, // LOAD_GLOBAL 'tfb_log_start'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xbb, // LOAD_FAST 11
    0x10,0x52, // LOAD_CONST_STRING 'filename'
    0x55, // LOAD_SUBSCR
    0xb8, // LOAD_FAST 8
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x58, // LOAD_METHOD 'sleep'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x81,0x7c, // LOAD_GLOBAL 'display_stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x48,0xb7,0x01, // SETUP_EXCEPT 183
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0xb6, // LOAD_FAST 6
    0x14,0x49, // LOAD_METHOD 'info'
    0x36,0x00, // CALL_METHOD 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xb0, // LOAD_FAST 0
    0x13,0x59, // LOAD_ATTR 'host'
    0xbb, // LOAD_FAST 11
    0x10,0x5a, // LOAD_CONST_STRING 'url'
    0x55, // LOAD_SUBSCR
    0x86, // LOAD_CONST_SMALL_INT 6
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0xce, // STORE_FAST 14
    0xb0, // LOAD_FAST 0
    0x14,0x5b, // LOAD_METHOD 'matrix_get'
    0xbe, // LOAD_FAST 14
    0x36,0x01, // CALL_METHOD 1
    0xcf, // STORE_FAST 15
    0x80, // LOAD_CONST_SMALL_INT 0
    0x26,0x10, // STORE_FAST_N 16
    0xbf, // LOAD_FAST 15
    0x14,0x5c, // LOAD_METHOD 'generate'
    0x10,0x5d, // LOAD_CONST_STRING 'chunk_size'
    0xb7, // LOAD_FAST 7
    0x36,0x82,0x00, // CALL_METHOD 256
    0x5f, // GET_ITER_STACK
    0x4b,0x59, // FOR_ITER 89
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x81,0x79, // LOAD_GLOBAL 'len'
    0x24,0x11, // LOAD_FAST_N 17
    0x34,0x01, // CALL_FUNCTION 1
    0xb7, // LOAD_FAST 7
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x5e, // POP_JUMP_IF_FALSE 30
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x24,0x11, // LOAD_FAST_N 17
    0x12,0x81,0x7d, // LOAD_GLOBAL 'bytes'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0xb7, // LOAD_FAST 7
    0x12,0x81,0x79, // LOAD_GLOBAL 'len'
    0x24,0x11, // LOAD_FAST_N 17
    0x34,0x01, // CALL_FUNCTION 1
    0xf3, // BINARY_OP 28 __sub__
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x11, // STORE_FAST_N 17
    0xb6, // LOAD_FAST 6
    0x14,0x5e, // LOAD_METHOD 'writeblocks'
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x11, // LOAD_FAST_N 17
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x24,0x10, // LOAD_FAST_N 16
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xf8, // BINARY_OP 33 __mod__
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x10, // LOAD_FAST_N 16
    0xb7, // LOAD_FAST 7
    0xf4, // BINARY_OP 29 __mul__
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x24,0x10, // LOAD_FAST_N 16
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x10, // STORE_FAST_N 16
    0x42,0xa5,0x7f, // JUMP -91
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xb6, // LOAD_FAST 6
    0x14,0x49, // LOAD_METHOD 'info'
    0x36,0x00, // CALL_METHOD 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0x14,0x5f, // LOAD_METHOD 'set_boot'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x60, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x4a,0x23, // POP_EXCEPT_JUMP 35
    0x57, // DUP_TOP
    0x12,0x81,0x7e, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0x26,0x12, // STORE_FAST_N 18
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x7f, // LOAD_GLOBAL 'display_start'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x61, // LOAD_METHOD 'print_exception'
    0x24,0x12, // LOAD_FAST_N 18
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x26,0x12, // STORE_FAST_N 18
    0x28,0x12, // DELETE_FAST 18
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x82,0x00, // LOAD_GLOBAL 'tfb_log_stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_upgrade = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_upgrade,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 551,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 29,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 65,
        .line_info = fun_data_tulip_upgrade + 4,
        .line_info_top = fun_data_tulip_upgrade + 52,
        .opcodes = fun_data_tulip_upgrade + 52,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_joyk
static const byte fun_data_tulip_joyk[201] = {
    0x40,0x24, // prelude
    0x62, // names: joyk
    0x90,0x0f,0x26,0x2b,0x26,0x2e,0x2e,0x2e,0x2e,0x2c,0x2c,0x2c,0x2c,0x2c,0x2d,0x2c,0x2f, // code info
    0x12,0x82,0x01, // LOAD_GLOBAL 'joy'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0x12,0x82,0x02, // LOAD_GLOBAL 'keys'
    0x34,0x00, // CALL_FUNCTION 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x85, // LOAD_CONST_SMALL_INT 5
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x9d,0x01, // FOR_ITER 157
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x22,0x80,0x4f, // LOAD_CONST_SMALL_INT 79
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x63, // LOAD_ATTR 'RIGHT'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x64, // LOAD_ATTR 'LEFT'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x22,0x80,0x52, // LOAD_CONST_SMALL_INT 82
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x65, // LOAD_ATTR 'UP'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x22,0x80,0x51, // LOAD_CONST_SMALL_INT 81
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x66, // LOAD_ATTR 'DOWN'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x9d, // LOAD_CONST_SMALL_INT 29
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x67, // LOAD_ATTR 'B'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x9b, // LOAD_CONST_SMALL_INT 27
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x68, // LOAD_ATTR 'A'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x56, // LOAD_ATTR 'Y'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x96, // LOAD_CONST_SMALL_INT 22
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x69, // LOAD_ATTR 'X'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0xa8, // LOAD_CONST_SMALL_INT 40
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x6a, // LOAD_ATTR 'START'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x22,0x34, // LOAD_CONST_SMALL_INT 52
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x6b, // LOAD_ATTR 'SELECT'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x94, // LOAD_CONST_SMALL_INT 20
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x6c, // LOAD_ATTR 'L1'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0xb2, // LOAD_FAST 2
    0x9a, // LOAD_CONST_SMALL_INT 26
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x12,0x0c, // LOAD_GLOBAL 'Joy'
    0x13,0x6d, // LOAD_ATTR 'R1'
    0xed, // BINARY_OP 22 __or__
    0xc0, // STORE_FAST 0
    0x42,0xe0,0x7e, // JUMP -160
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_joyk = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_joyk,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 201,
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
        .qstr_block_name_idx = 98,
        .line_info = fun_data_tulip_joyk + 3,
        .line_info_top = fun_data_tulip_joyk + 20,
        .opcodes = fun_data_tulip_joyk + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_run
static const byte fun_data_tulip_run[118] = {
    0xc9,0x02,0x1c, // prelude
    0x6e,0x82,0x03, // names: run, module
    0x90,0x23,0x2a,0x26,0x22,0x54,0x4c,0x2a,0x30,0x2a,0x26, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x6f, // IMPORT_NAME 'gc'
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x43, // IMPORT_NAME 'sys'
    0xc2, // STORE_FAST 2
    0x12,0x05, // LOAD_GLOBAL 'cd'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x48,0x0c, // SETUP_EXCEPT 12
    0x12,0x82,0x04, // LOAD_GLOBAL 'exec'
    0x10,0x70, // LOAD_CONST_STRING 'import %s'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x2e, // POP_EXCEPT_JUMP 46
    0x57, // DUP_TOP
    0x12,0x82,0x05, // LOAD_GLOBAL 'KeyboardInterrupt'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x24, // POP_EXCEPT_JUMP 36
    0x57, // DUP_TOP
    0x12,0x81,0x7e, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5c, // POP_JUMP_IF_FALSE 28
    0xc3, // STORE_FAST 3
    0x49,0x12, // SETUP_FINALLY 18
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x61, // LOAD_METHOD 'print_exception'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x28,0x03, // DELETE_FAST 3
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x82,0x04, // LOAD_GLOBAL 'exec'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x71, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x05, // LOAD_GLOBAL 'cd'
    0x10,0x72, // LOAD_CONST_STRING '..'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_run = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_run,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 118,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 110,
        .line_info = fun_data_tulip_run + 6,
        .line_info_top = fun_data_tulip_run + 17,
        .opcodes = fun_data_tulip_run + 17,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_url_save
static const byte fun_data_tulip_url_save[52] = {
    0xd0,0x84,0x01,0x18, // prelude
    0x73,0x5a,0x52,0x82,0x06,0x76, // names: url_save, url, filename, mode, headers
    0x90,0x32,0x25,0x26,0x31,0x26, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x74, // IMPORT_NAME 'urequests'
    0xc4, // STORE_FAST 4
    0x12,0x81,0x7c, // LOAD_GLOBAL 'display_stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x14,0x75, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x10,0x76, // LOAD_CONST_STRING 'headers'
    0xb3, // LOAD_FAST 3
    0x36,0x82,0x01, // CALL_METHOD 257
    0x14,0x77, // LOAD_METHOD 'save'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0xc5, // STORE_FAST 5
    0x12,0x81,0x7f, // LOAD_GLOBAL 'display_start'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_url_save = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_tulip_url_save,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 115,
        .line_info = fun_data_tulip_url_save + 10,
        .line_info_top = fun_data_tulip_url_save + 16,
        .opcodes = fun_data_tulip_url_save + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_url_get
static const byte fun_data_tulip_url_get[28] = {
    0xc2,0x01,0x0e, // prelude
    0x78,0x5a,0x76, // names: url_get, url, headers
    0x90,0x39,0x25,0x2b, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x74, // IMPORT_NAME 'urequests'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x75, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x10,0x76, // LOAD_CONST_STRING 'headers'
    0xb1, // LOAD_FAST 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_url_get = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_tulip_url_get,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 28,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 120,
        .line_info = fun_data_tulip_url_get + 6,
        .line_info_top = fun_data_tulip_url_get + 10,
        .opcodes = fun_data_tulip_url_get + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_screenshot
static const byte fun_data_tulip_screenshot[146] = {
    0x81,0x11,0x24, // prelude
    0x79,0x52, // names: screenshot, filename
    0x90,0x3e,0x2b,0x26,0x27,0x22,0x2b,0x28,0x25,0x23,0x23,0x23,0x2e,0x39,0x26,0x32, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x7a, // LOAD_CONST_STRING 'rm'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x06, // IMPORT_NAME 'upysh'
    0x1c,0x7a, // IMPORT_FROM 'rm'
    0xc1, // STORE_FAST 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x82,0x07, // LOAD_GLOBAL 'int_screenshot'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x81,0x08, // LOAD_GLOBAL 'ip'
    0x34,0x00, // CALL_FUNCTION 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0xd5,0x80, // POP_JUMP_IF_FALSE 85
    0x12,0x82,0x07, // LOAD_GLOBAL 'int_screenshot'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x74, // IMPORT_NAME 'urequests'
    0xc2, // STORE_FAST 2
    0x23,0x12, // LOAD_CONST_OBJ 18
    0xc3, // STORE_FAST 3
    0x23,0x13, // LOAD_CONST_OBJ 19
    0xc4, // STORE_FAST 4
    0x23,0x14, // LOAD_CONST_OBJ 20
    0xc5, // STORE_FAST 5
    0x12,0x82,0x08, // LOAD_GLOBAL 'open'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x10,0x7b, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x14,0x7c, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc6, // STORE_FAST 6
    0xb2, // LOAD_FAST 2
    0x14,0x7d, // LOAD_METHOD 'post'
    0xb3, // LOAD_FAST 3
    0x10,0x7e, // LOAD_CONST_STRING 'data'
    0xb6, // LOAD_FAST 6
    0x10,0x76, // LOAD_CONST_STRING 'headers'
    0x2c,0x02, // BUILD_MAP 2
    0xb4, // LOAD_FAST 4
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x62, // STORE_MAP
    0xb5, // LOAD_FAST 5
    0x23,0x16, // LOAD_CONST_OBJ 22
    0x62, // STORE_MAP
    0x36,0x84,0x01, // CALL_METHOD 513
    0x13,0x7f, // LOAD_ATTR 'content'
    0xc7, // STORE_FAST 7
    0xb1, // LOAD_FAST 1
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x23,0x17, // LOAD_CONST_OBJ 23
    0xb7, // LOAD_FAST 7
    0x14,0x81,0x00, // LOAD_METHOD 'decode'
    0x10,0x81,0x01, // LOAD_CONST_STRING 'ascii'
    0x36,0x01, // CALL_METHOD 1
    0x8f, // LOAD_CONST_SMALL_INT 15
    0x96, // LOAD_CONST_SMALL_INT 22
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
    0x23,0x18, // LOAD_CONST_OBJ 24
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_screenshot = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_screenshot,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 146,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 121,
        .line_info = fun_data_tulip_screenshot + 5,
        .line_info_top = fun_data_tulip_screenshot + 21,
        .opcodes = fun_data_tulip_screenshot + 21,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_ansi_fg
static const byte fun_data_tulip_ansi_fg[14] = {
    0x11,0x0c, // prelude
    0x81,0x02,0x82,0x09, // names: ansi_fg, pal_idx
    0x90,0x51, // code info
    0x10,0x81,0x03, // LOAD_CONST_STRING '\x1b[38;5;%dm'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_ansi_fg = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_ansi_fg,
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
        .qstr_block_name_idx = 130,
        .line_info = fun_data_tulip_ansi_fg + 6,
        .line_info_top = fun_data_tulip_ansi_fg + 8,
        .opcodes = fun_data_tulip_ansi_fg + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_ansi_bg
static const byte fun_data_tulip_ansi_bg[14] = {
    0x11,0x0c, // prelude
    0x81,0x04,0x82,0x09, // names: ansi_bg, pal_idx
    0x90,0x55, // code info
    0x10,0x81,0x05, // LOAD_CONST_STRING '\x1b[48;5;%dm'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_ansi_bg = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_ansi_bg,
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
        .qstr_block_name_idx = 132,
        .line_info = fun_data_tulip_ansi_bg + 6,
        .line_info_top = fun_data_tulip_ansi_bg + 8,
        .opcodes = fun_data_tulip_ansi_bg + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_color
static const byte fun_data_tulip_color[52] = {
    0x33,0x1c, // prelude
    0x81,0x06,0x81,0x1d,0x82,0x0a,0x82,0x0b, // names: color, r, g, b
    0x90,0x58,0x22,0x28,0x2a,0x2a, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x22,0x81,0x60, // LOAD_CONST_SMALL_INT 224
    0xef, // BINARY_OP 24 __and__
    0xe0, // BINARY_OP 9 __ior__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xb1, // LOAD_FAST 1
    0x22,0x81,0x60, // LOAD_CONST_SMALL_INT 224
    0xef, // BINARY_OP 24 __and__
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf1, // BINARY_OP 26 __rshift__
    0xe0, // BINARY_OP 9 __ior__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x22,0x81,0x40, // LOAD_CONST_SMALL_INT 192
    0xef, // BINARY_OP 24 __and__
    0x86, // LOAD_CONST_SMALL_INT 6
    0xf1, // BINARY_OP 26 __rshift__
    0xe0, // BINARY_OP 9 __ior__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_color = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_tulip_color,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 52,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 134,
        .line_info = fun_data_tulip_color + 10,
        .line_info_top = fun_data_tulip_color + 16,
        .opcodes = fun_data_tulip_color + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_rgb
static const byte fun_data_tulip_rgb[39] = {
    0x31,0x12, // prelude
    0x81,0x07,0x82,0x0c, // names: rgb, px0
    0x90,0x60,0x26,0x28,0x28, // code info
    0xb0, // LOAD_FAST 0
    0x22,0x81,0x60, // LOAD_CONST_SMALL_INT 224
    0xef, // BINARY_OP 24 __and__
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf0, // BINARY_OP 25 __lshift__
    0x22,0x81,0x60, // LOAD_CONST_SMALL_INT 224
    0xef, // BINARY_OP 24 __and__
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x86, // LOAD_CONST_SMALL_INT 6
    0xf0, // BINARY_OP 25 __lshift__
    0x22,0x81,0x40, // LOAD_CONST_SMALL_INT 192
    0xef, // BINARY_OP 24 __and__
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x2a,0x03, // BUILD_TUPLE 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_rgb = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_rgb,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 39,
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
        .qstr_block_name_idx = 135,
        .line_info = fun_data_tulip_rgb + 6,
        .line_info_top = fun_data_tulip_rgb + 11,
        .opcodes = fun_data_tulip_rgb + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_ip
static const byte fun_data_tulip_ip[67] = {
    0x3c,0x14, // prelude
    0x81,0x08, // names: ip
    0x90,0x66,0x22,0x50,0x25,0x2b,0x28,0x49, // code info
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x09, // IMPORT_NAME 'network'
    0xc0, // STORE_FAST 0
    0x4a,0x0d, // POP_EXCEPT_JUMP 13
    0x57, // DUP_TOP
    0x12,0x81,0x77, // LOAD_GLOBAL 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x59, // POP_TOP
    0x10,0x81,0x0a, // LOAD_CONST_STRING '127.0.0.1'
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x14,0x81,0x0b, // LOAD_METHOD 'WLAN'
    0xb0, // LOAD_FAST 0
    0x13,0x81,0x0c, // LOAD_ATTR 'STA_IF'
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x81,0x0d, // LOAD_METHOD 'isconnected'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb1, // LOAD_FAST 1
    0x14,0x81,0x0e, // LOAD_METHOD 'ifconfig'
    0x36,0x00, // CALL_METHOD 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_ip = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_ip,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 67,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 136,
        .line_info = fun_data_tulip_ip + 4,
        .line_info_top = fun_data_tulip_ip + 12,
        .opcodes = fun_data_tulip_ip + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_set_time
static const byte fun_data_tulip_set_time[44] = {
    0x10,0x10, // prelude
    0x81,0x0f, // names: set_time
    0x90,0x71,0x26,0x29,0x28,0x22, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x10, // IMPORT_NAME 'ntptime'
    0xc0, // STORE_FAST 0
    0x12,0x81,0x08, // LOAD_GLOBAL 'ip'
    0x34,0x00, // CALL_FUNCTION 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x19, // LOAD_CONST_OBJ 25
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x81,0x11, // LOAD_METHOD 'settime'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_set_time = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip_set_time,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 44,
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
        .qstr_block_name_idx = 143,
        .line_info = fun_data_tulip_set_time + 4,
        .line_info_top = fun_data_tulip_set_time + 10,
        .opcodes = fun_data_tulip_set_time + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_wifi
static const byte fun_data_tulip_wifi[96] = {
    0xd3,0x01,0x24, // prelude
    0x81,0x12,0x82,0x0d,0x82,0x0e,0x82,0x0f, // names: wifi, ssid, passwd, wait_timeout
    0x90,0x79,0x2b,0x2b,0x28,0x29,0x22,0x22,0x24,0x36, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x09, // IMPORT_NAME 'network'
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x42, // IMPORT_NAME 'time'
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x14,0x81,0x0b, // LOAD_METHOD 'WLAN'
    0xb3, // LOAD_FAST 3
    0x13,0x81,0x0c, // LOAD_ATTR 'STA_IF'
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x81,0x13, // LOAD_METHOD 'active'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x81,0x14, // LOAD_METHOD 'connect'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc6, // STORE_FAST 6
    0x42,0x4b, // JUMP 11
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0x14,0x58, // LOAD_METHOD 'sleep'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x81,0x0d, // LOAD_METHOD 'isconnected'
    0x36,0x00, // CALL_METHOD 0
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x26, // POP_JUMP_IF_TRUE -26
    0x12,0x81,0x08, // LOAD_GLOBAL 'ip'
    0x34,0x00, // CALL_FUNCTION 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_wifi = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_tulip_wifi,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 96,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 146,
        .line_info = fun_data_tulip_wifi + 11,
        .line_info_top = fun_data_tulip_wifi + 21,
        .opcodes = fun_data_tulip_wifi + 21,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_tar_create
static const byte fun_data_tulip_tar_create[49] = {
    0x31,0x12, // prelude
    0x81,0x15,0x82,0x10, // names: tar_create, directory
    0x90,0x85,0x26,0x2f,0x28, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x16, // IMPORT_NAME 'utarfile'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x81,0x17, // LOAD_METHOD 'TarFile'
    0xb0, // LOAD_FAST 0
    0x10,0x81,0x18, // LOAD_CONST_STRING '.tar'
    0xf2, // BINARY_OP 27 __add__
    0x10,0x81,0x19, // LOAD_CONST_STRING 'w'
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x81,0x1a, // LOAD_METHOD 'add'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x81,0x1b, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_tar_create = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_tulip_tar_create,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 49,
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
        .qstr_block_name_idx = 149,
        .line_info = fun_data_tulip_tar_create + 6,
        .line_info_top = fun_data_tulip_tar_create + 11,
        .opcodes = fun_data_tulip_tar_create + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of tulip__lt_module_gt_
// frozen bytecode for file tulip.py, scope tulip_tar_extract
static const byte fun_data_tulip_tar_extract[332] = {
    0xa2,0x13,0x3c, // prelude
    0x81,0x1c,0x82,0x11,0x82,0x12, // names: tar_extract, file_name, show_progress
    0x90,0x8b,0x25,0x26,0x46,0x2b,0x2d,0x26,0x2c,0x2a,0x22,0x5f,0x3a,0x53,0x36,0x53,0x2f,0x28,0x26,0x22,0x2e,0x28,0x56,0x3c, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x3e, // IMPORT_NAME 'os'
    0xc2, // STORE_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x16, // IMPORT_NAME 'utarfile'
    0xc3, // STORE_FAST 3
    0x12,0x81,0x7c, // LOAD_GLOBAL 'display_stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x14,0x81,0x17, // LOAD_METHOD 'TarFile'
    0xb0, // LOAD_FAST 0
    0x10,0x81,0x1d, // LOAD_CONST_STRING 'r'
    0x36,0x02, // CALL_METHOD 2
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x10,0x81,0x1e, // LOAD_CONST_STRING 'extracting'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x5f, // GET_ITER_STACK
    0x4b,0xf5,0x01, // FOR_ITER 245
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x1f, // LOAD_ATTR 'type'
    0xb3, // LOAD_FAST 3
    0x13,0x81,0x20, // LOAD_ATTR 'DIRTYPE'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xd8,0x80, // POP_JUMP_IF_FALSE 88
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x10,0x81,0x22, // LOAD_CONST_STRING './'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x7b, // POP_JUMP_IF_FALSE 59
    0x48,0x15, // SETUP_EXCEPT 21
    0xb2, // LOAD_FAST 2
    0x14,0x81,0x23, // LOAD_METHOD 'mkdir'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x14,0x81,0x24, // LOAD_METHOD 'strip'
    0x10,0x81,0x25, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x24, // POP_EXCEPT_JUMP 36
    0x57, // DUP_TOP
    0x12,0x82,0x13, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5c, // POP_JUMP_IF_FALSE 28
    0xc6, // STORE_FAST 6
    0x49,0x12, // SETUP_FINALLY 18
    0xb1, // LOAD_FAST 1
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x1a, // LOAD_CONST_OBJ 26
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x23,0x1b, // LOAD_CONST_OBJ 27
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x10,0x81,0x26, // LOAD_CONST_STRING 'making:'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0x8d,0x81, // JUMP 141
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x14,0x4a, // LOAD_METHOD 'startswith'
    0x10,0x40, // LOAD_CONST_STRING '.'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x10,0x81,0x27, // LOAD_CONST_STRING '._'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0xdd, // BINARY_OP 6 <in>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0xb1, // LOAD_FAST 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x10,0x81,0x28, // LOAD_CONST_STRING 'ignoring'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0xe4,0x80, // JUMP 100
    0xb1, // LOAD_FAST 1
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x23,0x1c, // LOAD_CONST_OBJ 28
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x14,0x81,0x29, // LOAD_METHOD 'extractfile'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0x14,0x7c, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc8, // STORE_FAST 8
    0x48,0x22, // SETUP_EXCEPT 34
    0x12,0x82,0x08, // LOAD_GLOBAL 'open'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x10,0x0d, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x11, // SETUP_WITH 17
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x14,0x81,0x2a, // LOAD_METHOD 'write'
    0xb8, // LOAD_FAST 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0x14,0x81,0x1b, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x4a,0x23, // POP_EXCEPT_JUMP 35
    0x57, // DUP_TOP
    0x12,0x82,0x13, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0xc6, // STORE_FAST 6
    0x49,0x11, // SETUP_FINALLY 17
    0xb1, // LOAD_FAST 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x81,0x78, // LOAD_GLOBAL 'print'
    0x10,0x81,0x2b, // LOAD_CONST_STRING 'borked on:'
    0xb5, // LOAD_FAST 5
    0x13,0x81,0x21, // LOAD_ATTR 'name'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x88,0x7e, // JUMP -248
    0x12,0x81,0x7f, // LOAD_GLOBAL 'display_start'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_tulip_tar_extract = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_tulip_tar_extract,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 332,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 21,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 156,
        .line_info = fun_data_tulip_tar_extract + 9,
        .line_info_top = fun_data_tulip_tar_extract + 33,
        .opcodes = fun_data_tulip_tar_extract + 33,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_tulip__lt_module_gt_[] = {
    &raw_code_tulip_screen_size,
    &raw_code_tulip_Game,
    &raw_code_tulip_Sprite,
    &raw_code_tulip_Player,
    &raw_code_tulip_Colors,
    &raw_code_tulip_Joy,
    &raw_code_tulip_version,
    &raw_code_tulip_free_disk_bytes,
    &raw_code_tulip_upgrade,
    &raw_code_tulip_joyk,
    &raw_code_tulip_run,
    &raw_code_tulip_url_save,
    &raw_code_tulip_url_get,
    &raw_code_tulip_screenshot,
    &raw_code_tulip_ansi_fg,
    &raw_code_tulip_ansi_bg,
    &raw_code_tulip_color,
    &raw_code_tulip_rgb,
    &raw_code_tulip_ip,
    &raw_code_tulip_set_time,
    &raw_code_tulip_wifi,
    &raw_code_tulip_tar_create,
    &raw_code_tulip_tar_extract,
};

static const mp_raw_code_t raw_code_tulip__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_tulip__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 256,
    #endif
    .children = (void *)&children_tulip__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 23,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_tulip__lt_module_gt_ + 3,
        .line_info_top = fun_data_tulip__lt_module_gt_ + 54,
        .opcodes = fun_data_tulip__lt_module_gt_ + 54,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_tulip[294] = {
    MP_QSTR_tulip_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR__tulip,
    MP_QSTR_world,
    MP_QSTR_cd,
    MP_QSTR_upysh,
    MP_QSTR_alles,
    MP_QSTR_Game,
    MP_QSTR_Sprite,
    MP_QSTR_Player,
    MP_QSTR_Colors,
    MP_QSTR_Joy,
    MP_QSTR_wb,
    MP_QSTR_User_hyphen_Agent,
    MP_QSTR_screen_size,
    MP_QSTR__0x1b__bracket_open_0_semicolon_30m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_31m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_32m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_33m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_34m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_35m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_36m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_37m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_30m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_31m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_32m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_33m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_34m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_35m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_36m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_37m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_40m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_41m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_42m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_43m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_44m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_45m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_46m,
    MP_QSTR__0x1b__bracket_open_0_semicolon_47m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_40m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_41m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_42m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_43m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_44m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_45m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_46m,
    MP_QSTR__0x1b__bracket_open_1_semicolon_47m,
    MP_QSTR__0x1b__bracket_open_1m,
    MP_QSTR__0x1b__bracket_open_2m,
    MP_QSTR__0x1b__bracket_open_3m,
    MP_QSTR__0x1b__bracket_open_4m,
    MP_QSTR__0x1b__bracket_open_5m,
    MP_QSTR__0x1b__bracket_open_7m,
    MP_QSTR__0x1b__bracket_open_9m,
    MP_QSTR__0x1b__bracket_open_0m,
    MP_QSTR_version,
    MP_QSTR_replace,
    MP_QSTR__hyphen_,
    MP_QSTR_,
    MP_QSTR__hyphen_dirty,
    MP_QSTR_free_disk_bytes,
    MP_QSTR_os,
    MP_QSTR_statvfs,
    MP_QSTR__dot_,
    MP_QSTR_upgrade,
    MP_QSTR_time,
    MP_QSTR_sys,
    MP_QSTR_esp32,
    MP_QSTR_machine,
    MP_QSTR_Partition,
    MP_QSTR_RUNNING,
    MP_QSTR_get_next_update,
    MP_QSTR_info,
    MP_QSTR_startswith,
    MP_QSTR_ota,
    MP_QSTR_tulipcc_hyphen_,
    MP_QSTR_files,
    MP_QSTR_limit,
    MP_QSTR_room_id,
    MP_QSTR_firmware_room_id,
    MP_QSTR_reverse,
    MP_QSTR_filename,
    MP_QSTR_nice_time,
    MP_QSTR_age_ms,
    MP_QSTR_end,
    MP_QSTR_Y,
    MP_QSTR_y,
    MP_QSTR_sleep,
    MP_QSTR_host,
    MP_QSTR_url,
    MP_QSTR_matrix_get,
    MP_QSTR_generate,
    MP_QSTR_chunk_size,
    MP_QSTR_writeblocks,
    MP_QSTR_set_boot,
    MP_QSTR_reset,
    MP_QSTR_print_exception,
    MP_QSTR_joyk,
    MP_QSTR_RIGHT,
    MP_QSTR_LEFT,
    MP_QSTR_UP,
    MP_QSTR_DOWN,
    MP_QSTR_B,
    MP_QSTR_A,
    MP_QSTR_X,
    MP_QSTR_START,
    MP_QSTR_SELECT,
    MP_QSTR_L1,
    MP_QSTR_R1,
    MP_QSTR_run,
    MP_QSTR_gc,
    MP_QSTR_import_space__percent_s,
    MP_QSTR_collect,
    MP_QSTR__dot__dot_,
    MP_QSTR_url_save,
    MP_QSTR_urequests,
    MP_QSTR_get,
    MP_QSTR_headers,
    MP_QSTR_save,
    MP_QSTR_url_get,
    MP_QSTR_screenshot,
    MP_QSTR_rm,
    MP_QSTR_rb,
    MP_QSTR_read,
    MP_QSTR_post,
    MP_QSTR_data,
    MP_QSTR_content,
    MP_QSTR_decode,
    MP_QSTR_ascii,
    MP_QSTR_ansi_fg,
    MP_QSTR__0x1b__bracket_open_38_semicolon_5_semicolon__percent_dm,
    MP_QSTR_ansi_bg,
    MP_QSTR__0x1b__bracket_open_48_semicolon_5_semicolon__percent_dm,
    MP_QSTR_color,
    MP_QSTR_rgb,
    MP_QSTR_ip,
    MP_QSTR_network,
    MP_QSTR_127_dot_0_dot_0_dot_1,
    MP_QSTR_WLAN,
    MP_QSTR_STA_IF,
    MP_QSTR_isconnected,
    MP_QSTR_ifconfig,
    MP_QSTR_set_time,
    MP_QSTR_ntptime,
    MP_QSTR_settime,
    MP_QSTR_wifi,
    MP_QSTR_active,
    MP_QSTR_connect,
    MP_QSTR_tar_create,
    MP_QSTR_utarfile,
    MP_QSTR_TarFile,
    MP_QSTR__dot_tar,
    MP_QSTR_w,
    MP_QSTR_add,
    MP_QSTR_close,
    MP_QSTR_tar_extract,
    MP_QSTR_r,
    MP_QSTR_extracting,
    MP_QSTR_type,
    MP_QSTR_DIRTYPE,
    MP_QSTR_name,
    MP_QSTR__dot__slash_,
    MP_QSTR_mkdir,
    MP_QSTR_strip,
    MP_QSTR__slash_,
    MP_QSTR_making_colon_,
    MP_QSTR__dot__,
    MP_QSTR_ignoring,
    MP_QSTR_extractfile,
    MP_QSTR_write,
    MP_QSTR_borked_space_on_colon_,
    MP_QSTR___init__,
    MP_QSTR_debug,
    MP_QSTR_quit,
    MP_QSTR_mem_pointer,
    MP_QSTR_num_sprites,
    MP_QSTR_sprite_id,
    MP_QSTR_mem_pos,
    MP_QSTR_width,
    MP_QSTR_height,
    MP_QSTR_x,
    MP_QSTR_x_v,
    MP_QSTR_y_v,
    MP_QSTR_clamp,
    MP_QSTR_SCREEN_WIDTH,
    MP_QSTR_SCREEN_HEIGHT,
    MP_QSTR_load,
    MP_QSTR_SPRITES,
    MP_QSTR_SPRITE_RAM_BYTES,
    MP_QSTR_off,
    MP_QSTR_on,
    MP_QSTR_moveto,
    MP_QSTR_move,
    MP_QSTR_joy_move,
    MP_QSTR_timing,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_BLACK,
    MP_QSTR_RED,
    MP_QSTR_GREEN,
    MP_QSTR_BROWN,
    MP_QSTR_BLUE,
    MP_QSTR_PURPLE,
    MP_QSTR_CYAN,
    MP_QSTR_LIGHT_GRAY,
    MP_QSTR_DARK_GRAY,
    MP_QSTR_LIGHT_RED,
    MP_QSTR_LIGHT_GREEN,
    MP_QSTR_YELLOW,
    MP_QSTR_LIGHT_BLUE,
    MP_QSTR_LIGHT_PURPLE,
    MP_QSTR_LIGHT_CYAN,
    MP_QSTR_LIGHT_WHITE,
    MP_QSTR_BG_BLACK,
    MP_QSTR_BG_RED,
    MP_QSTR_BG_GREEN,
    MP_QSTR_BG_BROWN,
    MP_QSTR_BG_BLUE,
    MP_QSTR_BG_PURPLE,
    MP_QSTR_BG_CYAN,
    MP_QSTR_BG_LIGHT_GRAY,
    MP_QSTR_BG_DARK_GRAY,
    MP_QSTR_BG_LIGHT_RED,
    MP_QSTR_BG_LIGHT_GREEN,
    MP_QSTR_BG_YELLOW,
    MP_QSTR_BG_LIGHT_BLUE,
    MP_QSTR_BG_LIGHT_PURPLE,
    MP_QSTR_BG_LIGHT_CYAN,
    MP_QSTR_BG_LIGHT_WHITE,
    MP_QSTR_BOLD,
    MP_QSTR_FAINT,
    MP_QSTR_ITALIC,
    MP_QSTR_UNDERLINE,
    MP_QSTR_FLASH,
    MP_QSTR_BLINK,
    MP_QSTR_REVERSE,
    MP_QSTR_INVERSE,
    MP_QSTR_NEGATIVE,
    MP_QSTR_STRIKE,
    MP_QSTR_CROSSED,
    MP_QSTR_RESET,
    MP_QSTR_DEFAULT,
    MP_QSTR_END,
    MP_QSTR_SORTED_HUE,
    MP_QSTR_build_strings,
    MP_QSTR_ImportError,
    MP_QSTR_print,
    MP_QSTR_len,
    MP_QSTR_input,
    MP_QSTR_tfb_log_start,
    MP_QSTR_display_stop,
    MP_QSTR_bytes,
    MP_QSTR_Exception,
    MP_QSTR_display_start,
    MP_QSTR_tfb_log_stop,
    MP_QSTR_joy,
    MP_QSTR_keys,
    MP_QSTR_module,
    MP_QSTR_exec,
    MP_QSTR_KeyboardInterrupt,
    MP_QSTR_mode,
    MP_QSTR_int_screenshot,
    MP_QSTR_open,
    MP_QSTR_pal_idx,
    MP_QSTR_g,
    MP_QSTR_b,
    MP_QSTR_px0,
    MP_QSTR_ssid,
    MP_QSTR_passwd,
    MP_QSTR_wait_timeout,
    MP_QSTR_directory,
    MP_QSTR_file_name,
    MP_QSTR_show_progress,
    MP_QSTR_OSError,
    MP_QSTR_self,
    MP_QSTR_tfb_save,
    MP_QSTR_key_scan,
    MP_QSTR_collisions,
    MP_QSTR_frame_callback,
    MP_QSTR_gpu_reset,
    MP_QSTR_tfb_restore,
    MP_QSTR_display_restart,
    MP_QSTR_sprite_clear,
    MP_QSTR_copy_of,
    MP_QSTR_sprite_register,
    MP_QSTR_sprite_png,
    MP_QSTR_sprite_off,
    MP_QSTR_sprite_on,
    MP_QSTR_sprite_move,
    MP_QSTR_int,
    MP_QSTR_speed,
    MP_QSTR_super,
};

// constants
static const mp_obj_str_t const_obj_tulip_1 = {{&mp_type_str}, 57577, 100, (const byte*)"\x55\x70\x67\x72\x61\x64\x69\x6e\x67\x20\x6f\x6e\x6c\x79\x20\x77\x6f\x72\x6b\x73\x20\x6f\x6e\x20\x54\x75\x6c\x69\x70\x20\x43\x43\x20\x66\x6f\x72\x20\x6e\x6f\x77\x2e\x20\x56\x69\x73\x69\x74\x20\x74\x75\x6c\x69\x70\x2e\x63\x6f\x6d\x70\x75\x74\x65\x72\x20\x74\x6f\x20\x64\x6f\x77\x6e\x6c\x6f\x61\x64\x20\x74\x68\x65\x20\x6c\x61\x74\x65\x73\x74\x20\x54\x75\x6c\x69\x70\x20\x44\x65\x73\x6b\x74\x6f\x70\x2e"};
static const mp_obj_str_t const_obj_tulip_2 = {{&mp_type_str}, 30082, 75, (const byte*)"\x59\x6f\x75\x20\x61\x72\x65\x20\x6e\x6f\x74\x20\x75\x73\x69\x6e\x67\x20\x4f\x54\x41\x20\x70\x61\x72\x74\x69\x74\x69\x6f\x6e\x73\x2c\x20\x6c\x69\x6b\x65\x6c\x79\x20\x62\x65\x63\x61\x75\x73\x65\x20\x6f\x66\x20\x79\x6f\x75\x72\x20\x66\x6c\x61\x73\x68\x20\x73\x69\x7a\x65\x20\x28\x4e\x38\x52\x38\x2e\x29"};
static const mp_obj_str_t const_obj_tulip_3 = {{&mp_type_str}, 9026, 61, (const byte*)"\x54\x68\x65\x20\x73\x63\x72\x65\x65\x6e\x20\x77\x69\x6c\x6c\x20\x62\x6c\x61\x6e\x6b\x20\x66\x6f\x72\x20\x32\x2d\x33\x20\x6d\x69\x6e\x75\x74\x65\x73\x2e\x20\x53\x65\x65\x20\x73\x74\x61\x74\x75\x73\x20\x69\x6e\x20\x6d\x6f\x6e\x69\x74\x6f\x72\x2e"};
static const mp_obj_str_t const_obj_tulip_4 = {{&mp_type_str}, 32349, 25, (const byte*)"\x4c\x61\x74\x65\x73\x74\x20\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x3a\x20\x25\x73\x20\x20\x20\x25\x73"};
static const mp_obj_str_t const_obj_tulip_6 = {{&mp_type_str}, 36168, 26, (const byte*)"\x49\x6e\x73\x74\x61\x6c\x6c\x20\x69\x74\x3f\x20\x20\x20\x20\x20\x20\x20\x28\x59\x79\x2f\x4e\x6e\x29\x20"};
static const mp_obj_str_t const_obj_tulip_8 = {{&mp_type_str}, 44693, 28, (const byte*)"\x46\x6c\x61\x73\x68\x69\x6e\x67\x20\x4f\x54\x41\x20\x70\x61\x72\x74\x69\x74\x69\x6f\x6e\x20\x25\x73\x2e\x2e\x2e"};
static const mp_obj_str_t const_obj_tulip_9 = {{&mp_type_str}, 37613, 39, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x25\x73\x2f\x5f\x6d\x61\x74\x72\x69\x78\x2f\x6d\x65\x64\x69\x61\x2f\x72\x30\x2f\x64\x6f\x77\x6e\x6c\x6f\x61\x64\x2f\x25\x73"};
static const mp_obj_str_t const_obj_tulip_10 = {{&mp_type_str}, 61640, 26, (const byte*)"\x4c\x61\x73\x74\x20\x63\x68\x75\x6e\x6b\x2c\x20\x61\x64\x64\x69\x6e\x67\x20\x70\x61\x64\x64\x69\x6e\x67"};
static const mp_obj_str_t const_obj_tulip_11 = {{&mp_type_bytes}, 46426, 1, (const byte*)"\xff"};
static const mp_obj_str_t const_obj_tulip_12 = {{&mp_type_str}, 35643, 27, (const byte*)"\x25\x64\x20\x62\x6c\x6f\x63\x6b\x73\x2c\x20\x25\x64\x20\x62\x79\x74\x65\x73\x20\x66\x6c\x61\x73\x68\x65\x64"};
static const mp_obj_str_t const_obj_tulip_13 = {{&mp_type_str}, 12676, 31, (const byte*)"\x53\x65\x74\x74\x69\x6e\x67\x20\x62\x6f\x6f\x74\x20\x70\x61\x72\x74\x69\x74\x69\x6f\x6e\x20\x74\x6f\x20\x25\x73\x2e\x2e\x2e"};
static const mp_obj_str_t const_obj_tulip_18 = {{&mp_type_str}, 53341, 29, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x61\x70\x69\x2e\x69\x6d\x67\x75\x72\x2e\x63\x6f\x6d\x2f\x33\x2f\x69\x6d\x61\x67\x65"};
static const mp_obj_str_t const_obj_tulip_19 = {{&mp_type_str}, 62771, 25, (const byte*)"\x43\x6c\x69\x65\x6e\x74\x2d\x49\x44\x20\x33\x39\x33\x39\x63\x32\x64\x33\x66\x35\x61\x36\x66\x38\x33"};
static const mp_obj_str_t const_obj_tulip_23 = {{&mp_type_str}, 15190, 26, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x69\x2e\x69\x6d\x67\x75\x72\x2e\x63\x6f\x6d\x2f\x25\x73\x2e\x70\x6e\x67"};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_tulip_29 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_tulip_29_macro MP_ROM_PTR(&const_obj_tulip_29)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_tulip_29_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_tulip_29_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
static const mp_obj_str_t const_obj_tulip_31 = {{&mp_type_str}, 65251, 58, (const byte*)"\x4e\x6f\x20\x6d\x6f\x72\x65\x20\x73\x70\x72\x69\x74\x65\x20\x52\x41\x4d\x2e\x20\x43\x75\x72\x72\x65\x6e\x74\x20\x70\x6f\x69\x6e\x74\x65\x72\x20\x25\x64\x2c\x20\x79\x6f\x75\x20\x77\x61\x6e\x74\x20\x74\x6f\x20\x61\x64\x64\x20\x25\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_tulip[32] = {
    MP_ROM_QSTR(MP_QSTR_TulipCC_slash_4_dot_0),
    MP_ROM_PTR(&const_obj_tulip_1),
    MP_ROM_PTR(&const_obj_tulip_2),
    MP_ROM_PTR(&const_obj_tulip_3),
    MP_ROM_PTR(&const_obj_tulip_4),
    MP_ROM_QSTR(MP_QSTR_You_space_have_space_version_colon__space__percent_s),
    MP_ROM_PTR(&const_obj_tulip_6),
    MP_ROM_QSTR(MP_QSTR_Flashing_space__percent_s_dot__dot__dot__space__bracket_open__percent_s_bracket_close_),
    MP_ROM_PTR(&const_obj_tulip_8),
    MP_ROM_PTR(&const_obj_tulip_9),
    MP_ROM_PTR(&const_obj_tulip_10),
    MP_ROM_PTR(&const_obj_tulip_11),
    MP_ROM_PTR(&const_obj_tulip_12),
    MP_ROM_PTR(&const_obj_tulip_13),
    MP_ROM_QSTR(MP_QSTR_Rebooting_dot__dot__dot_),
    MP_ROM_QSTR(MP_QSTR_Error_space_running_space__percent_s_colon_),
    MP_ROM_QSTR(MP_QSTR_del_space_sys_dot_modules_bracket_open__quot__percent_s_quot__bracket_close_),
    MP_ROM_QSTR(MP_QSTR__temp_ss_dot_png),
    MP_ROM_PTR(&const_obj_tulip_18),
    MP_ROM_PTR(&const_obj_tulip_19),
    MP_ROM_QSTR(MP_QSTR_multipart_slash_form_hyphen_data_semicolon_),
    MP_ROM_QSTR(MP_QSTR_authorization),
    MP_ROM_QSTR(MP_QSTR_content_hyphen_type),
    MP_ROM_PTR(&const_obj_tulip_23),
    MP_ROM_QSTR(MP_QSTR_Need_space_wi_hyphen_fi_space_on),
    MP_ROM_QSTR(MP_QSTR_Need_space_to_space_be_space_on_space_wifi),
    MP_ROM_QSTR(MP_QSTR_Warning_colon__space_directory),
    MP_ROM_QSTR(MP_QSTR_already_space_exists),
    MP_ROM_QSTR(MP_QSTR_extracting_colon_),
    const_obj_tulip_29_macro,
    MP_ROM_QSTR(MP_QSTR_No_space_more_space_sprite_space_handles_dot_),
    MP_ROM_PTR(&const_obj_tulip_31),
};

static const mp_frozen_module_t frozen_module_tulip = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_tulip,
        .obj_table = (mp_obj_t *)&const_obj_table_data_tulip,
    },
    .rc = &raw_code_tulip__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module upysh
// - original source file: build-standard/tulip/obj/frozen_mpy/upysh.mpy
// - frozen file name: upysh.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file upysh.py, scope upysh__lt_module_gt_
static const byte fun_data_upysh__lt_module_gt_[156] = {
    0x10,0x38, // prelude
    0x01, // names: <module>
    0x26,0x66,0x89,0x19,0x89,0x08,0x89,0x08,0x88,0x09,0x64,0x20,0x84,0x10,0x84,0x0c,0x88,0x0d,0x89,0x0f,0x26,0x26,0x26,0x46,0x26,0x26,0x26, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'LS'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'LS'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x05, // LOAD_CONST_STRING 'PWD'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'PWD'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x06, // LOAD_CONST_STRING 'CLEAR'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x06, // STORE_NAME 'CLEAR'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x0d, // STORE_NAME 'head'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x11, // STORE_NAME 'cat'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x12, // STORE_NAME 'cp'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1a, // STORE_NAME 'newfile'
    0x50, // LOAD_CONST_FALSE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x1d, // STORE_NAME 'rm'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x08, // MAKE_FUNCTION 8
    0x10,0x07, // LOAD_CONST_STRING 'Man'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x07, // STORE_NAME 'Man'
    0x11,0x07, // LOAD_NAME 'Man'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x28, // STORE_NAME 'man'
    0x11,0x05, // LOAD_NAME 'PWD'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x29, // STORE_NAME 'pwd'
    0x11,0x04, // LOAD_NAME 'LS'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2a, // STORE_NAME 'ls'
    0x11,0x06, // LOAD_NAME 'CLEAR'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2b, // STORE_NAME 'clear'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x08, // LOAD_ATTR 'chdir'
    0x16,0x2c, // STORE_NAME 'cd'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x09, // LOAD_ATTR 'mkdir'
    0x16,0x09, // STORE_NAME 'mkdir'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x0a, // LOAD_ATTR 'rename'
    0x16,0x2d, // STORE_NAME 'mv'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x0b, // LOAD_ATTR 'rmdir'
    0x16,0x0b, // STORE_NAME 'rmdir'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_LS
static const byte fun_data_upysh_LS[30] = {
    0x08,0x0a, // prelude
    0x04, // names: LS
    0x68,0x40,0x64,0x20, // code info
    0x11,0x2e, // LOAD_NAME '__name__'
    0x16,0x2f, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'LS'
    0x16,0x30, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x10,0x0c, // LOAD_CONST_STRING '.'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_LS
// frozen bytecode for file upysh.py, scope upysh_LS___repr__
static const byte fun_data_upysh_LS___repr__[16] = {
    0x11,0x0a, // prelude
    0x22,0x3f, // names: __repr__, self
    0x60,0x60,0x26, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x23, // LOAD_METHOD '__call__'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x10,0x10, // LOAD_CONST_STRING ''
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_LS___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_LS___repr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
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
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_LS___repr__ + 4,
        .line_info_top = fun_data_upysh_LS___repr__ + 7,
        .opcodes = fun_data_upysh_LS___repr__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh_LS
// frozen bytecode for file upysh.py, scope upysh_LS___call__
static const byte fun_data_upysh_LS___call__[120] = {
    0xe2,0x01,0x1c, // prelude
    0x23,0x3f,0x3a, // names: __call__, self, path
    0x80,0x0a,0x2c,0x26,0x25,0x2a,0x2d,0x25,0x2a,0x29,0x52, // code info
    0x12,0x40, // LOAD_GLOBAL 'list'
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'ilistdir'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x24, // LOAD_METHOD 'sort'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x18, // FOR_ITER 24
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x34, // LOAD_GLOBAL 'print'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x26, // JUMP -26
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x33, // FOR_ITER 51
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x34, // LOAD_GLOBAL 'print'
    0x10,0x25, // LOAD_CONST_STRING '% 9d %s'
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x4b, // JUMP 11
    0x12,0x34, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x0b, // JUMP -53
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_LS___call__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upysh_LS___call__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 120,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_LS___call__ + 6,
        .line_info_top = fun_data_upysh_LS___call__ + 17,
        .opcodes = fun_data_upysh_LS___call__ + 17,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upysh_LS[] = {
    &raw_code_upysh_LS___repr__,
    &raw_code_upysh_LS___call__,
};

static const mp_raw_code_t raw_code_upysh_LS = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upysh_LS,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 30,
    #endif
    .children = (void *)&children_upysh_LS,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_upysh_LS + 3,
        .line_info_top = fun_data_upysh_LS + 7,
        .opcodes = fun_data_upysh_LS + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_PWD
static const byte fun_data_upysh_PWD[24] = {
    0x00,0x08, // prelude
    0x05, // names: PWD
    0x88,0x1e,0x64, // code info
    0x11,0x2e, // LOAD_NAME '__name__'
    0x16,0x2f, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'PWD'
    0x16,0x30, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_PWD
// frozen bytecode for file upysh.py, scope upysh_PWD___repr__
static const byte fun_data_upysh_PWD___repr__[13] = {
    0x11,0x08, // prelude
    0x22,0x3f, // names: __repr__, self
    0x80,0x1f, // code info
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x26, // LOAD_METHOD 'getcwd'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_PWD___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_PWD___repr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 13,
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
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_PWD___repr__ + 4,
        .line_info_top = fun_data_upysh_PWD___repr__ + 6,
        .opcodes = fun_data_upysh_PWD___repr__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh_PWD
// frozen bytecode for file upysh.py, scope upysh_PWD___call__
static const byte fun_data_upysh_PWD___call__[12] = {
    0x11,0x08, // prelude
    0x23,0x3f, // names: __call__, self
    0x80,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x22, // LOAD_METHOD '__repr__'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_PWD___call__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_PWD___call__,
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
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_PWD___call__ + 4,
        .line_info_top = fun_data_upysh_PWD___call__ + 6,
        .opcodes = fun_data_upysh_PWD___call__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upysh_PWD[] = {
    &raw_code_upysh_PWD___repr__,
    &raw_code_upysh_PWD___call__,
};

static const mp_raw_code_t raw_code_upysh_PWD = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upysh_PWD,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
    #endif
    .children = (void *)&children_upysh_PWD,
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
        .qstr_block_name_idx = 5,
        .line_info = fun_data_upysh_PWD + 3,
        .line_info_top = fun_data_upysh_PWD + 6,
        .opcodes = fun_data_upysh_PWD + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_CLEAR
static const byte fun_data_upysh_CLEAR[24] = {
    0x00,0x08, // prelude
    0x06, // names: CLEAR
    0x88,0x26,0x64, // code info
    0x11,0x2e, // LOAD_NAME '__name__'
    0x16,0x2f, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'CLEAR'
    0x16,0x30, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_CLEAR
// frozen bytecode for file upysh.py, scope upysh_CLEAR___repr__
static const byte fun_data_upysh_CLEAR___repr__[9] = {
    0x09,0x08, // prelude
    0x22,0x3f, // names: __repr__, self
    0x80,0x27, // code info
    0x10,0x27, // LOAD_CONST_STRING '\x1b[2J\x1b[H'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_CLEAR___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_CLEAR___repr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 9,
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
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_CLEAR___repr__ + 4,
        .line_info_top = fun_data_upysh_CLEAR___repr__ + 6,
        .opcodes = fun_data_upysh_CLEAR___repr__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh_CLEAR
// frozen bytecode for file upysh.py, scope upysh_CLEAR___call__
static const byte fun_data_upysh_CLEAR___call__[12] = {
    0x11,0x08, // prelude
    0x23,0x3f, // names: __call__, self
    0x80,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x22, // LOAD_METHOD '__repr__'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_CLEAR___call__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_CLEAR___call__,
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
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_CLEAR___call__ + 4,
        .line_info_top = fun_data_upysh_CLEAR___call__ + 6,
        .opcodes = fun_data_upysh_CLEAR___call__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upysh_CLEAR[] = {
    &raw_code_upysh_CLEAR___repr__,
    &raw_code_upysh_CLEAR___call__,
};

static const mp_raw_code_t raw_code_upysh_CLEAR = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upysh_CLEAR,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
    #endif
    .children = (void *)&children_upysh_CLEAR,
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
        .qstr_block_name_idx = 6,
        .line_info = fun_data_upysh_CLEAR + 3,
        .line_info_top = fun_data_upysh_CLEAR + 6,
        .opcodes = fun_data_upysh_CLEAR + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_head
static const byte fun_data_upysh_head[63] = {
    0xde,0x01,0x14, // prelude
    0x0d,0x31,0x32, // names: head, f, n
    0x80,0x2e,0x28,0x26,0x26,0x23,0x22, // code info
    0x12,0x33, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x27, // SETUP_WITH 39
    0xc0, // STORE_FAST 0
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5a, // JUMP 26
    0x57, // DUP_TOP
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x52, // JUMP 18
    0x12,0x34, // LOAD_GLOBAL 'print'
    0xb3, // LOAD_FAST 3
    0x10,0x0f, // LOAD_CONST_STRING 'end'
    0x10,0x10, // LOAD_CONST_STRING ''
    0x34,0x82,0x01, // CALL_FUNCTION 257
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x21, // POP_JUMP_IF_TRUE -31
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_head = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upysh_head,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 63,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_upysh_head + 6,
        .line_info_top = fun_data_upysh_head + 13,
        .opcodes = fun_data_upysh_head + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_cat
static const byte fun_data_upysh_cat[16] = {
    0x19,0x08, // prelude
    0x11,0x31, // names: cat, f
    0x80,0x37, // code info
    0x12,0x0d, // LOAD_GLOBAL 'head'
    0xb0, // LOAD_FAST 0
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_cat = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_cat,
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
        .qstr_block_name_idx = 17,
        .line_info = fun_data_upysh_cat + 4,
        .line_info_top = fun_data_upysh_cat + 6,
        .opcodes = fun_data_upysh_cat + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_cp
static const byte fun_data_upysh_cp[132] = {
    0xea,0x02,0x20, // prelude
    0x12,0x35,0x36, // names: cp, s, t
    0x80,0x3b,0x22,0x30,0x56,0x23,0x28,0x26,0x34,0x20,0x27,0x25,0x22, // code info
    0x48,0x1f, // SETUP_EXCEPT 31
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x13, // LOAD_METHOD 'stat'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb1, // LOAD_FAST 1
    0x14,0x14, // LOAD_METHOD 'rstrip'
    0x10,0x15, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x10,0x15, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x12,0x37, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x38, // LOAD_GLOBAL 'bytearray'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x12,0x39, // LOAD_GLOBAL 'memoryview'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x33, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x2b, // SETUP_WITH 43
    0xc0, // STORE_FAST 0
    0x12,0x33, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x17, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x1e, // SETUP_WITH 30
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x18, // LOAD_METHOD 'readinto'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xda, // BINARY_OP 3 __le__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x4e, // JUMP 14
    0xb1, // LOAD_FAST 1
    0x14,0x19, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x24, // JUMP -28
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_cp = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upysh_cp,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 132,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_upysh_cp + 6,
        .line_info_top = fun_data_upysh_cp + 19,
        .opcodes = fun_data_upysh_cp + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_newfile
static const byte fun_data_upysh_newfile[73] = {
    0xd1,0x02,0x16, // prelude
    0x1a,0x3a, // names: newfile, path
    0x80,0x4b,0x27,0x2a,0x20,0x22,0x4e,0x24,0x27, // code info
    0x12,0x34, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x33, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x1b, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x27, // SETUP_WITH 39
    0xc1, // STORE_FAST 1
    0x48,0x07, // SETUP_EXCEPT 7
    0x12,0x3b, // LOAD_GLOBAL 'input'
    0x34,0x00, // CALL_FUNCTION 0
    0xc2, // STORE_FAST 2
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x57, // DUP_TOP
    0x12,0x3c, // LOAD_GLOBAL 'EOFError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x59, // POP_TOP
    0x40,0x53,0x01, // UNWIND_JUMP 19
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x14,0x19, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x19, // LOAD_METHOD 'write'
    0x10,0x1c, // LOAD_CONST_STRING '\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_newfile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_newfile,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 73,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_upysh_newfile + 5,
        .line_info_top = fun_data_upysh_newfile + 14,
        .opcodes = fun_data_upysh_newfile + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_rm
static const byte fun_data_upysh_rm[117] = {
    0xe6,0x01,0x18, // prelude
    0x1d,0x3d,0x3e, // names: rm, d, recursive
    0x80,0x57,0x22,0x33,0x2b,0x30,0x33,0x4a,0x4b, // code info
    0x48,0x55, // SETUP_EXCEPT 85
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x13, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x44,0x7b, // POP_JUMP_IF_FALSE 59
    0xb1, // LOAD_FAST 1
    0x44,0x78, // POP_JUMP_IF_FALSE 56
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'ilistdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x24, // FOR_ITER 36
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x0c, // LOAD_CONST_STRING '.'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x1f, // LOAD_CONST_STRING '..'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x1d, // LOAD_GLOBAL 'rm'
    0x10,0x15, // LOAD_CONST_STRING '/'
    0x14,0x20, // LOAD_METHOD 'join'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x1a, // JUMP -38
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x0b, // LOAD_METHOD 'rmdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x48, // JUMP 8
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x21, // LOAD_METHOD 'remove'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x0d, // POP_EXCEPT_JUMP 13
    0x59, // POP_TOP
    0x12,0x34, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_rm = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_upysh_rm,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 117,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_upysh_rm + 6,
        .line_info_top = fun_data_upysh_rm + 15,
        .opcodes = fun_data_upysh_rm + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_Man
static const byte fun_data_upysh_Man[19] = {
    0x00,0x06, // prelude
    0x07, // names: Man
    0x88,0x64, // code info
    0x11,0x2e, // LOAD_NAME '__name__'
    0x16,0x2f, // STORE_NAME '__module__'
    0x10,0x07, // LOAD_CONST_STRING 'Man'
    0x16,0x30, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_Man
// frozen bytecode for file upysh.py, scope upysh_Man___repr__
static const byte fun_data_upysh_Man___repr__[9] = {
    0x09,0x08, // prelude
    0x22,0x3f, // names: __repr__, self
    0x80,0x65, // code info
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_upysh_Man___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_upysh_Man___repr__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 9,
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
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_Man___repr__ + 4,
        .line_info_top = fun_data_upysh_Man___repr__ + 6,
        .opcodes = fun_data_upysh_Man___repr__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upysh_Man[] = {
    &raw_code_upysh_Man___repr__,
};

static const mp_raw_code_t raw_code_upysh_Man = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upysh_Man,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = (void *)&children_upysh_Man,
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
        .qstr_block_name_idx = 7,
        .line_info = fun_data_upysh_Man + 3,
        .line_info_top = fun_data_upysh_Man + 5,
        .opcodes = fun_data_upysh_Man + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_upysh__lt_module_gt_[] = {
    &raw_code_upysh_LS,
    &raw_code_upysh_PWD,
    &raw_code_upysh_CLEAR,
    &raw_code_upysh_head,
    &raw_code_upysh_cat,
    &raw_code_upysh_cp,
    &raw_code_upysh_newfile,
    &raw_code_upysh_rm,
    &raw_code_upysh_Man,
};

static const mp_raw_code_t raw_code_upysh__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_upysh__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 156,
    #endif
    .children = (void *)&children_upysh__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 9,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_upysh__lt_module_gt_ + 3,
        .line_info_top = fun_data_upysh__lt_module_gt_ + 30,
        .opcodes = fun_data_upysh__lt_module_gt_ + 30,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_upysh[66] = {
    MP_QSTR_upysh_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_os,
    MP_QSTR_LS,
    MP_QSTR_PWD,
    MP_QSTR_CLEAR,
    MP_QSTR_Man,
    MP_QSTR_chdir,
    MP_QSTR_mkdir,
    MP_QSTR_rename,
    MP_QSTR_rmdir,
    MP_QSTR__dot_,
    MP_QSTR_head,
    MP_QSTR_readline,
    MP_QSTR_end,
    MP_QSTR_,
    MP_QSTR_cat,
    MP_QSTR_cp,
    MP_QSTR_stat,
    MP_QSTR_rstrip,
    MP_QSTR__slash_,
    MP_QSTR_rb,
    MP_QSTR_wb,
    MP_QSTR_readinto,
    MP_QSTR_write,
    MP_QSTR_newfile,
    MP_QSTR_w,
    MP_QSTR__0x0a_,
    MP_QSTR_rm,
    MP_QSTR_ilistdir,
    MP_QSTR__dot__dot_,
    MP_QSTR_join,
    MP_QSTR_remove,
    MP_QSTR___repr__,
    MP_QSTR___call__,
    MP_QSTR_sort,
    MP_QSTR__percent__space_9d_space__percent_s,
    MP_QSTR_getcwd,
    MP_QSTR__0x1b__bracket_open_2J_0x1b__bracket_open_H,
    MP_QSTR_man,
    MP_QSTR_pwd,
    MP_QSTR_ls,
    MP_QSTR_clear,
    MP_QSTR_cd,
    MP_QSTR_mv,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_f,
    MP_QSTR_n,
    MP_QSTR_open,
    MP_QSTR_print,
    MP_QSTR_s,
    MP_QSTR_t,
    MP_QSTR_OSError,
    MP_QSTR_bytearray,
    MP_QSTR_memoryview,
    MP_QSTR_path,
    MP_QSTR_input,
    MP_QSTR_EOFError,
    MP_QSTR_d,
    MP_QSTR_recursive,
    MP_QSTR_self,
    MP_QSTR_list,
    MP_QSTR_len,
};

// constants
static const mp_obj_int_t const_obj_upysh_0 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0x0,0x4000}}};
static const mp_obj_str_t const_obj_upysh_1 = {{&mp_type_str}, 25826, 58, (const byte*)"\x54\x79\x70\x65\x20\x66\x69\x6c\x65\x20\x63\x6f\x6e\x74\x65\x6e\x74\x73\x20\x6c\x69\x6e\x65\x20\x62\x79\x20\x6c\x69\x6e\x65\x2c\x20\x66\x69\x6e\x69\x73\x68\x20\x77\x69\x74\x68\x20\x45\x4f\x46\x20\x28\x43\x74\x72\x6c\x2b\x44\x29\x2e"};
static const mp_obj_str_t const_obj_upysh_5 = {{&mp_type_str}, 30724, 256, (const byte*)"\x0a\x75\x70\x79\x73\x68\x20\x69\x73\x20\x69\x6e\x74\x65\x6e\x64\x65\x64\x20\x74\x6f\x20\x62\x65\x20\x69\x6d\x70\x6f\x72\x74\x65\x64\x20\x75\x73\x69\x6e\x67\x3a\x0a\x66\x72\x6f\x6d\x20\x75\x70\x79\x73\x68\x20\x69\x6d\x70\x6f\x72\x74\x20\x2a\x0a\x0a\x54\x6f\x20\x73\x65\x65\x20\x74\x68\x69\x73\x20\x68\x65\x6c\x70\x20\x74\x65\x78\x74\x20\x61\x67\x61\x69\x6e\x2c\x20\x74\x79\x70\x65\x20\x22\x6d\x61\x6e\x22\x2e\x0a\x0a\x75\x70\x79\x73\x68\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x73\x3a\x0a\x63\x6c\x65\x61\x72\x2c\x20\x6c\x73\x2c\x20\x6c\x73\x28\x2e\x2e\x2e\x29\x2c\x20\x68\x65\x61\x64\x28\x2e\x2e\x2e\x29\x2c\x20\x63\x61\x74\x28\x2e\x2e\x2e\x29\x2c\x20\x6e\x65\x77\x66\x69\x6c\x65\x28\x2e\x2e\x2e\x29\x0a\x63\x70\x28\x27\x73\x72\x63\x27\x2c\x20\x27\x64\x65\x73\x74\x27\x29\x2c\x20\x6d\x76\x28\x27\x6f\x6c\x64\x27\x2c\x20\x27\x6e\x65\x77\x27\x29\x2c\x20\x72\x6d\x28\x2e\x2e\x2e\x29\x0a\x70\x77\x64\x2c\x20\x63\x64\x28\x2e\x2e\x2e\x29\x2c\x20\x6d\x6b\x64\x69\x72\x28\x2e\x2e\x2e\x29\x2c\x20\x72\x6d\x64\x69\x72\x28\x2e\x2e\x2e\x29\x0a"};

// constant table
static const mp_rom_obj_t const_obj_table_data_upysh[6] = {
    MP_ROM_PTR(&const_obj_upysh_0),
    MP_ROM_PTR(&const_obj_upysh_1),
    MP_ROM_QSTR(MP_QSTR_rm_space_of_space__squot__percent_s_squot__space_failed),
    MP_ROM_QSTR(MP_QSTR__space__space__space__space__lt_dir_gt__space__percent_s),
    MP_ROM_QSTR(MP_QSTR__space__space__space__space__space__space__space__space__space__space__percent_s),
    MP_ROM_PTR(&const_obj_upysh_5),
};

static const mp_frozen_module_t frozen_module_upysh = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_upysh,
        .obj_table = (mp_obj_t *)&const_obj_table_data_upysh,
    },
    .rc = &raw_code_upysh__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module sequencer
// - original source file: build-standard/tulip/obj/frozen_mpy/sequencer.mpy
// - frozen file name: sequencer.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file sequencer.py, scope sequencer__lt_module_gt_
static const byte fun_data_sequencer__lt_module_gt_[116] = {
    0x10,0x38, // prelude
    0x01, // names: <module>
    0x20,0x66,0x24,0x24,0x23,0x23,0x43,0x23,0x23,0x23,0x24,0x49,0x84,0x0c,0x84,0x14,0x84,0x08,0x84,0x0c,0x68,0x40,0x88,0x08,0x47,0x80,0x11, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'machine'
    0x16,0x02, // STORE_NAME 'machine'
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x17,0x11, // STORE_GLOBAL 'ppq'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x17,0x12, // STORE_GLOBAL 'bpm'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x13, // STORE_NAME 'swing'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x14, // STORE_GLOBAL 'tick'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x15, // STORE_GLOBAL 'beat'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x16, // STORE_GLOBAL 'playing'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x17, // STORE_NAME 'seq_id'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x18, // STORE_GLOBAL 'loop_length'
    0x2c,0x00, // BUILD_MAP 0
    0x16,0x19, // STORE_NAME 'sequence'
    0x11,0x02, // LOAD_NAME 'machine'
    0x14,0x03, // LOAD_METHOD 'Timer'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x16,0x1a, // STORE_NAME 'timer'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x04, // STORE_NAME 'handleInterrupt'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x06, // STORE_NAME 'set_bpm'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x08, // STORE_NAME 'stop'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x09, // STORE_NAME 'start'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x0e, // STORE_NAME 'seek'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x0f, // STORE_NAME 'schedule'
    0x11,0x06, // LOAD_NAME 'set_bpm'
    0x12,0x12, // LOAD_GLOBAL 'bpm'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_handleInterrupt
static const byte fun_data_sequencer_handleInterrupt[87] = {
    0x49,0x18, // prelude
    0x04,0x1a, // names: handleInterrupt, timer
    0x80,0x11,0x20,0x26,0x27,0x26,0x23,0x2d,0x23,0x39, // code info
    0x12,0x14, // LOAD_GLOBAL 'tick'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x17,0x14, // STORE_GLOBAL 'tick'
    0x12,0x14, // LOAD_GLOBAL 'tick'
    0x12,0x11, // LOAD_GLOBAL 'ppq'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x15, // LOAD_GLOBAL 'beat'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x17,0x15, // STORE_GLOBAL 'beat'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x14, // STORE_GLOBAL 'tick'
    0x12,0x18, // LOAD_GLOBAL 'loop_length'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x15, // LOAD_GLOBAL 'beat'
    0x12,0x18, // LOAD_GLOBAL 'loop_length'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x15, // STORE_GLOBAL 'beat'
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0x14,0x05, // LOAD_METHOD 'get'
    0x12,0x15, // LOAD_GLOBAL 'beat'
    0x2c,0x00, // BUILD_MAP 0
    0x36,0x02, // CALL_METHOD 2
    0x14,0x05, // LOAD_METHOD 'get'
    0x12,0x14, // LOAD_GLOBAL 'tick'
    0x2b,0x00, // BUILD_LIST 0
    0x36,0x02, // CALL_METHOD 2
    0x5f, // GET_ITER_STACK
    0x4b,0x0c, // FOR_ITER 12
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc1, // STORE_FAST 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x35,0x01, // CALL_FUNCTION_VAR_KW 1
    0x59, // POP_TOP
    0x42,0x32, // JUMP -14
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_handleInterrupt = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_sequencer_handleInterrupt,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 87,
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
        .qstr_block_name_idx = 4,
        .line_info = fun_data_sequencer_handleInterrupt + 4,
        .line_info_top = fun_data_sequencer_handleInterrupt + 14,
        .opcodes = fun_data_sequencer_handleInterrupt + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_set_bpm
static const byte fun_data_sequencer_set_bpm[55] = {
    0x29,0x0e, // prelude
    0x06,0x1b, // names: set_bpm, in_bpm
    0x80,0x1d,0x20,0x23,0x39, // code info
    0xb0, // LOAD_FAST 0
    0x17,0x12, // STORE_GLOBAL 'bpm'
    0x12,0x1c, // LOAD_GLOBAL 'int'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x12,0x1d, // LOAD_GLOBAL 'float'
    0x12,0x12, // LOAD_GLOBAL 'bpm'
    0x34,0x01, // CALL_FUNCTION 1
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf7, // BINARY_OP 32 __truediv__
    0x12,0x1d, // LOAD_GLOBAL 'float'
    0x12,0x11, // LOAD_GLOBAL 'ppq'
    0x34,0x01, // CALL_FUNCTION 1
    0xf4, // BINARY_OP 29 __mul__
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x17,0x1e, // STORE_GLOBAL 'us_per_tick'
    0x12,0x1a, // LOAD_GLOBAL 'timer'
    0x14,0x07, // LOAD_METHOD 'period'
    0x12,0x1c, // LOAD_GLOBAL 'int'
    0x12,0x1e, // LOAD_GLOBAL 'us_per_tick'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_set_bpm = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_sequencer_set_bpm,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 55,
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
        .qstr_block_name_idx = 6,
        .line_info = fun_data_sequencer_set_bpm + 4,
        .line_info_top = fun_data_sequencer_set_bpm + 9,
        .opcodes = fun_data_sequencer_set_bpm + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_stop
static const byte fun_data_sequencer_stop[24] = {
    0x08,0x0c, // prelude
    0x08, // names: stop
    0x80,0x32,0x20,0x24,0x47, // code info
    0x12,0x16, // LOAD_GLOBAL 'playing'
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x1a, // LOAD_GLOBAL 'timer'
    0x14,0x08, // LOAD_METHOD 'stop'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x16, // STORE_GLOBAL 'playing'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_stop = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_sequencer_stop,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
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
        .qstr_block_name_idx = 8,
        .line_info = fun_data_sequencer_stop + 3,
        .line_info_top = fun_data_sequencer_stop + 8,
        .opcodes = fun_data_sequencer_stop + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_start
static const byte fun_data_sequencer_start[48] = {
    0x38,0x0c, // prelude
    0x09, // names: start
    0x80,0x3a,0x20,0x24,0x23, // code info
    0x12,0x16, // LOAD_GLOBAL 'playing'
    0x43,0x62, // POP_JUMP_IF_TRUE 34
    0x81, // LOAD_CONST_SMALL_INT 1
    0x17,0x16, // STORE_GLOBAL 'playing'
    0x12,0x1a, // LOAD_GLOBAL 'timer'
    0x14,0x0a, // LOAD_METHOD 'init'
    0x10,0x07, // LOAD_CONST_STRING 'period'
    0x12,0x1c, // LOAD_GLOBAL 'int'
    0x12,0x1e, // LOAD_GLOBAL 'us_per_tick'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x0b, // LOAD_CONST_STRING 'mode'
    0x12,0x02, // LOAD_GLOBAL 'machine'
    0x13,0x03, // LOAD_ATTR 'Timer'
    0x13,0x0c, // LOAD_ATTR 'PERIODIC'
    0x10,0x0d, // LOAD_CONST_STRING 'callback'
    0x12,0x04, // LOAD_GLOBAL 'handleInterrupt'
    0x36,0x86,0x00, // CALL_METHOD 768
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_start = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_sequencer_start,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 48,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_sequencer_start + 3,
        .line_info_top = fun_data_sequencer_start + 8,
        .opcodes = fun_data_sequencer_start + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_seek
static const byte fun_data_sequencer_seek[18] = {
    0x92,0x01,0x0e, // prelude
    0x0e,0x1f,0x20, // names: seek, in_beat, in_tick
    0x80,0x45,0x20,0x23, // code info
    0xb0, // LOAD_FAST 0
    0x17,0x15, // STORE_GLOBAL 'beat'
    0xb1, // LOAD_FAST 1
    0x17,0x14, // STORE_GLOBAL 'tick'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_seek = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_sequencer_seek,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 18,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_sequencer_seek + 6,
        .line_info_top = fun_data_sequencer_seek + 10,
        .opcodes = fun_data_sequencer_seek + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of sequencer__lt_module_gt_
// frozen bytecode for file sequencer.py, scope sequencer_schedule
static const byte fun_data_sequencer_schedule[59] = {
    0xb8,0x05,0x12, // prelude
    0x0f,0x21,0x22,0x15,0x14, // names: schedule, method, args, beat, tick
    0x80,0x4d,0x2d,0x31, // code info
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0x14,0x05, // LOAD_METHOD 'get'
    0xb2, // LOAD_FAST 2
    0x2c,0x00, // BUILD_MAP 0
    0x36,0x02, // CALL_METHOD 2
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0x14,0x05, // LOAD_METHOD 'get'
    0xb3, // LOAD_FAST 3
    0x2b,0x00, // BUILD_LIST 0
    0x36,0x02, // CALL_METHOD 2
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x56, // STORE_SUBSCR
    0x12,0x19, // LOAD_GLOBAL 'sequence'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x55, // LOAD_SUBSCR
    0x14,0x10, // LOAD_METHOD 'append'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_sequencer_schedule = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_sequencer_schedule,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 59,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_sequencer_schedule + 8,
        .line_info_top = fun_data_sequencer_schedule + 12,
        .opcodes = fun_data_sequencer_schedule + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_sequencer__lt_module_gt_[] = {
    &raw_code_sequencer_handleInterrupt,
    &raw_code_sequencer_set_bpm,
    &raw_code_sequencer_stop,
    &raw_code_sequencer_start,
    &raw_code_sequencer_seek,
    &raw_code_sequencer_schedule,
};

static const mp_raw_code_t raw_code_sequencer__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_sequencer__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 116,
    #endif
    .children = (void *)&children_sequencer__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 6,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_sequencer__lt_module_gt_ + 3,
        .line_info_top = fun_data_sequencer__lt_module_gt_ + 30,
        .opcodes = fun_data_sequencer__lt_module_gt_ + 30,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_sequencer[35] = {
    MP_QSTR_sequencer_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_machine,
    MP_QSTR_Timer,
    MP_QSTR_handleInterrupt,
    MP_QSTR_get,
    MP_QSTR_set_bpm,
    MP_QSTR_period,
    MP_QSTR_stop,
    MP_QSTR_start,
    MP_QSTR_init,
    MP_QSTR_mode,
    MP_QSTR_PERIODIC,
    MP_QSTR_callback,
    MP_QSTR_seek,
    MP_QSTR_schedule,
    MP_QSTR_append,
    MP_QSTR_ppq,
    MP_QSTR_bpm,
    MP_QSTR_swing,
    MP_QSTR_tick,
    MP_QSTR_beat,
    MP_QSTR_playing,
    MP_QSTR_seq_id,
    MP_QSTR_loop_length,
    MP_QSTR_sequence,
    MP_QSTR_timer,
    MP_QSTR_in_bpm,
    MP_QSTR_int,
    MP_QSTR_float,
    MP_QSTR_us_per_tick,
    MP_QSTR_in_beat,
    MP_QSTR_in_tick,
    MP_QSTR_method,
    MP_QSTR_args,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_sequencer_0 = {{&mp_type_float}, (mp_float_t)120};
#define const_obj_sequencer_0_macro MP_ROM_PTR(&const_obj_sequencer_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_sequencer_0_macro ((mp_rom_obj_t)(0xc3700002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_sequencer_0_macro ((mp_rom_obj_t)(0xc062000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_sequencer_1 = {{&mp_type_float}, (mp_float_t)1000000};
#define const_obj_sequencer_1_macro MP_ROM_PTR(&const_obj_sequencer_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_sequencer_1_macro ((mp_rom_obj_t)(0xc9f42402))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_sequencer_1_macro ((mp_rom_obj_t)(0xc132848000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_sequencer_2 = {{&mp_type_float}, (mp_float_t)60};
#define const_obj_sequencer_2_macro MP_ROM_PTR(&const_obj_sequencer_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_sequencer_2_macro ((mp_rom_obj_t)(0xc2f00002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_sequencer_2_macro ((mp_rom_obj_t)(0xc052000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_sequencer_3 = {{&mp_type_float}, (mp_float_t)1000};
#define const_obj_sequencer_3_macro MP_ROM_PTR(&const_obj_sequencer_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_sequencer_3_macro ((mp_rom_obj_t)(0xc4fa0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_sequencer_3_macro ((mp_rom_obj_t)(0xc093400000000000))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_sequencer[4] = {
    const_obj_sequencer_0_macro,
    const_obj_sequencer_1_macro,
    const_obj_sequencer_2_macro,
    const_obj_sequencer_3_macro,
};

static const mp_frozen_module_t frozen_module_sequencer = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_sequencer,
        .obj_table = (mp_obj_t *)&const_obj_table_data_sequencer,
    },
    .rc = &raw_code_sequencer__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module utarfile
// - original source file: build-standard/tulip/obj/frozen_mpy/utarfile.mpy
// - frozen file name: utarfile.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file utarfile.py, scope utarfile__lt_module_gt_
static const byte fun_data_utarfile__lt_module_gt_[288] = {
    0x18,0x40, // prelude
    0x01, // names: <module>
    0x26,0x66,0x22,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x75,0x24,0x64,0x24,0x25,0x66,0x64,0x20,0x89,0x23,0x69,0x40,0x64,0x60,0x26,0x26,0x66,0x64,0x20,0x64,0x20, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'uctypes'
    0x16,0x02, // STORE_NAME 'uctypes'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x2c,0x08, // BUILD_MAP 8
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x06, // LOAD_CONST_STRING 'name'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x87, // LOAD_CONST_SMALL_INT 7
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x07, // LOAD_CONST_STRING 'mode'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x80,0x6c, // LOAD_CONST_SMALL_INT 108
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x87, // LOAD_CONST_SMALL_INT 7
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x08, // LOAD_CONST_STRING 'uid'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x80,0x74, // LOAD_CONST_SMALL_INT 116
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x87, // LOAD_CONST_SMALL_INT 7
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x09, // LOAD_CONST_STRING 'gid'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x80,0x7c, // LOAD_CONST_SMALL_INT 124
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x0a, // LOAD_CONST_STRING 'size'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x81,0x08, // LOAD_CONST_SMALL_INT 136
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x0b, // LOAD_CONST_STRING 'mtime'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x81,0x14, // LOAD_CONST_SMALL_INT 148
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x88, // LOAD_CONST_SMALL_INT 8
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x0c, // LOAD_CONST_STRING 'chksum'
    0x62, // STORE_MAP
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x04, // LOAD_ATTR 'ARRAY'
    0x22,0x81,0x1c, // LOAD_CONST_SMALL_INT 156
    0xed, // BINARY_OP 22 __or__
    0x11,0x02, // LOAD_NAME 'uctypes'
    0x13,0x05, // LOAD_ATTR 'UINT8'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xed, // BINARY_OP 22 __or__
    0x2a,0x02, // BUILD_TUPLE 2
    0x10,0x0d, // LOAD_CONST_STRING 'typeflag'
    0x62, // STORE_MAP
    0x16,0x40, // STORE_NAME 'TAR_HEADER'
    0x10,0x0e, // LOAD_CONST_STRING 'dir'
    0x16,0x41, // STORE_NAME 'DIRTYPE'
    0x10,0x0f, // LOAD_CONST_STRING 'file'
    0x16,0x42, // STORE_NAME 'REGTYPE'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x43, // STORE_NAME 'NUL'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x16,0x44, // STORE_NAME 'BLOCKSIZE'
    0x11,0x44, // LOAD_NAME 'BLOCKSIZE'
    0x94, // LOAD_CONST_SMALL_INT 20
    0xf4, // BINARY_OP 29 __mul__
    0x16,0x45, // STORE_NAME 'RECORDSIZE'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x13, // STORE_NAME 'roundup'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x10, // LOAD_CONST_STRING 'FileSection'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x10, // STORE_NAME 'FileSection'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x11, // LOAD_CONST_STRING 'TarInfo'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x11, // STORE_NAME 'TarInfo'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x14, // STORE_NAME '_setstring'
    0x22,0x83,0xe0,0x00, // LOAD_CONST_SMALL_INT 61440
    0x16,0x46, // STORE_NAME '_S_IFMT'
    0x22,0x82,0x80,0x00, // LOAD_CONST_SMALL_INT 32768
    0x16,0x47, // STORE_NAME '_S_IFREG'
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0x16,0x48, // STORE_NAME '_S_IFDIR'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x17, // STORE_NAME '_isdir'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x18, // STORE_NAME '_isreg'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x06, // MAKE_FUNCTION 6
    0x10,0x12, // LOAD_CONST_STRING 'TarFile'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x12, // STORE_NAME 'TarFile'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile_roundup
static const byte fun_data_utarfile_roundup[18] = {
    0x22,0x0a, // prelude
    0x13,0x49,0x1d, // names: roundup, val, align
    0x80,0x19, // code info
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
static const mp_raw_code_t raw_code_utarfile_roundup = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_utarfile_roundup,
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
        .qstr_block_name_idx = 19,
        .line_info = fun_data_utarfile_roundup + 5,
        .line_info_top = fun_data_utarfile_roundup + 7,
        .opcodes = fun_data_utarfile_roundup + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile_FileSection
static const byte fun_data_utarfile_FileSection[44] = {
    0x08,0x12, // prelude
    0x10, // names: FileSection
    0x88,0x1d,0x64,0x40,0x8b,0x0a,0x84,0x09, // code info
    0x11,0x4a, // LOAD_NAME '__name__'
    0x16,0x4b, // STORE_NAME '__module__'
    0x10,0x10, // LOAD_CONST_STRING 'FileSection'
    0x16,0x4c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x1a, // STORE_NAME '__init__'
    0x22,0x84,0x80,0x00, // LOAD_CONST_SMALL_INT 65536
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x1e, // STORE_NAME 'read'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1f, // STORE_NAME 'readinto'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x20, // STORE_NAME 'skip'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of utarfile_FileSection
// frozen bytecode for file utarfile.py, scope utarfile_FileSection___init__
static const byte fun_data_utarfile_FileSection___init__[28] = {
    0xa8,0x04,0x12, // prelude
    0x1a,0x51,0x1b,0x1c,0x52, // names: __init__, self, f, content_len, aligned_len
    0x80,0x1e,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x1b, // STORE_ATTR 'f'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x1c, // STORE_ATTR 'content_len'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x18,0x1d, // STORE_ATTR 'align'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_FileSection___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_utarfile_FileSection___init__,
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
        .qstr_block_name_idx = 26,
        .line_info = fun_data_utarfile_FileSection___init__ + 8,
        .line_info_top = fun_data_utarfile_FileSection___init__ + 12,
        .opcodes = fun_data_utarfile_FileSection___init__ + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_FileSection
// frozen bytecode for file utarfile.py, scope utarfile_FileSection_read
static const byte fun_data_utarfile_FileSection_read[62] = {
    0xaa,0x01,0x18, // prelude
    0x1e,0x51,0x53, // names: read, self, sz
    0x80,0x23,0x27,0x23,0x27,0x24,0x29,0x26,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x1e, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x54, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xb1, // LOAD_FAST 1
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x1c, // STORE_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_FileSection_read = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_utarfile_FileSection_read,
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
        .qstr_block_name_idx = 30,
        .line_info = fun_data_utarfile_FileSection_read + 6,
        .line_info_top = fun_data_utarfile_FileSection_read + 15,
        .opcodes = fun_data_utarfile_FileSection_read + 15,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_FileSection
// frozen bytecode for file utarfile.py, scope utarfile_FileSection_readinto
static const byte fun_data_utarfile_FileSection_readinto[66] = {
    0x2a,0x16, // prelude
    0x1f,0x51,0x55, // names: readinto, self, buf
    0x80,0x2d,0x27,0x22,0x2b,0x2d,0x29,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x63, // RETURN_VALUE
    0x12,0x54, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x56, // LOAD_GLOBAL 'memoryview'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x1f, // LOAD_METHOD 'readinto'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x1c, // STORE_ATTR 'content_len'
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_FileSection_readinto = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_utarfile_FileSection_readinto,
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
        .qstr_block_name_idx = 31,
        .line_info = fun_data_utarfile_FileSection_readinto + 5,
        .line_info_top = fun_data_utarfile_FileSection_readinto + 13,
        .opcodes = fun_data_utarfile_FileSection_readinto + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_FileSection
// frozen bytecode for file utarfile.py, scope utarfile_FileSection_skip
static const byte fun_data_utarfile_FileSection_skip[57] = {
    0x39,0x14, // prelude
    0x20,0x51, // names: skip, self
    0x80,0x36,0x28,0x23,0x26,0x22,0x27,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1c, // LOAD_ATTR 'content_len'
    0xb0, // LOAD_FAST 0
    0x13,0x1d, // LOAD_ATTR 'align'
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0x12,0x57, // LOAD_GLOBAL 'bytearray'
    0x90, // LOAD_CONST_SMALL_INT 16
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x42,0x55, // JUMP 21
    0x12,0x58, // LOAD_GLOBAL 'min'
    0xb1, // LOAD_FAST 1
    0x90, // LOAD_CONST_SMALL_INT 16
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x1f, // LOAD_METHOD 'readinto'
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
static const mp_raw_code_t raw_code_utarfile_FileSection_skip = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_FileSection_skip,
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
        .qstr_block_name_idx = 32,
        .line_info = fun_data_utarfile_FileSection_skip + 4,
        .line_info_top = fun_data_utarfile_FileSection_skip + 12,
        .opcodes = fun_data_utarfile_FileSection_skip + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_utarfile_FileSection[] = {
    &raw_code_utarfile_FileSection___init__,
    &raw_code_utarfile_FileSection_read,
    &raw_code_utarfile_FileSection_readinto,
    &raw_code_utarfile_FileSection_skip,
};

static const mp_raw_code_t raw_code_utarfile_FileSection = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_utarfile_FileSection,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 44,
    #endif
    .children = (void *)&children_utarfile_FileSection,
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
        .qstr_block_name_idx = 16,
        .line_info = fun_data_utarfile_FileSection + 3,
        .line_info_top = fun_data_utarfile_FileSection + 11,
        .opcodes = fun_data_utarfile_FileSection + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile_TarInfo
static const byte fun_data_utarfile_TarInfo[19] = {
    0x00,0x06, // prelude
    0x11, // names: TarInfo
    0x88,0x40, // code info
    0x11,0x4a, // LOAD_NAME '__name__'
    0x16,0x4b, // STORE_NAME '__module__'
    0x10,0x11, // LOAD_CONST_STRING 'TarInfo'
    0x16,0x4c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x21, // STORE_NAME '__str__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of utarfile_TarInfo
// frozen bytecode for file utarfile.py, scope utarfile_TarInfo___str__
static const byte fun_data_utarfile_TarInfo___str__[21] = {
    0x21,0x08, // prelude
    0x21,0x51, // names: __str__, self
    0x80,0x41, // code info
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'name'
    0xb0, // LOAD_FAST 0
    0x13,0x22, // LOAD_ATTR 'type'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'size'
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarInfo___str__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_TarInfo___str__,
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
        .qstr_block_name_idx = 33,
        .line_info = fun_data_utarfile_TarInfo___str__ + 4,
        .line_info_top = fun_data_utarfile_TarInfo___str__ + 6,
        .opcodes = fun_data_utarfile_TarInfo___str__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_utarfile_TarInfo[] = {
    &raw_code_utarfile_TarInfo___str__,
};

static const mp_raw_code_t raw_code_utarfile_TarInfo = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_utarfile_TarInfo,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
    #endif
    .children = (void *)&children_utarfile_TarInfo,
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
        .qstr_block_name_idx = 17,
        .line_info = fun_data_utarfile_TarInfo + 3,
        .line_info_top = fun_data_utarfile_TarInfo + 5,
        .opcodes = fun_data_utarfile_TarInfo + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile__setstring
static const byte fun_data_utarfile__setstring[41] = {
    0x5b,0x10, // prelude
    0x14,0x4d,0x4e,0x4f, // names: _setstring, b, s, maxlen
    0x80,0x45,0x20,0x37, // code info
    0x12,0x50, // LOAD_GLOBAL 'enumerate'
    0xb1, // LOAD_FAST 1
    0x14,0x15, // LOAD_METHOD 'encode'
    0x10,0x16, // LOAD_CONST_STRING 'utf-8'
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0xb2, // LOAD_FAST 2
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0xb3, // LOAD_FAST 3
    0x56, // STORE_SUBSCR
    0x42,0x34, // JUMP -12
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile__setstring = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_utarfile__setstring,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 41,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_utarfile__setstring + 6,
        .line_info_top = fun_data_utarfile__setstring + 10,
        .opcodes = fun_data_utarfile__setstring + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile__isdir
static const byte fun_data_utarfile__isdir[16] = {
    0x11,0x08, // prelude
    0x17,0x33, // names: _isdir, finfo
    0x80,0x50, // code info
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x46, // LOAD_GLOBAL '_S_IFMT'
    0xef, // BINARY_OP 24 __and__
    0x12,0x48, // LOAD_GLOBAL '_S_IFDIR'
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile__isdir = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile__isdir,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
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
        .qstr_block_name_idx = 23,
        .line_info = fun_data_utarfile__isdir + 4,
        .line_info_top = fun_data_utarfile__isdir + 6,
        .opcodes = fun_data_utarfile__isdir + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile__isreg
static const byte fun_data_utarfile__isreg[16] = {
    0x11,0x08, // prelude
    0x18,0x33, // names: _isreg, finfo
    0x80,0x54, // code info
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x46, // LOAD_GLOBAL '_S_IFMT'
    0xef, // BINARY_OP 24 __and__
    0x12,0x47, // LOAD_GLOBAL '_S_IFREG'
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile__isreg = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile__isreg,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 16,
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
        .qstr_block_name_idx = 24,
        .line_info = fun_data_utarfile__isreg + 4,
        .line_info_top = fun_data_utarfile__isreg + 6,
        .opcodes = fun_data_utarfile__isreg + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile__lt_module_gt_
// frozen bytecode for file utarfile.py, scope utarfile_TarFile
static const byte fun_data_utarfile_TarFile[74] = {
    0x10,0x1e, // prelude
    0x12, // names: TarFile
    0x88,0x58,0x8b,0x0c,0x84,0x16,0x64,0x64,0x60,0x64,0x88,0x25,0x88,0x15, // code info
    0x11,0x4a, // LOAD_NAME '__name__'
    0x16,0x4b, // STORE_NAME '__module__'
    0x10,0x12, // LOAD_CONST_STRING 'TarFile'
    0x16,0x4c, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x10,0x19, // LOAD_CONST_STRING 'r'
    0x51, // LOAD_CONST_NONE
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x1a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x28, // STORE_NAME 'next'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2f, // STORE_NAME '__iter__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x30, // STORE_NAME '__next__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x31, // STORE_NAME 'extractfile'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x32, // STORE_NAME 'addfile'
    0x52, // LOAD_CONST_TRUE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x06, // MAKE_FUNCTION_DEFARGS 6
    0x16,0x3c, // STORE_NAME 'add'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x3f, // STORE_NAME 'close'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile___init__
static const byte fun_data_utarfile_TarFile___init__[79] = {
    0xc0,0x85,0x01,0x1e, // prelude
    0x1a,0x51,0x06,0x07,0x59, // names: __init__, self, name, mode, fileobj
    0x80,0x59,0x2d,0x26,0x27,0x23,0x46,0x2b,0x24,0x24, // code info
    0x2c,0x02, // BUILD_MAP 2
    0x10,0x23, // LOAD_CONST_STRING 'rb'
    0x10,0x19, // LOAD_CONST_STRING 'r'
    0x62, // STORE_MAP
    0x10,0x24, // LOAD_CONST_STRING 'wb'
    0x10,0x25, // LOAD_CONST_STRING 'w'
    0x62, // STORE_MAP
    0xc4, // STORE_FAST 4
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x5a, // LOAD_GLOBAL 'ValueError'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb3, // LOAD_FAST 3
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x1b, // STORE_ATTR 'f'
    0x42,0x4b, // JUMP 11
    0x12,0x5b, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0x34,0x02, // CALL_FUNCTION 2
    0xb0, // LOAD_FAST 0
    0x18,0x1b, // STORE_ATTR 'f'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x26, // STORE_ATTR 'subf'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x07, // STORE_ATTR 'mode'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x27, // STORE_ATTR 'offset'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_utarfile_TarFile___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 79,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_utarfile_TarFile___init__ + 9,
        .line_info_top = fun_data_utarfile_TarFile___init__ + 19,
        .opcodes = fun_data_utarfile_TarFile___init__ + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile_next
static const byte fun_data_utarfile_TarFile_next[194] = {
    0x49,0x26, // prelude
    0x28,0x51, // names: next, self
    0x80,0x65,0x25,0x28,0x2a,0x23,0x22,0x4d,0x74,0x29,0x42,0x25,0x32,0x2f,0x32,0x3a,0x31, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x26, // LOAD_ATTR 'subf'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb0, // LOAD_FAST 0
    0x13,0x26, // LOAD_ATTR 'subf'
    0x14,0x20, // LOAD_METHOD 'skip'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x54, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x29, // LOAD_METHOD 'struct'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x2a, // LOAD_METHOD 'addressof'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x12,0x40, // LOAD_GLOBAL 'TAR_HEADER'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x13,0x2b, // LOAD_ATTR 'LITTLE_ENDIAN'
    0x36,0x03, // CALL_METHOD 3
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x13,0x06, // LOAD_ATTR 'name'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x11, // LOAD_GLOBAL 'TarInfo'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0x12,0x5c, // LOAD_GLOBAL 'str'
    0xb2, // LOAD_FAST 2
    0x13,0x06, // LOAD_ATTR 'name'
    0x10,0x16, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0x14,0x2c, // LOAD_METHOD 'rstrip'
    0x10,0x2d, // LOAD_CONST_STRING '\x00'
    0x36,0x01, // CALL_METHOD 1
    0xb3, // LOAD_FAST 3
    0x18,0x06, // STORE_ATTR 'name'
    0x12,0x5d, // LOAD_GLOBAL 'int'
    0x12,0x5e, // LOAD_GLOBAL 'bytes'
    0xb2, // LOAD_FAST 2
    0x13,0x0a, // LOAD_ATTR 'size'
    0x34,0x01, // CALL_FUNCTION 1
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x02, // CALL_FUNCTION 2
    0xb3, // LOAD_FAST 3
    0x18,0x0a, // STORE_ATTR 'size'
    0x12,0x42, // LOAD_GLOBAL 'REGTYPE'
    0x12,0x41, // LOAD_GLOBAL 'DIRTYPE'
    0x2b,0x02, // BUILD_LIST 2
    0xb3, // LOAD_FAST 3
    0x13,0x06, // LOAD_ATTR 'name'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x10,0x2e, // LOAD_CONST_STRING '/'
    0xd9, // BINARY_OP 2 __eq__
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x18,0x22, // STORE_ATTR 'type'
    0x12,0x10, // LOAD_GLOBAL 'FileSection'
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0xb3, // LOAD_FAST 3
    0x13,0x0a, // LOAD_ATTR 'size'
    0x12,0x13, // LOAD_GLOBAL 'roundup'
    0xb3, // LOAD_FAST 3
    0x13,0x0a, // LOAD_ATTR 'size'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x03, // CALL_FUNCTION 3
    0x57, // DUP_TOP
    0xb0, // LOAD_FAST 0
    0x18,0x26, // STORE_ATTR 'subf'
    0xb3, // LOAD_FAST 3
    0x18,0x26, // STORE_ATTR 'subf'
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x13, // LOAD_GLOBAL 'roundup'
    0xb3, // LOAD_FAST 3
    0x13,0x0a, // LOAD_ATTR 'size'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x34,0x02, // CALL_FUNCTION 2
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile_next = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_TarFile_next,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 194,
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
        .line_info = fun_data_utarfile_TarFile_next + 4,
        .line_info_top = fun_data_utarfile_TarFile_next + 21,
        .opcodes = fun_data_utarfile_TarFile_next + 21,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile___iter__
static const byte fun_data_utarfile_TarFile___iter__[8] = {
    0x09,0x08, // prelude
    0x2f,0x51, // names: __iter__, self
    0x80,0x7b, // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile___iter__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_TarFile___iter__,
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
        .qstr_block_name_idx = 47,
        .line_info = fun_data_utarfile_TarFile___iter__ + 4,
        .line_info_top = fun_data_utarfile_TarFile___iter__ + 6,
        .opcodes = fun_data_utarfile_TarFile___iter__ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile___next__
static const byte fun_data_utarfile_TarFile___next__[25] = {
    0x19,0x0e, // prelude
    0x30,0x51, // names: __next__, self
    0x80,0x7e,0x26,0x25,0x23, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x28, // LOAD_METHOD 'next'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x12,0x5f, // LOAD_GLOBAL 'StopIteration'
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile___next__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_TarFile___next__,
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
        .qstr_block_name_idx = 48,
        .line_info = fun_data_utarfile_TarFile___next__ + 4,
        .line_info_top = fun_data_utarfile_TarFile___next__ + 9,
        .opcodes = fun_data_utarfile_TarFile___next__ + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile_extractfile
static const byte fun_data_utarfile_TarFile_extractfile[11] = {
    0x12,0x0a, // prelude
    0x31,0x51,0x60, // names: extractfile, self, tarinfo
    0x80,0x84, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x26, // LOAD_ATTR 'subf'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile_extractfile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_utarfile_TarFile_extractfile,
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
        .qstr_block_name_idx = 49,
        .line_info = fun_data_utarfile_TarFile_extractfile + 5,
        .line_info_top = fun_data_utarfile_TarFile_extractfile + 7,
        .opcodes = fun_data_utarfile_TarFile_extractfile + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile_addfile
static const byte fun_data_utarfile_TarFile_addfile[330] = {
    0xfb,0x01,0x44, // prelude
    0x32,0x51,0x60,0x59, // names: addfile, self, tarinfo, fileobj
    0x80,0x88,0x27,0x24,0x24,0x24,0x27,0x22,0x29,0x25,0x34,0x2c,0x33,0x2f,0x2f,0x2d,0x2f,0x56,0x4d,0x26,0x4d,0x29,0x6d,0x23,0x2d,0x29,0x28,0x23,0x26,0x29, // code info
    0x12,0x57, // LOAD_GLOBAL 'bytearray'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0x13,0x06, // LOAD_ATTR 'name'
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x13,0x33, // LOAD_ATTR 'finfo'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x12,0x17, // LOAD_GLOBAL '_isdir'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0x14,0x34, // LOAD_METHOD 'endswith'
    0x10,0x2e, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb4, // LOAD_FAST 4
    0x10,0x2e, // LOAD_CONST_STRING '/'
    0xe5, // BINARY_OP 14 __iadd__
    0xc4, // STORE_FAST 4
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x29, // LOAD_METHOD 'struct'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x14,0x2a, // LOAD_METHOD 'addressof'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x12,0x40, // LOAD_GLOBAL 'TAR_HEADER'
    0x12,0x02, // LOAD_GLOBAL 'uctypes'
    0x13,0x2b, // LOAD_ATTR 'LITTLE_ENDIAN'
    0x36,0x03, // CALL_METHOD 3
    0xc7, // STORE_FAST 7
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x06, // LOAD_ATTR 'name'
    0xb4, // LOAD_FAST 4
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x07, // LOAD_ATTR 'mode'
    0x10,0x35, // LOAD_CONST_STRING '%06o '
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x9f,0x7f, // LOAD_CONST_SMALL_INT 4095
    0xef, // BINARY_OP 24 __and__
    0xf8, // BINARY_OP 33 __mod__
    0x87, // LOAD_CONST_SMALL_INT 7
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x08, // LOAD_ATTR 'uid'
    0x10,0x35, // LOAD_CONST_STRING '%06o '
    0xb5, // LOAD_FAST 5
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x87, // LOAD_CONST_SMALL_INT 7
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x09, // LOAD_ATTR 'gid'
    0x10,0x35, // LOAD_CONST_STRING '%06o '
    0xb5, // LOAD_FAST 5
    0x85, // LOAD_CONST_SMALL_INT 5
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x87, // LOAD_CONST_SMALL_INT 7
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x0a, // LOAD_ATTR 'size'
    0x10,0x36, // LOAD_CONST_STRING '%011o '
    0xb6, // LOAD_FAST 6
    0xf8, // BINARY_OP 33 __mod__
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x0b, // LOAD_ATTR 'mtime'
    0x10,0x36, // LOAD_CONST_STRING '%011o '
    0xb5, // LOAD_FAST 5
    0x88, // LOAD_CONST_SMALL_INT 8
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x0d, // LOAD_ATTR 'typeflag'
    0x12,0x17, // LOAD_GLOBAL '_isdir'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x37, // LOAD_CONST_STRING '5'
    0x42,0x42, // JUMP 2
    0x10,0x38, // LOAD_CONST_STRING '0'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x0c, // LOAD_ATTR 'chksum'
    0x10,0x39, // LOAD_CONST_STRING ' '
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf4, // BINARY_OP 29 __mul__
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x61, // LOAD_GLOBAL 'sum'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0x12,0x14, // LOAD_GLOBAL '_setstring'
    0xb7, // LOAD_FAST 7
    0x13,0x0c, // LOAD_ATTR 'chksum'
    0x10,0x3a, // LOAD_CONST_STRING '%06o\x00'
    0xb8, // LOAD_FAST 8
    0xf8, // BINARY_OP 33 __mod__
    0x87, // LOAD_CONST_SMALL_INT 7
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3b, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x54, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0xb2, // LOAD_FAST 2
    0x44,0x7d, // POP_JUMP_IF_FALSE 61
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3b, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x14,0x1e, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0xb9, // LOAD_FAST 9
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0xb9, // LOAD_FAST 9
    0xd1, // UNARY_OP 1 __neg__
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0xef, // BINARY_OP 24 __and__
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x44,0x5c, // POP_JUMP_IF_FALSE 28
    0x12,0x57, // LOAD_GLOBAL 'bytearray'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3b, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x54, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile_addfile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_utarfile_TarFile_addfile,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 330,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 50,
        .line_info = fun_data_utarfile_TarFile_addfile + 7,
        .line_info_top = fun_data_utarfile_TarFile_addfile + 37,
        .opcodes = fun_data_utarfile_TarFile_addfile + 37,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile_add
static const byte fun_data_utarfile_TarFile_add[177] = {
    0xef,0x01,0x2a, // prelude
    0x3c,0x51,0x06,0x62, // names: add, self, name, recursive
    0x80,0xac,0x25,0x24,0x22,0x53,0x2a,0x23,0x52,0x28,0x22,0x32,0x28,0x27,0x23,0x2b,0x53, // code info
    0x12,0x11, // LOAD_GLOBAL 'TarInfo'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0x18,0x06, // STORE_ATTR 'name'
    0x48,0x0c, // SETUP_EXCEPT 12
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x3d, // LOAD_METHOD 'stat'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xb3, // LOAD_FAST 3
    0x18,0x33, // STORE_ATTR 'finfo'
    0x4a,0x14, // POP_EXCEPT_JUMP 20
    0x57, // DUP_TOP
    0x12,0x63, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x59, // POP_TOP
    0x12,0x64, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xb1, // LOAD_FAST 1
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x12,0x17, // LOAD_GLOBAL '_isdir'
    0xb3, // LOAD_FAST 3
    0x13,0x33, // LOAD_ATTR 'finfo'
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x53, // POP_JUMP_IF_TRUE 19
    0x12,0x18, // LOAD_GLOBAL '_isreg'
    0xb3, // LOAD_FAST 3
    0x13,0x33, // LOAD_ATTR 'finfo'
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x64, // LOAD_GLOBAL 'print'
    0xb1, // LOAD_FAST 1
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x17, // LOAD_GLOBAL '_isdir'
    0xb3, // LOAD_FAST 3
    0x13,0x33, // LOAD_ATTR 'finfo'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x12,0x41, // LOAD_GLOBAL 'DIRTYPE'
    0x42,0x42, // JUMP 2
    0x12,0x42, // LOAD_GLOBAL 'REGTYPE'
    0xb3, // LOAD_FAST 3
    0x18,0x22, // STORE_ATTR 'type'
    0xb3, // LOAD_FAST 3
    0x13,0x22, // LOAD_ATTR 'type'
    0x12,0x41, // LOAD_GLOBAL 'DIRTYPE'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x68, // POP_JUMP_IF_FALSE 40
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD 'addfile'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x44,0x5c, // POP_JUMP_IF_FALSE 28
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x3e, // LOAD_METHOD 'ilistdir'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x12, // FOR_ITER 18
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x14,0x3c, // LOAD_METHOD 'add'
    0xb1, // LOAD_FAST 1
    0x10,0x2e, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x2c, // JUMP -20
    0x42,0x4e, // JUMP 14
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD 'addfile'
    0xb3, // LOAD_FAST 3
    0x12,0x5b, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x23, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile_add = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_utarfile_TarFile_add,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 177,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 60,
        .line_info = fun_data_utarfile_TarFile_add + 7,
        .line_info_top = fun_data_utarfile_TarFile_add + 24,
        .opcodes = fun_data_utarfile_TarFile_add + 24,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of utarfile_TarFile
// frozen bytecode for file utarfile.py, scope utarfile_TarFile_close
static const byte fun_data_utarfile_TarFile_close[87] = {
    0x31,0x16, // prelude
    0x3f,0x51, // names: close, self
    0x80,0xc2,0x28,0x2f,0x2c,0x27,0x23,0x2f,0x28, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'mode'
    0x10,0x25, // LOAD_CONST_STRING 'w'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x74, // POP_JUMP_IF_FALSE 52
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3b, // LOAD_METHOD 'write'
    0x12,0x43, // LOAD_GLOBAL 'NUL'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf4, // BINARY_OP 29 __mul__
    0xf4, // BINARY_OP 29 __mul__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x44, // LOAD_GLOBAL 'BLOCKSIZE'
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf4, // BINARY_OP 29 __mul__
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x27, // STORE_ATTR 'offset'
    0xb0, // LOAD_FAST 0
    0x13,0x27, // LOAD_ATTR 'offset'
    0x12,0x45, // LOAD_GLOBAL 'RECORDSIZE'
    0xf8, // BINARY_OP 33 __mod__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3b, // LOAD_METHOD 'write'
    0x12,0x43, // LOAD_GLOBAL 'NUL'
    0x12,0x45, // LOAD_GLOBAL 'RECORDSIZE'
    0xb1, // LOAD_FAST 1
    0xf3, // BINARY_OP 28 __sub__
    0xf4, // BINARY_OP 29 __mul__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x1b, // LOAD_ATTR 'f'
    0x14,0x3f, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x1b, // STORE_ATTR 'f'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_utarfile_TarFile_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_utarfile_TarFile_close,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 87,
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
        .qstr_block_name_idx = 63,
        .line_info = fun_data_utarfile_TarFile_close + 4,
        .line_info_top = fun_data_utarfile_TarFile_close + 13,
        .opcodes = fun_data_utarfile_TarFile_close + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_utarfile_TarFile[] = {
    &raw_code_utarfile_TarFile___init__,
    &raw_code_utarfile_TarFile_next,
    &raw_code_utarfile_TarFile___iter__,
    &raw_code_utarfile_TarFile___next__,
    &raw_code_utarfile_TarFile_extractfile,
    &raw_code_utarfile_TarFile_addfile,
    &raw_code_utarfile_TarFile_add,
    &raw_code_utarfile_TarFile_close,
};

static const mp_raw_code_t raw_code_utarfile_TarFile = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_utarfile_TarFile,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 74,
    #endif
    .children = (void *)&children_utarfile_TarFile,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 8,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_utarfile_TarFile + 3,
        .line_info_top = fun_data_utarfile_TarFile + 17,
        .opcodes = fun_data_utarfile_TarFile + 17,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_utarfile__lt_module_gt_[] = {
    &raw_code_utarfile_roundup,
    &raw_code_utarfile_FileSection,
    &raw_code_utarfile_TarInfo,
    &raw_code_utarfile__setstring,
    &raw_code_utarfile__isdir,
    &raw_code_utarfile__isreg,
    &raw_code_utarfile_TarFile,
};

static const mp_raw_code_t raw_code_utarfile__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_utarfile__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 288,
    #endif
    .children = (void *)&children_utarfile__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 7,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_utarfile__lt_module_gt_ + 3,
        .line_info_top = fun_data_utarfile__lt_module_gt_ + 34,
        .opcodes = fun_data_utarfile__lt_module_gt_ + 34,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_utarfile[101] = {
    MP_QSTR_utarfile_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_uctypes,
    MP_QSTR_os,
    MP_QSTR_ARRAY,
    MP_QSTR_UINT8,
    MP_QSTR_name,
    MP_QSTR_mode,
    MP_QSTR_uid,
    MP_QSTR_gid,
    MP_QSTR_size,
    MP_QSTR_mtime,
    MP_QSTR_chksum,
    MP_QSTR_typeflag,
    MP_QSTR_dir,
    MP_QSTR_file,
    MP_QSTR_FileSection,
    MP_QSTR_TarInfo,
    MP_QSTR_TarFile,
    MP_QSTR_roundup,
    MP_QSTR__setstring,
    MP_QSTR_encode,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR__isdir,
    MP_QSTR__isreg,
    MP_QSTR_r,
    MP_QSTR___init__,
    MP_QSTR_f,
    MP_QSTR_content_len,
    MP_QSTR_align,
    MP_QSTR_read,
    MP_QSTR_readinto,
    MP_QSTR_skip,
    MP_QSTR___str__,
    MP_QSTR_type,
    MP_QSTR_rb,
    MP_QSTR_wb,
    MP_QSTR_w,
    MP_QSTR_subf,
    MP_QSTR_offset,
    MP_QSTR_next,
    MP_QSTR_struct,
    MP_QSTR_addressof,
    MP_QSTR_LITTLE_ENDIAN,
    MP_QSTR_rstrip,
    MP_QSTR__0x00_,
    MP_QSTR__slash_,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR_extractfile,
    MP_QSTR_addfile,
    MP_QSTR_finfo,
    MP_QSTR_endswith,
    MP_QSTR__percent_06o_space_,
    MP_QSTR__percent_011o_space_,
    MP_QSTR_5,
    MP_QSTR_0,
    MP_QSTR__space_,
    MP_QSTR__percent_06o_0x00_,
    MP_QSTR_write,
    MP_QSTR_add,
    MP_QSTR_stat,
    MP_QSTR_ilistdir,
    MP_QSTR_close,
    MP_QSTR_TAR_HEADER,
    MP_QSTR_DIRTYPE,
    MP_QSTR_REGTYPE,
    MP_QSTR_NUL,
    MP_QSTR_BLOCKSIZE,
    MP_QSTR_RECORDSIZE,
    MP_QSTR__S_IFMT,
    MP_QSTR__S_IFREG,
    MP_QSTR__S_IFDIR,
    MP_QSTR_val,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_b,
    MP_QSTR_s,
    MP_QSTR_maxlen,
    MP_QSTR_enumerate,
    MP_QSTR_self,
    MP_QSTR_aligned_len,
    MP_QSTR_sz,
    MP_QSTR_len,
    MP_QSTR_buf,
    MP_QSTR_memoryview,
    MP_QSTR_bytearray,
    MP_QSTR_min,
    MP_QSTR_fileobj,
    MP_QSTR_ValueError,
    MP_QSTR_open,
    MP_QSTR_str,
    MP_QSTR_int,
    MP_QSTR_bytes,
    MP_QSTR_StopIteration,
    MP_QSTR_tarinfo,
    MP_QSTR_sum,
    MP_QSTR_recursive,
    MP_QSTR_OSError,
    MP_QSTR_print,
};

// constants
static const mp_obj_str_t const_obj_utarfile_0 = {{&mp_type_bytes}, 46501, 1, (const byte*)"\x00"};
static const mp_obj_str_t const_obj_utarfile_6 = {{&mp_type_str}, 40209, 46, (const byte*)"\x69\x73\x20\x6e\x6f\x74\x20\x61\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x6f\x72\x20\x72\x65\x67\x75\x6c\x61\x72\x20\x66\x69\x6c\x65\x20\x2d\x20\x73\x6b\x69\x70\x70\x69\x6e\x67\x2e"};

// constant table
static const mp_rom_obj_t const_obj_table_data_utarfile[7] = {
    MP_ROM_PTR(&const_obj_utarfile_0),
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_QSTR(MP_QSTR_TarInfo_paren_open__percent_r_comma__space__percent_s_comma__space__percent_d_paren_close_),
    MP_ROM_QSTR(MP_QSTR_mode_space_must_space_be_space__squot_r_squot__space_or_space__squot_w_squot_),
    MP_ROM_QSTR(MP_QSTR_Cannot_space_stat),
    MP_ROM_QSTR(MP_QSTR__space__hyphen__space_skipping_dot_),
    MP_ROM_PTR(&const_obj_utarfile_6),
};

static const mp_frozen_module_t frozen_module_utarfile = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_utarfile,
        .obj_table = (mp_obj_t *)&const_obj_table_data_utarfile,
    },
    .rc = &raw_code_utarfile__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module music
// - original source file: build-standard/tulip/obj/frozen_mpy/music.mpy
// - frozen file name: music.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file music.py, scope music__lt_module_gt_
static const byte fun_data_music__lt_module_gt_[61] = {
    0x18,0x14, // prelude
    0x01, // names: <module>
    0x60,0x89,0x16,0x89,0x0a,0x8b,0x25,0x89,0x38, // code info
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x02, // LOAD_CONST_STRING 'Progression'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x02, // STORE_NAME 'Progression'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x03, // LOAD_CONST_STRING 'Key'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x03, // STORE_NAME 'Key'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x04, // LOAD_CONST_STRING 'Note'
    0x11,0x6c, // LOAD_NAME 'object'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'Note'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x05, // LOAD_CONST_STRING 'NoteClass'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'NoteClass'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x04, // MAKE_FUNCTION 4
    0x10,0x06, // LOAD_CONST_STRING 'Chord'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x06, // STORE_NAME 'Chord'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music__lt_module_gt_
// frozen bytecode for file music.py, scope music_Progression
static const byte fun_data_music_Progression[47] = {
    0x30,0x0a, // prelude
    0x02, // names: Progression
    0x68,0x60,0x32,0x48, // code info
    0x11,0x6d, // LOAD_NAME '__name__'
    0x16,0x6e, // STORE_NAME '__module__'
    0x10,0x02, // LOAD_CONST_STRING 'Progression'
    0x16,0x6f, // STORE_NAME '__qualname__'
    0x10,0x07, // LOAD_CONST_STRING 'I'
    0x10,0x08, // LOAD_CONST_STRING 'II'
    0x10,0x09, // LOAD_CONST_STRING 'III'
    0x10,0x0a, // LOAD_CONST_STRING 'IV'
    0x10,0x0b, // LOAD_CONST_STRING 'V'
    0x10,0x0c, // LOAD_CONST_STRING 'VI'
    0x10,0x0d, // LOAD_CONST_STRING 'VII'
    0x2b,0x07, // BUILD_LIST 7
    0x16,0x47, // STORE_NAME 'roman_major'
    0x32,0x00, // MAKE_FUNCTION 0
    0x11,0x47, // LOAD_NAME 'roman_major'
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x4b, // STORE_NAME 'roman_minor'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x42, // STORE_NAME '__init__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_Progression
// frozen bytecode for file music.py, scope music_Progression__lt_listcomp_gt_
static const byte fun_data_music_Progression__lt_listcomp_gt_[23] = {
    0x41,0x08, // prelude
    0x40,0x65, // names: <listcomp>, *
    0x80,0x07, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x41, // LOAD_METHOD 'lower'
    0x36,0x00, // CALL_METHOD 0
    0x2f,0x14, // STORE_COMP 20
    0x42,0x34, // JUMP -12
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Progression__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Progression__lt_listcomp_gt_,
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
        .qstr_block_name_idx = 64,
        .line_info = fun_data_music_Progression__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_music_Progression__lt_listcomp_gt_ + 6,
        .opcodes = fun_data_music_Progression__lt_listcomp_gt_ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Progression
// frozen bytecode for file music.py, scope music_Progression___init__
static const byte fun_data_music_Progression___init__[151] = {
    0x8b,0x12,0x22, // prelude
    0x42,0x81,0x05,0x43,0x44, // names: __init__, self, progression, key
    0x80,0x0b,0x24,0x24,0x45,0x27,0x22,0x1f,0x4c,0x22,0x1f,0x4c, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x43, // STORE_ATTR 'progression'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x44, // STORE_ATTR 'key'
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x18,0x45, // STORE_ATTR 'chords'
    0xb0, // LOAD_FAST 0
    0x13,0x43, // LOAD_ATTR 'progression'
    0x5f, // GET_ITER_STACK
    0x4b,0x6e, // FOR_ITER 110
    0xc3, // STORE_FAST 3
    0x48,0x23, // SETUP_EXCEPT 35
    0xb0, // LOAD_FAST 0
    0x13,0x45, // LOAD_ATTR 'chords'
    0x14,0x26, // LOAD_METHOD 'append'
    0x12,0x06, // LOAD_GLOBAL 'Chord'
    0xb0, // LOAD_FAST 0
    0x13,0x44, // LOAD_ATTR 'key'
    0x13,0x46, // LOAD_ATTR 'notes'
    0xb0, // LOAD_FAST 0
    0x13,0x47, // LOAD_ATTR 'roman_major'
    0x14,0x48, // LOAD_METHOD 'index'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x55, // LOAD_SUBSCR
    0x14,0x49, // LOAD_METHOD 'name'
    0x36,0x00, // CALL_METHOD 0
    0x10,0x4a, // LOAD_CONST_STRING ':maj'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x45, // POP_EXCEPT_JUMP 69
    0x57, // DUP_TOP
    0x12,0x81,0x06, // LOAD_GLOBAL 'ValueError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x7d, // POP_JUMP_IF_FALSE 61
    0x59, // POP_TOP
    0x48,0x23, // SETUP_EXCEPT 35
    0xb0, // LOAD_FAST 0
    0x13,0x45, // LOAD_ATTR 'chords'
    0x14,0x26, // LOAD_METHOD 'append'
    0x12,0x06, // LOAD_GLOBAL 'Chord'
    0xb0, // LOAD_FAST 0
    0x13,0x44, // LOAD_ATTR 'key'
    0x13,0x46, // LOAD_ATTR 'notes'
    0xb0, // LOAD_FAST 0
    0x13,0x4b, // LOAD_ATTR 'roman_minor'
    0x14,0x48, // LOAD_METHOD 'index'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x55, // LOAD_SUBSCR
    0x14,0x49, // LOAD_METHOD 'name'
    0x36,0x00, // CALL_METHOD 0
    0x10,0x4c, // LOAD_CONST_STRING ':min'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x15, // POP_EXCEPT_JUMP 21
    0x57, // DUP_TOP
    0x12,0x81,0x06, // LOAD_GLOBAL 'ValueError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x59, // POP_TOP
    0x12,0x81,0x07, // LOAD_GLOBAL 'print'
    0x23,0x49, // LOAD_CONST_OBJ 73
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x90,0x7f, // JUMP -112
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Progression___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_music_Progression___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 151,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 66,
        .line_info = fun_data_music_Progression___init__ + 8,
        .line_info_top = fun_data_music_Progression___init__ + 20,
        .opcodes = fun_data_music_Progression___init__ + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Progression[] = {
    &raw_code_music_Progression__lt_listcomp_gt_,
    &raw_code_music_Progression___init__,
};

static const mp_raw_code_t raw_code_music_Progression = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music_Progression,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 47,
    #endif
    .children = (void *)&children_music_Progression,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 2,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 2,
        .line_info = fun_data_music_Progression + 3,
        .line_info_top = fun_data_music_Progression + 7,
        .opcodes = fun_data_music_Progression + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music__lt_module_gt_
// frozen bytecode for file music.py, scope music_Key
static const byte fun_data_music_Key[25] = {
    0x00,0x0a, // prelude
    0x03, // names: Key
    0x88,0x1a,0x64,0x40, // code info
    0x11,0x6d, // LOAD_NAME '__name__'
    0x16,0x6e, // STORE_NAME '__module__'
    0x10,0x03, // LOAD_CONST_STRING 'Key'
    0x16,0x6f, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x42, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x50, // STORE_NAME 'note_in_key'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_Key
// frozen bytecode for file music.py, scope music_Key___init__
static const byte fun_data_music_Key___init__[35] = {
    0x22,0x10, // prelude
    0x42,0x81,0x05,0x4d, // names: __init__, self, key_string
    0x80,0x1b,0x24,0x2b, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x4d, // STORE_ATTR 'key_string'
    0x12,0x06, // LOAD_GLOBAL 'Chord'
    0xb1, // LOAD_FAST 1
    0x10,0x4e, // LOAD_CONST_STRING '/scale'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x4f, // STORE_ATTR 'chord'
    0xb0, // LOAD_FAST 0
    0x13,0x4f, // LOAD_ATTR 'chord'
    0x13,0x46, // LOAD_ATTR 'notes'
    0xb0, // LOAD_FAST 0
    0x18,0x46, // STORE_ATTR 'notes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Key___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Key___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 35,
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
        .qstr_block_name_idx = 66,
        .line_info = fun_data_music_Key___init__ + 6,
        .line_info_top = fun_data_music_Key___init__ + 10,
        .opcodes = fun_data_music_Key___init__ + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Key
// frozen bytecode for file music.py, scope music_Key_note_in_key
static const byte fun_data_music_Key_note_in_key[20] = {
    0x11,0x0a, // prelude
    0x50,0x52, // names: note_in_key, note
    0x80,0x20,0x2b, // code info
    0xb0, // LOAD_FAST 0
    0x12,0x81,0x05, // LOAD_GLOBAL 'self'
    0x13,0x46, // LOAD_ATTR 'notes'
    0xdd, // BINARY_OP 6 <in>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Key_note_in_key = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Key_note_in_key,
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
        .qstr_block_name_idx = 80,
        .line_info = fun_data_music_Key_note_in_key + 4,
        .line_info_top = fun_data_music_Key_note_in_key + 7,
        .opcodes = fun_data_music_Key_note_in_key + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Key[] = {
    &raw_code_music_Key___init__,
    &raw_code_music_Key_note_in_key,
};

static const mp_raw_code_t raw_code_music_Key = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music_Key,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 25,
    #endif
    .children = (void *)&children_music_Key,
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
        .qstr_block_name_idx = 3,
        .line_info = fun_data_music_Key + 3,
        .line_info_top = fun_data_music_Key + 7,
        .opcodes = fun_data_music_Key + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music__lt_module_gt_
// frozen bytecode for file music.py, scope music_Note
static const byte fun_data_music_Note[33] = {
    0x00,0x12, // prelude
    0x04, // names: Note
    0x88,0x24,0x80,0x08,0x84,0x0f,0x64,0x60, // code info
    0x11,0x6d, // LOAD_NAME '__name__'
    0x16,0x6e, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Note'
    0x16,0x6f, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x42, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x53, // STORE_NAME 'midinote'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x55, // STORE_NAME 'frequency'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_Note
// frozen bytecode for file music.py, scope music_Note___init__
static const byte fun_data_music_Note___init__[133] = {
    0xb1,0x80,0x80,0x40,0x1c, // prelude
    0x42,0x81,0x05, // names: __init__, self
    0x80,0x2d,0x2d,0x37,0x28,0x50,0x2c,0x48,0x2a,0x2a,0x48, // code info
    0x12,0x81,0x08, // LOAD_GLOBAL 'isinstance'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x81,0x09, // LOAD_GLOBAL 'int'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf8, // BINARY_OP 33 __mod__
    0x12,0x81,0x09, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x51, // STORE_ATTR 'octave'
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x52, // STORE_ATTR 'note'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x14,0x53, // LOAD_METHOD 'midinote'
    0x36,0x00, // CALL_METHOD 0
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x54, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x42,0x74, // JUMP 52
    0x12,0x81,0x08, // LOAD_GLOBAL 'isinstance'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x18,0x52, // STORE_ATTR 'note'
    0x42,0x4a, // JUMP 10
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x52, // STORE_ATTR 'note'
    0x12,0x81,0x0a, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x18,0x51, // STORE_ATTR 'octave'
    0x42,0x44, // JUMP 4
    0x84, // LOAD_CONST_SMALL_INT 4
    0xb0, // LOAD_FAST 0
    0x18,0x51, // STORE_ATTR 'octave'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Note___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x04,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Note___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 133,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 4,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 66,
        .line_info = fun_data_music_Note___init__ + 8,
        .line_info_top = fun_data_music_Note___init__ + 19,
        .opcodes = fun_data_music_Note___init__ + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Note
// frozen bytecode for file music.py, scope music_Note_midinote
static const byte fun_data_music_Note_midinote[22] = {
    0x21,0x0c, // prelude
    0x53,0x81,0x05, // names: midinote, self
    0x80,0x3c,0x60, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0x14,0x53, // LOAD_METHOD 'midinote'
    0x10,0x51, // LOAD_CONST_STRING 'octave'
    0xb0, // LOAD_FAST 0
    0x13,0x51, // LOAD_ATTR 'octave'
    0x36,0x82,0x00, // CALL_METHOD 256
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Note_midinote = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Note_midinote,
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
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 83,
        .line_info = fun_data_music_Note_midinote + 5,
        .line_info_top = fun_data_music_Note_midinote + 8,
        .opcodes = fun_data_music_Note_midinote + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Note
// frozen bytecode for file music.py, scope music_Note_frequency
static const byte fun_data_music_Note_frequency[22] = {
    0x21,0x0c, // prelude
    0x55,0x81,0x05, // names: frequency, self
    0x80,0x42,0x60, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0x14,0x55, // LOAD_METHOD 'frequency'
    0x10,0x51, // LOAD_CONST_STRING 'octave'
    0xb0, // LOAD_FAST 0
    0x13,0x51, // LOAD_ATTR 'octave'
    0x36,0x82,0x00, // CALL_METHOD 256
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Note_frequency = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Note_frequency,
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
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 85,
        .line_info = fun_data_music_Note_frequency + 5,
        .line_info_top = fun_data_music_Note_frequency + 8,
        .opcodes = fun_data_music_Note_frequency + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Note[] = {
    &raw_code_music_Note___init__,
    &raw_code_music_Note_midinote,
    &raw_code_music_Note_frequency,
};

static const mp_raw_code_t raw_code_music_Note = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music_Note,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = (void *)&children_music_Note,
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
        .qstr_block_name_idx = 4,
        .line_info = fun_data_music_Note + 3,
        .line_info_top = fun_data_music_Note + 11,
        .opcodes = fun_data_music_Note + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music__lt_module_gt_
// frozen bytecode for file music.py, scope music_NoteClass
static const byte fun_data_music_NoteClass[562] = {
    0xd8,0xa0,0x10,0x4a, // prelude
    0x05, // names: NoteClass
    0x88,0x4a,0x20,0x38,0x38,0x38,0x38,0x38,0x1f,0x1f,0x1f,0x1f,0x6c,0x40,0x22,0x1f,0x1f,0x58,0x2f,0x2f,0x51,0x3c,0x24,0x27,0x2b,0x27,0x2e,0x27,0x4e,0x84,0x0a,0x68,0x20,0x64,0x60,0x64, // code info
    0x11,0x6d, // LOAD_NAME '__name__'
    0x16,0x6e, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'NoteClass'
    0x16,0x6f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x23,0x13, // LOAD_CONST_OBJ 19
    0x23,0x14, // LOAD_CONST_OBJ 20
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x23,0x16, // LOAD_CONST_OBJ 22
    0x23,0x17, // LOAD_CONST_OBJ 23
    0x23,0x18, // LOAD_CONST_OBJ 24
    0x23,0x19, // LOAD_CONST_OBJ 25
    0x23,0x1a, // LOAD_CONST_OBJ 26
    0x23,0x1b, // LOAD_CONST_OBJ 27
    0x23,0x1c, // LOAD_CONST_OBJ 28
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0x23,0x1e, // LOAD_CONST_OBJ 30
    0x23,0x1f, // LOAD_CONST_OBJ 31
    0x23,0x20, // LOAD_CONST_OBJ 32
    0x23,0x21, // LOAD_CONST_OBJ 33
    0x23,0x22, // LOAD_CONST_OBJ 34
    0x23,0x23, // LOAD_CONST_OBJ 35
    0x23,0x24, // LOAD_CONST_OBJ 36
    0x23,0x25, // LOAD_CONST_OBJ 37
    0x23,0x26, // LOAD_CONST_OBJ 38
    0x23,0x27, // LOAD_CONST_OBJ 39
    0x23,0x28, // LOAD_CONST_OBJ 40
    0x23,0x29, // LOAD_CONST_OBJ 41
    0x23,0x2a, // LOAD_CONST_OBJ 42
    0x23,0x2b, // LOAD_CONST_OBJ 43
    0x23,0x2c, // LOAD_CONST_OBJ 44
    0x23,0x2d, // LOAD_CONST_OBJ 45
    0x23,0x2e, // LOAD_CONST_OBJ 46
    0x23,0x2f, // LOAD_CONST_OBJ 47
    0x23,0x30, // LOAD_CONST_OBJ 48
    0x23,0x31, // LOAD_CONST_OBJ 49
    0x23,0x32, // LOAD_CONST_OBJ 50
    0x23,0x33, // LOAD_CONST_OBJ 51
    0x23,0x34, // LOAD_CONST_OBJ 52
    0x23,0x35, // LOAD_CONST_OBJ 53
    0x23,0x36, // LOAD_CONST_OBJ 54
    0x23,0x37, // LOAD_CONST_OBJ 55
    0x23,0x38, // LOAD_CONST_OBJ 56
    0x23,0x39, // LOAD_CONST_OBJ 57
    0x23,0x3a, // LOAD_CONST_OBJ 58
    0x23,0x3b, // LOAD_CONST_OBJ 59
    0x23,0x3c, // LOAD_CONST_OBJ 60
    0x23,0x3d, // LOAD_CONST_OBJ 61
    0x23,0x3e, // LOAD_CONST_OBJ 62
    0x23,0x3f, // LOAD_CONST_OBJ 63
    0x23,0x40, // LOAD_CONST_OBJ 64
    0x23,0x41, // LOAD_CONST_OBJ 65
    0x23,0x42, // LOAD_CONST_OBJ 66
    0x23,0x43, // LOAD_CONST_OBJ 67
    0x23,0x44, // LOAD_CONST_OBJ 68
    0x23,0x45, // LOAD_CONST_OBJ 69
    0x23,0x46, // LOAD_CONST_OBJ 70
    0x23,0x47, // LOAD_CONST_OBJ 71
    0x22,0x88,0x17, // LOAD_CONST_SMALL_INT 1047
    0x22,0x88,0x55, // LOAD_CONST_SMALL_INT 1109
    0x22,0x89,0x17, // LOAD_CONST_SMALL_INT 1175
    0x22,0x89,0x5d, // LOAD_CONST_SMALL_INT 1245
    0x22,0x8a,0x27, // LOAD_CONST_SMALL_INT 1319
    0x22,0x8a,0x75, // LOAD_CONST_SMALL_INT 1397
    0x22,0x8b,0x48, // LOAD_CONST_SMALL_INT 1480
    0x22,0x8c,0x20, // LOAD_CONST_SMALL_INT 1568
    0x22,0x8c,0x7d, // LOAD_CONST_SMALL_INT 1661
    0x22,0x8d,0x60, // LOAD_CONST_SMALL_INT 1760
    0x22,0x8e,0x49, // LOAD_CONST_SMALL_INT 1865
    0x22,0x8f,0x38, // LOAD_CONST_SMALL_INT 1976
    0x22,0x90,0x2d, // LOAD_CONST_SMALL_INT 2093
    0x22,0x91,0x29, // LOAD_CONST_SMALL_INT 2217
    0x22,0x92,0x2d, // LOAD_CONST_SMALL_INT 2349
    0x22,0x93,0x39, // LOAD_CONST_SMALL_INT 2489
    0x22,0x94,0x4d, // LOAD_CONST_SMALL_INT 2637
    0x22,0x95,0x6a, // LOAD_CONST_SMALL_INT 2794
    0x22,0x97,0x10, // LOAD_CONST_SMALL_INT 2960
    0x22,0x98,0x40, // LOAD_CONST_SMALL_INT 3136
    0x22,0x99,0x7a, // LOAD_CONST_SMALL_INT 3322
    0x22,0x9b,0x40, // LOAD_CONST_SMALL_INT 3520
    0x22,0x9d,0x11, // LOAD_CONST_SMALL_INT 3729
    0x22,0x9e,0x6f, // LOAD_CONST_SMALL_INT 3951
    0x22,0xa0,0x5a, // LOAD_CONST_SMALL_INT 4186
    0x22,0xa2,0x53, // LOAD_CONST_SMALL_INT 4435
    0x22,0xa4,0x5b, // LOAD_CONST_SMALL_INT 4699
    0x22,0xa6,0x72, // LOAD_CONST_SMALL_INT 4978
    0x22,0xa9,0x1a, // LOAD_CONST_SMALL_INT 5274
    0x22,0xab,0x54, // LOAD_CONST_SMALL_INT 5588
    0x22,0xae,0x20, // LOAD_CONST_SMALL_INT 5920
    0x22,0xb1,0x00, // LOAD_CONST_SMALL_INT 6272
    0x22,0xb3,0x75, // LOAD_CONST_SMALL_INT 6645
    0x22,0xb7,0x00, // LOAD_CONST_SMALL_INT 7040
    0x22,0xba,0x23, // LOAD_CONST_SMALL_INT 7459
    0x22,0xbd,0x5e, // LOAD_CONST_SMALL_INT 7902
    0x2b,0x6c, // BUILD_LIST 108
    0x16,0x5c, // STORE_NAME 'freq'
    0x2c,0x15, // BUILD_MAP 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0e, // LOAD_CONST_STRING 'C'
    0x62, // STORE_MAP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x0f, // LOAD_CONST_STRING 'C#'
    0x62, // STORE_MAP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x10, // LOAD_CONST_STRING 'Db'
    0x62, // STORE_MAP
    0x82, // LOAD_CONST_SMALL_INT 2
    0x10,0x11, // LOAD_CONST_STRING 'D'
    0x62, // STORE_MAP
    0x83, // LOAD_CONST_SMALL_INT 3
    0x10,0x12, // LOAD_CONST_STRING 'D#'
    0x62, // STORE_MAP
    0x83, // LOAD_CONST_SMALL_INT 3
    0x10,0x13, // LOAD_CONST_STRING 'Eb'
    0x62, // STORE_MAP
    0x84, // LOAD_CONST_SMALL_INT 4
    0x10,0x14, // LOAD_CONST_STRING 'E'
    0x62, // STORE_MAP
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x15, // LOAD_CONST_STRING 'E#'
    0x62, // STORE_MAP
    0x84, // LOAD_CONST_SMALL_INT 4
    0x10,0x16, // LOAD_CONST_STRING 'Fb'
    0x62, // STORE_MAP
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x17, // LOAD_CONST_STRING 'F'
    0x62, // STORE_MAP
    0x86, // LOAD_CONST_SMALL_INT 6
    0x10,0x18, // LOAD_CONST_STRING 'F#'
    0x62, // STORE_MAP
    0x86, // LOAD_CONST_SMALL_INT 6
    0x10,0x19, // LOAD_CONST_STRING 'Gb'
    0x62, // STORE_MAP
    0x87, // LOAD_CONST_SMALL_INT 7
    0x10,0x1a, // LOAD_CONST_STRING 'G'
    0x62, // STORE_MAP
    0x88, // LOAD_CONST_SMALL_INT 8
    0x10,0x1b, // LOAD_CONST_STRING 'G#'
    0x62, // STORE_MAP
    0x88, // LOAD_CONST_SMALL_INT 8
    0x10,0x1c, // LOAD_CONST_STRING 'Ab'
    0x62, // STORE_MAP
    0x89, // LOAD_CONST_SMALL_INT 9
    0x10,0x1d, // LOAD_CONST_STRING 'A'
    0x62, // STORE_MAP
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x10,0x1e, // LOAD_CONST_STRING 'A#'
    0x62, // STORE_MAP
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x10,0x1f, // LOAD_CONST_STRING 'Bb'
    0x62, // STORE_MAP
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x10,0x20, // LOAD_CONST_STRING 'B'
    0x62, // STORE_MAP
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x10,0x21, // LOAD_CONST_STRING 'Cb'
    0x62, // STORE_MAP
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x10,0x22, // LOAD_CONST_STRING 'B#'
    0x62, // STORE_MAP
    0x16,0x46, // STORE_NAME 'notes'
    0x11,0x70, // LOAD_NAME 'list'
    0x11,0x46, // LOAD_NAME 'notes'
    0x14,0x23, // LOAD_METHOD 'keys'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x22, // FOR_ITER 34
    0x16,0x71, // STORE_NAME 'n'
    0x11,0x46, // LOAD_NAME 'notes'
    0x11,0x71, // LOAD_NAME 'n'
    0x55, // LOAD_SUBSCR
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf2, // BINARY_OP 27 __add__
    0x11,0x46, // LOAD_NAME 'notes'
    0x11,0x71, // LOAD_NAME 'n'
    0x10,0x24, // LOAD_CONST_STRING '1'
    0xf2, // BINARY_OP 27 __add__
    0x56, // STORE_SUBSCR
    0x11,0x46, // LOAD_NAME 'notes'
    0x11,0x71, // LOAD_NAME 'n'
    0x55, // LOAD_SUBSCR
    0x98, // LOAD_CONST_SMALL_INT 24
    0xf2, // BINARY_OP 27 __add__
    0x11,0x46, // LOAD_NAME 'notes'
    0x11,0x71, // LOAD_NAME 'n'
    0x10,0x25, // LOAD_CONST_STRING '2'
    0xf2, // BINARY_OP 27 __add__
    0x56, // STORE_SUBSCR
    0x42,0x1c, // JUMP -36
    0x10,0x0e, // LOAD_CONST_STRING 'C'
    0x10,0x0f, // LOAD_CONST_STRING 'C#'
    0x10,0x11, // LOAD_CONST_STRING 'D'
    0x10,0x12, // LOAD_CONST_STRING 'D#'
    0x10,0x14, // LOAD_CONST_STRING 'E'
    0x10,0x17, // LOAD_CONST_STRING 'F'
    0x10,0x18, // LOAD_CONST_STRING 'F#'
    0x10,0x1a, // LOAD_CONST_STRING 'G'
    0x10,0x1b, // LOAD_CONST_STRING 'G#'
    0x10,0x1d, // LOAD_CONST_STRING 'A'
    0x10,0x1e, // LOAD_CONST_STRING 'A#'
    0x10,0x20, // LOAD_CONST_STRING 'B'
    0x2b,0x0c, // BUILD_LIST 12
    0x16,0x72, // STORE_NAME 'basenames'
    0x2b,0x00, // BUILD_LIST 0
    0x16,0x58, // STORE_NAME 'names'
    0x11,0x72, // LOAD_NAME 'basenames'
    0x5f, // GET_ITER_STACK
    0x4b,0x0d, // FOR_ITER 13
    0x16,0x71, // STORE_NAME 'n'
    0x11,0x58, // LOAD_NAME 'names'
    0x14,0x26, // LOAD_METHOD 'append'
    0x11,0x71, // LOAD_NAME 'n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x31, // JUMP -15
    0x11,0x72, // LOAD_NAME 'basenames'
    0x5f, // GET_ITER_STACK
    0x4b,0x10, // FOR_ITER 16
    0x16,0x71, // STORE_NAME 'n'
    0x11,0x58, // LOAD_NAME 'names'
    0x14,0x26, // LOAD_METHOD 'append'
    0x11,0x71, // LOAD_NAME 'n'
    0x10,0x24, // LOAD_CONST_STRING '1'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x2e, // JUMP -18
    0x11,0x72, // LOAD_NAME 'basenames'
    0x5f, // GET_ITER_STACK
    0x4b,0x10, // FOR_ITER 16
    0x16,0x71, // STORE_NAME 'n'
    0x11,0x58, // LOAD_NAME 'names'
    0x14,0x26, // LOAD_METHOD 'append'
    0x11,0x71, // LOAD_NAME 'n'
    0x10,0x25, // LOAD_CONST_STRING '2'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x2e, // JUMP -18
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x42, // STORE_NAME '__init__'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x53, // STORE_NAME 'midinote'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x49, // STORE_NAME 'name'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x5a, // STORE_NAME '__repr__'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x55, // STORE_NAME 'frequency'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_NoteClass
// frozen bytecode for file music.py, scope music_NoteClass___init__
static const byte fun_data_music_NoteClass___init__[59] = {
    0x2a,0x16, // prelude
    0x42,0x81,0x05,0x52, // names: __init__, self, note
    0x80,0x66,0x24,0x2b,0x4e,0x44,0x27, // code info
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x56, // STORE_ATTR 'valid'
    0x12,0x81,0x08, // LOAD_GLOBAL 'isinstance'
    0xb1, // LOAD_FAST 1
    0x12,0x81,0x0b, // LOAD_GLOBAL 'str'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x13,0x46, // LOAD_ATTR 'notes'
    0x14,0x57, // LOAD_METHOD 'get'
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x36,0x02, // CALL_METHOD 2
    0xb0, // LOAD_FAST 0
    0x18,0x52, // STORE_ATTR 'note'
    0x42,0x44, // JUMP 4
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x52, // STORE_ATTR 'note'
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x56, // STORE_ATTR 'valid'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_NoteClass___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_NoteClass___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 59,
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
        .qstr_block_name_idx = 66,
        .line_info = fun_data_music_NoteClass___init__ + 6,
        .line_info_top = fun_data_music_NoteClass___init__ + 13,
        .opcodes = fun_data_music_NoteClass___init__ + 13,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_NoteClass
// frozen bytecode for file music.py, scope music_NoteClass_midinote
static const byte fun_data_music_NoteClass_midinote[17] = {
    0xa2,0x01,0x0c, // prelude
    0x53,0x81,0x05,0x51, // names: midinote, self, octave
    0x80,0x71, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0xb1, // LOAD_FAST 1
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf4, // BINARY_OP 29 __mul__
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_NoteClass_midinote = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_NoteClass_midinote,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 17,
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
        .qstr_block_name_idx = 83,
        .line_info = fun_data_music_NoteClass_midinote + 7,
        .line_info_top = fun_data_music_NoteClass_midinote + 9,
        .opcodes = fun_data_music_NoteClass_midinote + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_NoteClass
// frozen bytecode for file music.py, scope music_NoteClass_name
static const byte fun_data_music_NoteClass_name[29] = {
    0x11,0x0e, // prelude
    0x49,0x81,0x05, // names: name, self
    0x80,0x74,0x27,0x48, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb0, // LOAD_FAST 0
    0x13,0x58, // LOAD_ATTR 'names'
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0x10,0x59, // LOAD_CONST_STRING 'unknown'
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_NoteClass_name = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_NoteClass_name,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
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
        .qstr_block_name_idx = 73,
        .line_info = fun_data_music_NoteClass_name + 5,
        .line_info_top = fun_data_music_NoteClass_name + 9,
        .opcodes = fun_data_music_NoteClass_name + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_NoteClass
// frozen bytecode for file music.py, scope music_NoteClass___repr__
static const byte fun_data_music_NoteClass___repr__[16] = {
    0x19,0x0a, // prelude
    0x5a,0x81,0x05, // names: __repr__, self
    0x80,0x7a, // code info
    0x10,0x5b, // LOAD_CONST_STRING '%s'
    0xb0, // LOAD_FAST 0
    0x14,0x49, // LOAD_METHOD 'name'
    0x36,0x00, // CALL_METHOD 0
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_NoteClass___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_NoteClass___repr__,
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
        .qstr_block_name_idx = 90,
        .line_info = fun_data_music_NoteClass___repr__ + 5,
        .line_info_top = fun_data_music_NoteClass___repr__ + 7,
        .opcodes = fun_data_music_NoteClass___repr__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_NoteClass
// frozen bytecode for file music.py, scope music_NoteClass_frequency
static const byte fun_data_music_NoteClass_frequency[21] = {
    0xa2,0x01,0x0c, // prelude
    0x55,0x81,0x05,0x51, // names: frequency, self, octave
    0x80,0x7d, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x5c, // LOAD_ATTR 'freq'
    0xb1, // LOAD_FAST 1
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf4, // BINARY_OP 29 __mul__
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR 'note'
    0xf2, // BINARY_OP 27 __add__
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_NoteClass_frequency = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_NoteClass_frequency,
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
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 85,
        .line_info = fun_data_music_NoteClass_frequency + 7,
        .line_info_top = fun_data_music_NoteClass_frequency + 9,
        .opcodes = fun_data_music_NoteClass_frequency + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_NoteClass[] = {
    &raw_code_music_NoteClass___init__,
    &raw_code_music_NoteClass_midinote,
    &raw_code_music_NoteClass_name,
    &raw_code_music_NoteClass___repr__,
    &raw_code_music_NoteClass_frequency,
};

static const mp_raw_code_t raw_code_music_NoteClass = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music_NoteClass,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 562,
    #endif
    .children = (void *)&children_music_NoteClass,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 108,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_music_NoteClass + 5,
        .line_info_top = fun_data_music_NoteClass + 41,
        .opcodes = fun_data_music_NoteClass + 41,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music__lt_module_gt_
// frozen bytecode for file music.py, scope music_Chord
static const byte fun_data_music_Chord[523] = {
    0x88,0x10,0x56, // prelude
    0x06, // names: Chord
    0x88,0x82,0x60,0x1f,0x1f,0x41,0x42,0x2b,0x2b,0x2d,0x32,0x32,0x2d,0x2d,0x33,0x29,0x2c,0x27,0x2e,0x2b,0x2e,0x2b,0x30,0x30,0x30,0x2d,0x30,0x2d,0x2b,0x2d,0x31,0x33,0x2c,0x6e,0x84,0x09,0x64,0x20,0x84,0x15,0x64,0x68, // code info
    0x11,0x6d, // LOAD_NAME '__name__'
    0x16,0x6e, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'Chord'
    0x16,0x6f, // STORE_NAME '__qualname__'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x84, // LOAD_CONST_SMALL_INT 4
    0x85, // LOAD_CONST_SMALL_INT 5
    0x86, // LOAD_CONST_SMALL_INT 6
    0x86, // LOAD_CONST_SMALL_INT 6
    0x87, // LOAD_CONST_SMALL_INT 7
    0x88, // LOAD_CONST_SMALL_INT 8
    0x89, // LOAD_CONST_SMALL_INT 9
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x8d, // LOAD_CONST_SMALL_INT 13
    0x8e, // LOAD_CONST_SMALL_INT 14
    0x93, // LOAD_CONST_SMALL_INT 19
    0x95, // LOAD_CONST_SMALL_INT 21
    0x2b,0x12, // BUILD_LIST 18
    0x30,0x12, // UNPACK_SEQUENCE 18
    0x16,0x73, // STORE_NAME 'root'
    0x16,0x74, // STORE_NAME 'minor_second'
    0x16,0x75, // STORE_NAME 'major_second'
    0x16,0x76, // STORE_NAME 'minor_third'
    0x16,0x77, // STORE_NAME 'major_third'
    0x16,0x78, // STORE_NAME 'perfect_fourth'
    0x16,0x79, // STORE_NAME 'diminished_fifth'
    0x16,0x7a, // STORE_NAME 'augmented_fourth'
    0x16,0x7b, // STORE_NAME 'perfect_fifth'
    0x16,0x7c, // STORE_NAME 'minor_sixth'
    0x16,0x7d, // STORE_NAME 'major_sixth'
    0x16,0x7e, // STORE_NAME 'minor_seventh'
    0x16,0x7f, // STORE_NAME 'major_seventh'
    0x16,0x81,0x00, // STORE_NAME 'perfect_octave'
    0x16,0x81,0x01, // STORE_NAME 'minor_ninth'
    0x16,0x81,0x02, // STORE_NAME 'major_ninth'
    0x16,0x81,0x03, // STORE_NAME 'perfect_fifth_octave'
    0x16,0x81,0x04, // STORE_NAME 'major_thirteenth'
    0x2c,0x1b, // BUILD_MAP 27
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x27, // LOAD_CONST_STRING 'maj'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x76, // LOAD_NAME 'minor_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x28, // LOAD_CONST_STRING 'min'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x29, // LOAD_CONST_STRING '7'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x2a, // LOAD_CONST_STRING '7(#9)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x2b, // LOAD_CONST_STRING '7(b9)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x76, // LOAD_NAME 'minor_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x2c, // LOAD_CONST_STRING 'min7'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7f, // LOAD_NAME 'major_seventh'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x2d, // LOAD_CONST_STRING 'maj7'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7f, // LOAD_NAME 'major_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x11,0x81,0x04, // LOAD_NAME 'major_thirteenth'
    0x2b,0x06, // BUILD_LIST 6
    0x10,0x2e, // LOAD_CONST_STRING 'maj13'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x02, // BUILD_LIST 2
    0x10,0x2f, // LOAD_CONST_STRING 'maj/5'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x81,0x03, // LOAD_NAME 'perfect_fifth_octave'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x30, // LOAD_CONST_STRING '5'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x2b,0x01, // BUILD_LIST 1
    0x10,0x24, // LOAD_CONST_STRING '1'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x31, // LOAD_CONST_STRING 'add9'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x32, // LOAD_CONST_STRING 'maj/3'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x81,0x04, // LOAD_NAME 'major_thirteenth'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x33, // LOAD_CONST_STRING 'maj/13'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x78, // LOAD_NAME 'perfect_fourth'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x34, // LOAD_CONST_STRING 'sus4'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7f, // LOAD_NAME 'major_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x35, // LOAD_CONST_STRING 'maj(9)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x78, // LOAD_NAME 'perfect_fourth'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x36, // LOAD_CONST_STRING 'sus4(b7,9)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7f, // LOAD_NAME 'major_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x37, // LOAD_CONST_STRING 'maj/9'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x78, // LOAD_NAME 'perfect_fourth'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x38, // LOAD_CONST_STRING 'sus4(b7)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x76, // LOAD_NAME 'minor_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x39, // LOAD_CONST_STRING 'min9'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x3a, // LOAD_CONST_STRING 'maj/b7'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x76, // LOAD_NAME 'minor_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x2b,0x03, // BUILD_LIST 3
    0x10,0x3b, // LOAD_CONST_STRING 'min/3'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7d, // LOAD_NAME 'major_sixth'
    0x2b,0x04, // BUILD_LIST 4
    0x10,0x3c, // LOAD_CONST_STRING 'maj6'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x77, // LOAD_NAME 'major_third'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7d, // LOAD_NAME 'major_sixth'
    0x11,0x7f, // LOAD_NAME 'major_seventh'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0x2b,0x05, // BUILD_LIST 5
    0x10,0x3d, // LOAD_CONST_STRING 'maj6(b7)'
    0x62, // STORE_MAP
    0x11,0x73, // LOAD_NAME 'root'
    0x11,0x78, // LOAD_NAME 'perfect_fourth'
    0x11,0x7b, // LOAD_NAME 'perfect_fifth'
    0x11,0x7e, // LOAD_NAME 'minor_seventh'
    0x11,0x81,0x02, // LOAD_NAME 'major_ninth'
    0x11,0x81,0x04, // LOAD_NAME 'major_thirteenth'
    0x2b,0x06, // BUILD_LIST 6
    0x23,0x48, // LOAD_CONST_OBJ 72
    0x62, // STORE_MAP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x85, // LOAD_CONST_SMALL_INT 5
    0x87, // LOAD_CONST_SMALL_INT 7
    0x89, // LOAD_CONST_SMALL_INT 9
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x2b,0x07, // BUILD_LIST 7
    0x10,0x3e, // LOAD_CONST_STRING 'maj/scale'
    0x62, // STORE_MAP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x85, // LOAD_CONST_SMALL_INT 5
    0x87, // LOAD_CONST_SMALL_INT 7
    0x88, // LOAD_CONST_SMALL_INT 8
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2b,0x07, // BUILD_LIST 7
    0x10,0x3f, // LOAD_CONST_STRING 'min/scale'
    0x62, // STORE_MAP
    0x16,0x68, // STORE_NAME 'annotations_map'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x42, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x5a, // STORE_NAME '__repr__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x62, // STORE_NAME 'parse_chord'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x58, // STORE_NAME 'names'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x6a, // STORE_NAME 'frequencies'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x6b, // STORE_NAME 'midinotes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord___init__
static const byte fun_data_music_Chord___init__[55] = {
    0x22,0x18, // prelude
    0x42,0x81,0x05,0x5d, // names: __init__, self, chord_string
    0x80,0xa7,0x24,0x25,0x25,0x25,0x24,0x29, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x5d, // STORE_ATTR 'chord_string'
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x18,0x46, // STORE_ATTR 'notes'
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x18,0x5e, // STORE_ATTR 'annotation'
    0x10,0x59, // LOAD_CONST_STRING 'unknown'
    0xb0, // LOAD_FAST 0
    0x18,0x5f, // STORE_ATTR 'annotation_name'
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x56, // STORE_ATTR 'valid'
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0x10,0x60, // LOAD_CONST_STRING 'none'
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x61, // STORE_ATTR 'root_note'
    0xb0, // LOAD_FAST 0
    0x14,0x62, // LOAD_METHOD 'parse_chord'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 55,
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
        .qstr_block_name_idx = 66,
        .line_info = fun_data_music_Chord___init__ + 6,
        .line_info_top = fun_data_music_Chord___init__ + 14,
        .opcodes = fun_data_music_Chord___init__ + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord___repr__
static const byte fun_data_music_Chord___repr__[25] = {
    0x29,0x0a, // prelude
    0x5a,0x81,0x05, // names: __repr__, self
    0x80,0xb0, // code info
    0x23,0x4a, // LOAD_CONST_OBJ 74
    0xb0, // LOAD_FAST 0
    0x13,0x61, // LOAD_ATTR 'root_note'
    0xb0, // LOAD_FAST 0
    0x13,0x5f, // LOAD_ATTR 'annotation_name'
    0xb0, // LOAD_FAST 0
    0x13,0x46, // LOAD_ATTR 'notes'
    0xb0, // LOAD_FAST 0
    0x13,0x5d, // LOAD_ATTR 'chord_string'
    0x2a,0x04, // BUILD_TUPLE 4
    0xf8, // BINARY_OP 33 __mod__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord___repr__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Chord___repr__,
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
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 90,
        .line_info = fun_data_music_Chord___repr__ + 5,
        .line_info_top = fun_data_music_Chord___repr__ + 7,
        .opcodes = fun_data_music_Chord___repr__ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord_parse_chord
static const byte fun_data_music_Chord_parse_chord[147] = {
    0x42,0x25, // prelude
    0x62,0x81,0x05,0x5d, // names: parse_chord, self, chord_string
    0x80,0xb4,0x52,0x42,0x2d,0x29,0x28,0x25,0x42,0x2f,0x4f,0x2b,0x47,0x45,0x00, // code info
    0xb1, // LOAD_FAST 1
    0x10,0x63, // LOAD_CONST_STRING 'N'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0xb1, // LOAD_FAST 1
    0x10,0x64, // LOAD_CONST_STRING '&pause'
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0xb1, // LOAD_FAST 1
    0x10,0x65, // LOAD_CONST_STRING '*'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xb1, // LOAD_FAST 1
    0x14,0x66, // LOAD_METHOD 'find'
    0x10,0x67, // LOAD_CONST_STRING ':'
    0x36,0x01, // CALL_METHOD 1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc2, // STORE_FAST 2
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x61, // STORE_ATTR 'root_note'
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x61, // LOAD_ATTR 'root_note'
    0x13,0x56, // LOAD_ATTR 'valid'
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x50, // LOAD_CONST_FALSE
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x56, // STORE_ATTR 'valid'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x66, // LOAD_METHOD 'find'
    0x10,0x67, // LOAD_CONST_STRING ':'
    0x36,0x01, // CALL_METHOD 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x68, // LOAD_ATTR 'annotations_map'
    0x14,0x57, // LOAD_METHOD 'get'
    0xb3, // LOAD_FAST 3
    0x2b,0x00, // BUILD_LIST 0
    0x36,0x02, // CALL_METHOD 2
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x69, // STORE_ATTR 'annotations'
    0x12,0x81,0x0a, // LOAD_GLOBAL 'len'
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x69, // LOAD_ATTR 'annotations'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb3, // LOAD_FAST 3
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x5f, // STORE_ATTR 'annotation_name'
    0x42,0x45, // JUMP 5
    0x50, // LOAD_CONST_FALSE
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x56, // STORE_ATTR 'valid'
    0xb0, // LOAD_FAST 0
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x69, // LOAD_ATTR 'annotations'
    0x34,0x01, // CALL_FUNCTION 1
    0x25,0x00, // LOAD_DEREF 0
    0x18,0x46, // STORE_ATTR 'notes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of music_Chord_parse_chord
// frozen bytecode for file music.py, scope music_Chord_parse_chord__lt_listcomp_gt_
static const byte fun_data_music_Chord_parse_chord__lt_listcomp_gt_[31] = {
    0x52,0x0a, // prelude
    0x40,0x65,0x65, // names: <listcomp>, *, *
    0x80,0xc6, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x11, // FOR_ITER 17
    0xc2, // STORE_FAST 2
    0x12,0x05, // LOAD_GLOBAL 'NoteClass'
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x61, // LOAD_ATTR 'root_note'
    0x13,0x52, // LOAD_ATTR 'note'
    0xb2, // LOAD_FAST 2
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x2d, // JUMP -19
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord_parse_chord__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_parse_chord__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 31,
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
        .qstr_block_name_idx = 64,
        .line_info = fun_data_music_Chord_parse_chord__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_music_Chord_parse_chord__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_music_Chord_parse_chord__lt_listcomp_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Chord_parse_chord[] = {
    &raw_code_music_Chord_parse_chord__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_music_Chord_parse_chord = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_parse_chord,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 147,
    #endif
    .children = (void *)&children_music_Chord_parse_chord,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 98,
        .line_info = fun_data_music_Chord_parse_chord + 6,
        .line_info_top = fun_data_music_Chord_parse_chord + 20,
        .opcodes = fun_data_music_Chord_parse_chord + 21,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord_names
static const byte fun_data_music_Chord_names[15] = {
    0x11,0x0a, // prelude
    0x58,0x81,0x05, // names: names, self
    0x80,0xc9, // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x13,0x46, // LOAD_ATTR 'notes'
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of music_Chord_names
// frozen bytecode for file music.py, scope music_Chord_names__lt_listcomp_gt_
static const byte fun_data_music_Chord_names__lt_listcomp_gt_[23] = {
    0x41,0x08, // prelude
    0x40,0x65, // names: <listcomp>, *
    0x80,0xc9, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x49, // LOAD_METHOD 'name'
    0x36,0x00, // CALL_METHOD 0
    0x2f,0x14, // STORE_COMP 20
    0x42,0x34, // JUMP -12
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord_names__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Chord_names__lt_listcomp_gt_,
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
        .qstr_block_name_idx = 64,
        .line_info = fun_data_music_Chord_names__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_music_Chord_names__lt_listcomp_gt_ + 6,
        .opcodes = fun_data_music_Chord_names__lt_listcomp_gt_ + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Chord_names[] = {
    &raw_code_music_Chord_names__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_music_Chord_names = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_music_Chord_names,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 15,
    #endif
    .children = (void *)&children_music_Chord_names,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 88,
        .line_info = fun_data_music_Chord_names + 5,
        .line_info_top = fun_data_music_Chord_names + 7,
        .opcodes = fun_data_music_Chord_names + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord_frequencies
static const byte fun_data_music_Chord_frequencies[20] = {
    0x9a,0x01,0x0d, // prelude
    0x6a,0x81,0x05,0x51, // names: frequencies, self, octave
    0x80,0xcc,0x01, // code info
    0xb1, // LOAD_FAST 1
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0xb0, // LOAD_FAST 0
    0x13,0x46, // LOAD_ATTR 'notes'
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of music_Chord_frequencies
// frozen bytecode for file music.py, scope music_Chord_frequencies__lt_listcomp_gt_
static const byte fun_data_music_Chord_frequencies__lt_listcomp_gt_[29] = {
    0x5a,0x0a, // prelude
    0x40,0x65,0x65, // names: <listcomp>, *, *
    0x80,0xcc, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0f, // FOR_ITER 15
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x55, // LOAD_METHOD 'frequency'
    0x10,0x51, // LOAD_CONST_STRING 'octave'
    0x25,0x00, // LOAD_DEREF 0
    0x36,0x82,0x00, // CALL_METHOD 256
    0x2f,0x14, // STORE_COMP 20
    0x42,0x2f, // JUMP -17
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord_frequencies__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_frequencies__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 64,
        .line_info = fun_data_music_Chord_frequencies__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_music_Chord_frequencies__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_music_Chord_frequencies__lt_listcomp_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Chord_frequencies[] = {
    &raw_code_music_Chord_frequencies__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_music_Chord_frequencies = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_frequencies,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = (void *)&children_music_Chord_frequencies,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 106,
        .line_info = fun_data_music_Chord_frequencies + 7,
        .line_info_top = fun_data_music_Chord_frequencies + 9,
        .opcodes = fun_data_music_Chord_frequencies + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of music_Chord
// frozen bytecode for file music.py, scope music_Chord_midinotes
static const byte fun_data_music_Chord_midinotes[20] = {
    0x9a,0x01,0x0d, // prelude
    0x6b,0x81,0x05,0x51, // names: midinotes, self, octave
    0x80,0xcf,0x01, // code info
    0xb1, // LOAD_FAST 1
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0xb0, // LOAD_FAST 0
    0x13,0x46, // LOAD_ATTR 'notes'
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of music_Chord_midinotes
// frozen bytecode for file music.py, scope music_Chord_midinotes__lt_listcomp_gt_
static const byte fun_data_music_Chord_midinotes__lt_listcomp_gt_[29] = {
    0x5a,0x0a, // prelude
    0x40,0x65,0x65, // names: <listcomp>, *, *
    0x80,0xcf, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0f, // FOR_ITER 15
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x53, // LOAD_METHOD 'midinote'
    0x10,0x51, // LOAD_CONST_STRING 'octave'
    0x25,0x00, // LOAD_DEREF 0
    0x36,0x82,0x00, // CALL_METHOD 256
    0x2f,0x14, // STORE_COMP 20
    0x42,0x2f, // JUMP -17
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_music_Chord_midinotes__lt_listcomp_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_midinotes__lt_listcomp_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 29,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 64,
        .line_info = fun_data_music_Chord_midinotes__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_music_Chord_midinotes__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_music_Chord_midinotes__lt_listcomp_gt_ + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Chord_midinotes[] = {
    &raw_code_music_Chord_midinotes__lt_listcomp_gt_,
};

static const mp_raw_code_t raw_code_music_Chord_midinotes = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_music_Chord_midinotes,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = (void *)&children_music_Chord_midinotes,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 1,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 107,
        .line_info = fun_data_music_Chord_midinotes + 7,
        .line_info_top = fun_data_music_Chord_midinotes + 9,
        .opcodes = fun_data_music_Chord_midinotes + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music_Chord[] = {
    &raw_code_music_Chord___init__,
    &raw_code_music_Chord___repr__,
    &raw_code_music_Chord_parse_chord,
    &raw_code_music_Chord_names,
    &raw_code_music_Chord_frequencies,
    &raw_code_music_Chord_midinotes,
};

static const mp_raw_code_t raw_code_music_Chord = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music_Chord,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 523,
    #endif
    .children = (void *)&children_music_Chord,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 6,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_music_Chord + 4,
        .line_info_top = fun_data_music_Chord + 46,
        .opcodes = fun_data_music_Chord + 46,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_music__lt_module_gt_[] = {
    &raw_code_music_Progression,
    &raw_code_music_Key,
    &raw_code_music_Note,
    &raw_code_music_NoteClass,
    &raw_code_music_Chord,
};

static const mp_raw_code_t raw_code_music__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_music__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 61,
    #endif
    .children = (void *)&children_music__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 5,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_music__lt_module_gt_ + 3,
        .line_info_top = fun_data_music__lt_module_gt_ + 12,
        .opcodes = fun_data_music__lt_module_gt_ + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_music[140] = {
    MP_QSTR_music_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_Progression,
    MP_QSTR_Key,
    MP_QSTR_Note,
    MP_QSTR_NoteClass,
    MP_QSTR_Chord,
    MP_QSTR_I,
    MP_QSTR_II,
    MP_QSTR_III,
    MP_QSTR_IV,
    MP_QSTR_V,
    MP_QSTR_VI,
    MP_QSTR_VII,
    MP_QSTR_C,
    MP_QSTR_C_hash_,
    MP_QSTR_Db,
    MP_QSTR_D,
    MP_QSTR_D_hash_,
    MP_QSTR_Eb,
    MP_QSTR_E,
    MP_QSTR_E_hash_,
    MP_QSTR_Fb,
    MP_QSTR_F,
    MP_QSTR_F_hash_,
    MP_QSTR_Gb,
    MP_QSTR_G,
    MP_QSTR_G_hash_,
    MP_QSTR_Ab,
    MP_QSTR_A,
    MP_QSTR_A_hash_,
    MP_QSTR_Bb,
    MP_QSTR_B,
    MP_QSTR_Cb,
    MP_QSTR_B_hash_,
    MP_QSTR_keys,
    MP_QSTR_1,
    MP_QSTR_2,
    MP_QSTR_append,
    MP_QSTR_maj,
    MP_QSTR_min,
    MP_QSTR_7,
    MP_QSTR_7_paren_open__hash_9_paren_close_,
    MP_QSTR_7_paren_open_b9_paren_close_,
    MP_QSTR_min7,
    MP_QSTR_maj7,
    MP_QSTR_maj13,
    MP_QSTR_maj_slash_5,
    MP_QSTR_5,
    MP_QSTR_add9,
    MP_QSTR_maj_slash_3,
    MP_QSTR_maj_slash_13,
    MP_QSTR_sus4,
    MP_QSTR_maj_paren_open_9_paren_close_,
    MP_QSTR_sus4_paren_open_b7_comma_9_paren_close_,
    MP_QSTR_maj_slash_9,
    MP_QSTR_sus4_paren_open_b7_paren_close_,
    MP_QSTR_min9,
    MP_QSTR_maj_slash_b7,
    MP_QSTR_min_slash_3,
    MP_QSTR_maj6,
    MP_QSTR_maj6_paren_open_b7_paren_close_,
    MP_QSTR_maj_slash_scale,
    MP_QSTR_min_slash_scale,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_lower,
    MP_QSTR___init__,
    MP_QSTR_progression,
    MP_QSTR_key,
    MP_QSTR_chords,
    MP_QSTR_notes,
    MP_QSTR_roman_major,
    MP_QSTR_index,
    MP_QSTR_name,
    MP_QSTR__colon_maj,
    MP_QSTR_roman_minor,
    MP_QSTR__colon_min,
    MP_QSTR_key_string,
    MP_QSTR__slash_scale,
    MP_QSTR_chord,
    MP_QSTR_note_in_key,
    MP_QSTR_octave,
    MP_QSTR_note,
    MP_QSTR_midinote,
    MP_QSTR_AssertionError,
    MP_QSTR_frequency,
    MP_QSTR_valid,
    MP_QSTR_get,
    MP_QSTR_names,
    MP_QSTR_unknown,
    MP_QSTR___repr__,
    MP_QSTR__percent_s,
    MP_QSTR_freq,
    MP_QSTR_chord_string,
    MP_QSTR_annotation,
    MP_QSTR_annotation_name,
    MP_QSTR_none,
    MP_QSTR_root_note,
    MP_QSTR_parse_chord,
    MP_QSTR_N,
    MP_QSTR__amp_pause,
    MP_QSTR__star_,
    MP_QSTR_find,
    MP_QSTR__colon_,
    MP_QSTR_annotations_map,
    MP_QSTR_annotations,
    MP_QSTR_frequencies,
    MP_QSTR_midinotes,
    MP_QSTR_object,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_list,
    MP_QSTR_n,
    MP_QSTR_basenames,
    MP_QSTR_root,
    MP_QSTR_minor_second,
    MP_QSTR_major_second,
    MP_QSTR_minor_third,
    MP_QSTR_major_third,
    MP_QSTR_perfect_fourth,
    MP_QSTR_diminished_fifth,
    MP_QSTR_augmented_fourth,
    MP_QSTR_perfect_fifth,
    MP_QSTR_minor_sixth,
    MP_QSTR_major_sixth,
    MP_QSTR_minor_seventh,
    MP_QSTR_major_seventh,
    MP_QSTR_perfect_octave,
    MP_QSTR_minor_ninth,
    MP_QSTR_major_ninth,
    MP_QSTR_perfect_fifth_octave,
    MP_QSTR_major_thirteenth,
    MP_QSTR_self,
    MP_QSTR_ValueError,
    MP_QSTR_print,
    MP_QSTR_isinstance,
    MP_QSTR_int,
    MP_QSTR_len,
    MP_QSTR_str,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_0 = {{&mp_type_float}, (mp_float_t)16.35};
#define const_obj_music_0_macro MP_ROM_PTR(&const_obj_music_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_0_macro ((mp_rom_obj_t)(0xc202ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_0_macro ((mp_rom_obj_t)(0xc03459999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_1 = {{&mp_type_float}, (mp_float_t)17.32};
#define const_obj_music_1_macro MP_ROM_PTR(&const_obj_music_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_1_macro ((mp_rom_obj_t)(0xc20a8f5e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_1_macro ((mp_rom_obj_t)(0xc03551eb851eb852))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_2 = {{&mp_type_float}, (mp_float_t)18.35};
#define const_obj_music_2_macro MP_ROM_PTR(&const_obj_music_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_2_macro ((mp_rom_obj_t)(0xc212ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_2_macro ((mp_rom_obj_t)(0xc03659999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_3 = {{&mp_type_float}, (mp_float_t)19.45};
#define const_obj_music_3_macro MP_ROM_PTR(&const_obj_music_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_3_macro ((mp_rom_obj_t)(0xc21b999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_3_macro ((mp_rom_obj_t)(0xc037733333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_4 = {{&mp_type_float}, (mp_float_t)20.6};
#define const_obj_music_4_macro MP_ROM_PTR(&const_obj_music_4)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_4_macro ((mp_rom_obj_t)(0xc224ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_4_macro ((mp_rom_obj_t)(0xc03899999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_5 = {{&mp_type_float}, (mp_float_t)21.83};
#define const_obj_music_5_macro MP_ROM_PTR(&const_obj_music_5)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_5_macro ((mp_rom_obj_t)(0xc22ea3d6))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_5_macro ((mp_rom_obj_t)(0xc039d47ae147ae14))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_6 = {{&mp_type_float}, (mp_float_t)23.12};
#define const_obj_music_6_macro MP_ROM_PTR(&const_obj_music_6)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_6_macro ((mp_rom_obj_t)(0xc238f5c2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_6_macro ((mp_rom_obj_t)(0xc03b1eb851eb851f))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_7 = {{&mp_type_float}, (mp_float_t)24.5};
#define const_obj_music_7_macro MP_ROM_PTR(&const_obj_music_7)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_7_macro ((mp_rom_obj_t)(0xc2440002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_7_macro ((mp_rom_obj_t)(0xc03c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_8 = {{&mp_type_float}, (mp_float_t)25.96};
#define const_obj_music_8_macro MP_ROM_PTR(&const_obj_music_8)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_8_macro ((mp_rom_obj_t)(0xc24fae16))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_8_macro ((mp_rom_obj_t)(0xc03df5c28f5c28f6))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_9 = {{&mp_type_float}, (mp_float_t)27.5};
#define const_obj_music_9_macro MP_ROM_PTR(&const_obj_music_9)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_9_macro ((mp_rom_obj_t)(0xc25c0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_9_macro ((mp_rom_obj_t)(0xc03f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_10 = {{&mp_type_float}, (mp_float_t)29.14};
#define const_obj_music_10_macro MP_ROM_PTR(&const_obj_music_10)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_10_macro ((mp_rom_obj_t)(0xc2691eba))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_10_macro ((mp_rom_obj_t)(0xc04123d70a3d70a4))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_11 = {{&mp_type_float}, (mp_float_t)30.87};
#define const_obj_music_11_macro MP_ROM_PTR(&const_obj_music_11)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_11_macro ((mp_rom_obj_t)(0xc276f5c2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_11_macro ((mp_rom_obj_t)(0xc042deb851eb851f))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_12 = {{&mp_type_float}, (mp_float_t)32.7};
#define const_obj_music_12_macro MP_ROM_PTR(&const_obj_music_12)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_12_macro ((mp_rom_obj_t)(0xc282ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_12_macro ((mp_rom_obj_t)(0xc04459999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_13 = {{&mp_type_float}, (mp_float_t)34.65};
#define const_obj_music_13_macro MP_ROM_PTR(&const_obj_music_13)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_13_macro ((mp_rom_obj_t)(0xc28a999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_13_macro ((mp_rom_obj_t)(0xc045533333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_14 = {{&mp_type_float}, (mp_float_t)36.71};
#define const_obj_music_14_macro MP_ROM_PTR(&const_obj_music_14)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_14_macro ((mp_rom_obj_t)(0xc292d70a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_14_macro ((mp_rom_obj_t)(0xc0465ae147ae147b))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_15 = {{&mp_type_float}, (mp_float_t)38.89};
#define const_obj_music_15_macro MP_ROM_PTR(&const_obj_music_15)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_15_macro ((mp_rom_obj_t)(0xc29b8f5e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_15_macro ((mp_rom_obj_t)(0xc04771eb851eb852))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_16 = {{&mp_type_float}, (mp_float_t)41.2};
#define const_obj_music_16_macro MP_ROM_PTR(&const_obj_music_16)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_16_macro ((mp_rom_obj_t)(0xc2a4ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_16_macro ((mp_rom_obj_t)(0xc04899999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_17 = {{&mp_type_float}, (mp_float_t)43.65};
#define const_obj_music_17_macro MP_ROM_PTR(&const_obj_music_17)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_17_macro ((mp_rom_obj_t)(0xc2ae999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_17_macro ((mp_rom_obj_t)(0xc049d33333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_18 = {{&mp_type_float}, (mp_float_t)46.25};
#define const_obj_music_18_macro MP_ROM_PTR(&const_obj_music_18)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_18_macro ((mp_rom_obj_t)(0xc2b90002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_18_macro ((mp_rom_obj_t)(0xc04b200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_19 = {{&mp_type_float}, (mp_float_t)49};
#define const_obj_music_19_macro MP_ROM_PTR(&const_obj_music_19)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_19_macro ((mp_rom_obj_t)(0xc2c40002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_19_macro ((mp_rom_obj_t)(0xc04c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_20 = {{&mp_type_float}, (mp_float_t)51.91};
#define const_obj_music_20_macro MP_ROM_PTR(&const_obj_music_20)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_20_macro ((mp_rom_obj_t)(0xc2cfa3d6))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_20_macro ((mp_rom_obj_t)(0xc04df47ae147ae14))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_21 = {{&mp_type_float}, (mp_float_t)55};
#define const_obj_music_21_macro MP_ROM_PTR(&const_obj_music_21)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_21_macro ((mp_rom_obj_t)(0xc2dc0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_21_macro ((mp_rom_obj_t)(0xc04f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_22 = {{&mp_type_float}, (mp_float_t)58.27};
#define const_obj_music_22_macro MP_ROM_PTR(&const_obj_music_22)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_22_macro ((mp_rom_obj_t)(0xc2e9147a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_22_macro ((mp_rom_obj_t)(0xc051228f5c28f5c3))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_23 = {{&mp_type_float}, (mp_float_t)61.74};
#define const_obj_music_23_macro MP_ROM_PTR(&const_obj_music_23)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_23_macro ((mp_rom_obj_t)(0xc2f6f5c2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_23_macro ((mp_rom_obj_t)(0xc052deb851eb851f))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_24 = {{&mp_type_float}, (mp_float_t)65.41};
#define const_obj_music_24_macro MP_ROM_PTR(&const_obj_music_24)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_24_macro ((mp_rom_obj_t)(0xc302d1ee))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_24_macro ((mp_rom_obj_t)(0xc0545a3d70a3d70a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_25 = {{&mp_type_float}, (mp_float_t)69.3};
#define const_obj_music_25_macro MP_ROM_PTR(&const_obj_music_25)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_25_macro ((mp_rom_obj_t)(0xc30a999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_25_macro ((mp_rom_obj_t)(0xc055533333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_26 = {{&mp_type_float}, (mp_float_t)73.42};
#define const_obj_music_26_macro MP_ROM_PTR(&const_obj_music_26)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_26_macro ((mp_rom_obj_t)(0xc312d70a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_26_macro ((mp_rom_obj_t)(0xc0565ae147ae147b))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_27 = {{&mp_type_float}, (mp_float_t)77.78};
#define const_obj_music_27_macro MP_ROM_PTR(&const_obj_music_27)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_27_macro ((mp_rom_obj_t)(0xc31b8f5e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_27_macro ((mp_rom_obj_t)(0xc05771eb851eb852))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_28 = {{&mp_type_float}, (mp_float_t)82.41};
#define const_obj_music_28_macro MP_ROM_PTR(&const_obj_music_28)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_28_macro ((mp_rom_obj_t)(0xc324d1ee))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_28_macro ((mp_rom_obj_t)(0xc0589a3d70a3d70a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_29 = {{&mp_type_float}, (mp_float_t)87.31};
#define const_obj_music_29_macro MP_ROM_PTR(&const_obj_music_29)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_29_macro ((mp_rom_obj_t)(0xc32e9eba))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_29_macro ((mp_rom_obj_t)(0xc059d3d70a3d70a4))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_30 = {{&mp_type_float}, (mp_float_t)92.5};
#define const_obj_music_30_macro MP_ROM_PTR(&const_obj_music_30)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_30_macro ((mp_rom_obj_t)(0xc3390002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_30_macro ((mp_rom_obj_t)(0xc05b200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_31 = {{&mp_type_float}, (mp_float_t)98};
#define const_obj_music_31_macro MP_ROM_PTR(&const_obj_music_31)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_31_macro ((mp_rom_obj_t)(0xc3440002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_31_macro ((mp_rom_obj_t)(0xc05c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_32 = {{&mp_type_float}, (mp_float_t)103.8};
#define const_obj_music_32_macro MP_ROM_PTR(&const_obj_music_32)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_32_macro ((mp_rom_obj_t)(0xc34f999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_32_macro ((mp_rom_obj_t)(0xc05df33333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_33 = {{&mp_type_float}, (mp_float_t)110};
#define const_obj_music_33_macro MP_ROM_PTR(&const_obj_music_33)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_33_macro ((mp_rom_obj_t)(0xc35c0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_33_macro ((mp_rom_obj_t)(0xc05f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_34 = {{&mp_type_float}, (mp_float_t)116.5};
#define const_obj_music_34_macro MP_ROM_PTR(&const_obj_music_34)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_34_macro ((mp_rom_obj_t)(0xc3690002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_34_macro ((mp_rom_obj_t)(0xc061200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_35 = {{&mp_type_float}, (mp_float_t)123.5};
#define const_obj_music_35_macro MP_ROM_PTR(&const_obj_music_35)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_35_macro ((mp_rom_obj_t)(0xc3770002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_35_macro ((mp_rom_obj_t)(0xc062e00000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_36 = {{&mp_type_float}, (mp_float_t)130.8};
#define const_obj_music_36_macro MP_ROM_PTR(&const_obj_music_36)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_36_macro ((mp_rom_obj_t)(0xc382ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_36_macro ((mp_rom_obj_t)(0xc06459999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_37 = {{&mp_type_float}, (mp_float_t)138.6};
#define const_obj_music_37_macro MP_ROM_PTR(&const_obj_music_37)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_37_macro ((mp_rom_obj_t)(0xc38a999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_37_macro ((mp_rom_obj_t)(0xc065533333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_38 = {{&mp_type_float}, (mp_float_t)146.8};
#define const_obj_music_38_macro MP_ROM_PTR(&const_obj_music_38)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_38_macro ((mp_rom_obj_t)(0xc392ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_38_macro ((mp_rom_obj_t)(0xc06659999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_39 = {{&mp_type_float}, (mp_float_t)155.6};
#define const_obj_music_39_macro MP_ROM_PTR(&const_obj_music_39)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_39_macro ((mp_rom_obj_t)(0xc39b999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_39_macro ((mp_rom_obj_t)(0xc067733333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_40 = {{&mp_type_float}, (mp_float_t)164.8};
#define const_obj_music_40_macro MP_ROM_PTR(&const_obj_music_40)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_40_macro ((mp_rom_obj_t)(0xc3a4ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_40_macro ((mp_rom_obj_t)(0xc06899999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_41 = {{&mp_type_float}, (mp_float_t)174.6};
#define const_obj_music_41_macro MP_ROM_PTR(&const_obj_music_41)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_41_macro ((mp_rom_obj_t)(0xc3ae999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_41_macro ((mp_rom_obj_t)(0xc069d33333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_42 = {{&mp_type_float}, (mp_float_t)185};
#define const_obj_music_42_macro MP_ROM_PTR(&const_obj_music_42)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_42_macro ((mp_rom_obj_t)(0xc3b90002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_42_macro ((mp_rom_obj_t)(0xc06b200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_43 = {{&mp_type_float}, (mp_float_t)196};
#define const_obj_music_43_macro MP_ROM_PTR(&const_obj_music_43)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_43_macro ((mp_rom_obj_t)(0xc3c40002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_43_macro ((mp_rom_obj_t)(0xc06c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_44 = {{&mp_type_float}, (mp_float_t)207.7};
#define const_obj_music_44_macro MP_ROM_PTR(&const_obj_music_44)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_44_macro ((mp_rom_obj_t)(0xc3cfb332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_44_macro ((mp_rom_obj_t)(0xc06df66666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_45 = {{&mp_type_float}, (mp_float_t)220};
#define const_obj_music_45_macro MP_ROM_PTR(&const_obj_music_45)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_45_macro ((mp_rom_obj_t)(0xc3dc0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_45_macro ((mp_rom_obj_t)(0xc06f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_46 = {{&mp_type_float}, (mp_float_t)233.1};
#define const_obj_music_46_macro MP_ROM_PTR(&const_obj_music_46)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_46_macro ((mp_rom_obj_t)(0xc3e9199a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_46_macro ((mp_rom_obj_t)(0xc071233333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_47 = {{&mp_type_float}, (mp_float_t)246.9};
#define const_obj_music_47_macro MP_ROM_PTR(&const_obj_music_47)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_47_macro ((mp_rom_obj_t)(0xc3f6e666))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_47_macro ((mp_rom_obj_t)(0xc072dccccccccccd))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_48 = {{&mp_type_float}, (mp_float_t)261.6};
#define const_obj_music_48_macro MP_ROM_PTR(&const_obj_music_48)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_48_macro ((mp_rom_obj_t)(0xc402ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_48_macro ((mp_rom_obj_t)(0xc07459999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_49 = {{&mp_type_float}, (mp_float_t)277.2};
#define const_obj_music_49_macro MP_ROM_PTR(&const_obj_music_49)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_49_macro ((mp_rom_obj_t)(0xc40a999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_49_macro ((mp_rom_obj_t)(0xc075533333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_50 = {{&mp_type_float}, (mp_float_t)293.7};
#define const_obj_music_50_macro MP_ROM_PTR(&const_obj_music_50)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_50_macro ((mp_rom_obj_t)(0xc412d99a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_50_macro ((mp_rom_obj_t)(0xc0765b3333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_51 = {{&mp_type_float}, (mp_float_t)311.1};
#define const_obj_music_51_macro MP_ROM_PTR(&const_obj_music_51)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_51_macro ((mp_rom_obj_t)(0xc41b8cce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_51_macro ((mp_rom_obj_t)(0xc07771999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_52 = {{&mp_type_float}, (mp_float_t)329.6};
#define const_obj_music_52_macro MP_ROM_PTR(&const_obj_music_52)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_52_macro ((mp_rom_obj_t)(0xc424ccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_52_macro ((mp_rom_obj_t)(0xc07899999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_53 = {{&mp_type_float}, (mp_float_t)349.2};
#define const_obj_music_53_macro MP_ROM_PTR(&const_obj_music_53)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_53_macro ((mp_rom_obj_t)(0xc42e999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_53_macro ((mp_rom_obj_t)(0xc079d33333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_54 = {{&mp_type_float}, (mp_float_t)370};
#define const_obj_music_54_macro MP_ROM_PTR(&const_obj_music_54)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_54_macro ((mp_rom_obj_t)(0xc4390002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_54_macro ((mp_rom_obj_t)(0xc07b200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_55 = {{&mp_type_float}, (mp_float_t)392};
#define const_obj_music_55_macro MP_ROM_PTR(&const_obj_music_55)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_55_macro ((mp_rom_obj_t)(0xc4440002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_55_macro ((mp_rom_obj_t)(0xc07c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_56 = {{&mp_type_float}, (mp_float_t)415.3};
#define const_obj_music_56_macro MP_ROM_PTR(&const_obj_music_56)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_56_macro ((mp_rom_obj_t)(0xc44fa666))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_56_macro ((mp_rom_obj_t)(0xc07df4cccccccccd))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_57 = {{&mp_type_float}, (mp_float_t)440};
#define const_obj_music_57_macro MP_ROM_PTR(&const_obj_music_57)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_57_macro ((mp_rom_obj_t)(0xc45c0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_57_macro ((mp_rom_obj_t)(0xc07f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_58 = {{&mp_type_float}, (mp_float_t)466.2};
#define const_obj_music_58_macro MP_ROM_PTR(&const_obj_music_58)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_58_macro ((mp_rom_obj_t)(0xc469199a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_58_macro ((mp_rom_obj_t)(0xc081233333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_59 = {{&mp_type_float}, (mp_float_t)493.9};
#define const_obj_music_59_macro MP_ROM_PTR(&const_obj_music_59)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_59_macro ((mp_rom_obj_t)(0xc476f332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_59_macro ((mp_rom_obj_t)(0xc082de6666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_60 = {{&mp_type_float}, (mp_float_t)523.3};
#define const_obj_music_60_macro MP_ROM_PTR(&const_obj_music_60)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_60_macro ((mp_rom_obj_t)(0xc482d332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_60_macro ((mp_rom_obj_t)(0xc0845a6666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_61 = {{&mp_type_float}, (mp_float_t)554.4};
#define const_obj_music_61_macro MP_ROM_PTR(&const_obj_music_61)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_61_macro ((mp_rom_obj_t)(0xc48a999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_61_macro ((mp_rom_obj_t)(0xc085533333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_62 = {{&mp_type_float}, (mp_float_t)587.3};
#define const_obj_music_62_macro MP_ROM_PTR(&const_obj_music_62)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_62_macro ((mp_rom_obj_t)(0xc492d332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_62_macro ((mp_rom_obj_t)(0xc0865a6666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_63 = {{&mp_type_float}, (mp_float_t)622.3};
#define const_obj_music_63_macro MP_ROM_PTR(&const_obj_music_63)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_63_macro ((mp_rom_obj_t)(0xc49b9332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_63_macro ((mp_rom_obj_t)(0xc087726666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_64 = {{&mp_type_float}, (mp_float_t)659.3};
#define const_obj_music_64_macro MP_ROM_PTR(&const_obj_music_64)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_64_macro ((mp_rom_obj_t)(0xc4a4d332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_64_macro ((mp_rom_obj_t)(0xc0889a6666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_65 = {{&mp_type_float}, (mp_float_t)698.5};
#define const_obj_music_65_macro MP_ROM_PTR(&const_obj_music_65)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_65_macro ((mp_rom_obj_t)(0xc4aea002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_65_macro ((mp_rom_obj_t)(0xc089d40000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_66 = {{&mp_type_float}, (mp_float_t)740};
#define const_obj_music_66_macro MP_ROM_PTR(&const_obj_music_66)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_66_macro ((mp_rom_obj_t)(0xc4b90002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_66_macro ((mp_rom_obj_t)(0xc08b200000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_67 = {{&mp_type_float}, (mp_float_t)784};
#define const_obj_music_67_macro MP_ROM_PTR(&const_obj_music_67)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_67_macro ((mp_rom_obj_t)(0xc4c40002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_67_macro ((mp_rom_obj_t)(0xc08c800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_68 = {{&mp_type_float}, (mp_float_t)830.6};
#define const_obj_music_68_macro MP_ROM_PTR(&const_obj_music_68)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_68_macro ((mp_rom_obj_t)(0xc4cfa666))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_68_macro ((mp_rom_obj_t)(0xc08df4cccccccccd))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_69 = {{&mp_type_float}, (mp_float_t)880};
#define const_obj_music_69_macro MP_ROM_PTR(&const_obj_music_69)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_69_macro ((mp_rom_obj_t)(0xc4dc0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_69_macro ((mp_rom_obj_t)(0xc08f800000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_70 = {{&mp_type_float}, (mp_float_t)932.3};
#define const_obj_music_70_macro MP_ROM_PTR(&const_obj_music_70)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_70_macro ((mp_rom_obj_t)(0xc4e91332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_70_macro ((mp_rom_obj_t)(0xc091226666666666))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_music_71 = {{&mp_type_float}, (mp_float_t)987.8};
#define const_obj_music_71_macro MP_ROM_PTR(&const_obj_music_71)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_music_71_macro ((mp_rom_obj_t)(0xc4f6f332))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_music_71_macro ((mp_rom_obj_t)(0xc092de6666666666))
#endif
static const mp_obj_str_t const_obj_music_73 = {{&mp_type_str}, 40500, 35, (const byte*)"\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x70\x61\x72\x73\x65\x20\x70\x72\x6f\x67\x72\x65\x73\x73\x69\x6f\x6e\x20\x73\x74\x72\x69\x6e\x67\x20"};

// constant table
static const mp_rom_obj_t const_obj_table_data_music[75] = {
    const_obj_music_0_macro,
    const_obj_music_1_macro,
    const_obj_music_2_macro,
    const_obj_music_3_macro,
    const_obj_music_4_macro,
    const_obj_music_5_macro,
    const_obj_music_6_macro,
    const_obj_music_7_macro,
    const_obj_music_8_macro,
    const_obj_music_9_macro,
    const_obj_music_10_macro,
    const_obj_music_11_macro,
    const_obj_music_12_macro,
    const_obj_music_13_macro,
    const_obj_music_14_macro,
    const_obj_music_15_macro,
    const_obj_music_16_macro,
    const_obj_music_17_macro,
    const_obj_music_18_macro,
    const_obj_music_19_macro,
    const_obj_music_20_macro,
    const_obj_music_21_macro,
    const_obj_music_22_macro,
    const_obj_music_23_macro,
    const_obj_music_24_macro,
    const_obj_music_25_macro,
    const_obj_music_26_macro,
    const_obj_music_27_macro,
    const_obj_music_28_macro,
    const_obj_music_29_macro,
    const_obj_music_30_macro,
    const_obj_music_31_macro,
    const_obj_music_32_macro,
    const_obj_music_33_macro,
    const_obj_music_34_macro,
    const_obj_music_35_macro,
    const_obj_music_36_macro,
    const_obj_music_37_macro,
    const_obj_music_38_macro,
    const_obj_music_39_macro,
    const_obj_music_40_macro,
    const_obj_music_41_macro,
    const_obj_music_42_macro,
    const_obj_music_43_macro,
    const_obj_music_44_macro,
    const_obj_music_45_macro,
    const_obj_music_46_macro,
    const_obj_music_47_macro,
    const_obj_music_48_macro,
    const_obj_music_49_macro,
    const_obj_music_50_macro,
    const_obj_music_51_macro,
    const_obj_music_52_macro,
    const_obj_music_53_macro,
    const_obj_music_54_macro,
    const_obj_music_55_macro,
    const_obj_music_56_macro,
    const_obj_music_57_macro,
    const_obj_music_58_macro,
    const_obj_music_59_macro,
    const_obj_music_60_macro,
    const_obj_music_61_macro,
    const_obj_music_62_macro,
    const_obj_music_63_macro,
    const_obj_music_64_macro,
    const_obj_music_65_macro,
    const_obj_music_66_macro,
    const_obj_music_67_macro,
    const_obj_music_68_macro,
    const_obj_music_69_macro,
    const_obj_music_70_macro,
    const_obj_music_71_macro,
    MP_ROM_QSTR(MP_QSTR_sus4_paren_open_b7_comma_9_comma_13_paren_close_),
    MP_ROM_PTR(&const_obj_music_73),
    MP_ROM_QSTR(MP_QSTR__percent_s_space__hyphen__space__percent_s_space__percent_s_space__paren_open_from_space__percent_s_paren_close_),
};

static const mp_frozen_module_t frozen_module_music = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_music,
        .obj_table = (mp_obj_t *)&const_obj_table_data_music,
    },
    .rc = &raw_code_music__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module _boot
// - original source file: build-standard/tulip/obj/frozen_mpy/_boot.mpy
// - frozen file name: _boot.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file _boot.py, scope _boot__lt_module_gt_
static const byte fun_data__boot__lt_module_gt_[458] = {
    0xd4,0x02,0x52, // prelude
    0x01, // names: <module>
    0x26,0x26,0x2c,0x26,0x28,0x72,0x20,0x22,0x2c,0x22,0x24,0x57,0x2b,0x26,0x32,0x2c,0x2a,0x72,0x20,0x2d,0x22,0x70,0x23,0x63,0x20,0x2a,0x2a,0x27,0x2e,0x4d,0x2b,0x30,0x2a,0x37,0x30,0x2a,0x1f,0x61,0x23,0x53, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'gc'
    0x16,0x02, // STORE_NAME 'gc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'uos'
    0x16,0x03, // STORE_NAME 'uos'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'tulip'
    0x16,0x04, // STORE_NAME 'tulip'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'world'
    0x16,0x05, // STORE_NAME 'world'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'alles'
    0x16,0x06, // STORE_NAME 'alles'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x07, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x08, // IMPORT_NAME 'upysh'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x09, // LOAD_CONST_STRING 'edit'
    0x10,0x0a, // LOAD_CONST_STRING 'run'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x04, // IMPORT_NAME 'tulip'
    0x1c,0x09, // IMPORT_FROM 'edit'
    0x16,0x09, // STORE_NAME 'edit'
    0x1c,0x0a, // IMPORT_FROM 'run'
    0x16,0x0a, // STORE_NAME 'run'
    0x59, // POP_TOP
    0x48,0x6c, // SETUP_EXCEPT 108
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0b, // LOAD_CONST_STRING 'bdev'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x0c, // IMPORT_NAME 'flashbdev'
    0x1c,0x0b, // IMPORT_FROM 'bdev'
    0x16,0x0b, // STORE_NAME 'bdev'
    0x59, // POP_TOP
    0x48,0x11, // SETUP_EXCEPT 17
    0x11,0x0b, // LOAD_NAME 'bdev'
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x0d, // LOAD_METHOD 'mount'
    0x11,0x0b, // LOAD_NAME 'bdev'
    0x10,0x0e, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x4a,0x2d, // POP_EXCEPT_JUMP 45
    0x57, // DUP_TOP
    0x11,0x22, // LOAD_NAME 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x16,0x23, // STORE_NAME 'e'
    0x49,0x1a, // SETUP_FINALLY 26
    0x11,0x24, // LOAD_NAME 'print'
    0x11,0x25, // LOAD_NAME 'str'
    0x11,0x23, // LOAD_NAME 'e'
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0f, // IMPORT_NAME 'inisetup'
    0x16,0x0f, // STORE_NAME 'inisetup'
    0x11,0x0f, // LOAD_NAME 'inisetup'
    0x14,0x10, // LOAD_METHOD 'setup'
    0x36,0x00, // CALL_METHOD 0
    0x16,0x26, // STORE_NAME 'vfs'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x16,0x23, // STORE_NAME 'e'
    0x19,0x23, // DELETE_NAME 'e'
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x11, // LOAD_CONST_STRING 'Partition'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x12, // IMPORT_NAME 'esp32'
    0x1c,0x11, // IMPORT_FROM 'Partition'
    0x16,0x11, // STORE_NAME 'Partition'
    0x59, // POP_TOP
    0x11,0x11, // LOAD_NAME 'Partition'
    0x11,0x11, // LOAD_NAME 'Partition'
    0x13,0x13, // LOAD_ATTR 'RUNNING'
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x27, // STORE_NAME 'currentPartition'
    0x11,0x27, // LOAD_NAME 'currentPartition'
    0x14,0x14, // LOAD_METHOD 'mark_app_valid_cancel_rollback'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x4a,0xef,0x01, // POP_EXCEPT_JUMP 239
    0x57, // DUP_TOP
    0x11,0x28, // LOAD_NAME 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0xe7,0x81, // POP_JUMP_IF_FALSE 231
    0x59, // POP_TOP
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x15, // LOAD_METHOD 'getenv'
    0x10,0x16, // LOAD_CONST_STRING 'HOME'
    0x36,0x01, // CALL_METHOD 1
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xf2, // BINARY_OP 27 __add__
    0x16,0x29, // STORE_NAME 'tulipcc'
    0x48,0x09, // SETUP_EXCEPT 9
    0x11,0x2a, // LOAD_NAME 'mkdir'
    0x11,0x29, // LOAD_NAME 'tulipcc'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x11,0x22, // LOAD_NAME 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0xa5,0x01, // SETUP_EXCEPT 165
    0x11,0x2a, // LOAD_NAME 'mkdir'
    0x11,0x29, // LOAD_NAME 'tulipcc'
    0x10,0x17, // LOAD_CONST_STRING '/ex'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x11,0x2a, // LOAD_NAME 'mkdir'
    0x11,0x29, // LOAD_NAME 'tulipcc'
    0x10,0x18, // LOAD_CONST_STRING '/ex/g'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x11,0x24, // LOAD_NAME 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x11,0x04, // LOAD_NAME 'tulip'
    0x14,0x19, // LOAD_METHOD 'app_path'
    0x36,0x00, // CALL_METHOD 0
    0x14,0x1a, // LOAD_METHOD 'endswith'
    0x10,0x1b, // LOAD_CONST_STRING 'unix'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x11,0x04, // LOAD_NAME 'tulip'
    0x14,0x19, // LOAD_METHOD 'app_path'
    0x36,0x00, // CALL_METHOD 0
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf2, // BINARY_OP 27 __add__
    0x16,0x2b, // STORE_NAME 'tulip_home'
    0x42,0x4b, // JUMP 11
    0x11,0x04, // LOAD_NAME 'tulip'
    0x14,0x19, // LOAD_METHOD 'app_path'
    0x36,0x00, // CALL_METHOD 0
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf2, // BINARY_OP 27 __add__
    0x16,0x2b, // STORE_NAME 'tulip_home'
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x1c, // LOAD_METHOD 'listdir'
    0x11,0x2b, // LOAD_NAME 'tulip_home'
    0x10,0x17, // LOAD_CONST_STRING '/ex'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x23, // FOR_ITER 35
    0x16,0x2c, // STORE_NAME 'file'
    0x11,0x2c, // LOAD_NAME 'file'
    0x14,0x1a, // LOAD_METHOD 'endswith'
    0x10,0x1d, // LOAD_CONST_STRING '.py'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0x11,0x2d, // LOAD_NAME 'cp'
    0x11,0x2b, // LOAD_NAME 'tulip_home'
    0x10,0x1e, // LOAD_CONST_STRING '/ex/'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x2c, // LOAD_NAME 'file'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x29, // LOAD_NAME 'tulipcc'
    0x10,0x1e, // LOAD_CONST_STRING '/ex/'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x2c, // LOAD_NAME 'file'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x1c, // LOAD_METHOD 'listdir'
    0x11,0x2b, // LOAD_NAME 'tulip_home'
    0x10,0x18, // LOAD_CONST_STRING '/ex/g'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x23, // FOR_ITER 35
    0x16,0x2c, // STORE_NAME 'file'
    0x11,0x2c, // LOAD_NAME 'file'
    0x14,0x1a, // LOAD_METHOD 'endswith'
    0x10,0x1f, // LOAD_CONST_STRING '.png'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0x11,0x2d, // LOAD_NAME 'cp'
    0x11,0x2b, // LOAD_NAME 'tulip_home'
    0x10,0x20, // LOAD_CONST_STRING '/ex/g/'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x2c, // LOAD_NAME 'file'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x29, // LOAD_NAME 'tulipcc'
    0x10,0x20, // LOAD_CONST_STRING '/ex/g/'
    0xf2, // BINARY_OP 27 __add__
    0x11,0x2c, // LOAD_NAME 'file'
    0xf2, // BINARY_OP 27 __add__
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x11,0x22, // LOAD_NAME 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x11,0x2e, // LOAD_NAME 'cd'
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x15, // LOAD_METHOD 'getenv'
    0x10,0x16, // LOAD_CONST_STRING 'HOME'
    0x36,0x01, // CALL_METHOD 1
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x11,0x02, // LOAD_NAME 'gc'
    0x14,0x21, // LOAD_METHOD 'collect'
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
    .fun_data_len = 458,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 3,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data__boot__lt_module_gt_ + 4,
        .line_info_top = fun_data__boot__lt_module_gt_ + 44,
        .opcodes = fun_data__boot__lt_module_gt_ + 44,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data__boot[47] = {
    MP_QSTR__boot_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_gc,
    MP_QSTR_uos,
    MP_QSTR_tulip,
    MP_QSTR_world,
    MP_QSTR_alles,
    MP_QSTR__star_,
    MP_QSTR_upysh,
    MP_QSTR_edit,
    MP_QSTR_run,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_inisetup,
    MP_QSTR_setup,
    MP_QSTR_Partition,
    MP_QSTR_esp32,
    MP_QSTR_RUNNING,
    MP_QSTR_mark_app_valid_cancel_rollback,
    MP_QSTR_getenv,
    MP_QSTR_HOME,
    MP_QSTR__slash_ex,
    MP_QSTR__slash_ex_slash_g,
    MP_QSTR_app_path,
    MP_QSTR_endswith,
    MP_QSTR_unix,
    MP_QSTR_listdir,
    MP_QSTR__dot_py,
    MP_QSTR__slash_ex_slash_,
    MP_QSTR__dot_png,
    MP_QSTR__slash_ex_slash_g_slash_,
    MP_QSTR_collect,
    MP_QSTR_OSError,
    MP_QSTR_e,
    MP_QSTR_print,
    MP_QSTR_str,
    MP_QSTR_vfs,
    MP_QSTR_currentPartition,
    MP_QSTR_ImportError,
    MP_QSTR_tulipcc,
    MP_QSTR_mkdir,
    MP_QSTR_tulip_home,
    MP_QSTR_file,
    MP_QSTR_cp,
    MP_QSTR_cd,
};

// constants
static const mp_obj_str_t const_obj__boot_1 = {{&mp_type_str}, 13874, 41, (const byte*)"\x4c\x6f\x61\x64\x69\x6e\x67\x20\x6e\x65\x77\x20\x65\x78\x61\x6d\x70\x6c\x65\x73\x20\x69\x6e\x74\x6f\x20\x65\x78\x2f\x20\x66\x6f\x6c\x64\x65\x72\x20\x2e\x2e\x2e\x20"};
static const mp_obj_str_t const_obj__boot_3 = {{&mp_type_str}, 45933, 30, (const byte*)"\x2f\x43\x6f\x6e\x74\x65\x6e\x74\x73\x2f\x52\x65\x73\x6f\x75\x72\x63\x65\x73\x2f\x74\x75\x6c\x69\x70\x5f\x68\x6f\x6d\x65"};

// constant table
static const mp_rom_obj_t const_obj_table_data__boot[4] = {
    MP_ROM_QSTR(MP_QSTR__slash_Documents_slash_tulipcc),
    MP_ROM_PTR(&const_obj__boot_1),
    MP_ROM_QSTR(MP_QSTR__slash__dot__dot__slash__dot__dot__slash_tulip_home),
    MP_ROM_PTR(&const_obj__boot_3),
};

static const mp_frozen_module_t frozen_module__boot = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data__boot,
        .obj_table = (mp_obj_t *)&const_obj_table_data__boot,
    },
    .rc = &raw_code__boot__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module alles
// - original source file: build-standard/tulip/obj/frozen_mpy/alles.mpy
// - frozen file name: alles.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file alles.py, scope alles__lt_module_gt_
static const byte fun_data_alles__lt_module_gt_[407] = {
    0xc0,0x20,0x7c, // prelude
    0x01, // names: <module>
    0x58,0x25,0x24,0x23,0x25,0x3f,0x3e,0x33,0x24,0x45,0x60,0x88,0x2d,0x84,0x09,0x64,0x20,0x1f,0x74,0x80,0x30,0x68,0x40,0x60,0x20,0x68,0x60,0x68,0x68,0x60,0x84,0x08,0x60,0x8c,0x0b,0x60,0x89,0x0e,0x84,0x0b,0x60,0x8c,0x0b,0x60,0x8b,0x1e,0x60,0x6a,0x40,0x60,0x8c,0x12,0x60,0x8b,0x0e,0x88,0x0b,0x84,0x0a,0x25,0x64, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'struct'
    0x16,0x02, // STORE_NAME 'struct'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'time'
    0x16,0x04, // STORE_NAME 'time'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'tulip'
    0x16,0x05, // STORE_NAME 'tulip'
    0x22,0x82,0x00, // LOAD_CONST_SMALL_INT 256
    0x16,0x6b, // STORE_NAME 'BLOCK_SIZE'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x6c, // STORE_NAME 'SAMPLE_RATE'
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x16,0x6d, // STORE_NAME 'OSCS'
    0x22,0x83,0x10, // LOAD_CONST_SMALL_INT 400
    0x16,0x6e, // STORE_NAME 'MAX_QUEUE'
    0x11,0x6f, // LOAD_NAME 'range'
    0x8c, // LOAD_CONST_SMALL_INT 12
    0x34,0x01, // CALL_FUNCTION 1
    0x30,0x0c, // UNPACK_SEQUENCE 12
    0x16,0x70, // STORE_NAME 'SINE'
    0x16,0x71, // STORE_NAME 'PULSE'
    0x16,0x72, // STORE_NAME 'SAW_DOWN'
    0x16,0x73, // STORE_NAME 'SAW_UP'
    0x16,0x74, // STORE_NAME 'TRIANGLE'
    0x16,0x75, // STORE_NAME 'NOISE'
    0x16,0x76, // STORE_NAME 'KS'
    0x16,0x77, // STORE_NAME 'PCM'
    0x16,0x78, // STORE_NAME 'ALGO'
    0x16,0x79, // STORE_NAME 'PARTIAL'
    0x16,0x7a, // STORE_NAME 'PARTIALS'
    0x16,0x7b, // STORE_NAME 'OFF'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x30,0x0a, // UNPACK_SEQUENCE 10
    0x16,0x7c, // STORE_NAME 'TARGET_AMP'
    0x16,0x7d, // STORE_NAME 'TARGET_DUTY'
    0x16,0x7e, // STORE_NAME 'TARGET_FREQ'
    0x16,0x7f, // STORE_NAME 'TARGET_FILTER_FREQ'
    0x16,0x81,0x00, // STORE_NAME 'TARGET_RESONANCE'
    0x16,0x81,0x01, // STORE_NAME 'TARGET_FEEDBACK'
    0x16,0x81,0x02, // STORE_NAME 'TARGET_LINEAR'
    0x16,0x81,0x03, // STORE_NAME 'TARGET_TRUE_EXPONENTIAL'
    0x16,0x81,0x04, // STORE_NAME 'TARGET_DX7_EXPONENTIAL'
    0x16,0x81,0x05, // STORE_NAME 'TARGET_PAN'
    0x11,0x6f, // LOAD_NAME 'range'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x34,0x01, // CALL_FUNCTION 1
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x16,0x81,0x06, // STORE_NAME 'FILTER_NONE'
    0x16,0x81,0x07, // STORE_NAME 'FILTER_LPF'
    0x16,0x81,0x08, // STORE_NAME 'FILTER_BPF'
    0x16,0x81,0x09, // STORE_NAME 'FILTER_HPF'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x81,0x0a, // STORE_GLOBAL 'mesh_flag'
    0x22,0x3f, // LOAD_CONST_SMALL_INT 63
    0x16,0x81,0x0b, // STORE_NAME 'CHORUS_OSC'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x07, // STORE_NAME 'preset'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x18, // STORE_NAME 'millis'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1a, // STORE_NAME 'trunc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x10,0x06, // LOAD_CONST_STRING ''
    0x10,0x06, // LOAD_CONST_STRING ''
    0x10,0x06, // LOAD_CONST_STRING ''
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x29, // BUILD_TUPLE 41
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x1f, // STORE_NAME 'message'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x49, // STORE_NAME 'send'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x4b, // STORE_NAME 'reset'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x06, // MAKE_FUNCTION_DEFARGS 6
    0x16,0x4c, // STORE_NAME 'volume'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x4e, // STORE_NAME 'latency_ms'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x4f, // STORE_NAME 'test'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x09, // MAKE_FUNCTION_DEFARGS 9
    0x16,0x51, // STORE_NAME 'play_patches'
    0x11,0x6d, // LOAD_NAME 'OSCS'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x0a, // MAKE_FUNCTION_DEFARGS 10
    0x16,0x52, // STORE_NAME 'polyphony'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x54, // STORE_NAME 'eq_test'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x0c, // MAKE_FUNCTION_DEFARGS 12
    0x16,0x59, // STORE_NAME 'sweep'
    0x22,0x80,0x78, // LOAD_CONST_SMALL_INT 120
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0d, // MAKE_FUNCTION_DEFARGS 13
    0x16,0x5b, // STORE_NAME 'drums'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x11,0x70, // LOAD_NAME 'SINE'
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0e, // MAKE_FUNCTION_DEFARGS 14
    0x16,0x5d, // STORE_NAME 'c_major'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x05, // BUILD_TUPLE 5
    0x53, // LOAD_NULL
    0x33,0x0f, // MAKE_FUNCTION_DEFARGS 15
    0x16,0x5e, // STORE_NAME 'chorus'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x10, // MAKE_FUNCTION_DEFARGS 16
    0x16,0x62, // STORE_NAME 'reverb'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x11, // MAKE_FUNCTION_DEFARGS 17
    0x16,0x67, // STORE_NAME 'mesh'
    0x32,0x12, // MAKE_FUNCTION 18
    0x16,0x6a, // STORE_NAME 'local'
    0x10,0x06, // LOAD_CONST_STRING ''
    0x16,0x81,0x0c, // STORE_NAME 'send_buffer'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x81,0x0d, // STORE_NAME 'buffer_size'
    0x11,0x6a, // LOAD_NAME 'local'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_preset
static const byte fun_data_alles_preset[687] = {
    0xa2,0x91,0x40,0x5e, // prelude
    0x07,0x81,0x0e,0x08, // names: preset, which, osc
    0x80,0x10,0x40,0x29,0x25,0x38,0x25,0x1f,0x69,0x25,0x58,0x26,0x2b,0x3a,0x1f,0x22,0x26,0x2b,0x3a,0x1f,0x22,0x46,0x2b,0x3e,0x1f,0x25,0x25,0x3b,0x25,0x3b,0x25,0x39,0x25,0x39,0x25,0x3b,0x25,0x39,0x25,0x36,0x25,0x39,0x25, // code info
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x68, // POP_JUMP_IF_FALSE 40
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0x22,0x93,0x44, // LOAD_CONST_SMALL_INT 2500
    0x10,0x0d, // LOAD_CONST_STRING 'resonance'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x72, // LOAD_GLOBAL 'SAW_DOWN'
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x07, // LOAD_GLOBAL 'FILTER_LPF'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x12,0x7f, // LOAD_GLOBAL 'TARGET_FILTER_FREQ'
    0xf2, // BINARY_OP 27 __add__
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x90,0x00, // CALL_FUNCTION_VAR_KW 2048
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xc6,0x80, // POP_JUMP_IF_FALSE 70
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x10,0x11, // LOAD_CONST_STRING 'mod_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x10,0x12, // LOAD_CONST_STRING 'mod_source'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8e,0x00, // CALL_FUNCTION_VAR_KW 1792
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xc6,0x80, // POP_JUMP_IF_FALSE 70
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x10,0x11, // LOAD_CONST_STRING 'mod_target'
    0x12,0x7e, // LOAD_GLOBAL 'TARGET_FREQ'
    0x10,0x12, // LOAD_CONST_STRING 'mod_source'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8e,0x00, // CALL_FUNCTION_VAR_KW 1792
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x85, // LOAD_CONST_SMALL_INT 5
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xcd,0x80, // POP_JUMP_IF_FALSE 77
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x10,0x13, // LOAD_CONST_STRING 'phase'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x10,0x14, // LOAD_CONST_STRING '500,0,0,0'
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x10,0x11, // LOAD_CONST_STRING 'mod_target'
    0x12,0x7e, // LOAD_GLOBAL 'TARGET_FREQ'
    0x10,0x12, // LOAD_CONST_STRING 'mod_source'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x90,0x00, // CALL_FUNCTION_VAR_KW 2048
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x86, // LOAD_CONST_SMALL_INT 6
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x75, // LOAD_GLOBAL 'NOISE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x10,0x15, // LOAD_CONST_STRING '250,0,0,0'
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x87, // LOAD_CONST_SMALL_INT 7
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x75, // LOAD_GLOBAL 'NOISE'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7c, // LOAD_GLOBAL 'TARGET_AMP'
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x88, // LOAD_CONST_SMALL_INT 8
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x77, // LOAD_GLOBAL 'PCM'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x89, // LOAD_CONST_SMALL_INT 9
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x77, // LOAD_GLOBAL 'PCM'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x8a, // LOAD_CONST_SMALL_INT 10
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x77, // LOAD_GLOBAL 'PCM'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0x22,0x80,0x46, // LOAD_CONST_SMALL_INT 70
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x8b, // LOAD_CONST_SMALL_INT 11
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x77, // LOAD_GLOBAL 'PCM'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x78, // LOAD_GLOBAL 'ALGO'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x95, // LOAD_CONST_SMALL_INT 21
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x8d, // LOAD_CONST_SMALL_INT 13
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x77, // LOAD_GLOBAL 'PCM'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x8e, // LOAD_CONST_SMALL_INT 14
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x67, // POP_JUMP_IF_FALSE 39
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x78, // LOAD_GLOBAL 'ALGO'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0x22,0x3e, // LOAD_CONST_SMALL_INT 62
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0x22,0x8f,0x50, // LOAD_CONST_SMALL_INT 2000
    0x10,0x0d, // LOAD_CONST_STRING 'resonance'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x07, // LOAD_GLOBAL 'FILTER_LPF'
    0x10,0x0b, // LOAD_CONST_STRING 'bp0_target'
    0x12,0x7f, // LOAD_GLOBAL 'TARGET_FILTER_FREQ'
    0x10,0x0a, // LOAD_CONST_STRING 'bp0'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x34,0x90,0x00, // CALL_FUNCTION 2048
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_preset = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_preset,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 687,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 21,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_alles_preset + 8,
        .line_info_top = fun_data_alles_preset + 51,
        .opcodes = fun_data_alles_preset + 51,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_millis
static const byte fun_data_alles_millis[12] = {
    0x08,0x06, // prelude
    0x18, // names: millis
    0x80,0x40, // code info
    0x12,0x05, // LOAD_GLOBAL 'tulip'
    0x14,0x19, // LOAD_METHOD 'ticks_ms'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_millis = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_alles_millis,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 12,
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
        .qstr_block_name_idx = 24,
        .line_info = fun_data_alles_millis + 3,
        .line_info_top = fun_data_alles_millis + 5,
        .opcodes = fun_data_alles_millis + 5,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_trunc
static const byte fun_data_alles_trunc[24] = {
    0x19,0x0a, // prelude
    0x1a,0x81,0x0f, // names: trunc, number
    0x80,0x46, // code info
    0x10,0x1b, // LOAD_CONST_STRING '%.6f'
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x14,0x1c, // LOAD_METHOD 'rstrip'
    0x10,0x1d, // LOAD_CONST_STRING '0'
    0x36,0x01, // CALL_METHOD 1
    0x14,0x1c, // LOAD_METHOD 'rstrip'
    0x10,0x1e, // LOAD_CONST_STRING '.'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_trunc = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_alles_trunc,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
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
        .qstr_block_name_idx = 26,
        .line_info = fun_data_alles_trunc + 5,
        .line_info_top = fun_data_alles_trunc + 7,
        .opcodes = fun_data_alles_trunc + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_message
static const byte fun_data_alles_message[907] = {
    0xe1,0xa1,0x84,0x80,0x85,0x80,0x01,0xc2,0x02, // prelude
    0x1f,0x08,0x09,0x16,0x17,0x0f,0x5f,0x10,0x5a,0x81,0x10,0x81,0x11,0x4b,0x13,0x5c,0x4d,0x81,0x12,0x4c,0x0c,0x0d,0x0a,0x81,0x13,0x81,0x14,0x0b,0x81,0x15,0x81,0x16,0x11,0x81,0x17,0x12,0x55,0x56,0x57,0x0e,0x81,0x18,0x81,0x19,0x4e,0x81,0x1a,0x60,0x61,0x63,0x64,0x65,0x66, // names: message, osc, wave, patch, note, vel, amp, freq, duty, feedback, timestamp, reset, phase, pan, client, retries, volume, filter_freq, resonance, bp0, bp1, bp2, bp0_target, bp1_target, bp2_target, mod_target, debug, mod_source, eq_l, eq_m, eq_h, filter_type, algorithm, ratio, latency_ms, algo_source, chorus_level, chorus_delay, reverb_level, reverb_liveness, reverb_damping, reverb_xover
    0x80,0x4e,0x24,0x2a,0x2e,0x33,0x33,0x32,0x32,0x32,0x33,0x33,0x32,0x32,0x33,0x32,0x32,0x32,0x35,0x34,0x34,0x34,0x35,0x33,0x33,0x33,0x31,0x35,0x35,0x35,0x35,0x35,0x33,0x35,0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,0x55, // code info
    0x10,0x06, // LOAD_CONST_STRING ''
    0x26,0x29, // STORE_FAST_N 41
    0xb9, // LOAD_FAST 9
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x12,0x18, // LOAD_GLOBAL 'millis'
    0x34,0x00, // CALL_FUNCTION 0
    0xc9, // STORE_FAST 9
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x20, // LOAD_CONST_STRING 't'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb9, // LOAD_FAST 9
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x21, // LOAD_CONST_STRING 'v'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x22, // LOAD_CONST_STRING 'w'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb7, // LOAD_FAST 7
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x23, // LOAD_CONST_STRING 'd'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb8, // LOAD_FAST 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x24, // LOAD_CONST_STRING 'b'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xb8, // LOAD_FAST 8
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x25, // LOAD_CONST_STRING 'f'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xb6, // LOAD_FAST 6
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x26, // LOAD_CONST_STRING 'n'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x27, // LOAD_CONST_STRING 'p'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xbb, // LOAD_FAST 11
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x28, // LOAD_CONST_STRING 'P'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xbc, // LOAD_FAST 12
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x29, // LOAD_CONST_STRING 'Q'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xbc, // LOAD_FAST 12
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xbd, // LOAD_FAST 13
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2a, // LOAD_CONST_STRING 'c'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xbd, // LOAD_FAST 13
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2b, // LOAD_CONST_STRING 'a'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2c, // LOAD_CONST_STRING 'l'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xbf, // LOAD_FAST 15
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2d, // LOAD_CONST_STRING 'V'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0xbf, // LOAD_FAST 15
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x21, // LOAD_FAST_N 33
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2e, // LOAD_CONST_STRING 'N'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x21, // LOAD_FAST_N 33
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x11, // LOAD_FAST_N 17
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x2f, // LOAD_CONST_STRING 'R'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x11, // LOAD_FAST_N 17
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x10, // LOAD_FAST_N 16
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x30, // LOAD_CONST_STRING 'F'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x10, // LOAD_FAST_N 16
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x20, // LOAD_FAST_N 32
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x31, // LOAD_CONST_STRING 'I'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x20, // LOAD_FAST_N 32
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1f, // LOAD_FAST_N 31
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x32, // LOAD_CONST_STRING 'o'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x1f, // LOAD_FAST_N 31
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x12,0x81,0x1c, // LOAD_GLOBAL 'len'
    0x24,0x12, // LOAD_FAST_N 18
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x33, // LOAD_CONST_STRING 'A%s'
    0x24,0x12, // LOAD_FAST_N 18
    0xf8, // BINARY_OP 33 __mod__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x12,0x81,0x1c, // LOAD_GLOBAL 'len'
    0x24,0x13, // LOAD_FAST_N 19
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x34, // LOAD_CONST_STRING 'B%s'
    0x24,0x13, // LOAD_FAST_N 19
    0xf8, // BINARY_OP 33 __mod__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x12,0x81,0x1c, // LOAD_GLOBAL 'len'
    0x24,0x14, // LOAD_FAST_N 20
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x35, // LOAD_CONST_STRING 'C%s'
    0x24,0x14, // LOAD_FAST_N 20
    0xf8, // BINARY_OP 33 __mod__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x22, // LOAD_FAST_N 34
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x36, // LOAD_CONST_STRING 'O%s'
    0x24,0x22, // LOAD_FAST_N 34
    0xf8, // BINARY_OP 33 __mod__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x15, // LOAD_FAST_N 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x37, // LOAD_CONST_STRING 'T'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x15, // LOAD_FAST_N 21
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x16, // LOAD_FAST_N 22
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x38, // LOAD_CONST_STRING 'W'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x16, // LOAD_FAST_N 22
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x17, // LOAD_FAST_N 23
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x39, // LOAD_CONST_STRING 'X'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x17, // LOAD_FAST_N 23
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x18, // LOAD_FAST_N 24
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3a, // LOAD_CONST_STRING 'g'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x18, // LOAD_FAST_N 24
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1a, // LOAD_FAST_N 26
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3b, // LOAD_CONST_STRING 'L'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x1a, // LOAD_FAST_N 26
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3c, // LOAD_CONST_STRING 'S'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x19, // LOAD_FAST_N 25
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3d, // LOAD_CONST_STRING 'D'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x19, // LOAD_FAST_N 25
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1b, // LOAD_FAST_N 27
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3e, // LOAD_CONST_STRING 'x'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x1b, // LOAD_FAST_N 27
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1c, // LOAD_FAST_N 28
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x3f, // LOAD_CONST_STRING 'y'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x1c, // LOAD_FAST_N 28
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1d, // LOAD_FAST_N 29
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x40, // LOAD_CONST_STRING 'z'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x1a, // LOAD_GLOBAL 'trunc'
    0x24,0x1d, // LOAD_FAST_N 29
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x1e, // LOAD_FAST_N 30
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x41, // LOAD_CONST_STRING 'G'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x1e, // LOAD_FAST_N 30
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x23, // LOAD_FAST_N 35
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x42, // LOAD_CONST_STRING 'k'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x23, // LOAD_FAST_N 35
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x24, // LOAD_FAST_N 36
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x43, // LOAD_CONST_STRING 'm'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x24, // LOAD_FAST_N 36
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x25, // LOAD_FAST_N 37
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x44, // LOAD_CONST_STRING 'h'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x25, // LOAD_FAST_N 37
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x26, // LOAD_FAST_N 38
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x45, // LOAD_CONST_STRING 'H'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x26, // LOAD_FAST_N 38
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x27, // LOAD_FAST_N 39
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x46, // LOAD_CONST_STRING 'j'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x27, // LOAD_FAST_N 39
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x28, // LOAD_FAST_N 40
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x47, // LOAD_CONST_STRING 'J'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0x24,0x28, // LOAD_FAST_N 40
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x29, // STORE_FAST_N 41
    0x24,0x29, // LOAD_FAST_N 41
    0x10,0x48, // LOAD_CONST_STRING 'Z'
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_message = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 41,
    .fun_data = fun_data_alles_message,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 907,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 45,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 41,
        .n_kwonly_args = 0,
        .n_def_pos_args = 41,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_alles_message + 62,
        .line_info_top = fun_data_alles_message + 106,
        .opcodes = fun_data_alles_message + 106,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_send
static const byte fun_data_alles_send[33] = {
    0xb1,0x81,0x40,0x0e, // prelude
    0x49,0x81,0x12, // names: send, retries
    0x80,0x7d,0x20,0x29, // code info
    0x12,0x1f, // LOAD_GLOBAL 'message'
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x00, // CALL_FUNCTION_VAR_KW 256
    0xc2, // STORE_FAST 2
    0x12,0x05, // LOAD_GLOBAL 'tulip'
    0x14,0x4a, // LOAD_METHOD 'alles_send'
    0xb2, // LOAD_FAST 2
    0x12,0x81,0x0a, // LOAD_GLOBAL 'mesh_flag'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_send = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_alles_send,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 73,
        .line_info = fun_data_alles_send + 7,
        .line_info_top = fun_data_alles_send + 11,
        .opcodes = fun_data_alles_send + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_reset
static const byte fun_data_alles_reset[46] = {
    0xb9,0x81,0x40,0x0c, // prelude
    0x4b,0x08, // names: reset, osc
    0x80,0x86,0x26,0x4e, // code info
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x4b, // LOAD_CONST_STRING 'reset'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x00, // CALL_FUNCTION_VAR_KW 512
    0x59, // POP_TOP
    0x42,0x4e, // JUMP 14
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x4b, // LOAD_CONST_STRING 'reset'
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x00, // CALL_FUNCTION_VAR_KW 512
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_reset = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_alles_reset,
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
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 75,
        .line_info = fun_data_alles_reset + 6,
        .line_info_top = fun_data_alles_reset + 10,
        .opcodes = fun_data_alles_reset + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_volume
static const byte fun_data_alles_volume[22] = {
    0xb2,0x01,0x0a, // prelude
    0x4c,0x4c,0x4d, // names: volume, volume, client
    0x80,0x8c, // code info
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x4d, // LOAD_CONST_STRING 'client'
    0xb1, // LOAD_FAST 1
    0x10,0x4c, // LOAD_CONST_STRING 'volume'
    0xb0, // LOAD_FAST 0
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_volume = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_volume,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 22,
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
        .qstr_block_name_idx = 76,
        .line_info = fun_data_alles_volume + 6,
        .line_info_top = fun_data_alles_volume + 8,
        .opcodes = fun_data_alles_volume + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_latency_ms
static const byte fun_data_alles_latency_ms[23] = {
    0xb2,0x01,0x0c, // prelude
    0x4e,0x81,0x1d,0x4d, // names: latency_ms, latency, client
    0x80,0x8f, // code info
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x4d, // LOAD_CONST_STRING 'client'
    0xb1, // LOAD_FAST 1
    0x10,0x4e, // LOAD_CONST_STRING 'latency_ms'
    0xb0, // LOAD_FAST 0
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_latency_ms = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_latency_ms,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 23,
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
        .qstr_block_name_idx = 78,
        .line_info = fun_data_alles_latency_ms + 7,
        .line_info_top = fun_data_alles_latency_ms + 9,
        .opcodes = fun_data_alles_latency_ms + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_test
static const byte fun_data_alles_test[78] = {
    0x88,0x10,0x0e, // prelude
    0x4f, // names: test
    0x80,0x95,0x20,0x30,0x25,0x37, // code info
    0x12,0x70, // LOAD_GLOBAL 'SINE'
    0x12,0x72, // LOAD_GLOBAL 'SAW_DOWN'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x12,0x74, // LOAD_GLOBAL 'TRIANGLE'
    0x12,0x75, // LOAD_GLOBAL 'NOISE'
    0x2b,0x05, // BUILD_LIST 5
    0x5f, // GET_ITER_STACK
    0x4b,0x30, // FOR_ITER 48
    0xc0, // STORE_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x64, // JUMP 36
    0x57, // DUP_TOP
    0xc1, // STORE_FAST 1
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0xb0, // LOAD_FAST 0
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0xa8, // LOAD_CONST_SMALL_INT 40
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x8a,0x00, // CALL_FUNCTION 1280
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x17, // POP_JUMP_IF_TRUE -41
    0x59, // POP_TOP
    0x42,0x0e, // JUMP -50
    0x42,0xbe,0x7f, // JUMP -66
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_test = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_alles_test,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 78,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 79,
        .line_info = fun_data_alles_test + 4,
        .line_info_top = fun_data_alles_test + 10,
        .opcodes = fun_data_alles_test + 10,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_play_patches
static const byte fun_data_alles_play_patches[105] = {
    0xaa,0x90,0x41,0x1e, // prelude
    0x51,0x81,0x1e,0x81,0x1f, // names: play_patches, wait, patch_total
    0x80,0xa0,0x22,0x22,0x20,0x26,0x24,0x24,0x3f,0x28, // code info
    0x52, // LOAD_CONST_TRUE
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc4, // STORE_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc2,0x80, // JUMP 66
    0x57, // DUP_TOP
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0xb1, // LOAD_FAST 1
    0xf8, // BINARY_OP 33 __mod__
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xc4, // STORE_FAST 4
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb5, // LOAD_FAST 5
    0x12,0x6d, // LOAD_GLOBAL 'OSCS'
    0xf8, // BINARY_OP 33 __mod__
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0xb5, // LOAD_FAST 5
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0xf2, // BINARY_OP 27 __add__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x78, // LOAD_GLOBAL 'ALGO'
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0xb6, // LOAD_FAST 6
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8c,0x00, // CALL_FUNCTION_VAR_KW 1536
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb5, // LOAD_FAST 5
    0x12,0x6d, // LOAD_GLOBAL 'OSCS'
    0xf8, // BINARY_OP 33 __mod__
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x98, // LOAD_CONST_SMALL_INT 24
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb8,0x7f, // POP_JUMP_IF_TRUE -72
    0x59, // POP_TOP
    0x42,0xb0,0x7f, // JUMP -80
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_play_patches = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_play_patches,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 105,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 22,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 81,
        .line_info = fun_data_alles_play_patches + 9,
        .line_info_top = fun_data_alles_play_patches + 19,
        .opcodes = fun_data_alles_play_patches + 19,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_polyphony
static const byte fun_data_alles_polyphony[173] = {
    0xb9,0x91,0x40,0x22, // prelude
    0x52,0x81,0x20, // names: polyphony, max_voices
    0x80,0xae,0x22,0x23,0x2d,0x2c,0x3a,0x2c,0x20,0x26,0x33,0x1f,0x26,0x29, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x2b,0x00, // BUILD_LIST 0
    0xc3, // STORE_FAST 3
    0x12,0x81,0x21, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf7, // BINARY_OP 32 __truediv__
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x61, // JUMP 33
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x14,0x53, // LOAD_METHOD 'append'
    0x12,0x81,0x21, // LOAD_GLOBAL 'int'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x14,0x53, // LOAD_METHOD 'append'
    0x12,0x81,0x21, // LOAD_GLOBAL 'int'
    0xb4, // LOAD_FAST 4
    0x12,0x6d, // LOAD_GLOBAL 'OSCS'
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf7, // BINARY_OP 32 __truediv__
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1a, // POP_JUMP_IF_TRUE -38
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x12,0x81,0x22, // LOAD_GLOBAL 'print'
    0x12,0x81,0x1b, // LOAD_GLOBAL 'str'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0x12,0x81,0x22, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0xb5, // LOAD_FAST 5
    0x9e, // LOAD_CONST_SMALL_INT 30
    0xb2, // LOAD_FAST 2
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0xf4, // BINARY_OP 29 __mul__
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0xb5, // LOAD_FAST 5
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x10,0x16, // LOAD_CONST_STRING 'patch'
    0xb2, // LOAD_FAST 2
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x06, // LOAD_GLOBAL 'FILTER_NONE'
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0xb2, // LOAD_FAST 2
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0xf4, // BINARY_OP 29 __mul__
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0x9e, // LOAD_CONST_SMALL_INT 30
    0xb2, // LOAD_FAST 2
    0xf2, // BINARY_OP 27 __add__
    0x10,0x4d, // LOAD_CONST_STRING 'client'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x90,0x00, // CALL_FUNCTION_VAR_KW 2048
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0xf8, // BINARY_OP 33 __mod__
    0xc2, // STORE_FAST 2
    0x42,0xae,0x7f, // JUMP -82
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_polyphony = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_alles_polyphony,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 173,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 24,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 82,
        .line_info = fun_data_alles_polyphony + 7,
        .line_info_top = fun_data_alles_polyphony + 21,
        .opcodes = fun_data_alles_polyphony + 21,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_eq_test
static const byte fun_data_alles_eq_test[144] = {
    0x68,0x18, // prelude
    0x54, // names: eq_test
    0x80,0xbc,0x25,0x1f,0x2c,0x25,0x34,0x35,0x29,0x28,0x25, // code info
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2b,0x03, // BUILD_LIST 3
    0x8f, // LOAD_CONST_SMALL_INT 15
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2b,0x03, // BUILD_LIST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x8f, // LOAD_CONST_SMALL_INT 15
    0x2b,0x03, // BUILD_LIST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x8f, // LOAD_CONST_SMALL_INT 15
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2b,0x03, // BUILD_LIST 3
    0x71, // LOAD_CONST_SMALL_INT -15
    0x71, // LOAD_CONST_SMALL_INT -15
    0x8f, // LOAD_CONST_SMALL_INT 15
    0x2b,0x03, // BUILD_LIST 3
    0x71, // LOAD_CONST_SMALL_INT -15
    0x71, // LOAD_CONST_SMALL_INT -15
    0x9e, // LOAD_CONST_SMALL_INT 30
    0x2b,0x03, // BUILD_LIST 3
    0x71, // LOAD_CONST_SMALL_INT -15
    0x9e, // LOAD_CONST_SMALL_INT 30
    0x71, // LOAD_CONST_SMALL_INT -15
    0x2b,0x03, // BUILD_LIST 3
    0x9e, // LOAD_CONST_SMALL_INT 30
    0x71, // LOAD_CONST_SMALL_INT -15
    0x71, // LOAD_CONST_SMALL_INT -15
    0x2b,0x03, // BUILD_LIST 3
    0x2b,0x08, // BUILD_LIST 8
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x4c, // FOR_ITER 76
    0xc1, // STORE_FAST 1
    0x12,0x81,0x22, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x2a,0x03, // BUILD_TUPLE 3
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x55, // LOAD_CONST_STRING 'eq_l'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x56, // LOAD_CONST_STRING 'eq_m'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x10,0x57, // LOAD_CONST_STRING 'eq_h'
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x34,0x86,0x00, // CALL_FUNCTION 768
    0x59, // POP_TOP
    0x12,0x5b, // LOAD_GLOBAL 'drums'
    0x10,0x58, // LOAD_CONST_STRING 'loops'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x34,0x82,0x00, // CALL_FUNCTION 256
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x4b, // LOAD_GLOBAL 'reset'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xb2,0x7f, // JUMP -78
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_eq_test = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_alles_eq_test,
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
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 84,
        .line_info = fun_data_alles_eq_test + 3,
        .line_info_top = fun_data_alles_eq_test + 14,
        .opcodes = fun_data_alles_eq_test + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_sweep
static const byte fun_data_alles_sweep[197] = {
    0xd3,0x91,0x01,0x26, // prelude
    0x59,0x81,0x23,0x81,0x24,0x58, // names: sweep, speed, res, loops
    0x80,0xca,0x24,0x22,0x23,0x2f,0x28,0x1f,0x2a,0x1f,0x2c,0x1f,0x28, // code info
    0x22,0x8f,0x50, // LOAD_CONST_SMALL_INT 2000
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc4, // STORE_FAST 4
    0x42,0x9d,0x81, // JUMP 157
    0x80, // LOAD_CONST_SMALL_INT 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0x85, // LOAD_CONST_SMALL_INT 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x84, // LOAD_CONST_SMALL_INT 4
    0x85, // LOAD_CONST_SMALL_INT 5
    0x2b,0x08, // BUILD_LIST 8
    0x5f, // GET_ITER_STACK
    0x4b,0x8f,0x01, // FOR_ITER 143
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0x22,0x80,0x64, // LOAD_CONST_SMALL_INT 100
    0xf2, // BINARY_OP 27 __add__
    0xb3, // LOAD_FAST 3
    0xf8, // BINARY_OP 33 __mod__
    0xc4, // STORE_FAST 4
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x07, // LOAD_GLOBAL 'FILTER_LPF'
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0xb4, // LOAD_FAST 4
    0x22,0x81,0x7a, // LOAD_CONST_SMALL_INT 250
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0d, // LOAD_CONST_STRING 'resonance'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0x10,0x5a, // LOAD_CONST_STRING 'duty'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x90,0x00, // CALL_FUNCTION 2048
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x07, // LOAD_GLOBAL 'FILTER_LPF'
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0xb4, // LOAD_FAST 4
    0x22,0x83,0x74, // LOAD_CONST_SMALL_INT 500
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0d, // LOAD_CONST_STRING 'resonance'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0x10,0x5a, // LOAD_CONST_STRING 'duty'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x90,0x00, // CALL_FUNCTION 2048
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x10,0x0e, // LOAD_CONST_STRING 'filter_type'
    0x12,0x81,0x07, // LOAD_GLOBAL 'FILTER_LPF'
    0x10,0x0c, // LOAD_CONST_STRING 'filter_freq'
    0xb4, // LOAD_FAST 4
    0x10,0x0d, // LOAD_CONST_STRING 'resonance'
    0xb1, // LOAD_FAST 1
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x12,0x71, // LOAD_GLOBAL 'PULSE'
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0x86, // LOAD_CONST_SMALL_INT 6
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0x10,0x5a, // LOAD_CONST_STRING 'duty'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x90,0x00, // CALL_FUNCTION 2048
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xee,0x7e, // JUMP -146
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x43,0xdd,0x7e, // POP_JUMP_IF_TRUE -163
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_sweep = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 3,
    .fun_data = fun_data_alles_sweep,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 197,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 27,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 89,
        .line_info = fun_data_alles_sweep + 10,
        .line_info_top = fun_data_alles_sweep + 23,
        .opcodes = fun_data_alles_sweep + 23,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_drums
static const byte fun_data_alles_drums[359] = {
    0xd2,0x90,0x41,0x3e, // prelude
    0x5b,0x81,0x25,0x58, // names: drums, bpm, loops
    0x80,0xd8,0x2d,0x2d,0x30,0x30,0x2d,0x2d,0x30,0x39,0x2f,0x23,0x24,0x2e,0x25,0x2f,0x25,0x2d,0x25,0x2c,0x25,0x2c,0x25,0x2c,0x27,0x59,0x2f, // code info
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x8d, // LOAD_CONST_SMALL_INT 13
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x01, // CALL_FUNCTION_VAR_KW 513
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x01, // CALL_FUNCTION_VAR_KW 513
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x89, // LOAD_CONST_SMALL_INT 9
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x10,0x5c, // LOAD_CONST_STRING 'pan'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x86,0x01, // CALL_FUNCTION_VAR_KW 769
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x5c, // LOAD_CONST_STRING 'pan'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x86,0x01, // CALL_FUNCTION_VAR_KW 769
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x8b, // LOAD_CONST_SMALL_INT 11
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x01, // CALL_FUNCTION_VAR_KW 513
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'preset'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x87, // LOAD_CONST_SMALL_INT 7
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x84,0x01, // CALL_FUNCTION_VAR_KW 513
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x88, // LOAD_CONST_SMALL_INT 8
    0x90, // LOAD_CONST_SMALL_INT 16
    0xa0, // LOAD_CONST_SMALL_INT 32
    0x2b,0x06, // BUILD_LIST 6
    0x30,0x06, // UNPACK_SEQUENCE 6
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0xc8, // STORE_FAST 8
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xb7, // LOAD_FAST 7
    0xf2, // BINARY_OP 27 __add__
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0xb4, // LOAD_FAST 4
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xb5, // LOAD_FAST 5
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0x2b,0x08, // BUILD_LIST 8
    0xc9, // STORE_FAST 9
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x22,0x32, // LOAD_CONST_SMALL_INT 50
    0x22,0x34, // LOAD_CONST_SMALL_INT 52
    0x22,0x33, // LOAD_CONST_SMALL_INT 51
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2b,0x08, // BUILD_LIST 8
    0xca, // STORE_FAST 10
    0x42,0xad,0x81, // JUMP 173
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0xc1, // STORE_FAST 1
    0x12,0x81,0x26, // LOAD_GLOBAL 'enumerate'
    0xb9, // LOAD_FAST 9
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x9f,0x01, // FOR_ITER 159
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xcb, // STORE_FAST 11
    0xcc, // STORE_FAST 12
    0xbc, // LOAD_FAST 12
    0xb3, // LOAD_FAST 3
    0xef, // BINARY_OP 24 __and__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x86,0x00, // CALL_FUNCTION_VAR_KW 768
    0x59, // POP_TOP
    0xbc, // LOAD_FAST 12
    0xb4, // LOAD_FAST 4
    0xef, // BINARY_OP 24 __and__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0xbc, // LOAD_FAST 12
    0xb5, // LOAD_FAST 5
    0xef, // BINARY_OP 24 __and__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0xbc, // LOAD_FAST 12
    0xb6, // LOAD_FAST 6
    0xef, // BINARY_OP 24 __and__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0xbc, // LOAD_FAST 12
    0xb7, // LOAD_FAST 7
    0xef, // BINARY_OP 24 __and__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x00, // CALL_FUNCTION 512
    0x59, // POP_TOP
    0xba, // LOAD_FAST 10
    0xbb, // LOAD_FAST 11
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x87, // LOAD_CONST_SMALL_INT 7
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x10,0x17, // LOAD_CONST_STRING 'note'
    0xba, // LOAD_FAST 10
    0xbb, // LOAD_FAST 11
    0x55, // LOAD_SUBSCR
    0x8c, // LOAD_CONST_SMALL_INT 12
    0xf3, // BINARY_OP 28 __sub__
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x88,0x00, // CALL_FUNCTION_VAR_KW 1024
    0x59, // POP_TOP
    0x42,0x4f, // JUMP 15
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x87, // LOAD_CONST_SMALL_INT 7
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x86,0x00, // CALL_FUNCTION_VAR_KW 768
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x50, // LOAD_METHOD 'sleep'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf4, // BINARY_OP 29 __mul__
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf7, // BINARY_OP 32 __truediv__
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xde,0x7e, // JUMP -162
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x43,0xcd,0x7e, // POP_JUMP_IF_TRUE -179
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_drums = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_drums,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 359,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 27,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 91,
        .line_info = fun_data_alles_drums + 8,
        .line_info_top = fun_data_alles_drums + 35,
        .opcodes = fun_data_alles_drums + 35,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_c_major
static const byte fun_data_alles_c_major[86] = {
    0xf2,0x80,0x41,0x10, // prelude
    0x5d,0x81,0x27,0x09, // names: c_major, octave, wave
    0x80,0xf9,0x38,0x38, // code info
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x12, // LOAD_CONST_OBJ 18
    0xb0, // LOAD_FAST 0
    0xf4, // BINARY_OP 29 __mul__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x13, // LOAD_CONST_OBJ 19
    0xb0, // LOAD_FAST 0
    0xf4, // BINARY_OP 29 __mul__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x23,0x14, // LOAD_CONST_OBJ 20
    0xb0, // LOAD_FAST 0
    0xf4, // BINARY_OP 29 __mul__
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'vel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x8a,0x00, // CALL_FUNCTION_VAR_KW 1280
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_c_major = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 2,
    .fun_data = fun_data_alles_c_major,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 86,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 93,
        .line_info = fun_data_alles_c_major + 8,
        .line_info_top = fun_data_alles_c_major + 12,
        .opcodes = fun_data_alles_c_major + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_chorus
static const byte fun_data_alles_chorus[109] = {
    0xc9,0x85,0x80,0x01,0x2e, // prelude
    0x5e,0x81,0x28,0x81,0x29,0x10,0x5f,0x09, // names: chorus, level, max_delay, freq, amp, wave
    0x90,0x01,0x23,0x25,0x25,0x25,0x25,0x25,0x25,0x4a,0x47,0x25,0x25,0x25,0x25, // code info
    0x2c,0x00, // BUILD_MAP 0
    0xc5, // STORE_FAST 5
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0x10,0x10, // LOAD_CONST_STRING 'freq'
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb3, // LOAD_FAST 3
    0xb5, // LOAD_FAST 5
    0x10,0x5f, // LOAD_CONST_STRING 'amp'
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x10,0x09, // LOAD_CONST_STRING 'wave'
    0x56, // STORE_SUBSCR
    0x12,0x81,0x1c, // LOAD_GLOBAL 'len'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x81,0x0b, // LOAD_GLOBAL 'CHORUS_OSC'
    0xb5, // LOAD_FAST 5
    0x10,0x08, // LOAD_CONST_STRING 'osc'
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0xb5, // LOAD_FAST 5
    0x10,0x60, // LOAD_CONST_STRING 'chorus_level'
    0x56, // STORE_SUBSCR
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb1, // LOAD_FAST 1
    0xb5, // LOAD_FAST 5
    0x10,0x61, // LOAD_CONST_STRING 'chorus_delay'
    0x56, // STORE_SUBSCR
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x53, // LOAD_NULL
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x00, // CALL_FUNCTION_VAR_KW 256
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_chorus = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 5,
    .fun_data = fun_data_alles_chorus,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 109,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 5,
        .qstr_block_name_idx = 94,
        .line_info = fun_data_alles_chorus + 13,
        .line_info_top = fun_data_alles_chorus + 28,
        .opcodes = fun_data_alles_chorus + 28,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_reverb
static const byte fun_data_alles_reverb[79] = {
    0xc0,0x84,0x80,0x01,0x28, // prelude
    0x62,0x81,0x28,0x81,0x2a,0x81,0x2b,0x81,0x2c, // names: reverb, level, liveness, damping, xover_hz
    0x90,0x16,0x23,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25, // code info
    0x2c,0x00, // BUILD_MAP 0
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0xb4, // LOAD_FAST 4
    0x10,0x63, // LOAD_CONST_STRING 'reverb_level'
    0x56, // STORE_SUBSCR
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb1, // LOAD_FAST 1
    0xb4, // LOAD_FAST 4
    0x10,0x64, // LOAD_CONST_STRING 'reverb_liveness'
    0x56, // STORE_SUBSCR
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0x10,0x65, // LOAD_CONST_STRING 'reverb_damping'
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x10,0x66, // LOAD_CONST_STRING 'reverb_xover'
    0x56, // STORE_SUBSCR
    0x12,0x49, // LOAD_GLOBAL 'send'
    0x53, // LOAD_NULL
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x00, // CALL_FUNCTION_VAR_KW 256
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_reverb = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_alles_reverb,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 79,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 98,
        .line_info = fun_data_alles_reverb + 14,
        .line_info_top = fun_data_alles_reverb + 25,
        .opcodes = fun_data_alles_reverb + 25,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_mesh
static const byte fun_data_alles_mesh[71] = {
    0x99,0x01,0x1a, // prelude
    0x67,0x81,0x2d, // names: mesh, local_ip
    0x90,0x24,0x20,0x2a,0x28,0x22,0x28,0x24,0x26,0x28, // code info
    0x12,0x05, // LOAD_GLOBAL 'tulip'
    0x14,0x68, // LOAD_METHOD 'ip'
    0x36,0x00, // CALL_METHOD 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x22, // LOAD_GLOBAL 'print'
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x4e, // LOAD_GLOBAL 'latency_ms'
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x17,0x81,0x0a, // STORE_GLOBAL 'mesh_flag'
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x05, // LOAD_GLOBAL 'tulip'
    0x14,0x69, // LOAD_METHOD 'multicast_start'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x05, // LOAD_GLOBAL 'tulip'
    0x14,0x69, // LOAD_METHOD 'multicast_start'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_mesh = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_alles_mesh,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 71,
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
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 103,
        .line_info = fun_data_alles_mesh + 6,
        .line_info_top = fun_data_alles_mesh + 16,
        .opcodes = fun_data_alles_mesh + 16,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of alles__lt_module_gt_
// frozen bytecode for file alles.py, scope alles_local
static const byte fun_data_alles_local[19] = {
    0x08,0x0a, // prelude
    0x6a, // names: local
    0x90,0x2f,0x40,0x24, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x17,0x81,0x0a, // STORE_GLOBAL 'mesh_flag'
    0x12,0x4e, // LOAD_GLOBAL 'latency_ms'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_alles_local = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_alles_local,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 19,
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
        .qstr_block_name_idx = 106,
        .line_info = fun_data_alles_local + 3,
        .line_info_top = fun_data_alles_local + 7,
        .opcodes = fun_data_alles_local + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_alles__lt_module_gt_[] = {
    &raw_code_alles_preset,
    &raw_code_alles_millis,
    &raw_code_alles_trunc,
    &raw_code_alles_message,
    &raw_code_alles_send,
    &raw_code_alles_reset,
    &raw_code_alles_volume,
    &raw_code_alles_latency_ms,
    &raw_code_alles_test,
    &raw_code_alles_play_patches,
    &raw_code_alles_polyphony,
    &raw_code_alles_eq_test,
    &raw_code_alles_sweep,
    &raw_code_alles_drums,
    &raw_code_alles_c_major,
    &raw_code_alles_chorus,
    &raw_code_alles_reverb,
    &raw_code_alles_mesh,
    &raw_code_alles_local,
};

static const mp_raw_code_t raw_code_alles__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_alles__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 407,
    #endif
    .children = (void *)&children_alles__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 19,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 41,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_alles__lt_module_gt_ + 4,
        .line_info_top = fun_data_alles__lt_module_gt_ + 65,
        .opcodes = fun_data_alles__lt_module_gt_ + 65,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_alles[174] = {
    MP_QSTR_alles_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_struct,
    MP_QSTR_os,
    MP_QSTR_time,
    MP_QSTR_tulip,
    MP_QSTR_,
    MP_QSTR_preset,
    MP_QSTR_osc,
    MP_QSTR_wave,
    MP_QSTR_bp0,
    MP_QSTR_bp0_target,
    MP_QSTR_filter_freq,
    MP_QSTR_resonance,
    MP_QSTR_filter_type,
    MP_QSTR_vel,
    MP_QSTR_freq,
    MP_QSTR_mod_target,
    MP_QSTR_mod_source,
    MP_QSTR_phase,
    MP_QSTR_500_comma_0_comma_0_comma_0,
    MP_QSTR_250_comma_0_comma_0_comma_0,
    MP_QSTR_patch,
    MP_QSTR_note,
    MP_QSTR_millis,
    MP_QSTR_ticks_ms,
    MP_QSTR_trunc,
    MP_QSTR__percent__dot_6f,
    MP_QSTR_rstrip,
    MP_QSTR_0,
    MP_QSTR__dot_,
    MP_QSTR_message,
    MP_QSTR_t,
    MP_QSTR_v,
    MP_QSTR_w,
    MP_QSTR_d,
    MP_QSTR_b,
    MP_QSTR_f,
    MP_QSTR_n,
    MP_QSTR_p,
    MP_QSTR_P,
    MP_QSTR_Q,
    MP_QSTR_c,
    MP_QSTR_a,
    MP_QSTR_l,
    MP_QSTR_V,
    MP_QSTR_N,
    MP_QSTR_R,
    MP_QSTR_F,
    MP_QSTR_I,
    MP_QSTR_o,
    MP_QSTR_A_percent_s,
    MP_QSTR_B_percent_s,
    MP_QSTR_C_percent_s,
    MP_QSTR_O_percent_s,
    MP_QSTR_T,
    MP_QSTR_W,
    MP_QSTR_X,
    MP_QSTR_g,
    MP_QSTR_L,
    MP_QSTR_S,
    MP_QSTR_D,
    MP_QSTR_x,
    MP_QSTR_y,
    MP_QSTR_z,
    MP_QSTR_G,
    MP_QSTR_k,
    MP_QSTR_m,
    MP_QSTR_h,
    MP_QSTR_H,
    MP_QSTR_j,
    MP_QSTR_J,
    MP_QSTR_Z,
    MP_QSTR_send,
    MP_QSTR_alles_send,
    MP_QSTR_reset,
    MP_QSTR_volume,
    MP_QSTR_client,
    MP_QSTR_latency_ms,
    MP_QSTR_test,
    MP_QSTR_sleep,
    MP_QSTR_play_patches,
    MP_QSTR_polyphony,
    MP_QSTR_append,
    MP_QSTR_eq_test,
    MP_QSTR_eq_l,
    MP_QSTR_eq_m,
    MP_QSTR_eq_h,
    MP_QSTR_loops,
    MP_QSTR_sweep,
    MP_QSTR_duty,
    MP_QSTR_drums,
    MP_QSTR_pan,
    MP_QSTR_c_major,
    MP_QSTR_chorus,
    MP_QSTR_amp,
    MP_QSTR_chorus_level,
    MP_QSTR_chorus_delay,
    MP_QSTR_reverb,
    MP_QSTR_reverb_level,
    MP_QSTR_reverb_liveness,
    MP_QSTR_reverb_damping,
    MP_QSTR_reverb_xover,
    MP_QSTR_mesh,
    MP_QSTR_ip,
    MP_QSTR_multicast_start,
    MP_QSTR_local,
    MP_QSTR_BLOCK_SIZE,
    MP_QSTR_SAMPLE_RATE,
    MP_QSTR_OSCS,
    MP_QSTR_MAX_QUEUE,
    MP_QSTR_range,
    MP_QSTR_SINE,
    MP_QSTR_PULSE,
    MP_QSTR_SAW_DOWN,
    MP_QSTR_SAW_UP,
    MP_QSTR_TRIANGLE,
    MP_QSTR_NOISE,
    MP_QSTR_KS,
    MP_QSTR_PCM,
    MP_QSTR_ALGO,
    MP_QSTR_PARTIAL,
    MP_QSTR_PARTIALS,
    MP_QSTR_OFF,
    MP_QSTR_TARGET_AMP,
    MP_QSTR_TARGET_DUTY,
    MP_QSTR_TARGET_FREQ,
    MP_QSTR_TARGET_FILTER_FREQ,
    MP_QSTR_TARGET_RESONANCE,
    MP_QSTR_TARGET_FEEDBACK,
    MP_QSTR_TARGET_LINEAR,
    MP_QSTR_TARGET_TRUE_EXPONENTIAL,
    MP_QSTR_TARGET_DX7_EXPONENTIAL,
    MP_QSTR_TARGET_PAN,
    MP_QSTR_FILTER_NONE,
    MP_QSTR_FILTER_LPF,
    MP_QSTR_FILTER_BPF,
    MP_QSTR_FILTER_HPF,
    MP_QSTR_mesh_flag,
    MP_QSTR_CHORUS_OSC,
    MP_QSTR_send_buffer,
    MP_QSTR_buffer_size,
    MP_QSTR_which,
    MP_QSTR_number,
    MP_QSTR_feedback,
    MP_QSTR_timestamp,
    MP_QSTR_retries,
    MP_QSTR_bp1,
    MP_QSTR_bp2,
    MP_QSTR_bp1_target,
    MP_QSTR_bp2_target,
    MP_QSTR_debug,
    MP_QSTR_algorithm,
    MP_QSTR_ratio,
    MP_QSTR_algo_source,
    MP_QSTR_str,
    MP_QSTR_len,
    MP_QSTR_latency,
    MP_QSTR_wait,
    MP_QSTR_patch_total,
    MP_QSTR_max_voices,
    MP_QSTR_int,
    MP_QSTR_print,
    MP_QSTR_speed,
    MP_QSTR_res,
    MP_QSTR_bpm,
    MP_QSTR_enumerate,
    MP_QSTR_octave,
    MP_QSTR_level,
    MP_QSTR_max_delay,
    MP_QSTR_liveness,
    MP_QSTR_damping,
    MP_QSTR_xover_hz,
    MP_QSTR_local_ip,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_0 = {{&mp_type_float}, (mp_float_t)44100};
#define const_obj_alles_0_macro MP_ROM_PTR(&const_obj_alles_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_0_macro ((mp_rom_obj_t)(0xc7ac4402))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_0_macro ((mp_rom_obj_t)(0xc0e9888000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_alles_1 = {{&mp_type_tuple}, 10, {
    MP_ROM_INT(1),
    MP_ROM_INT(2),
    MP_ROM_INT(4),
    MP_ROM_INT(8),
    MP_ROM_INT(16),
    MP_ROM_INT(32),
    MP_ROM_INT(64),
    MP_ROM_INT(128),
    MP_ROM_INT(256),
    MP_ROM_INT(512),
}};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_2 = {{&mp_type_float}, (mp_float_t)0.5};
#define const_obj_alles_2_macro MP_ROM_PTR(&const_obj_alles_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_2_macro ((mp_rom_obj_t)(0xbf800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_2_macro ((mp_rom_obj_t)(0xbfe4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_3 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_alles_3_macro MP_ROM_PTR(&const_obj_alles_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_3_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_3_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
static const mp_obj_str_t const_obj_alles_6 = {{&mp_type_str}, 4663, 25, (const byte*)"\x30\x2c\x30\x2c\x35\x30\x30\x2c\x31\x2c\x31\x30\x30\x30\x2c\x30\x2e\x32\x35\x2c\x37\x35\x30\x2c\x30"};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_7 = {{&mp_type_float}, (mp_float_t)1.5};
#define const_obj_alles_7_macro MP_ROM_PTR(&const_obj_alles_7)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_7_macro ((mp_rom_obj_t)(0xc0400002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_7_macro ((mp_rom_obj_t)(0xbffc000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_9 = {{&mp_type_float}, (mp_float_t)0.25};
#define const_obj_alles_9_macro MP_ROM_PTR(&const_obj_alles_9)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_9_macro ((mp_rom_obj_t)(0xbf000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_9_macro ((mp_rom_obj_t)(0xbfd4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_12 = {{&mp_type_float}, (mp_float_t)2.5};
#define const_obj_alles_12_macro MP_ROM_PTR(&const_obj_alles_12)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_12_macro ((mp_rom_obj_t)(0xc0a00002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_12_macro ((mp_rom_obj_t)(0xc008000000000000))
#endif
static const mp_obj_str_t const_obj_alles_14 = {{&mp_type_str}, 3081, 25, (const byte*)"\x6f\x73\x63\x20\x25\x64\x20\x6e\x6f\x74\x65\x20\x25\x64\x20\x66\x69\x6c\x74\x65\x72\x20\x25\x66\x20"};
static const mp_obj_str_t const_obj_alles_15 = {{&mp_type_str}, 14685, 35, (const byte*)"\x65\x71\x5f\x6c\x20\x3d\x20\x25\x64\x64\x42\x20\x65\x71\x5f\x6d\x20\x3d\x20\x25\x64\x64\x42\x20\x65\x71\x5f\x68\x20\x3d\x20\x25\x64\x64\x42"};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_16 = {{&mp_type_float}, (mp_float_t)0.9};
#define const_obj_alles_16_macro MP_ROM_PTR(&const_obj_alles_16)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_16_macro ((mp_rom_obj_t)(0xbfe66666))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_16_macro ((mp_rom_obj_t)(0xbff0cccccccccccd))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_17 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_alles_17_macro MP_ROM_PTR(&const_obj_alles_17)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_17_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_17_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_18 = {{&mp_type_float}, (mp_float_t)220.5};
#define const_obj_alles_18_macro MP_ROM_PTR(&const_obj_alles_18)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_18_macro ((mp_rom_obj_t)(0xc3dc8002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_18_macro ((mp_rom_obj_t)(0xc06f900000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_19 = {{&mp_type_float}, (mp_float_t)138.5};
#define const_obj_alles_19_macro MP_ROM_PTR(&const_obj_alles_19)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_19_macro ((mp_rom_obj_t)(0xc38a8002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_19_macro ((mp_rom_obj_t)(0xc065500000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_alles_20 = {{&mp_type_float}, (mp_float_t)164.5};
#define const_obj_alles_20_macro MP_ROM_PTR(&const_obj_alles_20)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_alles_20_macro ((mp_rom_obj_t)(0xc3a48002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_alles_20_macro ((mp_rom_obj_t)(0xc068900000000000))
#endif
static const mp_obj_str_t const_obj_alles_21 = {{&mp_type_str}, 20340, 55, (const byte*)"\x4e\x65\x65\x64\x20\x74\x6f\x20\x62\x65\x20\x6f\x6e\x20\x77\x69\x66\x69\x2e\x20\x55\x73\x65\x20\x74\x75\x6c\x69\x70\x2e\x77\x69\x66\x69\x28\x27\x73\x73\x69\x64\x27\x2c\x20\x27\x70\x61\x73\x73\x77\x6f\x72\x64\x27\x29\x2e"};

// constant table
static const mp_rom_obj_t const_obj_table_data_alles[22] = {
    const_obj_alles_0_macro,
    MP_ROM_PTR(&const_obj_alles_1),
    const_obj_alles_2_macro,
    const_obj_alles_3_macro,
    MP_ROM_QSTR(MP_QSTR_10_comma_1_comma_250_comma_0_dot_7_comma_500_comma_0),
    MP_ROM_QSTR(MP_QSTR_100_comma_0_dot_5_comma_25_comma_0),
    MP_ROM_PTR(&const_obj_alles_6),
    const_obj_alles_7_macro,
    MP_ROM_QSTR(MP_QSTR_150_comma_1_comma_250_comma_0_dot_25_comma_250_comma_0),
    const_obj_alles_9_macro,
    MP_ROM_QSTR(MP_QSTR_150_comma_1_comma_400_comma_0_comma_0_comma_0),
    MP_ROM_QSTR(MP_QSTR_25_comma_1_comma_75_comma_0_comma_0_comma_0),
    const_obj_alles_12_macro,
    MP_ROM_QSTR(MP_QSTR_1_comma_1_comma_500_comma_0_comma_0_comma_0),
    MP_ROM_PTR(&const_obj_alles_14),
    MP_ROM_PTR(&const_obj_alles_15),
    const_obj_alles_16_macro,
    const_obj_alles_17_macro,
    const_obj_alles_18_macro,
    const_obj_alles_19_macro,
    const_obj_alles_20_macro,
    MP_ROM_PTR(&const_obj_alles_21),
};

static const mp_frozen_module_t frozen_module_alles = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_alles,
        .obj_table = (mp_obj_t *)&const_obj_table_data_alles,
    },
    .rc = &raw_code_alles__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module urequests
// - original source file: build-standard/tulip/obj/frozen_mpy/urequests.mpy
// - frozen file name: urequests.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file urequests.py, scope urequests__lt_module_gt_
static const byte fun_data_urequests__lt_module_gt_[76] = {
    0x30,0x24, // prelude
    0x01, // names: <module>
    0x66,0x89,0x38,0x62,0x40,0x8d,0x85,0x64,0x20,0x64,0x20,0x64,0x20,0x64,0x20,0x64,0x20, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'usocket'
    0x16,0x02, // STORE_NAME 'usocket'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x03, // LOAD_CONST_STRING 'Response'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x03, // STORE_NAME 'Response'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2c,0x00, // BUILD_MAP 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x2a,0x07, // BUILD_TUPLE 7
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x05, // STORE_NAME 'request'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2b, // STORE_NAME 'head'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x2d, // STORE_NAME 'get'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x2e, // STORE_NAME 'post'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x30, // STORE_NAME 'put'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x32, // STORE_NAME 'patch'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x34, // STORE_NAME 'delete'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_Response
static const byte fun_data_urequests_Response[77] = {
    0x08,0x1e, // prelude
    0x03, // names: Response
    0x68,0x20,0x64,0x40,0x84,0x07,0x8a,0x07,0x8c,0x10,0x88,0x0a,0x68,0x20, // code info
    0x11,0x41, // LOAD_NAME '__name__'
    0x16,0x42, // STORE_NAME '__module__'
    0x10,0x03, // LOAD_CONST_STRING 'Response'
    0x16,0x43, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x36, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x26, // STORE_NAME 'close'
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x3a, // STORE_NAME 'generate'
    0x10,0x04, // LOAD_CONST_STRING 'wb'
    0x22,0xa0,0x00, // LOAD_CONST_SMALL_INT 4096
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x3c, // STORE_NAME 'save'
    0x11,0x44, // LOAD_NAME 'property'
    0x32,0x04, // MAKE_FUNCTION 4
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x3d, // STORE_NAME 'content'
    0x11,0x44, // LOAD_NAME 'property'
    0x32,0x05, // MAKE_FUNCTION 5
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x3e, // STORE_NAME 'text'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x3f, // STORE_NAME 'json'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response___init__
static const byte fun_data_urequests_Response___init__[24] = {
    0x1a,0x0e, // prelude
    0x36,0x53,0x54, // names: __init__, self, f
    0x60,0x40,0x24,0x25, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x37, // STORE_ATTR 'raw'
    0x10,0x24, // LOAD_CONST_STRING 'utf-8'
    0xb0, // LOAD_FAST 0
    0x18,0x38, // STORE_ATTR 'encoding'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x39, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response___init__ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 2,
    .fun_data = fun_data_urequests_Response___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 24,
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
        .qstr_block_name_idx = 54,
        .line_info = fun_data_urequests_Response___init__ + 5,
        .line_info_top = fun_data_urequests_Response___init__ + 9,
        .opcodes = fun_data_urequests_Response___init__ + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_close
static const byte fun_data_urequests_Response_close[32] = {
    0x11,0x0e, // prelude
    0x26,0x53, // names: close, self
    0x80,0x0a,0x25,0x28,0x24, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x37, // STORE_ATTR 'raw'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x39, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_close = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_Response_close,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 32,
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
        .line_info = fun_data_urequests_Response_close + 4,
        .line_info_top = fun_data_urequests_Response_close + 9,
        .opcodes = fun_data_urequests_Response_close + 9,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_generate
static const byte fun_data_urequests_Response_generate[33] = {
    0xaa,0x41,0x12, // prelude
    0x3a,0x53,0x55, // names: generate, self, chunk_size
    0x80,0x11,0x20,0x29,0x23,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x3b, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x45, // JUMP 5
    0xb2, // LOAD_FAST 2
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x2d, // JUMP -19
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_generate = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x01,
    .n_pos_args = 2,
    .fun_data = fun_data_urequests_Response_generate,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 33,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 58,
        .line_info = fun_data_urequests_Response_generate + 6,
        .line_info_top = fun_data_urequests_Response_generate + 12,
        .opcodes = fun_data_urequests_Response_generate + 12,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_save
static const byte fun_data_urequests_Response_save[93] = {
    0xc8,0x84,0x01,0x26, // prelude
    0x3c,0x53,0x56,0x57,0x55, // names: save, self, filename, mode, chunk_size
    0x80,0x19,0x27,0x22,0x20,0x29,0x28,0x29,0x22,0x29,0x26,0x28,0x24,0x24, // code info
    0x12,0x58, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc4, // STORE_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc5, // STORE_FAST 5
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x3b, // LOAD_METHOD 'read'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb5, // LOAD_FAST 5
    0x12,0x50, // LOAD_GLOBAL 'len'
    0xb6, // LOAD_FAST 6
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xc5, // STORE_FAST 5
    0x12,0x50, // LOAD_GLOBAL 'len'
    0xb6, // LOAD_FAST 6
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x49, // JUMP 9
    0xb4, // LOAD_FAST 4
    0x14,0x1b, // LOAD_METHOD 'write'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0xb4, // LOAD_FAST 4
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x37, // STORE_ATTR 'raw'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x39, // STORE_ATTR '_cached'
    0xb5, // LOAD_FAST 5
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_save = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 4,
    .fun_data = fun_data_urequests_Response_save,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 93,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 60,
        .line_info = fun_data_urequests_Response_save + 9,
        .line_info_top = fun_data_urequests_Response_save + 23,
        .opcodes = fun_data_urequests_Response_save + 23,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_content
static const byte fun_data_urequests_Response_content[48] = {
    0x2d,0x12, // prelude
    0x3d,0x53, // names: content, self
    0x80,0x29,0x27,0x22,0x4b,0x28,0x25, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x39, // LOAD_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x49,0x0b, // SETUP_FINALLY 11
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x3b, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x39, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR 'raw'
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x37, // STORE_ATTR 'raw'
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x39, // LOAD_ATTR '_cached'
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_content = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_Response_content,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 48,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 61,
        .line_info = fun_data_urequests_Response_content + 4,
        .line_info_top = fun_data_urequests_Response_content + 11,
        .opcodes = fun_data_urequests_Response_content + 11,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_text
static const byte fun_data_urequests_Response_text[17] = {
    0x19,0x08, // prelude
    0x3e,0x53, // names: text, self
    0x80,0x33, // code info
    0x12,0x4d, // LOAD_GLOBAL 'str'
    0xb0, // LOAD_FAST 0
    0x13,0x3d, // LOAD_ATTR 'content'
    0xb0, // LOAD_FAST 0
    0x13,0x38, // LOAD_ATTR 'encoding'
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_text = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_Response_text,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 17,
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
        .qstr_block_name_idx = 62,
        .line_info = fun_data_urequests_Response_text + 4,
        .line_info_top = fun_data_urequests_Response_text + 6,
        .opcodes = fun_data_urequests_Response_text + 6,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests_Response
// frozen bytecode for file urequests.py, scope urequests_Response_json
static const byte fun_data_urequests_Response_json[21] = {
    0x21,0x0a, // prelude
    0x3f,0x53, // names: json, self
    0x80,0x36,0x45, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x1e, // IMPORT_NAME 'ujson'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x40, // LOAD_METHOD 'loads'
    0xb0, // LOAD_FAST 0
    0x13,0x3d, // LOAD_ATTR 'content'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_Response_json = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_Response_json,
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
        .qstr_block_name_idx = 63,
        .line_info = fun_data_urequests_Response_json + 4,
        .line_info_top = fun_data_urequests_Response_json + 7,
        .opcodes = fun_data_urequests_Response_json + 7,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_urequests_Response[] = {
    &raw_code_urequests_Response___init__,
    &raw_code_urequests_Response_close,
    &raw_code_urequests_Response_generate,
    &raw_code_urequests_Response_save,
    &raw_code_urequests_Response_content,
    &raw_code_urequests_Response_text,
    &raw_code_urequests_Response_json,
};

static const mp_raw_code_t raw_code_urequests_Response = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_urequests_Response,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 77,
    #endif
    .children = (void *)&children_urequests_Response,
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
        .qstr_block_name_idx = 3,
        .line_info = fun_data_urequests_Response + 3,
        .line_info_top = fun_data_urequests_Response + 17,
        .opcodes = fun_data_urequests_Response + 17,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_request
static const byte fun_data_urequests_request[1024] = {
    0xc5,0xa1,0x85,0x01,0xda,0x02, // prelude
    0x05,0x45,0x46,0x47,0x3f,0x2a,0x48,0x49,0x4a,0x4b, // names: request, method, url, data, json, headers, stream, auth, timeout, parse_headers
    0x80,0x46,0x22,0x37,0x26,0x45,0x25,0x29,0x32,0x4b,0x22,0x5a,0x2f,0x27,0x26,0x27,0x26,0x46,0x47,0x49,0x27,0x2f,0x48,0x31,0x46,0x23,0x26,0x44,0x54,0x66,0x48,0x23,0x2b,0x26,0x2f,0x2f,0x26,0x4c,0x26,0x29,0x29,0x2b,0x2b,0x26,0x28,0x46,0x28,0x29,0x23,0x23,0x4b,0x2f,0x29,0x24,0x23,0x26,0x30,0x29,0x2b,0x4b,0x48,0x48,0x2a,0x4a,0x2a,0x2a,0x24,0x2a,0x2a,0x20,0x28,0x2b,0x43,0x2a,0x27,0x33,0x3f,0x36,0x50,0x2c,0x25,0x22,0x25,0x2a,0x2f,0x4d,0x54,0x27,0x42,0x23,0x27,0x30,0x4c,0x4d,0x28,0x26,0x26,0x27,0x26, // code info
    0x51, // LOAD_CONST_NONE
    0xc9, // STORE_FAST 9
    0xb2, // LOAD_FAST 2
    0x46,0x13, // JUMP_IF_FALSE_OR_POP 19
    0x12,0x4c, // LOAD_GLOBAL 'getattr'
    0xb2, // LOAD_FAST 2
    0x10,0x06, // LOAD_CONST_STRING '__next__'
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0x46,0x09, // JUMP_IF_FALSE_OR_POP 9
    0x12,0x4c, // LOAD_GLOBAL 'getattr'
    0xb2, // LOAD_FAST 2
    0x10,0x07, // LOAD_CONST_STRING '__len__'
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0xd3, // UNARY_OP 3 <not>
    0xca, // STORE_FAST 10
    0xb6, // LOAD_FAST 6
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x70, // POP_JUMP_IF_FALSE 48
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'ubinascii'
    0xcb, // STORE_FAST 11
    0xb6, // LOAD_FAST 6
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xcc, // STORE_FAST 12
    0xcd, // STORE_FAST 13
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x14,0x09, // LOAD_METHOD 'format'
    0xbc, // LOAD_FAST 12
    0xbd, // LOAD_FAST 13
    0x36,0x02, // CALL_METHOD 2
    0xce, // STORE_FAST 14
    0x12,0x4d, // LOAD_GLOBAL 'str'
    0xbb, // LOAD_FAST 11
    0x14,0x0a, // LOAD_METHOD 'b2a_base64'
    0xbe, // LOAD_FAST 14
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x10,0x0b, // LOAD_CONST_STRING 'ascii'
    0x34,0x02, // CALL_FUNCTION 2
    0xce, // STORE_FAST 14
    0x10,0x0c, // LOAD_CONST_STRING 'Basic {}'
    0x14,0x09, // LOAD_METHOD 'format'
    0xbe, // LOAD_FAST 14
    0x36,0x01, // CALL_METHOD 1
    0xb4, // LOAD_FAST 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x56, // STORE_SUBSCR
    0x48,0x13, // SETUP_EXCEPT 19
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'split'
    0x10,0x0e, // LOAD_CONST_STRING '/'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x02, // CALL_METHOD 2
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x26,0x12, // STORE_FAST_N 18
    0x4a,0x1d, // POP_EXCEPT_JUMP 29
    0x57, // DUP_TOP
    0x12,0x4e, // LOAD_GLOBAL 'ValueError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'split'
    0x10,0x0e, // LOAD_CONST_STRING '/'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x10,0x0f, // LOAD_CONST_STRING ''
    0x26,0x12, // STORE_FAST_N 18
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xbf, // LOAD_FAST 15
    0x10,0x10, // LOAD_CONST_STRING 'http:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0x26,0x13, // STORE_FAST_N 19
    0x42,0x5c, // JUMP 28
    0xbf, // LOAD_FAST 15
    0x10,0x11, // LOAD_CONST_STRING 'https:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x12, // IMPORT_NAME 'ussl'
    0x26,0x14, // STORE_FAST_N 20
    0x22,0x83,0x3b, // LOAD_CONST_SMALL_INT 443
    0x26,0x13, // STORE_FAST_N 19
    0x42,0x49, // JUMP 9
    0x12,0x4e, // LOAD_GLOBAL 'ValueError'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xbf, // LOAD_FAST 15
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x10,0x13, // LOAD_CONST_STRING ':'
    0x24,0x11, // LOAD_FAST_N 17
    0xdd, // BINARY_OP 6 <in>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0x24,0x11, // LOAD_FAST_N 17
    0x14,0x0d, // LOAD_METHOD 'split'
    0x10,0x13, // LOAD_CONST_STRING ':'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0x26,0x11, // STORE_FAST_N 17
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x4f, // LOAD_GLOBAL 'int'
    0x24,0x13, // LOAD_FAST_N 19
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x02, // LOAD_GLOBAL 'usocket'
    0x14,0x14, // LOAD_METHOD 'getaddrinfo'
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x13, // LOAD_FAST_N 19
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x02, // LOAD_GLOBAL 'usocket'
    0x13,0x15, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0x26,0x15, // STORE_FAST_N 21
    0x24,0x15, // LOAD_FAST_N 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x26,0x15, // STORE_FAST_N 21
    0x51, // LOAD_CONST_NONE
    0x26,0x16, // STORE_FAST_N 22
    0xb8, // LOAD_FAST 8
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x2c,0x00, // BUILD_MAP 0
    0x26,0x16, // STORE_FAST_N 22
    0x12,0x02, // LOAD_GLOBAL 'usocket'
    0x14,0x16, // LOAD_METHOD 'socket'
    0x24,0x15, // LOAD_FAST_N 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x02, // LOAD_GLOBAL 'usocket'
    0x13,0x15, // LOAD_ATTR 'SOCK_STREAM'
    0x24,0x15, // LOAD_FAST_N 21
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x36,0x03, // CALL_METHOD 3
    0x26,0x17, // STORE_FAST_N 23
    0xb7, // LOAD_FAST 7
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x17, // LOAD_METHOD 'settimeout'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x48,0x93,0x04, // SETUP_EXCEPT 531
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x18, // LOAD_METHOD 'connect'
    0x24,0x15, // LOAD_FAST_N 21
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xbf, // LOAD_FAST 15
    0x10,0x11, // LOAD_CONST_STRING 'https:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x24,0x14, // LOAD_FAST_N 20
    0x14,0x19, // LOAD_METHOD 'wrap_socket'
    0x24,0x17, // LOAD_FAST_N 23
    0x10,0x1a, // LOAD_CONST_STRING 'server_hostname'
    0x24,0x11, // LOAD_FAST_N 17
    0x36,0x82,0x01, // CALL_METHOD 257
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb0, // LOAD_FAST 0
    0x24,0x12, // LOAD_FAST_N 18
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x10,0x1c, // LOAD_CONST_STRING 'Host'
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x24,0x11, // LOAD_FAST_N 17
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x5f, // GET_ITER_STACK
    0x4b,0x2a, // FOR_ITER 42
    0x26,0x18, // STORE_FAST_N 24
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x24,0x18, // LOAD_FAST_N 24
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0xb4, // LOAD_FAST 4
    0x24,0x18, // LOAD_FAST_N 24
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x14, // JUMP -44
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x5f, // POP_JUMP_IF_FALSE 31
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x1d, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x1e, // IMPORT_NAME 'ujson'
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x19, // LOAD_FAST_N 25
    0x14,0x1f, // LOAD_METHOD 'dumps'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x44,0x5d, // POP_JUMP_IF_FALSE 29
    0xba, // LOAD_FAST 10
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x4f, // JUMP 15
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x12,0x50, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x44,0xc0,0x80, // POP_JUMP_IF_FALSE 64
    0xba, // LOAD_FAST 10
    0x44,0x75, // POP_JUMP_IF_FALSE 53
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x26, // FOR_ITER 38
    0x26,0x1a, // STORE_FAST_N 26
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x12,0x50, // LOAD_GLOBAL 'len'
    0x24,0x1a, // LOAD_FAST_N 26
    0x34,0x01, // CALL_FUNCTION 1
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x24,0x1a, // LOAD_FAST_N 26
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x18, // JUMP -40
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0x10,0x20, // LOAD_CONST_STRING '0\r\n\r\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x48, // JUMP 8
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1b, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x21, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x0d, // LOAD_METHOD 'split'
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x26,0x1b, // STORE_FAST_N 27
    0x12,0x50, // LOAD_GLOBAL 'len'
    0x24,0x1b, // LOAD_FAST_N 27
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x4e, // LOAD_GLOBAL 'ValueError'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x24,0x1b, // LOAD_FAST_N 27
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x4f, // LOAD_GLOBAL 'int'
    0x24,0x1b, // LOAD_FAST_N 27
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x1c, // STORE_FAST_N 28
    0x10,0x0f, // LOAD_CONST_STRING ''
    0x26,0x1d, // STORE_FAST_N 29
    0x12,0x50, // LOAD_GLOBAL 'len'
    0x24,0x1b, // LOAD_FAST_N 27
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x1b, // LOAD_FAST_N 27
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x14,0x22, // LOAD_METHOD 'rstrip'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1d, // STORE_FAST_N 29
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x21, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x1b, // LOAD_FAST_N 27
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x24,0x1b, // LOAD_FAST_N 27
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x42,0xb2,0x81, // JUMP 178
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x23, // LOAD_METHOD 'startswith'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x36,0x01, // CALL_METHOD 1
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x24,0x1b, // LOAD_FAST_N 27
    0xdd, // BINARY_OP 6 <in>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x4e, // LOAD_GLOBAL 'ValueError'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x12,0x4d, // LOAD_GLOBAL 'str'
    0x24,0x1b, // LOAD_FAST_N 27
    0x10,0x24, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x42,0xd1,0x80, // JUMP 81
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x23, // LOAD_METHOD 'startswith'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x36,0x01, // CALL_METHOD 1
    0x44,0xc6,0x80, // POP_JUMP_IF_FALSE 70
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0x24,0x1c, // LOAD_FAST_N 28
    0x57, // DUP_TOP
    0x5b, // ROT_THREE
    0xda, // BINARY_OP 3 __le__
    0x46,0x06, // JUMP_IF_FALSE_OR_POP 6
    0x22,0x82,0x2b, // LOAD_CONST_SMALL_INT 299
    0xda, // BINARY_OP 3 __le__
    0x42,0x42, // JUMP 2
    0x5a, // ROT_TWO
    0x59, // POP_TOP
    0x43,0x72, // POP_JUMP_IF_TRUE 50
    0x24,0x1c, // LOAD_FAST_N 28
    0x22,0x82,0x2d, // LOAD_CONST_SMALL_INT 301
    0x22,0x82,0x2e, // LOAD_CONST_SMALL_INT 302
    0x22,0x82,0x2f, // LOAD_CONST_SMALL_INT 303
    0x22,0x82,0x33, // LOAD_CONST_SMALL_INT 307
    0x22,0x82,0x34, // LOAD_CONST_SMALL_INT 308
    0x2b,0x05, // BUILD_LIST 5
    0xdd, // BINARY_OP 6 <in>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x4d, // LOAD_GLOBAL 'str'
    0x24,0x1b, // LOAD_FAST_N 27
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x7e, // LOAD_CONST_SMALL_INT -2
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x10,0x24, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0xc9, // STORE_FAST 9
    0x42,0x4a, // JUMP 10
    0x12,0x51, // LOAD_GLOBAL 'NotImplementedError'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x24,0x1c, // LOAD_FAST_N 28
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x42,0x40, // JUMP 0
    0xb8, // LOAD_FAST 8
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x73, // JUMP 51
    0xb8, // LOAD_FAST 8
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x12,0x4d, // LOAD_GLOBAL 'str'
    0x24,0x1b, // LOAD_FAST_N 27
    0x10,0x24, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x0d, // LOAD_METHOD 'split'
    0x10,0x13, // LOAD_CONST_STRING ':'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0x26,0x18, // STORE_FAST_N 24
    0x26,0x1e, // STORE_FAST_N 30
    0x24,0x1e, // LOAD_FAST_N 30
    0x14,0x25, // LOAD_METHOD 'strip'
    0x36,0x00, // CALL_METHOD 0
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x18, // LOAD_FAST_N 24
    0x56, // STORE_SUBSCR
    0x42,0x48, // JUMP 8
    0xb8, // LOAD_FAST 8
    0x24,0x1b, // LOAD_FAST_N 27
    0x24,0x16, // LOAD_FAST_N 22
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0xb8,0x7e, // JUMP -200
    0x4a,0x10, // POP_EXCEPT_JUMP 16
    0x57, // DUP_TOP
    0x12,0x52, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x64, // RAISE_LAST
    0x5d, // END_FINALLY
    0xb9, // LOAD_FAST 9
    0x44,0x70, // POP_JUMP_IF_FALSE 48
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x24,0x1c, // LOAD_FAST_N 28
    0x22,0x82,0x2d, // LOAD_CONST_SMALL_INT 301
    0x22,0x82,0x2e, // LOAD_CONST_SMALL_INT 302
    0x22,0x82,0x2f, // LOAD_CONST_SMALL_INT 303
    0x2b,0x03, // BUILD_LIST 3
    0xdd, // BINARY_OP 6 <in>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x27, // LOAD_CONST_STRING 'GET'
    0xb9, // LOAD_FAST 9
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x34,0x06, // CALL_FUNCTION 6
    0x63, // RETURN_VALUE
    0x12,0x05, // LOAD_GLOBAL 'request'
    0xb0, // LOAD_FAST 0
    0xb9, // LOAD_FAST 9
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x34,0x06, // CALL_FUNCTION 6
    0x63, // RETURN_VALUE
    0x42,0x64, // JUMP 36
    0x12,0x03, // LOAD_GLOBAL 'Response'
    0x24,0x17, // LOAD_FAST_N 23
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x1f, // STORE_FAST_N 31
    0x24,0x1c, // LOAD_FAST_N 28
    0x24,0x1f, // LOAD_FAST_N 31
    0x18,0x28, // STORE_ATTR 'status_code'
    0x24,0x1d, // LOAD_FAST_N 29
    0x24,0x1f, // LOAD_FAST_N 31
    0x18,0x29, // STORE_ATTR 'reason'
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x1f, // LOAD_FAST_N 31
    0x18,0x2a, // STORE_ATTR 'headers'
    0x24,0x1f, // LOAD_FAST_N 31
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_request = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 9,
    .fun_data = fun_data_urequests_request,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 1024,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 41,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 9,
        .n_kwonly_args = 0,
        .n_def_pos_args = 7,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_urequests_request + 16,
        .line_info_top = fun_data_urequests_request + 115,
        .opcodes = fun_data_urequests_request + 115,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_head
static const byte fun_data_urequests_head[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x2b,0x46, // names: head, url
    0x80,0xc6, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x2c, // LOAD_CONST_STRING 'HEAD'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_head = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_head,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_urequests_head + 6,
        .line_info_top = fun_data_urequests_head + 8,
        .opcodes = fun_data_urequests_head + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_get
static const byte fun_data_urequests_get[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x2d,0x46, // names: get, url
    0x80,0xca, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x27, // LOAD_CONST_STRING 'GET'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_get = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_get,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_urequests_get + 6,
        .line_info_top = fun_data_urequests_get + 8,
        .opcodes = fun_data_urequests_get + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_post
static const byte fun_data_urequests_post[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x2e,0x46, // names: post, url
    0x80,0xce, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x2f, // LOAD_CONST_STRING 'POST'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_post = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_post,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 46,
        .line_info = fun_data_urequests_post + 6,
        .line_info_top = fun_data_urequests_post + 8,
        .opcodes = fun_data_urequests_post + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_put
static const byte fun_data_urequests_put[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x30,0x46, // names: put, url
    0x80,0xd2, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x31, // LOAD_CONST_STRING 'PUT'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_put = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_put,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 48,
        .line_info = fun_data_urequests_put + 6,
        .line_info_top = fun_data_urequests_put + 8,
        .opcodes = fun_data_urequests_put + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_patch
static const byte fun_data_urequests_patch[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x32,0x46, // names: patch, url
    0x80,0xd6, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x33, // LOAD_CONST_STRING 'PATCH'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_patch = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_patch,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 50,
        .line_info = fun_data_urequests_patch + 6,
        .line_info_top = fun_data_urequests_patch + 8,
        .opcodes = fun_data_urequests_patch + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests_delete
static const byte fun_data_urequests_delete[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x34,0x46, // names: delete, url
    0x80,0xda, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x35, // LOAD_CONST_STRING 'DELETE'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code_urequests_delete = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x02,
    .n_pos_args = 1,
    .fun_data = fun_data_urequests_delete,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 20,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 52,
        .line_info = fun_data_urequests_delete + 6,
        .line_info_top = fun_data_urequests_delete + 8,
        .opcodes = fun_data_urequests_delete + 8,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const mp_raw_code_t *const children_urequests__lt_module_gt_[] = {
    &raw_code_urequests_Response,
    &raw_code_urequests_request,
    &raw_code_urequests_head,
    &raw_code_urequests_get,
    &raw_code_urequests_post,
    &raw_code_urequests_put,
    &raw_code_urequests_patch,
    &raw_code_urequests_delete,
};

static const mp_raw_code_t raw_code_urequests__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data_urequests__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 76,
    #endif
    .children = (void *)&children_urequests__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 8,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_urequests__lt_module_gt_ + 3,
        .line_info_top = fun_data_urequests__lt_module_gt_ + 20,
        .opcodes = fun_data_urequests__lt_module_gt_ + 20,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data_urequests[89] = {
    MP_QSTR_urequests_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_usocket,
    MP_QSTR_Response,
    MP_QSTR_wb,
    MP_QSTR_request,
    MP_QSTR___next__,
    MP_QSTR___len__,
    MP_QSTR_ubinascii,
    MP_QSTR_format,
    MP_QSTR_b2a_base64,
    MP_QSTR_ascii,
    MP_QSTR_Basic_space__brace_open__brace_close_,
    MP_QSTR_split,
    MP_QSTR__slash_,
    MP_QSTR_,
    MP_QSTR_http_colon_,
    MP_QSTR_https_colon_,
    MP_QSTR_ussl,
    MP_QSTR__colon_,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_socket,
    MP_QSTR_settimeout,
    MP_QSTR_connect,
    MP_QSTR_wrap_socket,
    MP_QSTR_server_hostname,
    MP_QSTR_write,
    MP_QSTR_Host,
    MP_QSTR_AssertionError,
    MP_QSTR_ujson,
    MP_QSTR_dumps,
    MP_QSTR_0_0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_readline,
    MP_QSTR_rstrip,
    MP_QSTR_startswith,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR_strip,
    MP_QSTR_close,
    MP_QSTR_GET,
    MP_QSTR_status_code,
    MP_QSTR_reason,
    MP_QSTR_headers,
    MP_QSTR_head,
    MP_QSTR_HEAD,
    MP_QSTR_get,
    MP_QSTR_post,
    MP_QSTR_POST,
    MP_QSTR_put,
    MP_QSTR_PUT,
    MP_QSTR_patch,
    MP_QSTR_PATCH,
    MP_QSTR_delete,
    MP_QSTR_DELETE,
    MP_QSTR___init__,
    MP_QSTR_raw,
    MP_QSTR_encoding,
    MP_QSTR__cached,
    MP_QSTR_generate,
    MP_QSTR_read,
    MP_QSTR_save,
    MP_QSTR_content,
    MP_QSTR_text,
    MP_QSTR_json,
    MP_QSTR_loads,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_property,
    MP_QSTR_method,
    MP_QSTR_url,
    MP_QSTR_data,
    MP_QSTR_stream,
    MP_QSTR_auth,
    MP_QSTR_timeout,
    MP_QSTR_parse_headers,
    MP_QSTR_getattr,
    MP_QSTR_str,
    MP_QSTR_ValueError,
    MP_QSTR_int,
    MP_QSTR_len,
    MP_QSTR_NotImplementedError,
    MP_QSTR_OSError,
    MP_QSTR_self,
    MP_QSTR_f,
    MP_QSTR_chunk_size,
    MP_QSTR_filename,
    MP_QSTR_mode,
    MP_QSTR_open,
};

// constants
static const mp_obj_str_t const_obj_urequests_0 = {{&mp_type_bytes}, 14943, 5, (const byte*)"\x7b\x7d\x3a\x7b\x7d"};
static const mp_obj_str_t const_obj_urequests_3 = {{&mp_type_bytes}, 47477, 17, (const byte*)"\x25\x73\x20\x2f\x25\x73\x20\x48\x54\x54\x50\x2f\x31\x2e\x30\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_4 = {{&mp_type_bytes}, 35534, 10, (const byte*)"\x48\x6f\x73\x74\x3a\x20\x25\x73\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_5 = {{&mp_type_bytes}, 26975, 2, (const byte*)"\x3a\x20"};
static const mp_obj_str_t const_obj_urequests_6 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_7 = {{&mp_type_bytes}, 14729, 32, (const byte*)"\x43\x6f\x6e\x74\x65\x6e\x74\x2d\x54\x79\x70\x65\x3a\x20\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e\x2f\x6a\x73\x6f\x6e\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_8 = {{&mp_type_bytes}, 16551, 28, (const byte*)"\x54\x72\x61\x6e\x73\x66\x65\x72\x2d\x45\x6e\x63\x6f\x64\x69\x6e\x67\x3a\x20\x63\x68\x75\x6e\x6b\x65\x64\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_9 = {{&mp_type_bytes}, 8065, 20, (const byte*)"\x43\x6f\x6e\x74\x65\x6e\x74\x2d\x4c\x65\x6e\x67\x74\x68\x3a\x20\x25\x64\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_10 = {{&mp_type_bytes}, 54399, 21, (const byte*)"\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x3a\x20\x63\x6c\x6f\x73\x65\x0d\x0a\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_11 = {{&mp_type_bytes}, 50079, 4, (const byte*)"\x25\x78\x0d\x0a"};
static const mp_obj_str_t const_obj_urequests_12 = {{&mp_type_str}, 62148, 29, (const byte*)"\x48\x54\x54\x50\x20\x65\x72\x72\x6f\x72\x3a\x20\x42\x61\x64\x53\x74\x61\x74\x75\x73\x4c\x69\x6e\x65\x3a\x0a\x25\x73"};
static const mp_obj_str_t const_obj_urequests_13 = {{&mp_type_bytes}, 59642, 18, (const byte*)"\x54\x72\x61\x6e\x73\x66\x65\x72\x2d\x45\x6e\x63\x6f\x64\x69\x6e\x67\x3a"};
static const mp_obj_str_t const_obj_urequests_14 = {{&mp_type_bytes}, 3583, 7, (const byte*)"\x63\x68\x75\x6e\x6b\x65\x64"};
static const mp_obj_str_t const_obj_urequests_16 = {{&mp_type_bytes}, 1090, 9, (const byte*)"\x4c\x6f\x63\x61\x74\x69\x6f\x6e\x3a"};
static const mp_obj_str_t const_obj_urequests_17 = {{&mp_type_str}, 18157, 29, (const byte*)"\x52\x65\x64\x69\x72\x65\x63\x74\x20\x25\x64\x20\x6e\x6f\x74\x20\x79\x65\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_urequests[18] = {
    MP_ROM_PTR(&const_obj_urequests_0),
    MP_ROM_QSTR(MP_QSTR_Authorization),
    MP_ROM_QSTR(MP_QSTR_Unsupported_space_protocol_colon__space_),
    MP_ROM_PTR(&const_obj_urequests_3),
    MP_ROM_PTR(&const_obj_urequests_4),
    MP_ROM_PTR(&const_obj_urequests_5),
    MP_ROM_PTR(&const_obj_urequests_6),
    MP_ROM_PTR(&const_obj_urequests_7),
    MP_ROM_PTR(&const_obj_urequests_8),
    MP_ROM_PTR(&const_obj_urequests_9),
    MP_ROM_PTR(&const_obj_urequests_10),
    MP_ROM_PTR(&const_obj_urequests_11),
    MP_ROM_PTR(&const_obj_urequests_12),
    MP_ROM_PTR(&const_obj_urequests_13),
    MP_ROM_PTR(&const_obj_urequests_14),
    MP_ROM_QSTR(MP_QSTR_Unsupported_space_),
    MP_ROM_PTR(&const_obj_urequests_16),
    MP_ROM_PTR(&const_obj_urequests_17),
};

static const mp_frozen_module_t frozen_module_urequests = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_urequests,
        .obj_table = (mp_obj_t *)&const_obj_table_data_urequests,
    },
    .rc = &raw_code_urequests__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// collection of all frozen modules

const char mp_frozen_names[] = {
    #ifdef MP_FROZEN_STR_NAMES
    MP_FROZEN_STR_NAMES
    #endif
    "asyncio/__init__.py\0"
    "asyncio/core.py\0"
    "asyncio/event.py\0"
    "asyncio/funcs.py\0"
    "asyncio/lock.py\0"
    "asyncio/stream.py\0"
    "uasyncio.py\0"
    "world.py\0"
    "tulip.py\0"
    "upysh.py\0"
    "sequencer.py\0"
    "utarfile.py\0"
    "music.py\0"
    "_boot.py\0"
    "alles.py\0"
    "urequests.py\0"
    "\0"
};

const mp_frozen_module_t *const mp_frozen_mpy_content[] = {
    &frozen_module_asyncio___init__,
    &frozen_module_asyncio_core,
    &frozen_module_asyncio_event,
    &frozen_module_asyncio_funcs,
    &frozen_module_asyncio_lock,
    &frozen_module_asyncio_stream,
    &frozen_module_uasyncio,
    &frozen_module_world,
    &frozen_module_tulip,
    &frozen_module_upysh,
    &frozen_module_sequencer,
    &frozen_module_utarfile,
    &frozen_module_music,
    &frozen_module__boot,
    &frozen_module_alles,
    &frozen_module_urequests,
};

#ifdef MICROPY_FROZEN_LIST_ITEM
MICROPY_FROZEN_LIST_ITEM("asyncio", "asyncio/__init__.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/core", "asyncio/core.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/event", "asyncio/event.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/funcs", "asyncio/funcs.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/lock", "asyncio/lock.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/stream", "asyncio/stream.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio", "uasyncio.py")
MICROPY_FROZEN_LIST_ITEM("world", "world.py")
MICROPY_FROZEN_LIST_ITEM("tulip", "tulip.py")
MICROPY_FROZEN_LIST_ITEM("upysh", "upysh.py")
MICROPY_FROZEN_LIST_ITEM("sequencer", "sequencer.py")
MICROPY_FROZEN_LIST_ITEM("utarfile", "utarfile.py")
MICROPY_FROZEN_LIST_ITEM("music", "music.py")
MICROPY_FROZEN_LIST_ITEM("_boot", "_boot.py")
MICROPY_FROZEN_LIST_ITEM("alles", "alles.py")
MICROPY_FROZEN_LIST_ITEM("urequests", "urequests.py")
#endif

/*
byte sizes:
qstr content: 780 unique, 8921 bytes
bc content: 20551
const str content: 1963
const int content: 4
const obj content: 1936
const table qstr content: 0 entries, 0 bytes
const table ptr content: 202 entries, 808 bytes
raw code content: 260 * 4 = 4160
mp_frozen_mpy_names_content: 209
mp_frozen_mpy_content_size: 64
total: 38616
*/
