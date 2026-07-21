#include "CoolPickerModelessDlg.h"

void CoolPickerModelessDlg::Destroy() {
	DestroyWindow(hwnd);
}

CoolPickerModelessDlg::~CoolPickerModelessDlg() {
	Destroy();
	if (colDlg != NULL) delete colDlg;
	hwnd = NULL;
	colDlg = NULL;
}

#if MAX_RELEASE >= 29000
// =========================================================================
// Max 2027
// =========================================================================
CoolPickerModelessDlg::CoolPickerModelessDlg(HWND hwndOwner, const AColor& initColor, ColorPicker::Flags flags, IPoint2* pos,
	ColorPickerCallback* callback, const wchar_t* name, BOOL bModal, ColorContext context)
{
	hwnd = NULL;
	colDlg = NULL;
	colDlg = new CoolPickerDlg(hwndOwner, initColor, flags, pos, callback, name, context, 0, this);
	hwnd = colDlg->StartModeless();
}

void CoolPickerModelessDlg::InstallNewCB(DWORD col, ColorPickerCallback* pcb, const wchar_t* name) {
	if (colDlg) colDlg->InstallNewCB(col, pcb, name);
}

void CoolPickerModelessDlg::InstallNewCB(AColor col, ColorPickerCallback* pcb, const wchar_t* name) {
	if (colDlg) colDlg->InstallNewCB(col, pcb, name);
}

IPoint2 CoolPickerModelessDlg::GetPosition() const {
	return colDlg ? colDlg->GetPosition() : IPoint2(0, 0);
}

void CoolPickerModelessDlg::ModifyColor(const AColor& color, ColorPicker::Flags flags) {
	if (colDlg) colDlg->ModifyColor(color);
}

void CoolPickerModelessDlg::SetNewColor(const AColor& color, ColorPicker::Flags flags) {
	if (colDlg) colDlg->SetNewColor(color, L"");
}

void CoolPickerModelessDlg::SetNewColor(const AColor& color, ColorPicker::Flags flags, const wchar_t* name, ColorContext context) {
	if (colDlg) colDlg->SetNewColor(color, name);
}

void CoolPickerModelessDlg::SetName(const wchar_t* name) {
	if (colDlg) {
		colDlg->colName = name;
		SetWindowText(colDlg->hwPanel, name);
	}
}

bool CoolPickerModelessDlg::InColorSamplerMode() const {
	return false;
}

DWORD CoolPickerModelessDlg::GetColor() {
	return colDlg ? colDlg->GetColor() : 0;
}

AColor CoolPickerModelessDlg::GetAColor() {
	return colDlg ? colDlg->GetAColor() : AColor(0, 0, 0, 0);
}

void CoolPickerModelessDlg::SetNewColor(DWORD color, const wchar_t* name) {
	if (colDlg) colDlg->SetNewColor(color, name);
}

void CoolPickerModelessDlg::SetNewColor(AColor color, const wchar_t* name) {
	if (colDlg) colDlg->SetNewColor(color, name);
}

void CoolPickerModelessDlg::ModifyColor(DWORD color) {
	if (colDlg) colDlg->ModifyColor(color);
}

void CoolPickerModelessDlg::ModifyColor(AColor color) {
	if (colDlg) colDlg->ModifyColor(color);
}
#else
// =========================================================================
// Max 2026, 2025, 2024
// =========================================================================
CoolPickerModelessDlg::CoolPickerModelessDlg(HWND hwndOwner, const AColor& initColor, IPoint2* pos,
	HSVCallback* callback, const wchar_t* name, BOOL isObjectColor, ColorContext context)
{
	hwnd = NULL;
	colDlg = NULL;
	colDlg = new CoolPickerDlg(hwndOwner, initColor, pos, callback, name, context, isObjectColor, this);
	hwnd = colDlg->StartModeless();
}

void CoolPickerModelessDlg::InstallNewCB(DWORD col, HSVCallback* pcb, const MCHAR* name, ColorContext context) {
	if (colDlg) {
		colDlg->colorContext = context;
		colDlg->InstallNewCB(col, pcb, name);
	}
}

void CoolPickerModelessDlg::InstallNewCB(AColor col, HSVCallback* pcb, const MCHAR* name, ColorContext context) {
	if (colDlg) {
		colDlg->colorContext = context;
		colDlg->InstallNewCB(col, pcb, name);
	}
}

void CoolPickerModelessDlg::SetNewColor(DWORD color, const MCHAR* name) {
	if (colDlg) colDlg->SetNewColor(color, name);
}

void CoolPickerModelessDlg::SetNewColor(AColor color, const MCHAR* name) {
	if (colDlg) colDlg->SetNewColor(color, name);
}

void CoolPickerModelessDlg::ModifyColor(DWORD color) {
	if (colDlg) colDlg->ModifyColor(color);
}

void CoolPickerModelessDlg::ModifyColor(AColor color) {
	if (colDlg) colDlg->ModifyColor(color);
}

IPoint2 CoolPickerModelessDlg::GetPosition() {
	return colDlg ? colDlg->GetPosition() : IPoint2(0, 0);
}

bool CoolPickerModelessDlg::IsColorSamplerMode() const {
	return false;
}

DWORD CoolPickerModelessDlg::GetColor() {
	return colDlg ? colDlg->GetColor() : 0;
}

AColor CoolPickerModelessDlg::GetAColor() {
	return colDlg ? colDlg->GetAColor() : AColor(0.0f, 0.0f, 0.0f, 0.0f);
}
#endif

void CoolPickerModelessDlg::RefreshUI() {
	if (colDlg && colDlg->hwPanel) {
		InvalidateRect(colDlg->hwPanel, NULL, FALSE);
	}
}

RECT CoolPickerModelessDlg::GetRect() const {
	RECT rect = { 0 };
	if (hwnd) GetWindowRect(hwnd, &rect);
	return rect;
}