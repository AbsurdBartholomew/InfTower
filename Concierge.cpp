// Concierge.cpp: implementation of the CConcierge class.
//
//////////////////////////////////////////////////////////////////////

#include "Concierge.h"
// for name entry dialog
#include <windows.h>
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConcierge::CConcierge()
{

}

CConcierge::~CConcierge()
{

}

bool CConcierge::Check(void* data)
{
	return GetIsInRange(*(Vector3*)data);
}

void CConcierge::Action()
{
	MessageBox(NULL, "Hey! How can I help you?", "The Concierge", MB_OK);
	MessageBox(NULL, "You're here for your room?", "The Concierge", MB_OK);
	MessageBox(NULL, "Okay. Just let me get your information. And your name is?", "The Concierge", MB_OK);
	// maybe prompt the player for their name here...

	MessageBox(NULL, "Okay! Your room number is 30026, which is on the 300th floor.", "The Concierge", MB_OK);
	MessageBox(NULL, "You might want to take the elevator for that.", "The Concierge", MB_OK);
	MessageBox(NULL, "Here's the key. Enjoy your stay!", "The Concierge", MB_OK);
}