/*////////////////////////////////////////////////
//Simple Graphics 1.1 - very small WinAPI Wrapper
//дл€ успешной компил€ции необходимо прилинковать
//библиотеку gdi32
//1.1: + Textout functions, Fullscreen
////////////////////////////////////////////////*/
#ifndef __SG__
#define __SG__
#undef   UNICODE
#undef  _UNICODE
#include <windows.h>
#include <string.h>
#include <stdio.h>
#ifndef __cplusplus
#define bool  int
#define true  1
#define false 0
#endif

HWND   SGHWnd               = 0;
HDC    SGWndDC              = 0;

bool   _sgWndFreeze         = false;
int    _sgMouseX            = 0;
int    _sgMouseY            = 0;
int    _sgMouseButtons      = 0;
int    _sgWndUpdateInterval = 10;
HANDLE _mainThread          = 0;
POINT  _sgWndSize           = {0};

/* Simple Graphics Functions
-----------------------------------------*/
void   sgCreateWindow (int width, int height);
void   sgCreateFullscreenWindow()               {sgCreateWindow(0, 0);}
int    sgGetMouseX()                            {return _sgMouseX;}
int    sgGetMouseY()                            {return _sgMouseY;}
int    sgGetMouseButtons()                      {return _sgMouseButtons;}
void   sgSelectTool(HDC dc, HGDIOBJ tool);
void   sgWndFreeze(bool state);
void   sgSelectFont(const char *face, int pointSize, unsigned int flags);
int    sgGetTextExtentX(const char *text);
int    sgGetTextExtentY(const char *text);
void   sgTextOut(int x, int y, const char *text);
/*---------------------------------------*/
enum {
    SG_FONT_NORMAL = 0x0, SG_FONT_BOLD      = 0x1, SG_FONT_ITALIC = 0x2,
    SG_FONT_STRIKE = 0x4, SG_FONT_UNDERLINE = 0x8
};

void sgSelectTool(HDC dc, HGDIOBJ tool)
{
   if (!GetObjectType(tool)) printf("\nSG_ERROR: The object is not exist!\n");
   DeleteObject(SelectObject(dc, tool));
}

void sgWndFreeze(bool state)
{
   _sgWndFreeze = state;
}

void sgSelectFont(const char *face, int pointSize, unsigned int flags)
{
	LOGFONT font ={0};
    strcpy (font.lfFaceName, face);
    font.lfWeight    = (flags & SG_FONT_BOLD) ? FW_SEMIBOLD : FW_NORMAL;
    font.lfItalic    =  flags & SG_FONT_ITALIC;
    font.lfUnderline =  flags & SG_FONT_UNDERLINE;
    font.lfStrikeOut =  flags & SG_FONT_STRIKE;
	font.lfHeight    =  pointSize;
	font.lfCharSet   = DEFAULT_CHARSET;
	sgSelectTool(SGWndDC, CreateFontIndirect(&font));
}

int sgGetTextExtentX(const char *text)
{
    SIZE textSize = {0};
    GetTextExtentPoint32(SGWndDC, text, strlen(text), &textSize);
	return textSize.cx;
}

int sgGetTextExtentY(const char *text)
{
    SIZE textSize = {0};
    GetTextExtentPoint32(SGWndDC, text, strlen(text), &textSize);
	return textSize.cy;

}

void sgTextOut(int x, int y, const char *text)
{

	TextOut(SGWndDC, x, y , text, strlen(text));
    GetPixel(SGWndDC, 0, 0); //dummy call
}

void _sgOnPaint (HWND wnd)
{
    if (!_sgWndFreeze)
    {
		HDC	dc		   =  0;
		PAINTSTRUCT ps = {0};
        RECT r		   = {0};
        SuspendThread(_mainThread);
		dc = BeginPaint (wnd, &ps);
        if (!dc) return;
        GetClientRect(wnd, &r);
	   {POINT size = {r.right - r.left, r.bottom - r.top};
		BitBlt (dc, 0, 0, size.x, size.y, SGWndDC, 0, 0, SRCCOPY);}
        ResumeThread(_mainThread);
        EndPaint (wnd, &ps);
   }
}

