#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "colorutl.h"

static void __h_S_v_2_r_G_b__(float h, float s, float v, float *r, float *g, float *b) {
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

static void __h_S_v_2_h_S_l__(float h, float s_hsv, float v, float *h_hsl, float *s_hsl, float *l) {
    *h_hsl = h;
    *l = v * (1 - s_hsv / 2);
    *s_hsl = (*l == 0 || *l == 1) ? 0 : (v - *l) / fminf(*l, 1 - *l);
}

__attribute__((weak, alias("__h_S_v_2_r_G_b__"))) void hsv2rgb(float h, float s, float v, float *r, float *g, float *b);
__attribute__((weak, alias("__h_S_v_2_h_S_l__"))) void hsv2hsl(float h, float s_hsv, float v, float *h_hsl, float *s_hsl, float *l);
