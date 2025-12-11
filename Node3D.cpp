// Node3D.cpp: implementation of the CNode3D class.
//
//////////////////////////////////////////////////////////////////////

#include "Node3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool hasRegistered = false;

CNode3D::CNode3D()
{
	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);
	m_version = 0;

	if(hasRegistered == false)
	{
		Register(0);
		hasRegistered = true;
	}

	m_isVisible = true;
	m_canCollide = true;
}

CNode3D::~CNode3D()
{

}

void CNode3D::Init()
{

}

void CNode3D::Update(int dT)
{

}

void CNode3D::Draw()
{
	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Draw();
	}
}

void CNode3D::CollideAndSlide()
{
	m_position += m_velocity;
}

void CNode3D::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CNode3D::Load(const char *filePath)
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

void CNode3D::Serialize(const char *filePath)
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