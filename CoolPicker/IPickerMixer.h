#ifndef _I_Picker_Mixer_
#define _I_Picker_Mixer_

#include "CoolPick.h"
extern GlobalOptions glob;

class IPickerMixer : public ICustomControl
{
private:
	HWND hWnd;
	HBITMAP oldBmp;
public:
	HDC hdc, memdc;
	HBITMAP membmp;

	int width, height, x, y, lx, ly;
	float* plane;
	double weights[ext][ext];
	double weightl[ext][ext];
	float oldone[ext][ext][3];

	ISpinnerControl* flowSpin;
	ICustEdit* flowEdit;

	int oldr, oldg, oldb, saver, saveg, saveb;
	BOOL picking;

	//Constructor / Destructor
	IPickerMixer();
	IPickerMixer(HWND hwMixer);
	~IPickerMixer();

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void UpdatePickerMixer();
	void Flipout();
	void Stroke(int, int, int);
	void SetBack();

	void SaveMixer();
	void LoadMixer();
	void DumpCopy();
	void GrabCopy();
};

// Setup Functions
void ReleaseIPickerMixer(IPickerMixer *pickerMixer);
IPickerMixer *GetIPickerMixer(HWND hwMixer);
LRESULT WINAPI PickerMixerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ColorMixerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
