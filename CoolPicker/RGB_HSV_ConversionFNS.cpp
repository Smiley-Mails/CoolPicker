
#include "RGB_HSV_ConversionFNS.h"

//Pre: All parameters of H, S, V, and A must be in the range of 0.0 thru 1.0
//		The algorithm is written for S and V to be in a range of 0 to 1 and H to be 0 to 360. 
//Note:	Notice that H becomes un-normalized in the function itself.
AColor fHSVtoRGBA (float H, float S, float V, float A)
{
	float r = 0.0f,g = 0.0f,b = 0.0f,a = 0.0f;
	long int i = 0;
	float hh = 0.0f;
	float f = 0.0f;
	float p = 0.0f;
	float q = 0.0f;
	float t = 0.0f;

	//UnNormalize H
	H = (H * 360.0f);
	a = A; //Nice and simple isn't it?

	if (S == 0) // color is on black-and-white center line
	{
		if (H == 0)
		{
			r = V;     // achromatic: shades of gray
			g = V;
			b = V;
		}
		else if ( H == 360.0 ) // chromatic color
		{     
			r = 0;
			g = 0;
			b = 0;
		}
	}
	else 
	{
		hh = H;

		if (hh == 360) { hh = 0; }
		hh = hh / 60.0;
		i = (long) hh;
		f = hh - (float) i;
		p = V * (1.0 - S);
		q = V * (1.0 - (S * f));
		t = V * (1.0 - (S * (1.0 - f)));

		switch (i)
		{
			case 0: { r = V; g = t; b = p; break; }
			case 1: { r = q; g = V; b = p; break; }
			case 2: { r = p; g = V; b = t; break; }
			case 3: { r = p; g = q; b = V; break; }
			case 4: { r = t; g = p; b = V; break; }
			case 5: { r = V; g = p; b = q; break; }
			default:{ r = 0; g = 0; b = 0; break; }
		}
	}
  return AColor(r,g,b,a);
}

//Pre: r, g, b is a range of 0.0 thru 1.0
//Pre: The class of argument rgb must have member variables of r,g,b
//Post: h, will be normalized. i.e. assigned a range of 0.0 thru 1.0. 
//		The variables s and v will be of the same range that rgb is...

//Remarks:
//By using the template form for this function, It can be used with any
//	of the max standard color classes that have r, b and g member variables...
//	Which makes this a nice versatile function.
template<class T> void fRGBAtoHSV(T rgb, float &H, float &S, float &V )
{
	float delta, min;
	min = Minimum(rgb.r,rgb.g,rgb.b);
	V = Maximum(rgb.r,rgb.g,rgb.b);
	delta = V-min;
	if ( V == 0.0f)
	{
		S = 0;
	}
	else { S = delta / V;}

	if ( S == 0.0f )
	{
		H = 0.0f;
	}
	else if (rgb.r == V)       // Chromatic
	{ 
		// between yellow and magenta [degrees]
		H = 60.0 * (rgb.g - rgb.b) / delta;
	}
	else if ( rgb.g == V)
	{ 
		// between cyan and yellow
        H = 120.0 + 60.0 * (rgb.b - rgb.r) / delta;
	}
	else if ( rgb.b == V)
	{ 
		// between magenta and cyan
		H = 240.0 + 60.0 * (rgb.r - rgb.g) / delta;
	}
    if ( H < 0.0 ) 
	{ 
		H = H + 360.0f;
	}
	//Normalize H back to a range between [0 - 1.0] since H was in units of degrees.
	H = H / 360.0f;
}