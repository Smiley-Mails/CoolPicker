/**********************************************************************
 *<
	FILE: CoolPick.h

	DESCRIPTION: Template Utility

	CREATED BY: Daniel Rind

	MODIFIED BY: Chris Johnson

	HISTORY: Version 2.0 updates include floating point conversions...

 *>	Copyright (c) 2005, All Rights Reserved.
 **********************************************************************/

#ifndef __COOLPICK__H
#define __COOLPICK__H

//from the windows API
#define LEAN_AND_MEAN

//WARNING SUPPRESSION:
//These warnings are disabled because if I didn't, the code would
//take a very long time to compile. The reason I have to supress these in the first
//place is because this entire project is set to compile at warning level 4. I 
//thought the extra oversight from the compiler would be helpful. Unfortunately
//the 3dsMax SDK is compiled at warning level 3, and due to the tight coupling 
//between the 3dsMax SDK header files, almost the entire sdk is included and parsed
//in this project, and thus thousands of warnings.... It's really a sad situation. :(

//#pragma warning (disable : 4100 4238 4239 4244 4245 4515 4512)
//#pragma warning (default: 4263 4264)

//from the 3ds max SDK
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <maxscript\maxscript.h>
#include "hsv.h"

//Cool Picker specific header files
#include "global_constants.h"
#include "global_options.h"
#include "misc_graphics_functions.h"

//Custom Color Conversion Functions
#include "MinMax.h"
#include "RGB_HSV_ConversionFNS.h"
#include "RGB_CMYK_ConversionFNS.h"
#include "RGB_LIQ_ConversionFNS.h"

//Custom Cool Picker Controls (Small controls)
#include "IColRamp.h"
#include "IColSwatch.h"
#include "IColMode.h"

//Custom Cool Picker Controls (Larger controls)
#include "IPickerPlane.h"
#include "IPickerMixer.h"
#include "IPickerTriCircle.h"

//Forward Declarations
//INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//Global Variables
#define COOLPICKER_CLASS_ID	Class_ID(0x434f96fc, 0x78aad528);

//from DllEntry.cpp
extern MCHAR* GetString(int id); 
extern HINSTANCE hInstance; 

class CoolPickerDlg; //forward declaration

void SetUpIntControls(HWND hWnd, CoolPickerDlg* imp, HIMAGELIST &himl, FlyOffData* fod );
void SetUpFltControls(HWND hWnd, CoolPickerDlg* imp, HIMAGELIST &himl, FlyOffData* fod );

#endif // __COOLPICK__H
