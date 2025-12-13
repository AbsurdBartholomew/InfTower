// GameWorld.h: interface for the CGameWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEWORLD_H__D4013E79_94D0_42B7_ADF0_B2B6EC7BF98F__INCLUDED_)
#define AFX_GAMEWORLD_H__D4013E79_94D0_42B7_ADF0_B2B6EC7BF98F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "Node3D.h"
#include "Floor.h"
#include "Player.h"

class CGameWorld : public CNode3D  
{
public:
	CGameWorld();
	virtual ~CGameWorld();
	
	void Init();
	void Update(int dT);
	void Draw();

	void InitEditor(HWND parent, HINSTANCE instance);

	HDC m_hdc;
	HWND m_hwnd;

private:
	CFloor *m_prevFloor;
	CFloor *m_currentFloor;
	CFloor *m_nextFloor;

	CPlayer *m_player;
#ifdef _EDITOR
	void CreateEditorWindow(HWND parent, HINSTANCE instance);
#endif
};

#endif // !defined(AFX_GAMEWORLD_H__D4013E79_94D0_42B7_ADF0_B2B6EC7BF98F__INCLUDED_)
