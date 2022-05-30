// editor.c
#include <stddef.h>
#include <stdio.h>
#include "tulip_helpers.h"
#include "display.h"

uint8_t * text;

uint8_t *saved_tfb;
uint8_t *saved_tfbf;
uint16_t saved_tfb_y;
uint16_t saved_tfb_x;
uint8_t quit_flag = 0;

uint16_t file_y = 0;
uint16_t file_x = 0;

void save_tfb() {
	saved_tfb = malloc(TFB_ROWS*TFB_COLS);
	saved_tfbf= malloc(TFB_ROWS*TFB_COLS);
	for(uint16_t y=0;y<TFB_ROWS*TFB_COLS;y++) {
		saved_tfb[y] = TFB[y];
		saved_tfbf[y]= TFBf[y];
		TFB[y] = 0;
		TFBf[y] = 0;
	}
	saved_tfb_y = tfb_y_row;
	saved_tfb_x = tfb_x_col;
	tfb_y_row = 0;
	tfb_x_col = 0;
}
void restore_tfb() {
	for(uint16_t y=0;y<TFB_ROWS*TFB_COLS;y++) {
		TFB[y] = saved_tfb[y];
		TFBf[y] = saved_tfbf[y];
	}
	free(saved_tfb);
	free(saved_tfbf);
	tfb_y_row = saved_tfb_y;
	tfb_x_col = saved_tfb_x;
}

void open_file(const char *filename) {
	printf("opening file %s\n", filename);
	uint32_t fs = file_size(filename);
	text = (uint8_t*) malloc(fs+1);
	uint32_t bytes_read = read_file(filename, text, fs, 0);
	text[fs-1] = 0;
	display_tfb_str((char*)text, bytes_read, tfb_pal_color);
}

void editor_quit() {
	restore_tfb();
	free(text);
}

void editor_backspace() {

}
void editor_tab() {

}
void editor_crlf() {

}
void editor_yank() {

}
void editor_unyank() {

}
void editor_left() {

}
void editor_right() {

}
void editor_up() {

}
void editor_down() {

}
void editor_insert_character(int c) {

}

void process_char(int c) {
	printf("%d\n", c);
	if(c==127) { // backspace
		editor_backspace();
	} else if(c == 9) { // tab, control-I
		editor_tab();
	} else if(c==10 || c == 13) { // CRLF
		editor_crlf();
	} else if(c == 3) {  // control-C 
		// TODO should show line number
	} else if(c ==11) { // control-K, yank
		editor_yank();
	} else if(c== 21) { // control-U, unyank
		editor_unyank();
	} else if(c==15) { // control-O, save as TODO
		// TODO 
	} else if(c==23) { // control-W, "where is" aka search TODO 
		// TODO 
	} else if(c==24) { // control-X, save 
		// TODO save too
		quit_flag = 1;
	} else if(c == 1) { // control-A, start of line
		// TODO
	} else if(c == 5) { // control-E, end of line
		// TODO
	} else if(c==25) { // control Y, page up
		for(uint8_t i=0;i<TFB_ROWS-10;i++) editor_up();
	} else if(c==22) { // control V, page down 
		for(uint8_t i=0;i<TFB_ROWS-10;i++) editor_down();
	} else if(c==27) { // ansi code, up / down / left / right / forward delete
		char s = (char)check_rx_char(); // Skip the [
		s = (char)check_rx_char(); // Get the code
		if(s == 'D') { //left 
			editor_left();
		} else if(s=='C') { // right
			editor_right();
		} else if(s=='B') { // down
			editor_down();
		} else if(s=='A') { // up
			editor_up();
		} else if(s=='3') { // forward delete
			s = (char)check_rx_char(); // skip the ~
			// AFAIK this is what forward delete is
			editor_right();
			editor_backspace();
		}
	} else if(c>31 && c<127) {
		editor_insert_character(c);
	} else {
		// Ignore unsupported keycodes...
	}
}

void editor(const char * filename) {
	save_tfb();
	display_tfb_str("Tulip Editor v2\n", strlen("Tulip Editor v2\n"), FORMAT_BOLD|1|ANSI_BOLD_WHITE);
	if(filename != NULL) { 
		printf("editor fn is %s\n", filename);
		open_file(filename);
	} else {
		printf("no filename given\n");
	}
	
	while(quit_flag == 0) {
		int c = check_rx_char();
		if(c>=0) {
			process_char(c);
		}
		vPortYield();
	}	
	editor_quit();
}

