// lvgl_u8g2.c
// render u8g2 fonts in lvgl 


#include "lvgl.h"
#include "u8g2_fonts.h"


lv_font_t lv_u8g2_font_helvB14_tr;

/* Get info about glyph of `unicode_letter` in `font` font.
 * Store the result in `dsc_out`.
 * The next letter (`unicode_letter_next`) might be used to calculate the width required by this glyph (kerning)
 */
bool my_get_glyph_dsc_cb(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
    uint32_t font_no = *((uint32_t*) font->user_data);
    dsc_out->adv_w = u8g2_glyph_width(font_no, unicode_letter);        /*Horizontal space required by the glyph in [px]*/
    dsc_out->box_h = 8;         /*Height of the bitmap in [px]*/
    dsc_out->box_w = 6;         /*Width of the bitmap in [px]*/
    dsc_out->ofs_x = 0;         /*X offset of the bitmap in [pf]*/
    dsc_out->ofs_y = 3;         /*Y offset of the bitmap measured from the as line*/
    dsc_out->format= LV_FONT_GLYPH_FORMAT_A1;
    dsc_out->gid.index = unicode_letter; 
    return true;                /*true: glyph found; false: glyph was not found*/
}

//uint8_t bitmap[8] = {123,149,126,29,49,151, 123, 23};
/* Get the bitmap of `unicode_letter` from `font`. */

int16_t u8g2_DrawGlyph_target(u8g2_font_t *u8g2, uint16_t encoding, uint8_t * target);

const void * my_get_glyph_bitmap_cb(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf)
{
    const lv_font_t * font_p = g_dsc->resolved_font;
    uint32_t font_no = *((uint32_t*) font_p->user_data);

    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    uint32_t gid = g_dsc->gid.index;
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);

    u8g2_DrawGlyph_target(&ufont, gid, draw_buf->data);

    // put it in draw_buf
    return draw_buf;
}

void setup_lvgl_u8g2() {
    uint32_t font_no = 5;
    lv_u8g2_font_helvB14_tr.get_glyph_dsc = my_get_glyph_dsc_cb;        /*Set a callback to get info about glyphs*/
    lv_u8g2_font_helvB14_tr.get_glyph_bitmap = my_get_glyph_bitmap_cb;  /*Set a callback to get bitmap of a glyph*/
    lv_u8g2_font_helvB14_tr.line_height = 14;                       /*The real line height where any text fits*/
    lv_u8g2_font_helvB14_tr.base_line = 10; ///ascent_A // base_line;                      /*Base line measured from the top of line_height*/
    lv_u8g2_font_helvB14_tr.user_data = &font_no;                      /*Optionally some extra user data*/
}

