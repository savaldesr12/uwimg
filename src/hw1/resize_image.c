#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    int i, j, k;
    float x, y, ax, ay, bx, by;
    image res = make_image(w, h, im.c);
    ax = im.w * 1.0 / res.w;
    ay = im.h * 1.0/ res.h;
    bx = -0.5 + 0.5*ax;
    by = -0.5 + 0.5*ay;

    
    for (k = 0; k < res.c; k++) {
        for (i = 0; i < res.w; i++) {
            for (j = 0; j < res.h; j++) {
                x = ax * i + bx;
                y =  ay * j + by;
                set_pixel(res, i, j, k, nn_interpolate(im, x, y, k));
            }
        }
    }
    return res;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float d1, d2, d3, d4, v1, v2, v3, v4;
    d1 = x - floor(x);
    d2 = ceil(x) - x;
    d3 =  y - floor(y);
    d4 = ceil(y) - y;

    v1 = get_pixel(im, floor(x), floor(y), c);
    v2 = get_pixel(im, ceil(x), floor(y), c);
    v3 = get_pixel(im, floor(x), ceil(y), c);
    v4 = get_pixel(im, ceil(x), ceil(y), c);

    return v1*d2*d4 + v2*d1*d4 + v3*d2*d3 + v4*d1*d3;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
        int i, j, k;
    float x, y, ax, ay, bx, by;
    image res = make_image(w, h, im.c);
    ax = im.w * 1.0 / res.w;
    ay = im.h * 1.0/ res.h;
    bx = -0.5 + 0.5*ax;
    by = -0.5 + 0.5*ay;

    
    for (k = 0; k < res.c; k++) {
        for (i = 0; i < res.w; i++) {
            for (j = 0; j < res.h; j++) {
                x = ax * i + bx;
                y =  ay * j + by;
                set_pixel(res, i, j, k, bilinear_interpolate(im, x, y, k));
            }
        }
    }
    return res;
}

