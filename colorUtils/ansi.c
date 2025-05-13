#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

// RGB to ANSI 256 color
static uint8_t __r_G_b_2_a_N_s_I_2_5_6__(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t ir = r * 5 / 255;
    uint8_t ig = g * 5 / 255;
    uint8_t ib = b * 5 / 255;
    return 16 + 36 * ir + 6 * ig + ib;
}

__attribute__((weak, alias("__r_G_b_2_a_N_s_I_2_5_6__"))) uint8_t rgb2ansi256(uint8_t r, uint8_t g, uint8_t b);
