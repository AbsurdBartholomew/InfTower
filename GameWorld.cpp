// GameWorld.cpp: implementation of the CGameWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "GameWorld.h"
#include <stdlib.h>
#include <windows.h>
#include <GL/glu.h>
#include "MidiPlayer.h"
#include "DisplayList.h"
#include "StaircaseDoor.h"
#include "Concierge.h"
#include "SodaMachine.h"
#include "resource.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _EDITOR
#include <Commctrl.h>

static int splashFrames = 0;
extern bool editorInvoked;
WNDCLASSEX splashWc;
HWND splashHWnd;
HBITMAP bmp;
HBRUSH brush;

LRESULT CALLBACK SplashProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

    switch(msg)
    {
	case WM_INITDIALOG:
			SetTimer( hWnd, 1000, 1000, NULL );
			break;
	case WM_LBUTTONDOWN:
			DestroyWindow(hWnd);
	case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);

			brush = CreatePatternBrush(bmp);
			FillRect(hdc, &rt, brush);

			DeleteObject(brush);
			EndPaint(hWnd, &ps);

		break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;
		case WM_TIMER:
			if(wParam == 1000) InvalidateRect(hWnd, NULL, FALSE);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateSplash(HWND parent, HINSTANCE instance)
{
	RECT rc;

	MSG Msg;

	if(splashWc.cbSize == 0)
	{
		splashWc.cbSize = sizeof(WNDCLASSEX);
		splashWc.style = 0;
		splashWc.lpfnWndProc = SplashProc;
		splashWc.cbClsExtra = 0;
		splashWc.cbWndExtra = 0;
		splashWc.hInstance = instance;
		splashWc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		splashWc.hCursor = LoadCursor(NULL, IDC_ARROW);
		splashWc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		splashWc.lpszMenuName = NULL;
		splashWc.lpszClassName = "Splash";
		splashWc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&splashWc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}

	splashHWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		"Splash",
		"Splash",
		WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		177,
		185,
		parent,
		NULL,
		instance,
		NULL);

	if(splashHWnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
		return;
    }

	GetWindowRect (splashHWnd, &rc) ;
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;

	SetWindowPos(splashHWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	ShowWindow(splashHWnd, SW_SHOWNORMAL);
	UpdateWindow(splashHWnd);

	while(WM_QUIT != Msg.message)
    {
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		} else
		{
			splashFrames++;
			if(splashFrames >= 3 * 60000000)
			{
				DestroyWindow(splashHWnd);
				splashFrames = 0;
			}
		}
    }
}
/**********************************************************************************************/
// Editor

WNDCLASSEX editWc;
HWND editHWnd;
HWND nodeTree;

