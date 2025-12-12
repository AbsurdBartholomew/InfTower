// GameGlobals.cpp: implementation of the CGameGlobals class.
//
//////////////////////////////////////////////////////////////////////

#include "GameGlobals.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool hasRegistered = false;

CGameGlobals::CGameGlobals()
{
	m_id = Hash((unsigned char*)GetResourceName());
	Log::Print("resource ID %s hashes to %d\n", GetResourceName(), m_id);
	m_version = 0;

	if(hasRegistered == false)
	{
		Register(0);
		hasRegistered = true;
	}

	strcpy(m_name, "Anonymous Hotelgoer");
}

CGameGlobals::~CGameGlobals()
{

}

void CGameGlobals::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CGameGlobals::Load(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "rb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not open DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fread(&m_id, sizeof(m_id), 1, fPtr);
	fread(&m_version, sizeof(m_version), 1, fPtr);
	fread(m_name, 32, 1, fPtr);
	fread(&globals, sizeof(GameGlobals), 1, fPtr);

	fclose(fPtr);
}

void CGameGlobals::Serialize(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "wb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not create DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fwrite(&m_id, sizeof(m_id), 1, fPtr);
	fwrite(&m_version, sizeof(m_version), 1, fPtr);
	
	fwrite(m_name, 32, 1, fPtr);

	fwrite(&globals, sizeof(GameGlobals), 1, fPtr);

	fclose(fPtr);
}