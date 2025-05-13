#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

// Apply gamma correction (default gamma = 2.2)
static uint8_t __a_p_p_l_y_G_a_m_m_a_8__(uint8_t value, float gamma) {
    float norm = value / 255.0f;
    float corrected = powf(norm, gamma);
    return (uint8_t)(corrected * 255.0f + 0.5f);
}

static float __a_p_p_l_y_G_a_m_m_a_F__(float value, float gamma) {
    return powf(value, gamma);
}


__attribute__((weak, alias("__a_p_p_l_y_G_a_m_m_a_8__"))) uint8_t applyGamma8(uint8_t value, float gamma);
__attribute__((weak, alias("__a_p_p_l_y_G_a_m_m_a_F__"))) float applyGammaF(float value, float gamma);
