#include "CoolPickerDlg.h"
#include "CoolPickerProcedure.h"
#include "3dsmaxport.h"
#include <ColorManagement/IColorPipelineMgr.h>
#include <ColorManagement/IColorPipeline.h>
#include <ColorManagement/ColorConverter.h>

using namespace MaxSDK::ColorManagement;

#ifdef GetPrivateProfileIntW
#undef GetPrivateProfileIntW
#endif
#ifdef WritePrivateProfileStringW
#undef WritePrivateProfileStringW
#endif

// Constructor
#if MAX_RELEASE >= 29000
	// =========================================================================
	// 3ds Max 2027
	// =========================================================================
	CoolPickerDlg::CoolPickerDlg(HWND hOwner, const AColor& col, ColorPicker::Flags flags, IPoint2* pos,
		ColorPickerCallback* cb, const wchar_t* name, ColorContext context, int objClr, CoolPickerModelessDlg* smp)
	{
		hwOwner = hOwner;
		pickerFlags = flags;
		colorContext = context;

		// Определяем режим на основе флагов или контекста (например, если контекст не дефолтный — это плавающие цвета)
		glob.FloatingPointMode = (context != ColorContext::COLOR_SPACE_CONTEXT_NONE) ? TRUE : FALSE;

		//a_origRGB = a_currentRGB = col;
		//origRGB = currentRGB = AColor(col).toRGB();

		AColor displayCol = WorkingToDisplay(col);
		a_origRGB = a_currentRGB = displayCol;
		origRGB = currentRGB = displayCol.toRGB();

		rSpin = gSpin = bSpin = NULL;
		if (pos) curPos = initPos = *pos;
		callback = cb;
		colName = name;
		Modeless = FALSE;
		hwPanel = NULL;
		objColor = objClr ? TRUE : FALSE;
		sm = smp;
	}
#else
	// =========================================================================
	// 3ds Max 2026
	// =========================================================================
	CoolPickerDlg::CoolPickerDlg(HWND hOwner, const AColor& col, IPoint2* pos,
		HSVCallback* cb, const wchar_t* name, ColorContext context, int objClr, CoolPickerModelessDlg* smp)
	{
		hwOwner = hOwner;
		isObjectColor = objClr ? TRUE : FALSE; // Используем старое поле вместо флагов
		colorContext = context;
		glob.FloatingPointMode = (context != ColorContext::COLOR_SPACE_CONTEXT_NONE) ? TRUE : FALSE;

		AColor displayCol = WorkingToDisplay(col);
		a_origRGB = a_currentRGB = displayCol;
		origRGB = currentRGB = displayCol.toRGB();

		rSpin = gSpin = bSpin = NULL;
		if (pos) curPos = initPos = *pos;
		callback = cb;
		colName = name;
		Modeless = FALSE;
		hwPanel = NULL;
		objColor = objClr ? TRUE : FALSE;
		sm = smp;
	}
#endif

// Destructor
CoolPickerDlg::~CoolPickerDlg()
{
	if (hwPanel) hwPanel = NULL;
	if (callback) {
		callback->BeingDestroyed(curPos);
		callback = NULL;
	}
	sm = NULL;
}

// Изменено возвращаемое значение на INT_PTR
INT_PTR CoolPickerDlg::StartModal()
{
	Modeless = FALSE;
	INT_PTR result = DialogBoxParam(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		hwOwner,
		CoolPickerOptionsDlgProc,
		(LPARAM)this);
	return result;
}

HWND CoolPickerDlg::StartModeless()
{
	Modeless = TRUE;
	hwPanel = (HWND)CreateDialogParam(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		hwOwner,
		CoolPickerOptionsDlgProc,
		(LPARAM)this);
	return hwPanel;
}

void CoolPickerDlg::DoPaint(HWND hWnd)
{
	// TODO: Handle the painting here
}

void CoolPickerDlg::SetNewColor(DWORD color, const wchar_t* name)
{
	colName = name;
	origRGB = currentRGB = color;

	// Обязательно применяем WorkingToDisplay для стартового цвета
	AColor temp(
		GetRValue(color) / 255.0f,
		GetGValue(color) / 255.0f,
		GetBValue(color) / 255.0f,
		1.0f
	);
	a_origRGB = a_currentRGB = WorkingToDisplay(temp);

	SetSliders(SPACE_ALL, FALSE);
	SetRamps();

	if (wcslen(name) > 0)
	{
		colName += L" - Cool Picker";
		SetWindowText(hwPanel, colName.data());
	}
	else
	{
		SetWindowText(hwPanel, colName.data());
	}
	InvalidateRect(hwPanel, NULL, FALSE);
}

