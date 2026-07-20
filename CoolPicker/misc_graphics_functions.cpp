#include "misc_graphics_functions.h"

void DrawPickerSprite(HDC hDC, int x, int y)
{
	int i = 0;
	int cola = 0x00000000;
	int colb = 0x00FFFFFF;


	for (i = -2; i <= 2; i++)
	{
		SetPixelV(hDC, x + 3, y + i, cola);
		SetPixelV(hDC, x - 5, y + i, cola);
		SetPixelV(hDC, x + i, y + 3, cola);
		SetPixelV(hDC, x + i, y - 5, cola);

		SetPixelV(hDC, x - 4, y - 3, cola);
		SetPixelV(hDC, x - 3, y - 4, cola);
		SetPixelV(hDC, x - 4, y + 3, cola);
		SetPixelV(hDC, x + 3, y - 4, cola);


		SetPixelV(hDC, x + 4, y + i, colb);
		SetPixelV(hDC, x + i, y + 4, colb);
		SetPixelV(hDC, x - 4, y + i, colb);
		SetPixelV(hDC, x + i, y - 4, colb);

		SetPixelV(hDC, x + 3, y + 3, colb);
		SetPixelV(hDC, x + 3, y - 3, colb);
		SetPixelV(hDC, x - 3, y - 3, colb);
		SetPixelV(hDC, x - 3, y + 3, colb);


		SetPixelV(hDC, x - 3, y + i, cola);
		SetPixelV(hDC, x + 5, y + i, cola);
		SetPixelV(hDC, x + i, y - 3, cola);
		SetPixelV(hDC, x + i, y + 5, cola);

		SetPixelV(hDC, x + 4, y + 3, cola);
		SetPixelV(hDC, x + 3, y + 4, cola);
		SetPixelV(hDC, x + 4, y - 3, cola);
		SetPixelV(hDC, x - 3, y + 4, cola);
	}
}

void FillGradient(HWND hWnd, COLORREF c1, COLORREF c2, COLORREF c3)
{
	HDC hDC;
	RECT rect;

	hDC = GetDC(hWnd);
	if (!hDC) return;

	GetClientRect(hWnd, &rect);

	TRIVERTEX trivert[4];
	GRADIENT_RECT gRect[2];

	// Заполняем структуру TRIVERTEX напрямую из исходных цветов.
	// Значения сдвигаются на 8 бит (<< 8), так как структура ожидает 16-битный цвет для WinAPI
	trivert[0].Alpha = 0x0000;
	trivert[0].Blue = GetBValue(c1) << 8;
	trivert[0].Green = GetGValue(c1) << 8;
	trivert[0].Red = GetRValue(c1) << 8;
	trivert[0].x = rect.left;
	trivert[0].y = rect.top;

	trivert[1].Alpha = 0x0000;
	trivert[1].Blue = GetBValue(c3) << 8;
	trivert[1].Green = GetGValue(c3) << 8;
	trivert[1].Red = GetRValue(c3) << 8;
	trivert[1].x = rect.right / 2;
	trivert[1].y = rect.bottom;

	trivert[2].Alpha = 0x0000;
	trivert[2].Blue = GetBValue(c3) << 8;
	trivert[2].Green = GetGValue(c3) << 8;
	trivert[2].Red = GetRValue(c3) << 8;
	trivert[2].x = rect.right / 2;
	trivert[2].y = rect.top;

	trivert[3].Alpha = 0x0000;
	trivert[3].Blue = GetBValue(c2) << 8;
	trivert[3].Green = GetGValue(c2) << 8;
	trivert[3].Red = GetRValue(c2) << 8;
	trivert[3].x = rect.right;
	trivert[3].y = rect.bottom;

	gRect[0].UpperLeft = 0;
	gRect[0].LowerRight = 1;
	gRect[1].UpperLeft = 2;
	gRect[1].LowerRight = 3;

	GradientFill(hDC, trivert, 4, gRect, 2, GRADIENT_FILL_RECT_H);

	ReleaseDC(hWnd, hDC);
}

