// Rsrc.cpp: implementation of the CRsrc class.
//
//////////////////////////////////////////////////////////////////////

#include "Rsrc.h"
#include <stdio.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Definitions/Constants
//////////////////////////////////////////////////////////////////////

#define MAX_TYPES 128

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HashToConstructorType constructorArray[MAX_TYPES];
int nRegisteredTypes = 0;

CRsrc::CRsrc()
{
	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);
}

CRsrc::~CRsrc()
{

}

void CRsrc::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CRsrc::RegisterAll()
{
	
}