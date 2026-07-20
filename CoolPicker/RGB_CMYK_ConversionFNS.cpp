#include "misc_graphics_functions.h"
#include "RGB_CMYK_ConversionFNS.h"

// CMYKtoRGB Conversion functions
DWORD CMYKtoRGB(int c, int m, int y, int k)
{
	int r = 0, g = 0, b = 0;

	r = 255 - c;
	g = 255 - m;
	b = 255 - y;

	r -= k;
	g -= k;
	b -= k;

	r = CP_LIMITO_255(r);
	g = CP_LIMITO_255(g);
	b = CP_LIMITO_255(b);

	return RGB(r, g, b);
}
AColor CMYKtoRGBA(float c, float m, float y, float k, float alpha)
{
	float r, g, b;

	r = 1 - c;
	g = 1 - m;
	b = 1 - y;

	r -= k;
	g -= k;
	b -= k;

	r = CP_LIMITO_1(r);
	g = CP_LIMITO_1(g);
	b = CP_LIMITO_1(b);

	return AColor(r, g, b, alpha);
}

// RGBtoCMYK Conversion functions...
void RGBtoCMYK(DWORD rgb, int *c, int *m, int *y, int *k)
{
	*c = 255 - GetRValue(rgb);
	*m = 255 - GetGValue(rgb);
	*y = 255 - GetBValue(rgb);

	*k = 0;
}
void RGBAtoCMYK(AColor rgb, float *c, float *m, float *y, float *k)
{
	*c = 1.0f - rgb.r;
	*m = 1.0f - rgb.g;
	*y = 1.0f - rgb.b;

	*k = 0;
}