// lvgl_u8g2.c
// render u8g2 fonts in lvgl 


#include "lvgl.h"
#include "u8g2_fonts.h"


lv_font_t lv_u8g2_font_helvB14_tr;
// will make this less nasty asap
#define MAX_FONT_W 50
#define MAX_FONT_H 80
uint8_t databuf[MAX_FONT_W*MAX_FONT_H];


/* Get info about glyph of `unicode_letter` in `font` font.
 * Store the result in `dsc_out`.
 * The next letter (`unicode_letter_next`) might be used to calculate the width required by this glyph (kerning)
 */
bool my_get_glyph_dsc_cb(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
    // Even though this callback is just to get font/glyph info, u8g2 can't really know everything about the glyph
    // until u8g2 draws it. so i draw it, save the bitmap to a temp part of RAM , extract the info, and the
    // drawing CB will use the same bit of ram without having to re-draw. 
    uint32_t font_no = *((uint32_t*) font->user_data);
    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);
    for(uint16_t i=0;i<(MAX_FONT_H*MAX_FONT_W);i++) { databuf[i] = 0; }
    int16_t adv = u8g2_DrawGlyph_target(&ufont, unicode_letter, databuf);
    u8g2_font_decode_t font_decode = u8g2_GetGlyphInfo(&ufont, unicode_letter);
    fprintf(stderr, "%c width is %d height %d x %d y %d adv is %d tx %d ty %d\n", unicode_letter, font_decode.glyph_width,font_decode.glyph_height, font_decode.x, font_decode.y,adv, font_decode.target_x, font_decode.target_y );
    dsc_out->adv_w = adv;        /*Horizontal space required by the glyph in [px]*/
    dsc_out->box_h = font_decode.glyph_height*2;           /*Height of the bitmap in [px]*/
    dsc_out->box_w = font_decode.glyph_width*2;         /*Width of the bitmap in [px]*/
    dsc_out->ofs_x = 0; //font_decode.target_x;         /*X offset of the bitmap in [pf]*/
    dsc_out->ofs_y = 0; //abs(font_decode.target_y);         /*Y offset of the bitmap measured from the as line*/
    if(abs(font_decode.target_y) != font_decode.glyph_height) {
        //dsc_out->ofs_y = -font_decode.target_y;
    }
    dsc_out->format= LV_FONT_GLYPH_FORMAT_A1;
    dsc_out->gid.index = unicode_letter; 
    return true;                /*true: glyph found; false: glyph was not found*/
}

const void * my_get_glyph_bitmap_cb(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf)
{
    memcpy(draw_buf->data, databuf, g_dsc->box_w*g_dsc->box_h);
    return draw_buf;
}

uint32_t font_no= 5;
void setup_lvgl_u8g2() {
    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);
    fprintf(stderr, "font ascent_A %d descent_g %d max_height %d\n", ufont.font_info.ascent_A, ufont.font_info.descent_g, ufont.font_info.max_char_height);
    lv_u8g2_font_helvB14_tr.get_glyph_dsc = my_get_glyph_dsc_cb;        /*Set a callback to get info about glyphs*/
    lv_u8g2_font_helvB14_tr.get_glyph_bitmap = my_get_glyph_bitmap_cb;  /*Set a callback to get bitmap of a glyph*/
    lv_u8g2_font_helvB14_tr.line_height = ufont.font_info.max_char_height;                       /*The real line height where any text fits*/
    lv_u8g2_font_helvB14_tr.base_line = abs(ufont.font_info.descent_g); ///ascent_A // base_line;                      /*Base line measured from the top of line_height*/
    lv_u8g2_font_helvB14_tr.user_data = &font_no;                      /*Optionally some extra user data*/
}

