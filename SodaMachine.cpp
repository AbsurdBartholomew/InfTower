// SodaMachine.cpp: implementation of the CSodaMachine class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "SodaMachine.h"
#include "Input.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSodaMachine::CSodaMachine()
{
	m_name = GetResourceName();
	m_interactionIcon = ICON_GENERIC_INTERACTION;
	m_range = 5.0f;
}

CSodaMachine::~CSodaMachine()
{

}


bool CSodaMachine::Check(void* data)
{
	bool inRange = GetIsInRange(*(Vector3*)data);
	if(inRange)
	{
		m_showInteractionIcon = true;
	} else m_showInteractionIcon = false;
	return inRange;
}

void CSodaMachine::Action()
{
	PlaySound("Rsrc/button.wav", NULL, SND_ASYNC);
}