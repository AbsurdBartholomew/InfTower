// StaircaseDoor.cpp: implementation of the CStaircaseDoor class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "StaircaseDoor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaircaseDoor::CStaircaseDoor()
{

}

CStaircaseDoor::~CStaircaseDoor()
{

}

bool CStaircaseDoor::Check()
{
	return false;
}

void CStaircaseDoor::Action()
{
	MessageBox(NULL, "I was pressed and there was a line here which I cannot remember", "Soooo", MB_OK);
}