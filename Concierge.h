// Concierge.h: interface for the CConcierge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONCIERGE_H__C4A7ED38_DC08_4C58_8DF7_AD57EF9AB4A1__INCLUDED_)
#define AFX_CONCIERGE_H__C4A7ED38_DC08_4C58_8DF7_AD57EF9AB4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Interactive.h"

class CConcierge : public CInteractive  
{
public:
	CConcierge();
	virtual ~CConcierge();
	
	bool Check(void* data);
	void Action();
};

#endif // !defined(AFX_CONCIERGE_H__C4A7ED38_DC08_4C58_8DF7_AD57EF9AB4A1__INCLUDED_)
