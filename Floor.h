// Floor.h: interface for the CFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOOR_H__49330625_4045_4E60_9B8F_A051C6644FC0__INCLUDED_)
#define AFX_FLOOR_H__49330625_4045_4E60_9B8F_A051C6644FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rsrc.h"
#include "DisplayList.h"
#include "Node3D.h"
#include <vector>

class CFloor : public CNode3D  
{
public:
	CFloor();
	virtual ~CFloor();
	
	static CFloor *New() { return new CFloor(); }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "Floor"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".flr"; }

	const char *m_nextFloorName;
};

#endif // !defined(AFX_FLOOR_H__49330625_4045_4E60_9B8F_A051C6644FC0__INCLUDED_)
