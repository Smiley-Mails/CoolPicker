/*¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
				BIG PICKERS							
	This is the Large Plane that shows all			
	sorts of gradients, and has all sorts			
	of color options.								
////////////////////////////////////////////////////*/

#ifndef _I_Picker_Plane_
#define _I_Picker_Plane_

#include "CoolPick.h"

extern GlobalOptions glob;

class IPickerPlane : public ICustomControl
{
private:
	HWND hWnd;
public:
	int mode;
	int comp;
	int olda, oldb, oldc;

	HDC memDC, hDC;
	HBITMAP memBM;
	RECT rect;
	TRIVERTEX trivert[plane_width * plane_height];
	GRADIENT_TRIANGLE gTris[(plane_width - 1) * (plane_height - 1) * 2];

	IPickerPlane(HWND hwPlane) 
	{ 
		hWnd = hwPlane; 
		olda = oldb = oldc = 0;

		int i, j;

		for (j = 0; j < (plane_height - 1); j++)
		{
			for (i = 0; i < (plane_width - 1); i++)
			{
				gTris[j*(plane_width - 1) * 2 + i*2].Vertex1 = j * plane_width + i;
				gTris[j*(plane_width - 1) * 2 + i*2].Vertex2 = j * plane_width + i + 1;
				gTris[j*(plane_width - 1) * 2 + i*2].Vertex3 = j * plane_width + i + plane_width;

				gTris[j*(plane_width - 1) * 2 + i*2 + 1].Vertex1 = j * plane_width + i + plane_width;
				gTris[j*(plane_width - 1) * 2 + i*2 + 1].Vertex2 = j * plane_width + i + 1;
				gTris[j*(plane_width - 1) * 2 + i*2 + 1].Vertex3 = j * plane_width + i + plane_width + 1;
			}
		}
		SetMode(glob.planeSubmode);
		SetComp(0);

		hDC = GetDC(hWnd);

		GetClientRect(hWnd, &rect);

		memDC = CreateCompatibleDC(hDC);
		memBM = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);

		SelectObject(memDC, memBM);

		RepaintGrad();

		PostMessage(hWnd, CC_PLANE_INIT, (WPARAM)this, 0); 

		DebugPrint(_T("PickerPlane Constructor Called\n"));
	}

	~IPickerPlane() 
	{ 
		DebugPrint(_T("%d\n"), DeleteDC(memDC));
		DebugPrint(_T("%d\n"), ReleaseDC(hWnd, hDC));
		DebugPrint(_T("%d\n"), DeleteObject(memBM));

		DestroyWindow(hWnd); 
		hWnd = NULL; 
	}

	void Disable() {}
	int IsEnabled(){ return TRUE; }
	void Enable(int cmd) {}
	void Enable2(int cmd) {}
	HWND GetHwnd() { return hWnd; }
//	void SetTooltip(bool bEnable, MCHAR* text) {}
	void SetTooltip(bool bEnable, const MCHAR* text) {}

	void UpdatePickerPlane();
	void SetMode(int);
	void SetComp(int);
	void RepaintGrad();
	void PutPicker(int, int);
};

void ReleaseIPickerPlane(IPickerPlane *pickerPlane);
IPickerPlane *GetIPickerPlane(HWND hwPlane);
LRESULT WINAPI PickerPlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ColorPlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif