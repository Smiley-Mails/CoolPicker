/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
					MISC GRAPHICS FUNCTIONS			
////////////////////////////////////////////////////*/
#ifndef _Misc_Graphics_Functions_
#define _Misc_Graphics_Functions_
//#pragma warning(disable : 4100 4239 4238 4244 4245)
//#pragma warning (disable : 4244 )
//#pragma warning (default: 4263 4264)

#include "max.h"
#include "Gamma.h"

void DrawPickerSprite(HDC hDC, int x, int y);
void FillGradient(HWND hWnd, COLORREF c1, COLORREF c2, COLORREF c3);
void FillRainbow(HWND hWnd);
void PutPicker(HWND hWnd, int pos);
void FPutPicker(HWND hWnd, float pos);
int  CP_LIMITO_255(int x);
float CP_LIMITO_1(float x);

#endif