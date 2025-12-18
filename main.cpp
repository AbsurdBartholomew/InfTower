/* Win32 GL Infinite Tower Game
	For Anchor Collective
	Author: sbart
	Year: 2025					*/

#include <windows.h>
#include <Commctrl.h>
#include <GL/gl.h>
#include <stdio.h>
#include "resource.h"
#include "DisplayList.h"
#include "GameWorld.h"
#include "Rsrc.h"
#include "Log.h"
#include "Input.h"

#define TARGET_FPS 60

HDC hDC;
HPALETTE hPalette = NULL;
TCHAR winTitle[32];
CGameWorld *gameWorld;
HWND currentHWnd;
bool fullscreen;
INT64 QPCFrequency;
HBITMAP palHbmp;
BITMAP palBitmap;

#ifdef _EDITOR
bool editorInvoked;
#endif

inline INT64 ElapsedMicroseconds(INT64 startCount, INT64 endCount)
{
    INT64 elapsedMicroseconds = endCount - startCount;
    elapsedMicroseconds *= 1000000;
    elapsedMicroseconds /= QPCFrequency;
    return elapsedMicroseconds;
}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	int i;

	CInput::m_msg = uMsg;
	CInput::m_wParam = wParam;

	switch(uMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDS_SAVE:
			Log::Print("Save called...\n");
			break;
		case IDS_EDITOR:
#ifdef _EDITOR
			if(editorInvoked == false)
			{
				if(fullscreen == false)
				{
					Log::Print("Editor Invoked\n");
					gameWorld->InitEditor(currentHWnd, GetModuleHandle(NULL));

					editorInvoked = true;
				} else Log::Print("Editor was invoked but the game has to be running in windowed mode for it to work. Continuing normally...\n");
			}
#endif
			break;
		case IDS_LOADGAME:
			Log::Print("Load game called...\n");
			break;
#ifdef _EDITOR
		case IDS_DRAWWIRE:
			gameWorld->m_drawWire = !gameWorld->m_drawWire;
#endif
		}
		return 0;

	case WM_KEYDOWN:
		CInput::m_keys[wParam] = true;
		return 0;
	case WM_KEYUP:
		CInput::m_keys[wParam] = false;
		return 0;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_ACTIVATE:
		for(i = 0; i < 256; i++)
		{
			CInput::m_keys[i] = false; // inputs get stuck after dialogs open
		}
		return 0;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_CLOSE:
		Log::CloseLogFile();
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}

