/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --no-compress --no-prefilter --bpp 1 --size 14 --font Montserrat-Medium.ttf -r 0x20-0x7F,0xB0,0x2022 --font FontAwesome5-Solid+Brands+Regular.woff -r 61441,61448,61451,61452,61452,61453,61457,61459,61461,61465,61468,61473,61478,61479,61480,61502,61507,61512,61515,61516,61517,61521,61522,61523,61524,61543,61544,61550,61552,61553,61556,61559,61560,61561,61563,61587,61589,61636,61637,61639,61641,61664,61671,61674,61683,61724,61732,61787,61931,62016,62017,62018,62019,62020,62087,62099,62212,62189,62810,63426,63650 --format lvgl -o lv_font_montserrat_14.c --force-fast-kern-format
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_MONTSERRAT_14
#define LV_FONT_MONTSERRAT_14 1
#endif

#if LV_FONT_MONTSERRAT_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xaa, 0xa8, 0xf0,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x11, 0x11, 0x3f, 0xe4, 0x42, 0x21, 0x13, 0xfe,
    0x44, 0x66, 0x22, 0x0,

    /* U+0024 "$" */
    0x10, 0x20, 0x43, 0xed, 0x12, 0x24, 0x38, 0x1c,
    0x24, 0x4c, 0xb7, 0xc2, 0x4, 0x0,

    /* U+0025 "%" */
    0x70, 0x91, 0x22, 0x24, 0x45, 0x7, 0x5c, 0xc,
    0x42, 0x88, 0x91, 0x12, 0x24, 0x38,

    /* U+0026 "&" */
    0x38, 0x44, 0x44, 0x68, 0x30, 0x59, 0x8d, 0x86,
    0xc7, 0x7d,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x4b, 0x49, 0x24, 0x92, 0x64, 0x80,

    /* U+0029 ")" */
    0x49, 0x12, 0x49, 0x24, 0xa4, 0x80,

    /* U+002A "*" */
    0x25, 0x5d, 0xf2, 0x0,

    /* U+002B "+" */
    0x10, 0x4f, 0xc4, 0x10, 0x40,

    /* U+002C "," */
    0xfe,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x46, 0x21, 0x18, 0x84, 0x62, 0x11, 0x88,
    0x40,

    /* U+0030 "0" */
    0x3c, 0x42, 0xc3, 0x81, 0x81, 0x81, 0x81, 0xc3,
    0x42, 0x3c,

    /* U+0031 "1" */
    0xe4, 0x92, 0x49, 0x24,

    /* U+0032 "2" */
    0x79, 0x18, 0x10, 0x20, 0xc3, 0x4, 0x10, 0x41,
    0xfc,

    /* U+0033 "3" */
    0xfc, 0x8, 0x20, 0xc3, 0xc1, 0xc0, 0x81, 0x87,
    0xf0,

    /* U+0034 "4" */
    0x4, 0x6, 0x6, 0x6, 0x6, 0x22, 0x13, 0xfe,
    0x4, 0x2, 0x1, 0x0,

    /* U+0035 "5" */
    0x7e, 0x81, 0x2, 0x7, 0xc0, 0xc0, 0x81, 0xc6,
    0xf8,

    /* U+0036 "6" */
    0x3e, 0x83, 0x4, 0xb, 0xd8, 0xe0, 0xc1, 0x46,
    0x78,

    /* U+0037 "7" */
    0xff, 0x82, 0x86, 0x4, 0xc, 0x8, 0x18, 0x10,
    0x30, 0x20,

    /* U+0038 "8" */
    0x3c, 0x42, 0x42, 0x42, 0x3c, 0xc3, 0x81, 0x81,
    0xc3, 0x3c,

    /* U+0039 "9" */
    0x79, 0x8a, 0xc, 0x1c, 0x6f, 0x40, 0x83, 0x5,
    0xf0,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0xf0, 0xf, 0xe0,

    /* U+003C "<" */
    0x2, 0x1c, 0xe6, 0xe, 0x3, 0x81, 0x80,

    /* U+003D "=" */
    0xfc, 0x0, 0x3f,

    /* U+003E ">" */
    0x1, 0xc0, 0xe0, 0x71, 0xce, 0x20, 0x0,

    /* U+003F "?" */
    0x7d, 0x8c, 0x8, 0x10, 0xc3, 0x4, 0x0, 0x10,
    0x20,

    /* U+0040 "@" */
    0xf, 0x81, 0x83, 0x19, 0xdc, 0x91, 0xa9, 0x4,
    0xc8, 0x26, 0x41, 0x32, 0x9, 0x88, 0xca, 0x3b,
    0x98, 0x0, 0x60, 0x0, 0xf8, 0x0,

    /* U+0041 "A" */
    0xc, 0x3, 0x1, 0xa0, 0x48, 0x31, 0x8, 0x43,
    0xf9, 0x2, 0x40, 0xf0, 0x10,

    /* U+0042 "B" */
    0xfe, 0x83, 0x81, 0x83, 0xfe, 0x83, 0x81, 0x81,
    0x83, 0xfe,

    /* U+0043 "C" */
    0x1e, 0x30, 0x90, 0x10, 0x8, 0x4, 0x2, 0x0,
    0x80, 0x61, 0xf, 0x0,

    /* U+0044 "D" */
    0xfc, 0x41, 0xa0, 0x50, 0x18, 0xc, 0x6, 0x3,
    0x2, 0x83, 0x7e, 0x0,

    /* U+0045 "E" */
    0xff, 0x2, 0x4, 0xf, 0xd0, 0x20, 0x40, 0x81,
    0xfc,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0xa0, 0x40, 0x81,
    0x0,

    /* U+0047 "G" */
    0x1f, 0x30, 0xd0, 0x10, 0x8, 0x4, 0x6, 0x2,
    0x81, 0x60, 0x8f, 0x80,

    /* U+0048 "H" */
    0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81, 0x81,
    0x81, 0x81,

    /* U+0049 "I" */
    0xff, 0xc0,

    /* U+004A "J" */
    0x7c, 0x10, 0x41, 0x4, 0x10, 0x41, 0x45, 0xe0,

    /* U+004B "K" */
    0x82, 0x86, 0x8c, 0x98, 0xb0, 0xf8, 0xc8, 0x8c,
    0x86, 0x83,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x83, 0xf0,

    /* U+004D "M" */
    0x80, 0x70, 0x3c, 0xf, 0x87, 0xb3, 0x64, 0x99,
    0xe6, 0x31, 0x8c, 0x60, 0x10,

    /* U+004E "N" */
    0x81, 0xc1, 0xe1, 0xb1, 0x99, 0x99, 0x8d, 0x87,
    0x83, 0x81,

    /* U+004F "O" */
    0x1e, 0x18, 0x64, 0xa, 0x1, 0x80, 0x60, 0x18,
    0x5, 0x2, 0x61, 0x87, 0x80,

    /* U+0050 "P" */
    0xfc, 0x82, 0x81, 0x81, 0x81, 0x82, 0xfc, 0x80,
    0x80, 0x80,

    /* U+0051 "Q" */
    0x1e, 0xc, 0x31, 0x2, 0x40, 0x28, 0x5, 0x0,
    0xa0, 0x12, 0x4, 0x61, 0x87, 0xe0, 0x19, 0x1,
    0xe0,

    /* U+0052 "R" */
    0xfc, 0x82, 0x81, 0x81, 0x81, 0x83, 0xfe, 0x86,
    0x82, 0x81,

    /* U+0053 "S" */
    0x7d, 0x82, 0x4, 0x7, 0x7, 0x81, 0x81, 0x86,
    0xf8,

    /* U+0054 "T" */
    0xff, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8,

    /* U+0055 "U" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x42, 0x3c,

    /* U+0056 "V" */
    0xc0, 0xd0, 0x26, 0x19, 0x84, 0x21, 0xc, 0xc1,
    0x20, 0x78, 0xc, 0x3, 0x0,

    /* U+0057 "W" */
    0x41, 0x82, 0x83, 0xd, 0x8e, 0x19, 0x16, 0x22,
    0x24, 0xc6, 0xc9, 0x85, 0x1a, 0xa, 0x1c, 0x1c,
    0x38, 0x10, 0x60,

    /* U+0058 "X" */
    0x41, 0xb1, 0x8c, 0x83, 0x80, 0xc0, 0xe0, 0x58,
    0x64, 0x61, 0x20, 0xc0,

    /* U+0059 "Y" */
    0xc1, 0xa0, 0x88, 0x86, 0x41, 0x40, 0xe0, 0x20,
    0x10, 0x8, 0x4, 0x0,

    /* U+005A "Z" */
    0xff, 0x2, 0x6, 0xc, 0x18, 0x10, 0x30, 0x60,
    0x40, 0xff,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x49, 0xc0,

    /* U+005C "\\" */
    0x41, 0x6, 0x8, 0x20, 0xc1, 0x4, 0x18, 0x20,
    0x82, 0x4, 0x10,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x93, 0xc0,

    /* U+005E "^" */
    0x30, 0xc6, 0x92, 0x4a, 0x10,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0xcc,

    /* U+0061 "a" */
    0x7a, 0x10, 0x5f, 0x86, 0x18, 0xdf,

    /* U+0062 "b" */
    0x80, 0x80, 0x80, 0xbc, 0xc2, 0x81, 0x81, 0x81,
    0x81, 0xc2, 0xbc,

    /* U+0063 "c" */
    0x3c, 0x8e, 0x4, 0x8, 0x10, 0x11, 0x9e,

    /* U+0064 "d" */
    0x1, 0x1, 0x1, 0x3d, 0x43, 0x81, 0x81, 0x81,
    0x81, 0x43, 0x3d,

    /* U+0065 "e" */
    0x38, 0x8a, 0xf, 0xf8, 0x18, 0x11, 0x1e,

    /* U+0066 "f" */
    0x39, 0x4, 0x3e, 0x41, 0x4, 0x10, 0x41, 0x4,
    0x0,

    /* U+0067 "g" */
    0x3d, 0x43, 0x81, 0x81, 0x81, 0x81, 0x43, 0x3d,
    0x1, 0x42, 0x7c,

    /* U+0068 "h" */
    0x81, 0x2, 0x5, 0xec, 0x70, 0x60, 0xc1, 0x83,
    0x6, 0x8,

    /* U+0069 "i" */
    0xdf, 0xe0,

    /* U+006A "j" */
    0x11, 0x1, 0x11, 0x11, 0x11, 0x11, 0x1e,

    /* U+006B "k" */
    0x81, 0x2, 0x4, 0x29, 0x96, 0x3c, 0x68, 0x99,
    0x1a, 0x18,

    /* U+006C "l" */
    0xff, 0xe0,

    /* U+006D "m" */
    0xbc, 0xf6, 0x38, 0xe0, 0x83, 0x4, 0x18, 0x20,
    0xc1, 0x6, 0x8, 0x30, 0x41,

    /* U+006E "n" */
    0xbd, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xc1,

    /* U+006F "o" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3c,

    /* U+0070 "p" */
    0xbc, 0xc2, 0x81, 0x81, 0x81, 0x81, 0xc2, 0xbc,
    0x80, 0x80, 0x80,

    /* U+0071 "q" */
    0x3d, 0x43, 0x81, 0x81, 0x81, 0x81, 0x43, 0x3d,
    0x1, 0x1, 0x1,

    /* U+0072 "r" */
    0xbc, 0x88, 0x88, 0x88,

    /* U+0073 "s" */
    0x7e, 0x8, 0x3c, 0x3c, 0x18, 0x7e,

    /* U+0074 "t" */
    0x41, 0xf, 0x90, 0x41, 0x4, 0x10, 0x40, 0xf0,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x71, 0xbd,

    /* U+0076 "v" */
    0xc3, 0x42, 0x42, 0x64, 0x24, 0x3c, 0x18, 0x18,

    /* U+0077 "w" */
    0xc2, 0x12, 0x30, 0x91, 0x4c, 0xca, 0x42, 0x92,
    0x14, 0x70, 0xe3, 0x2, 0x18,

    /* U+0078 "x" */
    0x42, 0x64, 0x3c, 0x18, 0x18, 0x2c, 0x64, 0x42,

    /* U+0079 "y" */
    0xc3, 0x42, 0x42, 0x64, 0x24, 0x3c, 0x18, 0x18,
    0x10, 0x10, 0xe0,

    /* U+007A "z" */
    0xfc, 0x31, 0x84, 0x21, 0x8c, 0x3f,

    /* U+007B "{" */
    0x69, 0x24, 0xa2, 0x49, 0x24, 0x40,

    /* U+007C "|" */
    0xff, 0xfc,

    /* U+007D "}" */
    0xc4, 0x44, 0x44, 0x34, 0x44, 0x44, 0x48,

    /* U+007E "~" */
    0x65, 0x38,

    /* U+00B0 "°" */
    0x74, 0x63, 0x17, 0x0,

    /* U+2022 "•" */
    0xfc,

    /* U+F001 "" */
    0x0, 0x0, 0x0, 0x7c, 0x7, 0xf8, 0x7f, 0xf0,
    0xff, 0x61, 0xf0, 0xc3, 0x1, 0x86, 0x3, 0xc,
    0x6, 0x18, 0x3c, 0x30, 0xfb, 0xe1, 0xff, 0xc0,
    0x8f, 0x0, 0x0,

    /* U+F008 "" */
    0x9f, 0xf7, 0xc0, 0x79, 0x1, 0x7c, 0x7, 0x9f,
    0xf6, 0x7f, 0xdf, 0x1, 0xe4, 0x5, 0xf0, 0x1e,
    0x7f, 0xd0,

    /* U+F00B "" */
    0xf7, 0xff, 0xdf, 0xff, 0x7f, 0xc0, 0x0, 0xf7,
    0xff, 0xdf, 0xff, 0x7f, 0xc0, 0x0, 0x0, 0x3,
    0xdf, 0xff, 0x7f, 0xfd, 0xff,

    /* U+F00C "" */
    0x0, 0x1c, 0x0, 0xf0, 0x7, 0x90, 0x3c, 0xe1,
    0xe3, 0xcf, 0x7, 0xf8, 0xf, 0xc0, 0x1e, 0x0,
    0x10, 0x0,

    /* U+F00D "" */
    0x40, 0xbc, 0xf7, 0xf8, 0xfc, 0x1e, 0xf, 0xc7,
    0xfb, 0xcf, 0x40, 0x80,

    /* U+F011 "" */
    0x3, 0x0, 0xcc, 0xc7, 0x33, 0x98, 0xc6, 0xc3,
    0xf, 0xc, 0x3c, 0x30, 0xf0, 0xc3, 0xe0, 0x1d,
    0x80, 0x67, 0x87, 0x8f, 0xfc, 0xf, 0xc0,

    /* U+F013 "" */
    0xf, 0x0, 0x78, 0x17, 0xe9, 0xff, 0xef, 0xff,
    0xb8, 0x79, 0xc3, 0x8e, 0x1c, 0xf9, 0xff, 0xff,
    0x9f, 0xfc, 0x1e, 0x0, 0xf0, 0x1, 0x0,

    /* U+F015 "" */
    0x1, 0x9c, 0x3, 0xdc, 0xe, 0x7c, 0x1d, 0xbc,
    0x3b, 0xdc, 0x67, 0xe6, 0xcf, 0xfb, 0x1f, 0xf8,
    0x1f, 0xf8, 0x1e, 0x78, 0x1e, 0x78, 0x1e, 0x78,

    /* U+F019 "" */
    0x3, 0x80, 0xe, 0x0, 0x38, 0x0, 0xe0, 0x3,
    0x80, 0xe, 0x1, 0xff, 0x3, 0xf8, 0x7, 0xc0,
    0xe, 0xf, 0xd7, 0xff, 0xff, 0xff, 0xef, 0xff,
    0xf0,

    /* U+F01C "" */
    0x1f, 0xf8, 0x1f, 0xf8, 0x30, 0xc, 0x70, 0xe,
    0x60, 0x6, 0xe0, 0x7, 0xfc, 0x3f, 0xfc, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

    /* U+F021 "" */
    0xf, 0x8c, 0x7f, 0xb3, 0x87, 0xdc, 0xf, 0xe0,
    0xff, 0x3, 0xf0, 0x0, 0x0, 0x0, 0xfc, 0xf,
    0xf0, 0x6f, 0x3, 0xbe, 0x1c, 0xdf, 0xe3, 0x1f,
    0x0,

    /* U+F026 "" */
    0x2, 0xc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xe,
    0xc, 0x8,

    /* U+F027 "" */
    0x2, 0x1, 0x80, 0xe3, 0xf8, 0xfe, 0xff, 0x9f,
    0xe7, 0xfa, 0xe, 0x1, 0x80, 0x20,

    /* U+F028 "" */
    0x0, 0x8, 0x2, 0xc, 0x6, 0x26, 0xe, 0x32,
    0xfe, 0x19, 0xfe, 0xc9, 0xfe, 0x49, 0xfe, 0x49,
    0xfe, 0x89, 0xe, 0x13, 0x6, 0x32, 0x2, 0x6,
    0x0, 0xc,

    /* U+F03E "" */
    0xff, 0xff, 0x3f, 0xfc, 0xfb, 0xf3, 0xc7, 0xfe,
    0xf, 0x90, 0x3c, 0x0, 0xf0, 0x3, 0xff, 0xff,
    0xff, 0xf0,

    /* U+F043 "" */
    0xc, 0x3, 0x1, 0xe0, 0x78, 0x3f, 0x1f, 0xe7,
    0xfb, 0xff, 0xff, 0xf7, 0xfc, 0xfd, 0x9e, 0x7f,
    0x7, 0x80,

    /* U+F048 "" */
    0xc1, 0xe1, 0xf1, 0xf9, 0xfd, 0xff, 0xff, 0xff,
    0xbf, 0xcf, 0xe3, 0xf0, 0xf8, 0x30,

    /* U+F04B "" */
    0xe0, 0xf, 0x80, 0xfc, 0xf, 0xf0, 0xff, 0x8f,
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xf8, 0xfe,
    0xf, 0x80, 0xf0, 0x4, 0x0,

    /* U+F04C "" */
    0xf9, 0xff, 0x9f, 0xf9, 0xff, 0x9f, 0xf9, 0xff,
    0x9f, 0xf9, 0xff, 0x9f, 0xf9, 0xff, 0x9f, 0xf9,
    0xff, 0x9f,

    /* U+F04D "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff,

    /* U+F051 "" */
    0xc1, 0xf0, 0xfc, 0x7f, 0x3f, 0xdf, 0xff, 0xff,
    0xfb, 0xf9, 0xf8, 0xf8, 0x78, 0x30,

    /* U+F052 "" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0xfc, 0x7f, 0xe7,
    0xff, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff,
    0xff, 0xff,

    /* U+F053 "" */
    0x6, 0x1c, 0x71, 0xc7, 0x1c, 0x38, 0x38, 0x38,
    0x38, 0x38, 0x30,

    /* U+F054 "" */
    0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc3, 0x8e, 0x38,
    0xe3, 0x86, 0x0,

    /* U+F067 "" */
    0x6, 0x0, 0x60, 0x6, 0x0, 0x60, 0x6, 0xf,
    0xff, 0xff, 0xf0, 0x60, 0x6, 0x0, 0x60, 0x6,
    0x0, 0x60,

    /* U+F068 "" */
    0xff, 0xff, 0xff,

    /* U+F06E "" */
    0x7, 0xe0, 0x1c, 0x38, 0x39, 0x9c, 0x71, 0xce,
    0xf7, 0xcf, 0xf7, 0xcf, 0x73, 0x8e, 0x38, 0x1c,
    0x1c, 0x38, 0x7, 0xe0,

    /* U+F070 "" */
    0x40, 0x0, 0x38, 0x0, 0x7, 0xbf, 0x0, 0x78,
    0x70, 0xe, 0x4e, 0x8, 0xf9, 0xc7, 0x1f, 0x79,
    0xe3, 0xde, 0x38, 0x3f, 0x87, 0x7, 0xc0, 0xe0,
    0xe0, 0xf, 0x8e, 0x0, 0x1, 0xc0, 0x0, 0x10,

    /* U+F071 "" */
    0x1, 0x80, 0x3, 0xc0, 0x3, 0xc0, 0x7, 0xe0,
    0x7, 0xe0, 0xe, 0x70, 0x1e, 0x70, 0x1e, 0x78,
    0x3e, 0x7c, 0x3f, 0xfc, 0x7e, 0x7e, 0xfe, 0x7e,
    0xff, 0xff, 0xff, 0xfe,

    /* U+F074 "" */
    0x0, 0x10, 0x0, 0xef, 0xf, 0xfe, 0x7f, 0x19,
    0xb8, 0xc, 0x40, 0x60, 0x3, 0x84, 0x1d, 0xbb,
    0xe7, 0xff, 0xf, 0xc0, 0xe, 0x0, 0x10,

    /* U+F077 "" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0x9c, 0x70, 0xee,
    0x7, 0xc0, 0x30,

    /* U+F078 "" */
    0x40, 0x2e, 0x7, 0x70, 0xe3, 0x9c, 0x1f, 0x80,
    0xf0, 0x6, 0x0,

    /* U+F079 "" */
    0x18, 0x0, 0xf, 0x7f, 0x87, 0xff, 0xe3, 0xfc,
    0x18, 0x5a, 0x6, 0x6, 0x1, 0x81, 0x81, 0x68,
    0x60, 0xff, 0x1f, 0xff, 0x87, 0xfb, 0xc0, 0x0,
    0x60,

    /* U+F07B "" */
    0x7c, 0x3, 0xf8, 0xf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0,

    /* U+F093 "" */
    0x0, 0x0, 0xc, 0x0, 0x7c, 0x7, 0xf8, 0x3f,
    0xf0, 0xff, 0xc0, 0x78, 0x1, 0xe0, 0x7, 0x80,
    0x1e, 0xf, 0x37, 0xff, 0xff, 0xff, 0xaf, 0xff,
    0xf0,

    /* U+F095 "" */
    0x0, 0x3c, 0x1, 0xf0, 0x7, 0xc0, 0x1f, 0x0,
    0x7c, 0x0, 0xe0, 0x3, 0x80, 0x1c, 0x10, 0xf1,
    0xe7, 0x8f, 0xfc, 0x3f, 0xe0, 0x7e, 0x1, 0xc0,
    0x0,

    /* U+F0C4 "" */
    0x70, 0xf, 0x8f, 0xd9, 0xef, 0xbc, 0x7f, 0x81,
    0xf0, 0x1f, 0x7, 0xf8, 0xfb, 0xcd, 0x9e, 0xf8,
    0xf7, 0x0,

    /* U+F0C5 "" */
    0xf, 0xa0, 0x7d, 0xfb, 0xe1, 0xdf, 0xfe, 0xff,
    0xf7, 0xff, 0xbf, 0xfd, 0xff, 0xef, 0xff, 0x7f,
    0xfb, 0xff, 0xc0, 0xf, 0xf8, 0x7f, 0xc0,

    /* U+F0C7 "" */
    0xff, 0xc7, 0xff, 0x30, 0x1d, 0x80, 0xfc, 0x7,
    0xff, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xc7,
    0xff, 0xff, 0xff, 0xf0,

    /* U+F0C9 "" */
    0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xff, 0xf0, 0x0, 0x0, 0xf, 0xff, 0xff,
    0xf0,

    /* U+F0E0 "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0xdf,
    0xe7, 0xbf, 0x7f, 0x33, 0xff, 0x3f, 0xff, 0xff,
    0xff, 0xf0,

    /* U+F0E7 "" */
    0x7c, 0x3e, 0x1f, 0x1f, 0xf, 0xf7, 0xff, 0xfd,
    0xfc, 0xe, 0x6, 0x7, 0x3, 0x1, 0x80, 0x80,

    /* U+F0EA "" */
    0x1c, 0x7, 0xbc, 0x3f, 0xe1, 0xf0, 0xf, 0x7a,
    0x7b, 0xdb, 0xde, 0x1e, 0xf0, 0xf7, 0xff, 0xbf,
    0xfd, 0xfe, 0xf, 0xf0, 0x7f, 0x83, 0xf8,

    /* U+F0F3 "" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0xfc, 0x3f, 0xc3,
    0xfc, 0x3f, 0xc7, 0xfe, 0x7f, 0xef, 0xff, 0xff,
    0xf0, 0x0, 0xf, 0x0, 0x60,

    /* U+F11C "" */
    0xff, 0xff, 0xff, 0xff, 0xca, 0x53, 0xca, 0x53,
    0xff, 0xff, 0xe5, 0xa7, 0xe5, 0xa7, 0xff, 0xff,
    0xc8, 0x13, 0xff, 0xff, 0xff, 0xff,

    /* U+F124 "" */
    0x0, 0x1c, 0x1, 0xf0, 0x3f, 0x83, 0xfe, 0x3f,
    0xfb, 0xff, 0xcf, 0xff, 0x1f, 0xf8, 0x3, 0xe0,
    0xf, 0x0, 0x3c, 0x0, 0xe0, 0x3, 0x80, 0x4,
    0x0,

    /* U+F15B "" */
    0xfd, 0x9f, 0xbb, 0xf7, 0xfe, 0xf, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe,

    /* U+F1EB "" */
    0x0, 0x0, 0x3, 0xff, 0x3, 0xff, 0xf1, 0xe0,
    0x1e, 0xe0, 0x1, 0xd0, 0xfc, 0x20, 0xff, 0xc0,
    0x78, 0x78, 0x8, 0x4, 0x0, 0x0, 0x0, 0xc,
    0x0, 0x7, 0x80, 0x0, 0xc0, 0x0,

    /* U+F240 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7f,
    0xf7, 0xdf, 0xfc, 0xf7, 0xff, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F241 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7f,
    0xc7, 0xdf, 0xf0, 0xf7, 0xfc, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F242 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7e,
    0x7, 0xdf, 0x80, 0xf7, 0xe0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F243 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x70,
    0x7, 0xdc, 0x0, 0xf7, 0x0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F244 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x0,
    0x7, 0xc0, 0x0, 0xf0, 0x0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F287 "" */
    0x0, 0x70, 0x0, 0x7c, 0x0, 0x37, 0x0, 0x8,
    0x0, 0xf6, 0x1, 0xbf, 0xff, 0xff, 0x18, 0x18,
    0x2, 0x0, 0x0, 0xdc, 0x0, 0x1f, 0x0, 0x1,
    0xc0,

    /* U+F293 "" */
    0x1f, 0xf, 0xf1, 0xef, 0x7c, 0xec, 0xad, 0xc1,
    0xfc, 0x7f, 0x8f, 0xe0, 0xf9, 0x4b, 0xe3, 0x3c,
    0xe7, 0xb8, 0x3e, 0x0,

    /* U+F2ED "" */
    0xf, 0xf, 0xff, 0x0, 0x7, 0xfe, 0x7f, 0xe6,
    0xf6, 0x6f, 0x66, 0xf6, 0x6f, 0x66, 0xf6, 0x6f,
    0x66, 0xf6, 0x7f, 0xe7, 0xfe,

    /* U+F304 "" */
    0x0, 0x30, 0x1, 0xe0, 0x7, 0xc0, 0x6f, 0x3,
    0xd8, 0x1f, 0x80, 0xfe, 0x7, 0xf0, 0x3f, 0x81,
    0xfc, 0xf, 0xe0, 0x3f, 0x0, 0xf8, 0x3, 0x80,
    0x0,

    /* U+F55A "" */
    0xf, 0xff, 0x87, 0xff, 0xf3, 0xf7, 0x3d, 0xfc,
    0x8f, 0xff, 0x87, 0xff, 0xe1, 0xf7, 0xf2, 0x3c,
    0xfd, 0xcf, 0x1f, 0xff, 0xc3, 0xff, 0xe0,

    /* U+F7C2 "" */
    0x1f, 0xc7, 0xfd, 0xa9, 0xf5, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf7, 0xfc,

    /* U+F8A2 "" */
    0x0, 0x4, 0x0, 0x31, 0x80, 0xce, 0x3, 0xff,
    0xff, 0xff, 0xf3, 0x80, 0x6, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 60, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 60, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 88, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 6, .adv_w = 157, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 139, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 32, .adv_w = 189, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 154, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 47, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 57, .adv_w = 75, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 63, .adv_w = 76, .box_w = 3, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 69, .adv_w = 90, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 73, .adv_w = 130, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 78, .adv_w = 51, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 79, .adv_w = 86, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 80, .adv_w = 51, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 81, .adv_w = 79, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 90, .adv_w = 149, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 83, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 129, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 150, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 129, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 138, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 144, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 138, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 51, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 51, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 186, .adv_w = 130, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 193, .adv_w = 130, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 196, .adv_w = 130, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 203, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 232, .box_w = 13, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 234, .adv_w = 164, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 170, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 162, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 185, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 150, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 142, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 173, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 182, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 69, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 161, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 133, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 214, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 182, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 372, .adv_w = 188, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 385, .adv_w = 162, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 188, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 412, .adv_w = 163, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 422, .adv_w = 139, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 131, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 177, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 159, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 464, .adv_w = 252, .box_w = 15, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 151, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 495, .adv_w = 145, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 147, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 517, .adv_w = 75, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 523, .adv_w = 79, .box_w = 6, .box_h = 14, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 534, .adv_w = 75, .box_w = 3, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 540, .adv_w = 131, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 545, .adv_w = 112, .box_w = 7, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 546, .adv_w = 134, .box_w = 3, .box_h = 2, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 547, .adv_w = 134, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 137, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 79, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 598, .adv_w = 155, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 609, .adv_w = 153, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 619, .adv_w = 62, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 64, .box_w = 4, .box_h = 14, .ofs_x = -2, .ofs_y = -3},
    {.bitmap_index = 628, .adv_w = 138, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 638, .adv_w = 62, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 237, .box_w = 13, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 653, .adv_w = 153, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 660, .adv_w = 142, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 668, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 679, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 690, .adv_w = 92, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 93, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 152, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 715, .adv_w = 125, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 201, .box_w = 13, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 124, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 744, .adv_w = 125, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 755, .adv_w = 117, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 761, .adv_w = 79, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 767, .adv_w = 67, .box_w = 1, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 769, .adv_w = 79, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 776, .adv_w = 130, .box_w = 7, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 778, .adv_w = 94, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 782, .adv_w = 70, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 783, .adv_w = 224, .box_w = 15, .box_h = 14, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 810, .adv_w = 224, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 828, .adv_w = 224, .box_w = 14, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 849, .adv_w = 224, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 867, .adv_w = 154, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 879, .adv_w = 224, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 902, .adv_w = 224, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 925, .adv_w = 252, .box_w = 16, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 949, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 974, .adv_w = 252, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 996, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1021, .adv_w = 112, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1031, .adv_w = 168, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1045, .adv_w = 252, .box_w = 16, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1071, .adv_w = 224, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1089, .adv_w = 154, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1107, .adv_w = 196, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1121, .adv_w = 196, .box_w = 12, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1142, .adv_w = 196, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1160, .adv_w = 196, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1178, .adv_w = 196, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1192, .adv_w = 196, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1210, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1221, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1232, .adv_w = 196, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1250, .adv_w = 196, .box_w = 12, .box_h = 2, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 1253, .adv_w = 252, .box_w = 16, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1273, .adv_w = 280, .box_w = 18, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1305, .adv_w = 252, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1333, .adv_w = 224, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1356, .adv_w = 196, .box_w = 12, .box_h = 7, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1367, .adv_w = 196, .box_w = 12, .box_h = 7, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1378, .adv_w = 280, .box_w = 18, .box_h = 11, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 1403, .adv_w = 224, .box_w = 14, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1423, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1448, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1473, .adv_w = 196, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1491, .adv_w = 196, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1514, .adv_w = 196, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1534, .adv_w = 196, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1551, .adv_w = 224, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1569, .adv_w = 140, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1585, .adv_w = 196, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1608, .adv_w = 196, .box_w = 12, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1629, .adv_w = 252, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1651, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1676, .adv_w = 168, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1694, .adv_w = 280, .box_w = 18, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1724, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1745, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1766, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1787, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1808, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1829, .adv_w = 280, .box_w = 18, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1854, .adv_w = 196, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1874, .adv_w = 196, .box_w = 12, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1895, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1920, .adv_w = 280, .box_w = 18, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1943, .adv_w = 168, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1961, .adv_w = 225, .box_w = 14, .box_h = 8, .ofs_x = 0, .ofs_y = 1}
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
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 2, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 10, 0, 6, -5, 0, 0,
    0, 0, -12, -13, 2, 11, 5, 4,
    -9, 2, 11, 1, 9, 2, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 13, 2, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, -7, 0, 0, 0, 0,
    0, -4, 4, 4, 0, 0, -2, 0,
    -2, 2, 0, -2, 0, -2, -1, -4,
    0, 0, 0, 0, -2, 0, 0, -3,
    -3, 0, 0, -2, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -2, 0, -3, 0, -6, 0, -27, 0,
    0, -4, 0, 4, 7, 0, 0, -4,
    2, 2, 7, 4, -4, 4, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, -3, -11, 0, -9,
    -2, 0, 0, 0, 0, 0, 9, 0,
    -7, -2, -1, 1, 0, -4, 0, 0,
    -2, -17, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -18, -2, 9,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -9, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 7,
    0, 2, 0, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 9, 2,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2,
    4, 2, 7, -2, 0, 0, 4, -2,
    -7, -31, 2, 6, 4, 0, -3, 0,
    8, 0, 7, 0, 7, 0, -21, 0,
    -3, 7, 0, 7, -2, 4, 2, 0,
    0, 1, -2, 0, 0, -4, 18, 0,
    18, 0, 7, 0, 9, 3, 4, 7,
    0, 0, 0, -8, 0, 0, 0, 0,
    1, -2, 0, 2, -4, -3, -4, 2,
    0, -2, 0, 0, 0, -9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -15, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, -12, 0, -14, 0, 0, 0,
    0, -2, 0, 22, -3, -3, 2, 2,
    -2, 0, -3, 2, 0, 0, -12, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -22, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -14, 0, 13, 0, 0, -8, 0,
    7, 0, -15, -22, -15, -4, 7, 0,
    0, -15, 0, 3, -5, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 6, 7, -27, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 11, 0, 2, 0, 0, 0,
    0, 0, 2, 2, -3, -4, 0, -1,
    -1, -2, 0, 0, -2, 0, 0, 0,
    -4, 0, -2, 0, -5, -4, 0, -6,
    -7, -7, -4, 0, -4, 0, -4, 0,
    0, 0, 0, -2, 0, 0, 2, 0,
    2, -2, 0, 1, 0, 0, 0, 2,
    -2, 0, 0, 0, -2, 2, 2, -1,
    0, 0, 0, -4, 0, -1, 0, 0,
    0, 0, 0, 1, 0, 3, -2, 0,
    -3, 0, -4, 0, 0, -2, 0, 7,
    0, 0, -2, 0, 0, 0, 0, 0,
    -1, 1, -2, -2, 0, 0, -2, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -1, 0, -2, -3, 0,
    0, 0, 0, 0, 1, 0, 0, -2,
    0, -2, -2, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, -2, -3, 0, -3, 0, -7,
    -2, -7, 4, 0, 0, -4, 2, 4,
    6, 0, -6, -1, -3, 0, -1, -11,
    2, -2, 2, -12, 2, 0, 0, 1,
    -12, 0, -12, -2, -19, -2, 0, -11,
    0, 4, 6, 0, 3, 0, 0, 0,
    0, 0, 0, -4, -3, 0, -7, 0,
    0, 0, -2, 0, 0, 0, -2, 0,
    0, 0, 0, 0, -1, -1, 0, -1,
    -3, 0, 0, 0, 0, 0, 0, 0,
    -2, -2, 0, -2, -3, -2, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 0, -3,
    0, -2, 0, -4, 2, 0, 0, -3,
    1, 2, 2, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 2,
    0, 0, -2, 0, -2, -2, -3, 0,
    0, 0, 0, 0, 0, 0, 2, 0,
    -2, 0, 0, 0, 0, -2, -3, 0,
    -4, 0, 7, -2, 1, -7, 0, 0,
    6, -11, -12, -9, -4, 2, 0, -2,
    -15, -4, 0, -4, 0, -4, 3, -4,
    -14, 0, -6, 0, 0, 1, -1, 2,
    -2, 0, 2, 0, -7, -9, 0, -11,
    -5, -5, -5, -7, -3, -6, 0, -4,
    -6, 1, 0, 1, 0, -2, 0, 0,
    0, 2, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, -1, 0, -1, -2, 0, -4, -5,
    -5, -1, 0, -7, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 1,
    -1, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 11, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    -4, 0, 0, 0, 0, -11, -7, 0,
    0, 0, -3, -11, 0, 0, -2, 2,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, -4, 0,
    0, 0, 0, 3, 0, 2, -4, -4,
    0, -2, -2, -3, 0, 0, 0, 0,
    0, 0, -7, 0, -2, 0, -3, -2,
    0, -5, -6, -7, -2, 0, -4, 0,
    -7, 0, 0, 0, 0, 18, 0, 0,
    1, 0, 0, -3, 0, 2, 0, -10,
    0, 0, 0, 0, 0, -21, -4, 7,
    7, -2, -9, 0, 2, -3, 0, -11,
    -1, -3, 2, -16, -2, 3, 0, 3,
    -8, -3, -8, -7, -9, 0, 0, -13,
    0, 13, 0, 0, -1, 0, 0, 0,
    -1, -1, -2, -6, -7, 0, -21, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, -1, -2, -3, 0, 0,
    -4, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -4, 0, 0, 4,
    -1, 3, 0, -5, 2, -2, -1, -6,
    -2, 0, -3, -2, -2, 0, -3, -4,
    0, 0, -2, -1, -2, -4, -3, 0,
    0, -2, 0, 2, -2, 0, -5, 0,
    0, 0, -4, 0, -4, 0, -4, -4,
    2, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 2, 0, -3, 0, -2, -3,
    -7, -2, -2, -2, -1, -2, -3, -1,
    0, 0, 0, 0, 0, -2, -2, -2,
    0, 0, 0, 0, 3, -2, 0, -2,
    0, 0, 0, -2, -3, -2, -2, -3,
    -2, 0, 2, 9, -1, 0, -6, 0,
    -2, 4, 0, -2, -9, -3, 3, 0,
    0, -11, -4, 2, -4, 2, 0, -2,
    -2, -7, 0, -3, 1, 0, 0, -4,
    0, 0, 0, 2, 2, -4, -4, 0,
    -4, -2, -3, -2, -2, 0, -4, 1,
    -4, -4, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, -3,
    0, 0, -2, -2, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    -3, 0, -4, 0, 0, 0, -7, 0,
    2, -5, 4, 0, -2, -11, 0, 0,
    -5, -2, 0, -9, -6, -6, 0, 0,
    -10, -2, -9, -9, -11, 0, -6, 0,
    2, 15, -3, 0, -5, -2, -1, -2,
    -4, -6, -4, -8, -9, -5, -2, 0,
    0, -2, 0, 1, 0, 0, -16, -2,
    7, 5, -5, -8, 0, 1, -7, 0,
    -11, -2, -2, 4, -21, -3, 1, 0,
    0, -15, -3, -12, -2, -16, 0, 0,
    -16, 0, 13, 1, 0, -2, 0, 0,
    0, 0, -1, -2, -9, -2, 0, -15,
    0, 0, 0, 0, -7, 0, -2, 0,
    -1, -6, -11, 0, 0, -1, -3, -7,
    -2, 0, -2, 0, 0, 0, 0, -10,
    -2, -7, -7, -2, -4, -6, -2, -4,
    0, -4, -2, -7, -3, 0, -3, -4,
    -2, -4, 0, 1, 0, -2, -7, 0,
    4, 0, -4, 0, 0, 0, 0, 3,
    0, 2, -4, 9, 0, -2, -2, -3,
    0, 0, 0, 0, 0, 0, -7, 0,
    -2, 0, -3, -2, 0, -5, -6, -7,
    -2, 0, -4, 2, 9, 0, 0, 0,
    0, 18, 0, 0, 1, 0, 0, -3,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, -4, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, -2, 0, 0, -4,
    -2, 0, 0, -4, 0, 4, -1, 0,
    0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 3, 4, 2, -2, 0, -7,
    -4, 0, 7, -7, -7, -4, -4, 9,
    4, 2, -19, -2, 4, -2, 0, -2,
    2, -2, -8, 0, -2, 2, -3, -2,
    -7, -2, 0, 0, 7, 4, 0, -6,
    0, -12, -3, 6, -3, -9, 1, -3,
    -7, -7, -2, 9, 2, 0, -3, 0,
    -6, 0, 2, 7, -5, -8, -9, -6,
    7, 0, 1, -16, -2, 2, -4, -2,
    -5, 0, -5, -8, -3, -3, -2, 0,
    0, -5, -5, -2, 0, 7, 5, -2,
    -12, 0, -12, -3, 0, -8, -13, -1,
    -7, -4, -7, -6, 6, 0, 0, -3,
    0, -4, -2, 0, -2, -4, 0, 4,
    -7, 2, 0, 0, -12, 0, -2, -5,
    -4, -2, -7, -6, -7, -5, 0, -7,
    -2, -5, -4, -7, -2, 0, 0, 1,
    11, -4, 0, -7, -2, 0, -2, -4,
    -5, -6, -6, -9, -3, -4, 4, 0,
    -3, 0, -11, -3, 1, 4, -7, -8,
    -4, -7, 7, -2, 1, -21, -4, 4,
    -5, -4, -8, 0, -7, -9, -3, -2,
    -2, -2, -5, -7, -1, 0, 0, 7,
    6, -2, -15, 0, -13, -5, 5, -9,
    -15, -4, -8, -9, -11, -7, 4, 0,
    0, 0, 0, -3, 0, 0, 2, -3,
    4, 2, -4, 4, 0, 0, -7, -1,
    0, -1, 0, 1, 1, -2, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 2, 7, 0, 0, -3, 0, 0,
    0, 0, -2, -2, -3, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 2, 0,
    -2, 0, 9, 0, 4, 1, 1, -3,
    0, 4, 0, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 0, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -13, 0, -2, 4, 0, 7,
    0, 0, 22, 3, -4, -4, 2, 2,
    -2, 1, -11, 0, 0, 11, -13, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -15, 9, 31, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -13, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, -4,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, -6, 0,
    0, 1, 0, 0, 2, 29, -4, -2,
    7, 6, -6, 2, 0, 0, 2, 2,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -29, 6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, 0, 0, -6, 0, 0, 0, 0,
    -5, -1, 0, 0, 0, -5, 0, -3,
    0, -11, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, -2, 0, 0, -4, 0, -3, 0,
    -6, 0, 0, 0, -4, 2, -3, 0,
    0, -6, -2, -5, 0, 0, -6, 0,
    -2, 0, -11, 0, -2, 0, 0, -18,
    -4, -9, -2, -8, 0, 0, -15, 0,
    -6, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -3, -4, -2, -4, 0, 0,
    0, 0, -5, 0, -5, 3, -2, 4,
    0, -2, -5, -2, -4, -4, 0, -3,
    -1, -2, 2, -6, -1, 0, 0, 0,
    -20, -2, -3, 0, -5, 0, -2, -11,
    -2, 0, 0, -2, -2, 0, 0, 0,
    0, 2, 0, -2, -4, -2, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0,
    0, -5, 0, -2, 0, 0, 0, -4,
    2, 0, 0, 0, -6, -2, -4, 0,
    0, -6, 0, -2, 0, -11, 0, 0,
    0, 0, -22, 0, -4, -8, -11, 0,
    0, -15, 0, -2, -3, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -3, -1,
    -3, 1, 0, 0, 4, -3, 0, 7,
    11, -2, -2, -7, 3, 11, 4, 5,
    -6, 3, 9, 3, 6, 5, 6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 14, 11, -4, -2, 0, -2,
    18, 10, 18, 0, 0, 0, 2, 0,
    0, 8, 0, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, -19, -3, -2, -9,
    -11, 0, 0, -15, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, -19, -3, -2,
    -9, -11, 0, 0, -9, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, -5, 2, 0, -2,
    2, 4, 2, -7, 0, 0, -2, 2,
    0, 2, 0, 0, 0, 0, -6, 0,
    -2, -2, -4, 0, -2, -9, 0, 14,
    -2, 0, -5, -2, 0, -2, -4, 0,
    -2, -6, -4, -3, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, -19,
    -3, -2, -9, -11, 0, 0, -15, 0,
    0, 0, 0, 0, 0, 11, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 0, -7, -3, -2, 7, -2, -2,
    -9, 1, -1, 1, -2, -6, 0, 5,
    0, 2, 1, 2, -5, -9, -3, 0,
    -9, -4, -6, -9, -9, 0, -4, -4,
    -3, -3, -2, -2, -3, -2, 0, -2,
    -1, 3, 0, 3, -2, 0, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, -2, -2, 0, 0,
    -6, 0, -1, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 0, -3,
    0, 0, 0, 0, -2, 0, 0, -4,
    -2, 2, 0, -4, -4, -2, 0, -6,
    -2, -5, -2, -3, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -15, 0, 7, 0, 0, -4, 0,
    0, 0, 0, -3, 0, -2, 0, 0,
    -1, 0, 0, -2, 0, -5, 0, 0,
    9, -3, -7, -7, 2, 2, 2, 0,
    -6, 2, 3, 2, 7, 2, 7, -2,
    -6, 0, 0, -9, 0, 0, -7, -6,
    0, 0, -4, 0, -3, -4, 0, -3,
    0, -3, 0, -2, 3, 0, -2, -7,
    -2, 8, 0, 0, -2, 0, -4, 0,
    0, 3, -5, 0, 2, -2, 2, 0,
    0, -7, 0, -2, -1, 0, -2, 2,
    -2, 0, 0, 0, -9, -3, -5, 0,
    -7, 0, 0, -11, 0, 8, -2, 0,
    -4, 0, 1, 0, -2, 0, -2, -7,
    0, -2, 2, 0, 0, 0, 0, -2,
    0, 0, 2, -3, 1, 0, 0, -3,
    -2, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -14, 0, 5, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    -2, -2, 0, 0, 0, 4, 0, 5,
    0, 0, 0, 0, 0, -14, -13, 1,
    10, 7, 4, -9, 2, 9, 0, 8,
    0, 4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 12, 0, 0,
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
const lv_font_t lv_font_montserrat_14 = {
#else
lv_font_t lv_font_montserrat_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_MONTSERRAT_14*/

