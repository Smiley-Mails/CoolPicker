/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
					Global Constants
////////////////////////////////////////////////////*/
#ifndef _Global_Constants_
#define _Global_Constants_

#include <windows.h>

//Used in CoolPick.cpp
const unsigned short SPACE_ALL  = 0;
const unsigned short SPACE_RGB  = 1;
const unsigned short SPACE_HSV  = 2;
const unsigned short SPACE_CMYK = 3;
const unsigned short SPACE_LIQ  = 4;

//Used in class IPickerTriCircle
const unsigned int CC_PAINTER_INIT	  = WM_USER;
const unsigned int CC_PAINTER_UPDATE  = WM_USER+1;
const unsigned int CC_PAINTER_REPAINT = WM_USER+2;
const unsigned int CC_PAINTER_CHANGE  = WM_USER+3;

//Used in class IPickerTriCircle
const unsigned short int ringCross = 25;

//Used in class IPickerMixer
const unsigned int CC_MIXER_INIT	= WM_USER;
const unsigned int CC_MIXER_UPDATE	= WM_USER+1;
const unsigned int CC_MIXER_REPAINT = WM_USER+2;
const unsigned int CC_MIXER_CHANGE	= WM_USER+3;

//Used in class IPickerMixer
const int extmin = -20;
const int extmax = 20;
const int ext	 = 40;

//Used in class IPickerPlane
const unsigned int CC_PLANE_INIT	= WM_USER;
const unsigned int CC_PLANE_UPDATE	= WM_USER+1;
const unsigned int CC_PLANE_REPAINT = WM_USER+2;
const unsigned int CC_PLANE_CHANGE	= WM_USER+3;
const unsigned int CC_PLANE_RETEXT	= WM_USER+4;

//Used in class IPickerPlane
const unsigned short int plane_width  = 25;
const unsigned short int plane_height = 25;

const MCHAR CONFIGNAME[] = _T("CoolPick.ini");

//Constants used by class IColRamp 
const unsigned int CC_RAMP_UPDATE	 = WM_USER;
const unsigned int CC_RAMP_REPAINT	 = WM_USER+1;
const unsigned int CC_RAMP_CHANGE	 = WM_USER+2;
const unsigned int CC_RAMP_ENDCHANGE = WM_USER+3;

//Constants used by class IColSwatch 
const unsigned int CC_SWATCH_UPDATE	 = WM_USER+10;
const unsigned int CC_SWATCH_REPAINT = WM_USER+11;

//Constants used by class IColMode and in CoolPick.cpp
const short int MODE_BIGPLANE = 0;
const short int MODE_TRIANGLE = 1;
const short int MODE_MIXER	  = 2;
const short int MODE_PALETTE  = 3;

//Constants used in class IPickerPlane and in CoolPick.cpp
const short int MODE_RG = 0;
const short int MODE_GB = 1;
const short int MODE_RB = 2;
const short int MODE_HS = 3; //Default value in class GlobalOptions contructor (global_options.h)
const short int MODE_VS = 4;
const short int MODE_HV = 5;
const short int MODE_CM = 6;
const short int MODE_MY = 7;
const short int MODE_CY = 8;
const short int MODE_LI = 9;
const short int MODE_IQ = 10;
const short int MODE_LQ = 11;

//Constants used by class IColMode
const unsigned int CC_BIG_INIT			= WM_USER+20;
const unsigned int CC_BIG_SETMODE		= WM_USER+21;
const unsigned int CC_BIG_CHANGEDMODE	= WM_USER+22; //Also used in class IPickerPlane and coolpick.cpp
const unsigned int CC_BIG_COLORCHANGED	= WM_USER+23; //Used in All three IPickerXXXX classes
const unsigned int CC_BIG_SETCOLOR		= WM_USER+24; 
const unsigned int CC_BIG_CHANGE		= WM_USER+25; //Used almost everywhere

#endif