void CoolPickerDlg::SetNewColor(AColor color, const wchar_t* name)
{
	colName = name;

	AColor displayCol = WorkingToDisplay(color);
	a_origRGB = a_currentRGB = displayCol;

	fSetSliders(SPACE_ALL, FALSE);
	fSetRamps();

	if (wcslen(name) > 0)
	{
		colName += L" - Cool Picker";
		SetWindowText(hwPanel, colName.data());
	}
	else
	{
		SetWindowText(hwPanel, colName.data());
	}
	InvalidateRect(hwPanel, NULL, FALSE);
}

void CoolPickerDlg::LoadSettings()
{
	wchar_t filename[400] = { 0 };

	WStr dirPath = TheManager->GetDir(APP_PLUGCFG_DIR);
	wcscpy_s(filename, 400, dirPath.data());
	int len = (int)wcslen(filename);
	if (len)
	{
		if (wcscmp(&filename[len - 1], L"\\"))
			wcscat_s(filename, 400, L"\\");
	}
	wcscat_s(filename, 400, CONFIGNAME);

	// Используем явный вызов через модуль kernel32, чтобы обойти конфликты перегрузок Max SDK
	HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
	if (hKernel)
	{
		typedef UINT(WINAPI* PFN_GetPrivateProfileIntW)(LPCWSTR, LPCWSTR, INT, LPCWSTR);
		PFN_GetPrivateProfileIntW pGetParam = (PFN_GetPrivateProfileIntW)GetProcAddress(hKernel, "GetPrivateProfileIntW");
		if (pGetParam)
		{
			glob.bigMode = pGetParam(L"General", L"Mode", MODE_BIGPLANE, filename);
			glob.rangeMax = pGetParam(L"General", L"RangeMax", 255, filename);
			glob.rangeCoeff = 255.0f / (float)glob.rangeMax;
			glob.planeSubmode = pGetParam(L"Plane", L"Mode", MODE_HS, filename);
			glob.mixBack = pGetParam(L"Plane", L"MixBack", 0, filename);
			glob.flow = pGetParam(L"Plane", L"MixFlow", 30, filename);
			glob.mixMode = pGetParam(L"Plane", L"MixMode", 1, filename);
			return;
		}
	}

	// Резервный дефолт, если что-то пошло не так
	glob.bigMode = MODE_BIGPLANE;
	glob.rangeMax = 255;
	glob.rangeCoeff = 1.0f;
	glob.planeSubmode = MODE_HS;
	glob.mixBack = 0;
	glob.flow = 30;
	glob.mixMode = 1;
}

void CoolPickerDlg::WriteSettings()
{
	wchar_t filename[400] = { 0 };
	wchar_t value[64] = { 0 };

	WStr dirPath = TheManager->GetDir(APP_PLUGCFG_DIR);
	wcscpy_s(filename, 400, dirPath.data());
	int len = (int)wcslen(filename);
	if (len)
	{
		if (wcscmp(&filename[len - 1], L"\\"))
			wcscat_s(filename, 400, L"\\");
	}
	wcscat_s(filename, 400, CONFIGNAME);

	HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
	if (hKernel)
	{
		typedef BOOL(WINAPI* PFN_WritePrivateProfileStringW)(LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR);
		PFN_WritePrivateProfileStringW pWriteParam = (PFN_WritePrivateProfileStringW)GetProcAddress(hKernel, "WritePrivateProfileStringW");
		if (pWriteParam)
		{
			swprintf_s(value, 64, L"%d", glob.bigMode);
			pWriteParam(L"General", L"Mode", value, filename);

			swprintf_s(value, 64, L"%d", glob.rangeMax);
			pWriteParam(L"General", L"RangeMax", value, filename);

			swprintf_s(value, 64, L"%d", glob.planeSubmode);
			pWriteParam(L"Plane", L"Mode", value, filename);

			swprintf_s(value, 64, L"%d", glob.mixBack);
			pWriteParam(L"Plane", L"MixBack", value, filename);

			swprintf_s(value, 64, L"%d", glob.flow);
			pWriteParam(L"Plane", L"MixFlow", value, filename);

			swprintf_s(value, 64, L"%d", glob.mixMode);
			pWriteParam(L"Plane", L"MixMode", value, filename);
		}
	}
}

AColor CoolPickerDlg::WorkingToDisplay(const AColor& col)
{
	// Получаем инстанс менеджера
	auto mgr = IColorPipelineMgr::GetInstance();

	// Проверяем, включен ли Color Management (не равен ли он kUNMANAGED)
	if (mgr && mgr->GetColorPipelineMode() != ColorPipelineMode::kUNMANAGED) {

		// Получаем прямой пайплайн для UI колор-пикера (inverse = false)
		auto pipeline = mgr->GetDefaultViewingPipeline(DisplayViewTarget::kCOLOR_PICKER, false);

		if (pipeline) {
			// Запрашиваем типизированный конвертер для структуры AColor
			auto converter = pipeline->GetColorConverter<AColor, AColor>();
			if (converter) {
				// Конвертируем цвет из Rendering Space в Display Space
				return converter->ConvertColor(col);
			}
		}
	}
	return col;
}

