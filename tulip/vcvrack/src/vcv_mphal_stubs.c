// Headless stubs for symbols the desktop HAL references but a VCV plugin
// doesn't have: the Tulip text framebuffer (unix_mphal.c echoes stdout to the
// screen) — here stdout just goes to the host's stderr/console.

#include <stdint.h>

uint8_t tfb_fg_pal_color = 0;
uint8_t tfb_bg_pal_color = 0;

void display_tfb_str(unsigned char *str, uint16_t len, uint8_t format, uint8_t fg_color, uint8_t bg_color) {
    (void)str; (void)len; (void)format; (void)fg_color; (void)bg_color;
}
