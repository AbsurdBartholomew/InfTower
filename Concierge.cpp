// Concierge.cpp: implementation of the CConcierge class.
//
//////////////////////////////////////////////////////////////////////

#include "Concierge.h"
// for name entry dialog
#include <windows.h>
#include "resource.h"
#include "main.h"
#include "Input.h"
#include "MidiPlayer.h"

#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConcierge::CConcierge()
{
	m_name = GetResourceName();
	m_range = 8.0f;
	m_interactionIcon = ICON_TALK_TO;
}

CConcierge::~CConcierge()
{

}

bool CConcierge::Check(void* data)
{
	bool inRange = GetIsInRange(*(Vector3*)data);
	if(inRange)
	{
		m_showInteractionIcon = true;
	} else m_showInteractionIcon = false;
	return inRange && m_active == true && CInput::IsKeyHeld(VK_SPACE);
}

static bool DoNameEntry()
{

}

void CConcierge::Action()
{
	m_sawMessage = true;

	MessageBox(currentHWnd, "Hey! How can I help you?", "The Concierge", MB_OK);
	MessageBox(currentHWnd, "You're here for your room?", "The Concierge", MB_OK);
	//MessageBox(NULL, "Okay. Just let me get your information. And your name is?", "The Concierge", MB_OK);
	//DialogBox(NULL, MAKEINTRESOURCE(IDD_NAMEENTRY), currentHWnd, NULL);

	MessageBox(currentHWnd, "Okay! Your room number is 30026, which is on the 300th floor.", "The Concierge", MB_OK);
	MessageBox(currentHWnd, "You might want to take the elevator for that.", "The Concierge", MB_OK);
	MessageBox(currentHWnd, "Here's the key. Enjoy your stay!", "The Concierge", MB_OK);

	CPlayer::globals.globals.SpokeToConcierge = true;
	CPlayer::globals.Serialize("SaveData.glb");

	CMidiPlayer::playMIDIFile(currentHWnd, "Rsrc/frontdesk.mid");

	m_active = false;
}