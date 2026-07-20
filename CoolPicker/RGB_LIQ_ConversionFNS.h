#ifndef _RGB_LIQ_FUNCTIONS_
#define _RGB_LIQ_FUNCTIONS_

#include "misc_graphics_functions.h"

// LIQtoRGB Conversion functions...
DWORD LIQtoRGB(int l, int i, int q);
AColor fLIQtoRGBA(float l, float i, float q, float alpha);

// RGBtoLIQ conversion functions....
void RGBtoLIQ(DWORD rgb, int *l, int *i, int *q);
void RGBAtoLIQ(AColor rgb, float *l, float *i, float *q);

#endif