// editor.c
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef LOCALDEV
#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#else // esp32
#include "tulip_helpers.h"
#include "display.h"
#endif

// shared vars for editor

char * text;
char ** text_lines;
uint8_t *saved_tfb;
uint8_t *saved_tfbf;
uint16_t saved_tfb_y;
uint16_t saved_tfb_x;
uint8_t quit_flag = 0;

uint16_t file_y = 0;
uint16_t file_x = 0;
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;



// for local dev of the editor
#ifdef LOCALDEV
#define TFB_ROWS 50 
#define TFB_COLS 128 
uint8_t TFB[TFB_ROWS*TFB_COLS];
uint8_t TFBf[TFB_ROWS*TFB_COLS];
uint16_t tfb_y_row =0;
uint16_t tfb_x_col = 0;
FILE * elog;
#define check_rx_char getch

uint32_t file_size(const char *filename) {
	FILE * f = fopen(filename, "r");
	fseek (f, 0, SEEK_END);
	uint32_t file_size = ftell (f);
	fclose(f);
	return file_size;

}
uint32_t read_file(const char * filename, char * data, int32_t file_size, uint8_t binary) {
	// if file_size <0 read the whole thing 
	FILE * f;
	if(binary) {
		f = fopen(filename,"rb");
	} else {
		f = fopen(filename, "r");
	}
	if(file_size < 0) {
		fseek (f, 0, SEEK_END);
		file_size = ftell (f);
  		fseek (f, 0, SEEK_SET);
  	}
    uint32_t s = fread(data, 1, file_size, f);
	fclose(f);
	return s;
}

void setup_display() {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	memset(TFB, 0, TFB_ROWS*TFB_COLS);
	memset(TFBf, 0, TFB_ROWS*TFB_COLS);
}

void update_tfb() {
	for(uint8_t y=0;y<TFB_ROWS;y++) {
		for(uint8_t x=0;x<TFB_COLS;x++) {
			if(TFB[y*TFB_COLS+x] > 32) {
				mvaddch(y, x, TFB[y*TFB_COLS + x]);
			}
		}
	}
	refresh();
}
#endif


void dbgprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
#ifdef LOCALDEV
    vfprintf(elog, fmt, args);
    fflush(elog);
#else
    vfprintf(stderr, fmt, args);
#endif
    va_end(args);
}



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

void string_at_row(char * s, uint16_t len, uint16_t y) {
	for(uint16_t i=0;i<len;i++) {
		if(s[i] > 32) {
			TFB[y*TFB_COLS+i] = s[i];
		}
	}
#ifdef LOCALDEV
	update_tfb();
#endif
}

void open_file(const char *filename) {
	uint16_t lines =0 ;
	uint16_t c = 0;
	dbgprintf("opening file %s\n", filename);
	uint32_t fs = file_size(filename);
	text = (char*) malloc(fs+2);
	uint32_t bytes_read = read_file(filename, (uint8_t*)text, fs, 0);
	//dbgprintf("%s\n", text);
	text[fs-1] = 0;
	if(bytes_read) lines =1; // at least one line
	for(uint32_t i=0;i<bytes_read;i++) if(text[i]=='\n') lines++;
	text_lines = (char**)malloc(sizeof(char*)*lines);
	text_lines[c++] = text;
	uint16_t last = 0;
	for(uint32_t i=0;i<bytes_read-1;i++) {
		if(text[i]=='\n') {
			string_at_row(text_lines[c-1], i-last, c-1 );
			last = i+1;
			text_lines[c++] = &text[last];
		}
	}
}

void editor_quit() {
	restore_tfb();
	free(text);
#ifdef LOCALDEV
	endwin();
#endif
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
	//display_tfb_str("Tulip Editor v2\n", strlen("Tulip Editor v2\n"), FORMAT_BOLD|1|ANSI_BOLD_WHITE);
	if(filename != NULL) { 
		dbgprintf("editor fn is %s\n", filename);
		open_file(filename);
	} else {
		dbgprintf("no filename given\n");
	}

	while(quit_flag == 0) {
		int c = check_rx_char();
		if(c>=0) {
			process_char(c);
		}
#ifdef LOCALDEV
		update_tfb();
#else
		vPortYield();
#endif
	}	
	editor_quit();
}


#ifdef LOCALDEV
int main(int argc , char*argv[]) {
	elog = fopen("edit.log", "a");
	setup_display();
	if(argc>1) editor(argv[1]);
	return 0;
}
#endif

