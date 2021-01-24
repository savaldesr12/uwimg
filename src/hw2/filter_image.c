#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    int i, j, k;
    float sum = 0.0;;
    //printf("Norm value = %0.5f\n", norm);
    for (k = 0; k < im.c; k++) {
        for (i = 0; i < im.w; i++) {
            for (j = 0; j < im.h; j++) {
                sum += get_pixel(im, i, j, k);
            }
        }
    }
    for (k = 0; k < im.c; k++) {
    	for (i = 0; i < im.w; i++) {
	    for (j = 0; j < im.h; j++) {
		if (sum != 0) {
		   set_pixel(im, i, j, k, get_pixel(im, i,j,k) / sum);
		} else {
		   set_pixel(im, i, j, k, 0);
		}	
	    }
	}
    }
}

image make_box_filter(int w)
{
    int i, j;
    image res = make_image(w, w, 1);
    for (i = 0; i < w; i++) {
    	for (j = 0; j < w; j++) {
	    set_pixel(res, i, j, 0, 1.0);
	}
    }
    l1_normalize(res);
    return res;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    image res;
    int i, j, k, a, b, filter_x, filter_y, nxt_x, nxt_y;
    float q = 0.0;
    res = make_image(im.w, im.h, preserve ? im.c : 1);
    for (i = 0; i < im.w; i++) {
        for (j = 0; j < im.h; j++) {
            // We are at one center, now iterate through the filter to
            // get the value for this point
            if (preserve == 0) {
                q = 0.0;
            }
            for (k = 0; k < im.c; k++) {
                if (preserve == 1) {
                    q = 0.0;
                }
                for (a = i - floor(filter.w / 2); a <= i + ceil(filter.w / 2); a++) {
                    for (b = j - floor(filter.h/ 2); b <= j + ceil(filter.h / 2); b++) {
                        filter_x = a - i + floor(filter.w / 2);
                        filter_y = b - j + floor(filter.h / 2);
                        q += get_pixel(filter, filter_x, filter_y, filter.c == 1 ? 0 : k) * get_pixel(im, a, b, k);
                    }
                }
                if (preserve) {
                    set_pixel(res, i, j, k, q);
                }
            }
            if (!preserve) {
                set_pixel(res, i, j, 0, q);
            }
        }
    }
    return res;
}

image make_highpass_filter()
{
    image res = make_image(3, 3, 1);
    set_pixel(res,0, 0, 0, 0.0);
    set_pixel(res, 1, 0, 0, -1.0);
    set_pixel(res, 2, 0, 0, 0.0);
    set_pixel(res, 0, 1, 0, -1.0);
    set_pixel(res, 1, 1, 0, 4.0);
    set_pixel(res, 2, 1, 0, -1.0);
    set_pixel(res, 0, 2, 0, 0.0);
    set_pixel(res, 1, 2, 0, -1.0);
    set_pixel(res, 2, 2, 0, 0.0);  
    return res;
}

image make_sharpen_filter()
{
    image res = make_image(3, 3, 1);
    set_pixel(res,0, 0, 0, 0.0);
    set_pixel(res, 1, 0, 0, -1.0);
    set_pixel(res, 2, 0, 0, 0.0);
    set_pixel(res, 0, 1, 0, -1.0);
    set_pixel(res, 1, 1, 0, 5.0);
    set_pixel(res, 2, 1, 0, -1.0);
    set_pixel(res, 0, 2, 0, 0.0);
    set_pixel(res, 1, 2, 0, -1.0);
    set_pixel(res, 2, 2, 0, 0.0);
    return res;
}

image make_emboss_filter()
{
    image res = make_image(3, 3, 1);
    set_pixel(res,0, 0, 0, -2.0);
    set_pixel(res, 1, 0, 0, -1.0);
    set_pixel(res, 2, 0, 0, 0.0);
    set_pixel(res, 0, 1, 0, -1.0);
    set_pixel(res, 1, 1, 0, 1.0);
    set_pixel(res, 2, 1, 0, 1.0);
    set_pixel(res, 0, 2, 0, 0.0);
    set_pixel(res, 1, 2, 0, 1.0);
    set_pixel(res, 2, 2, 0, 2.0);
    return res;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    int i, j, nxt_x, nxt_y;
    int w = 6 * sigma;
    float nxt;
    if (w % 2 == 0) {
    	w += 1;
    }
    image res = make_image(w, w, 1);
    for (i = 0; i < w; i++) {
    	for (j = 0; j < w; j++) {
	    nxt_x = i - w / 2;
	    nxt_y = j - w/ 2;
	    nxt = 1.0 / (TWOPI * sigma * sigma) * expf((-nxt_x * nxt_x - nxt_y*nxt_y)/ (2 * sigma * sigma));
	    set_pixel(res, i, j, 0, nxt); 
	}
    }
    l1_normalize(res);
    return res;
}