LRESULT CALLBACK EditorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

    switch(msg)
    {
		case WM_PAINT:

		break;
        case WM_CLOSE:
            DestroyWindow(splashHWnd);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void CGameWorld::CreateEditorWindow(HWND parent, HINSTANCE instance)
{
	RECT rc;

	if(editWc.cbSize == 0)
	{
		editWc.cbSize = sizeof(WNDCLASSEX);
		editWc.style = 0;
		editWc.lpfnWndProc = EditorProc;
		editWc.cbClsExtra = 0;
		editWc.cbWndExtra = 0;
		editWc.hInstance = instance;
		editWc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		editWc.hCursor = LoadCursor(NULL, IDC_ARROW);
		editWc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		editWc.lpszMenuName = NULL;
		editWc.lpszClassName = "Editor";
		editWc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&editWc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}

	editHWnd = CreateWindowEx(0,
		"Editor",
		"Node List",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		202, 478,
		parent, NULL, instance, NULL);

	if(editHWnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
		return;
    }
	GetWindowRect (editHWnd, &rc) ;
	int xPos = ((GetSystemMetrics(SM_CXSCREEN) - rc.right)/2) - 300;
	int yPos = ((GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2) - -60;

	nodeTree = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		0,
		WS_CHILD | WS_VISIBLE,
		0, 0, 
		rc.right, rc.bottom,
		editHWnd, NULL, instance, NULL);


	SetWindowPos(editHWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	ShowWindow(editHWnd, SW_SHOWNORMAL);
	UpdateWindow(editHWnd);

	// Populate the list...
	for(int i = 0; i < m_children.size(); i++)
	{
		TVITEM treeItem;
		TVINSERTSTRUCT treeInsert;
		HTREEITEM hti;

		treeItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		treeItem.pszText = (char*)m_children[i]->m_name;
		treeItem.cchTextMax = 32;

		Log::Print("Adding item %s to tree\n", m_children[i]->m_name);
	}
}

#endif

float aspect_ratio;
float near_plane = 0.2f;
float far_plane = 50000.0f;

static const GLfloat light_pos[8][4] = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { 8.0f, 3.0f, 0.0f, 1.0f },
    { -8.0f, 3.0f, 0.0f, 1.0f },
    { 0.0f, 3.0f, 8.0f, 1.0f },
    { 0.0f, 3.0f, -8.0f, 1.0f },
};

static const GLfloat light_diffuse[8][4] = {
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
};

GLfloat fogColor[4] = {0.8f, 0.8f, 0.9f, 1.0f};

const int BUILDING_RAND_POS_MIN = -4;
const int BUILDING_RAND_POS_MAX = 4;

static const GLfloat amb[] = { (float)0.9f, (float)0.9f, (float)0.9f, 1.f };

CDisplayList dl;
CStaircaseDoor tempDoor;
CConcierge conc;
CSodaMachine machine;

static const GLfloat environment_color[] = { (float)0.0f, (float)128/255.0f, (float)128/255.0f, 1.f };


CGameWorld::CGameWorld()
{

}

CGameWorld::~CGameWorld()
{

}

void CGameWorld::Init()
{
	/*
	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Init();
	}*/

	m_player = new CPlayer();
	m_player->m_position = Vector3(0.0f, 0.0f, 83.0f);
	AddChild(m_player);
	
	tempDoor.m_position = Vector3(25.110859f, 0.0f, 24.607515f);
	tempDoor.m_range = 5.0f;

	conc.m_position = Vector3(-0.384541f, 0.0f, 23.603014f);
	machine.m_position = Vector3(-25.187f, 0.0f, 8.249f);

	AddChild(&tempDoor);
	AddChild(&conc);
	AddChild(&machine);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, (GLint)640, (GLint)480);

	aspect_ratio = (float)640 / (float)480;

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//CMidiPlayer::playMIDIFile(m_hwnd, "Rsrc/bob.mid");

	dl.Load("Rsrc/hotel_extV3.dl");

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float light_radius = 5.0f;

    for (int i = 0; i < 8; i++)
    {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse[i]);
        glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 2.0f/light_radius);
        glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 1.0f/(light_radius*light_radius));
    }

	GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 0.3f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
}

void CGameWorld::Update(int dT)
{
	/*
	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->InternalUpdate(dT);
	}*/

	if(tempDoor.Check(&m_player->m_position))
	{
		tempDoor.Action();
	}

	if(conc.Check(&m_player->m_position))
	{
		conc.Action();
	}

	if(machine.Check(&m_player->m_position))
	{
		m_player->m_verticalLookOffset = lerp(m_player->m_verticalLookOffset, -8.0f, 0.3);
	} else m_player->m_verticalLookOffset = lerp(m_player->m_verticalLookOffset, 0.0f, 0.3);
}

void CGameWorld::Draw()
{
	glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_FOG);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glColor3d(1.0f, 1.0f, 1.0f);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.1f);
	glFogf(GL_FOG_START, 16.0f);
	glFogf(GL_FOG_END, 64.0f);

	m_player->UpdateCamera();
	//m_player->m_position.z += -0.01f;
	//m_player->m_position.x += 0.001f;

	//gluLookAt(
    //    m_player->m_position.x, 3, -m_player->m_position.z,
    //    0,0,0,
    //    0, 1, 0);
	
	/*
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0,0,1.0f);
		glRotatef(1.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glIndexi(1);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2i(0,  1);
		glIndexi(2);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-1, -1);
		glIndexi(3);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2i(1, -1);
		glEnd();
	glPopMatrix();*/
	
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTranslatef(0, -1, 0);
	glScalef(0.5f,0.5f,0.5f);
	//glScalef(3,3,3);
	dl.Draw();
	glPopMatrix();
	
	/*
	glPushMatrix();
	{
		glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MIN, 0.0f, BUILDING_RAND_POS_MIN);

            glTexCoord2f(32.0f, 0);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MAX, 0.0f, BUILDING_RAND_POS_MIN);

            glTexCoord2f(32.0f, 32.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MAX, 0.0f, BUILDING_RAND_POS_MAX);

            glTexCoord2f(0.0f, 32.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MIN, 0.0f, BUILDING_RAND_POS_MAX);
        }
        glEnd();
	}
	glPopMatrix();*/

	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Draw();
	}
	
	glFlush();
}



void CGameWorld::InitEditor(HWND parent, HINSTANCE instance)
{
#ifdef _EDITOR
	CreateSplash(parent, instance);
	CreateEditorWindow(parent, instance);
#endif
}