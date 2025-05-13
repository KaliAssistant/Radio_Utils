#ifndef COLORUTL_H
#define COLORUTL_H

#include <stdint.h>

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


#ifdef __cplusplus
extern "C" {
#endif

void hsv2rgb(float h, float s, float v, float *r, float *g, float *b);
void rgb2hsv(float r, float g, float b, float *h, float *s, float *v);
void hsl2rgb(float h, float s, float l, float *r, float *g, float *b);
void rgb2hsl(float r, float g, float b, float *h, float *s, float *l);
void hsv2hsl(float h, float s_hsv, float v, float *h_hsl, float *s_hsl, float *l);
void hsl2hsv(float h, float s_hsl, float l, float *h_hsv, float *s_hsv, float *v);

uint8_t rgb2ansi256(uint8_t r, uint8_t g, uint8_t b);

void rgb565_2f01(uint16_t rgb565, float *r, float *g, float *b);
uint16_t f01_2rgb565(float r, float g, float b);

void rgb888_2f01(uint8_t r8, uint8_t g8, uint8_t b8, float *r, float *g, float *b);
void f01_2rgb888(float r, float g, float b, uint8_t *r8, uint8_t *g8, uint8_t *b8);

uint16_t blend2rgb565(uint16_t bg, uint16_t fg, uint8_t a8);

void blend2rgb888(uint8_t bg_r, uint8_t bg_g, uint8_t bg_b,
                  uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                  uint8_t a8,
                  uint8_t *out_r, uint8_t *out_g, uint8_t *out_b);

ARGB32_t blend2argb32(ARGB32_t dst, ARGB32_t src);
RGBA32_t blend2rgba32(RGBA32_t dst, RGBA32_t src);
uint16_t rgb888_2rgb565(uint8_t r, uint8_t g, uint8_t b);

void rgb565_2rgb888(uint16_t rgb565, uint8_t* r, uint8_t* g, uint8_t* b); 

uint8_t rgb565_2gray(uint16_t rgb565);

uint8_t rgb888_2gray(uint8_t r, uint8_t g, uint8_t b);

uint8_t gray2_1bit(uint8_t gray);

uint8_t applyGamma8(uint8_t value, float gamma);

float applyGammaF(float value, float gamma);


#ifdef __cplusplus
}
#endif
#endif // COLORUTL_H
