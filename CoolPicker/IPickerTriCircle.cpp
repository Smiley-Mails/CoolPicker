/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
		TRIANGLE-CIRCLE PICKER						
	This is the large Triangle and Circle			
	color picker.									
////////////////////////////////////////////////////*/
#include "IPickerTriCircle.h"

//Constructor
IPickerTriCircle::IPickerTriCircle(HWND hwPainter) 
{ 
	oldh = 0;
	olds = 0;
	oldv = 0;
	
	hSpin = NULL;
	hEdit = NULL;

	hWnd = hwPainter; 

	hDC = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	cent.x = rect.right / 2;
	cent.y = rect.bottom / 2;
	radius = (rect.right < rect.bottom)?rect.right/2:rect.bottom/2;

	ledge = cent.x - (int)((float)(radius - ringCross) * 0.5);
	redge = cent.x + radius - ringCross;
	tedge = cent.y - (int)((float)(radius - ringCross) * 0.86602);
	bedge = cent.y + (int)((float)(radius - ringCross) * 0.86602);

	memDC = CreateCompatibleDC(hDC);
	memBM = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);

	SelectObject(memDC, memBM);

	gTris[0].Vertex1 = 0;
	gTris[0].Vertex2 = 1;
	gTris[0].Vertex3 = 2;

	gTris[1].Vertex1 = 2;
	gTris[1].Vertex2 = 1;
	gTris[1].Vertex3 = 3;

	FillRect(memDC, &rect, GetSysColorBrush(COLOR_BTNFACE));

	RepaintTri();
	RepaintWheel();

	PostMessage(hWnd, CC_PAINTER_INIT, (WPARAM)this, 0); 
}


//Destructor
IPickerTriCircle::~IPickerTriCircle() 
{ 
	DeleteDC(memDC);
	ReleaseDC(hWnd, hDC);
	DeleteObject(memBM);

	hSpin = NULL;
	hEdit = NULL;

	DestroyWindow(hWnd); 
	hWnd = NULL; 
}

void IPickerTriCircle::UpdatePickerPainter()
{
	PostMessage(hWnd, CC_PAINTER_UPDATE, 0, 0);
}

void IPickerTriCircle::RepaintWheel()
{
	int x = 0, y = 0;
	int dx = 0, dy = 0;
	int hue = 0;

	int rectBottom = rect.bottom;
	for (y = 0; y < rectBottom; y++)
	{
		int rectRight = rect.right;
		for (x = 0; x < rectRight; x++)
		{
			dx = x - cent.x;
			dy = y - cent.y;
			
			BOOL condA = dx*dx + dy*dy < radius*radius;
			BOOL condB = dx*dx + dy*dy > (radius - ringCross)*(radius - ringCross);
			if (condA && condB)
			{
				hue = atan((float)dy/(float)dx) * 180 / 3.14159;
				hue = hue * 255 / 360;

				if (dx > 0 && dy > 0)
					SetPixel(memDC, x, y, HSVtoRGB(hue, 255, 255));
				else if (dx > 0 && dy < 0)
					SetPixel(memDC, x, y, HSVtoRGB(hue + 255, 255, 255));
				else if (dx < 0 && dy < 0)
					SetPixel(memDC, x, y, HSVtoRGB(hue + 128, 255, 255));
				else if (dx < 0 && dy > 0)
					SetPixel(memDC, x, y, HSVtoRGB(hue + 128, 255, 255));
				else if (dx == 0 && dy < 0)
					SetPixel(memDC, x, y, HSVtoRGB(192, 255, 255));
				else if (dx == 0 && dy > 0)
					SetPixel(memDC, x, y, HSVtoRGB(64, 255, 255));
				else if (dx < 0 && dy == 0)
					SetPixel(memDC, x, y, HSVtoRGB(128, 255, 255));
				else if (dx > 0 && dy == 0)
					SetPixel(memDC, x, y, HSVtoRGB(0, 255, 255));
			}
		}
	}
}

