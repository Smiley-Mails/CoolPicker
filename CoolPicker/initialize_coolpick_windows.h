/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				COMMON SETUP STUFF				//
	This registers windows						//
////////////////////////////////////////////////*/
#ifndef _Initialize_CoolPick_Windows_
#define _Initialize_CoolPick_Windows_

#include <windows.h>

//Forward declaration for WinProcs
LRESULT WINAPI ColorRampProc	  (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ColorSwatchProc	  (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ColorModeProc	  (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI PickerPlaneProc	  (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI PickerTriCircleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI PickerMixerProc	  (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitCoolPickerControls(HINSTANCE hInst)
{
	WNDCLASSEX	wc;

	// Color Ramps

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = ColorRampProc;
	wc.lpszClassName = _T("CustColorRamp");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Color Swatches

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = ColorSwatchProc;
	wc.lpszClassName = _T("CustColorSwatch");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Color Mode 

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = ColorModeProc;
	wc.lpszClassName = _T("CustColorBig");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Separate Picker Controls

	// Plane Picker

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = PickerPlaneProc;
	wc.lpszClassName = _T("CustPickerPlane");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Triangle-Circle Picker

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = PickerTriCircleProc;
	wc.lpszClassName = _T("CustPickerPainter");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Mixer Picker

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = PickerMixerProc;
	wc.lpszClassName = _T("CustPickerMixer");
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);

	// Palette Picker

/*	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = PickerPaletteProc;
	wc.lpszClassName = "CustPickerPalette";
	wc.lpszMenuName = NULL;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);
*/

}

#endif