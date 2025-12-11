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

bool CStaircaseDoor::Check(void* data)
{
	return GetIsInRange(*(Vector3*)data);
}

void CStaircaseDoor::Action()
{
	MessageBox(NULL, "The stairs", "The Stairs", MB_OK);
	MessageBox(NULL, "call to you...", "The Stairs", MB_OK);
}