void IPickerTriCircle::RepaintTri()
{
	trivert[0].Alpha = 0x00;
	trivert[0].Red = 0xff00;
	trivert[0].Green = 0xff00;
	trivert[0].Blue = 0xff00;
	trivert[0].x = ledge;
	trivert[0].y = tedge;

	trivert[1].Alpha = 0x00;
	// Получаем исходный цвет из HSV
	DWORD rgbColor = HSVtoRGB(oldh, 255, 255);
	// Безопасно раскладываем его на каналы для GRADIENT_FILL_TRIANGLE (вручную сдвигаем байты в 16-битный формат WORD)
	trivert[1].Red = (WORD)GetRValue(rgbColor) << 8;
	trivert[1].Green = (WORD)GetGValue(rgbColor) << 8;
	trivert[1].Blue = (WORD)GetBValue(rgbColor) << 8;
	trivert[1].x = redge;
	trivert[1].y = rect.bottom / 2;

	// Убираем сломанный старый метод gammaCorrect((UBYTE)0x80) и ставим честный средний серый цвет (0x80 -> 0x8000)
	trivert[2].Alpha = 0x00;
	trivert[2].Red = 0x8000;
	trivert[2].Green = 0x8000;
	trivert[2].Blue = 0x8000;
	trivert[2].x = ledge;
	trivert[2].y = rect.bottom / 2;

	trivert[3].Alpha = 0x00;
	trivert[3].Red = 0x0000;
	trivert[3].Green = 0x0000;
	trivert[3].Blue = 0x0000;
	trivert[3].x = ledge;
	trivert[3].y = bedge;

	GradientFill(memDC, trivert, 4, gTris, 2, GRADIENT_FILL_TRIANGLE);
}

void IPickerTriCircle::PutTriPicker(int s, int v)
{
	int dx = redge - ledge;
	int dy = bedge - tedge;
	float vn = (float)v/255.0;
	float sn = (float)s/255.0;

	int x = ledge + (dx * vn) * (sn);
	int y = bedge - ((dy * vn) * (1.0 - sn) + ((dy/2) * vn) * (sn));
	DrawPickerSprite(hDC, x, y);
}

void IPickerTriCircle::PutWheelPicker(int h)
{
	int x = cent.x + cos(h * 360 / 255 * 3.14159 / 180) * (float)(radius - ringCross / 2);
	int y = cent.y + sin(h * 360 / 255 * 3.14159 / 180) * (float)(radius - ringCross / 2);

	DrawPickerSprite(hDC, x, y);
}


// SetUp Functions
void ReleaseIPickerPainter(IPickerTriCircle *pickerPainter)
{
	delete pickerPainter;
	pickerPainter = NULL;
}
IPickerTriCircle *GetIPickerPainter(HWND hwPainter)
{
	return new IPickerTriCircle(hwPainter);
}
LRESULT WINAPI PickerTriCircleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IPickerTriCircle* imp = NULL;
	static BOOL capTri = FALSE;
	static BOOL capWheel = FALSE;

	switch (msg)
	{
	case CC_PAINTER_UPDATE:
		{
		BitBlt(imp->hDC, 0, 0, imp->rect.right, imp->rect.bottom, imp->memDC, 0, 0, SRCCOPY);

		imp->PutTriPicker(imp->olds, imp->oldv);
		imp->PutWheelPicker(imp->oldh);

		break;
		}
	case WM_LBUTTONUP:
		{
		ReleaseCapture();
		capTri = capWheel = FALSE;
		break;
		}
	case WM_LBUTTONDOWN: //For the Triangle Circle
		if (wParam == MK_LBUTTON)
		{
			int posx, posy;

			posx = GET_X_LPARAM(lParam) - imp->cent.x;
			posy = GET_Y_LPARAM(lParam) - imp->cent.y;

			if (posx*posx + posy*posy > (imp->radius - ringCross)*(imp->radius - ringCross) && posx*posx + posy*posy < imp->radius * imp->radius) 
			{
				SetCapture(hWnd);
				capWheel = TRUE;
			}		
			else if (posx*posx + posy*posy < (imp->radius - ringCross)*(imp->radius - ringCross))
			{
				SetCapture(hWnd);
				capTri = TRUE;          
			}
		}
	case WM_MOUSEMOVE: //For the Wheel picker
		if (wParam == MK_LBUTTON && capWheel)
		{
			int posx, posy;
			float hue;

			posx = GET_X_LPARAM(lParam) - imp->cent.x;
			posy = GET_Y_LPARAM(lParam) - imp->cent.y;

			hue = atan((float)posy/(float)posx) * 180 / 3.14159;
			hue = hue * 255 / 360;

			if (posx > 0 && posy > 0)
				imp->oldh = hue;
			else if (posx > 0 && posy < 0)
				imp->oldh = hue + 255;
			else if (posx < 0 && posy < 0)
				imp->oldh = hue + 128;
			else if (posx < 0 && posy > 0)
				imp->oldh = hue + 128; 
			else if (posx == 0 && posy < 0)
				imp->oldh = 192;
			else if (posx == 0 && posy > 0)
				imp->oldh = 64;
			else if (posx < 0 && posy == 0)
				imp->oldh = 128;
			else if (posx > 0 && posy == 0)
				imp->oldh = 0;

			imp->RepaintTri();
//			PostMessage(GetParent(hWnd), CC_PAINTER_CHANGE, (UINT)hWnd, 0);
			PostMessage(GetParent(hWnd), CC_PAINTER_CHANGE, (WPARAM)hWnd, 0);
		}
		else if (wParam == MK_LBUTTON && capTri)
		{
			float posx, posy;
			float sat, val, fvalvec;

			posx = GET_X_LPARAM(lParam) - imp->ledge;

			if (posx < 0)
			{
				posx = 0;
			}

			posy = GET_Y_LPARAM(lParam);

			if (posy > imp->bedge)
				posy = imp->bedge;

			if (posy < imp->tedge)
				posy = imp->tedge;

			posy = fabs(posy - imp->bedge);

// Phucking Saturation... works at last! Sat is computed from the angle of a line through the black
// point and the little puck... a value between 0 and 60 degrees, scaled to 0...255. Groovy, huh?

			sat = atan((float)posx/(float)posy) * 180.0 / 3.14159;
			sat = sat * 255.0 / 60.0;

			imp->olds = CP_LIMITO_255(sat);

// And now the Value stuff... hate this part... I have to get this working somehow from the length.
// Firsties, get the length of the mouse pos. vector...

			val = sqrt(posx*posx + posy*posy);

// Next, get the length of the val = 255 vector through that point... sheesh! what a fuck...
			float sn = (float)imp->olds/255.0;

			float nx = (imp->redge - imp->ledge) * sn;
			float ny = (imp->bedge - imp->tedge) * (1.0 - sn) + ((imp->bedge - imp->tedge) / 2) * sn;

			fvalvec = sqrt(nx*nx + ny*ny);

// goody... now scale v with the fvalvec, multiply it by 255 and clamp it

			imp->oldv = CP_LIMITO_255((val / fvalvec) * 255.0);

//			PostMessage(GetParent(hWnd), CC_PAINTER_CHANGE, (UINT)hWnd, 0);
			PostMessage(GetParent(hWnd), CC_PAINTER_CHANGE, (WPARAM)hWnd, 0);
		}
		break;
	case WM_ERASEBKGND:
		PostMessage(GetParent(hWnd), CC_PAINTER_REPAINT, (WPARAM)hWnd, 0);
		return -1;
		break;
	case CC_PAINTER_INIT:
		imp = ((IPickerTriCircle*)wParam);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}


