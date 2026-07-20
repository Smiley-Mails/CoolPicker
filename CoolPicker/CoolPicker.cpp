#include "CoolPicker.h"
#include "CoolPickerDlg.h"
#include "CoolPickerModelessDlg.h"

// Конструктор и деструктор
CoolPicker::CoolPicker() {}
CoolPicker::~CoolPicker() {}

#if MAX_RELEASE >= 29000
// =========================================================================
// РЕАЛИЗАЦИЯ: 3ds Max 2027 SDK
// =========================================================================

ColorPicker* CoolPicker::CreateColorPicker(
    HWND hwndOwner,
    const AColor& initColor,
    ColorPicker::Flags flags,
    IPoint2* rp,
    ColorPickerCallback* pCallback,
    const wchar_t* name,
    ColorContext context,
    BOOL bModal
)
{
    return new CoolPickerModelessDlg(hwndOwner, initColor, flags, rp, pCallback, name, bModal, context);
}

ColorPicker* CoolPicker::CreateColorPicker(
    HWND hwndOwner,
    const AColor& initColor,
    ColorPicker::Flags flags,
    IPoint2* rp,
    ColorPickerCallback* pCallback,
    const wchar_t* name,
    BOOL bModal
)
{
    return CoolPicker::CreateColorPicker(hwndOwner, initColor, flags, rp, pCallback, name, static_cast<ColorContext>(0), bModal);
}

INT_PTR CoolPicker::ModalColorPicker(
    HWND hwndOwner,
    AColor& initColor,
    ColorPicker::Flags flags,
    IPoint2* rp,
    ColorPickerCallback* pCallback,
    const wchar_t* name,
    ColorContext context
)
{
    INT_PTR res = 0;
    CoolPickerDlg colDlg(hwndOwner, initColor, flags, rp, pCallback, name, context);
    res = colDlg.StartModal();

    if (res)
    {
        initColor = colDlg.GetAColor();
        if (rp) *rp = colDlg.GetPosition();
    }
    return res;
}

#else
// =========================================================================
// РЕАЛИЗАЦИЯ: 3ds Max 2026 SDK
// =========================================================================

INT_PTR CoolPicker::ModalColorPicker(
    HWND hwndOwner,
    DWORD* lpc,
    IPoint2* spos,
    HSVCallback* callBack,
    const MCHAR* name,
    ColorContext context
)
{
    AColor col = AColor(*lpc);
    CoolPickerDlg colDlg(hwndOwner, col, spos, callBack, name, context, 0, NULL);
    INT_PTR res = colDlg.StartModal();
    if (res)
    {
        if (lpc) *lpc = colDlg.GetColor();
        if (spos) *spos = colDlg.GetPosition();
    }
    return res;
}

INT_PTR CoolPicker::ModalColorPicker(
    HWND hwndOwner,
    AColor* lpc,
    IPoint2* spos,
    HSVCallback* callBack,
    const MCHAR* name,
    ColorContext context
)
{
    AColor col = lpc ? *lpc : AColor(0.0f, 0.0f, 0.0f, 1.0f);
    CoolPickerDlg colDlg(hwndOwner, col, spos, callBack, name, context, 0, NULL);
    INT_PTR res = colDlg.StartModal();
    if (res)
    {
        if (lpc) *lpc = colDlg.GetAColor();
        if (spos) *spos = colDlg.GetPosition();
    }
    return res;
}

ColorPicker* CoolPicker::CreateColorPicker(
    HWND hwndOwner,
    DWORD initColor,
    IPoint2* spos,
    HSVCallback* pcallback,
    const MCHAR* name,
    BOOL isObjectColor,
    ColorContext context
)
{
    return new CoolPickerModelessDlg(hwndOwner, AColor(initColor), spos, pcallback, name, isObjectColor, context);
}

ColorPicker* CoolPicker::CreateColorPicker(
    HWND hwndOwner,
    AColor initColor,
    IPoint2* spos,
    HSVCallback* pcallback,
    const MCHAR* name,
    BOOL isObjectColor,
    ColorContext context
)
{
    return new CoolPickerModelessDlg(hwndOwner, initColor, spos, pcallback, name, isObjectColor, context);
}
#endif