#include "RGB_LIQ_ConversionFNS.h"

// LIQtoRGB Conversion functions...
DWORD LIQtoRGB(int l, int i, int q)
{
	float r, g, b, l2, i2, q2, y;
	l2 = (float)l;
	i2 = (float)i;
	q2 = (float)q;
	y = 298.082f * (l2 - 16);

	//Standard digital YCbCr conversion
	r = (1.0f/256.0f) * (y + 408.583f * (q2 - 128));
	g = (1.0f/256.0f) * (y + (-100.291f * (i2 - 128)) + (-208.120f * (q2 - 128)));
	b = (1.0f/256.0f) * (y + 516.411f * (i2 - 128));

	return RGB(CP_LIMITO_255((int)r), CP_LIMITO_255((int)g), CP_LIMITO_255((int)b));
}

AColor fLIQtoRGBA(float l, float i, float q, float alpha)
{
	//This function is no longer LIQ to RGB, it is now YCbCr to RGB ...
	float r, g, b, l2, i2, q2, y;
	l2 = l * 255.0f;
	i2 = i * 255.0f;
	q2 = q * 255.0f;
	y = 298.082f * (l2 - 16);

	//Standard digital YCbCr conversion

	r = (1.0f/256.0f) * (y + 408.583f * (q2 - 128));
	g = (1.0f/256.0f) * (y + (-100.291f * (i2 - 128)) + (-208.120f * (q2 - 128)));
	b = (1.0f/256.0f) * (y + 516.411f * (i2 - 128));
	
	return AColor( CP_LIMITO_1(r/255.0f), CP_LIMITO_1(g/255.0f) , CP_LIMITO_1(b/255.0f), alpha);
}

// RGBtoLIQ conversion functions....
void RGBtoLIQ(DWORD rgb, int *l, int *i, int *q)
{
	float r, g, b, l2, i2, q2;

	r = (float)GetRValue(rgb);
	g = (float)GetGValue(rgb);
	b = (float)GetBValue(rgb);

	l2 = 16 + (1.0f/256.0f) * ((65.738f * r) + (129.057f * g) + (25.064f * b));
	i2 = 128 + (1.0f/256.0f) * ((-37.945f * r) + (-74.494f * g) + (112.439f * b));
	q2 = 128 + (1.0f/256.0f) * ((112.439f * r) + (-94.154f * g) + (-18.285f * b));

	*l = CP_LIMITO_255((int)l2);
	*i = CP_LIMITO_255((int)i2);
	*q = CP_LIMITO_255((int)q2);
}

void RGBAtoLIQ(AColor rgb, float *l, float *i, float *q)
{
	float l2, i2, q2, r, g, b;
	r = rgb.r * 255.0f;
	g = rgb.g * 255.0f;
	b = rgb.b * 255.0f;

	l2 = 16 + (1.0f/256.0f) * ((65.738f * r) + (129.057f * g) + (25.064f * b));
	i2 = 128 + (1.0f/256.0f) * ((-37.945f * r) + (-74.494f * g) + (112.439f * b));
	q2 = 128 + (1.0f/256.0f) * ((112.439f * r) + (-94.154f * g) + (-18.285f * b));

	*l = CP_LIMITO_1(l2/255.0f);
	*i = CP_LIMITO_1(i2/255.0f);
	*q = CP_LIMITO_1(q2/255.0f);
}