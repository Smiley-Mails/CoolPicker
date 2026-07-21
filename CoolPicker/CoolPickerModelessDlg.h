#ifndef _COOLPICKERMODELESSDLG_H_
#define _COOLPICKERMODELESSDLG_H_

#include "CoolPick.h"
#include "CoolPicker.h"
#include "CoolPickerDlg.h"
#include <maxversion.h>

class CoolPickerDlg; // forward declaration

class CoolPickerModelessDlg : public ColorPicker
{
	HWND hwnd;
	CoolPickerDlg* colDlg;

public:
#if MAX_RELEASE >= 29000
	// =========================================================================
	// Max 2027
	// =========================================================================
	CoolPickerModelessDlg(HWND hwndOwner, const AColor& initColor, ColorPicker::Flags flags, IPoint2* pos,
		ColorPickerCallback* callback, const wchar_t* name, BOOL bModal, ColorContext context);

	~CoolPickerModelessDlg();

	void ModifyColor(const AColor& color, ColorPicker::Flags flags) override;
	void SetNewColor(const AColor& color, ColorPicker::Flags flags) override;
	void SetNewColor(const AColor& color, ColorPicker::Flags flags, const wchar_t* name, ColorContext context) override;
	void SetName(const wchar_t* name) override;
	bool InColorSamplerMode() const override;
	IPoint2 GetPosition() const override;

	void InstallNewCB(DWORD col, ColorPickerCallback* pcb, const wchar_t* name);
	void InstallNewCB(AColor col, ColorPickerCallback* pcb, const wchar_t* name);

	DWORD GetColor();
	AColor GetAColor();
	void Destroy();

	void SetNewColor(DWORD color, const wchar_t* name);
	void SetNewColor(AColor color, const wchar_t* name);
	void ModifyColor(DWORD color);
	void ModifyColor(AColor color);
#else
	// =========================================================================
	// Max 2026, 2025, 2024
	// =========================================================================
	CoolPickerModelessDlg(HWND hwndOwner, const AColor& initColor, IPoint2* pos,
		HSVCallback* callback, const wchar_t* name, BOOL isObjectColor, ColorContext context);

	~CoolPickerModelessDlg();

	void ModifyColor(DWORD color) override;
	void SetNewColor(DWORD color, const MCHAR* name) override;
	void SetNewColor(AColor color, const MCHAR* name) override;
	void ModifyColor(AColor color) override;
	bool IsColorSamplerMode() const override;
	IPoint2 GetPosition() override;

	void InstallNewCB(DWORD col, HSVCallback* pcb, const MCHAR* name, ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE) override;
	void InstallNewCB(AColor col, HSVCallback* pcb, const MCHAR* name, ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE) override;

	DWORD GetColor() override;
	AColor GetAColor() override;
	void Destroy() override;
#endif

	void RefreshUI() override;
	RECT GetRect() const override;
};

#endif