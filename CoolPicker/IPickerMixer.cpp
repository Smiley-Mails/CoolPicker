/*===================================================================
					MIXER CONTROL
	This is for the Painter color picker. This is
	the one	where the user paints with a brush and
	mixes up colors.
===================================================================*/
#include "IPickerMixer.h"
#include <maxversion.h>

// Макрос для кросс-версийного получения безопасного указателя на строку пути
#if MAX_RELEASE >= 27000
	// Для 3ds Max 2025, 2026 и 2027 (где GetDir возвращает объект-контейнер)
#define GET_DIR_PTR(dirId) TheManager->GetDir(dirId).data()
#else
	// Для 3ds Max 2024 (где GetDir возвращает сырой const wchar_t*)
#define GET_DIR_PTR(dirId) TheManager->GetDir(dirId)
#endif

// Constructor / Destructor
IPickerMixer::IPickerMixer()
{
	//Initialize Pointers
	plane = NULL;
	flowEdit = NULL;
	flowSpin = NULL;
}
IPickerMixer::IPickerMixer(HWND hwMixer)
{
	IPickerMixer();
	RECT clientArea;
	int i, j;
	double curweight;

	hWnd = hwMixer;

	GetClientRect(hWnd, &clientArea);

	height = clientArea.bottom;
	width = clientArea.right;

	picking = FALSE;

	plane = (float*)malloc(width * height * 3 * 4);

	if (plane == NULL) {
		MessageBox(NULL, _T("Critical shit has happened. Go buy more memory!"), _T("Cool Picker"), MB_OK | MB_ICONSTOP);
	}

	hdc = GetDC(hWnd);
	memdc = CreateCompatibleDC(hdc);
	membmp = CreateCompatibleBitmap(hdc, width, height);

	SelectObject(memdc, membmp);

	SetBack();

	// Let's initialise the weights calculation here:
	for (i = extmin; i < extmax; i++)
	{
		for (j = extmin; j < extmax; j++)
		{
			curweight = sqrt((float)(i * i + j * j));
			if (curweight >= (double)extmax) {
				curweight = (double)extmax;
			}

			curweight /= (double)extmax;

			weights[i + extmax][j + extmax] = fabs((1.0 - (curweight) * (curweight)) / (double)10.0);
			weightl[i + extmax][j + extmax] = fabs((1.0 - (curweight) * (curweight)) / (double)4.0);
		}
	}

	// Finally, grab the dump of the last session from disk

	GrabCopy();

	PostMessage(hWnd, CC_MIXER_INIT, (WPARAM)this, 0);
}

IPickerMixer::~IPickerMixer()
{
	if (plane != NULL) {
		free(plane);
	}
	plane = NULL;
	flowEdit = NULL;
	flowSpin = NULL;

	DeleteDC(memdc);
	ReleaseDC(hWnd, hdc);
	DeleteObject(membmp);

	DestroyWindow(hWnd);
	hWnd = NULL;
}

void IPickerMixer::SaveMixer()
{
	MCHAR filename[260];
	FILE* stream;

	// Адаптировано под требования SDK 2024-2027 через макрос
	_tcscpy_s(filename, 260, GET_DIR_PTR(APP_PLUGCFG_DIR));

	int len = (int)_tcslen(filename);
	if (len)
	{
		if (_tcscmp(&filename[len - 1], _T("\\")))
			_tcscat_s(filename, 260, _T("\\"));
	}
	_tcscat_s(filename, 260, _T("CoolPick.raw"));

	if (_tfopen_s(&stream, filename, _T("wb")) == 0)
	{
		fwrite(plane, width * height * 3 * 4, 1, stream);
		fclose(stream);
	}
}

void IPickerMixer::LoadMixer()
{
	MCHAR filename[260];
	FILE* stream;

	// Адаптировано под требования SDK 2024-2027 через макрос
	_tcscpy_s(filename, 260, GET_DIR_PTR(APP_PLUGCFG_DIR));

	int len = (int)_tcslen(filename);
	if (len)
	{
		if (_tcscmp(&filename[len - 1], _T("\\")))
			_tcscat_s(filename, 260, _T("\\"));
	}
	_tcscat_s(filename, 260, _T("CoolPick.raw"));

	if (_tfopen_s(&stream, filename, _T("rb")) == 0)
	{
		fread(plane, width * height * 3 * 4, 1, stream);
		fclose(stream);
	}
}

