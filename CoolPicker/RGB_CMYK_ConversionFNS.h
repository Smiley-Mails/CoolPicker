#ifndef _RGB_CMYK_
#define _RGB_CMYK_

#include "misc_graphics_functions.h"

// CMYKtoRGB Conversion functions
DWORD CMYKtoRGB(int c, int m, int y, int k);
AColor CMYKtoRGBA(float c, float m, float y, float k, float alpha);

// RGBtoCMYK Conversion functions...
void RGBtoCMYK(DWORD rgb, int *c, int *m, int *y, int *k);
void RGBAtoCMYK(AColor rgb, float *c, float *m, float *y, float *k);

#endif