// DisplayList.cpp: implementation of the CDisplayList class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "DisplayList.h"
#include <stdio.h>
#include <stdlib.h>
#include "Node3D.h"

//////////////////////////////////////////////////////////////////////
// Definitions/Constants
//////////////////////////////////////////////////////////////////////

#define DL_VERSION 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisplayList::CDisplayList()
{
	char d[16];
	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);

	sprintf(d, "%d", m_id);
	//MessageBox(NULL, GetResourceName(), d, MB_OK);
	m_version = DL_VERSION;
}

CDisplayList::~CDisplayList()
{

}


void CDisplayList::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CDisplayList::Load(const char *filePath)
{
	//char TEST[128];
	FILE *fPtr = fopen(filePath, "rb");
	int id = 0;
	int ver = 0;

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not open DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fread(&id, sizeof(m_id), 1, fPtr);
	fread(&ver, sizeof(m_version), 1, fPtr);

	if(id != m_id)
	{
		MessageBox(NULL, "Not a display list!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	char thisTextureName[32] = "";
	int tempTxtrNameSize = 0;
    int faceCount = 0;

	fread(&tempTxtrNameSize, sizeof(tempTxtrNameSize), 1, fPtr);
	fread(thisTextureName, 1, tempTxtrNameSize, fPtr);

	fread(&faceCount, 4, 1, fPtr);
    printf("Texture Name: '%s'\nFace Count: %d\n", thisTextureName, faceCount);

	m_triCount = faceCount * 3;
	m_tris = new Tri[m_triCount];

	//sprintf(TEST, "Face Count: %d\nTri Count: %d\n Texture Name:%s\n", faceCount, m_triCount, thisTextureName);
	//MessageBox(NULL, TEST, "Display List", MB_OK);

	for(int v = 0; v < m_triCount; v++)
	{
		Tri thisTri;

		fread(&thisTri.Normal, sizeof(Vector3), 1, fPtr);
        fread(&thisTri.Vtx, sizeof(Vector3), 1, fPtr);

        m_tris[v] = thisTri;
	}

	for(int u = 0; u < faceCount * 3; u++)
    {                    
		fread(&m_tris[u].UV, sizeof(Vector2), 1, fPtr);
    }

	m_glList = glGenLists(1);

	// time to actually construct the list
	glNewList(m_glList, GL_COMPILE);
    glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glBegin(GL_TRIANGLES);

	for(int t = 0; t < m_triCount; t++)
	{
		glNormal3f(m_tris[t].Normal.x, m_tris[t].Normal.y, m_tris[t].Normal.z);
        glTexCoord2f(m_tris[t].UV.x, -m_tris[t].UV.y);
        glVertex3f(m_tris[t].Vtx.x, m_tris[t].Vtx.y, m_tris[t].Vtx.z);
	}

	glEnd();
	glEndList();
	
	fclose(fPtr);
}

void CDisplayList::Serialize(const char *filePath)
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

void CDisplayList::Draw()
{
	glCallList(m_glList);
}