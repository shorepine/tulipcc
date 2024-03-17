/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --no-compress --no-prefilter --bpp 1 --size 8 --font Montserrat-Medium.ttf -r 0x20-0x7F,0xB0,0x2022 --font FontAwesome5-Solid+Brands+Regular.woff -r 61441,61448,61451,61452,61452,61453,61457,61459,61461,61465,61468,61473,61478,61479,61480,61502,61507,61512,61515,61516,61517,61521,61522,61523,61524,61543,61544,61550,61552,61553,61556,61559,61560,61561,61563,61587,61589,61636,61637,61639,61641,61664,61671,61674,61683,61724,61732,61787,61931,62016,62017,62018,62019,62020,62087,62099,62212,62189,62810,63426,63650 --format lvgl -o lv_font_montserrat_8.c --force-fast-kern-format
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_MONTSERRAT_8
#define LV_FONT_MONTSERRAT_8 1
#endif

#if LV_FONT_MONTSERRAT_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xe8,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x57, 0xd5, 0xf5, 0x0,

    /* U+0024 "$" */
    0x2f, 0xa6, 0x3f, 0x20,

    /* U+0025 "%" */
    0xe2, 0xcf, 0xcd, 0x1c,

    /* U+0026 "&" */
    0x63, 0x19, 0x2e, 0x80,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x6a, 0xa4,

    /* U+0029 ")" */
    0x95, 0x58,

    /* U+002A "*" */
    0x5f, 0x80,

    /* U+002B "+" */
    0x5d, 0x0,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x25, 0x29, 0x20,

    /* U+0030 "0" */
    0x74, 0x63, 0x17, 0x0,

    /* U+0031 "1" */
    0xd5, 0x40,

    /* U+0032 "2" */
    0xf1, 0x24, 0xf0,

    /* U+0033 "3" */
    0x78, 0x8c, 0x17, 0x0,

    /* U+0034 "4" */
    0x22, 0x25, 0xf1, 0x0,

    /* U+0035 "5" */
    0xf8, 0xe1, 0xf0,

    /* U+0036 "6" */
    0x78, 0xf9, 0x70,

    /* U+0037 "7" */
    0xf9, 0x22, 0x40,

    /* U+0038 "8" */
    0xf4, 0x99, 0x3f, 0x0,

    /* U+0039 "9" */
    0xe9, 0xf1, 0xe0,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x98,

    /* U+003C "<" */
    0x68, 0x70,

    /* U+003D "=" */
    0xe3, 0x80,

    /* U+003E ">" */
    0xc3, 0xc0,

    /* U+003F "?" */
    0xf1, 0x20, 0x20,

    /* U+0040 "@" */
    0x3c, 0xfe, 0xc5, 0xc5, 0xff, 0x38,

    /* U+0041 "A" */
    0x30, 0xc4, 0x9e, 0x84,

    /* U+0042 "B" */
    0xfc, 0x7d, 0x1f, 0x80,

    /* U+0043 "C" */
    0x7e, 0x21, 0x87, 0x80,

    /* U+0044 "D" */
    0xf4, 0x63, 0x1f, 0x0,

    /* U+0045 "E" */
    0xf8, 0xf8, 0xf0,

    /* U+0046 "F" */
    0xf8, 0xf8, 0x80,

    /* U+0047 "G" */
    0x74, 0x63, 0x17, 0x0,

    /* U+0048 "H" */
    0x8c, 0x7f, 0x18, 0x80,

    /* U+0049 "I" */
    0xf8,

    /* U+004A "J" */
    0x71, 0x11, 0x70,

    /* U+004B "K" */
    0x95, 0x39, 0x28, 0x80,

    /* U+004C "L" */
    0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0x87, 0x3d, 0x6d, 0x84,

    /* U+004E "N" */
    0x8e, 0x6b, 0x38, 0x80,

    /* U+004F "O" */
    0x7a, 0x38, 0x63, 0x78,

    /* U+0050 "P" */
    0xf4, 0x63, 0xe8, 0x0,

    /* U+0051 "Q" */
    0x7a, 0x38, 0x63, 0x78, 0x70,

    /* U+0052 "R" */
    0xf4, 0x63, 0xe9, 0x0,

    /* U+0053 "S" */
    0xe8, 0x43, 0xf0,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+0055 "U" */
    0x8c, 0x63, 0x17, 0x0,

    /* U+0056 "V" */
    0x89, 0x25, 0xc, 0x30,

    /* U+0057 "W" */
    0x88, 0xac, 0x95, 0x4a, 0xc2, 0x20,

    /* U+0058 "X" */
    0x4a, 0x88, 0xa8, 0x80,

    /* U+0059 "Y" */
    0x8a, 0x94, 0x42, 0x0,

    /* U+005A "Z" */
    0xf8, 0x88, 0x8f, 0x80,

    /* U+005B "[" */
    0xea, 0xac,

    /* U+005C "\\" */
    0x92, 0x24, 0x88,

    /* U+005D "]" */
    0xd5, 0x5c,

    /* U+005E "^" */
    0x5a, 0x80,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x80,

    /* U+0061 "a" */
    0xee, 0x9f,

    /* U+0062 "b" */
    0x84, 0x3d, 0x18, 0xf8,

    /* U+0063 "c" */
    0x78, 0x87,

    /* U+0064 "d" */
    0x8, 0x5f, 0x18, 0xbc,

    /* U+0065 "e" */
    0x6f, 0x86,

    /* U+0066 "f" */
    0x64, 0xe4, 0x44,

    /* U+0067 "g" */
    0x7c, 0x62, 0xf7, 0x0,

    /* U+0068 "h" */
    0x88, 0xf9, 0x99,

    /* U+0069 "i" */
    0xbc,

    /* U+006A "j" */
    0x20, 0x92, 0x58,

    /* U+006B "k" */
    0x88, 0x9a, 0xe9,

    /* U+006C "l" */
    0xfc,

    /* U+006D "m" */
    0xef, 0x26, 0x4c, 0x90,

    /* U+006E "n" */
    0xf9, 0x99,

    /* U+006F "o" */
    0x74, 0x62, 0xe0,

    /* U+0070 "p" */
    0xf4, 0x63, 0xe8, 0x0,

    /* U+0071 "q" */
    0x7c, 0x62, 0xf0, 0x80,

    /* U+0072 "r" */
    0xf2, 0x40,

    /* U+0073 "s" */
    0xe8, 0x6e,

    /* U+0074 "t" */
    0x4e, 0x44, 0x60,

    /* U+0075 "u" */
    0x99, 0x9f,

    /* U+0076 "v" */
    0x99, 0x62,

    /* U+0077 "w" */
    0x93, 0x55, 0xb3, 0x20,

    /* U+0078 "x" */
    0x96, 0x69,

    /* U+0079 "y" */
    0x95, 0x62, 0xc0,

    /* U+007A "z" */
    0xf2, 0x4f,

    /* U+007B "{" */
    0x69, 0x44, 0x98,

    /* U+007C "|" */
    0xfe,

    /* U+007D "}" */
    0xc9, 0x14, 0xb0,

    /* U+007E "~" */
    0x70,

    /* U+00B0 "°" */
    0xf7, 0x80,

    /* U+2022 "•" */
    0x80,

    /* U+F001 "" */
    0x3, 0x3f, 0x3d, 0x21, 0x21, 0x27, 0xe7, 0xe0,

    /* U+F008 "" */
    0xff, 0xd1, 0x78, 0xff, 0xfa, 0x2f, 0xfc,

    /* U+F00B "" */
    0xde, 0xff, 0xde, 0xff, 0xff, 0x0, 0xff, 0xff,

    /* U+F00C "" */
    0x3, 0x7, 0xce, 0xfc, 0x78, 0x30,

    /* U+F00D "" */
    0xcd, 0xe3, 0x1e, 0xcc,

    /* U+F011 "" */
    0x10, 0xab, 0x5c, 0x98, 0x30, 0x51, 0x1c,

    /* U+F013 "" */
    0x18, 0x5c, 0xff, 0x66, 0x66, 0xff, 0x7c, 0x18,

    /* U+F015 "" */
    0xb, 0x1b, 0x9a, 0xdf, 0xf7, 0xf3, 0xb9, 0xdc,

    /* U+F019 "" */
    0x18, 0xc, 0x6, 0xf, 0xc3, 0xc7, 0xfb, 0xfb,
    0xff,

    /* U+F01C "" */
    0x3e, 0x20, 0xb0, 0x7c, 0x7f, 0xff, 0xfc,

    /* U+F021 "" */
    0x3d, 0x43, 0xc7, 0x0, 0x0, 0xe3, 0xc6, 0xbc,

    /* U+F026 "" */
    0x3, 0xff, 0xf3, 0x0,

    /* U+F027 "" */
    0x0, 0xcf, 0x7d, 0xf0, 0xc0, 0x0,

    /* U+F028 "" */
    0x2, 0x0, 0x8c, 0xbe, 0xdf, 0x6f, 0x94, 0xc4,
    0x4,

    /* U+F03E "" */
    0xff, 0x9f, 0xfb, 0xd1, 0x81, 0xff,

    /* U+F043 "" */
    0x20, 0xc7, 0x1e, 0xfa, 0xed, 0x9c,

    /* U+F048 "" */
    0x8c, 0xff, 0xff, 0xce, 0x20,

    /* U+F04B "" */
    0xc1, 0xe3, 0xe7, 0xff, 0xff, 0x38, 0x60,

    /* U+F04C "" */
    0xef, 0xdf, 0xbf, 0x7e, 0xfd, 0xfb, 0x80,

    /* U+F04D "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,

    /* U+F051 "" */
    0x8e, 0x7f, 0xff, 0xe6, 0x20,

    /* U+F052 "" */
    0x10, 0x71, 0xf7, 0xf0, 0x1f, 0xff, 0x80,

    /* U+F053 "" */
    0x11, 0x99, 0x86, 0x18, 0x40,

    /* U+F054 "" */
    0x43, 0xc, 0x33, 0x31, 0x0,

    /* U+F067 "" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+F068 "" */
    0xfe,

    /* U+F06E "" */
    0x3e, 0x31, 0xb7, 0x7b, 0xb6, 0x31, 0xf0,

    /* U+F070 "" */
    0x40, 0xd, 0xe0, 0x76, 0x37, 0x66, 0x6c, 0x63,
    0x7, 0x30, 0x2,

    /* U+F071 "" */
    0x8, 0xe, 0x7, 0x6, 0xc7, 0x73, 0xfb, 0xdf,
    0xff,

    /* U+F074 "" */
    0x2, 0xef, 0x3a, 0x3a, 0xef, 0x2,

    /* U+F077 "" */
    0x10, 0x71, 0xb6, 0x30,

    /* U+F078 "" */
    0x82, 0xd8, 0xe0, 0x80,

    /* U+F079 "" */
    0x40, 0x77, 0xb8, 0x48, 0x24, 0x3b, 0xdc, 0x4,

    /* U+F07B "" */
    0xf0, 0xff, 0xff, 0xff, 0xff, 0xff,

    /* U+F093 "" */
    0x8, 0xe, 0xf, 0x83, 0x81, 0xc7, 0xfb, 0xfb,
    0xff,

    /* U+F095 "" */
    0x3, 0x7, 0x7, 0x2, 0x6, 0x6c, 0xf8, 0xe0,

    /* U+F0C4 "" */
    0xe7, 0x5b, 0xe1, 0x8f, 0x95, 0xb9, 0x80,

    /* U+F0C5 "" */
    0x3d, 0xf3, 0xff, 0xff, 0xff, 0xf7, 0xfc,

    /* U+F0C7 "" */
    0xfd, 0xe, 0x1f, 0xfe, 0xfd, 0xff, 0x80,

    /* U+F0C9 "" */
    0xff, 0xfc, 0x7, 0xf0, 0x0, 0x3f, 0x80,

    /* U+F0E0 "" */
    0xff, 0xff, 0xbc, 0xdb, 0xe7, 0xff,

    /* U+F0E7 "" */
    0xe7, 0x3f, 0xf3, 0x10, 0x80,

    /* U+F0EA "" */
    0xf9, 0xf3, 0x7, 0xef, 0x9f, 0xff, 0x9f,

    /* U+F0F3 "" */
    0x10, 0x71, 0xf3, 0xe7, 0xdf, 0xff, 0x88,

    /* U+F11C "" */
    0xff, 0xdd, 0x7b, 0x7f, 0xfa, 0x2f, 0xfc,

    /* U+F124 "" */
    0x3, 0xf, 0x3e, 0xfe, 0x7c, 0x1c, 0x18, 0x8,

    /* U+F15B "" */
    0xf9, 0xfb, 0xc7, 0xff, 0xff, 0xff, 0xff,

    /* U+F1EB "" */
    0x0, 0x1f, 0xec, 0xc, 0x78, 0x21, 0x0, 0x0,
    0xc0, 0x30,

    /* U+F240 "" */
    0xff, 0xa0, 0x3f, 0xf7, 0xff, 0xff, 0xc0,

    /* U+F241 "" */
    0xff, 0xa0, 0x3f, 0xe7, 0xfb, 0xff, 0xc0,

    /* U+F242 "" */
    0xff, 0xa0, 0x3f, 0x87, 0xe3, 0xff, 0xc0,

    /* U+F243 "" */
    0xff, 0xa0, 0x3f, 0x7, 0xc3, 0xff, 0xc0,

    /* U+F244 "" */
    0xff, 0xa0, 0x38, 0x6, 0x3, 0xff, 0x80,

    /* U+F287 "" */
    0xe, 0x4, 0xd, 0xb, 0xff, 0xc8, 0x81, 0xc0,

    /* U+F293 "" */
    0x7b, 0xbb, 0x73, 0xce, 0xde, 0xde,

    /* U+F2ED "" */
    0xff, 0xff, 0xff, 0x7e, 0xfd, 0xfb, 0xbe,

    /* U+F304 "" */
    0x6, 0xf, 0x1f, 0x3e, 0x7c, 0xf8, 0xf0, 0xe0,

    /* U+F55A "" */
    0x3f, 0xdf, 0xff, 0x9f, 0xe7, 0x7f, 0xcf, 0xf0,

    /* U+F7C2 "" */
    0x3e, 0xd7, 0xaf, 0xff, 0xff, 0xff, 0xff,

    /* U+F8A2 "" */
    0x1, 0x61, 0xff, 0x60
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 34, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 34, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 50, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 3, .adv_w = 90, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 79, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 11, .adv_w = 108, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 88, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 19, .adv_w = 27, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 20, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 22, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 24, .adv_w = 51, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 26, .adv_w = 74, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 28, .adv_w = 29, .box_w = 1, .box_h = 2, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 29, .adv_w = 49, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 30, .adv_w = 29, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 45, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 34, .adv_w = 85, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 47, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 73, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 73, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 47, .adv_w = 86, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 73, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 77, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 82, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 29, .box_w = 1, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 29, .box_w = 1, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 69, .adv_w = 74, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 71, .adv_w = 74, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 73, .adv_w = 74, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 75, .adv_w = 73, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 132, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 84, .adv_w = 94, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 97, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 93, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 106, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 86, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 103, .adv_w = 81, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 99, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 104, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 40, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 66, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 92, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 76, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 122, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 104, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 108, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 92, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 146, .adv_w = 93, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 91, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 144, .box_w = 9, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 86, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 83, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 84, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 185, .adv_w = 45, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 188, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 190, .adv_w = 75, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 192, .adv_w = 64, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 193, .adv_w = 77, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 194, .adv_w = 77, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 196, .adv_w = 87, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 73, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 87, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 78, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 208, .adv_w = 45, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 88, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 215, .adv_w = 87, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 36, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 36, .box_w = 3, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 222, .adv_w = 79, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 36, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 135, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 87, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 81, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 87, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 239, .adv_w = 87, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 243, .adv_w = 52, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 64, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 53, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 87, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 115, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 71, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 263, .adv_w = 67, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 45, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 268, .adv_w = 38, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 269, .adv_w = 45, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 272, .adv_w = 74, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 273, .adv_w = 54, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 275, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 276, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 284, .adv_w = 128, .box_w = 9, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 299, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 88, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 316, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 324, .adv_w = 144, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 332, .adv_w = 128, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 341, .adv_w = 144, .box_w = 9, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 356, .adv_w = 64, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 360, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 366, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 375, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 88, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 387, .adv_w = 112, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 392, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 399, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 413, .adv_w = 112, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 418, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 425, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 430, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 435, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 442, .adv_w = 112, .box_w = 7, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 443, .adv_w = 144, .box_w = 9, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 450, .adv_w = 160, .box_w = 11, .box_h = 8, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 461, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 470, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 112, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 480, .adv_w = 112, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 484, .adv_w = 160, .box_w = 9, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 492, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 128, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 507, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 515, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 529, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 80, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 554, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 561, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 568, .adv_w = 144, .box_w = 9, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 583, .adv_w = 96, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 590, .adv_w = 160, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 600, .adv_w = 160, .box_w = 10, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 607, .adv_w = 160, .box_w = 10, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 614, .adv_w = 160, .box_w = 10, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 621, .adv_w = 160, .box_w = 10, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 628, .adv_w = 160, .box_w = 10, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 635, .adv_w = 160, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 643, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 649, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 656, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 664, .adv_w = 160, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 672, .adv_w = 96, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 679, .adv_w = 129, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x1f72, 0xef51, 0xef58, 0xef5b, 0xef5c, 0xef5d, 0xef61,
    0xef63, 0xef65, 0xef69, 0xef6c, 0xef71, 0xef76, 0xef77, 0xef78,
    0xef8e, 0xef93, 0xef98, 0xef9b, 0xef9c, 0xef9d, 0xefa1, 0xefa2,
    0xefa3, 0xefa4, 0xefb7, 0xefb8, 0xefbe, 0xefc0, 0xefc1, 0xefc4,
    0xefc7, 0xefc8, 0xefc9, 0xefcb, 0xefe3, 0xefe5, 0xf014, 0xf015,
    0xf017, 0xf019, 0xf030, 0xf037, 0xf03a, 0xf043, 0xf06c, 0xf074,
    0xf0ab, 0xf13b, 0xf190, 0xf191, 0xf192, 0xf193, 0xf194, 0xf1d7,
    0xf1e3, 0xf23d, 0xf254, 0xf4aa, 0xf712, 0xf7f2
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 63475, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 62, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 0, 13, 14, 15, 16, 17,
    18, 19, 12, 20, 20, 0, 0, 0,
    21, 22, 23, 24, 25, 22, 26, 27,
    28, 29, 29, 30, 31, 32, 29, 29,
    22, 33, 34, 35, 3, 36, 30, 37,
    37, 38, 39, 40, 41, 42, 43, 0,
    44, 0, 45, 46, 47, 48, 49, 50,
    51, 45, 52, 52, 53, 48, 45, 45,
    46, 46, 54, 55, 56, 57, 51, 58,
    58, 59, 58, 60, 41, 0, 0, 9,
    61, 9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 12,
    18, 19, 20, 21, 21, 0, 0, 0,
    22, 23, 24, 25, 23, 25, 25, 25,
    23, 25, 25, 26, 25, 25, 25, 25,
    23, 25, 23, 25, 3, 27, 28, 29,
    29, 30, 31, 32, 33, 34, 35, 0,
    36, 0, 37, 38, 39, 39, 39, 0,
    39, 38, 40, 41, 38, 38, 42, 42,
    39, 42, 39, 42, 43, 44, 45, 46,
    46, 47, 46, 48, 0, 0, 35, 9,
    49, 9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 0, 3, -3, 0, 0,
    0, 0, -7, -8, 1, 6, 3, 2,
    -5, 1, 6, 0, 5, 1, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 8, 1, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, -4, 0, 0, 0, 0,
    0, -3, 2, 3, 0, 0, -1, 0,
    -1, 1, 0, -1, 0, -1, -1, -3,
    0, 0, 0, 0, -1, 0, 0, -2,
    -2, 0, 0, -1, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    -1, 0, -2, 0, -3, 0, -15, 0,
    0, -3, 0, 3, 4, 0, 0, -3,
    1, 1, 4, 3, -2, 3, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -3, -2, -6, 0, -5,
    -1, 0, 0, 0, 0, 0, 5, 0,
    -4, -1, 0, 0, 0, -2, 0, 0,
    -1, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, -1, 5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
    0, 1, 0, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 5, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    3, 1, 4, -1, 0, 0, 3, -1,
    -4, -18, 1, 3, 3, 0, -2, 0,
    5, 0, 4, 0, 4, 0, -12, 0,
    -2, 4, 0, 4, -1, 3, 1, 0,
    0, 0, -1, 0, 0, -2, 10, 0,
    10, 0, 4, 0, 5, 2, 2, 4,
    0, 0, 0, -5, 0, 0, 0, 0,
    0, -1, 0, 1, -2, -2, -3, 1,
    0, -1, 0, 0, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -7, 0, -8, 0, 0, 0,
    0, -1, 0, 13, -2, -2, 1, 1,
    -1, 0, -2, 1, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 8, 0, 0, -5, 0,
    4, 0, -9, -12, -9, -3, 4, 0,
    0, -9, 0, 2, -3, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 4, -16, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 0, 1, 0, 0, 0,
    0, 0, 1, 1, -2, -3, 0, 0,
    0, -1, 0, 0, -1, 0, 0, 0,
    -3, 0, -1, 0, -3, -3, 0, -3,
    -4, -4, -2, 0, -3, 0, -3, 0,
    0, 0, 0, -1, 0, 0, 1, 0,
    1, -1, 0, 0, 0, 0, 0, 1,
    -1, 0, 0, 0, -1, 1, 1, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, -1, 0,
    -2, 0, -2, 0, 0, -1, 0, 4,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, -1, -1, 0, 0, -1, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -1, 0, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, -1, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, -1, -2, 0, -2, 0, -4,
    -1, -4, 3, 0, 0, -3, 1, 3,
    3, 0, -3, 0, -2, 0, 0, -6,
    1, -1, 1, -7, 1, 0, 0, 0,
    -7, 0, -7, -1, -11, -1, 0, -6,
    0, 3, 4, 0, 2, 0, 0, 0,
    0, 0, 0, -2, -2, 0, -4, 0,
    0, 0, -1, 0, 0, 0, -1, 0,
    0, 0, 0, 0, -1, -1, 0, -1,
    -2, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, 0, -1, -2, -1, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, 0, -2,
    0, -1, 0, -3, 1, 0, 0, -2,
    1, 1, 1, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 1,
    0, 0, -1, 0, -1, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    -1, 0, 0, 0, 0, -1, -2, 0,
    -2, 0, 4, -1, 0, -4, 0, 0,
    3, -6, -7, -5, -3, 1, 0, -1,
    -8, -2, 0, -2, 0, -3, 2, -2,
    -8, 0, -3, 0, 0, 1, 0, 1,
    -1, 0, 1, 0, -4, -5, 0, -6,
    -3, -3, -3, -4, -2, -3, 0, -2,
    -3, 1, 0, 0, 0, -1, 0, 0,
    0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, 0, 0, -1, 0, -2, -3,
    -3, 0, 0, -4, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 1,
    -1, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 6, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    -2, 0, 0, 0, 0, -6, -4, 0,
    0, 0, -2, -6, 0, 0, -1, 1,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, -2, 0,
    0, 0, 0, 2, 0, 1, -3, -3,
    0, -1, -1, -2, 0, 0, 0, 0,
    0, 0, -4, 0, -1, 0, -2, -1,
    0, -3, -3, -4, -1, 0, -3, 0,
    -4, 0, 0, 0, 0, 10, 0, 0,
    1, 0, 0, -2, 0, 1, 0, -6,
    0, 0, 0, 0, 0, -12, -2, 4,
    4, -1, -5, 0, 1, -2, 0, -6,
    -1, -2, 1, -9, -1, 2, 0, 2,
    -4, -2, -5, -4, -5, 0, 0, -8,
    0, 7, 0, 0, -1, 0, 0, 0,
    -1, -1, -1, -3, -4, 0, -12, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, -1, -1, -2, 0, 0,
    -3, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, 3,
    0, 2, 0, -3, 1, -1, 0, -3,
    -1, 0, -2, -1, -1, 0, -2, -2,
    0, 0, -1, 0, -1, -2, -2, 0,
    0, -1, 0, 1, -1, 0, -3, 0,
    0, 0, -3, 0, -2, 0, -2, -2,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 1, 0, -2, 0, -1, -2,
    -4, -1, -1, -1, 0, -1, -2, 0,
    0, 0, 0, 0, 0, -1, -1, -1,
    0, 0, 0, 0, 2, -1, 0, -1,
    0, 0, 0, -1, -2, -1, -1, -2,
    -1, 0, 1, 5, 0, 0, -3, 0,
    -1, 3, 0, -1, -5, -2, 2, 0,
    0, -6, -2, 1, -2, 1, 0, -1,
    -1, -4, 0, -2, 1, 0, 0, -2,
    0, 0, 0, 1, 1, -3, -2, 0,
    -2, -1, -2, -1, -1, 0, -2, 1,
    -2, -2, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    0, 0, -1, -1, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    -2, 0, -3, 0, 0, 0, -4, 0,
    1, -3, 3, 0, -1, -6, 0, 0,
    -3, -1, 0, -5, -3, -4, 0, 0,
    -6, -1, -5, -5, -6, 0, -3, 0,
    1, 9, -2, 0, -3, -1, 0, -1,
    -2, -3, -2, -5, -5, -3, -1, 0,
    0, -1, 0, 0, 0, 0, -9, -1,
    4, 3, -3, -5, 0, 0, -4, 0,
    -6, -1, -1, 3, -12, -2, 0, 0,
    0, -8, -2, -7, -1, -9, 0, 0,
    -9, 0, 8, 0, 0, -1, 0, 0,
    0, 0, -1, -1, -5, -1, 0, -8,
    0, 0, 0, 0, -4, 0, -1, 0,
    0, -4, -6, 0, 0, -1, -2, -4,
    -1, 0, -1, 0, 0, 0, 0, -6,
    -1, -4, -4, -1, -2, -3, -1, -2,
    0, -3, -1, -4, -2, 0, -2, -2,
    -1, -2, 0, 1, 0, -1, -4, 0,
    3, 0, -2, 0, 0, 0, 0, 2,
    0, 1, -3, 5, 0, -1, -1, -2,
    0, 0, 0, 0, 0, 0, -4, 0,
    -1, 0, -2, -1, 0, -3, -3, -4,
    -1, 0, -3, 1, 5, 0, 0, 0,
    0, 10, 0, 0, 1, 0, 0, -2,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -3, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -1, -1, 0, 0, -3,
    -1, 0, 0, -3, 0, 2, -1, 0,
    0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 2, 3, 1, -1, 0, -4,
    -2, 0, 4, -4, -4, -3, -3, 5,
    2, 1, -11, -1, 3, -1, 0, -1,
    1, -1, -4, 0, -1, 1, -2, -1,
    -4, -1, 0, 0, 4, 3, 0, -4,
    0, -7, -2, 4, -2, -5, 0, -2,
    -4, -4, -1, 5, 1, 0, -2, 0,
    -3, 0, 1, 4, -3, -5, -5, -3,
    4, 0, 0, -9, -1, 1, -2, -1,
    -3, 0, -3, -5, -2, -2, -1, 0,
    0, -3, -3, -1, 0, 4, 3, -1,
    -7, 0, -7, -2, 0, -4, -7, 0,
    -4, -2, -4, -4, 3, 0, 0, -2,
    0, -3, -1, 0, -1, -2, 0, 2,
    -4, 1, 0, 0, -7, 0, -1, -3,
    -2, -1, -4, -3, -4, -3, 0, -4,
    -1, -3, -2, -4, -1, 0, 0, 0,
    6, -2, 0, -4, -1, 0, -1, -3,
    -3, -3, -4, -5, -2, -3, 3, 0,
    -2, 0, -6, -2, 1, 3, -4, -5,
    -3, -4, 4, -1, 1, -12, -2, 3,
    -3, -2, -5, 0, -4, -5, -2, -1,
    -1, -1, -3, -4, 0, 0, 0, 4,
    4, -1, -8, 0, -8, -3, 3, -5,
    -9, -3, -4, -5, -6, -4, 3, 0,
    0, 0, 0, -2, 0, 0, 1, -2,
    3, 1, -2, 3, 0, 0, -4, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 1, 4, 0, 0, -2, 0, 0,
    0, 0, -1, -1, -2, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    -1, 0, 5, 0, 2, 0, 0, -2,
    0, 3, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -8, 0, -1, 2, 0, 4,
    0, 0, 13, 2, -3, -3, 1, 1,
    -1, 0, -6, 0, 0, 6, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -9, 5, 18, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, -2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, -3, 0,
    0, 0, 0, 0, 1, 17, -3, -1,
    4, 3, -3, 1, 0, 0, 1, 1,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -17, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, -3, 0, 0, 0, 0,
    -3, -1, 0, 0, 0, -3, 0, -2,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, -2, 0, -2, 0,
    -3, 0, 0, 0, -2, 1, -2, 0,
    0, -3, -1, -3, 0, 0, -3, 0,
    -1, 0, -6, 0, -1, 0, 0, -10,
    -2, -5, -1, -5, 0, 0, -9, 0,
    -3, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -2, -2, -1, -2, 0, 0,
    0, 0, -3, 0, -3, 2, -1, 3,
    0, -1, -3, -1, -2, -2, 0, -2,
    -1, -1, 1, -3, 0, 0, 0, 0,
    -11, -1, -2, 0, -3, 0, -1, -6,
    -1, 0, 0, -1, -1, 0, 0, 0,
    0, 1, 0, -1, -2, -1, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0,
    0, -3, 0, -1, 0, 0, 0, -3,
    1, 0, 0, 0, -3, -1, -3, 0,
    0, -4, 0, -1, 0, -6, 0, 0,
    0, 0, -12, 0, -3, -5, -6, 0,
    0, -9, 0, -1, -2, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -2, -1,
    -2, 0, 0, 0, 2, -2, 0, 4,
    6, -1, -1, -4, 2, 6, 2, 3,
    -3, 2, 5, 2, 4, 3, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 8, 6, -2, -1, 0, -1,
    10, 6, 10, 0, 0, 0, 1, 0,
    0, 5, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 2,
    0, 0, 0, 0, -11, -2, -1, -5,
    -6, 0, 0, -9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, -11, -2, -1,
    -5, -6, 0, 0, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, -3, 1, 0, -1,
    1, 2, 1, -4, 0, 0, -1, 1,
    0, 1, 0, 0, 0, 0, -3, 0,
    -1, -1, -3, 0, -1, -5, 0, 8,
    -1, 0, -3, -1, 0, -1, -2, 0,
    -1, -4, -3, -2, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, -11,
    -2, -1, -5, -6, 0, 0, -9, 0,
    0, 0, 0, 0, 0, 6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, -4, -2, -1, 4, -1, -1,
    -5, 0, -1, 0, -1, -3, 0, 3,
    0, 1, 0, 1, -3, -5, -2, 0,
    -5, -2, -3, -5, -5, 0, -2, -3,
    -2, -2, -1, -1, -2, -1, 0, -1,
    0, 2, 0, 2, -1, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -1, -1, 0, 0,
    -3, 0, -1, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1, 0, -2,
    0, 0, 0, 0, -1, 0, 0, -2,
    -1, 1, 0, -2, -2, -1, 0, -4,
    -1, -3, -1, -2, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 4, 0, 0, -2, 0,
    0, 0, 0, -2, 0, -1, 0, 0,
    -1, 0, 0, -1, 0, -3, 0, 0,
    5, -2, -4, -4, 1, 1, 1, 0,
    -4, 1, 2, 1, 4, 1, 4, -1,
    -3, 0, 0, -5, 0, 0, -4, -3,
    0, 0, -3, 0, -2, -2, 0, -2,
    0, -2, 0, -1, 2, 0, -1, -4,
    -1, 5, 0, 0, -1, 0, -3, 0,
    0, 2, -3, 0, 1, -1, 1, 0,
    0, -4, 0, -1, 0, 0, -1, 1,
    -1, 0, 0, 0, -5, -2, -3, 0,
    -4, 0, 0, -6, 0, 5, -1, 0,
    -2, 0, 1, 0, -1, 0, -1, -4,
    0, -1, 1, 0, 0, 0, 0, -1,
    0, 0, 1, -2, 0, 0, 0, -2,
    -1, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, 0, 3, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 3, 0, 3,
    0, 0, 0, 0, 0, -8, -7, 0,
    6, 4, 2, -5, 1, 5, 0, 5,
    0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 61,
    .right_class_cnt     = 49,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_montserrat_8 = {
#else
lv_font_t lv_font_montserrat_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 8,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_MONTSERRAT_8*/

