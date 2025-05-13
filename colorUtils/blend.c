#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"


static uint16_t __b_L_e_N_d_2_r_G_b_5_6_5__(uint16_t bg, uint16_t fg, uint8_t a8) {
    float alpha = a8 / 255.0f;

    uint8_t r = (uint8_t)(ALPHA_MIX(((bg >> 11) & 0x1F), ((fg >> 11) & 0x1F), alpha) + 0.5f);
    uint8_t g = (uint8_t)(ALPHA_MIX(((bg >> 5)  & 0x3F), ((fg >> 5)  & 0x3F), alpha) + 0.5f);
    uint8_t b = (uint8_t)(ALPHA_MIX((bg & 0x1F), (fg & 0x1F), alpha) + 0.5f);
 
    return (r << 11) | (g << 5) | b;
}

static void __b_L_e_N_d_2_r_G_b_8_8_8__(uint8_t bg_r, uint8_t bg_g, uint8_t bg_b,
                  uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                  uint8_t a8,
                  uint8_t *out_r, uint8_t *out_g, uint8_t *out_b) {
    float alpha = a8 / 255.0f;

    *out_r = (uint8_t)(ALPHA_MIX(bg_r, fg_r, alpha) + 0.5f);
    *out_g = (uint8_t)(ALPHA_MIX(bg_g, fg_g, alpha) + 0.5f);
    *out_b = (uint8_t)(ALPHA_MIX(bg_b, fg_b, alpha) + 0.5f);
}


static ARGB32_t __b_L_e_N_d_2_a_R_g_B_3_2__(ARGB32_t dst, ARGB32_t src) {
    uint8_t a = ARGB32_GET_A(src);
    float alpha = a / 255.0f;
    uint8_t r = (uint8_t)(ALPHA_MIX(ARGB32_GET_R(dst), ARGB32_GET_R(src), alpha) + 0.5f);
    uint8_t g = (uint8_t)(ALPHA_MIX(ARGB32_GET_G(dst), ARGB32_GET_G(src), alpha) + 0.5f);
    uint8_t b = (uint8_t)(ALPHA_MIX(ARGB32_GET_B(dst), ARGB32_GET_B(src), alpha) + 0.5f);
    return ARGB32_GET(0xFF, r, g, b);
}

static RGBA32_t __b_L_e_N_d_2_r_G_b_A_3_2__(RGBA32_t dst, RGBA32_t src) {
    uint8_t a = RGBA32_GET_A(src);
    float alpha = a / 255.0f;
    uint8_t r = (uint8_t)(ALPHA_MIX(RGBA32_GET_R(dst), RGBA32_GET_R(src), alpha) + 0.5f);
    uint8_t g = (uint8_t)(ALPHA_MIX(RGBA32_GET_G(dst), RGBA32_GET_G(src), alpha) + 0.5f);
    uint8_t b = (uint8_t)(ALPHA_MIX(RGBA32_GET_B(dst), RGBA32_GET_B(src), alpha) + 0.5f);
    return RGBA32_GET(r, g, b, 0xFF);
}


__attribute__((weak, alias("__b_L_e_N_d_2_r_G_b_5_6_5__"))) uint16_t blend2rgb565(uint16_t bg, uint16_t fg, uint8_t a8);
__attribute__((weak, alias("__b_L_e_N_d_2_r_G_b_8_8_8__")))
void blend2rgb888(uint8_t bg_r, uint8_t bg_g, uint8_t bg_b,
                                 uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                                 uint8_t a8,
                                 uint8_t *out_r, uint8_t *out_g, uint8_t *out_b);
__attribute__((weak, alias("__b_L_e_N_d_2_a_R_g_B_3_2__"))) ARGB32_t blend2argb32(ARGB32_t dst, ARGB32_t src);
__attribute__((weak, alias("__b_L_e_N_d_2_r_G_b_A_3_2__"))) RGBA32_t blend2rgba32(RGBA32_t dst, RGBA32_t src);
