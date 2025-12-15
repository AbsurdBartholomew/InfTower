// SodaMachine.h: interface for the CSodaMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SODAMACHINE_H__0014CF27_6A31_48D6_8AE3_ABA3757D6ECB__INCLUDED_)
#define AFX_SODAMACHINE_H__0014CF27_6A31_48D6_8AE3_ABA3757D6ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Interactive.h"

class CSodaMachine : public CInteractive  
{
public:
	CSodaMachine();
	virtual ~CSodaMachine();

	void Init();
	
	bool Check(void* data);
	void Action();

	const char *GetResourceName() { return "SodaMachine"; }
};

#endif // !defined(AFX_SODAMACHINE_H__0014CF27_6A31_48D6_8AE3_ABA3757D6ECB__INCLUDED_)
