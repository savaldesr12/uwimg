#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if(x >= im.w) x = im.w - 1;
    if(y >= im.h) y = im.h - 1;
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if (c < 0) c = 0;
    if (c >= im.c) c = im.c - 1;
    //assert(c >= 0);
    //assert(c < im.c);


    return im.data[x + im.w*(y + im.h*c)];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if(x >= im.w) return;
    if(y >= im.h) return;
    if(x < 0) return;
    if(y < 0) return;
    if (c < 0) return;
    if (c >= im.c) return;
    im.data[x + im.w*(y + im.h*c)] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w*im.h*im.c*(sizeof(float)));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    int i, j;
    float v;
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (i = 0; i < im.w; ++i) {
        for (j = 0; j < im.h; j++) {
            v = 0.299 * get_pixel(im, i, j, 0) + 0.587 * get_pixel(im, i, j, 1) + 0.114 * get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, v);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int i, j;
    for (i = 0; i < im.w; ++i) {
        for (j = 0; j < im.h; j++) {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) + v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    int i, j, k;
    for (k = 0; k < im.c; ++k) {
        for (i = 0; i < im.w; ++i) {
            for (j = 0; j < im.h; ++j) {
                if (get_pixel(im, i, j, k) < 0) {
                    set_pixel(im, i, j, k, 0.0);
                }
                if (get_pixel(im, i, j, k) > 1) {
                    set_pixel(im, i, j, k, (float) 1);
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    int i, j;
    float V, m, r, g, b, C, S, Hp, H;
    for (i = 0; i < im.w; i++) {
        for (j = 0; j < im.h; j++) {
            r = get_pixel(im, i, j, 0);
            g = get_pixel(im, i, j, 1);
            b = get_pixel(im, i, j, 2);
            V = three_way_max(r, g, b);
            m = three_way_min(r, g, b);
            C = V - m;
            if (V == 0) {
                S = 0.0;
            } else {
                S = C / V;
            }
            
            if (C == 0) {
                Hp = 0;
            } else if (V == r) {
                Hp = (g - b) / C;
            } else if (V == g) {
                Hp = ((b - r) / C) + 2.0;
            } else {
                Hp = ((r - g) / C) + 4.0;
            }
            
            if (Hp < 0) {
                H = (Hp / 6) + 1;
            } else {
                H = (Hp / 6);
            }
            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, S);
            set_pixel(im, i, j, 2, V);
        }
    }
}

static float helper_fn(int n, float V, float S, float H) {
    float res;
    float k = fmod((n + H), 6.0);
    res = V - V*S*(fmax(0, three_way_min(k, 4 - k, 1)));
    return res;
}


void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int i, j;
    float V, r, g, b, S, Hp, H;
    for (i = 0; i < im.w; i++) {
        for (j = 0; j < im.h; j++) {
            H = get_pixel(im, i, j, 0);
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);

            Hp = H * 6;
            r = helper_fn(5, V, S, Hp);
            g = helper_fn(3, V, S, Hp);
            b = helper_fn(1, V, S, Hp);

            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}
