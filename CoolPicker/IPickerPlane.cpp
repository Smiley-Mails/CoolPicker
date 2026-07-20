/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				BIG PICKERS							
	This is the Large Plane that shows all			
	sorts of gradients, and has all sorts			
	of color options.								
////////////////////////////////////////////////////*/

#include "IPickerPlane.h"

// Member Functions
void IPickerPlane::UpdatePickerPlane()
{
	PostMessage(hWnd, CC_PLANE_UPDATE, 0, 0);
}

void IPickerPlane::SetMode(int newMode)
{
	mode = newMode;
	glob.planeSubmode = newMode;
}

void IPickerPlane::SetComp(int newComp)
{
	comp = newComp;
}

void IPickerPlane::RepaintGrad()
{
	int i, j;

	switch (mode)
	{
	case MODE_HS:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = HSVtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, ((float)i / (float)(plane_height - 1)) * 255.0, comp);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_HV:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = HSVtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, comp, ((float)i / (float)(plane_height - 1)) * 255.0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_VS:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = HSVtoRGB(comp, ((float)j / (float)(plane_height - 1)) * 255.0, ((float)i / (float)(plane_width - 1)) * 255.0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_RG:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				float r_val = ((float)j / (float)(plane_width - 1));
				float g_val = ((float)i / (float)(plane_height - 1));
				float b_val = (float)comp / 255.0f;

				trivert[plane_width * i + j].Red = (int)(r_val * 255.0f) << 8;
				trivert[plane_width * i + j].Green = (int)(g_val * 255.0f) << 8;
				trivert[plane_width * i + j].Blue = (int)(b_val * 255.0f) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_GB:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				float r_val = (float)comp / 255.0f;
				float g_val = ((float)j / (float)(plane_width - 1));
				float b_val = ((float)i / (float)(plane_height - 1));

				trivert[plane_width * i + j].Red = (int)(r_val * 255.0f) << 8;
				trivert[plane_width * i + j].Green = (int)(g_val * 255.0f) << 8;
				trivert[plane_width * i + j].Blue = (int)(b_val * 255.0f) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_RB:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				float r_val = ((float)j / (float)(plane_width - 1));
				float g_val = (float)comp / 255.0f;
				float b_val = ((float)i / (float)(plane_height - 1));

				trivert[plane_width * i + j].Red = (int)(r_val * 255.0f) << 8;
				trivert[plane_width * i + j].Green = (int)(g_val * 255.0f) << 8;
				trivert[plane_width * i + j].Blue = (int)(b_val * 255.0f) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_CM:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = CMYKtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, ((float)i / (float)(plane_height - 1)) * 255.0, comp, 0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_MY:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = CMYKtoRGB(comp, ((float)j / (float)(plane_width - 1)) * 255.0, ((float)i / (float)(plane_height - 1)) * 255.0, 0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_CY:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = CMYKtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, comp, ((float)i / (float)(plane_height - 1)) * 255.0, 0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_LI:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = LIQtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, ((float)i / (float)(plane_height - 1)) * 255.0, comp);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_IQ:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = LIQtoRGB(comp, ((float)j / (float)(plane_width - 1)) * 255.0, ((float)i / (float)(plane_height - 1)) * 255.0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	case MODE_LQ:
		for (i = 0; i < plane_height; i++)
		{
			for (j = 0; j < plane_width; j++)
			{
				trivert[plane_width * i + j].Alpha = 0x00;
				DWORD rgb = LIQtoRGB(((float)j / (float)(plane_width - 1)) * 255.0, comp, ((float)i / (float)(plane_height - 1)) * 255.0);
				trivert[plane_width * i + j].Red = GetRValue(rgb) << 8;
				trivert[plane_width * i + j].Green = GetGValue(rgb) << 8;
				trivert[plane_width * i + j].Blue = GetBValue(rgb) << 8;
				trivert[plane_width * i + j].x = (j * rect.right) / (plane_width - 1);
				trivert[plane_width * i + j].y = (i * rect.bottom) / (plane_height - 1);
			}
		}
		break;
	}

	GradientFill(memDC, trivert, plane_width * plane_height, gTris, (plane_width - 1) * (plane_height - 1) * 2, GRADIENT_FILL_TRIANGLE);
}

void IPickerPlane::PutPicker(int x, int y)
{
	DrawPickerSprite(hDC, (x / 255.0) * rect.right - 1.0, (y / 255.0) * rect.bottom - 1.0);
}

// SetUp Functions
void ReleaseIPickerPlane(IPickerPlane* pickerPlane)
{
	delete pickerPlane;
}

IPickerPlane* GetIPickerPlane(HWND hwPlane)
{
	return new IPickerPlane(hwPlane);
}

LRESULT WINAPI PickerPlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IPickerPlane* imp;
	static BOOL capped = FALSE;

	switch (msg)
	{
	case CC_PLANE_UPDATE:
		BitBlt(imp->hDC, 0, 0, imp->rect.right, imp->rect.bottom, imp->memDC, 0, 0, SRCCOPY);

		switch (imp->mode)
		{
		case MODE_RG:
		case MODE_HS:
		case MODE_CM:
		case MODE_LI:
			imp->PutPicker(imp->olda, imp->oldb);
			break;
		case MODE_GB:
		case MODE_VS:
		case MODE_MY:
		case MODE_IQ:
			imp->PutPicker(imp->oldb, imp->oldc);
			break;
		case MODE_RB:
		case MODE_HV:
		case MODE_CY:
		case MODE_LQ:
			imp->PutPicker(imp->olda, imp->oldc);
			break;
		}

		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		capped = FALSE;
		break;
	case WM_LBUTTONDOWN:
		if (wParam == MK_LBUTTON)
		{
			SetCapture(hWnd);
			capped = TRUE;
		}
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON && capped)
		{

			float posx, posy;

			posx = (((double)GET_X_LPARAM(lParam)) / ((double)((imp->rect.right - imp->rect.left))));
			posx *= 255.0;

			posy = (((double)GET_Y_LPARAM(lParam)) / ((double)((imp->rect.bottom - imp->rect.top))));
			posy *= 255.0;

			switch (imp->mode)
			{
			case MODE_RG:
			case MODE_HS:
			case MODE_CM:
			case MODE_LI:
				imp->olda = CP_LIMITO_255(posx);
				imp->oldb = CP_LIMITO_255(posy);
				break;
			case MODE_GB:
			case MODE_VS:
			case MODE_MY:
			case MODE_IQ:
				imp->oldb = CP_LIMITO_255(posx);
				imp->oldc = CP_LIMITO_255(posy);
				break;
			case MODE_RB:
			case MODE_HV:
			case MODE_CY:
			case MODE_LQ:
				imp->olda = CP_LIMITO_255(posx);
				imp->oldc = CP_LIMITO_255(posy);
				break;
			}

			//			PostMessage(GetParent(hWnd), CC_PLANE_CHANGE, (UINT)hWnd, 0);
			PostMessage(GetParent(hWnd), CC_PLANE_CHANGE, (WPARAM)hWnd, 0);
		}
		break;
	case WM_ERASEBKGND:
		PostMessage(GetParent(hWnd), CC_PLANE_REPAINT, (WPARAM)hWnd, 0);
		return -1;
		break;
	case CC_PLANE_INIT:
		imp = ((IPickerPlane*)wParam);

		PostMessage(GetParent(hWnd), CC_PLANE_RETEXT, (WPARAM)hWnd, 0);

		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}