AColor CoolPickerDlg::DisplayToWorking(const AColor& col)
{
	// Получаем инстанс менеджера
	auto mgr = IColorPipelineMgr::GetInstance();

	// Проверяем, включен ли Color Management
	if (mgr && mgr->GetColorPipelineMode() != ColorPipelineMode::kUNMANAGED) {

		// Получаем ОБРАТНЫЙ пайплайн для UI колор-пикера (inverse = true)
		auto pipeline = mgr->GetDefaultViewingPipeline(DisplayViewTarget::kCOLOR_PICKER, true);

		if (pipeline) {
			// Запрашиваем типизированный конвертер для структуры AColor
			auto converter = pipeline->GetColorConverter<AColor, AColor>();
			if (converter) {
				// Конвертируем цвет из интерфейса (Display) обратно в сцену (Working)
				return converter->ConvertColor(col);
			}
		}
	}
	return col;
}

void CoolPickerDlg::ModifyColor(DWORD color)
{
	currentRGB = color;

	// Конвертируем сырой цвет в AColor и пропускаем через пайплайн
	AColor temp(
		GetRValue(color) / 255.0f,
		GetGValue(color) / 255.0f,
		GetBValue(color) / 255.0f,
		a_currentRGB.a
	);

	a_currentRGB = WorkingToDisplay(temp);

	InvalidateRect(hwPanel, NULL, FALSE);
}

void CoolPickerDlg::ModifyColor(AColor color)
{
	// Конвертируем цвет от Max (Working) обратно в интерфейс (Display)
	a_currentRGB = WorkingToDisplay(color);
	InvalidateRect(hwPanel, NULL, FALSE);
}

DWORD CoolPickerDlg::GetColor()
{
	return currentRGB;
}

AColor CoolPickerDlg::GetAColor()
{
	return a_currentRGB;
}

IPoint2 CoolPickerDlg::GetPosition()
{
	return curPos;
}

IPoint2 CoolPickerDlg::initPos(354, 94);

#if MAX_RELEASE >= 29000
	// =========================================================================
	// 3ds Max 2027
	// =========================================================================
	void CoolPickerDlg::InstallNewCB(DWORD col, ColorPickerCallback* pcb, const wchar_t* name)
	{
		if (callback)
		{
			// Явно приводим DWORD к типу AColor
			callback->ColorChanged(AColor(currentRGB), 1);
		}
		callback = pcb;
		SetNewColor(col, name);
	}

	void CoolPickerDlg::InstallNewCB(AColor col, ColorPickerCallback* pcb, const wchar_t* name)
	{
		if (callback)
		{
			callback->ColorChanged(a_currentRGB, 1);
		}
		callback = pcb;
		SetNewColor(col, name);
	}
#else
	// =========================================================================
	// 3ds Max 2026
	// =========================================================================
	void CoolPickerDlg::InstallNewCB(DWORD col, HSVCallback* pcb, const wchar_t* name)
	{
		if (callback)
		{
			callback->ColorChanged(AColor(currentRGB), 1);
		}
		callback = pcb;
		SetNewColor(col, name);
	}

	void CoolPickerDlg::InstallNewCB(AColor col, HSVCallback* pcb, const wchar_t* name)
	{
		if (callback)
		{
			callback->ColorChanged(a_currentRGB, 1);
		}
		callback = pcb;
		SetNewColor(col, name);
	}
#endif

void CoolPickerDlg::SetCurrentColor(int cSpace, BOOL update)
{
	switch (cSpace)
	{
	case SPACE_RGB:
		currentRGB = RGB(rSpin->GetIVal() * glob.rangeCoeff, gSpin->GetIVal() * glob.rangeCoeff, bSpin->GetIVal() * glob.rangeCoeff);
		break;
	case SPACE_HSV:
		currentRGB = HSVtoRGB(hSpin->GetIVal() * glob.rangeCoeff, sSpin->GetIVal() * glob.rangeCoeff, vSpin->GetIVal() * glob.rangeCoeff);
		break;
	case SPACE_CMYK:
		currentRGB = CMYKtoRGB(cSpin->GetIVal() * glob.rangeCoeff, mSpin->GetIVal() * glob.rangeCoeff, ySpin->GetIVal() * glob.rangeCoeff, kSpin->GetIVal() * glob.rangeCoeff);
		break;
	case SPACE_LIQ:
		currentRGB = LIQtoRGB(lSpin->GetIVal() * glob.rangeCoeff, iSpin->GetIVal() * glob.rangeCoeff, qSpin->GetIVal() * glob.rangeCoeff);
		break;
	}

	if (update)
	{
		if (callback)
		{
			// callback->ColorChanged(AColor(currentRGB), 1);
			
			// Оборачиваем в DisplayToWorking
			callback->ColorChanged(DisplayToWorking(AColor(currentRGB)), 1);
		}
	}
}

