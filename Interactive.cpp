// Interactive.cpp: implementation of the CInteractive class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Interactive.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *iconPaths[NUM_INTERACTION_ICONS] = {
	"Rsrc/icon_talk.bmp",
	"Rsrc/icon_use.bmp"
};

CInteractive::CInteractive()
{
	m_range = 5.0f;
	m_active = true;
}

CInteractive::~CInteractive()
{

}

bool CInteractive::GetIsInRange(Vector3 target)
{
	Vector3 t = m_position;
	t -= target;
	float dist = t.GetMagnitude();
	//Log::Print("target: %f %f %f. position: %f %f %f. dist: %f\n",target.x, target.y, target.z, m_position.x, m_position.y, m_position.z, dist);

	if(dist < m_range) return true;
	return false;
}

void CInteractive::Draw()
{
	if(m_showInteractionIcon == true && m_active == true)
	{
		DrawInteractionIcon();
	}
}

void CInteractive::DrawInteractionIcon()
{
	if(m_bitmapData == NULL)
	{
		BITMAP bitmap;
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, iconPaths[m_interactionIcon], IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		GetObject(bmp, sizeof(BITMAP), &bitmap);

		m_bitmapData = (unsigned char*)bitmap.bmBits;
		
		glGenTextures(1, &m_iconTexture);
		glBindTexture(GL_TEXTURE_2D, m_iconTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, m_bitmapData);
	}

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 640, 480, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE);
	glPushMatrix();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, m_iconTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, m_bitmapData);

	glTranslatef(32.0f, 32.0f, 0.0f);

	glBegin(GL_QUADS);
    {
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2f(1.0f, 0);
		glVertex2f(64, 0.0f);

		glTexCoord2f(1.0f, -1.0f);
		glVertex2f(64, 64);

		glTexCoord2f(0.0f, -1.0f);
		glVertex2f(0.0f, 64);
    }
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
	glPopMatrix();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}