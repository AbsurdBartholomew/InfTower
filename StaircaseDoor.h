// StaircaseDoor.h: interface for the CStaircaseDoor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STAIRCASEDOOR_H__92C0A620_FFEB_4544_B6BA_6CF49C0DED87__INCLUDED_)
#define AFX_STAIRCASEDOOR_H__92C0A620_FFEB_4544_B6BA_6CF49C0DED87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Interactive.h"

class CStaircaseDoor : public CInteractive  
{
public:
	CStaircaseDoor();
	virtual ~CStaircaseDoor();
	
	bool Check();
	void Action();
};

#endif // !defined(AFX_STAIRCASEDOOR_H__92C0A620_FFEB_4544_B6BA_6CF49C0DED87__INCLUDED_)
