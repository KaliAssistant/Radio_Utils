#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

static uint8_t __r_g_b_5_6_5_2_g_r_a_y__(uint16_t rgb565) {
    // Extract components
    uint8_t r = ((rgb565 >> 11) & 0x1F) << 3;  // 5-bit to 8-bit
    uint8_t g = ((rgb565 >> 5) & 0x3F) << 2;   // 6-bit to 8-bit
    uint8_t b = (rgb565 & 0x1F) << 3;          // 5-bit to 8-bit

    // Standard grayscale formula
    return (uint8_t)((r * 0.299f) + (g * 0.587f) + (b * 0.114f) + 0.5f);
}

static uint8_t __r_g_b_8_8_8_2_g_r_a_y__(uint8_t r, uint8_t g, uint8_t b) {
    return (uint8_t)((r * 0.299f) + (g * 0.587f) + (b * 0.114f) + 0.5f);
}

static uint8_t __g_r_a_y_2_1_b_i_t__(uint8_t gray) {
    return gray >= 128 ? 1 : 0;
}

__attribute__((weak, alias("__r_g_b_5_6_5_2_g_r_a_y__"))) uint8_t rgb565_2gray(uint16_t rgb565);
__attribute__((weak, alias("__r_g_b_8_8_8_2_g_r_a_y__"))) uint8_t rgb888_2gray(uint8_t r, uint8_t g, uint8_t b);
__attribute__((weak, alias("__g_r_a_y_2_1_b_i_t__"))) uint8_t gray2_1bit(uint8_t gray);