void CoolPickerDlg::fSetCurrentColor(int cSpace, BOOL update)
{
	switch (cSpace)
	{
	case SPACE_RGB:
	{
		a_currentRGB = AColor(rSpin->GetFVal(), gSpin->GetFVal(), bSpin->GetFVal(), alphaSpin->GetFVal());
		break;
	}
	case SPACE_HSV:
	{
		float r, g, b;
		DWORD tempRGB = HSVtoRGB(hSpin->GetFVal() * 255, sSpin->GetFVal() * 255, vSpin->GetFVal() * 255);
		r = GetRValue(tempRGB) / 255.0f;
		g = GetGValue(tempRGB) / 255.0f;
		b = GetBValue(tempRGB) / 255.0f;
		// Удерживаем значение прозрачности
		a_currentRGB = AColor(r, g, b, alphaSpin->GetFVal());
		break;
	}
	case SPACE_CMYK:
	{
		a_currentRGB = CMYKtoRGBA(cSpin->GetFVal(), mSpin->GetFVal(), ySpin->GetFVal(), kSpin->GetFVal(), alphaSpin->GetFVal());
		break;
	}
	case SPACE_LIQ:
	{
		a_currentRGB = fLIQtoRGBA(lSpin->GetFVal(), iSpin->GetFVal(), qSpin->GetFVal(), alphaSpin->GetFVal());
		break;
	}
	}

	if (update)
	{
		if (callback)
		{
			// callback->ColorChanged(a_currentRGB, 1);

			// Оборачиваем в DisplayToWorking
			callback->ColorChanged(DisplayToWorking(a_currentRGB), 1);
		}
	}
}

void CoolPickerDlg::SetSliderRange()
{
	rSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	gSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	bSpin->SetLimits(0, (int)glob.rangeMax, FALSE);

	hSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	sSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	vSpin->SetLimits(0, (int)glob.rangeMax, FALSE);

	cSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	mSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	ySpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	kSpin->SetLimits(0, (int)glob.rangeMax, FALSE);

	lSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	iSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
	qSpin->SetLimits(0, (int)glob.rangeMax, FALSE);
}

void CoolPickerDlg::fSetSliderRange()
{
	rSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	gSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	bSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);

	hSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	sSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	vSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);

	cSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	mSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	ySpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	kSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);

	lSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	iSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);
	qSpin->SetLimits(0.0f, (float)glob.rangeMax, FALSE);

	alphaSpin->SetLimits(0.0f, (float)glob.rangeType, FALSE);
}


void CoolPickerDlg::SetSliders(int cSpace, BOOL update)
{
	if (cSpace != SPACE_RGB)
	{
		int r, g, b;

		r = GetRValue(currentRGB) / glob.rangeCoeff;
		g = GetGValue(currentRGB) / glob.rangeCoeff;
		b = GetBValue(currentRGB) / glob.rangeCoeff;

		rSpin->SetValue(r, update);
		gSpin->SetValue(g, update);
		bSpin->SetValue(b, update);

		rEdit->SetText(r);
		gEdit->SetText(g);
		bEdit->SetText(b);
	}

	if (cSpace != SPACE_HSV)
	{
		int h, s, v;
		RGBtoHSV(currentRGB, &h, &s, &v);
		hSpin->SetValue((int)(h / glob.rangeCoeff), update);
		sSpin->SetValue((int)(s / glob.rangeCoeff), update);
		vSpin->SetValue((int)(v / glob.rangeCoeff), update);

		hEdit->SetText((int)(h / glob.rangeCoeff));
		sEdit->SetText((int)(s / glob.rangeCoeff));
		vEdit->SetText((int)(v / glob.rangeCoeff));
	}

	if (cSpace != SPACE_CMYK)
	{
		int c, m, y, k;
		RGBtoCMYK(currentRGB, &c, &m, &y, &k);

		cSpin->SetValue((int)(c / glob.rangeCoeff), update);
		mSpin->SetValue((int)(m / glob.rangeCoeff), update);
		ySpin->SetValue((int)(y / glob.rangeCoeff), update);
		kSpin->SetValue((int)(k / glob.rangeCoeff), update);

		cEdit->SetText((int)(c / glob.rangeCoeff));
		mEdit->SetText((int)(m / glob.rangeCoeff));
		yEdit->SetText((int)(y / glob.rangeCoeff));
		kEdit->SetText((int)(k / glob.rangeCoeff));
	}

	if (cSpace != SPACE_LIQ)
	{
		int l, i, q;
		RGBtoLIQ(currentRGB, &l, &i, &q);

		lSpin->SetValue((int)(l / glob.rangeCoeff), update);
		iSpin->SetValue((int)(i / glob.rangeCoeff), update);
		qSpin->SetValue((int)(q / glob.rangeCoeff), update);

		lEdit->SetText((int)(l / glob.rangeCoeff));
		iEdit->SetText((int)(i / glob.rangeCoeff));
		qEdit->SetText((int)(q / glob.rangeCoeff));
	}
}


