/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
		TRIANGLE-CIRCLE PICKER						
	This is the large Triangle and Circle			
	color picker.									
////////////////////////////////////////////////////*/
#ifndef _I_Picker_Tri_Circle_
#define _I_Picker_Tri_Circle_

#include "CoolPick.h"

class IPickerTriCircle : public ICustomControl
{
private:
	HWND hWnd;
public:
	//Fields ----------------
	int oldh, olds, oldv;
	IPoint2 cent;
	ISpinnerControl* hSpin;
	ICustEdit* hEdit;
	int radius;

	int ledge, redge, tedge, bedge;

	HDC memDC, hDC;
	HBITMAP memBM;
	RECT rect;
	TRIVERTEX trivert[4];
	GRADIENT_TRIANGLE gTris[2];

	//Methods ----------------
	//Constructor
	IPickerTriCircle(HWND hwPainter);
	//Destructor
	~IPickerTriCircle();

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void UpdatePickerPainter();
	void RepaintWheel();
	void RepaintTri();
	void PutTriPicker(int, int);
	void PutWheelPicker(int);
};


// SetUp Functions
void ReleaseIPickerPainter(IPickerTriCircle *pickerPainter);
IPickerTriCircle*	GetIPickerPainter(HWND hwPainter);
LRESULT WINAPI		PickerTriCircleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK		ColorTriCircleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif