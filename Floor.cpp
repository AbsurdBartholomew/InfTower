// Floor.cpp: implementation of the CFloor class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "Floor.h"

//////////////////////////////////////////////////////////////////////
// Definitions/Constants
//////////////////////////////////////////////////////////////////////

#define FLR_VERSION 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFloor::CFloor()
{
	char d[16];
	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);

	sprintf(d, "%d", m_id);
	//MessageBox(NULL, GetResourceName(), d, MB_OK);
	m_version = FLR_VERSION;
}

CFloor::~CFloor()
{

}

void CFloor::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CFloor::Load(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "rb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not open DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fread(&m_id, sizeof(m_id), 1, fPtr);
	fread(&m_version, sizeof(m_version), 1, fPtr);

	fclose(fPtr);
}

void CFloor::Serialize(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "wb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not create DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fwrite(&m_id, sizeof(m_id), 1, fPtr);
	fwrite(&m_version, sizeof(m_version), 1, fPtr);

	fclose(fPtr);
}