INT_PTR CALLBACK ColorTriCircleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static IPickerTriCircle* picker = NULL;

	switch (msg)
	{
	case CC_PAINTER_CHANGE:
		{
		picker->UpdatePickerPainter();
		picker->hSpin->SetValue((picker->oldh * 360) / 255, FALSE);
			//Color is HSV, encoded in an RGB format. Strange!
		PostMessage(GetParent(hWnd), CC_BIG_CHANGE, (WPARAM)RGB(picker->oldh, picker->olds, picker->oldv), 0);
		break;
		}
	case CC_PAINTER_REPAINT: 
		{
		PostMessage((HWND)wParam, CC_PAINTER_UPDATE, 0, 0);
		break;
		}
	case CC_BIG_COLORCHANGED:
		{
		int h = 0, s = 0, v = 0;

		h = GetRValue((COLORREF)wParam);
		s = GetGValue((COLORREF)wParam);
		v = GetBValue((COLORREF)wParam);

		if (picker->oldh != h)
		{
			picker->oldh = h;
			picker->RepaintTri();
			picker->hSpin->SetValue((picker->oldh * 360) / 255, FALSE);
		}

		picker->oldh = h;
		picker->olds = s;
		picker->oldv = v;

		picker->UpdatePickerPainter();

		break;
		}
	case WM_INITDIALOG: 
		{
		picker = GetIPickerPainter(GetDlgItem(hWnd, IDC_PICKER_PAINTER));
		picker->hSpin = SetupIntSpinner(hWnd, IDC_HUE_SPIN, IDC_HUE_EDIT, 0, 360, 0);
		picker->hEdit = GetICustEdit(GetDlgItem(hWnd, IDC_HUE_EDIT));
		picker->UpdatePickerPainter();
		break;
		}
	case CC_SPINNER_CHANGE:
	case WM_CUSTEDIT_ENTER:
		{
		picker->oldh = (picker->hSpin->GetIVal() * 255) / 360;
		picker->UpdatePickerPainter();
		PostMessage(GetParent(hWnd), CC_BIG_CHANGE, (WPARAM)RGB(picker->oldh, picker->olds, picker->oldv), 0);
		break;
		}
	case WM_DESTROY:
	case WM_CLOSE: 
		{
		ReleaseISpinner(picker->hSpin);
		ReleaseICustEdit(picker->hEdit);
		ReleaseIPickerPainter(picker);
		EndDialog(hWnd, 0);
		break;
		}
	default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ));
		break;
	}
	return FALSE;
}