void CoolPickerDlg::fSetSliders(int cSpace, BOOL update)
{
	float a = a_currentRGB.a;
	alphaSpin->SetValue(a, update);
	alphaEdit->SetText(a, 3);

	if (cSpace != SPACE_RGB)
	{
		float r, g, b;

		r = a_currentRGB.r;
		g = a_currentRGB.g;
		b = a_currentRGB.b;

		rSpin->SetValue(r, update);
		gSpin->SetValue(g, update);
		bSpin->SetValue(b, update);

		rEdit->SetText(r, 3);
		gEdit->SetText(g, 3);
		bEdit->SetText(b, 3);
	}

	if (cSpace != SPACE_HSV)
	{
		int hh, ss, vv;
		DWORD tempRGB = RGB(a_currentRGB.r * 255.0f, a_currentRGB.g * 255.0f, a_currentRGB.b * 255.0f);
		RGBtoHSV(tempRGB, &hh, &ss, &vv);
		float h, s, v;
		h = hh / 255.0f;
		s = ss / 255.0f;
		v = vv / 255.0f;

		hSpin->SetValue(h, update);
		sSpin->SetValue(s, update);
		vSpin->SetValue(v, update);

		hEdit->SetText(h, 3);
		sEdit->SetText(s, 3);
		vEdit->SetText(v, 3);
	}

	if (cSpace != SPACE_CMYK)
	{
		float c, m, y, k;
		RGBAtoCMYK(a_currentRGB, &c, &m, &y, &k);

		cSpin->SetValue(c, update);
		mSpin->SetValue(m, update);
		ySpin->SetValue(y, update);
		kSpin->SetValue(k, update);

		cEdit->SetText(c, 3);
		mEdit->SetText(m, 3);
		yEdit->SetText(y, 3);
		kEdit->SetText(k, 3);
	}

	if (cSpace != SPACE_LIQ)
	{
		float l, i, q;

		RGBAtoLIQ(a_currentRGB, &l, &i, &q);

		lSpin->SetValue(l, update);
		iSpin->SetValue(i, update);
		qSpin->SetValue(q, update);

		lEdit->SetText(l, 3);
		iEdit->SetText(i, 3);
		qEdit->SetText(q, 3);
	}
}