void _sgOnMouse (HWND hwnd, WPARAM buttons, LPARAM coords)
{
    _sgMouseX       = LOWORD(coords);
    _sgMouseY       = HIWORD(coords);
    _sgMouseButtons = (int) buttons;
}

LRESULT CALLBACK _sgWindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        case WM_DESTROY:        CloseHandle(_mainThread); DeleteDC(SGWndDC); PostQuitMessage (0); exit(0);
            break;
        case WM_PAINT:          _sgOnPaint(hwnd);
            break;
        case WM_TIMER:          InvalidateRect(hwnd, NULL, false); UpdateWindow(hwnd);
            break;
        case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN:
        case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_MOUSEMOVE: _sgOnMouse(hwnd, wp, lp);
            break;
    }
    return DefWindowProc (hwnd, msg, wp, lp);
}

DWORD WINAPI _sgWndCycle (LPVOID fullscreen)
{
	MSG msg = {0};
    WNDCLASSEX wc    = {sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, _sgWindowProc, 0, 0, NULL, NULL,
                        LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, TEXT("SGWindow"), NULL};
    int fullwidth    = 2 * GetSystemMetrics (SM_CXFIXEDFRAME) + _sgWndSize.x;
    int fullheight   = 2 * GetSystemMetrics (SM_CYFIXEDFRAME) + _sgWndSize.y + GetSystemMetrics (SM_CYCAPTION);
	DWORD style = WS_BORDER | WS_CAPTION | WS_SYSMENU;
	RegisterClassEx (&wc);
	if (*(bool *)fullscreen)
	{
		style = WS_POPUP;
		fullwidth  = _sgWndSize.x;
		fullheight = _sgWndSize.y;
	}
    SGHWnd           = CreateWindowEx (0, "SGWindow", "SGWindow",
                                style, GetSystemMetrics(SM_CXSCREEN) / 2 - fullwidth / 2,
                                GetSystemMetrics(SM_CYSCREEN) / 2 - fullheight / 2,
                                fullwidth, fullheight,
                                NULL, NULL, NULL, NULL);
   {HDC dc           = GetDC(SGHWnd);
    HBITMAP bmap     = CreateCompatibleBitmap(dc, _sgWndSize.x, _sgWndSize.y);
	SGWndDC          = CreateCompatibleDC(dc);
    ReleaseDC(SGHWnd, dc);
	SelectObject (SGWndDC, bmap);}
    PatBlt(SGWndDC, 0, 0, _sgWndSize.x, _sgWndSize.y, PATCOPY);
    SelectObject(SGWndDC, GetStockObject(HOLLOW_BRUSH));
    SetBkMode(SGWndDC, TRANSPARENT);
    SetTimer (SGHWnd, 0, _sgWndUpdateInterval, NULL);
    ShowWindow(SGHWnd, SW_SHOW);
    SetForegroundWindow(SGHWnd);
    UpdateWindow (SGHWnd);
    while (GetMessage  (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage  (&msg);
	}
    return (DWORD) msg.wParam;
}

void sgCreateWindow(int width, int height)
{
	 bool fullscreen = false;
     if(SGHWnd) return;
	 if (width == 0)
	 {
		 RECT r = {0};
		 fullscreen = true;
		 GetWindowRect(GetDesktopWindow(), &r);
		 _sgWndSize.x = r.right;
		 _sgWndSize.y = r.bottom;
	 }
	 else
	 {
		_sgWndSize.x = width;
		_sgWndSize.y = height;
	 }
     DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
                     &_mainThread, THREAD_SUSPEND_RESUME, FALSE, DUPLICATE_SAME_ACCESS);
	 {DWORD threadId; CreateThread (NULL, 0, _sgWndCycle, &fullscreen, 0,  &threadId);}
     while(!SGWndDC || GetForegroundWindow() != SGHWnd){ Sleep(10); }
}

#endif
