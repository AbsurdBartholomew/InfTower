// Rsrc.h: interface for the CRsrc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSRC_H__0BDDBAC3_C3D9_4D03_833F_E8AEAC516E8B__INCLUDED_)
#define AFX_RSRC_H__0BDDBAC3_C3D9_4D03_833F_E8AEAC516E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Hash.h"
#include <stddef.h>

class CRsrc  
{
public:
	CRsrc();
	virtual ~CRsrc();

	static CRsrc *New() { return NULL; }

	static void RegisterAll();
	
	unsigned long m_id;
	unsigned int m_version;

	virtual const char *GetResourceName() { return "Untitled"; }
	virtual const char *GetResourceExtension() { return ".rsc"; }
	virtual unsigned int GetResourceVersion() { return 0; }

	virtual void Serialize(const char *filePath) = 0;
	virtual void Load(const char *filePath) = 0;

	virtual void Register(unsigned short version);
};

typedef CRsrc* (*FnNew)(/* parameters unknown */);

struct HashToConstructorType
{
	unsigned long m_id;
	FnNew m_constructor;
};

extern HashToConstructorType constructorArray[];
extern int nRegisteredTypes;

#endif // !defined(AFX_RSRC_H__0BDDBAC3_C3D9_4D03_833F_E8AEAC516E8B__INCLUDED_)