void CoolPickerDlg::SetRamps()
{
	int r, g, b;
	int h, s, v;
	int c, m, y, k;
	int l, i, q;

	r = rSpin->GetIVal() * glob.rangeCoeff;
	g = gSpin->GetIVal() * glob.rangeCoeff;
	b = bSpin->GetIVal() * glob.rangeCoeff;

	h = hSpin->GetIVal() * glob.rangeCoeff;
	s = sSpin->GetIVal() * glob.rangeCoeff;
	v = vSpin->GetIVal() * glob.rangeCoeff;

	c = cSpin->GetIVal() * glob.rangeCoeff;
	m = mSpin->GetIVal() * glob.rangeCoeff;
	y = ySpin->GetIVal() * glob.rangeCoeff;
	k = kSpin->GetIVal() * glob.rangeCoeff;

	l = lSpin->GetIVal() * glob.rangeCoeff;
	i = iSpin->GetIVal() * glob.rangeCoeff;
	q = qSpin->GetIVal() * glob.rangeCoeff;

	rRamp->SetColors(RGB(0, g, b), RGB(255, g, b), RGB(128, g, b));
	rRamp->SetPos(r);
	rRamp->UpdateRamp();

	gRamp->SetColors(RGB(r, 0, b), RGB(r, 255, b), RGB(r, 128, b));
	gRamp->SetPos(g);
	gRamp->UpdateRamp();

	bRamp->SetColors(RGB(r, g, 0), RGB(r, g, 255), RGB(r, g, 128));
	bRamp->SetPos(b);
	bRamp->UpdateRamp();

	hRamp->SetColors(HSVtoRGB(0, s, v), HSVtoRGB(255, s, v), HSVtoRGB(128, s, v));
	hRamp->SetPos(h);
	hRamp->UpdateRamp();

	sRamp->SetColors(HSVtoRGB(h, 0, v), HSVtoRGB(h, 255, v), HSVtoRGB(h, 128, v));
	sRamp->SetPos(s);
	sRamp->UpdateRamp();

	vRamp->SetColors(HSVtoRGB(h, s, 0), HSVtoRGB(h, s, 255), HSVtoRGB(h, s, 128));
	vRamp->SetPos(v);
	vRamp->UpdateRamp();

	cRamp->SetColors(CMYKtoRGB(0, m, y, k), CMYKtoRGB(255, m, y, k), CMYKtoRGB(128, m, y, k));
	cRamp->SetPos(c);
	cRamp->UpdateRamp();

	mRamp->SetColors(CMYKtoRGB(c, 0, y, k), CMYKtoRGB(c, 255, y, k), CMYKtoRGB(c, 128, y, k));
	mRamp->SetPos(m);
	mRamp->UpdateRamp();

	yRamp->SetColors(CMYKtoRGB(c, m, 0, k), CMYKtoRGB(c, m, 255, k), CMYKtoRGB(c, m, 128, k));
	yRamp->SetPos(y);
	yRamp->UpdateRamp();

	kRamp->SetColors(CMYKtoRGB(c, m, y, 0), CMYKtoRGB(c, m, y, 255), CMYKtoRGB(c, m, y, 128));
	kRamp->SetPos(k);
	kRamp->UpdateRamp();

	lRamp->SetColors(LIQtoRGB(0, i, q), LIQtoRGB(255, i, q), LIQtoRGB(128, i, q));
	lRamp->SetPos(l);
	lRamp->UpdateRamp();

	iRamp->SetColors(LIQtoRGB(l, 0, q), LIQtoRGB(l, 255, q), LIQtoRGB(l, 128, q));
	iRamp->SetPos(i);
	iRamp->UpdateRamp();

	qRamp->SetColors(LIQtoRGB(l, i, 0), LIQtoRGB(l, i, 255), LIQtoRGB(l, i, 128));
	qRamp->SetPos(q);
	qRamp->UpdateRamp();

	if (mixRamp->pPos == 255)
	{
		mixRamp->SetColors(origRGB, currentRGB, RGB((GetRValue(origRGB) + r) / 2, (GetGValue(origRGB) + g) / 2, (GetBValue(origRGB) + b) / 2));
	}
	mixRamp->UpdateRamp();

	switch (bigPicker->mode)
	{
	case MODE_BIGPLANE:
		switch (bigPicker->submode)
		{
		case MODE_HS:
		case MODE_VS:
		case MODE_HV:
			bigPicker->SetColor(RGB(hSpin->GetIVal() * glob.rangeCoeff, sSpin->GetIVal() * glob.rangeCoeff, vSpin->GetIVal() * glob.rangeCoeff));
			break;
		case MODE_RG:
		case MODE_GB:
		case MODE_RB:
			bigPicker->SetColor(RGB(rSpin->GetIVal() * glob.rangeCoeff, gSpin->GetIVal() * glob.rangeCoeff, bSpin->GetIVal() * glob.rangeCoeff));
			break;
		case MODE_CM:
		case MODE_MY:
		case MODE_CY:
			bigPicker->SetColor(RGB(cSpin->GetIVal() * glob.rangeCoeff, mSpin->GetIVal() * glob.rangeCoeff, ySpin->GetIVal() * glob.rangeCoeff));
			break;
		case MODE_LI:
		case MODE_IQ:
		case MODE_LQ:
			bigPicker->SetColor(RGB(lSpin->GetIVal() * glob.rangeCoeff, iSpin->GetIVal() * glob.rangeCoeff, qSpin->GetIVal() * glob.rangeCoeff));
			break;
		}
		break;
	case MODE_TRIANGLE:
		bigPicker->SetColor(RGB(hSpin->GetIVal() * glob.rangeCoeff, sSpin->GetIVal() * glob.rangeCoeff, vSpin->GetIVal() * glob.rangeCoeff));
		break;
	case MODE_MIXER:
		bigPicker->SetColor(RGB(rSpin->GetIVal() * glob.rangeCoeff, gSpin->GetIVal() * glob.rangeCoeff, bSpin->GetIVal() * glob.rangeCoeff));
		break;
	}
	currentSwatch->SetColor(currentRGB);
	currentSwatch->UpdateSwatch();
	originalSwatch->SetColor(origRGB);
	originalSwatch->UpdateSwatch();
}