INT_PTR CALLBACK PickerMixerDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	IPickerMixer* picker;

	switch (msg)
	{
	case WM_COMMAND:
		picker = GetIPickerMixer(GetDlgItem(hWnd, IDC_PICKER_MIXER));
		assert(picker);
		switch (LOWORD(wParam))
		{
		case IDC_SETBACK:
			if (MessageBox(hWnd, _T("Are you sure you want to clear the Mixer area? Splashes will be lost..."), _T("Cool Picker"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				picker->SetBack();
				picker->Flipout();
			}
			break;
		default:
			break;
		}
		break;

	case WM_INITDIALOG:
		picker = GetIPickerMixer(GetDlgItem(hWnd, IDC_PICKER_MIXER));
		assert(picker);
		picker->flowSpin = SetupIntSpinner(hWnd, IDC_FLOW_SPIN, IDC_FLOW_EDIT, 1, 255, glob.flow);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

void IPickerMixer::DumpCopy()
{
	MCHAR filename[MAX_PATH] = { 0 };

	// Адаптировано под требования SDK 2024-2027 через макрос
	_tcscpy_s(filename, MAX_PATH, GET_DIR_PTR(APP_PLUGCFG_DIR));

	int len = (int)_tcslen(filename);
	if (len)
	{
		if (_tcscmp(&filename[len - 1], _T("\\")))
			_tcscat_s(filename, MAX_PATH, _T("\\"));
	}
	_tcscat_s(filename, MAX_PATH, _T("CoolPick.raw"));

	FILE* fRaw = NULL;
	errno_t result = _tfopen_s(&fRaw, filename, _T("w+b"));

	if (result != 0) {
		return;
	}

	fwrite(plane, 1, height * width * 3 * 4, fRaw);
	fclose(fRaw);
}

void IPickerMixer::GrabCopy()
{
	int curpos = 0;
	MCHAR filename[MAX_PATH] = { 0 };

	// Адаптировано под требования SDK 2024-2027 через макрос
	_tcscpy_s(filename, MAX_PATH, GET_DIR_PTR(APP_PLUGCFG_DIR));

	int len = (int)_tcslen(filename);
	if (len)
	{
		if (_tcscmp(&filename[len - 1], _T("\\")))
			_tcscat_s(filename, MAX_PATH, _T("\\"));
	}
	_tcscat_s(filename, MAX_PATH, _T("CoolPick.raw"));

	FILE* fRaw = NULL;
	errno_t result = _tfopen_s(&fRaw, filename, _T("rb"));

	if (result != 0)
		return;

	fread(plane, 1, height * width * 3 * 4, fRaw);
	fclose(fRaw);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			curpos = (x * height + y) * 3;
			SetPixelV(memdc, x, y, RGB((BYTE)plane[curpos], (BYTE)plane[curpos + 1], (BYTE)plane[curpos + 2]));
		}
	}

	Flipout();
}

void IPickerMixer::UpdatePickerMixer()
{
	PostMessage(hWnd, CC_MIXER_UPDATE, 0, 0);
}

void IPickerMixer::Flipout()
{
	BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY);
}

