/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				COLOR MODE HOUSING CONTROL				
	This is a master class that mediates and controls	
	different large color pickers. This holds the		
	states the of color picker, and switches between	
	the large plane mode, the triangle-circle mode and	
	the painter-mixer mode.								
////////////////////////////////////////////////////////*/
#ifndef _I_COL_MODE_
#define _I_COL_MODE_

#include "CoolPick.h"



class IColMode : public ICustomControl
{
private:
	HWND hWnd;
public:
	int mode;
	int submode;
	COLORREF currentRGB;
	AColor a_currentRGB;
	IColMode(HWND);
	~IColMode() { DestroyWindow(hWnd); hWnd = NULL; }

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void SetMode(int);
	void SetColor(COLORREF);
	void SetColor(AColor);
	void ForceRepaint();
};

void ReleaseIColMode(IColMode *colBig);
IColMode *GetIColMode(HWND hwBig);
LRESULT WINAPI ColorModeProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //_I_COL_MODE_