void CoolPickerDlg::fSetRamps()
{
	float r, g, b;
	float h, s, v;
	float c, m, y, k;
	float l, i, q;
	float alpha;
	r = CP_LIMITO_1(rSpin->GetFVal());
	g = CP_LIMITO_1(gSpin->GetFVal());
	b = CP_LIMITO_1(bSpin->GetFVal());

	h = CP_LIMITO_1(hSpin->GetFVal());
	s = CP_LIMITO_1(sSpin->GetFVal());
	v = CP_LIMITO_1(vSpin->GetFVal());

	c = CP_LIMITO_1(cSpin->GetFVal());
	m = CP_LIMITO_1(mSpin->GetFVal());
	y = CP_LIMITO_1(ySpin->GetFVal());
	k = CP_LIMITO_1(kSpin->GetFVal());

	l = CP_LIMITO_1(lSpin->GetFVal());
	i = CP_LIMITO_1(iSpin->GetFVal());
	q = CP_LIMITO_1(qSpin->GetFVal());

	alpha = CP_LIMITO_1(alphaSpin->GetFVal());

	rRamp->SetColors(AColor(0.0f, g, b, 0.0f), AColor(1.0f, g, b, 0.0f), AColor(0.50f, g, b, 0.0f));
	rRamp->SetPos(r);
	rRamp->UpdateRamp();

	gRamp->SetColors(AColor(r, 0.0f, b), AColor(r, 1.0f, b), AColor(r, 0.50f, b));
	gRamp->SetPos(g);
	gRamp->UpdateRamp();

	bRamp->SetColors(AColor(r, g, 0.0f), AColor(r, g, 1.0f), AColor(r, g, 0.5f));
	bRamp->SetPos(b);
	bRamp->UpdateRamp();

	hRamp->SetColors(fHSVtoRGBA(0, s, v, alpha), fHSVtoRGBA(1.0, s, v, alpha), fHSVtoRGBA(0.50, s, v, alpha));
	hRamp->SetPos(h);
	hRamp->UpdateRamp();

	sRamp->SetColors(fHSVtoRGBA(h, 0.0, v, alpha), fHSVtoRGBA(h, 1.0, v, alpha), fHSVtoRGBA(h, 0.50, v, alpha));
	sRamp->SetPos(s);
	sRamp->UpdateRamp();

	vRamp->SetColors(fHSVtoRGBA(h, s, 0.0, alpha), fHSVtoRGBA(h, s, 1.0, alpha), fHSVtoRGBA(h, s, 0.50, alpha));
	vRamp->SetPos(v);
	vRamp->UpdateRamp();

	cRamp->SetColors(CMYKtoRGBA(0, m, y, k, alpha), CMYKtoRGBA(1.0, m, y, k, alpha), CMYKtoRGBA(0.5, m, y, k, alpha));
	cRamp->SetPos(c);
	cRamp->UpdateRamp();

	mRamp->SetColors(CMYKtoRGBA(c, 0, y, k, alpha), CMYKtoRGBA(c, 1.0, y, k, alpha), CMYKtoRGBA(c, 0.5, y, k, alpha));
	mRamp->SetPos(m);
	mRamp->UpdateRamp();

	yRamp->SetColors(CMYKtoRGBA(c, m, 0, k, alpha), CMYKtoRGBA(c, m, 1.0, k, alpha), CMYKtoRGBA(c, m, 0.5, k, alpha));
	yRamp->SetPos(y);
	yRamp->UpdateRamp();

	kRamp->SetColors(CMYKtoRGBA(c, m, y, 0, alpha), CMYKtoRGBA(c, m, y, 1.0, alpha), CMYKtoRGBA(c, m, y, 0.5, alpha));
	kRamp->SetPos(k);
	kRamp->UpdateRamp();

	lRamp->SetColors(fLIQtoRGBA(0, i, q, alpha), fLIQtoRGBA(1.0, i, q, alpha), fLIQtoRGBA(0.5, i, q, alpha));
	lRamp->SetPos(l);
	lRamp->UpdateRamp();

	iRamp->SetColors(fLIQtoRGBA(l, 0, q, alpha), fLIQtoRGBA(l, 1.0, q, alpha), fLIQtoRGBA(l, 0.5, q, alpha));
	iRamp->SetPos(i);
	iRamp->UpdateRamp();

	qRamp->SetColors(fLIQtoRGBA(l, i, 0, alpha), fLIQtoRGBA(l, i, 1.0, alpha), fLIQtoRGBA(l, i, 0.5, alpha));
	qRamp->SetPos(q);
	qRamp->UpdateRamp();

	alphaRamp->SetColors(AColor(0.0f, 0.0f, 0.0f, 0.0f), AColor(1.0f, 1.0f, 1.0f, 1.0f), AColor(0.5f, 0.5f, 0.5f, 0.5f));
	alphaRamp->SetPos(alpha);
	alphaRamp->UpdateRamp();

	if (mixRamp->f_Pos >= 0.98f)
	{
		mixRamp->SetColors(a_origRGB, a_currentRGB, AColor((a_origRGB.r + r) / 2, (a_origRGB.g + g) / 2, (a_origRGB.b + b) / 2));
	}
	mixRamp->UpdateRamp();

	switch (bigPicker->mode)
	{
	case MODE_BIGPLANE:
		switch (bigPicker->submode)
		{
		case MODE_HS:
		case MODE_VS:
		case MODE_HV:
			bigPicker->SetColor(AColor(h, s, v, alpha));
			break;
		case MODE_RG:
		case MODE_GB:
		case MODE_RB:
			bigPicker->SetColor(AColor(r, g, b, alpha));
			break;
		case MODE_CM:
		case MODE_MY:
		case MODE_CY:
			bigPicker->SetColor(AColor(c, m, y, alpha));
			break;
		case MODE_LI:
		case MODE_IQ:
		case MODE_LQ:
			bigPicker->SetColor(AColor(l, i, q, alpha));
			break;
		}
		break;
	case MODE_TRIANGLE:
		bigPicker->SetColor(AColor(h, s, v, alpha));
		break;
	case MODE_MIXER:
		bigPicker->SetColor(AColor(r, g, b, alpha));
		break;
	}

	currentSwatch->SetColor(a_currentRGB.toRGB());
	currentSwatch->UpdateSwatch();
	originalSwatch->SetColor(a_origRGB.toRGB());
	originalSwatch->UpdateSwatch();
}

