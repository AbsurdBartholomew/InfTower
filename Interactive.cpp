// Interactive.cpp: implementation of the CInteractive class.
//
//////////////////////////////////////////////////////////////////////

#include "Interactive.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInteractive::CInteractive()
{
	m_range = 5.0f;
}

CInteractive::~CInteractive()
{

}

bool CInteractive::GetIsInRange(Vector3 target)
{
	float dist = (m_position - target).GetMagnitude();
	Log::Print("%d\n", dist);

	if(dist < m_range) return true;
	return false;
}