// AMYboard variant: the USB port runs as a USB MIDI *host* (like Tulip CC)
// instead of a TinyUSB CDC + gadget-MIDI device. The REPL moves to UART0 —
// the same pins the stderr console already uses — so connect a USB-serial
// dongle to the UART header to control the board.
//
// The TinyUSB device stack is still compiled (amy_midi.c references tud_midi_*
// whenever AMYBOARD is defined) but usb_init()/tusb_init() are never called,
// so every tud_* call is a safe no-op.
#include "../AMYBOARD/mpconfigboard.h"

#undef MICROPY_HW_BOARD_NAME
#define MICROPY_HW_BOARD_NAME               "AMYboard USB host"

#undef MICROPY_HW_ENABLE_UART_REPL
#define MICROPY_HW_ENABLE_UART_REPL         (1)
