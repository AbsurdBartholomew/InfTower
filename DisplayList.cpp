// DisplayList.cpp: implementation of the CDisplayList class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "DisplayList.h"
#include <stdio.h>
#include <stdlib.h>
#include "Node3D.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Definitions/Constants
//////////////////////////////////////////////////////////////////////

#define DL_VERSION 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisplayList::CDisplayList()
{
	//char d[16];
	m_id = Hash((unsigned char*)GetResourceName());
	//printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);
	Log::Print("resource ID %s hashes to %d\n", GetResourceName(), m_id);

	//sprintf(d, "%d", m_id);
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
	// right now bitmap loading is bugged on 9x so we need to check that first
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	Log::Print("OS Version: %d\n", osvi.dwMajorVersion);

	//char TEST[128];
	FILE *fPtr = fopen(filePath, "rb");
	int id = 0;
	int ver = 0;

	if(fPtr == NULL)
	{
		Log::Print("Could not open DL file %s\n", filePath);
		MessageBox(NULL, "Could not open DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fread(&id, sizeof(m_id), 1, fPtr);
	fread(&ver, sizeof(m_version), 1, fPtr);

	if(id != m_id)
	{
		Log::Print("Unexpected display list ID %d - likely not a DL!\n", id);
		MessageBox(NULL, "Not a display list!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	char thisTextureName[32] = "";
	int tempTxtrNameSize = 0;
    int faceCount = 0;

	fread(&tempTxtrNameSize, sizeof(tempTxtrNameSize), 1, fPtr);
	fread(m_bitmapName, 1, tempTxtrNameSize, fPtr);
	
	if(osvi.dwMajorVersion >= 5) LoadTexture();

	fread(&faceCount, 4, 1, fPtr);
	Log::Print("Texture Name: '%s'\nFace Count: %d\n", m_bitmapName, faceCount);

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
        glTexCoord2f(m_tris[t].UV.x, m_tris[t].UV.y);
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
		Log::Print("Could not create DL file %s\n", filePath);
		MessageBox(NULL, "Could not create DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fwrite(&m_id, sizeof(m_id), 1, fPtr);
	fwrite(&m_version, sizeof(m_version), 1, fPtr);

	fclose(fPtr);
}

void CDisplayList::Draw()
{
	if(m_data != NULL) glTexImage2D(GL_TEXTURE_2D, 0, 4, m_bitmap.bmWidth, m_bitmap.bmHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	glCallList(m_glList);
}

void CDisplayList::LoadTexture()
{
	char path[128];
	HBITMAP bmp;
	const char *pathStart = "rsrc/";

	sprintf(path, "%s%s", pathStart, m_bitmapName);
	Log::Print("Path is %s\n", path);

	bmp = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(bmp, sizeof(BITMAP), &m_bitmap);

	unsigned char R,G,B;
	m_data = (unsigned char*)m_bitmap.bmBits;

	for (int b = 0; b < m_bitmap.bmWidth * m_bitmap.bmHeight ; b++)
    {
		B = m_data[b*3]; 
		G = m_data[b*3+1]; 
		R = m_data[b*3+2];

		m_data[b*3] = R; 
		m_data[b*3+1] = G; 
		m_data[b*3+2] = B;
    }

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}