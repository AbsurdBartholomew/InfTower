// Interactive.h: interface for the CInteractive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_)
#define AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node3D.h"

class CInteractive : public CNode3D  
{
public:
	CInteractive();
	virtual ~CInteractive();
	
	virtual bool Check() = 0;
	virtual void Action() = 0;

	bool GetIsInRange(Vector3 target);

	float m_range;
};

#endif // !defined(AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_)
