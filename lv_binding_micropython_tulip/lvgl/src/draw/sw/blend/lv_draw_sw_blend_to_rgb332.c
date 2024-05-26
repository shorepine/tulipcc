/**
 * @file lv_draw_sw_blend_to_rgb332.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_sw_blend_to_rgb332.h"
#if LV_USE_DRAW_SW
#include <string.h>
#include "lv_draw_sw_blend.h"
#include "../../../misc/lv_math.h"
#include "../../../display/lv_display.h"
#include "../../../core/lv_refr.h"
#include "../../../misc/lv_color.h"
#include "../../../stdlib/lv_string.h"

#if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_NEON
    #include "neon/lv_blend_neon.h"
#elif LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_HELIUM
    #include "helium/lv_blend_helium.h"
#elif LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
    #include LV_DRAW_SW_ASM_CUSTOM_INCLUDE
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_color32_t fg_saved;
    lv_color32_t bg_saved;
    lv_color32_t res_saved;
    lv_opa_t res_alpha_saved;
    lv_opa_t ratio_saved;
} lv_color_mix_alpha_cache_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void /* LV_ATTRIBUTE_FAST_MEM */ rgb332_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc);

//static void /* LV_ATTRIBUTE_FAST_MEM */ rgb565_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc);

//static void /* LV_ATTRIBUTE_FAST_MEM */ rgb888_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc,
//                                                           const uint8_t src_px_size);

static void /* LV_ATTRIBUTE_FAST_MEM */ argb8888_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc);

static inline void /* LV_ATTRIBUTE_FAST_MEM */ lv_color_8_8_mix(const uint8_t src, uint8_t * dest, uint8_t mix);

static inline void /* LV_ATTRIBUTE_FAST_MEM */ blend_non_normal_pixel(uint8_t * dest, lv_color32_t src,
                                                                      lv_blend_mode_t mode);

