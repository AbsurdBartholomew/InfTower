// StaircaseDoor.cpp: implementation of the CStaircaseDoor class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "StaircaseDoor.h"
#include "main.h"
#include "Input.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaircaseDoor::CStaircaseDoor()
{
	m_interactionIcon = ICON_GENERIC_INTERACTION;
}

CStaircaseDoor::~CStaircaseDoor()
{

}

bool CStaircaseDoor::Check(void* data)
{
	bool inRange = GetIsInRange(*(Vector3*)data);
	if(inRange)
	{
		m_showInteractionIcon = true;
	} else m_showInteractionIcon = false;
	return inRange && CInput::IsKeyHeld(VK_SPACE) && m_active == true;
}

void CStaircaseDoor::Action()
{
	m_sawMessage = true;
	MessageBox(currentHWnd, "The stairs", "The Stairs", MB_OK);
	MessageBox(currentHWnd, "call to you...", "The Stairs", MB_OK);
}