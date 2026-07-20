#ifndef _RGB_HSV_
#define _RGB_HSV_

#include "MinMax.h"
#include "misc_graphics_functions.h"

//Pre: All parameters of H, S, V, and A must be in the range of 0.0 thru 1.0
//		The algorythm is written for S and V to be in a range of 0 to 1 and H to be 0 to 360. 
//Note:	Notice that H becomes un-normalized in the function itself.
AColor fHSVtoRGBA (float H, float S, float V, float A);


//Pre: r, g, b is a range of 0.0 thru 1.0
//Pre: The class of argument rgb must have member variables of r,g,b
//Post: h, will be normalized. i.e. assigned a range of 0.0 thru 1.0. 
//		The variables s and v will be of the same range that rgb is...

//Remarks:
//By using the template form for this function, It can be used with any
//	of the max standard color classes that have r, b and g member variables...
//	Which makes this a nice versatile function.
template<class T> void fRGBAtoHSV(T rgb, float &H, float &S, float &V );

#endif