static inline void * /* LV_ATTRIBUTE_FAST_MEM */ drawbuf_next_row(const void * buf, uint32_t stride);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_RGB332
    #define LV_DRAW_SW_COLOR_BLEND_TO_RGB332(...)                         LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_OPA
    #define LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_OPA(...)                LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_MASK
    #define LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_MASK(...)               LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_RGB332_MIX_MASK_OPA
    #define LV_DRAW_SW_COLOR_BLEND_TO_RGB332_MIX_MASK_OPA(...)            LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332(...)                 LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_WITH_OPA
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_WITH_OPA(...)        LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_WITH_MASK
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_WITH_MASK(...)       LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA(...)    LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332(...)                 LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_WITH_OPA
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_WITH_OPA(...)        LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_WITH_MASK
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_WITH_MASK(...)       LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA(...)    LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332
    #define LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332(...)               LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_OPA
    #define LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_OPA(...)      LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_MASK
    #define LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_MASK(...)     LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA
    #define LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA(...)  LV_RESULT_INVALID
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_color_to_rgb332(_lv_draw_sw_blend_fill_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    const lv_opa_t * mask = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;
    int32_t dest_stride = dsc->dest_stride;

    int32_t x;
    int32_t y;

    LV_UNUSED(w);
    LV_UNUSED(h);
    LV_UNUSED(x);
    LV_UNUSED(y);
    LV_UNUSED(opa);
    LV_UNUSED(mask);
    LV_UNUSED(mask_stride);
    LV_UNUSED(dest_stride);

    /*Simple fill*/
    if(mask == NULL && opa >= LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_RGB332(dsc)) {
            uint8_t color8 = lv_color_rgb332(dsc->color);
            uint8_t * dest_buf = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w - 16; x += 16) {
                    dest_buf[x + 0] = color8;
                    dest_buf[x + 1] = color8;
                    dest_buf[x + 2] = color8;
                    dest_buf[x + 3] = color8;

                    dest_buf[x + 4] = color8;
                    dest_buf[x + 5] = color8;
                    dest_buf[x + 6] = color8;
                    dest_buf[x + 7] = color8;

                    dest_buf[x + 8] = color8;
                    dest_buf[x + 9] = color8;
                    dest_buf[x + 10] = color8;
                    dest_buf[x + 11] = color8;

                    dest_buf[x + 12] = color8;
                    dest_buf[x + 13] = color8;
                    dest_buf[x + 14] = color8;
                    dest_buf[x + 15] = color8;
                }
                for(; x < w; x ++) {
                    dest_buf[x] = color8;
                }

                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
            }
        }
    }
    /*Opacity only*/
    else if(mask == NULL && opa < LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_OPA(dsc)) {
            uint32_t color8 = lv_color_rgb332(dsc->color);
            uint8_t * dest_buf = dsc->dest_buf;

            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    lv_color_8_8_mix(color8, &dest_buf[x], opa);
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
            }
        }

    }
    /*Masked with full opacity*/
    else if(mask && opa >= LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_RGB332_WITH_MASK(dsc)) {
            uint8_t color8 = lv_color_rgb332(dsc->color);
            uint8_t * dest_buf = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    lv_color_8_8_mix(color8, &dest_buf[x], mask[x]);
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
                mask += mask_stride;
            }
        }

    }
    /*Masked with opacity*/
    else {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_RGB332_MIX_MASK_OPA(dsc)) {
            uint8_t color8 = lv_color_rgb332(dsc->color);
            uint8_t * dest_buf = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    lv_color_8_8_mix(color8, &dest_buf[x], LV_OPA_MIX2(mask[x], opa));
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
                mask += mask_stride;
            }
        }
    }
}

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_image_to_rgb332(_lv_draw_sw_blend_image_dsc_t * dsc)
{
    switch(dsc->src_color_format) {
        case LV_COLOR_FORMAT_ARGB8888:
            argb8888_image_blend(dsc);
            break;
        case LV_COLOR_FORMAT_RGB332:
            rgb332_image_blend(dsc);
            break;
        default:
            LV_LOG_WARN("Not supported source color format %d", dsc->src_color_format);
            break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void LV_ATTRIBUTE_FAST_MEM rgb332_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint8_t * dest_buf_rgb332 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf_rgb332 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t dest_x;
    int32_t src_x;
    int32_t y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332(dsc)) {
                for(y = 0; y < h; y++) {
                    memcpy(dest_buf_rgb332, src_buf_rgb332, w);
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_rgb332 = drawbuf_next_row(src_buf_rgb332, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(dest_x = 0, src_x = 0; src_x < w; dest_x++, src_x++) {
                        lv_color_8_8_mix(src_buf_rgb332[src_x], &dest_buf_rgb332[dest_x], opa);
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_rgb332 = drawbuf_next_row(src_buf_rgb332, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(dest_x = 0, src_x = 0; src_x < w; dest_x++, src_x++) {
                        lv_color_8_8_mix(src_buf_rgb332[src_x], &dest_buf_rgb332[dest_x], mask_buf[src_x]);
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_rgb332 = drawbuf_next_row(src_buf_rgb332, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(dest_x = 0, src_x = 0; src_x < w; dest_x++, src_x++) {
                        lv_color_8_8_mix(src_buf_rgb332[src_x], &dest_buf_rgb332[dest_x], LV_OPA_MIX2(mask_buf[src_x], opa));
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_rgb332 = drawbuf_next_row(src_buf_rgb332, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        lv_color32_t src_argb;
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; src_x < w; dest_x++, src_x++) {
                src_argb.red = src_buf_rgb332[src_x];
                src_argb.green = src_buf_rgb332[src_x];
                src_argb.blue = src_buf_rgb332[src_x];
                if(mask_buf == NULL) src_argb.alpha = opa;
                else src_argb.alpha = LV_OPA_MIX2(mask_buf[dest_x], opa);
                blend_non_normal_pixel(&dest_buf_rgb332[dest_x], src_argb, dsc->blend_mode);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
            src_buf_rgb332 = drawbuf_next_row(src_buf_rgb332, src_stride);
        }
    }
}


static void LV_ATTRIBUTE_FAST_MEM argb8888_image_blend(_lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint8_t * dest_buf_rgb332 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const lv_color32_t * src_buf_c32 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t x;
    int32_t y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color_8_8_mix(lv_color_rgb332_32(src_buf_c32[x]), &dest_buf_rgb332[x], src_buf_c32[x].alpha);
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color_8_8_mix(lv_color_rgb332_32(src_buf_c32[x]), &dest_buf_rgb332[x], LV_OPA_MIX2(src_buf_c32[x].alpha, opa));
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color_8_8_mix(lv_color_rgb332_32(src_buf_c32[x]), &dest_buf_rgb332[x], LV_OPA_MIX2(src_buf_c32[x].alpha, mask_buf[x]));
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB332_BLEND_NORMAL_TO_RGB332_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color_8_8_mix(lv_color_rgb332_32(src_buf_c32[x]), &dest_buf_rgb332[x], LV_OPA_MIX3(src_buf_c32[x].alpha, opa,
                                                                                                            mask_buf[x]));
                    }
                    dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                lv_color32_t color_argb = src_buf_c32[x];
                if(mask_buf == NULL) color_argb.alpha = LV_OPA_MIX2(color_argb.alpha, opa);
                else color_argb.alpha = LV_OPA_MIX3(color_argb.alpha, mask_buf[x], opa);
                blend_non_normal_pixel(&dest_buf_rgb332[x], color_argb, dsc->blend_mode);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_rgb332 = drawbuf_next_row(dest_buf_rgb332, dest_stride);
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
        }
    }
}

static inline void LV_ATTRIBUTE_FAST_MEM lv_color_8_8_mix(const uint8_t src, uint8_t * dest, uint8_t mix)
{

    // make it some mixture of dest and src. closer to 255 it's more src, closer to 0 it's more dest
    uint8_t mix_inv_mask = (255-mix)>>5; // only 3 bits of mixing info
    // it's now inverted -- so 7 is fully dest, ... 0 is fully src
    switch(mix_inv_mask) {
        case 7:
            // just return dest = dest
            break;
        case 6:
            *dest = (src & 0b00100100) | (*dest & 0b11011011);
            break;
        case 5:
            *dest = (src & 0b01001001) | (*dest & 0b10110110);
            break;
        case 4:
            *dest = (src & 0b01101101) | (*dest & 0b10010010);
            break;
        case 3:
            *dest = (src & 0b10010010) | (*dest & 0b01101101);
            break;
        case 2:
            *dest = (src & 0b10110110) | (*dest & 0b01001001);
            break;
        case 1:
            *dest = (src & 0b11011011) | (*dest & 0b00100100);
            break;
        case 0:
            *dest = src;
            break;
    }

}

static inline void LV_ATTRIBUTE_FAST_MEM blend_non_normal_pixel(uint8_t * dest, lv_color32_t src, lv_blend_mode_t mode)
{
    // unclear if this is used by anything we do in tulip yet, so will just return 8_8 mix
    lv_color_8_8_mix(lv_color_rgb332_32(src), dest, src.alpha);
}

static inline void * LV_ATTRIBUTE_FAST_MEM drawbuf_next_row(const void * buf, uint32_t stride)
{
    return (void *)((uint8_t *)buf + stride);
}

#endif