void FillRainbow(HWND hWnd)
{
	HDC hDC;
	RECT rect;
	TRIVERTEX trivert[12];
	GRADIENT_RECT gRect[6];

	hDC = GetDC(hWnd);
	GetClientRect(hWnd, &rect);
	
	trivert[0].Alpha = 0x0000;
	trivert[0].Blue = 0x0000;
	trivert[0].Green = 0x0000;
	trivert[0].Red = 0xff00;
	trivert[0].x = rect.left;
	trivert[0].y = rect.top;

	trivert[1].Alpha = 0x0000;
	trivert[1].Blue = 0x0000;
	trivert[1].Green = 0xff00;
	trivert[1].Red = 0xff00;
	trivert[1].x = rect.right / 6;
	trivert[1].y = rect.bottom;

	trivert[2].Alpha = 0x0000;
	trivert[2].Blue = 0x0000;
	trivert[2].Green = 0xff00;
	trivert[2].Red = 0xff00;
	trivert[2].x = rect.right / 6;
	trivert[2].y = rect.top;

	trivert[3].Alpha = 0x0000;
	trivert[3].Blue = 0x0000;
	trivert[3].Green = 0xff00;
	trivert[3].Red = 0x0000;
	trivert[3].x = 2*rect.right / 6;
	trivert[3].y = rect.bottom;

	trivert[4].Alpha = 0x0000;
	trivert[4].Blue = 0x0000;
	trivert[4].Green = 0xff00;
	trivert[4].Red = 0x0000;
	trivert[4].x = 2*rect.right / 6;
	trivert[4].y = rect.top;

	trivert[5].Alpha = 0x0000;
	trivert[5].Blue = 0xff00;
	trivert[5].Green = 0xff00;
	trivert[5].Red = 0x0000;
	trivert[5].x = 3*rect.right / 6;
	trivert[5].y = rect.bottom;

	trivert[6].Alpha = 0x0000;
	trivert[6].Blue = 0xff00;
	trivert[6].Green = 0xff00;
	trivert[6].Red = 0x0000;
	trivert[6].x = 3*rect.right / 6;
	trivert[6].y = rect.top;

	trivert[7].Alpha = 0x0000;
	trivert[7].Blue = 0xff00;
	trivert[7].Green = 0x0000;
	trivert[7].Red = 0x0000;
	trivert[7].x = 4*rect.right / 6;
	trivert[7].y = rect.bottom;

	trivert[8].Alpha = 0x0000;
	trivert[8].Blue = 0xff00;
	trivert[8].Green = 0x0000;
	trivert[8].Red = 0x0000;
	trivert[8].x = 4*rect.right / 6;
	trivert[8].y = rect.top;

	trivert[9].Alpha = 0x0000;
	trivert[9].Blue = 0xff00;
	trivert[9].Green = 0x0000;
	trivert[9].Red = 0xff00;
	trivert[9].x = 5*rect.right / 6;
	trivert[9].y = rect.bottom;

	trivert[10].Alpha = 0x0000;
	trivert[10].Blue = 0xff00;
	trivert[10].Green = 0x0000;
	trivert[10].Red = 0xff00;
	trivert[10].x = 5*rect.right / 6;
	trivert[10].y = rect.top;

	trivert[11].Alpha = 0x0000;
	trivert[11].Blue = 0x0000;
	trivert[11].Green = 0x0000;
	trivert[11].Red = 0xff00;
	trivert[11].x = rect.right;
	trivert[11].y = rect.bottom;

	gRect[0].UpperLeft = 0;
	gRect[0].LowerRight = 1;
	gRect[1].UpperLeft = 2;
	gRect[1].LowerRight = 3;
	gRect[2].UpperLeft = 4;
	gRect[2].LowerRight = 5;
	gRect[3].UpperLeft = 6;
	gRect[3].LowerRight = 7;
	gRect[4].UpperLeft = 8;
	gRect[4].LowerRight = 9;
	gRect[5].UpperLeft = 10;
	gRect[5].LowerRight = 11;

	GradientFill(hDC,trivert,12,gRect,6,GRADIENT_FILL_RECT_H);

	ReleaseDC(hWnd, hDC);
}
void PutPicker(HWND hWnd, int pos)
{
	HDC hDC;
	RECT rect;
	float x;

	hDC = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	x = ((float)pos/255.0);
	x *= (float)((rect.right - rect.left) - 3);

	rect.left = x;
	rect.right = x+3;
	
	DrawFrameControl(hDC, &rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_TRANSPARENT);

	ReleaseDC(hWnd, hDC);
}
void FPutPicker(HWND hWnd, float pos)
{
	HDC hDC;
	RECT rect;
	float x = 0.0f;

	hDC = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	x = pos;
	x *= (float)((rect.right - rect.left) - 3);

	rect.left = x;
	rect.right = x+3;
	
	DrawFrameControl(hDC, &rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_TRANSPARENT);

	ReleaseDC(hWnd, hDC);
}

int CP_LIMITO_255(int x)   
{ 
	if (x < 0)   return 0; 
	if (x > 255) return 255;  
	return x; 
}
float CP_LIMITO_1(float x) 
{ 
	if (x < 0)    return 0; 
	if (x > 1.0f) return 1.0f; 
	return x; 
}