BOOL CoolPickerDlg::InitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HIMAGELIST& himl, FlyOffData* fod)
{
	LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
	// style &= ~WS_SYSMENU; // Убираем системное меню
	// style &= ~(WS_SYSMENU | WS_CAPTION); // Убираем системное меню и саму шапку
	SetWindowLongPtr(hWnd, GWL_STYLE, style);
	SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	DLSetWindowLongPtr(hWnd, this);
	// CenterWindow(hWnd, GetParent(hWnd));

	/*
	// Находим главное окно 3ds Max по его системному классу
	HWND hMaxWnd = ::FindWindowW(L"3dsmax", NULL);

	// Если нашли — центрируем по нему, если нет — используем старый резервный вариант
	CenterWindow(hWnd, hMaxWnd ? hMaxWnd : GetParent(hWnd));
	*/

	// 1. Безопасно получаем указатель на интерфейс
	Interface* coreInt = GetCOREInterface();
	// 2. Берем HWND только если интерфейс существует, иначе пишем NULL
	HWND hMaxWnd = coreInt ? coreInt->GetMAXHWnd() : NULL;
	// 3. Логика с бэкапом на родительское окно
	CenterWindow(hWnd, hMaxWnd ? hMaxWnd : GetParent(hWnd));

	LoadSettings();

	if (glob.FloatingPointMode)
	{
		PostMessage(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), BN_DISABLE, 0, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_RAD_R255), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_RAD_R100), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_ALPHATEXT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_ALPHA_GROUP), TRUE);
		glob.rangeType = 1;
		glob.rangeMax = 1;
		glob.rangeCoeff = 2.550f;
	}
	else 
	{
		EnableWindow(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_ALPHATEXT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_ALPHA_GROUP), TRUE);

		// LoadSettings() чуть выше уже прочитал glob.rangeMax из файла.
		// Связываем загруженное значение с текущим типом интерфейса.
		if (glob.rangeMax == 100 || glob.rangeMax == 1 || glob.rangeMax == 255)
		{
			glob.rangeType = glob.rangeMax;
		}
		else
		{
			// Резервный вариант, если в конфиге оказался мусор
			glob.rangeType = 255;
			glob.rangeMax = 255;
		}

		// Коэффициенты (glob.rangeCoeff) здесь больше писать не нужно, 
		// нижестоящий switch(glob.rangeType) всё сделает за нас.
		//glob.rangeType = 255;
		//glob.rangeMax = 255;
		//glob.rangeCoeff = 1.0f;
	}

	switch (glob.rangeType)
	{
	case 100:
	{
		glob.rangeCoeff = 2.55f;
		SetUpIntControls(hWnd, this, himl, fod);
		PostMessage(GetDlgItem(hWnd, IDC_RAD_R100), BM_SETCHECK, BST_CHECKED, 0);
		SetSliderRange();
		mixRamp->SetPos(100);
		SetSliders(SPACE_ALL, FALSE);
		bigPicker->SetMode(glob.bigMode);
		SetRamps();
		break;
	}
	case 255:
	{
		glob.rangeCoeff = 1.0f;
		SetUpIntControls(hWnd, this, himl, fod);
		PostMessage(GetDlgItem(hWnd, IDC_RAD_R255), BM_SETCHECK, BST_CHECKED, 0);
		SetSliderRange();
		mixRamp->SetPos(255);
		SetSliders(SPACE_ALL, FALSE);
		bigPicker->SetMode(glob.bigMode);
		SetRamps();
		break;
	}
	case 1:
	{
		glob.rangeCoeff = 255.0f;
		SetUpFltControls(hWnd, this, himl, fod);
		PostMessage(GetDlgItem(hWnd, IDC_RAD_R1FLOAT), BM_SETCHECK, BST_CHECKED, 0);
		fSetSliderRange();
		mixRamp->SetPos(1.0f);
		fSetSliders(SPACE_ALL, FALSE);
		bigPicker->SetMode(glob.bigMode);
		fSetRamps();
		break;
	}
	}
	if (objColor)
	{
		SetWindowText(GetDlgItem(hWnd, IDOK), L"Add Color");
		SetWindowText(GetDlgItem(hWnd, IDCANCEL), L"Close");
	}
	else
	{
		SetWindowText(GetDlgItem(hWnd, IDOK), L"OK");
		SetWindowText(GetDlgItem(hWnd, IDCANCEL), L"Cancel");
	}

	// Устанавливаем заголовок окна
	// "Шапку" - убрал, заголовок теперь не нужен.
	// SetWindowText(hWnd, colName);

	return TRUE;
}