static image add_sub_helper(image a, image b, int x) {
    int i, j, k;
    image res = make_image(a.w, a.h, a.c);
    for (k = 0; k < res.c; k++) {
        for (i = 0; i < res.w; i++) {
            for (j = 0; j < res.h; j++) {
                set_pixel(res, i, j, k, get_pixel(a, i, j, k) + x * get_pixel(b, i, j, k));
            }
        }
    }
    return res;
}

image add_image(image a, image b)
{
    // TODO
    image res = add_sub_helper(a, b, 1.0);
    return res;
}

image sub_image(image a, image b)
{
    // TODO
    return add_sub_helper(a, b, -1.0);
}

image make_gx_filter()
{
    // TODO
    image res = make_image(3, 3, 1);
    set_pixel(res,0, 0, 0, -1.0);
    set_pixel(res, 1, 0, 0, 0.0);
    set_pixel(res, 2, 0, 0, 1.0);
    set_pixel(res, 0, 1, 0, -2.0);
    set_pixel(res, 1, 1, 0, 0.0);
    set_pixel(res, 2, 1, 0, 2.0);
    set_pixel(res, 0, 2, 0, -1.0);
    set_pixel(res, 1, 2, 0, 0.0);
    set_pixel(res, 2, 2, 0, 1.0);
    return res;
}

image make_gy_filter()
{
    // TODO
    image res = make_image(3, 3, 1);
    set_pixel(res,0, 0, 0, -1.0);
    set_pixel(res, 1, 0, 0, -2.0);
    set_pixel(res, 2, 0, 0, -1.0);
    set_pixel(res, 0, 1, 0, 0.0);
    set_pixel(res, 1, 1, 0, 0.0);
    set_pixel(res, 2, 1, 0, 0.0);
    set_pixel(res, 0, 2, 0, 1.0);
    set_pixel(res, 1, 2, 0, 2.0);
    set_pixel(res, 2, 2, 0, 1.0);
    return res;
}

void feature_normalize(image im)
{
    // TODO
    int i, j, k;
    float min, max, range;
    min = INFINITY;
    max = -INFINITY;
    for (k = 0; k < im.c; k++) {
        for (i = 0; i < im.w; i++) {
            for (j = 0; j < im.h; j++) {
                min = fminf(min, get_pixel(im, i, j, k));
                max = fmaxf(max, get_pixel(im, i, j, k));
            }
        }
    }
    range = min - max;
    for (k = 0; k < im.c; k++) {
        for (i = 0; i < im.w; i++) {
            for (j = 0; j < im.h; j++) {
                if (range) {
                    set_pixel(im, i, j, k, (get_pixel(im, i, j, k) - min) / range);
                } else {
                    set_pixel(im, i, j, k, 0.0);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    int i, j;
    float nxt_gx, nxt_gy;
    image *res_ptr = calloc(2, sizeof(image));
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();
    image gx = convolve_image(im, gx_filter, 0);
    image gy = convolve_image(im, gy_filter, 0);
    image magnitude = make_image(im.w, im.h, 1);
    image theta = make_image(im.w, im.h, 1);
    for (i = 0; i < im.w; i++) {
        for (j = 0; j < im.h; j++) {
            nxt_gx = get_pixel(gx, i, j, 0);
            nxt_gy = get_pixel(gy, i, j, 0);
            set_pixel(magnitude, i, j, 0, sqrtf(1.0*nxt_gx*nxt_gx + 1.0*nxt_gy*nxt_gy));
            set_pixel(theta, i, j, 0, atan2f(nxt_gx, nxt_gy));
        }
    }
    res_ptr[0] = magnitude;
    res_ptr[1] = theta;
    free_image(gx_filter);
    free_image(gy_filter);
    free_image(gx);
    free_image(gy);
    return res_ptr;
}

image colorize_sobel(image im)
{
    // TODO
    int i, j, k;
    image *sobel = sobel_image(im);
    image res = make_image(im.w, im.h, im.c);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    for (k = 0; k < im.c; k++) {
        for (i = 0; i < im.w; i++) {
            for (j = 0; j < im.h; j++) {
                if (k == 0) {
                    set_pixel(res, i, j, k, get_pixel(sobel[1], i, j, 0));
                } else {
                    set_pixel(res, i, j, k, get_pixel(sobel[0], i, j, 0));
                }
            }
        }
    }
    free_image(sobel[0]);
    free_image(sobel[1]);
    free(sobel);
    hsv_to_rgb(res);
    return res;
}
