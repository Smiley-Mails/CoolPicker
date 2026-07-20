/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				COLOR RAMPS							
	This is the long narrow horizontal ramps that	
	that are used to specify each color component	
////////////////////////////////////////////////////*/
#include "IColRamp.h"
// Member Functions:
void IColRamp::SetColors(COLORREF col1, COLORREF col2, COLORREF col3)
{
	lCol = col1;
	rCol = col2;
	mCol = col3;
}
void IColRamp::SetColors(AColor col1, AColor col2, AColor col3)
{
	f_leftCol = col1;
	lCol = (COLORREF)col1.toRGB();
	f_rightCol = col2;
	rCol = (COLORREF)col2.toRGB();
	f_middleCol = col3;
	mCol = (COLORREF)col3.toRGB();
}
void IColRamp::SetRainbow()
{
	rainbow = TRUE;
}

void IColRamp::SetPos(int pos)
{
	pPos = pos;
}
void IColRamp::SetPos(float pos)
{
	f_Pos = pos;
	pPos = (int) pos*255;
}
void IColRamp::UpdateRamp()
{
	PostMessage(hWnd, CC_RAMP_UPDATE, (WPARAM)this, 0);
}
// Setup Functions:
IColRamp *GetIColRamp(HWND hwRamp)
{
	return new IColRamp(hwRamp);
}

void ReleaseIColRamp(IColRamp *colRamp)
{
	delete colRamp;
}

LRESULT WINAPI ColorRampProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case CC_RAMP_UPDATE:
			{
			if (glob.rangeType == 1)
			{
				if (((IColRamp*)wParam)->rainbow)
				{
					FillRainbow(hWnd);
				}
				else 
				{
					AColor left = ((IColRamp*)wParam)->f_leftCol;
					AColor right = ((IColRamp*)wParam)->f_rightCol;
					AColor middle = ((IColRamp*)wParam)->f_middleCol;
					FillGradient(hWnd, (COLORREF)left.toRGB(), (COLORREF)right.toRGB(), (COLORREF)middle.toRGB() );
				}
				FPutPicker(hWnd, ((IColRamp*)wParam)->f_Pos );
				break;
			}
			else if (glob.rangeType != 1)
			{
				if (((IColRamp*)wParam)->rainbow)
				{
					FillRainbow(hWnd);
				}
				else
				{
					FillGradient(hWnd,	((IColRamp*)wParam)->lCol,	\
										((IColRamp*)wParam)->rCol,	\
										((IColRamp*)wParam)->mCol );
				}
				PutPicker(hWnd, ((IColRamp*)wParam)->pPos);
				break;
			}
		}
		case WM_LBUTTONUP:
			PostMessage(GetParent(hWnd), CC_RAMP_ENDCHANGE, (WPARAM)hWnd, 0);
			ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			if (wParam == MK_LBUTTON)
			{		
				RECT rect;
				double pos;

				SetCapture(hWnd);

				GetClientRect(hWnd, &rect);
				
				pos = ( ((double)GET_X_LPARAM(lParam)) / ((double)((rect.right - rect.left)-3)) );
					
				if (glob.rangeType == 1)
				{
					pos *= 255.0;
//					PostMessage(GetParent(hWnd), CC_RAMP_CHANGE, (UINT)hWnd, (UINT)CP_LIMITO_255(pos));
					PostMessage(GetParent(hWnd), CC_RAMP_CHANGE, (WPARAM)hWnd, (UINT)CP_LIMITO_255(pos));
				}
				else if (glob.rangeType != 1)
				{
					pos *= 255.0;
//					PostMessage(GetParent(hWnd), CC_RAMP_CHANGE, (UINT)hWnd, (UINT)CP_LIMITO_255(pos));
					PostMessage(GetParent(hWnd), CC_RAMP_CHANGE, (WPARAM)hWnd, (UINT)CP_LIMITO_255(pos));
				}	
			}
			break;
		case WM_ERASEBKGND:
			PostMessage(GetParent(hWnd), CC_RAMP_REPAINT, (WPARAM)hWnd, 0);
			return -1;
			break;
		case WM_CREATE:
		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam ));
	}
	return FALSE;
}
