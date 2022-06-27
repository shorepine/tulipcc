// editor.c
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef LOCALDEV
#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#define tfb_bg_pal_color 0
#define tfb_fg_pal_color 0

#else // esp32
#include "tulip_helpers.h"
#include "display.h"
#endif

// shared vars for editor
char ** text_lines;
const char *fn;
char * yank;
uint16_t lines = 0; 
uint8_t dirty = 0;
uint8_t *saved_tfb;
uint8_t *saved_tfbf;
uint8_t *saved_tfbfg;
uint8_t *saved_tfbbg;
uint16_t saved_tfb_y;
uint16_t saved_tfb_x;
uint8_t quit_flag = 0;
uint16_t y_offset = 0;
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
#define EDITOR_TAB_SPACES 2
#define V_SCROLL_MARGIN 6


// for local dev of the editor
#ifdef LOCALDEV
#define TFB_ROWS 50 
#define TFB_COLS 128 
uint8_t TFB[TFB_ROWS*TFB_COLS];
uint8_t TFBf[TFB_ROWS*TFB_COLS];
uint8_t TFBfg[TFB_ROWS*TFB_COLS];
uint8_t TFBbg[TFB_ROWS*TFB_COLS];
uint16_t tfb_y_row =0;
uint16_t tfb_x_col = 0;
uint8_t tfb_pal_color = 15; // ANSI_BOLD_WHITE
#define FORMAT_INVERSE 0x80 
#define FORMAT_UNDERLINE 0x40
#define FORMAT_FLASH 0x20
#define FORMAT_BOLD 0x10 
FILE * elog;
#define check_rx_char getch
#define mp_hal_stdin_rx_chr getch

uint32_t file_size(const char *filename) {
	FILE * f = fopen(filename, "r");
	if(f==NULL) return 0;
	fseek (f, 0, SEEK_END);
	uint32_t file_size = ftell (f);
	fclose(f);
	return file_size;

}

uint32_t write_file(const char *filename, uint8_t *buf, uint32_t len, uint8_t binary) {
	FILE *f;
	f = fopen(filename, "w");
	uint32_t s= fwrite(buf, len, 1, f);
	fclose(f);
	return s;
}

uint32_t read_file(const char * filename, uint8_t * data, int32_t file_size, uint8_t binary) {
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
	memset(TFBfg, 0, TFB_ROWS*TFB_COLS);
	memset(TFBbg, 0, TFB_ROWS*TFB_COLS);
}

void local_draw_tfb() {
	for(uint8_t y=0;y<TFB_ROWS;y++) {
		// clear line
		move(y,0);
		clrtoeol();
		for(uint8_t x=0;x<TFB_COLS;x++) {
			if(TFB[y*TFB_COLS+x] > 31) {
				if(TFBf[y*TFB_COLS+x] & FORMAT_INVERSE) {
					attron(A_REVERSE);
				}
				mvaddch(y, x, TFB[y*TFB_COLS + x]);
				if(TFBf[y*TFB_COLS+x] & FORMAT_INVERSE) {
					attroff(A_REVERSE);
				}

			}
		}
	}
	move(cursor_y,cursor_x);
	refresh();
}
#endif

int mc=0;
int fc=0;
void * editor_malloc(uint32_t size) {
    mc++;
#ifdef LOCALDEV
	return malloc(size);
#else
	return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#endif
}


void editor_free(void* ptr) {
    fc++;
    free(ptr);
}

