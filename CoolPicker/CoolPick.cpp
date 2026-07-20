/**********************************************************************
 *<
	FILE: CoolPick.cpp

	DESCRIPTION:	This contains code handling of the controls that
				reside on the color picker dialog. The most important
				function is on this page is a windows procedure called
				CoolPickerOptionsDlgProc. EveryMessage sent between the
				custom controls get sent to this Proc, and all update
				calls are sent from there.
					This document also contains class definitions for
				the CoolPicker dialogs, which contain the Max custom 
				classes.

	CREATED BY: Daniel Rind

	MODIFIED BY: Chris Johnson

	HISTORY: Fall 2004-2005, updated to handle floating point numbers.

 *>	Copyright (c) 2005, All Rights Reserved.
 **********************************************************************/

#include "CoolPick.h"

#include "CoolPickerDlg.h"

//#pragma warning (disable : 4100 4238 4239 4244 )

GlobalOptions glob; //The one and only instance of GlobalOptions

//==============================================================================

class CoolPickerClassDesc : public ClassDesc2 
{
public:
	int IsPublic() override							{ return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new CoolPicker(); }
	const TCHAR *	ClassName() override			{ return GetString(IDS_CLASS_NAME); }
	SClass_ID SuperClassID() override				{ return COLPICK_CLASS_ID; }
	Class_ID ClassID() override						{ return COOLPICKER_CLASS_ID; }
	const TCHAR* Category() override				{ return GetString(IDS_CATEGORY); }

