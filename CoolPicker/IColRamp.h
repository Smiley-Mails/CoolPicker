/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				COLOR RAMPS							
	This is the long narrow horizontal ramps that	
	that are used to specify each color component	
////////////////////////////////////////////////////*/
#ifndef _I_COL_RAMP_
#define _I_COL_RAMP_

//#include <windows.h>
#include "CoolPick.h"
extern GlobalOptions glob;

class IColRamp : public ICustomControl
{
private:
	HWND hWnd;
public:
	COLORREF lCol, rCol, mCol;
	AColor f_leftCol, f_rightCol, f_middleCol; //For floating point data

	int pPos;
	float f_Pos;

	BOOL rainbow;

	IColRamp(HWND hwRamp) { hWnd = hwRamp; pPos = lCol = rCol = 0; mCol = 0xDA000000; rainbow = FALSE; }
	~IColRamp() { DestroyWindow(hWnd); hWnd = NULL; }

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void SetColors(COLORREF, COLORREF, COLORREF = 0xDA000000);
	void SetColors(AColor, AColor, AColor); //Overloaded function for mental ray
	void SetRainbow();
	void SetPos(int);
	void SetPos(float); //Overloaded function for mental ray
	void UpdateRamp();
};


//Declarations
void ReleaseIColRamp(IColRamp *colRamp);
IColRamp *GetIColRamp(HWND hwRamp);
LRESULT WINAPI ColorModeProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //_I_COL_RAMP_