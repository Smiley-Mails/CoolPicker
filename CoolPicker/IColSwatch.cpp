/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
					COLOR SWATCHES					
	These are the little colored squares that show	
	the current and previous colors					
	There are only two of them.						
////////////////////////////////////////////////////*/
#include "IColSwatch.h"
// Member Functions
void IColSwatch::SetColor(COLORREF col1)
{
	col = col1;
}
void IColSwatch::SetColor(AColor col1)
{
	fcol = col1;
	col = col1.toRGB();
}

void IColSwatch::UpdateSwatch()
{
	PostMessage(hWnd, CC_SWATCH_UPDATE, (WPARAM)this, 0);
}

// Setup Functions
IColSwatch *GetIColSwatch(HWND hwSwatch)
{
	return new IColSwatch(hwSwatch);
}

void ReleaseIColSwatch(IColSwatch *colSwatch)
{
	delete colSwatch;
}

void FillSwatch(HWND hWnd, COLORREF col)
{
	HDC hDC;
	RECT rect;
	HBRUSH hbr;

	hDC = GetDC(hWnd);
	GetClientRect(hWnd, &rect);

	hbr = CreateSolidBrush(col);

	FillRect(hDC, &rect, hbr);

	DeleteObject(hbr);
	ReleaseDC(hWnd, hDC);
}

LRESULT WINAPI ColorSwatchProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case CC_SWATCH_UPDATE:
			FillSwatch(hWnd, ((IColSwatch*)wParam)->col);
			break;
		case WM_ERASEBKGND:
			PostMessage(GetParent(hWnd), CC_SWATCH_REPAINT, (WPARAM)hWnd, 0);
			return -1;
			break;
		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return FALSE;
}
