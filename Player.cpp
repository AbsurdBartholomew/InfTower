// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "Player.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool hasRegistered = false;

CPlayer::CPlayer()
{
	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);
	m_version = 0;

	if(hasRegistered == false)
	{
		Register(0);
		hasRegistered = true;
	}
}

CPlayer::~CPlayer()
{

}

void CPlayer::UpdateCamera()
{
	glLoadIdentity();

	gluLookAt(
        0, 0, 0,
        0,0,0,
        0, 1, 0);
    glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

void CPlayer::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CPlayer::Load(const char *filePath)
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

void CPlayer::Serialize(const char *filePath)
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