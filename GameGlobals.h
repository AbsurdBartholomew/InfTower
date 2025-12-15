// GameGlobals.h: interface for the CGameGlobals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEGLOBALS_H__1540B66A_7F1D_4BA0_877C_481D6B9F6E8D__INCLUDED_)
#define AFX_GAMEGLOBALS_H__1540B66A_7F1D_4BA0_877C_481D6B9F6E8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rsrc.h"
#include <string.h>

enum ConciergeIntroState
{
	JUST_STARTED = 0, // the idea is that we can use this for any other state
	SPOKE_TO_CONCIERGE,
	SAW_THAT_ELEVATOR_WAS_BROKEN,
	REPORTED_BACK,
};

class CGameGlobals : public CRsrc  
{
public:
	CGameGlobals();
	virtual ~CGameGlobals();

	static CGameGlobals *New() { return new CGameGlobals(); }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "GameGlobals"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".glb"; }

	char m_name[32];
	
	struct GameGlobals
	{
		ConciergeIntroState introState;
	} globals;
};

#endif // !defined(AFX_GAMEGLOBALS_H__1540B66A_7F1D_4BA0_877C_481D6B9F6E8D__INCLUDED_)
