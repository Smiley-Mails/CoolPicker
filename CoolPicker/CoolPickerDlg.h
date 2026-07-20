#ifndef _COOLPICKERDLG_H_
#define _COOLPICKERDLG_H_

#include "CoolPick.h"
#include "CoolPicker.h"
#include "CoolPickerModelessDlg.h"
#include <maxversion.h>

class CoolPickerModelessDlg; // forward declaration

class CoolPickerDlg
{
public:
	HWND	hwOwner;
	HWND	hwPanel;

	DWORD	origRGB;
	DWORD	currentRGB;

	AColor	a_origRGB;
	AColor	a_currentRGB;

	IPoint2 curPos;
	TSTR	colName;

	#if MAX_RELEASE >= 29000
		ColorPickerCallback* callback; // Для Max 2027
		ColorPicker::Flags pickerFlags;
	#else
		HSVCallback* callback;         // Для Max 2026
		BOOL isObjectColor;            // В 2026 вместо флагов использовался bool/int
	#endif

	static IPoint2 initPos;
	BOOL	objColor;
	CoolPickerModelessDlg* sm;
	ColorContext colorContext;

	ISpinnerControl* rSpin, * gSpin, * bSpin;
	ICustEdit* rEdit, * gEdit, * bEdit;

	ISpinnerControl* hSpin, * sSpin, * vSpin;
	ICustEdit* hEdit, * sEdit, * vEdit;

	ISpinnerControl* cSpin, * mSpin, * ySpin, * kSpin;
	ICustEdit* cEdit, * mEdit, * yEdit, * kEdit;

	ISpinnerControl* lSpin, * iSpin, * qSpin;
	ICustEdit* lEdit, * iEdit, * qEdit;

	IColRamp* rRamp, * gRamp, * bRamp;
	IColRamp* hRamp, * sRamp, * vRamp;
	IColRamp* cRamp, * mRamp, * yRamp, * kRamp;
	IColRamp* lRamp, * iRamp, * qRamp;

	IColRamp* alphaRamp;
	ICustEdit* alphaEdit;
	ISpinnerControl* alphaSpin;

	IColRamp* mixRamp;
	IColSwatch* currentSwatch, * originalSwatch;

	ICustButton* modeSwitch;
	IColMode* bigPicker;

	// Modeless and Modal support
	BOOL	Modeless;
	INT_PTR	StartModal(); // Изменено на INT_PTR
	HWND	StartModeless();

	// Constructor
	#if MAX_RELEASE >= 29000
	// ПРИОРИТЕТ: Конструктор под Max 2027
		CoolPickerDlg(HWND hOwner, const AColor& col, ColorPicker::Flags flags, IPoint2* pos,
			ColorPickerCallback* cb, const wchar_t* name, ColorContext context, int objClr = 0, CoolPickerModelessDlg* smp = NULL);
	#else
	// АДАПТАЦИЯ: Конструктор под Max 2026
		CoolPickerDlg(HWND hOwner, const AColor& col, IPoint2* pos,
			HSVCallback* cb, const wchar_t* name, ColorContext context, int objClr = 0, CoolPickerModelessDlg* smp = NULL);
	#endif
	// Destructor
	~CoolPickerDlg();

	void DoPaint(HWND hWnd);
	void  SetCurrentColor(int cSpace, BOOL update);
	void fSetCurrentColor(int cSpace, BOOL update);
	void  SetSliders(int cSpace, BOOL update);
	void fSetSliders(int cSpace, BOOL update);
	void  SetSliderRange();
	void fSetSliderRange();
	void  SetRamps();
	void fSetRamps();

	void LoadSettings();
	void WriteSettings();

	void SetNewColor(DWORD color, const wchar_t* name);
	void SetNewColor(AColor color, const wchar_t* name);
	void ModifyColor(DWORD color);
	void ModifyColor(AColor color);

	DWORD GetColor();
	AColor GetAColor();
	IPoint2 GetPosition();

	#if MAX_RELEASE >= 29000
		// 3ds Max 2027
		void InstallNewCB(DWORD col, ColorPickerCallback* pcb, const wchar_t* name);
		void InstallNewCB(AColor col, ColorPickerCallback* pcb, const wchar_t* name);
	#else
		// 3ds Max 2026
		void InstallNewCB(DWORD col, HSVCallback* pcb, const wchar_t* name);
		void InstallNewCB(AColor col, HSVCallback* pcb, const wchar_t* name);
	#endif

	// Конвертация цвета сцены (Working) в цвет интерфейса (Display/UI)
	AColor WorkingToDisplay(const AColor& col);
	// Конвертация цвета интерфейса (Display/UI) обратно в цвет сцены (Working)
	AColor DisplayToWorking(const AColor& col);

	BOOL InitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HIMAGELIST& himl, FlyOffData* fod);
};

#endif