	const TCHAR* InternalName() override			{ return _T("CoolPicker"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() override					{ return hInstance; }					// returns owning module handle
	const wchar_t* NonLocalizedClassName() override { return _T("CoolPicker"); }

};

//==============================================================================
static CoolPickerClassDesc CoolPickerDesc;
ClassDesc2* GetCoolPickerDesc() {return &CoolPickerDesc;}

//==============================================================================
void SetUpIntControls(HWND hWnd, CoolPickerDlg* imp, HIMAGELIST &himl, FlyOffData* fod )
{
	// RGB
	imp->rSpin = SetupIntSpinner(hWnd, IDC_RED_SPIN, IDC_RED_EDIT, 0, glob.rangeMax, 0);
	imp->gSpin = SetupIntSpinner(hWnd, IDC_GREEN_SPIN, IDC_GREEN_EDIT, 0, glob.rangeMax, 0);
	imp->bSpin = SetupIntSpinner(hWnd, IDC_BLUE_SPIN, IDC_BLUE_EDIT, 0, glob.rangeMax, 0);

	imp->rEdit = GetICustEdit(GetDlgItem(hWnd, IDC_RED_EDIT));
	imp->rEdit->WantDlgNextCtl(TRUE);
	imp->gEdit = GetICustEdit(GetDlgItem(hWnd, IDC_GREEN_EDIT));
	imp->gEdit->WantDlgNextCtl(TRUE);
	imp->bEdit = GetICustEdit(GetDlgItem(hWnd, IDC_BLUE_EDIT));
	imp->bEdit->WantDlgNextCtl(TRUE);

	imp->rRamp = GetIColRamp(GetDlgItem(hWnd, IDC_RED_RAMP));
	imp->gRamp = GetIColRamp(GetDlgItem(hWnd, IDC_GREEN_RAMP));
	imp->bRamp = GetIColRamp(GetDlgItem(hWnd, IDC_BLUE_RAMP));
	
	// HSV
	imp->hSpin = SetupIntSpinner(hWnd, IDC_HUE_SPIN, IDC_HUE_EDIT, 0, glob.rangeMax, 0);
	imp->sSpin = SetupIntSpinner(hWnd, IDC_SAT_SPIN, IDC_SAT_EDIT, 0, glob.rangeMax, 0);
	imp->vSpin = SetupIntSpinner(hWnd, IDC_VAL_SPIN, IDC_VAL_EDIT, 0, glob.rangeMax, 0);

	imp->hEdit = GetICustEdit(GetDlgItem(hWnd, IDC_HUE_EDIT));
	imp->hEdit->SetNotifyOnKillFocus(TRUE);
	imp->sEdit = GetICustEdit(GetDlgItem(hWnd, IDC_SAT_EDIT));
	imp->sEdit->SetNotifyOnKillFocus(TRUE);
	imp->vEdit = GetICustEdit(GetDlgItem(hWnd, IDC_VAL_EDIT));
	imp->vEdit->SetNotifyOnKillFocus(TRUE);

	imp->hRamp = GetIColRamp(GetDlgItem(hWnd, IDC_HUE_RAMP));
	imp->hRamp->SetRainbow();
	imp->sRamp = GetIColRamp(GetDlgItem(hWnd, IDC_SAT_RAMP));
	imp->vRamp = GetIColRamp(GetDlgItem(hWnd, IDC_VAL_RAMP));
	// CMYK
	imp->cSpin = SetupIntSpinner(hWnd, IDC_CYAN_SPIN, IDC_CYAN_EDIT, 0, glob.rangeMax, 0);
	imp->mSpin = SetupIntSpinner(hWnd, IDC_MAGENTA_SPIN, IDC_MAGENTA_EDIT, 0, glob.rangeMax, 0);
	imp->ySpin = SetupIntSpinner(hWnd, IDC_YELLOW_SPIN, IDC_YELLOW_EDIT, 0, glob.rangeMax, 0);
	imp->kSpin = SetupIntSpinner(hWnd, IDC_BLACK_SPIN, IDC_BLACK_EDIT, 0, glob.rangeMax, 0);

	imp->cEdit = GetICustEdit(GetDlgItem(hWnd, IDC_CYAN_EDIT));
	imp->mEdit = GetICustEdit(GetDlgItem(hWnd, IDC_MAGENTA_EDIT));
	imp->yEdit = GetICustEdit(GetDlgItem(hWnd, IDC_YELLOW_EDIT));
	imp->kEdit = GetICustEdit(GetDlgItem(hWnd, IDC_BLACK_EDIT));

	imp->cRamp = GetIColRamp(GetDlgItem(hWnd, IDC_CYAN_RAMP));
	imp->mRamp = GetIColRamp(GetDlgItem(hWnd, IDC_MAGENTA_RAMP));
	imp->yRamp = GetIColRamp(GetDlgItem(hWnd, IDC_YELLOW_RAMP));
	imp->kRamp = GetIColRamp(GetDlgItem(hWnd, IDC_BLACK_RAMP));
	// LIQ
	imp->lSpin = SetupIntSpinner(hWnd, IDC_LUMA_SPIN, IDC_LUMA_EDIT, 0, glob.rangeMax, 0);
	imp->iSpin = SetupIntSpinner(hWnd, IDC_INP_SPIN, IDC_INP_EDIT, 0, glob.rangeMax, 0);
	imp->qSpin = SetupIntSpinner(hWnd, IDC_QUAD_SPIN, IDC_QUAD_EDIT, 0, glob.rangeMax, 0);

	imp->lEdit = GetICustEdit(GetDlgItem(hWnd, IDC_LUMA_EDIT));
	imp->iEdit = GetICustEdit(GetDlgItem(hWnd, IDC_INP_EDIT));
	imp->qEdit = GetICustEdit(GetDlgItem(hWnd, IDC_QUAD_EDIT));

	imp->lRamp = GetIColRamp(GetDlgItem(hWnd, IDC_LUMA_RAMP));
	imp->iRamp = GetIColRamp(GetDlgItem(hWnd, IDC_INP_RAMP));
	imp->qRamp = GetIColRamp(GetDlgItem(hWnd, IDC_QUAD_RAMP));
	// Alpha
	imp->alphaSpin = SetupIntSpinner(hWnd, IDC_ALPHA_SPIN2, IDC_ALPHA_EDIT, 0 , glob.rangeMax, 0);
	imp->alphaEdit = GetICustEdit(GetDlgItem(hWnd, IDC_ALPHA_EDIT));
	imp->alphaRamp = GetIColRamp(GetDlgItem(hWnd, IDC_ALPHA_RAMP));
	imp->alphaSpin->Enable(FALSE);
	imp->alphaEdit->Enable(FALSE);
	imp->alphaRamp->Enable(FALSE);

	imp->mixRamp = GetIColRamp(GetDlgItem(hWnd, IDC_MIX_RAMP));
	imp->currentSwatch = GetIColSwatch(GetDlgItem(hWnd, IDC_CURRENT_SWATCH));
	imp->originalSwatch = GetIColSwatch(GetDlgItem(hWnd, IDC_ORIGINAL_SWATCH));

	imp->modeSwitch = GetICustButton(GetDlgItem(hWnd, IDC_MODE));
	himl = ImageList_LoadImage(hInstance, MAKEINTRESOURCE(IDB_BIG), 54, 0,	CLR_DEFAULT, IMAGE_BITMAP, LR_DEFAULTCOLOR);
	imp->modeSwitch->SetImage(himl, 0, 0, 0, 0, 54, 18);
	imp->modeSwitch->SetFlyOff(3, fod, 0, 0, FLY_VARIABLE);
	imp->modeSwitch->SetCurFlyOff(glob.bigMode, FALSE);

	imp->bigPicker = GetIColMode(GetDlgItem(hWnd, IDC_BIG_PICKER));
}

void SetUpFltControls(HWND hWnd, CoolPickerDlg* imp, HIMAGELIST &himl, FlyOffData* fod )
{
	// RGB
	imp->rSpin = SetupFloatSpinner(hWnd, IDC_RED_SPIN, IDC_RED_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->gSpin = SetupFloatSpinner(hWnd, IDC_GREEN_SPIN, IDC_GREEN_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->bSpin = SetupFloatSpinner(hWnd, IDC_BLUE_SPIN, IDC_BLUE_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);

	imp->rEdit = GetICustEdit(GetDlgItem(hWnd, IDC_RED_EDIT));
	imp->gEdit = GetICustEdit(GetDlgItem(hWnd, IDC_GREEN_EDIT));
	imp->bEdit = GetICustEdit(GetDlgItem(hWnd, IDC_BLUE_EDIT));

	imp->rRamp = GetIColRamp(GetDlgItem(hWnd, IDC_RED_RAMP));
	imp->gRamp = GetIColRamp(GetDlgItem(hWnd, IDC_GREEN_RAMP));
	imp->bRamp = GetIColRamp(GetDlgItem(hWnd, IDC_BLUE_RAMP));
	
	// HSV
	imp->hSpin = SetupFloatSpinner(hWnd, IDC_HUE_SPIN, IDC_HUE_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->sSpin = SetupFloatSpinner(hWnd, IDC_SAT_SPIN, IDC_SAT_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->vSpin = SetupFloatSpinner(hWnd, IDC_VAL_SPIN, IDC_VAL_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);

	imp->hEdit = GetICustEdit(GetDlgItem(hWnd, IDC_HUE_EDIT));
	imp->sEdit = GetICustEdit(GetDlgItem(hWnd, IDC_SAT_EDIT));
	imp->vEdit = GetICustEdit(GetDlgItem(hWnd, IDC_VAL_EDIT));

	imp->hRamp = GetIColRamp(GetDlgItem(hWnd, IDC_HUE_RAMP));
	imp->hRamp->SetRainbow();
	imp->sRamp = GetIColRamp(GetDlgItem(hWnd, IDC_SAT_RAMP));
	imp->vRamp = GetIColRamp(GetDlgItem(hWnd, IDC_VAL_RAMP));
	
	// CMYK
	imp->cSpin = SetupFloatSpinner(hWnd, IDC_CYAN_SPIN, IDC_CYAN_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->mSpin = SetupFloatSpinner(hWnd, IDC_MAGENTA_SPIN, IDC_MAGENTA_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->ySpin = SetupFloatSpinner(hWnd, IDC_YELLOW_SPIN, IDC_YELLOW_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->kSpin = SetupFloatSpinner(hWnd, IDC_BLACK_SPIN, IDC_BLACK_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);

	imp->cEdit = GetICustEdit(GetDlgItem(hWnd, IDC_CYAN_EDIT));
	imp->mEdit = GetICustEdit(GetDlgItem(hWnd, IDC_MAGENTA_EDIT));
	imp->yEdit = GetICustEdit(GetDlgItem(hWnd, IDC_YELLOW_EDIT));
	imp->kEdit = GetICustEdit(GetDlgItem(hWnd, IDC_BLACK_EDIT));

	imp->cRamp = GetIColRamp(GetDlgItem(hWnd, IDC_CYAN_RAMP));
	imp->mRamp = GetIColRamp(GetDlgItem(hWnd, IDC_MAGENTA_RAMP));
	imp->yRamp = GetIColRamp(GetDlgItem(hWnd, IDC_YELLOW_RAMP));
	imp->kRamp = GetIColRamp(GetDlgItem(hWnd, IDC_BLACK_RAMP));
	
	// LIQ
	imp->lSpin = SetupFloatSpinner(hWnd, IDC_LUMA_SPIN, IDC_LUMA_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->iSpin = SetupFloatSpinner(hWnd, IDC_INP_SPIN, IDC_INP_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);
	imp->qSpin = SetupFloatSpinner(hWnd, IDC_QUAD_SPIN, IDC_QUAD_EDIT, 0.0f, 1.0f, 0.0f, 0.01f);

	imp->lEdit = GetICustEdit(GetDlgItem(hWnd, IDC_LUMA_EDIT));
	imp->iEdit = GetICustEdit(GetDlgItem(hWnd, IDC_INP_EDIT));
	imp->qEdit = GetICustEdit(GetDlgItem(hWnd, IDC_QUAD_EDIT));

	imp->lRamp = GetIColRamp(GetDlgItem(hWnd, IDC_LUMA_RAMP));
	imp->iRamp = GetIColRamp(GetDlgItem(hWnd, IDC_INP_RAMP));
	imp->qRamp = GetIColRamp(GetDlgItem(hWnd, IDC_QUAD_RAMP));
	
	// Alpha
	imp->alphaSpin = SetupFloatSpinner(hWnd, IDC_ALPHA_SPIN2, IDC_ALPHA_EDIT, 0.0f , 1.0f, 1.0f, 0.01f);
	imp->alphaEdit = GetICustEdit(GetDlgItem(hWnd, IDC_ALPHA_EDIT));
	imp->alphaRamp = GetIColRamp(GetDlgItem(hWnd, IDC_ALPHA_RAMP));

	// Misc
	imp->mixRamp = GetIColRamp(GetDlgItem(hWnd, IDC_MIX_RAMP));
	imp->currentSwatch = GetIColSwatch(GetDlgItem(hWnd, IDC_CURRENT_SWATCH));
	imp->originalSwatch = GetIColSwatch(GetDlgItem(hWnd, IDC_ORIGINAL_SWATCH));

	//Flyout buttons
	imp->modeSwitch = GetICustButton(GetDlgItem(hWnd, IDC_MODE));
	himl = ImageList_LoadImage(hInstance, MAKEINTRESOURCE(IDB_BIG), 54, 0,	CLR_DEFAULT, IMAGE_BITMAP, LR_DEFAULTCOLOR);
	imp->modeSwitch->SetImage(himl, 0, 0, 0, 0, 54, 18);
	imp->modeSwitch->SetFlyOff(3, fod, 0, 0, FLY_VARIABLE);
	imp->modeSwitch->SetCurFlyOff(glob.bigMode, FALSE);

	imp->bigPicker = GetIColMode(GetDlgItem(hWnd, IDC_BIG_PICKER));
}
//==============================================================================