HWND MakeWindow(const char *title, int x, int y, int w, int h)
{
	int         n, pf;
	HWND hWnd;
	LOGPALETTE* lpPal;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;
	WNDCLASS    wc;
	DWORD exStyle;
	DWORD style;
	RECT wRect;
	wRect.left = 0;
	wRect.right = w;
	wRect.top = 0;
	wRect.bottom = h;
	
	if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "InfTower";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}
    }

	if(fullscreen)
	{
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = 640;
		screenSettings.dmPelsHeight = 480;
		screenSettings.dmBitsPerPel = 32;
		screenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			exit(EXIT_FAILURE);
		}

		exStyle = WS_EX_APPWINDOW;
		style = WS_POPUP;
	} else
	{
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&wRect, style, FALSE, exStyle);

	hWnd = CreateWindowEx(exStyle, "InfTower", title, style,
			x, y, wRect.right - wRect.left, wRect.bottom - wRect.top, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

	hDC = GetDC(hWnd);

	/* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	//if(fullscreen == false) pfd.iPixelType = PFD_TYPE_RGBA; else pfd.iPixelType = PFD_TYPE_COLORINDEX;
	pfd.iPixelType = PFD_TYPE_RGBA; // unlikely i can get CI working :(
    pfd.cColorBits   = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) 
	{
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			   "Cannot find a suitable pixel format.", "Error", MB_OK); 
		return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
	{
		MessageBox(NULL, "SetPixelFormat() failed:  "
			   "Cannot set format specified.", "Error", MB_OK);
		return 0;
    } 

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) 
	{
		n = 1 << pfd.cColorBits;
		if (n > 256) n = 256;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
						sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
    
		/* if the pixel type is RGBA, then we want to make an RGB ramp,
		   otherwise (color index) set individual colors. */
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
			int redMask = (1 << pfd.cRedBits) - 1;
			int greenMask = (1 << pfd.cGreenBits) - 1;
			int blueMask = (1 << pfd.cBlueBits) - 1;
			int i;

			/* fill in the entries with an RGB color ramp. */
			for (i = 0; i < n; ++i) {
			lpPal->palPalEntry[i].peRed = 
				(((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
			lpPal->palPalEntry[i].peGreen = 
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			lpPal->palPalEntry[i].peBlue = 
				(((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
			lpPal->palPalEntry[i].peFlags = 0;
			}
		} else {
			/*
			lpPal->palPalEntry[0].peRed = 0;
			lpPal->palPalEntry[0].peGreen = 0;
			lpPal->palPalEntry[0].peBlue = 0;
			lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[1].peRed = 255;
			lpPal->palPalEntry[1].peGreen = 0;
			lpPal->palPalEntry[1].peBlue = 0;
			lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[2].peRed = 0;
			lpPal->palPalEntry[2].peGreen = 255;
			lpPal->palPalEntry[2].peBlue = 0;
			lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[3].peRed = 0;
			lpPal->palPalEntry[3].peGreen = 0;
			lpPal->palPalEntry[3].peBlue = 255;
			lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;*/

			palHbmp = (HBITMAP)LoadImage(NULL, "tower.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			GetObject(palHbmp, sizeof(BITMAP), &palBitmap);

			unsigned char *bData = (unsigned char*)palBitmap.bmBits;

			for(int i = 0; i < n; i++)
			{
				lpPal->palPalEntry[i].peRed = bData[i*3];
				lpPal->palPalEntry[i].peGreen = bData[i*3 + 1];
				lpPal->palPalEntry[i].peBlue = bData[i*3 + 2];
				lpPal->palPalEntry[i].peFlags = PC_EXPLICIT;
			}
		}

		hPalette = CreatePalette(lpPal);
		if (hPalette) {
			SelectPalette(hDC, hPalette, FALSE);
			RealizePalette(hDC);
		}

		free(lpPal);
    }

    ReleaseDC(hWnd, hDC);

	return hWnd;
}

static BOOL g_bActive  = TRUE; // Whether the app is active (not minimized)
static BOOL g_bReady   = FALSE; // Whether the app is ready to render frames

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HGLRC hRC;
	HWND hWnd;
	MSG msg;
	HACCEL hAccelTable;
	DWORD bufferType = PFD_DOUBLEBUFFER;
	BYTE color = PFD_TYPE_COLORINDEX;
	RECT rc;

	INT32 frameCount = 0;
    INT64 frameStart = 0, frameEnd = 0;
    INT64 averageFPS = 0, ticksAccumulator = 0;

    INT64 elapsedTime, overSleepDuration = 0;
    const INT64 TARGET_FRAME_TIME = (1000000 / TARGET_FPS) + 1;

	QueryPerformanceFrequency((LARGE_INTEGER*)&QPCFrequency);

	LoadString(hInstance, IDS_WINDOW_TITLE, winTitle, 32);

#ifdef _DEBUG
	if(MessageBox(NULL, "Would you like to run in fullscreen?", "Debug Only", MB_YESNO|MB_ICONQUESTION) == IDNO)
	{
		fullscreen = false;
	} else fullscreen = true;
#else
	fullscreen = true;
#endif

	hWnd = MakeWindow(winTitle, 0, 0, 640, 480);
	if(hWnd == NULL) exit(EXIT_FAILURE);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	InitCommonControls();

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	GetWindowRect (hWnd, &rc) ;
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;

	SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	currentHWnd = hWnd;

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR1);
	//testDL = new CDisplayList();
	//testDL->Serialize("Rsrc/test.dl");
	//testDL->Load("Rsrc/test.dl");

	Log::SetLogOutPath("inftower.log");

	gameWorld = new CGameWorld();
	gameWorld->m_hdc = hDC;
	gameWorld->m_hwnd = hWnd;
	gameWorld->Init();

	BOOL bGotMsg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	g_bReady = TRUE;

	/*
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		gameWorld->InternalUpdate(1);
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/

	while( WM_QUIT != msg.message  )
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if( g_bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

		if( bGotMsg )
        {
			// Translate and dispatch the message
            if( 0 == TranslateAccelerator( hWnd, hAccelTable, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
        }
		else
		{
			// Render a frame during idle time (no messages are waiting)
			if( g_bActive && g_bReady )
				gameWorld->InternalUpdate(1);
				gameWorld->Draw();

				SwapBuffers(hDC);
				
				// Limit framerate
				QueryPerformanceCounter((LARGE_INTEGER*)&frameEnd);
				elapsedTime = ElapsedMicroseconds(frameStart, frameEnd);

				while (elapsedTime < TARGET_FRAME_TIME)
				{
				    if ((elapsedTime + overSleepDuration) >= TARGET_FRAME_TIME)
				    {
				       overSleepDuration -= TARGET_FRAME_TIME - elapsedTime;
				        break;
				  }

				  Sleep(1);

				 QueryPerformanceCounter((LARGE_INTEGER*)&frameEnd);
				 elapsedTime = ElapsedMicroseconds(frameStart, frameEnd);

				 if (elapsedTime > TARGET_FRAME_TIME)
				     overSleepDuration += elapsedTime - TARGET_FRAME_TIME;
			 }


			 QueryPerformanceCounter((LARGE_INTEGER*)&frameEnd);
			 ticksAccumulator += frameEnd - frameStart;
			 frameCount += 1;

			 if ((frameCount % TARGET_FPS) == 0)
			 {
				   averageFPS = ((QPCFrequency * TARGET_FPS) + (ticksAccumulator - 1)) / ticksAccumulator; // round-off
				   ticksAccumulator = 0;
				   frameCount = 0;
				}

			 frameStart = frameEnd;
		}
    }

	return msg.wParam;
}