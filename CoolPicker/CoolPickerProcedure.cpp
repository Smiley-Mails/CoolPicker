#include "CoolPickerProcedure.h"
#include "3dsmaxport.h"

//This is the Master Dialog Procedure that handles everything. It is Very Large.
INT_PTR CALLBACK CoolPickerOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CoolPickerDlg* imp = NULL;
	static HIMAGELIST himl;
	static FlyOffData fod[3] = {
		{0,0,0,0},
		{1,1,1,1},
		{2,2,2,2},
	};

	static BOOL bPickingFromScreen = FALSE;
	static HCURSOR oldCursor;

	if (message == WM_INITDIALOG)
	{
		imp = (CoolPickerDlg*)lParam;
		if (imp) { return imp->InitDialog(hWnd, message, wParam, lParam, himl, fod); }
	}
	else
	{
		imp = DLGetWindowLongPtr<CoolPickerDlg*>(hWnd);
		switch (message) {
		case WM_NCHITTEST:
		{
			// Получаем стандартный результат проверки попадания курсора
			LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);

			// Если клик пришелся на пустую область окна (фон)
			if (hit == HTCLIENT)
			{
				// Говорим Windows, что курсор находится на заголовке, чтобы работало перетаскивание
				SetWindowLongPtr(hWnd, DWLP_MSGRESULT, HTCAPTION);
				return TRUE;
			}
			return FALSE; // Позволяем кнопкам и другим элементам нормально работать
		}

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDC_RAD_R255:
				if (SendMessage(GetDlgItem(hWnd, IDC_RAD_R255), BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R100), BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), BM_SETCHECK, BST_UNCHECKED, 0);

					// Синхронизация: копируем текущий цвет из Float в Integer
					if (glob.rangeType == 1)
					{
						imp->currentRGB = RGB(
							CP_LIMITO_255((int)(imp->a_currentRGB.r * 255.0f)),
							CP_LIMITO_255((int)(imp->a_currentRGB.g * 255.0f)),
							CP_LIMITO_255((int)(imp->a_currentRGB.b * 255.0f))
						);
						imp->origRGB = RGB(
							CP_LIMITO_255((int)(imp->a_origRGB.r * 255.0f)),
							CP_LIMITO_255((int)(imp->a_origRGB.g * 255.0f)),
							CP_LIMITO_255((int)(imp->a_origRGB.b * 255.0f))
						);
					}

					SetUpIntControls(hWnd, imp, himl, fod);

					glob.rangeType = 255;
					glob.rangeMax = 255;
					glob.rangeCoeff = 1.0f;
					imp->mixRamp->SetPos(255);
					imp->SetSliderRange();
					imp->SetSliders(SPACE_ALL, FALSE);
					imp->SetRamps();
				}
				break;

			case IDC_RAD_R100:
				if (SendMessage(GetDlgItem(hWnd, IDC_RAD_R100), BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R255), BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), BM_SETCHECK, BST_UNCHECKED, 0);

					// Синхронизация: копируем текущий цвет из Float в Integer
					if (glob.rangeType == 1)
					{
						imp->currentRGB = RGB(
							CP_LIMITO_255((int)(imp->a_currentRGB.r * 255.0f)),
							CP_LIMITO_255((int)(imp->a_currentRGB.g * 255.0f)),
							CP_LIMITO_255((int)(imp->a_currentRGB.b * 255.0f))
						);
						imp->origRGB = RGB(
							CP_LIMITO_255((int)(imp->a_origRGB.r * 255.0f)),
							CP_LIMITO_255((int)(imp->a_origRGB.g * 255.0f)),
							CP_LIMITO_255((int)(imp->a_origRGB.b * 255.0f))
						);
					}

					SetUpIntControls(hWnd, imp, himl, fod);

					glob.rangeType = 100;
					glob.rangeMax = 100;
					glob.rangeCoeff = 2.550f;
					imp->mixRamp->SetPos(255);
					imp->SetSliderRange();
					imp->SetSliders(SPACE_ALL, FALSE);
					imp->SetRamps();
				}
				break;

			case IDC_RAD_R1FLOAT:
				if (SendMessage(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R100), BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hWnd, IDC_RAD_R255), BM_SETCHECK, BST_UNCHECKED, 0);

					// 1. Читаем альфу из старого спиннера ДО его пересоздания
					float alpha = 1.0f;
					if (imp->alphaSpin) {
						alpha = imp->alphaSpin->GetFVal();
						if (alpha == 0.0f) {
							alpha = 1.0f;
						}
					}

					// Синхронизация: копируем текущий цвет из Integer во Float
					if (glob.rangeType != 1)
					{
						imp->a_currentRGB.r = GetRValue(imp->currentRGB) / 255.0f;
						imp->a_currentRGB.g = GetGValue(imp->currentRGB) / 255.0f;
						imp->a_currentRGB.b = GetBValue(imp->currentRGB) / 255.0f;

						imp->a_origRGB.r = GetRValue(imp->origRGB) / 255.0f;
						imp->a_origRGB.g = GetGValue(imp->origRGB) / 255.0f;
						imp->a_origRGB.b = GetBValue(imp->origRGB) / 255.0f;
					}

					// 2. Пересоздаем все спиннеры в режим Float (с плавающей точкой)!
					SetUpFltControls(hWnd, imp, himl, fod);

					// 3. Записываем альфу и проверяем оригинальный цвет
					imp->a_currentRGB.a = alpha;
					if (imp->a_origRGB.a == 0.0f) {
						imp->a_origRGB.a = 1.0f;
					}

					// 4. Обновляем переменные среды для Float
					glob.rangeType = 1;
					glob.rangeMax = 1;
					glob.rangeCoeff = 255.0f;

					// 5. Загружаем значения в интерфейс
					imp->mixRamp->SetPos(1.0f);
					imp->fSetSliderRange();
					imp->fSetSliders(SPACE_ALL, FALSE);
					imp->fSetRamps();

					// 6. Включаем интерфейс альфа-канала (так как перешли во Float)
					if (imp->alphaSpin) {
						imp->alphaSpin->Enable(TRUE);
					}
				}
				break;

			case IDC_MODE:
				switch (imp->modeSwitch->GetCurFlyOff())
				{
				case 0:
					imp->bigPicker->SetMode(MODE_BIGPLANE);
					break;
				case 1:
					imp->bigPicker->SetMode(MODE_TRIANGLE);
					break;
				case 2:
					imp->bigPicker->SetMode(MODE_MIXER);
					break;
				case 3:
					imp->bigPicker->SetMode(MODE_PALETTE);
					break;
				}
				switch (glob.rangeType)
				{
				case 1: { imp->fSetRamps(); break; }
				default: { imp->SetRamps(); break; }
				}
				break;
			case IDC_STATIC_PICK_DRAG_START:
			{
				SetCapture(hWnd);
				bPickingFromScreen = true;

				HCURSOR pickCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SCREEN_PICKER));
				oldCursor = SetCursor(pickCursor);
				const DWORD OCR_NORMAL = 32512;
				SetSystemCursor(pickCursor, OCR_NORMAL);
				break;
			}
			case IDRESET:
				if (glob.rangeType == 1)
				{
					imp->a_currentRGB = imp->a_origRGB;
					imp->fSetSliders(SPACE_ALL, FALSE);
					imp->fSetCurrentColor(SPACE_RGB, TRUE);
					imp->fSetRamps();
				}
				else
				{
					imp->currentRGB = imp->origRGB;
					imp->SetSliders(SPACE_ALL, FALSE);
					imp->SetCurrentColor(SPACE_RGB, TRUE);
					imp->SetRamps();
				}
				break;
			case IDCANCEL:
				switch (glob.rangeType)
				{
				case 1:
				{
					imp->a_currentRGB = imp->a_origRGB;
					// imp->callback->ColorChanged(imp->a_currentRGB, 1);
					imp->callback->ColorChanged(imp->DisplayToWorking(imp->a_currentRGB), 1);
					break;
				}
				case 100:
				case 255:
				{
					imp->currentRGB = imp->origRGB;
					// imp->callback->ColorChanged(AColor(Color(imp->currentRGB)), 1);
					imp->callback->ColorChanged(imp->DisplayToWorking(AColor(Color(imp->currentRGB))), 1);
					break;
				}
				}
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			case IDOK:
				switch (glob.rangeType)
				{
				case 1:
				{
					// imp->callback->ColorChanged(imp->a_currentRGB, 1);
					imp->callback->ColorChanged(imp->DisplayToWorking(imp->a_currentRGB), 1);
					break;
				}
				case 100:
				case 255:
				{
					if (imp->objColor)
					{
						::SendMessage(imp->hwOwner, WM_ADD_COLOR, imp->currentRGB, imp->currentRGB);
					}
					else
					{
						// imp->callback->ColorChanged(AColor(Color(imp->currentRGB)), 1);
						imp->callback->ColorChanged(imp->DisplayToWorking(AColor(Color(imp->currentRGB))), 1);
						break;
					}
				}
				}
				PostMessage(hWnd, WM_CLOSE, 1, 0);
				break;

			case IDC_ABOUT:
			{
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT_DLG), hWnd, AboutDlgProc);
				break;
			}
			}
			break;

		case CC_SPINNER_CHANGE:
		case WM_CUSTEDIT_ENTER:
		{
			switch (LOWORD(wParam))
			{
			case IDC_RED_SPIN:
			case IDC_GREEN_SPIN:
			case IDC_BLUE_SPIN:
				if (glob.rangeType == 1)
				{
					imp->fSetCurrentColor(SPACE_RGB, TRUE);
					imp->fSetSliders(SPACE_RGB, FALSE);
				}
				else if (glob.rangeType != 1)
				{
					imp->SetCurrentColor(SPACE_RGB, TRUE);
					imp->SetSliders(SPACE_RGB, FALSE);
				}
				break;
			case IDC_HUE_SPIN:
			case IDC_SAT_SPIN:
			case IDC_VAL_SPIN:
				if (glob.rangeType == 1)
				{
					imp->fSetCurrentColor(SPACE_HSV, TRUE);
					imp->fSetSliders(SPACE_HSV, FALSE);
				}
				else if (glob.rangeType != 1)
				{
					imp->SetCurrentColor(SPACE_HSV, TRUE);
					imp->SetSliders(SPACE_HSV, FALSE);
				}
				break;
			case IDC_CYAN_SPIN:
			case IDC_MAGENTA_SPIN:
			case IDC_YELLOW_SPIN:
			case IDC_BLACK_SPIN:
				if (glob.rangeType == 1)
				{
					imp->fSetCurrentColor(SPACE_CMYK, TRUE);
					imp->fSetSliders(SPACE_CMYK, FALSE);
				}
				else if (glob.rangeType != 1)
				{
					imp->SetCurrentColor(SPACE_CMYK, TRUE);
					imp->SetSliders(SPACE_CMYK, FALSE);
				}
				break;
			case IDC_LUMA_SPIN:
			case IDC_INP_SPIN:
			case IDC_QUAD_SPIN:
				if (glob.rangeType == 1)
				{
					imp->fSetCurrentColor(SPACE_LIQ, TRUE);
					imp->fSetSliders(SPACE_LIQ, FALSE);
				}
				else if (glob.rangeType != 1)
				{
					imp->SetCurrentColor(SPACE_LIQ, TRUE);
					imp->SetSliders(SPACE_LIQ, FALSE);
				}
				break;
			case IDC_ALPHA_SPIN2:
				if (glob.rangeType == 1) // Если мы в режиме Float
				{
					// Сохраняем новую Альфу в структуру и отправляем апдейт в Max
					imp->fSetCurrentColor(SPACE_RGB, TRUE);
				}
				imp->alphaRamp->UpdateRamp();
				break;
			}

			if (glob.rangeType == 1)
			{
				imp->fSetRamps();
			}
			else if (glob.rangeType != 1)
			{
				imp->SetRamps();
			}
		}
		break;

		case CC_RAMP_ENDCHANGE:
		{	//Only for the Mixer ramp at bottom
			if (GetDlgCtrlID((HWND)wParam) == IDC_MIX_RAMP)
			{
				if (glob.rangeType == 1)
				{
					imp->mixRamp->SetPos(1.0f);
					imp->fSetRamps();
				}
				else if (glob.rangeType != 1)
				{
					imp->mixRamp->SetPos(255);
					imp->SetRamps();
				}
			}
		}
		break;

		case CC_RAMP_CHANGE:
		{
			if (glob.rangeType == 1)
			{
				float value = ((float)lParam / 255);
				switch (GetDlgCtrlID((HWND)wParam))
				{
					//rgb
				case IDC_RED_RAMP: { imp->rSpin->SetValue(value, TRUE); break; }
				case IDC_GREEN_RAMP: { imp->gSpin->SetValue(value, TRUE); break; }
				case IDC_BLUE_RAMP: { imp->bSpin->SetValue(value, TRUE); break; }
								  //hsv
				case IDC_HUE_RAMP: { imp->hSpin->SetValue(value, TRUE); break; }
				case IDC_SAT_RAMP: { imp->sSpin->SetValue(value, TRUE); break; }
				case IDC_VAL_RAMP: { imp->vSpin->SetValue(value, TRUE); break; }
								 //cymk
				case IDC_CYAN_RAMP: { imp->cSpin->SetValue(value, TRUE); break; }
				case IDC_MAGENTA_RAMP: { imp->mSpin->SetValue(value, TRUE); break; }
				case IDC_YELLOW_RAMP: { imp->ySpin->SetValue(value, TRUE);	break; }
				case IDC_BLACK_RAMP: { imp->kSpin->SetValue(value, TRUE); break; }
								   //LIQ
				case IDC_LUMA_RAMP: { imp->lSpin->SetValue(value, TRUE); break; }
				case IDC_INP_RAMP: { imp->iSpin->SetValue(value, TRUE); break; }
				case IDC_QUAD_RAMP: { imp->qSpin->SetValue(value, TRUE); break; }
								  //Alpha
				case IDC_ALPHA_RAMP: { imp->alphaSpin->SetValue(value, TRUE); break; }
				case IDC_MIX_RAMP:
				{
					float r, g, b, a;
					float coeff;

					imp->mixRamp->SetPos(value);

					coeff = imp->mixRamp->f_Pos;

					r = imp->a_origRGB.r * (1.0f - coeff) + imp->mixRamp->f_rightCol.r * coeff;
					g = imp->a_origRGB.g * (1.0f - coeff) + imp->mixRamp->f_rightCol.g * coeff;
					b = imp->a_origRGB.b * (1.0f - coeff) + imp->mixRamp->f_rightCol.b * coeff;

					a = imp->alphaSpin->GetFVal();

					imp->a_currentRGB = AColor(r, g, b, a);
					// imp->callback->ColorChanged(imp->a_currentRGB, 1);
					imp->callback->ColorChanged(imp->DisplayToWorking(imp->a_currentRGB), 1);

					imp->fSetSliders(SPACE_ALL, FALSE);
					imp->fSetRamps();
					break;
				}
				}
			}
			else if (glob.rangeType != 1)
			{
				switch (GetDlgCtrlID((HWND)wParam))
				{
				case IDC_RED_RAMP:
					imp->rSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_GREEN_RAMP:
					imp->gSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_BLUE_RAMP:
					imp->bSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_HUE_RAMP:
					imp->hSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_SAT_RAMP:
					imp->sSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_VAL_RAMP:
					imp->vSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_CYAN_RAMP:
					imp->cSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_MAGENTA_RAMP:
					imp->mSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_YELLOW_RAMP:
					imp->ySpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_BLACK_RAMP:
					imp->kSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_LUMA_RAMP:
					imp->lSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_INP_RAMP:
					imp->iSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_QUAD_RAMP:
					imp->qSpin->SetValue((int)(lParam / glob.rangeCoeff), TRUE);
					break;
				case IDC_ALPHA_RAMP:
					break;
				case IDC_MIX_RAMP:
				{
					int r = 0, g = 0, b = 0;
					float coeff = 0.0f;

					imp->mixRamp->SetPos((float)lParam);
					imp->mixRamp->SetPos((int)lParam);

					coeff = ((float)imp->mixRamp->pPos) / 255.0f;

					r = (int)(((float)GetRValue(imp->origRGB)) * (1.0f - coeff) + ((float)GetRValue(imp->mixRamp->rCol)) * coeff);
					g = (int)(((float)GetGValue(imp->origRGB)) * (1.0f - coeff) + ((float)GetGValue(imp->mixRamp->rCol)) * coeff);
					b = (int)(((float)GetBValue(imp->origRGB)) * (1.0f - coeff) + ((float)GetBValue(imp->mixRamp->rCol)) * coeff);

					imp->currentRGB = RGB(r, g, b);
					// imp->callback->ColorChanged(AColor(Color(imp->currentRGB)), 1);
					imp->callback->ColorChanged(imp->DisplayToWorking(AColor(Color(imp->currentRGB))), 1);

					imp->SetSliders(SPACE_ALL, FALSE);
					imp->SetRamps();
					break;
				}
				}
			}
		}
		break;

		case CC_SWATCH_REPAINT:
		case CC_RAMP_REPAINT:
		{
			switch (GetDlgCtrlID((HWND)wParam))
			{
			case IDC_RED_RAMP:		imp->rRamp->UpdateRamp(); break;
			case IDC_GREEN_RAMP:	imp->gRamp->UpdateRamp(); break;
			case IDC_BLUE_RAMP:		imp->bRamp->UpdateRamp(); break;
			case IDC_HUE_RAMP:		imp->hRamp->UpdateRamp(); break;
			case IDC_SAT_RAMP:		imp->sRamp->UpdateRamp(); break;
			case IDC_VAL_RAMP:		imp->vRamp->UpdateRamp(); break;
			case IDC_CYAN_RAMP:		imp->cRamp->UpdateRamp(); break;
			case IDC_MAGENTA_RAMP:	imp->mRamp->UpdateRamp(); break;
			case IDC_YELLOW_RAMP:	imp->yRamp->UpdateRamp(); break;
			case IDC_BLACK_RAMP:	imp->kRamp->UpdateRamp(); break;
			case IDC_LUMA_RAMP:		imp->lRamp->UpdateRamp(); break;
			case IDC_INP_RAMP:		imp->iRamp->UpdateRamp(); break;
			case IDC_QUAD_RAMP:		imp->qRamp->UpdateRamp(); break;
			case IDC_ALPHA_RAMP:	imp->alphaRamp->UpdateRamp(); break;
			case IDC_MIX_RAMP:		imp->mixRamp->UpdateRamp(); break;
			case IDC_ORIGINAL_SWATCH: imp->originalSwatch->UpdateSwatch(); break;
			case IDC_CURRENT_SWATCH:  imp->currentSwatch->UpdateSwatch(); break;
			}
		}
		break;

		case CC_BIG_CHANGEDMODE:
		{
			if (glob.rangeType == 1) { imp->fSetRamps(); }
			else { imp->SetRamps(); }
		}
		break;

		case CC_BIG_COLORCHANGED:
		{
			switch (imp->bigPicker->mode)
			{
			case MODE_BIGPLANE:
				if (glob.rangeType == 1) {
					switch (imp->bigPicker->submode)
					{
					case MODE_HS:
					case MODE_HV:
					case MODE_VS:
						imp->hSpin->SetValue((float)((GetRValue(wParam) / 255.0f)), FALSE);
						imp->sSpin->SetValue((float)((GetGValue(wParam) / 255.0f)), FALSE);
						imp->vSpin->SetValue((float)((GetBValue(wParam) / 255.0f)), FALSE);

						imp->fSetCurrentColor(SPACE_HSV, TRUE);
						imp->fSetSliders(SPACE_HSV, FALSE);
						break;
					case MODE_RG:
					case MODE_GB:
					case MODE_RB:
						imp->rSpin->SetValue((float)(GetRValue(wParam) / 255.0f), FALSE);
						imp->gSpin->SetValue((float)(GetGValue(wParam) / 255.0f), FALSE);
						imp->bSpin->SetValue((float)(GetBValue(wParam) / 255.0f), FALSE);

						imp->fSetCurrentColor(SPACE_RGB, TRUE);
						imp->fSetSliders(SPACE_RGB, FALSE);
						break;
					case MODE_CM:
					case MODE_MY:
					case MODE_CY:
						imp->cSpin->SetValue((float)(GetRValue(wParam) / 255.0f), FALSE);
						imp->mSpin->SetValue((float)(GetGValue(wParam) / 255.0f), FALSE);
						imp->ySpin->SetValue((float)(GetBValue(wParam) / 255.0f), FALSE);
						imp->kSpin->SetValue(0.0f, FALSE);

						imp->fSetCurrentColor(SPACE_CMYK, TRUE);
						imp->fSetSliders(SPACE_CMYK, FALSE);
						break;
					case MODE_LI:
					case MODE_IQ:
					case MODE_LQ:
						imp->lSpin->SetValue((float)(GetRValue(wParam) / 255.0f), FALSE);
						imp->iSpin->SetValue((float)(GetGValue(wParam) / 255.0f), FALSE);
						imp->qSpin->SetValue((float)(GetBValue(wParam) / 255.0f), FALSE);

						imp->fSetCurrentColor(SPACE_LIQ, TRUE);
						imp->fSetSliders(SPACE_LIQ, FALSE);
						break;
					}
				}
				else {
					switch (imp->bigPicker->submode)
					{
					case MODE_HS:
					case MODE_HV:
					case MODE_VS:
						imp->hSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
						imp->sSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
						imp->vSpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);

						imp->SetCurrentColor(SPACE_HSV, TRUE);
						imp->SetSliders(SPACE_HSV, FALSE);
						break;
					case MODE_RG:
					case MODE_GB:
					case MODE_RB:
						imp->rSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
						imp->gSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
						imp->bSpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);

						imp->SetCurrentColor(SPACE_RGB, TRUE);
						imp->SetSliders(SPACE_RGB, FALSE);
						break;
					case MODE_CM:
					case MODE_MY:
					case MODE_CY:
						imp->cSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
						imp->mSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
						imp->ySpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);
						imp->kSpin->SetValue(0, FALSE);

						imp->SetCurrentColor(SPACE_CMYK, TRUE);
						imp->SetSliders(SPACE_CMYK, FALSE);
						break;
					case MODE_LI:
					case MODE_IQ:
					case MODE_LQ:
						imp->lSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
						imp->iSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
						imp->qSpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);

						imp->SetCurrentColor(SPACE_LIQ, TRUE);
						imp->SetSliders(SPACE_LIQ, FALSE);
						break;
					}
				}
				break;

			case MODE_TRIANGLE:
				if (glob.rangeType == 1)
				{
					imp->hSpin->SetValue((float)((GetRValue(wParam) / 255.0f)), FALSE);
					imp->sSpin->SetValue((float)((GetGValue(wParam) / 255.0f)), FALSE);
					imp->vSpin->SetValue((float)((GetBValue(wParam) / 255.0f)), FALSE);

					imp->fSetCurrentColor(SPACE_HSV, TRUE);
					imp->fSetSliders(SPACE_HSV, FALSE);
				}
				else
				{
					imp->hSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
					imp->sSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
					imp->vSpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);

					imp->SetCurrentColor(SPACE_HSV, TRUE);
					imp->SetSliders(SPACE_HSV, FALSE);
				}
				break;
			case MODE_MIXER:
				if (glob.rangeType == 1) {
					imp->rSpin->SetValue((float)(GetRValue(wParam) / 255.0f), FALSE);
					imp->gSpin->SetValue((float)(GetGValue(wParam) / 255.0f), FALSE);
					imp->bSpin->SetValue((float)(GetBValue(wParam) / 255.0f), FALSE);

					imp->fSetCurrentColor(SPACE_RGB, TRUE);
					imp->fSetSliders(SPACE_RGB, FALSE);
				}
				else {
					imp->rSpin->SetValue((int)(GetRValue(wParam) / glob.rangeCoeff), FALSE);
					imp->gSpin->SetValue((int)(GetGValue(wParam) / glob.rangeCoeff), FALSE);
					imp->bSpin->SetValue((int)(GetBValue(wParam) / glob.rangeCoeff), FALSE);

					imp->SetCurrentColor(SPACE_RGB, TRUE);
					imp->SetSliders(SPACE_RGB, FALSE);
				}
				break;
			}
			if (glob.rangeType == 1) { imp->fSetRamps(); }
			else { imp->SetRamps(); }
		}
		break;

		case WM_MOVE:
		{
			imp->curPos.x = LOWORD(lParam);
			imp->curPos.y = HIWORD(lParam);
		}
		break;

		case WM_LBUTTONUP:
		{
			if (bPickingFromScreen)
			{
				ReleaseCapture();
				bPickingFromScreen = FALSE;
				SetCursor(oldCursor);
				SystemParametersInfo(SPI_SETCURSORS, 0, 0, WM_SETTINGCHANGE | SPIF_UPDATEINIFILE);
			}
		}
		break;

		case WM_MOUSEMOVE:
		{
			if (bPickingFromScreen)
			{
				POINT pnt;
				GetCursorPos(&pnt);
				HWND pickedHwnd = WindowFromPoint(pnt);

				RECT rect;
				GetWindowRect(pickedHwnd, &rect);
				pnt.x -= rect.left;
				pnt.y -= rect.top;

				HDC hdc = GetDC(pickedHwnd);
				COLORREF clr = GetPixel(hdc, pnt.x, pnt.y);
				ReleaseDC(pickedHwnd, hdc);

				int r = GetRValue(clr);
				int g = GetGValue(clr);
				int b = GetBValue(clr);

				if (glob.rangeType == 1)
				{
					float fr = r / 255.0f;
					float fg = g / 255.0f;
					float fb = b / 255.0f;

					imp->rSpin->SetValue(fr, TRUE);
					imp->gSpin->SetValue(fg, TRUE);
					imp->bSpin->SetValue(fb, TRUE);
				}
				else if (glob.rangeType != 1)
				{
					int ir = (int)(r / glob.rangeCoeff);
					int ig = (int)(g / glob.rangeCoeff);
					int ib = (int)(b / glob.rangeCoeff);

					imp->rSpin->SetValue(ir, TRUE);
					imp->gSpin->SetValue(ig, TRUE);
					imp->bSpin->SetValue(ib, TRUE);
				}
			}
		}
		break;

		case WM_CLOSE:
		{
			// RGB
			ReleaseISpinner(imp->rSpin);
			ReleaseISpinner(imp->gSpin);
			ReleaseISpinner(imp->bSpin);

			ReleaseICustEdit(imp->rEdit);
			ReleaseICustEdit(imp->gEdit);
			ReleaseICustEdit(imp->bEdit);

			ReleaseIColRamp(imp->rRamp);
			ReleaseIColRamp(imp->gRamp);
			ReleaseIColRamp(imp->bRamp);

			// HSV
			ReleaseISpinner(imp->hSpin);
			ReleaseISpinner(imp->sSpin);
			ReleaseISpinner(imp->vSpin);

			ReleaseICustEdit(imp->hEdit);
			ReleaseICustEdit(imp->sEdit);
			ReleaseICustEdit(imp->vEdit);

			ReleaseIColRamp(imp->hRamp);
			ReleaseIColRamp(imp->sRamp);
			ReleaseIColRamp(imp->vRamp);

			// CMYK
			ReleaseISpinner(imp->cSpin);
			ReleaseISpinner(imp->mSpin);
			ReleaseISpinner(imp->ySpin);
			ReleaseISpinner(imp->kSpin);

			ReleaseICustEdit(imp->cEdit);
			ReleaseICustEdit(imp->mEdit);
			ReleaseICustEdit(imp->yEdit);
			ReleaseICustEdit(imp->kEdit);

			ReleaseIColRamp(imp->cRamp);
			ReleaseIColRamp(imp->mRamp);
			ReleaseIColRamp(imp->yRamp);
			ReleaseIColRamp(imp->kRamp);

			// LIQ
			ReleaseISpinner(imp->lSpin);
			ReleaseISpinner(imp->iSpin);
			ReleaseISpinner(imp->qSpin);

			ReleaseICustEdit(imp->lEdit);
			ReleaseICustEdit(imp->iEdit);
			ReleaseICustEdit(imp->qEdit);

			ReleaseIColRamp(imp->lRamp);
			ReleaseIColRamp(imp->iRamp);
			ReleaseIColRamp(imp->qRamp);

			// Alpha
			ReleaseISpinner(imp->alphaSpin);
			ReleaseICustEdit(imp->alphaEdit);
			ReleaseIColRamp(imp->alphaRamp);

			// Other Stuff
			ReleaseIColRamp(imp->mixRamp);
			ReleaseIColSwatch(imp->currentSwatch);
			ReleaseIColSwatch(imp->originalSwatch);

			ReleaseICustButton(imp->modeSwitch);

			ReleaseIColMode(imp->bigPicker);

			imp->callback->BeingDestroyed(imp->curPos);

			imp->WriteSettings();

			BOOL retval = (LOWORD(wParam) ? TRUE : FALSE);
			EndDialog(hWnd, retval);
			return TRUE;
		}
		}
	}

	return FALSE;
}

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		LONG_PTR style = GetWindowLongPtr(hDlg, GWL_STYLE);
		// style &= ~WS_SYSMENU;
		SetWindowLongPtr(hDlg, GWL_STYLE, style);
		SetWindowPos(hDlg, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		return TRUE;
	}

	case WM_CLOSE:
	{
		EndDialog(hDlg, FALSE);
		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_ABOUTDLG_OK:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}

		case IDCANCEL:
		{
			EndDialog(hDlg, FALSE);
			return TRUE;
		}
		}
		break;
	}
	}
	return FALSE;
}