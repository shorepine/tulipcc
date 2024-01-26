// keyscan.c
#include "keyscan.h"

#ifndef ESP_PLATFORM
#include <SDL.h>
#endif

uint8_t keyboard_send_keys_to_micropython = 1;
int8_t keyboard_grab_ui_focus = -1;

extern int mp_interrupt_char;
uint8_t last_scan[8] = {0,0,0,0,0,0,0,0};



// I pulled this from a Linux commit
/*
 * Convert UTF-8 bytes into a code page 437 character.
 * Based on the table in the Code_page_437 Wikipedia page.
*/


uint8_t code_points_00a0[] = {
   255, 173, 155, 156,   7, 157,   7,  21,
     7,   7, 166, 174, 170,   7,   7,   7,
   248, 241, 253,   7,   7, 230,  20, 250,
     7,   7, 167, 175, 172, 171,   7, 168,
     7,   7,   7,   7, 142, 143, 146, 128,
     7, 144,   7,   7,   7,   7,   7,   7,
     7, 165,   7,   7,   7,   7, 153,   7,
     7,   7,   7,   7, 154,   7,   7, 225,
   133, 160, 131,   7, 132, 134, 145, 135,
   138, 130, 136, 137, 141, 161, 140, 139,
     7, 164, 149, 162, 147,   7, 148, 246,
     7, 151, 163, 150, 129,   7,   7, 152,
};

uint8_t code_points_2550[] = {
   205, 186, 213, 214, 201, 184, 183, 187,
   212, 211, 200, 190, 189, 188, 198, 199,
   204, 181, 182, 185, 209, 210, 203, 207,
   208, 202, 216, 215, 206
};

static uint8_t utf8_convert_11bit(uint16_t code)
{
   switch (code) {
   case 0x0192: return 159;
   case 0x0393: return 226;
   case 0x0398: return 233;
   case 0x03A3: return 228;
   case 0x03A6: return 232;
   case 0x03A9: return 234;
   case 0x03B1: return 224;
   case 0x03B4: return 235;
   case 0x03B5: return 238;
   case 0x03C0: return 227;
   case 0x03C3: return 229;
   case 0x03C4: return 231;
   case 0x03C6: return 237;
   }

   return 0;
};

static uint8_t utf8_convert_2xxx(uint16_t code)
{
   switch (code) {
   case 0x2022: return 7;
   case 0x203C: return 19;
   case 0x207F: return 252;
   case 0x20A7: return 158;
   case 0x2190: return 27;
   case 0x2191: return 24;
   case 0x2192: return 26;
   case 0x2193: return 25;
   case 0x2194: return 29;
   case 0x2195: return 18;
   case 0x21A8: return 23;
   case 0x2219: return 249;
   case 0x221A: return 251;
   case 0x221E: return 236;
   case 0x221F: return 28;
   case 0x2229: return 239;
   case 0x2248: return 247;
   case 0x2261: return 240;
   case 0x2264: return 243;
   case 0x2265: return 242;
   case 0x2310: return 169;
   case 0x2320: return 244;
   case 0x2321: return 245;
   case 0x2500: return 196;
   case 0x2502: return 179;
   case 0x250C: return 218;
   case 0x2510: return 191;
   case 0x2514: return 192;
   case 0x2518: return 217;
   case 0x251C: return 195;
   case 0x2524: return 180;
   case 0x252C: return 194;
   case 0x2534: return 193;
   case 0x253C: return 197;
   case 0x2580: return 223;
   case 0x2584: return 220;
   case 0x2588: return 219;
   case 0x258C: return 221;
   case 0x2590: return 222;
   case 0x2591: return 176;
   case 0x2592: return 177;
   case 0x2593: return 178;
   case 0x25A0: return 254;
   case 0x25AC: return 22;
   case 0x25B2: return 30;
   case 0x25BA: return 16;
   case 0x25BC: return 31;
   case 0x25C4: return 17;
   case 0x25CB: return 9;
   case 0x25D8: return 8;
   case 0x25D9: return 10;
   case 0x263A: return 1;
   case 0x263B: return 2;
   case 0x263C: return 15;
   case 0x2640: return 12;
   case 0x2642: return 11;
   case 0x2660: return 6;
   case 0x2663: return 5;
   case 0x2665: return 3;
   case 0x2666: return 4;
   case 0x266A: return 13;
   case 0x266B: return 14;
   }

   return 0;
}

uint8_t convert_uc16_to_cp437(uint16_t code)
{
   if (code < 0x7f)        // ASCII
       return code;
   if (code < 0xa0)        // high control characters
       return code;
   if (code < 0x100)       // international characters
       return code_points_00a0[code - 0xa0];
   if (code < 0x800)
       return utf8_convert_11bit(code);
   if (code >= 0x2550 && code < 0x256d)    // block graphics
       return code_points_2550[code - 0x2550];

   return utf8_convert_2xxx(code);
}

