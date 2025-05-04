/*
 * File:        colorUtils/conv.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    This header provides color conversion and manipulation utilities including:
 *      - RGB565, RGB888, GRAY, 1BIT and float RGB [0–1] conversions
 *      - HSV, HSL conversions and cross-conversions
 *      - Packed ARGB32 and RGBA32 support
 *      - Fake alpha blending with A8-style opacity
 *      - 8bit/float Gamma Correction
 *
 *    Supported formats:
 *      - uint16_t RGB565 (5-6-5 bit)
 *      - uint8_t RGB888 (8-bit per channel)
 *      - float RGB [0.0–1.0]
 *      - uint32_t ARGB32_t (0xAARRGGBB)
 *      - uint32_t RGBA32_t (0xRRGGBBAA)
 *
 * Note: All color channels are assumed to be in sRGB space unless otherwise noted.
 *
 */

#ifndef COLORUTILS_CONV_H
#define COLORUTILS_CONV_H

#include <stdint.h>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

#define CLAMP01(x) ((float)(x) < 0 ? 0 : ((float)(x) > 1 ? 1 : (float)(x)))
#define ALPHA_MIX(a, b, alpha) ((a) * (1 - (alpha)) + (b) * (alpha))

typedef uint32_t ARGB32_t;  // 0xAARRGGBB
typedef uint32_t RGBA32_t;  // 0xRRGGBBAA

#define ARGB32_GET_A(argb) (((argb) >> 24) & 0xFF)
#define ARGB32_GET_R(argb) (((argb) >> 16) & 0xFF)
#define ARGB32_GET_G(argb) (((argb) >> 8)  & 0xFF)
#define ARGB32_GET_B(argb) ((argb) & 0xFF)
#define ARGB32_GET(a, r, g, b) (((uint32_t)(a) << 24) | ((r) << 16) | ((g) << 8) | (b))

#define RGBA32_GET_R(rgba) (((rgba) >> 24) & 0xFF)
#define RGBA32_GET_G(rgba) (((rgba) >> 16) & 0xFF)
#define RGBA32_GET_B(rgba) (((rgba) >> 8)  & 0xFF)
#define RGBA32_GET_A(rgba) ((rgba) & 0xFF)

#define RGBA32_GET(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))



// HSV to RGB
void hsv2rgb(float h, float s, float v, float *r, float *g, float *b) {
    h = fmodf(h, 360.0f);
    s = CLAMP01(s);
    v = CLAMP01(v);

    float c = v * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = v - c;

    float rp, gp, bp;

    if (h < 60)      { rp = c; gp = x; bp = 0; }
    else if (h < 120){ rp = x; gp = c; bp = 0; }
    else if (h < 180){ rp = 0; gp = c; bp = x; }
    else if (h < 240){ rp = 0; gp = x; bp = c; }
    else if (h < 300){ rp = x; gp = 0; bp = c; }
    else             { rp = c; gp = 0; bp = x; }

    *r = rp + m;
    *g = gp + m;
    *b = bp + m;
}

