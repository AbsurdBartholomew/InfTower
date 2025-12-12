// Input.h: interface for the CInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUT_H__E0DFD4DB_A066_48C5_9FD1_938656A241CA__INCLUDED_)
#define AFX_INPUT_H__E0DFD4DB_A066_48C5_9FD1_938656A241CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CInput  
{
public:
	CInput();
	virtual ~CInput();

	static bool IsKeyHeld(WPARAM param);
	
	friend LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	static UINT m_msg;
	static WPARAM m_wParam;
	static bool m_keys[256];
};

#endif // !defined(AFX_INPUT_H__E0DFD4DB_A066_48C5_9FD1_938656A241CA__INCLUDED_)