uint8_t convert_utf8_to_cp437(uint8_t c, uint32_t *esc)
{
   int shift;
   uint16_t ucs;

   if (c < 127)            // ASCII
       return c;
   if (c == 127)
       return 8;       // DEL (?)

   switch (c & 0xf0) {
   case 0xc0: case 0xd0:       // two bytes sequence
       *esc = (1U << 24) | ((c & 0x1f) << 6);
       return 0;
   case 0xe0:          // three bytes sequence
       *esc = (2U << 24) | ((c & 0x0f) << 12);
       return 0;
   case 0xf0:          // four bytes sequence
       *esc = (3U << 24) | ((c & 0x07) << 18);
       return 0;
   case 0x80: case 0x90: case 0xa0: case 0xb0: // continuation
       shift = (*esc >> 24) - 1;
       ucs = *esc & 0xffffff;
       if (shift) {
           *esc = (shift << 24) | ucs | (c & 0x3f) << (shift * 6);
           return 0;
       }
       *esc = 0;
       return convert_uc16_to_cp437(ucs | (c & 0x3f));
   }

   return 0;
}


uint16_t scan_ascii(uint8_t code, uint32_t modifier) {
#ifndef ESP_PLATFORM
    uint8_t shift = (modifier & KMOD_LSHIFT || modifier & KMOD_RSHIFT);
    uint8_t ctrl  = (modifier & KMOD_LCTRL || modifier & KMOD_RCTRL);
#else
    uint8_t shift = (modifier & KEY_MOD_LSHIFT || modifier & KEY_MOD_RSHIFT);
    uint8_t ctrl  = (modifier & KEY_MOD_LCTRL || modifier & KEY_MOD_RCTRL);
#endif    
    //uint8_t alt   = (modifier & KEY_MOD_LALT || modifier & KEY_MOD_RALT);
    //uint8_t meta  = (modifier & KEY_MOD_LMETA || modifier & KEY_MOD_RMETA);

    if(code >= 0x04 && code <= 0x1d) {
        // control-chars
        if(ctrl) return (code-0x04)+ 1;
        // capital letters
        if(shift) return (code-0x04)+'A';
        return code-0x04 + 'a';
    }
    if(code >= 0x1e && code <= 0x27) {
        if(shift) {
            switch(code) {
                case 0x1e: return '!'; 
                case 0x1f: return '@'; 
                case 0x20: return '#'; 
                case 0x21: return '$'; 
                case 0x22: return '%'; 
                case 0x23: return '^'; 
                case 0x24: return '&'; 
                case 0x25: return '*'; 
                case 0x26: return '('; 
                case 0x27: return ')'; 
            }
        } else {
            if(code == 0x27) return '0';
            return (code-0x1e) + '1';
        }
    }
    switch(code) {
        case KEY_ENTER: return 13;  
        case KEY_ESC: return 27; 
        case KEY_BACKSPACE: return 8; 
        case KEY_TAB: return 9; 
        case KEY_PAGEUP: return 25;
        case KEY_PAGEDOWN: return 22;

        case KEY_SPACE: return ' '; 
        case KEY_MINUS: if(shift) return '_'; else return '-';
        case KEY_EQUAL: if(shift) return '+'; else return '=';
        case KEY_LEFTBRACE: if(shift) return '{'; else return '[';
        case KEY_RIGHTBRACE: if(shift) return '}'; else return ']';
        case KEY_BACKSLASH: if(shift) return '|'; else return '\\';
        case KEY_HASHTILDE: if(shift) return '~'; else return '#';
        case KEY_SEMICOLON: if(shift) return ':'; else return ';';
        case KEY_APOSTROPHE: if(shift) return '\"'; else return '\'';
        case KEY_GRAVE: if(shift) return '~'; else return '`';
        case KEY_COMMA: if(shift) return '<'; else return ',';
        case KEY_DOT: if(shift) return '>'; else return '.';
        case KEY_SLASH: if(shift) return '?'; else return '/';

        // We return extended codes for these, and get converted to ANSI for the repl down the line
        case KEY_UP: return 259; 
        case KEY_DOWN: return 258; 
        case KEY_LEFT: return 260; 
        case KEY_RIGHT: return 261;
        case KEY_DELETE: return 262; 

    }
    return 0;
}



void send_key_to_micropython(uint16_t c) {
    // Deal with the extended codes that need ANSI escape chars 
    if(c>257 && c<263) { 
        tx_char(27);
        tx_char('[');
        if(c==258) tx_char('B');
        if(c==259) tx_char('A');
        if(c==260) tx_char('D');
        if(c==261) tx_char('C');
        if(c==262) { tx_char('3'); tx_char(126); }
    } else if (c==mp_interrupt_char) {
        // Send a ctrl-C to Micropython if sent 
        mp_sched_keyboard_interrupt();
    } else if (c==4) { // control-D
        tx_char(c);
    } else {
        tx_char(c);
    }
}
