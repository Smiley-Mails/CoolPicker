/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				COLOR MODE HOUSING CONTROL				
	This is a master class that mediates and controls	
	different large color pickers. This holds the		
	states the of color picker, and switches between	
	the large plane mode, the triangle-circle mode and	
	the painter-mixer mode.								
////////////////////////////////////////////////////////*/
#include "IColMode.h"


extern GlobalOptions glob;

// Member Functions
IColMode::IColMode(HWND hwBig)
{
	hWnd = hwBig; 
	currentRGB = 0; 
	mode = glob.bigMode;
	submode = glob.planeSubmode; 
	PostMessage(hWnd, CC_BIG_INIT, (WPARAM)this, 0); 
}

void IColMode::SetMode(int newMode)
{
	mode = newMode;
	glob.bigMode = mode;
	PostMessage(hWnd, CC_BIG_SETMODE, (WPARAM)this, (LPARAM)newMode);
}

void IColMode::SetColor(COLORREF newColor)
{
	currentRGB = newColor;
	PostMessage(hWnd, CC_BIG_SETCOLOR, (WPARAM)this, (LPARAM)newColor);
}
void IColMode::SetColor(AColor newColor)
{
	a_currentRGB = newColor;

	//NOTE: I took out code here that limited floating values to 1.0

	currentRGB = newColor.toRGB();
	PostMessage(hWnd, CC_BIG_SETCOLOR, (WPARAM)this, (LPARAM)currentRGB); 
}

// Setup Functions
IColMode* GetIColMode(HWND hwBig)
{
	return new IColMode(hwBig);
}
void ReleaseIColMode(IColMode *colBig)
{
	delete colBig;
}
LRESULT WINAPI ColorModeProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IColMode* imp;
	static HWND kid;

	switch (msg)
	{
		case CC_BIG_CHANGE:
			PostMessage(GetParent(hWnd), CC_BIG_COLORCHANGED, wParam, 0);
			break;
		case CC_BIG_CHANGEDMODE:
			imp->submode = wParam;
			PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, wParam, 0);
			break;
		case CC_BIG_SETCOLOR:
			PostMessage(kid, CC_BIG_COLORCHANGED, imp->currentRGB, 0);
			break;
		case CC_BIG_SETMODE:
			if (kid)
				DestroyWindow(kid);
			switch (lParam)
			{
			case MODE_BIGPLANE:
				kid = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_BIG_PLANE), hWnd, ColorPlaneProc, 0);
				break;
			case MODE_TRIANGLE:
				kid = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_BIG_TRICIRCLE), hWnd, ColorTriCircleProc, 0);
				break;
			case MODE_MIXER:
				kid = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_BIG_MIXER), hWnd, ColorMixerProc, 0);
				break;
			case MODE_PALETTE:
//				kid = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_BIG_PALETTE), hWnd, ColorPaletteProc, 0);
				//MessageBox(hWnd, "Joman, Palette!", "CoolPicker", MB_OK);
				break;
			}

			if (kid)
			{
				ShowWindow(hWnd, SW_SHOW);
			}

			break;
		case CC_BIG_INIT:
			imp = (IColMode*)wParam;
/*			kid = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_BIG_PLANE), hWnd, ColorPlaneProc, 0);
			if (kid)
			{
				ShowWindow(hWnd, SW_SHOW);
			}
*/			
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			DestroyWindow(kid);
		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return FALSE;
}
