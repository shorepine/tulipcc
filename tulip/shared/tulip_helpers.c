// tulip_helpers.c

#include "tulip_helpers.h"

#if !defined(AMYBOARD) && !defined(AMYBOARD_WEB)
#include "ui.h"
extern uint8_t keyboard_send_keys_to_micropython;
extern int8_t keyboard_grab_ui_focus;
#ifdef __EMSCRIPTEN__
extern int mp_js_repl_process_char(int);
#endif
void tx_char(int c) {
        if(keyboard_send_keys_to_micropython) {
            ringbuf_put(&stdin_ringbuf, c);
            #ifdef __EMSCRIPTEN__
                mp_js_repl_process_char(c);
            #endif
        } 
}

int check_rx_char() {
    int c = -1;
    c = ringbuf_get(&stdin_ringbuf);
    return c;
}
#endif

uint64_t get_stack_pointer() {
    char dummy[64];
    uint64_t val = (uint64_t)dummy;
    return val;
}


// Returns 0 if file doesn't exist, 2 if it's a file, XXX if it's a directory
uint8_t file_exists(const char *filename) {
    uint8_t response = mp_vfs_import_stat(filename);
    return response;
}

int32_t file_size(const char *filename) {
    if(file_exists(filename)) { 
        mp_obj_t m_args[1];
        m_args[0] = mp_obj_new_str(filename, strlen(filename));
        mp_obj_t stats_tuple = mp_vfs_stat(m_args[0]);
        // TODO , file not found response ??? 
        mp_obj_t *stats_items;
        mp_obj_get_array_fixed_n(stats_tuple, 10, &stats_items);
        return mp_obj_get_int(stats_items[6]);
    } else {
        return -1;
    }
}



// if len < 0, read the whole thing
uint32_t read_file(const char *filename, uint8_t *buf, int32_t len, uint8_t binary) {
    if(len<0) {
    	len = file_size(filename);
    }
    mp_obj_t m_args[2];
    m_args[0] = mp_obj_new_str(filename, strlen(filename));
    if(binary) {
	    m_args[1] = mp_obj_new_str("rb",2);
	} else {
	    m_args[1] = mp_obj_new_str("r",1);		
	}
    mp_obj_t file = mp_vfs_open(2, &m_args[0], (mp_map_t *)&mp_const_empty_map);
    int errcode;
    size_t bytes_read = mp_stream_rw(file, buf, len, &errcode, MP_STREAM_RW_READ | MP_STREAM_RW_ONCE);
    mp_stream_close(file);
    return bytes_read;
}

// overwrites if exists
uint32_t write_file(const char *filename, uint8_t *buf, uint32_t len, uint8_t binary) {
    mp_obj_t m_args[2];
    m_args[0] = mp_obj_new_str(filename, strlen(filename)+1);
    if(binary) {
	    m_args[1] = mp_obj_new_str("wb\0",3);
	} else {
	    m_args[1] = mp_obj_new_str("w\0",2);		
	}
    mp_obj_t file = mp_vfs_open(2, &m_args[0], (mp_map_t *)&mp_const_empty_map);
    int errcode;
    size_t bytes_written = mp_stream_rw(file, buf, len, &errcode, MP_STREAM_RW_WRITE | MP_STREAM_RW_ONCE);
    mp_stream_close(file);
    return bytes_written;
}

/*
ssize_t mp_stream_posix_write(void *stream, const void *buf, size_t len);
ssize_t mp_stream_posix_read(void *stream, void *buf, size_t len);
off_t mp_stream_posix_lseek(void *stream, off_t offset, int whence);
int mp_stream_posix_fsync(void *stream);
*/

void tulip_fclose(mp_obj_t file) {
    mp_stream_close(file);
}

mp_obj_t tulip_fopen(const char *filename, const char *mode) {
    mp_obj_t m_args[2];
    m_args[0] = mp_obj_new_str(filename, strlen(filename));
    m_args[1] = mp_obj_new_str(mode,strlen(mode));
    mp_obj_t file = mp_vfs_open(2, &m_args[0], (mp_map_t *)&mp_const_empty_map);
    return file;    
}

uint32_t tulip_fwrite(mp_obj_t file, uint8_t * buf, uint32_t len) {
    int errcode;
    size_t bytes_written = mp_stream_rw(file, buf, len, &errcode, MP_STREAM_RW_WRITE | MP_STREAM_RW_ONCE);
    return bytes_written;
}

uint32_t tulip_fread(mp_obj_t file, uint8_t * buf, uint32_t len) {
    int errcode;
    size_t bytes_read = mp_stream_rw(file, buf, len, &errcode, MP_STREAM_RW_READ | MP_STREAM_RW_ONCE);
    return bytes_read;
}

uint32_t tulip_fseek(mp_obj_t file, uint32_t seekpoint, int32_t whence) {
    #ifdef __EMSCRIPTEN__
    return 0;
    #else
    return mp_stream_posix_lseek(file, seekpoint, whence);
    #endif
}

int32_t tulip_getline(char * line, uint32_t * len, mp_obj_t file ) {
    // tulip_fread a char at a time until a \n
    uint8_t buf[1];
    int32_t rbytes = 0;
    for(uint8_t i=0;i<255;i++) {
        uint32_t bytes = tulip_fread(file, buf, 1);
        rbytes += bytes;
        if(bytes == 0) { // eof
            line[i] = 0;
            return rbytes;
        }
        if(buf[0] == '\n') {
            line[i] = 0;
            return rbytes;
        }
        line[i] = buf[0];
        *len = *len + 1;
    }
    line[254] = 0;
    return rbytes;
}