// Send debug out to uart on esp32 and a log file on local 
void dbg(const char *fmt, ...) {
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

void clear_row(uint16_t y) {
	if(y < TFB_ROWS) {
		for(uint16_t i=0;i<TFB_COLS;i++) {
			TFB[y*TFB_COLS+i] = 0;
		}	
	}
}
void format_at_row(uint8_t format, int16_t len, uint16_t y) {
	if(y<TFB_ROWS) {
		if(len <0) len = TFB_COLS;
		for(uint16_t i=0;i<len;i++) {
			TFBf[y*TFB_COLS+i] = format;
			TFBfg[y*TFB_COLS+i] = tfb_fg_pal_color;
			TFBbg[y*TFB_COLS+i] = tfb_bg_pal_color;
		}
	}
}
void string_at_row(char * s, int16_t len, uint16_t y) {
	if(len < 0) len=strlen(s);
	if(y<TFB_ROWS) {
		for(uint16_t i=0;i<len;i++) {
			TFB[y*TFB_COLS+i] = s[i];
			TFBf[y*TFB_COLS+i] = 0; ;
			TFBfg[y*TFB_COLS+i] = tfb_fg_pal_color;
			TFBbg[y*TFB_COLS+i] = tfb_bg_pal_color;
		}
		for(uint16_t i=len;i<TFB_COLS;i++) {
			TFB[y*TFB_COLS+i] = 0;
		}
#ifdef LOCALDEV
		local_draw_tfb();
#endif
	}
}


// (Re) paints the entire TFB
void paint_tfb(uint16_t start_at_y) {
	for(uint16_t y=start_at_y;y<TFB_ROWS-1;y++) {
		if(y_offset + y < lines) { 
			string_at_row(text_lines[y_offset+y], -1, y);
		} else {
			clear_row(y);
		}
	}
}



// Move the cursor to pos x,y and scroll the viewport if needed
void move_cursor(int16_t x, int16_t y) {
	// In localdev, the local_draw_tfb physically moves the cursor
	// Undo old cursor
	TFBf[cursor_y*TFB_COLS+cursor_x] = 0; 

	// Move viewport up/down (TFB_ROWS-1) / 2
	// Move cursor to next/prev line (which would now be in the middle of the screen)
	if(y < 0) {
		dbg("scroll up\n");
		uint16_t cursor_y_line_was = cursor_y + y_offset;
		if(y_offset > ((TFB_ROWS-1)/2)) {
			y_offset = y_offset - (TFB_ROWS-1)/2;
		} else {
			y_offset = 0;
		}
		cursor_y = (cursor_y_line_was-1) - y_offset;
		paint_tfb(0);
	} else if(y == TFB_ROWS-1) {
		dbg("scroll down\n");
		uint16_t cursor_y_line_was = cursor_y + y_offset;
		y_offset = y_offset + (TFB_ROWS-1)/2;
		cursor_y = (cursor_y_line_was+1) - y_offset;
		paint_tfb(0);
	} else {
		cursor_y = y;
	}
	// X scrolling TODO or NI, not sure yet
	if(x < 0) {
		dbg("scroll left");
	} else if(x == TFB_COLS) {
		dbg("scroll right");
	} else {
		cursor_x = x;
	}
	// Put in new cursor 
    TFBf[cursor_y*TFB_COLS+cursor_x] = FORMAT_INVERSE|FORMAT_FLASH;
    TFBfg[cursor_y*TFB_COLS+cursor_x] = tfb_fg_pal_color;
    TFBbg[cursor_y*TFB_COLS+cursor_x] = tfb_bg_pal_color;

    if(TFB[cursor_y*TFB_COLS+cursor_x]==0) TFB[cursor_y*TFB_COLS+cursor_x] = 32;

	// Update status bar
	char status[TFB_COLS];
	// TODO, padding better 
	float percent = ((float)(cursor_y+y_offset) / (float)lines) * 100.0;
	sprintf(status, "Tulip Editor v4.1      Row %04d / %04d [%02.2f%%] Col %3d  File: %s ", cursor_y+y_offset, lines,  percent, cursor_x, fn);
	string_at_row(status, strlen(status), TFB_ROWS-1);
	format_at_row(FORMAT_INVERSE, -1, TFB_ROWS-1);

}

void editor_page_up() {
	if(y_offset > (TFB_ROWS-V_SCROLL_MARGIN)) {
		y_offset = y_offset - (TFB_ROWS-V_SCROLL_MARGIN);
	} else {
		y_offset = 0;
	}
	paint_tfb(0);
	move_cursor(cursor_x, 0);
}

void editor_page_down() {
	dbg("page down y_offset is %d lines is %d\n", y_offset, lines);
	if(y_offset + (TFB_ROWS-V_SCROLL_MARGIN) < lines) {
		y_offset = y_offset + (TFB_ROWS-V_SCROLL_MARGIN);
		move_cursor(cursor_x, 0);
	} else {
		dbg("end of the file offset was %d\n", y_offset);
		move_cursor(cursor_x, lines-y_offset);
	}
	paint_tfb(0);

}

void save_tfb() {
	saved_tfb = (uint8_t*)editor_malloc(TFB_ROWS*TFB_COLS);
	saved_tfbf= (uint8_t*)editor_malloc(TFB_ROWS*TFB_COLS);
	saved_tfbfg= (uint8_t*)editor_malloc(TFB_ROWS*TFB_COLS);
	saved_tfbbg= (uint8_t*)editor_malloc(TFB_ROWS*TFB_COLS);
	for(uint16_t y=0;y<TFB_ROWS*TFB_COLS;y++) {
		saved_tfb[y] = TFB[y];
		saved_tfbf[y]= TFBf[y];
		saved_tfbfg[y]= TFBfg[y];
		saved_tfbbg[y]= TFBbg[y];
		TFB[y] = 0;
		TFBf[y] = 0;
		TFBfg[y] = tfb_fg_pal_color;
		TFBbg[y] = tfb_bg_pal_color;
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
		TFBfg[y] = saved_tfbfg[y];
		TFBbg[y] = saved_tfbbg[y];
	}
	editor_free(saved_tfb);
	editor_free(saved_tfbf);
	editor_free(saved_tfbfg);
	editor_free(saved_tfbbg);
	tfb_y_row = saved_tfb_y;
	tfb_x_col = saved_tfb_x;
}



void open_file(const char *filename) {
	lines =0;
	dirty = 0;
	uint16_t c = 0;
	fn = filename;
	dbg("opening file %s\n", filename);
	int32_t fs = file_size(filename);
	if(fs > 0) {
		char * text = (char*) editor_malloc(fs+2); 
		uint32_t bytes_read = read_file(filename, (uint8_t*)text, fs, 0);
		text[fs-1] = 0;
		if(bytes_read) lines =1; // at least one line
		for(uint32_t i=0;i<bytes_read;i++) if(text[i]=='\n') lines++;
		dbg("File %s has %d lines\n", filename, lines);
		text_lines = (char**)editor_malloc(sizeof(char*)*(lines+1));
		uint32_t last = 0;
		for(uint32_t i=0;i<bytes_read;i++) {
			if(text[i]=='\n' || i==bytes_read-1) {
				text_lines[c]  = editor_malloc(i-last + 1);
				uint16_t x;
				for(x=0;x<i-last;x++) { 
					text_lines[c][x] = text[last+x];
				}
				text_lines[c][x] = 0;
				//dbg("Line %d is ###%s###\n", c, text_lines[c]);
				//if(c < TFB_ROWS-1) string_at_row(text_lines[c], i-last, c );
				last = i+1;
				c++;
			}
		}
        dbg("Adding end line at row %d\n", c);
		text_lines[c] = (char*)editor_malloc(1); 
		text_lines[c][0] = 0;
        lines++;
		editor_free(text);
		dbg("File read with %d lines.\n", lines);
	} else {
		dbg("Opening new file %s for writing\n", fn);
		lines = 1;
		text_lines = (char**)editor_malloc(sizeof(char*)*(1));
		text_lines[0] = (char*)editor_malloc(1);
		text_lines[0][0] = 0;
	}

	paint_tfb(0);
}

void editor_quit() {
	quit_flag = 0;
	if(dirty) {
		dirty = 0;
		string_at_row("Save file? [Y/N]", -1, TFB_ROWS-1);
		format_at_row(FORMAT_INVERSE, strlen("Save file? [Y/N]"), TFB_ROWS-1);
		#ifdef LOCALDEV
		local_draw_tfb();
		#endif
		int c = mp_hal_stdin_rx_chr();
		if(c=='y' || c=='Y') {
			dbg("save !\n");
			uint32_t bytes = 0;
			for(uint16_t i=0;i<lines;i++) { bytes = bytes + strlen(text_lines[i]) + 1; }
			char * text = (char*)editor_malloc(bytes);
			uint32_t c = 0;
			for(uint16_t i=0;i<lines;i++) { 
				for(uint16_t j=0;j<strlen(text_lines[i]);j++) {
					text[c++] = text_lines[i][j];
				}
				text[c++] = '\n';
			}
			write_file(fn, (uint8_t*)text, c, 0);
			dbg("saved!\n");
            editor_free(text);
		}

	}
	restore_tfb();
	for(uint16_t i=0;i<lines;i++) {
		editor_free(text_lines[i]);
	}
	editor_free(text_lines);
	if(yank) editor_free(yank);
    yank = 0;
#ifdef LOCALDEV
	endwin();
#endif
    printf("mc %d fc %d\n", mc, fc);
}


void editor_insert_character(int c) {
	dirty = 1;
	char * source_line = text_lines[cursor_y + y_offset];
	char * dest_line = (char*)editor_malloc(strlen(source_line) + 2);
	for(uint16_t i=0;i<cursor_x;i++) {
		dest_line[i] = source_line[i];
	}
	dest_line[cursor_x] = (uint8_t) c;
	for(uint16_t i=cursor_x+1;i<strlen(source_line)+1;i++) {
		dest_line[i] = source_line[i-1];
	}
	dest_line[strlen(source_line)+1] = 0;
	editor_free(source_line); // had a crash here
	text_lines[cursor_y + y_offset] = dest_line;
	string_at_row(dest_line, -1, cursor_y);
	move_cursor(cursor_x+1, cursor_y);
}

void editor_linestart() {
	move_cursor(0, cursor_y);
}

void editor_lineend() {
	move_cursor(strlen(text_lines[cursor_y + y_offset]), cursor_y);
}

void editor_backspace() {
	dirty = 1;
	char* cur_line = text_lines[cursor_y + y_offset];
	if(cursor_x > 0) {
		for(uint16_t i=cursor_x-1;i<strlen(cur_line);i++) {
			cur_line[i] = cur_line[i + 1];
		}
		string_at_row(cur_line, -1, cursor_y);
		move_cursor(cursor_x-1, cursor_y);
	} else {
		// hard mode, move up
		if(cursor_y + y_offset > 0) {
			// we will have 1 less line when this is done
			char *above_line = text_lines[cursor_y+y_offset-1];
            char *old_line = text_lines[cursor_y+y_offset];
			char *new_line = (char*)editor_malloc(strlen(above_line) + strlen(cur_line) + 1);
			uint16_t c = 0;
			for(uint16_t i=0;i<strlen(above_line);i++) new_line[c++] = above_line[i];
			uint16_t split = c;
			for(uint16_t i=0;i<strlen(cur_line);i++) new_line[c++] = cur_line[i];
			new_line[c] = 0;
			editor_free(above_line);
			text_lines[cursor_y+y_offset-1] = new_line;
			lines--;


            char** dest_text_lines = (char**)editor_malloc(sizeof(char*) * (lines));
            for(uint16_t i=0;i<cursor_y+y_offset;i++) {
                dest_text_lines[i] = text_lines[i];
            }
			for(uint16_t i=cursor_y+y_offset;i<lines;i++) {
				dest_text_lines[i] = text_lines[i+1];
			}
            editor_free(text_lines);
            editor_free(old_line);
            text_lines = dest_text_lines;
			paint_tfb(cursor_y-1);

			// Move the cursor up at the split
			move_cursor(split, cursor_y-1);

		} // else do nothing, we're at the top of the file
	}
}

void editor_tab() {
	for(uint8_t i=0;i<EDITOR_TAB_SPACES;i++) editor_insert_character(' ');
}

void editor_crlf() {
	dirty = 1;
	char* cur_line = text_lines[cursor_y + y_offset];
	char** dest_text_lines = (char**)editor_malloc(sizeof(char*) * (lines + 1));
	for(uint16_t i=0;i<cursor_y+y_offset+1;i++) {
		dest_text_lines[i] = text_lines[i];
	}
	if(cursor_x == strlen(cur_line)) {
		// We are at the end of the line, so just make an empty line underneath and move on
		dest_text_lines[cursor_y+y_offset+1] = (char*)editor_malloc(1);
		dest_text_lines[cursor_y+y_offset+1][0] = 0;
	} else {
		// We are somewhere in the middle of the line, so split and copy
		// Get the text on cur line that has to go below
		uint16_t chars_to_copy = strlen(cur_line) - cursor_x;
		dest_text_lines[cursor_y+y_offset+1] = (char*)editor_malloc(chars_to_copy+1);
		uint16_t i;
		for(i=0;i<chars_to_copy;i++) {
			dest_text_lines[cursor_y+y_offset+1][i] = dest_text_lines[cursor_y+y_offset][cursor_x+i];
		}
		dest_text_lines[cursor_y+y_offset+1][i] = 0;
		// Chop the old line, no need to realloc, i think we're ok
		dest_text_lines[cursor_y+y_offset][cursor_x] = 0;
	}
	// Copy the remaining lines below that were not impacted
	for(uint16_t i=cursor_y+y_offset+2;i<lines+1;i++) {
		dest_text_lines[i] = text_lines[i-1];
	}
	lines++;
	editor_free(text_lines);		
	text_lines = dest_text_lines;

	// Redraw everything from the split (going down, as scroll)
	paint_tfb(cursor_y);
	// Update cursor
	move_cursor(0, cursor_y +1);
}




void editor_up() {
    if(cursor_y+y_offset > 0) {
        if(cursor_x < strlen(text_lines[cursor_y-1 + y_offset])) {
            move_cursor(cursor_x, cursor_y-1);
        } else {
            move_cursor(strlen(text_lines[cursor_y-1 + y_offset]), cursor_y-1);
        }
    }
}
void editor_down() {
    if(cursor_y + y_offset < lines-1) {
        if(cursor_x < strlen(text_lines[cursor_y+1+y_offset])) {
            move_cursor(cursor_x, cursor_y+1);
        } else {
            move_cursor(strlen(text_lines[cursor_y+1+y_offset]), cursor_y+1);
        }
    } 
}

void editor_right() {
    // easiest, just go right
    if(cursor_x < strlen(text_lines[cursor_y + y_offset])) {
        move_cursor(cursor_x + 1, cursor_y);
    } else {
        editor_down();
        editor_linestart();
    }
}
void editor_left() {
    // Easiest, if x is not zero, just move left
    if(cursor_x > 0) {
        move_cursor(cursor_x-1, cursor_y);
    } else {
        // If x is at the left, go up a line
        editor_up();
        // And move x cursor to the end of line
        editor_lineend();
    }
}


void editor_yank() {
    char * yanked_line = text_lines[cursor_y+y_offset];
    if(strlen(yanked_line)) {
        dirty = 1;
    	if(yank) editor_free(yank);
    	yank = (char*)editor_malloc(strlen(yanked_line)+1);
    	for(uint16_t i=0;i<strlen(yanked_line);i++) yank[i] = yanked_line[i];
    	yank[strlen(yanked_line)] = 0;
    	// Now remove this line from the text_lines
    	for(uint16_t i=cursor_y+y_offset;i<lines-1;i++) {
    		text_lines[i] = text_lines[i+1];
    	}
    	lines--;
    	// Got a crash here when yanking twice without an unyank
    	editor_free(yanked_line);
    	paint_tfb(cursor_y);
    	move_cursor(0, cursor_y);
    } else {
        editor_backspace();
        editor_down();
    }
}

void editor_unyank() {
	if(yank) {
		dirty = 1;
		dbg("unyanking ###%s###\n", yank);
		char** dest_text_lines = (char**)editor_malloc(sizeof(char*) * (lines + 1));
		for(uint16_t i=0;i<cursor_y+y_offset;i++) dest_text_lines[i] = text_lines[i];
		dest_text_lines[cursor_y+y_offset] = (char*)editor_malloc(strlen(yank)+1);
		for(uint16_t i=0;i<strlen(yank);i++) dest_text_lines[cursor_y+y_offset][i] = yank[i];
		dest_text_lines[cursor_y+y_offset][strlen(yank)] = 0;
		for(uint16_t i=cursor_y+y_offset;i<lines;i++) {
			dest_text_lines[i+1] = text_lines[i];
		}
		lines++;
		editor_free(text_lines);
		text_lines = dest_text_lines;
		paint_tfb(cursor_y);
		move_cursor(0,cursor_y);
        editor_down();
	}
}




void process_char(int c) {
	//dbg("Got char %d\n", c);
	if(c==127 || c==8) { // backspace
		editor_backspace();
	} else if(c == 9) { // tab, control-I
		editor_tab();
	} else if(c==10 || c == 13) { // CRLF
		editor_crlf();
	} else if(c == 3) {  // control-C 
	} else if(c ==11) { // control-K, yank
		editor_yank();
	} else if(c== 21) { // control-U, unyank
		editor_unyank();
	} else if(c==15) { // control-O, save as TODO
		// TODO 
	} else if(c==23) { // control-W, "where is" aka search TODO 
		// TODO 
	} else if(c==24) { // control-X, save 
		quit_flag = 1;
	} else if(c == 1) { // control-A, start of line
		editor_linestart();
	} else if(c == 5) { // control-E, end of line
		editor_lineend();
	} else if(c==25) { // control Y, page up
		editor_page_up();
	} else if(c==22) { // control V, page down 
		editor_page_down();
	} else if(c==27) { // ansi code, up / down / left / right / forward delete
		char s = (char)mp_hal_stdin_rx_chr(); // Skip the [
		s = (char)mp_hal_stdin_rx_chr(); // Get the code
		if(s == 'D') { //left 
			editor_left();
		} else if(s=='C') { // right
			editor_right();
		} else if(s=='B') { // down
			editor_down();
		} else if(s=='A') { // up
			editor_up();
		} else if(s=='3') { // forward delete
			s = (char)mp_hal_stdin_rx_chr(); // skip the ~
			// AFAIK this is what forward delete is
			editor_right();
			editor_backspace();
		}
	// local mode high bit arrows & forward delete 
	} else if(c == 259) { editor_up(); 
	} else if(c == 258) { editor_down(); 
	} else if(c == 260) { editor_left(); 
	} else if(c == 261) { editor_right(); 
	} else if(c == 330) { editor_right(); editor_backspace(); 
	} else if(c>31 && c<127) {
		editor_insert_character(c);
	} else {
		// Ignore unsupported keycodes...
	}
}

void editor(const char * filename) {
    mc = 0; fc = 0;
	save_tfb();
	if(filename != NULL) { 
		dbg("editor fn is %s\n", filename);
		open_file(filename);
	} else {
		dbg("no filename given\n");
	}
	move_cursor(0,0);
	y_offset = 0;
	while(quit_flag == 0) {
		#ifdef LOCALDEV
		local_draw_tfb();
#else
		vPortYield();
#endif
		int c = check_rx_char();
		//int c = mp_hal_stdin_rx_chr();
		if(c>=0) {
			process_char(c);
		}
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

