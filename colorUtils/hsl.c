#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

static void __h_S_l_2_r_G_b__(float h, float s, float l, float *r, float *g, float *b) {
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

static void __h_S_l_2_h_S_v__(float h, float s_hsl, float l, float *h_hsv, float *s_hsv, float *v) {
    *h_hsv = h;
    float t = s_hsl * fminf(l, 1 - l);
    *v = l + t;
    *s_hsv = (*v == 0) ? 0 : 2 * (1 - l / *v);
}

__attribute__((weak, alias("__h_S_l_2_r_G_b__"))) void hsl2rgb(float h, float s, float l, float *r, float *g, float *b);
__attribute__((weak, alias("__h_S_l_2_h_S_v__"))) void hsl2hsv(float h, float s_hsl, float l, float *h_hsv, float *s_hsv, float *v);
