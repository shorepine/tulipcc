/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --no-compress --no-prefilter --bpp 1 --size 12 --font Montserrat-Medium.ttf -r 0x20-0x7F,0xB0,0x2022 --font FontAwesome5-Solid+Brands+Regular.woff -r 61441,61448,61451,61452,61452,61453,61457,61459,61461,61465,61468,61473,61478,61479,61480,61502,61507,61512,61515,61516,61517,61521,61522,61523,61524,61543,61544,61550,61552,61553,61556,61559,61560,61561,61563,61587,61589,61636,61637,61639,61641,61664,61671,61674,61683,61724,61732,61787,61931,62016,62017,62018,62019,62020,62087,62099,62212,62189,62810,63426,63650 --format lvgl -o lv_font_montserrat_12.c --force-fast-kern-format
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_MONTSERRAT_12
#define LV_FONT_MONTSERRAT_12 1
#endif

#if LV_FONT_MONTSERRAT_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0x80,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x12, 0x22, 0x7f, 0x24, 0x24, 0x24, 0xff, 0x24,
    0x24,

    /* U+0024 "$" */
    0x10, 0x47, 0xa4, 0x93, 0x47, 0x87, 0x16, 0x57,
    0x84, 0x10,

    /* U+0025 "%" */
    0x62, 0x49, 0x25, 0x12, 0x86, 0xb0, 0xa4, 0x52,
    0x49, 0x23, 0x0,

    /* U+0026 "&" */
    0x30, 0x91, 0x23, 0x87, 0x13, 0x63, 0x46, 0x7a,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x5a, 0xaa, 0xa9, 0x40,

    /* U+0029 ")" */
    0xa5, 0x55, 0x5e, 0x80,

    /* U+002A "*" */
    0x27, 0xc9, 0xf2, 0x0,

    /* U+002B "+" */
    0x21, 0x3e, 0x42, 0x0,

    /* U+002C "," */
    0xe0,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x8, 0x84, 0x22, 0x10, 0x88, 0x42, 0x31, 0x8,
    0x0,

    /* U+0030 "0" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xa2, 0x38,

    /* U+0031 "1" */
    0xe4, 0x92, 0x49, 0x20,

    /* U+0032 "2" */
    0x7a, 0x10, 0x41, 0x8, 0x42, 0x10, 0xfc,

    /* U+0033 "3" */
    0xfc, 0x21, 0x8c, 0x38, 0x10, 0x61, 0xf8,

    /* U+0034 "4" */
    0x8, 0x30, 0x41, 0x6, 0x48, 0xbf, 0x82, 0x4,

    /* U+0035 "5" */
    0x7d, 0x4, 0x1e, 0xc, 0x10, 0x63, 0x78,

    /* U+0036 "6" */
    0x3d, 0x8, 0x2e, 0xce, 0x18, 0x53, 0x78,

    /* U+0037 "7" */
    0xff, 0xa, 0x30, 0x40, 0x82, 0x4, 0x18, 0x20,

    /* U+0038 "8" */
    0x79, 0xa, 0x14, 0x27, 0x98, 0xe0, 0xe3, 0x7c,

    /* U+0039 "9" */
    0x72, 0x28, 0x61, 0x7c, 0x10, 0x42, 0xf0,

    /* U+003A ":" */
    0xc6,

    /* U+003B ";" */
    0xc3, 0x80,

    /* U+003C "<" */
    0x1d, 0x88, 0x1c, 0xc,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0xc0, 0xe0, 0xcc, 0xc0,

    /* U+003F "?" */
    0xf4, 0x42, 0x11, 0x10, 0x80, 0x20,

    /* U+0040 "@" */
    0x1f, 0x4, 0x11, 0x3b, 0x68, 0xda, 0xb, 0x41,
    0x68, 0x2c, 0x8d, 0xce, 0xc8, 0x0, 0xc0, 0xf,
    0x80,

    /* U+0041 "A" */
    0x8, 0xe, 0x5, 0x6, 0x82, 0x23, 0x11, 0xfc,
    0x82, 0x81, 0x80,

    /* U+0042 "B" */
    0xfd, 0x6, 0xc, 0x1f, 0xd0, 0x60, 0xc1, 0xfc,

    /* U+0043 "C" */
    0x3c, 0xc7, 0x4, 0x8, 0x10, 0x30, 0x31, 0x3c,

    /* U+0044 "D" */
    0xfc, 0x86, 0x83, 0x81, 0x81, 0x81, 0x83, 0x86,
    0xfc,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0xfc,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x80,

    /* U+0047 "G" */
    0x3e, 0x61, 0xc0, 0x80, 0x81, 0x81, 0xc1, 0x61,
    0x3e,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1, 0x82,

    /* U+0049 "I" */
    0xff, 0x80,

    /* U+004A "J" */
    0x78, 0x42, 0x10, 0x84, 0x31, 0x70,

    /* U+004B "K" */
    0x87, 0x1a, 0x65, 0x8f, 0x1a, 0x22, 0x42, 0x86,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0xfc,

    /* U+004D "M" */
    0x80, 0xe0, 0xf0, 0x74, 0x5a, 0x2c, 0xa6, 0x73,
    0x11, 0x80, 0x80,

    /* U+004E "N" */
    0x83, 0x87, 0x8d, 0x99, 0x33, 0x63, 0xc3, 0x82,

    /* U+004F "O" */
    0x3e, 0x31, 0xb0, 0x70, 0x18, 0xc, 0x7, 0x6,
    0xc6, 0x3e, 0x0,

    /* U+0050 "P" */
    0xfd, 0xe, 0xc, 0x18, 0x7f, 0xa0, 0x40, 0x80,

    /* U+0051 "Q" */
    0x3e, 0x31, 0xb0, 0x70, 0x18, 0xc, 0x7, 0x6,
    0xc6, 0x3e, 0x2, 0x40, 0xe0,

    /* U+0052 "R" */
    0xfd, 0xe, 0xc, 0x18, 0x7f, 0xa3, 0x42, 0x82,

    /* U+0053 "S" */
    0x7a, 0x8, 0x30, 0x78, 0x30, 0x61, 0x78,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xa2, 0x38,

    /* U+0056 "V" */
    0x81, 0x20, 0x90, 0xcc, 0x42, 0x61, 0xa0, 0x50,
    0x38, 0x18, 0x0,

    /* U+0057 "W" */
    0x43, 0xa, 0x18, 0xd1, 0xc4, 0xca, 0x22, 0x49,
    0x16, 0x50, 0xe2, 0x83, 0xc, 0x18, 0x40,

    /* U+0058 "X" */
    0x42, 0x66, 0x24, 0x18, 0x18, 0x1c, 0x24, 0x62,
    0xc3,

    /* U+0059 "Y" */
    0x82, 0x8d, 0x11, 0x42, 0x82, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xfe, 0x8, 0x30, 0xc1, 0x4, 0x18, 0x20, 0xfe,

    /* U+005B "[" */
    0xea, 0xaa, 0xaa, 0xc0,

    /* U+005C "\\" */
    0x84, 0x21, 0x84, 0x21, 0x4, 0x21, 0x4, 0x21,
    0x0,

    /* U+005D "]" */
    0xd5, 0x55, 0x55, 0xc0,

    /* U+005E "^" */
    0x21, 0x14, 0xa8, 0xc4,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0xc2,

    /* U+0061 "a" */
    0x74, 0x42, 0xf8, 0xc5, 0xe0,

    /* U+0062 "b" */
    0x81, 0x2, 0x5, 0xcc, 0x50, 0x60, 0xc1, 0xc5,
    0x70,

    /* U+0063 "c" */
    0x79, 0x38, 0x20, 0x81, 0x37, 0x80,

    /* U+0064 "d" */
    0x2, 0x4, 0x9, 0xd4, 0x70, 0x60, 0xc1, 0x46,
    0x74,

    /* U+0065 "e" */
    0x7b, 0x28, 0x7f, 0x81, 0x7, 0x80,

    /* U+0066 "f" */
    0x34, 0x4f, 0x44, 0x44, 0x44,

    /* U+0067 "g" */
    0x3a, 0x8e, 0xc, 0x18, 0x28, 0xce, 0x81, 0x46,
    0xf8,

    /* U+0068 "h" */
    0x82, 0x8, 0x3e, 0xce, 0x18, 0x61, 0x86, 0x10,

    /* U+0069 "i" */
    0x9f, 0xc0,

    /* U+006A "j" */
    0x20, 0x12, 0x49, 0x24, 0x9c,

    /* U+006B "k" */
    0x82, 0x8, 0x22, 0x92, 0xcf, 0x34, 0x8a, 0x30,

    /* U+006C "l" */
    0xff, 0xc0,

    /* U+006D "m" */
    0xfb, 0xd9, 0xce, 0x10, 0xc2, 0x18, 0x43, 0x8,
    0x61, 0x8,

    /* U+006E "n" */
    0xfb, 0x38, 0x61, 0x86, 0x18, 0x40,

    /* U+006F "o" */
    0x38, 0x8a, 0xc, 0x18, 0x28, 0x8e, 0x0,

    /* U+0070 "p" */
    0xb9, 0x8a, 0xc, 0x18, 0x38, 0xae, 0x40, 0x81,
    0x0,

    /* U+0071 "q" */
    0x3a, 0x8e, 0xc, 0x18, 0x28, 0xce, 0x81, 0x2,
    0x4,

    /* U+0072 "r" */
    0xfa, 0x49, 0x20,

    /* U+0073 "s" */
    0x7c, 0x20, 0xe0, 0xc7, 0xc0,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x44, 0x30,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x87, 0x37, 0x40,

    /* U+0076 "v" */
    0x86, 0x89, 0x13, 0x42, 0x87, 0x4, 0x0,

    /* U+0077 "w" */
    0x84, 0x29, 0x89, 0x29, 0x25, 0x63, 0x28, 0x63,
    0xc, 0x60,

    /* U+0078 "x" */
    0x45, 0xa3, 0x84, 0x39, 0xa4, 0x40,

    /* U+0079 "y" */
    0x86, 0x89, 0x12, 0x42, 0x85, 0xc, 0x8, 0x21,
    0xc0,

    /* U+007A "z" */
    0xf8, 0xcc, 0x44, 0x63, 0xe0,

    /* U+007B "{" */
    0x69, 0x24, 0xa2, 0x49, 0x26,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xc9, 0x24, 0x8a, 0x49, 0x2c,

    /* U+007E "~" */
    0xea, 0x60,

    /* U+00B0 "°" */
    0x69, 0x96,

    /* U+2022 "•" */
    0xf0,

    /* U+F001 "" */
    0x0, 0x70, 0x3f, 0x1f, 0xf1, 0xfb, 0x1c, 0x31,
    0x83, 0x18, 0x31, 0x83, 0x19, 0xf7, 0x9f, 0xf8,
    0x47, 0x0,

    /* U+F008 "" */
    0xbf, 0xde, 0x7, 0xa0, 0x5e, 0x7, 0xbf, 0xde,
    0x7, 0xa0, 0x5e, 0x7, 0xbf, 0xd0,

    /* U+F00B "" */
    0xf7, 0xf7, 0xbf, 0xfd, 0xfe, 0x0, 0xf, 0x7f,
    0x7b, 0xff, 0xdf, 0xc0, 0x0, 0xf7, 0xf7, 0xbf,
    0xfd, 0xfc,

    /* U+F00C "" */
    0x0, 0x20, 0x7, 0x0, 0xe4, 0x1c, 0xe3, 0x87,
    0x70, 0x3e, 0x1, 0xc0, 0x8, 0x0,

    /* U+F00D "" */
    0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3,

    /* U+F011 "" */
    0x6, 0x2, 0x64, 0x76, 0xe6, 0x66, 0xc6, 0x3c,
    0x63, 0xc6, 0x3c, 0x3, 0x60, 0x67, 0xe, 0x3f,
    0xc0, 0xf0,

    /* U+F013 "" */
    0xe, 0x4, 0xf0, 0x7f, 0xef, 0xfe, 0x71, 0xe7,
    0xc, 0x71, 0xef, 0xfe, 0x7f, 0xe4, 0xf0, 0xe,
    0x0,

    /* U+F015 "" */
    0x3, 0x30, 0x1e, 0xc1, 0xcf, 0xc, 0xcc, 0x6f,
    0xdb, 0x7f, 0xb3, 0xff, 0xf, 0x3c, 0x3c, 0xf0,
    0xf3, 0xc0,

    /* U+F019 "" */
    0xe, 0x0, 0xe0, 0xe, 0x0, 0xe0, 0x3f, 0xc3,
    0xf8, 0x1f, 0x0, 0xe0, 0xf5, 0xff, 0xff, 0xff,
    0x5f, 0xff,

    /* U+F01C "" */
    0x1f, 0xe0, 0xc0, 0xc6, 0x1, 0x90, 0x2, 0xf8,
    0x7f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,

    /* U+F021 "" */
    0x0, 0x31, 0xf3, 0x71, 0xfc, 0x7, 0xc3, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0xfc, 0x3e, 0x3, 0xf8,
    0xec, 0xf8, 0xc0, 0x0,

    /* U+F026 "" */
    0xc, 0x7f, 0xff, 0xff, 0xf1, 0xc3,

    /* U+F027 "" */
    0xc, 0xe, 0x3f, 0x7f, 0x9f, 0xdf, 0xe0, 0x70,
    0x18,

    /* U+F028 "" */
    0x0, 0x60, 0x1, 0x83, 0x34, 0x38, 0xdf, 0xda,
    0xfe, 0x57, 0xf6, 0xbf, 0x8d, 0x1c, 0xd0, 0x61,
    0x80, 0x18,

    /* U+F03E "" */
    0xff, 0xf9, 0xff, 0x9f, 0xf9, 0xef, 0xfc, 0x7d,
    0x83, 0xc0, 0x38, 0x3, 0xff, 0xf0,

    /* U+F043 "" */
    0x0, 0xc, 0x7, 0x7, 0x83, 0xe3, 0xfb, 0xfd,
    0xff, 0xdf, 0xef, 0x99, 0xc7, 0x80,

    /* U+F048 "" */
    0xc3, 0xc7, 0xcf, 0xdf, 0xff, 0xff, 0xdf, 0xcf,
    0xc7, 0xc3,

    /* U+F04B "" */
    0x0, 0x1c, 0x3, 0xe0, 0x7f, 0xf, 0xf9, 0xff,
    0xbf, 0xff, 0xfe, 0xff, 0x9f, 0xc3, 0xe0, 0x70,
    0x0, 0x0,

    /* U+F04C "" */
    0xfb, 0xff, 0x7f, 0xef, 0xfd, 0xff, 0xbf, 0xf7,
    0xfe, 0xff, 0xdf, 0xfb, 0xff, 0x7c,

    /* U+F04D "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,

    /* U+F051 "" */
    0xc3, 0xe3, 0xf3, 0xfb, 0xff, 0xff, 0xfb, 0xf3,
    0xe3, 0xc3,

    /* U+F052 "" */
    0xc, 0x3, 0xc0, 0x7c, 0x1f, 0xc7, 0xfd, 0xff,
    0xbf, 0xf0, 0x0, 0xff, 0xff, 0xff, 0xff, 0x80,

    /* U+F053 "" */
    0xc, 0x73, 0x9c, 0xe3, 0x87, 0xe, 0x1c, 0x30,

    /* U+F054 "" */
    0x83, 0x87, 0xe, 0x1c, 0x73, 0x9c, 0xe2, 0x0,

    /* U+F067 "" */
    0xe, 0x1, 0xc0, 0x38, 0x7, 0xf, 0xff, 0xff,
    0xc3, 0x80, 0x70, 0xe, 0x1, 0xc0,

    /* U+F068 "" */
    0xff, 0xff, 0xfc,

    /* U+F06E "" */
    0xf, 0x81, 0xc7, 0x1c, 0x1d, 0xc6, 0x7e, 0xfb,
    0xf7, 0xdd, 0xdd, 0xc7, 0x1c, 0xf, 0x80,

    /* U+F070 "" */
    0x0, 0x1, 0xc0, 0x1, 0xdf, 0x0, 0xe3, 0x80,
    0xdb, 0x84, 0xfb, 0x9c, 0x77, 0x3c, 0x6e, 0x38,
    0x78, 0x38, 0x70, 0x1e, 0x30, 0x0, 0x30, 0x0,
    0x0,

    /* U+F071 "" */
    0x3, 0x0, 0x1c, 0x0, 0xf8, 0x3, 0xf0, 0x1c,
    0xc0, 0x73, 0x83, 0xcf, 0x1f, 0xfc, 0x7c, 0xfb,
    0xf3, 0xef, 0xff, 0x80,

    /* U+F074 "" */
    0x0, 0x0, 0x6, 0xe1, 0xff, 0x3f, 0x17, 0x60,
    0xe4, 0x1f, 0x6f, 0xbf, 0xf1, 0xf0, 0x6, 0x0,
    0x40,

    /* U+F077 "" */
    0x0, 0x3, 0x1, 0xe0, 0xcc, 0x61, 0xb0, 0x30,
    0x0,

    /* U+F078 "" */
    0x0, 0x30, 0x36, 0x18, 0xcc, 0x1e, 0x3, 0x0,
    0x0,

    /* U+F079 "" */
    0x30, 0x0, 0xf7, 0xf3, 0xf0, 0x65, 0xa0, 0xc3,
    0x1, 0x86, 0xb, 0x4c, 0x1f, 0x9f, 0xde, 0x0,
    0x18,

    /* U+F07B "" */
    0x78, 0xf, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+F093 "" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0xfc, 0x7, 0x0,
    0x70, 0x7, 0x0, 0x70, 0xf7, 0xff, 0xff, 0xff,
    0x5f, 0xff,

    /* U+F095 "" */
    0x0, 0x0, 0xf, 0x0, 0xf0, 0x1f, 0x0, 0xf0,
    0x6, 0x0, 0xe0, 0x1c, 0x73, 0xcf, 0xf8, 0xfe,
    0xf, 0xc0, 0x40, 0x0,

    /* U+F0C4 "" */
    0x70, 0x5b, 0x3f, 0x6f, 0x3f, 0xc1, 0xf0, 0x3e,
    0x1f, 0xe6, 0xde, 0xd9, 0xee, 0x8,

    /* U+F0C5 "" */
    0x1f, 0x43, 0xef, 0x7f, 0xef, 0xfd, 0xff, 0xbf,
    0xf7, 0xfe, 0xff, 0xdf, 0xf8, 0x3, 0xfc, 0x0,

    /* U+F0C7 "" */
    0xff, 0x98, 0x1b, 0x3, 0xe0, 0x7c, 0xf, 0xff,
    0xfe, 0x7f, 0x8f, 0xf9, 0xff, 0xfc,

    /* U+F0C9 "" */
    0xff, 0xe0, 0x0, 0x0, 0x0, 0xf, 0xfe, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xe0,

    /* U+F0E0 "" */
    0xff, 0xff, 0xff, 0x7f, 0xeb, 0xfd, 0xcf, 0x3e,
    0x67, 0xf9, 0xff, 0xff, 0xff, 0xf0,

    /* U+F0E7 "" */
    0x78, 0x78, 0xf8, 0xf0, 0xff, 0xfe, 0xfc, 0x1c,
    0x18, 0x18, 0x10, 0x30,

    /* U+F0EA "" */
    0x18, 0x3b, 0x8e, 0xe3, 0xf8, 0xe0, 0x3b, 0xae,
    0xe7, 0xbf, 0xef, 0xfb, 0xf0, 0xfc, 0x3f,

    /* U+F0F3 "" */
    0x4, 0x0, 0x80, 0x7c, 0x1f, 0xc3, 0xf8, 0x7f,
    0x1f, 0xf3, 0xfe, 0x7f, 0xdf, 0xfc, 0x0, 0x7,
    0x0,

    /* U+F11C "" */
    0xff, 0xff, 0x52, 0xbd, 0x4a, 0xff, 0xff, 0xeb,
    0x5f, 0xff, 0xfd, 0x2, 0xf4, 0xb, 0xff, 0xfc,

    /* U+F124 "" */
    0x0, 0x0, 0xf, 0x3, 0xf0, 0xfe, 0x3f, 0xef,
    0xfc, 0xff, 0xc0, 0x78, 0x7, 0x80, 0x78, 0x7,
    0x0, 0x70, 0x2, 0x0,

    /* U+F15B "" */
    0xfa, 0x7d, 0xbe, 0xff, 0xf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+F1EB "" */
    0x7, 0xc0, 0x7f, 0xf1, 0xe0, 0xf7, 0x0, 0x70,
    0x7c, 0x3, 0xfe, 0x6, 0xc, 0x0, 0x0, 0x3,
    0x80, 0x7, 0x0, 0xe, 0x0,

    /* U+F240 "" */
    0xff, 0xff, 0x80, 0x1f, 0x7f, 0xfe, 0xff, 0xbd,
    0xff, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F241 "" */
    0xff, 0xff, 0x80, 0x1f, 0x7f, 0x3e, 0xfe, 0x3d,
    0xfc, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F242 "" */
    0xff, 0xff, 0x80, 0x1f, 0x78, 0x3e, 0xf0, 0x3d,
    0xe0, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F243 "" */
    0xff, 0xff, 0x80, 0x1f, 0x60, 0x3e, 0xc0, 0x3d,
    0x80, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F244 "" */
    0xff, 0xff, 0x80, 0x1f, 0x0, 0x3e, 0x0, 0x3c,
    0x0, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F287 "" */
    0x0, 0xc0, 0x7, 0x80, 0x10, 0x7, 0x20, 0x6f,
    0xff, 0xfc, 0x41, 0x80, 0x40, 0x0, 0xb8, 0x0,
    0xf0,

    /* U+F293 "" */
    0x3e, 0x3b, 0x9c, 0xdb, 0x7c, 0xbf, 0x1f, 0x9f,
    0x87, 0xd5, 0xf9, 0x9d, 0xc7, 0xc0,

    /* U+F2ED "" */
    0xe, 0x1f, 0xfc, 0x0, 0x0, 0x7, 0xfc, 0xd5,
    0x9a, 0xb3, 0x56, 0x6a, 0xcd, 0x59, 0xab, 0x3f,
    0xe0,

    /* U+F304 "" */
    0x0, 0x40, 0xe, 0x0, 0xf0, 0x37, 0x7, 0xa0,
    0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc0, 0xf8,
    0xf, 0x0, 0x80, 0x0,

    /* U+F55A "" */
    0xf, 0xfe, 0x3f, 0xfc, 0xfb, 0x3b, 0xf0, 0xff,
    0xf3, 0xef, 0xc3, 0xcf, 0xb7, 0x8f, 0xff, 0xf,
    0xfe,

    /* U+F7C2 "" */
    0x1f, 0x9a, 0xbe, 0xaf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,

    /* U+F8A2 "" */
    0x0, 0x0, 0x3, 0x30, 0x37, 0x3, 0xff, 0xff,
    0xff, 0x70, 0x3, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 52, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 51, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 75, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 5, .adv_w = 135, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 119, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 24, .adv_w = 162, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 132, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 40, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 44, .adv_w = 65, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 48, .adv_w = 65, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 52, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 56, .adv_w = 112, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 60, .adv_w = 44, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 61, .adv_w = 74, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 62, .adv_w = 44, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 68, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 72, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 71, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 118, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 124, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 136, .adv_w = 118, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 44, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 44, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 146, .adv_w = 112, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 150, .adv_w = 112, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 153, .adv_w = 112, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 157, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 163, .adv_w = 199, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 180, .adv_w = 141, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 145, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 139, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 159, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 129, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 122, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 148, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 156, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 60, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 98, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 138, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 114, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 183, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 156, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 139, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 161, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 321, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 113, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 152, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 137, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 216, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 129, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 124, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 126, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 64, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 407, .adv_w = 68, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 416, .adv_w = 64, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 420, .adv_w = 112, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 424, .adv_w = 96, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 425, .adv_w = 115, .box_w = 4, .box_h = 2, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 426, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 131, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 440, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 131, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 455, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 68, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 466, .adv_w = 132, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 475, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 54, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 485, .adv_w = 55, .box_w = 3, .box_h = 13, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 490, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 54, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 203, .box_w = 11, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 131, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 122, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 523, .adv_w = 131, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 532, .adv_w = 131, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 541, .adv_w = 79, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 544, .adv_w = 96, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 79, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 554, .adv_w = 130, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 107, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 567, .adv_w = 173, .box_w = 11, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 106, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 583, .adv_w = 107, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 592, .adv_w = 100, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 67, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 602, .adv_w = 57, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 604, .adv_w = 67, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 609, .adv_w = 112, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 611, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 613, .adv_w = 60, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 614, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 632, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 646, .adv_w = 192, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 664, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 704, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 721, .adv_w = 216, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 739, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 757, .adv_w = 216, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 773, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 793, .adv_w = 96, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 808, .adv_w = 216, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 826, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 840, .adv_w = 132, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 854, .adv_w = 168, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 864, .adv_w = 168, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 882, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 896, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 910, .adv_w = 168, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 920, .adv_w = 168, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 936, .adv_w = 120, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 944, .adv_w = 120, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 952, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 966, .adv_w = 168, .box_w = 11, .box_h = 2, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 969, .adv_w = 216, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 984, .adv_w = 240, .box_w = 15, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1009, .adv_w = 216, .box_w = 14, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1029, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1046, .adv_w = 168, .box_w = 10, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1055, .adv_w = 168, .box_w = 10, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1064, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1081, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1095, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1113, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1133, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1147, .adv_w = 168, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1163, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1177, .adv_w = 168, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1190, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1204, .adv_w = 120, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1216, .adv_w = 168, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1231, .adv_w = 168, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1248, .adv_w = 216, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1264, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1284, .adv_w = 144, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1298, .adv_w = 240, .box_w = 15, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1319, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1333, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1347, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1361, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1375, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1389, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1406, .adv_w = 168, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1420, .adv_w = 168, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1437, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1457, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1474, .adv_w = 144, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1488, .adv_w = 193, .box_w = 12, .box_h = 8, .ofs_x = 0, .ofs_y = 1}
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
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 9, 0, 5, -4, 0, 0,
    0, 0, -11, -12, 1, 9, 4, 3,
    -8, 1, 9, 1, 8, 2, 6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 12, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, -6, 0, 0, 0, 0,
    0, -4, 3, 4, 0, 0, -2, 0,
    -1, 2, 0, -2, 0, -2, -1, -4,
    0, 0, 0, 0, -2, 0, 0, -2,
    -3, 0, 0, -2, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -2, 0, -3, 0, -5, 0, -23, 0,
    0, -4, 0, 4, 6, 0, 0, -4,
    2, 2, 6, 4, -3, 4, 0, 0,
    -11, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, -2, -9, 0, -8,
    -1, 0, 0, 0, 0, 0, 7, 0,
    -6, -2, -1, 1, 0, -3, 0, 0,
    -1, -14, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, -2, 7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 6,
    0, 2, 0, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 7, 2,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    4, 2, 6, -2, 0, 0, 4, -2,
    -6, -26, 1, 5, 4, 0, -2, 0,
    7, 0, 6, 0, 6, 0, -18, 0,
    -2, 6, 0, 6, -2, 4, 2, 0,
    0, 1, -2, 0, 0, -3, 15, 0,
    15, 0, 6, 0, 8, 2, 3, 6,
    0, 0, 0, -7, 0, 0, 0, 0,
    1, -1, 0, 1, -3, -2, -4, 1,
    0, -2, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, -11, 0, -12, 0, 0, 0,
    0, -1, 0, 19, -2, -2, 2, 2,
    -2, 0, -2, 2, 0, 0, -10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -19, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, 12, 0, 0, -7, 0,
    6, 0, -13, -19, -13, -4, 6, 0,
    0, -13, 0, 2, -4, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 5, 6, -23, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 9, 0, 1, 0, 0, 0,
    0, 0, 1, 1, -2, -4, 0, -1,
    -1, -2, 0, 0, -1, 0, 0, 0,
    -4, 0, -2, 0, -4, -4, 0, -5,
    -6, -6, -4, 0, -4, 0, -4, 0,
    0, 0, 0, -2, 0, 0, 2, 0,
    1, -2, 0, 1, 0, 0, 0, 2,
    -1, 0, 0, 0, -1, 2, 2, -1,
    0, 0, 0, -4, 0, -1, 0, 0,
    0, 0, 0, 1, 0, 2, -1, 0,
    -2, 0, -3, 0, 0, -1, 0, 6,
    0, 0, -2, 0, 0, 0, 0, 0,
    -1, 1, -1, -1, 0, 0, -2, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -1, 0, -2, -2, 0,
    0, 0, 0, 0, 1, 0, 0, -1,
    0, -2, -2, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, -1, -2, 0, -3, 0, -6,
    -1, -6, 4, 0, 0, -4, 2, 4,
    5, 0, -5, -1, -2, 0, -1, -9,
    2, -1, 1, -10, 2, 0, 0, 1,
    -10, 0, -10, -2, -17, -1, 0, -10,
    0, 4, 5, 0, 2, 0, 0, 0,
    0, 0, 0, -3, -2, 0, -6, 0,
    0, 0, -2, 0, 0, 0, -2, 0,
    0, 0, 0, 0, -1, -1, 0, -1,
    -2, 0, 0, 0, 0, 0, 0, 0,
    -2, -2, 0, -1, -2, -2, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 0, -2,
    0, -1, 0, -4, 2, 0, 0, -2,
    1, 2, 2, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 1,
    0, 0, -2, 0, -2, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 2, 0,
    -2, 0, 0, 0, 0, -2, -3, 0,
    -4, 0, 6, -1, 1, -6, 0, 0,
    5, -10, -10, -8, -4, 2, 0, -2,
    -12, -3, 0, -3, 0, -4, 3, -3,
    -12, 0, -5, 0, 0, 1, -1, 2,
    -1, 0, 2, 0, -6, -7, 0, -10,
    -5, -4, -5, -6, -2, -5, 0, -4,
    -5, 1, 0, 1, 0, -2, 0, 0,
    0, 1, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, -1, 0, -1, -2, 0, -3, -4,
    -4, -1, 0, -6, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 1,
    -1, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 9, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    -4, 0, 0, 0, 0, -10, -6, 0,
    0, 0, -3, -10, 0, 0, -2, 2,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, -3, 0,
    0, 0, 0, 2, 0, 1, -4, -4,
    0, -2, -2, -2, 0, 0, 0, 0,
    0, 0, -6, 0, -2, 0, -3, -2,
    0, -4, -5, -6, -2, 0, -4, 0,
    -6, 0, 0, 0, 0, 15, 0, 0,
    1, 0, 0, -2, 0, 2, 0, -8,
    0, 0, 0, 0, 0, -18, -3, 6,
    6, -2, -8, 0, 2, -3, 0, -10,
    -1, -2, 2, -13, -2, 2, 0, 3,
    -7, -3, -7, -6, -8, 0, 0, -12,
    0, 11, 0, 0, -1, 0, 0, 0,
    -1, -1, -2, -5, -6, 0, -18, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, -1, -2, -3, 0, 0,
    -4, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -4, 0, 0, 4,
    -1, 2, 0, -4, 2, -1, -1, -5,
    -2, 0, -2, -2, -1, 0, -3, -3,
    0, 0, -2, -1, -1, -3, -2, 0,
    0, -2, 0, 2, -1, 0, -4, 0,
    0, 0, -4, 0, -3, 0, -3, -3,
    2, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 2, 0, -3, 0, -1, -2,
    -6, -1, -1, -1, -1, -1, -2, -1,
    0, 0, 0, 0, 0, -2, -2, -2,
    0, 0, 0, 0, 2, -1, 0, -1,
    0, 0, 0, -1, -2, -1, -2, -2,
    -2, 0, 2, 8, -1, 0, -5, 0,
    -1, 4, 0, -2, -8, -2, 3, 0,
    0, -9, -3, 2, -3, 1, 0, -1,
    -2, -6, 0, -3, 1, 0, 0, -3,
    0, 0, 0, 2, 2, -4, -4, 0,
    -3, -2, -3, -2, -2, 0, -3, 1,
    -4, -3, 6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, -2,
    0, 0, -2, -2, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, -1, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    -3, 0, -4, 0, 0, 0, -6, 0,
    1, -4, 4, 0, -1, -9, 0, 0,
    -4, -2, 0, -8, -5, -5, 0, 0,
    -8, -2, -8, -7, -9, 0, -5, 0,
    2, 13, -2, 0, -4, -2, -1, -2,
    -3, -5, -3, -7, -8, -4, -2, 0,
    0, -1, 0, 1, 0, 0, -13, -2,
    6, 4, -4, -7, 0, 1, -6, 0,
    -10, -1, -2, 4, -18, -2, 1, 0,
    0, -12, -2, -10, -2, -14, 0, 0,
    -13, 0, 11, 1, 0, -1, 0, 0,
    0, 0, -1, -1, -7, -1, 0, -12,
    0, 0, 0, 0, -6, 0, -2, 0,
    -1, -5, -9, 0, 0, -1, -3, -6,
    -2, 0, -1, 0, 0, 0, 0, -9,
    -2, -6, -6, -2, -3, -5, -2, -3,
    0, -4, -2, -6, -3, 0, -2, -4,
    -2, -4, 0, 1, 0, -1, -6, 0,
    4, 0, -3, 0, 0, 0, 0, 2,
    0, 1, -4, 8, 0, -2, -2, -2,
    0, 0, 0, 0, 0, 0, -6, 0,
    -2, 0, -3, -2, 0, -4, -5, -6,
    -2, 0, -4, 2, 8, 0, 0, 0,
    0, 15, 0, 0, 1, 0, 0, -2,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -4, 0, 0, 0, 0, 0, -1,
    0, 0, 0, -2, -2, 0, 0, -4,
    -2, 0, 0, -4, 0, 3, -1, 0,
    0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 3, 4, 2, -2, 0, -6,
    -3, 0, 6, -6, -6, -4, -4, 8,
    3, 2, -17, -1, 4, -2, 0, -2,
    2, -2, -7, 0, -2, 2, -2, -2,
    -6, -2, 0, 0, 6, 4, 0, -5,
    0, -11, -2, 6, -2, -7, 1, -2,
    -6, -6, -2, 8, 2, 0, -3, 0,
    -5, 0, 2, 6, -4, -7, -8, -5,
    6, 0, 1, -14, -2, 2, -3, -1,
    -4, 0, -4, -7, -3, -3, -2, 0,
    0, -4, -4, -2, 0, 6, 4, -2,
    -11, 0, -11, -3, 0, -7, -11, -1,
    -6, -3, -6, -5, 5, 0, 0, -2,
    0, -4, -2, 0, -2, -3, 0, 3,
    -6, 2, 0, 0, -10, 0, -2, -4,
    -3, -1, -6, -5, -6, -4, 0, -6,
    -2, -4, -4, -6, -2, 0, 0, 1,
    9, -3, 0, -6, -2, 0, -2, -4,
    -4, -5, -5, -7, -2, -4, 4, 0,
    -3, 0, -10, -2, 1, 4, -6, -7,
    -4, -6, 6, -2, 1, -18, -3, 4,
    -4, -3, -7, 0, -6, -8, -2, -2,
    -2, -2, -4, -6, -1, 0, 0, 6,
    5, -1, -12, 0, -12, -4, 5, -7,
    -13, -4, -7, -8, -10, -6, 4, 0,
    0, 0, 0, -2, 0, 0, 2, -2,
    4, 1, -4, 4, 0, 0, -6, -1,
    0, -1, 0, 1, 1, -2, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 2, 6, 0, 0, -2, 0, 0,
    0, 0, -1, -1, -2, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 2, 0,
    -2, 0, 7, 0, 3, 1, 1, -2,
    0, 4, 0, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 0, 5, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, -2, 3, 0, 6,
    0, 0, 19, 2, -4, -4, 2, 2,
    -1, 1, -10, 0, 0, 9, -12, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -13, 7, 27, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, -4,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, -5, 0,
    0, 1, 0, 0, 2, 25, -4, -2,
    6, 5, -5, 2, 0, 0, 2, 2,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -25, 5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, -5, 0, 0, 0, 0,
    -4, -1, 0, 0, 0, -4, 0, -2,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -13, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, -2, 0, 0, -4, 0, -3, 0,
    -5, 0, 0, 0, -3, 2, -2, 0,
    0, -5, -2, -4, 0, 0, -5, 0,
    -2, 0, -9, 0, -2, 0, 0, -16,
    -4, -8, -2, -7, 0, 0, -13, 0,
    -5, -1, 0, 0, 0, 0, 0, 0,
    0, 0, -3, -3, -2, -3, 0, 0,
    0, 0, -4, 0, -4, 2, -2, 4,
    0, -1, -4, -1, -3, -4, 0, -2,
    -1, -1, 1, -5, -1, 0, 0, 0,
    -17, -2, -3, 0, -4, 0, -1, -9,
    -2, 0, 0, -1, -2, 0, 0, 0,
    0, 1, 0, -1, -3, -1, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0,
    0, -4, 0, -1, 0, 0, 0, -4,
    2, 0, 0, 0, -5, -2, -4, 0,
    0, -5, 0, -2, 0, -9, 0, 0,
    0, 0, -19, 0, -4, -7, -10, 0,
    0, -13, 0, -1, -3, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -3, -1,
    -3, 1, 0, 0, 3, -2, 0, 6,
    9, -2, -2, -6, 2, 9, 3, 4,
    -5, 2, 8, 2, 6, 4, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 12, 9, -3, -2, 0, -2,
    15, 8, 15, 0, 0, 0, 2, 0,
    0, 7, 0, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, -16, -2, -2, -8,
    -9, 0, 0, -13, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, -16, -2, -2,
    -8, -9, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, -4, 2, 0, -2,
    2, 3, 2, -6, 0, 0, -2, 2,
    0, 2, 0, 0, 0, 0, -5, 0,
    -2, -1, -4, 0, -2, -8, 0, 12,
    -2, 0, -4, -1, 0, -1, -3, 0,
    -2, -5, -4, -2, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, -16,
    -2, -2, -8, -9, 0, 0, -13, 0,
    0, 0, 0, 0, 0, 10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, 0, -6, -2, -2, 6, -2, -2,
    -8, 1, -1, 1, -1, -5, 0, 4,
    0, 2, 1, 2, -5, -8, -2, 0,
    -7, -4, -5, -8, -7, 0, -3, -4,
    -2, -2, -2, -1, -2, -1, 0, -1,
    -1, 3, 0, 3, -1, 0, 6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -2, -2, 0, 0,
    -5, 0, -1, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -12, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, -2, 0, -2,
    0, 0, 0, 0, -2, 0, 0, -3,
    -2, 2, 0, -3, -4, -1, 0, -6,
    -1, -4, -1, -2, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -13, 0, 6, 0, 0, -3, 0,
    0, 0, 0, -2, 0, -2, 0, 0,
    -1, 0, 0, -1, 0, -4, 0, 0,
    8, -2, -6, -6, 1, 2, 2, 0,
    -5, 1, 3, 1, 6, 1, 6, -1,
    -5, 0, 0, -8, 0, 0, -6, -5,
    0, 0, -4, 0, -2, -3, 0, -3,
    0, -3, 0, -1, 3, 0, -2, -6,
    -2, 7, 0, 0, -2, 0, -4, 0,
    0, 2, -4, 0, 2, -2, 2, 0,
    0, -6, 0, -1, -1, 0, -2, 2,
    -2, 0, 0, 0, -8, -2, -4, 0,
    -6, 0, 0, -9, 0, 7, -2, 0,
    -3, 0, 1, 0, -2, 0, -2, -6,
    0, -2, 2, 0, 0, 0, 0, -1,
    0, 0, 2, -2, 1, 0, 0, -2,
    -1, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -12, 0, 4, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    -2, -2, 0, 0, 0, 4, 0, 4,
    0, 0, 0, 0, 0, -12, -11, 1,
    8, 6, 3, -8, 1, 8, 0, 7,
    0, 4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 10, 0, 0,
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
const lv_font_t lv_font_montserrat_12 = {
#else
lv_font_t lv_font_montserrat_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
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



#endif /*#if LV_FONT_MONTSERRAT_12*/