void IPickerMixer::Stroke(int x, int y, int btn)
{
	int i = 0;
	int j = 0;
	int curpos = 0;

	if (btn == 1)
	{
		for (i = extmin; i < extmax; i++)
		{
			for (j = extmin; j < extmax; j++)
			{
				if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
				{
					curpos = (x + i) * height * 3 + (y + j) * 3;

					if (fabs(plane[curpos] - oldr) < 1.0)
					{
						plane[curpos] = (float)oldr;
					}
					else
					{
						plane[curpos] = (float)(plane[curpos] * (1.0 - weights[i + extmax][j + extmax]) + oldr * weights[i + extmax][j + extmax]);
					}

					if (fabs(plane[curpos + 1] - oldg) < 1.0)
					{
						plane[curpos + 1] = (float)oldg;
					}
					else
					{
						plane[curpos + 1] = (float)(plane[curpos + 1] * (1.0 - weights[i + extmax][j + extmax]) + oldg * weights[i + extmax][j + extmax]);
					}

					if (fabs(plane[curpos + 2] - oldb) < 1.0)
					{
						plane[curpos + 2] = (float)oldb;
					}
					else
					{
						plane[curpos + 2] = (float)(plane[curpos + 2] * (1.0 - weights[i + extmax][j + extmax]) + oldb * weights[i + extmax][j + extmax]);
					}

					SetPixelV(memdc, x + i, y + j, RGB((BYTE)plane[curpos], (BYTE)plane[curpos + 1], (BYTE)plane[curpos + 2]));
				}
			}
		}
	}
	else
	{
		for (i = extmin; i < extmax; i++)
		{
			for (j = extmin; j < extmax; j++)
			{
				if (lx + i >= 0 && lx + i < width && ly + j >= 0 && ly + j < height)
				{
					curpos = (lx + i) * height * 3 + (ly + j) * 3;
					oldone[i + extmax][j + extmax][0] = plane[curpos];
					oldone[i + extmax][j + extmax][1] = plane[curpos + 1];
					oldone[i + extmax][j + extmax][2] = plane[curpos + 2];
				}
			}
		}

		for (i = extmin; i < extmax; i++)
		{
			for (j = extmin; j < extmax; j++)
			{
				if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
				{
					curpos = (x + i) * height * 3 + (y + j) * 3;
					plane[curpos] = (float)(plane[curpos] * (1.0 - weightl[i + extmax][j + extmax]) + oldone[i + extmax][j + extmax][0] * weightl[i + extmax][j + extmax]);
					plane[curpos + 1] = (float)(plane[curpos + 1] * (1.0 - weightl[i + extmax][j + extmax]) + oldone[i + extmax][j + extmax][1] * weightl[i + extmax][j + extmax]);
					plane[curpos + 2] = (float)(plane[curpos + 2] * (1.0 - weightl[i + extmax][j + extmax]) + oldone[i + extmax][j + extmax][2] * weightl[i + extmax][j + extmax]);
					SetPixelV(memdc, x + i, y + j, RGB((BYTE)plane[curpos], (BYTE)plane[curpos + 1], (BYTE)plane[curpos + 2]));
				}
			}
		}
	}

	Flipout();
}

void IPickerMixer::SetBack()
{
	int curpos = 0;

	int count = width * height * 3;
	for (int i = 0; i < count; i += 3)
	{
		plane[i] = (float)GetRValue(glob.mixBack);
		plane[i + 1] = (float)GetGValue(glob.mixBack);
		plane[i + 2] = (float)GetBValue(glob.mixBack);
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			curpos = (x * height + y) * 3; // Исправлен маппинг пикселей под плоскость
			SetPixelV(memdc, x, y, RGB((BYTE)plane[curpos], (BYTE)plane[curpos + 1], (BYTE)plane[curpos + 2]));
		}
	}

	Flipout();
}

// Setup Functions
void ReleaseIPickerMixer(IPickerMixer* pickerMixer)
{
	if (pickerMixer != NULL)
	{
		delete pickerMixer;
	}
}

IPickerMixer* GetIPickerMixer(HWND hwMixer)
{
	return new IPickerMixer(hwMixer);
}

LRESULT WINAPI PickerMixerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IPickerMixer* imp = NULL;
	int curpos = 0;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		SetTimer(hWnd, 1, glob.flow, NULL);
		imp->lx = imp->x = GET_X_LPARAM(lParam);
		imp->ly = imp->y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONDOWN:
		if (imp->picking)
		{
			imp->picking = FALSE;
			imp->oldr = (int)imp->saver;
			imp->oldg = (int)imp->saveg;
			imp->oldb = (int)imp->saveb;
			PostMessage(GetParent(hWnd), CC_MIXER_CHANGE, (WPARAM)hWnd, 0);
			break;
		}
		SetCapture(hWnd);
		SetTimer(hWnd, 2, 1, NULL);
		imp->lx = imp->x = GET_X_LPARAM(lParam);
		imp->ly = imp->y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONUP:
		KillTimer(hWnd, 2);
		ReleaseCapture();
		imp->DumpCopy();
		break;
	case WM_LBUTTONUP:
		KillTimer(hWnd, 1);
		ReleaseCapture();
		imp->DumpCopy();
		break;
	case WM_MBUTTONDOWN:
		imp->picking = TRUE;
		imp->saver = imp->oldr;
		imp->saveg = imp->oldg;
		imp->saveb = imp->oldb;

		imp->lx = imp->x = GET_X_LPARAM(lParam);
		imp->ly = imp->y = GET_Y_LPARAM(lParam);

		curpos = (imp->x * imp->height + imp->y) * 3;
		imp->oldr = (int)imp->plane[curpos];
		imp->oldg = (int)imp->plane[curpos + 1];
		imp->oldb = (int)imp->plane[curpos + 2];
		PostMessage(GetParent(hWnd), CC_MIXER_CHANGE, (WPARAM)hWnd, 0);
		break;
	case WM_MBUTTONUP:
		imp->picking = FALSE;
		break;
	case WM_MOUSEMOVE:
		imp->lx = imp->x;
		imp->ly = imp->y;

		imp->x = GET_X_LPARAM(lParam);
		imp->y = GET_Y_LPARAM(lParam);

		if (wParam & MK_MBUTTON && imp->picking)
		{
			curpos = (imp->x * imp->height + imp->y) * 3;
			imp->oldr = (int)imp->plane[curpos];
			imp->oldg = (int)imp->plane[curpos + 1];
			imp->oldb = (int)imp->plane[curpos + 2];
			PostMessage(GetParent(hWnd), CC_MIXER_CHANGE, (WPARAM)hWnd, 0);
		}
		break;
	case CC_MIXER_UPDATE:
		imp->Flipout();
		break;
	case WM_TIMER:
		imp->Stroke(imp->x, imp->y, (int)wParam);
		break;
	case WM_ERASEBKGND:
		PostMessage(GetParent(hWnd), CC_MIXER_REPAINT, (WPARAM)hWnd, 0);
		return -1;
	case CC_MIXER_INIT:
		imp = ((IPickerMixer*)wParam);
		imp->Flipout();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}

