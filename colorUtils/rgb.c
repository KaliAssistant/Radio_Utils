#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

static void __r_G_b_2_h_S_v__(float r, float g, float b, float *h, float *s, float *v) {
    float max = fmaxf(r, fmaxf(g, b));
    float min = fminf(r, fminf(g, b));
    float delta = max - min;

    *v = max;
    *s = (max == 0) ? 0 : delta / max;

    if (delta == 0)
        *h = 0;
    else if (max == r)
        *h = 60.0f * fmodf(((g - b) / delta), 6.0f);
    else if (max == g)
        *h = 60.0f * (((b - r) / delta) + 2.0f);
    else
        *h = 60.0f * (((r - g) / delta) + 4.0f);

    if (*h < 0)
        *h += 360.0f;
}

static void __r_G_b_2_h_S_l__(float r, float g, float b, float *h, float *s, float *l) {
    float max = fmaxf(r, fmaxf(g, b));
    float min = fminf(r, fminf(g, b));
    float delta = max - min;

    *l = (max + min) / 2;

    if (delta == 0) {
        *h = 0;
        *s = 0;
    } else {
        *s = delta / (1 - fabsf(2 * *l - 1));

        if (max == r)
            *h = 60.0f * fmodf(((g - b) / delta), 6.0f);
        else if (max == g)
            *h = 60.0f * (((b - r) / delta) + 2.0f);
        else
            *h = 60.0f * (((r - g) / delta) + 4.0f);

        if (*h < 0)
            *h += 360.0f;
    }
}

static uint16_t __r_G_b_8_8_8_2_r_G_b_5_6_5__(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) |
           ((g & 0xFC) << 3) |
           (b >> 3);
}

static void __r_G_b_5_6_5_2_r_G_b_8_8_8__(uint16_t rgb565, uint8_t* r, uint8_t* g, uint8_t* b) {
    *r = ((rgb565 >> 11) & 0x1F) << 3;
    *g = ((rgb565 >> 5)  & 0x3F) << 2;
    *b = (rgb565 & 0x1F) << 3;
}


__attribute__((weak, alias("__r_G_b_2_h_S_v__"))) void rgb2hsv(float r, float g, float b, float *h, float *s, float *v);
__attribute__((weak, alias("__r_G_b_2_h_S_l__"))) void rgb2hsl(float r, float g, float b, float *h, float *s, float *l);
__attribute__((weak, alias("__r_G_b_8_8_8_2_r_G_b_5_6_5__"))) uint16_t rgb888_2rgb565(uint8_t r, uint8_t g, uint8_t b);
__attribute__((weak, alias("__r_G_b_5_6_5_2_r_G_b_8_8_8__"))) void rgb565_2rgb888(uint16_t rgb565, uint8_t* r, uint8_t* g, uint8_t* b);
