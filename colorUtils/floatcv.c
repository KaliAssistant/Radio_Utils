#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

static void __r_g_b_5_6_5_2_f_0_1__(uint16_t rgb565, float *r, float *g, float *b) {
    *r = ((rgb565 >> 11) & 0x1F) / 31.0f;
    *g = ((rgb565 >> 5)  & 0x3F) / 63.0f;
    *b = (rgb565 & 0x1F) / 31.0f;
}

static uint16_t __f_0_1_2_r_g_b_5_6_5__(float r, float g, float b) {
    uint8_t R = (uint8_t)(CLAMP01(r) * 31 + 0.5f);
    uint8_t G = (uint8_t)(CLAMP01(g) * 63 + 0.5f);
    uint8_t B = (uint8_t)(CLAMP01(b) * 31 + 0.5f);
    return (R << 11) | (G << 5) | B;
}

static void __r_g_b_8_8_8_2_f_0_1__(uint8_t r8, uint8_t g8, uint8_t b8, float *r, float *g, float *b) {
    *r = r8 / 255.0f;
    *g = g8 / 255.0f;
    *b = b8 / 255.0f;
}

static void __f_0_1_2_r_g_b_8_8_8_(float r, float g, float b, uint8_t *r8, uint8_t *g8, uint8_t *b8) {
    *r8 = (uint8_t)(CLAMP01(r) * 255 + 0.5f);
    *g8 = (uint8_t)(CLAMP01(g) * 255 + 0.5f);
    *b8 = (uint8_t)(CLAMP01(b) * 255 + 0.5f);
}


__attribute__((weak, alias("__r_g_b_5_6_5_2_f_0_1__"))) void rgb565_2f01(uint16_t rgb565, float *r, float *g, float *b);
__attribute__((weak, alias("__f_0_1_2_r_g_b_5_6_5__"))) uint16_t f01_2rgb565(float r, float g, float b);
__attribute__((weak, alias("__r_g_b_8_8_8_2_f_0_1__"))) void rgb888_2f01(uint8_t r8, uint8_t g8, uint8_t b8, float *r, float *g, float *b);
__attribute__((weak, alias("__f_0_1_2_r_g_b_8_8_8_")))  void f01_2rgb888(float r, float g, float b, uint8_t *r8, uint8_t *g8, uint8_t *b8);