INT_PTR CALLBACK ColorPlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND owner;
	static HIMAGELIST himl;
	static IPickerPlane* picker;
	static ICustButton* selector;
	static FlyOffData fod[12] = {
		{0,0,0,0},
		{1,1,1,1},
		{2,2,2,2},
		{3,3,3,3},
		{4,4,4,4},
		{5,5,5,5},
		{6,6,6,6},
		{7,7,7,7},
		{8,8,8,8},
		{9,9,9,9},
		{10,10,10,10},
		{11,11,11,11},
	};

	static const MCHAR* descText[12] = {
			_T("Red vs. Green"),
			_T("Green vs. Blue"),
			_T("Red vs. Blue"),
			_T("Hue vs. Saturation"),
			_T("Saturation vs. Value"),
			_T("Hue vs. Value"),
			_T("Cyan vs. Magenta"),
			_T("Magenta vs. Yellow"),
			_T("Cyan vs. Yellow"),
			_T("Luminance vs. Inphase"),
			_T("Inphase vs. Quadrature"),
			_T("Luminance vs. Quadrature"),
	};

	switch (msg)
	{
	case CC_PLANE_CHANGE:
		picker->UpdatePickerPlane();
		//		switch (picker->mode)
		//		{
		//		case MODE_HS:
		//		case MODE_VS:
		//		case MODE_HV:
		PostMessage(GetParent(hWnd), CC_BIG_CHANGE, (WPARAM)RGB(picker->olda, picker->oldb, picker->oldc), 0);
		//			break;
		//		case MODE_RG:
		//		case MODE_GB:
		//		case MODE_RB:
		//			PostMessage(GetParent(hWnd), CC_BIG_CHANGE, (WPARAM)RGB(picker->olda, picker->oldb, picker->oldc), 0);
		//			break;
		//		}
		break;
	case CC_PLANE_REPAINT:
		PostMessage((HWND)wParam, CC_PLANE_UPDATE, 0, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SELECTOR:
			switch (selector->GetCurFlyOff())
			{
			case 0:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_RG, 0);
				picker->SetMode(MODE_RG);
				picker->SetComp(picker->oldc);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 1:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_GB, 0);
				picker->SetMode(MODE_GB);
				picker->SetComp(picker->olda);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 2:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_RB, 0);
				picker->SetMode(MODE_RB);
				picker->SetComp(picker->oldb);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 3:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_HS, 0);
				picker->SetMode(MODE_HS);
				picker->SetComp(picker->oldc);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 4:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_VS, 0);
				picker->SetMode(MODE_VS);
				picker->SetComp(picker->olda);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 5:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_HV, 0);
				picker->SetMode(MODE_HV);
				picker->SetComp(picker->oldb);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 6:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_CM, 0);
				picker->SetMode(MODE_CM);
				picker->SetComp(picker->oldc);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 7:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_MY, 0);
				picker->SetMode(MODE_MY);
				picker->SetComp(picker->olda);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 8:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_CY, 0);
				picker->SetMode(MODE_CY);
				picker->SetComp(picker->oldb);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 9:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_LI, 0);
				picker->SetMode(MODE_LI);
				picker->SetComp(picker->oldc);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 10:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_IQ, 0);
				picker->SetMode(MODE_IQ);
				picker->SetComp(picker->olda);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			case 11:
				PostMessage(GetParent(hWnd), CC_BIG_CHANGEDMODE, MODE_LQ, 0);
				picker->SetMode(MODE_LQ);
				picker->SetComp(picker->oldb);
				picker->RepaintGrad();
				picker->UpdatePickerPlane();
				break;
			}
		}
	case CC_PLANE_RETEXT:
		SetWindowText(GetDlgItem(hWnd, IDC_DESCRIPTION), descText[selector->GetCurFlyOff()]);
		break;
	case CC_BIG_COLORCHANGED:
	{
		int a, b, c;

		a = GetRValue((COLORREF)wParam);
		b = GetGValue((COLORREF)wParam);
		c = GetBValue((COLORREF)wParam);

		switch (picker->mode)
		{
		case MODE_RG:
		case MODE_HS:
		case MODE_CM:
		case MODE_LI:
			if (picker->oldc != c)
			{
				picker->comp = c;
				picker->RepaintGrad();
			}
			break;
		case MODE_GB:
		case MODE_VS:
		case MODE_MY:
		case MODE_IQ:
			if (picker->olda != a)
			{
				picker->comp = a;
				picker->RepaintGrad();
			}
			break;
		case MODE_RB:
		case MODE_HV:
		case MODE_CY:
		case MODE_LQ:
			if (picker->oldb != b)
			{
				picker->comp = b;
				picker->RepaintGrad();
			}
			break;
		}

		picker->olda = a;
		picker->oldb = b;
		picker->oldc = c;

		picker->UpdatePickerPlane();

		break;
	}
	case WM_INITDIALOG:
		owner = hWnd;

		picker = GetIPickerPlane(GetDlgItem(hWnd, IDC_PICKER_PLANE));
		//		picker->UpdatePickerPlane();

		selector = GetICustButton(GetDlgItem(hWnd, IDC_SELECTOR));

		himl = ImageList_LoadImage(hInstance, MAKEINTRESOURCE(IDB_PLANE), 24, 0, CLR_DEFAULT, IMAGE_BITMAP, LR_DEFAULTCOLOR);
		selector->SetImage(himl, 0, 0, 0, 0, 24, 16);
		selector->SetFlyOff(12, fod, 0, glob.planeSubmode, FLY_VARIABLE);
		//		PostMessage(hWnd, 
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		ReleaseIPickerPlane(picker);
		ReleaseICustButton(selector);
		DebugPrint(_M("%d\n"), ImageList_Destroy(himl));

		EndDialog(hWnd, 0);
		break;
	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
		break;
	}
	return FALSE;
}