INT_PTR CALLBACK ColorMixerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IPickerMixer* picker = NULL;

	switch (msg)
	{
	case CC_MIXER_CHANGE:
		picker->UpdatePickerMixer();
		PostMessage(GetParent(hWnd), CC_BIG_CHANGE, (WPARAM)RGB(picker->oldr, picker->oldg, picker->oldb), 0);
		break;
	case CC_MIXER_REPAINT:
		PostMessage((HWND)wParam, CC_MIXER_UPDATE, 0, 0);
		break;
	case CC_BIG_COLORCHANGED:
		picker->oldr = GetRValue((COLORREF)wParam);
		picker->oldg = GetGValue((COLORREF)wParam);
		picker->oldb = GetBValue((COLORREF)wParam);
		picker->UpdatePickerMixer();
		break;
	case CC_SPINNER_CHANGE:
	case WM_CUSTEDIT_ENTER:
		glob.flow = 100 - picker->flowSpin->GetIVal();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SETBACK:
		{
			if (MessageBox(hWnd, _T("This will clear the mixing area to the currently selected color!\n\nAre you sure you want this?"), _T("Cool Picker"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				glob.mixBack = RGB(picker->oldr, picker->oldg, picker->oldb);
				picker->SetBack();
				picker->DumpCopy();
			}
			break;
		}
		case IDC_PRESETS:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				int lbLen, lbId;
				MCHAR* lbTxt;
				lbId = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				lbLen = (int)SendMessage((HWND)lParam, CB_GETLBTEXTLEN, lbId, 0);
				lbTxt = new MCHAR[lbLen + 1];
				SendMessage((HWND)lParam, CB_GETLBTEXT, lbId, (LPARAM)lbTxt);

				delete[] lbTxt;
			}
			break;
		case IDC_SAVE:
		{
			picker->SaveMixer();
			SendMessage(GetDlgItem(hWnd, IDC_PRESETS), CB_RESETCONTENT, 0, 0);

			MCHAR filename[400];
			// Адаптировано под требования SDK 2024-2027 через макрос
			_tcscpy_s(filename, 400, GET_DIR_PTR(APP_PLUGCFG_DIR));
			size_t len = _tcslen(filename);
			if (len)
			{
				if (_tcscmp(&filename[len - 1], _T("\\")))
					_tcscat_s(filename, 400, _T("\\"));
			}
			_tcscat_s(filename, 400, _T("CoolPick.raw"));
		}
		break;
		default:
			break;
		}
		break;
	case WM_INITDIALOG:
		picker = GetIPickerMixer(GetDlgItem(hWnd, IDC_PICKER_MIXER));
		assert(picker);
		picker->flowSpin = SetupIntSpinner(hWnd, IDC_FLOW_SPIN, IDC_FLOW_EDIT, 1, 100, 100 - glob.flow);
		picker->flowEdit = GetICustEdit(GetDlgItem(hWnd, IDC_FLOW_EDIT));
		picker->UpdatePickerMixer();

		PostMessage(GetDlgItem(hWnd, IDC_SETBACK), BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SETBACK)));
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		ReleaseISpinner(picker->flowSpin);
		ReleaseICustEdit(picker->flowEdit);
		ReleaseIPickerMixer(picker);
		EndDialog(hWnd, 0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}

