// keyscan.c
#include "keyscan.h"

#ifndef ESP_PLATFORM
#include <SDL.h>
#endif

extern int mp_interrupt_char;

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
    } else {
        tx_char(c);
    }
}
