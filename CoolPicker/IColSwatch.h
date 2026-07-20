/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
					COLOR SWATCHES					
	These are the little colored squares that show	
	the current and previous colors					
	There are only two of them.						
////////////////////////////////////////////////////*/
#ifndef _I_COL_SWATCH_
#define _I_COL_SWATCH_

#include "CoolPick.h"

class IColSwatch : public ICustomControl
{
private:
	HWND hWnd;
public:
	COLORREF col;
	AColor fcol;
	IColSwatch(HWND hwSwatch) { hWnd = hwSwatch; col = 0; }
	~IColSwatch() { DestroyWindow(hWnd); hWnd = NULL; }

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void SetColor(COLORREF);
	void SetColor(AColor);
	void UpdateSwatch();
};

//Declarations
IColSwatch *GetIColSwatch(HWND hwSwatch);
void ReleaseIColSwatch(IColSwatch *colSwatch);
void FillSwatch(HWND hWnd, COLORREF col);
LRESULT WINAPI ColorSwatchProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //_I_COL_SWATCH_