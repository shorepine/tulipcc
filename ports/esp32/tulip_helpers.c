// tulip_helpers.c

#include "tulip_helpers.h"

extern uint8_t keyboard_send_keys_to_micropython;

void tx_char(int c) {
    if(keyboard_send_keys_to_micropython) {
        ringbuf_put(&stdin_ringbuf, c);
    }
}

int check_rx_char() {
    int c = -1;
    c = ringbuf_get(&stdin_ringbuf);
    return c;
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
    printf("Loaded file %s with %d bytes, error %d\n", filename, bytes_read, errcode);
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
    printf("Wrote file %s with %d bytes, error %d\n",filename, bytes_written, errcode);
    mp_stream_close(file);
    return bytes_written;
}

void tulip_fclose(mp_obj_t file) {
    mp_stream_close(file);
}

mp_obj_t tulip_fopen(const char *filename, const char *mode) {
    mp_obj_t m_args[2];
    printf("fopen filename %s mode %s\n", filename, mode);
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