// RGB to HSV
void rgb2hsv(float r, float g, float b, float *h, float *s, float *v) {
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

// HSL to RGB
void hsl2rgb(float h, float s, float l, float *r, float *g, float *b) {
    h = fmodf(h, 360.0f);
    s = CLAMP01(s);
    l = CLAMP01(l);

    float c = (1 - fabsf(2 * l - 1)) * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = l - c / 2;

    float rp, gp, bp;

    if (h < 60)      { rp = c; gp = x; bp = 0; }
    else if (h < 120){ rp = x; gp = c; bp = 0; }
    else if (h < 180){ rp = 0; gp = c; bp = x; }
    else if (h < 240){ rp = 0; gp = x; bp = c; }
    else if (h < 300){ rp = x; gp = 0; bp = c; }
    else             { rp = c; gp = 0; bp = x; }

    *r = rp + m;
    *g = gp + m;
    *b = bp + m;
}

// RGB to HSL
void rgb2hsl(float r, float g, float b, float *h, float *s, float *l) {
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

// HSV to HSL
void hsv2hsl(float h, float s_hsv, float v, float *s_hsl, float *l) {
    *l = v * (1 - s_hsv / 2);
    *s_hsl = (*l == 0 || *l == 1) ? 0 : (v - *l) / fminf(*l, 1 - *l);
}

// HSL to HSV
void hsl2hsv(float h, float s_hsl, float l, float *s_hsv, float *v) {
    float t = s_hsl * fminf(l, 1 - l);
    *v = l + t;
    *s_hsv = (*v == 0) ? 0 : 2 * (1 - l / *v);
}

// RGB to ANSI 256 color
uint8_t rgb2ansi256(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t ir = r * 5 / 255;
    uint8_t ig = g * 5 / 255;
    uint8_t ib = b * 5 / 255;
    return 16 + 36 * ir + 6 * ig + ib;
}

void rgb565_2f01(uint16_t rgb565, float *r, float *g, float *b) {
    *r = ((rgb565 >> 11) & 0x1F) / 31.0f;
    *g = ((rgb565 >> 5)  & 0x3F) / 63.0f;
    *b = (rgb565 & 0x1F) / 31.0f;
}

uint16_t f01_2rgb565(float r, float g, float b) {
    uint16_t R = (uint16_t)(CLAMP01(r) * 31 + 0.5f);
    uint16_t G = (uint16_t)(CLAMP01(g) * 63 + 0.5f);
    uint16_t B = (uint16_t)(CLAMP01(b) * 31 + 0.5f);
    return (R << 11) | (G << 5) | B;
}

void rgb888_2f01(uint8_t r8, uint8_t g8, uint8_t b8, float *r, float *g, float *b) {
    *r = r8 / 255.0f;
    *g = g8 / 255.0f;
    *b = b8 / 255.0f;
}

void f01_2rgb888(float r, float g, float b, uint8_t *r8, uint8_t *g8, uint8_t *b8) {
    *r8 = (uint8_t)(CLAMP01(r) * 255 + 0.5f);
    *g8 = (uint8_t)(CLAMP01(g) * 255 + 0.5f);
    *b8 = (uint8_t)(CLAMP01(b) * 255 + 0.5f);
}

uint16_t blend2rgb565(uint16_t bg, uint16_t fg, uint8_t a8) {
    float alpha = a8 / 255.0f;

    float r1, g1, b1, r2, g2, b2;
    rgb565_2f01(bg, &r1, &g1, &b1);
    rgb565_2f01(fg, &r2, &g2, &b2);

    float r = ALPHA_MIX(r1, r2, alpha);
    float g = ALPHA_MIX(g1, g2, alpha);
    float b = ALPHA_MIX(b1, b2, alpha);

    return f01_2rgb565(r, g, b);
}

void blend2rgb888(uint8_t bg_r, uint8_t bg_g, uint8_t bg_b,
                  uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                  uint8_t a8,
                  uint8_t *out_r, uint8_t *out_g, uint8_t *out_b) {
    float alpha = a8 / 255.0f;

    float r = ALPHA_MIX(bg_r / 255.0f, fg_r / 255.0f, alpha);
    float g = ALPHA_MIX(bg_g / 255.0f, fg_g / 255.0f, alpha);
    float b = ALPHA_MIX(bg_b / 255.0f, fg_b / 255.0f, alpha);

    *out_r = (uint8_t)(CLAMP01(r) * 255 + 0.5f);
    *out_g = (uint8_t)(CLAMP01(g) * 255 + 0.5f);
    *out_b = (uint8_t)(CLAMP01(b) * 255 + 0.5f);
}


ARGB32_t blend2argb32(ARGB32_t dst, ARGB32_t src) {
    uint8_t a = ARGB32_GET_A(src);
    float alpha = a / 255.0f;
    uint8_t r = (uint8_t)(ALPHA_MIX(ARGB32_GET_R(dst), ARGB32_GET_R(src), alpha) + 0.5f);
    uint8_t g = (uint8_t)(ALPHA_MIX(ARGB32_GET_G(dst), ARGB32_GET_G(src), alpha) + 0.5f);
    uint8_t b = (uint8_t)(ALPHA_MIX(ARGB32_GET_B(dst), ARGB32_GET_B(src), alpha) + 0.5f);
    return ARGB32_GET(0xFF, r, g, b);
}

RGBA32_t blend2rgba32(RGBA32_t dst, RGBA32_t src) {
    uint8_t a = RGBA32_GET_A(src);
    float alpha = a / 255.0f;
    uint8_t r = (uint8_t)(ALPHA_MIX(RGBA32_GET_R(dst), RGBA32_GET_R(src), alpha) + 0.5f);
    uint8_t g = (uint8_t)(ALPHA_MIX(RGBA32_GET_G(dst), RGBA32_GET_G(src), alpha) + 0.5f);
    uint8_t b = (uint8_t)(ALPHA_MIX(RGBA32_GET_B(dst), RGBA32_GET_B(src), alpha) + 0.5f);
    return RGBA32_GET(0xFF, r, g, b);
}


uint16_t rgb888_2rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) |
           ((g & 0xFC) << 3) |
           (b >> 3);
}

void rgb565_2rgb888(uint16_t rgb565, uint8_t* r, uint8_t* g, uint8_t* b) {
    *r = ((rgb565 >> 11) & 0x1F) << 3;
    *g = ((rgb565 >> 5)  & 0x3F) << 2;
    *b = (rgb565 & 0x1F) << 3;
}

uint8_t rgb565_2gray(uint16_t rgb565) {
    // Extract components
    uint8_t r = ((rgb565 >> 11) & 0x1F) << 3;  // 5-bit to 8-bit
    uint8_t g = ((rgb565 >> 5) & 0x3F) << 2;   // 6-bit to 8-bit
    uint8_t b = (rgb565 & 0x1F) << 3;          // 5-bit to 8-bit

    // Standard grayscale formula
    return (uint8_t)((r * 0.299f) + (g * 0.587f) + (b * 0.114f) + 0.5f);
}

uint8_t rgb888_2gray(uint8_t r, uint8_t g, uint8_t b) {
    return (uint8_t)((r * 0.299f) + (g * 0.587f) + (b * 0.114f) + 0.5f);
}

uint8_t gray2_1bit(uint8_t gray) {
    return gray >= 128 ? 1 : 0;
}


// Apply gamma correction (default gamma = 2.2)
uint8_t applyGamma8(uint8_t value, float gamma) {
    float norm = value / 255.0f;
    float corrected = powf(norm, gamma);
    return (uint8_t)(corrected * 255.0f + 0.5f);
}

float applyGammaF(float value, float gamma) {
    return powf(value, gamma);
}

#ifdef __cplusplus
}
#endif

#endif // COLORUTILS_CONV_H
