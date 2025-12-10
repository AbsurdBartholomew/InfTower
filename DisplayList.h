// DisplayList.h: interface for the CDisplayList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYLIST_H__71A044AF_81B5_49A7_8C65_11771FC4CB09__INCLUDED_)
#define AFX_DISPLAYLIST_H__71A044AF_81B5_49A7_8C65_11771FC4CB09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rsrc.h"
#include <windows.h>
#include <GL/gl.h>
#include "Vector3.h"

struct Tri
{
	Vector3 Vtx;
	Vector3 Normal;
	Vector2 UV;
};

class CDisplayList : public CRsrc  
{
public:
	CDisplayList();
	virtual ~CDisplayList();

	void Draw();
	
	static CDisplayList *New() { return new CDisplayList(); }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "DisplayList"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".dl"; }

	GLuint m_glList;
	GLuint m_texture;

private:
	Tri* m_tris;
	int m_triCount;
	
	void LoadTexture();
	char m_bitmapName[32];

	unsigned char *m_data;
	BITMAP m_bitmap;
};

#endif // !defined(AFX_DISPLAYLIST_H__71A044AF_81B5_49A7_8C65_11771FC4CB09__INCLUDED_)
