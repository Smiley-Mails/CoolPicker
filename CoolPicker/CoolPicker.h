#ifndef _COOLPICKER_H_
#define _COOLPICKER_H_

#include <maxversion.h>
#include "CoolPick.h"
//#pragma warning (disable : 4100 4238 4239 4244 )
//#pragma warning (disable : 4244 )
//#pragma warning (default: 4263 4264)


class CoolPicker : public ColPick {
public:
    CoolPicker();
    ~CoolPicker();

    // --- Обязательные методы базового класса ColPick для Max 2027 ---
    const wchar_t* ClassName() override { return _T("CoolPicker"); }
    Class_ID ClassID() override { return COOLPICKER_CLASS_ID; }
    void DeleteThis() override { delete this; }
    INT_PTR Execute(int cmd, ULONG_PTR arg1, ULONG_PTR arg2, ULONG_PTR arg3) override { return 0; }

    // --- Основные методы интерфейса ---

    #if MAX_RELEASE >= 29000
        // =========================================================================
        // Для Max 2027
        // =========================================================================
        INT_PTR ModalColorPicker(
            HWND hwndOwner,
            AColor& initColor,
            ColorPicker::Flags flags,
            IPoint2* rp,
            ColorPickerCallback* pCallback,
            const wchar_t* name,
            ColorContext context
        ) override;

        ColorPicker* CreateColorPicker(
            HWND hwndOwner,
            const AColor& initColor,
            ColorPicker::Flags flags,
            IPoint2* rp,
            ColorPickerCallback* pCallback,
            const wchar_t* name,
            ColorContext context,
            BOOL bModal = FALSE
        ) override;

        ColorPicker* CreateColorPicker(
            HWND hwndOwner,
            const AColor& initColor,
            ColorPicker::Flags flags,
            IPoint2* rp,
            ColorPickerCallback* pCallback,
            const wchar_t* name,
            BOOL bModal = FALSE
        );
    #else
        // =========================================================================
        // Для 3ds Max 2026
        // =========================================================================
        INT_PTR ModalColorPicker(
            HWND hwndOwner,
            DWORD* lpc,
            IPoint2* spos,
            HSVCallback* callBack,
            const MCHAR* name,
            ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE
        ) override;

        INT_PTR ModalColorPicker(
            HWND hwndOwner,
            AColor* lpc,
            IPoint2* spos,
            HSVCallback* callBack,
            const MCHAR* name,
            ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE
        ) override;

        ColorPicker* CreateColorPicker(
            HWND hwndOwner,
            DWORD initColor,
            IPoint2* spos,
            HSVCallback* pcallback,
            const MCHAR* name,
            BOOL isObjectColor = FALSE,
            ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE
        ) override;

        ColorPicker* CreateColorPicker(
            HWND hwndOwner,
            AColor initColor,
            IPoint2* spos,
            HSVCallback* pcallback,
            const MCHAR* name,
            BOOL isObjectColor = FALSE,
            ColorContext context = ColorContext::COLOR_SPACE_CONTEXT_NONE
        ) override;
    #endif
};
#endif
