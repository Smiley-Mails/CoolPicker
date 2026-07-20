#pragma once

#include "CoolPick.h"

#include "CoolPicker.h"
#include "CoolPickerDlg.h"
#include "CoolPickerModelessDlg.h"
#include "CoolPickerProcedure.h"

INT_PTR CALLBACK CoolPickerOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);