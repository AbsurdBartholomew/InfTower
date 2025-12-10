// GameWorld.cpp: implementation of the CGameWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "GameWorld.h"
#include <stdlib.h>
#include <windows.h>
#include <GL/glu.h>
#include "MidiPlayer.h"
#include "DisplayList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float aspect_ratio;
float near_plane = 1.0f;
float far_plane = 50000.0f;

static const GLfloat light_pos[8][4] = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { 8.0f, 3.0f, 0.0f, 1.0f },
    { -8.0f, 3.0f, 0.0f, 1.0f },
    { 0.0f, 3.0f, 8.0f, 1.0f },
    { 0.0f, 3.0f, -8.0f, 1.0f },
};

static const GLfloat light_diffuse[8][4] = {
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
};

const int BUILDING_RAND_POS_MIN = -4;
const int BUILDING_RAND_POS_MAX = 4;

static const GLfloat amb[] = { (float)0.5f, (float)0.5f, (float)0.5f, 1.f };

CDisplayList dl;

static const GLfloat environment_color[] = { (float)0.0f, (float)128/255.0f, (float)128/255.0f, 1.f };


CGameWorld::CGameWorld()
{

}

CGameWorld::~CGameWorld()
{

}

void CGameWorld::Init()
{
	/*
	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Init();
	}*/

	m_player = new CPlayer();
	AddChild(m_player);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, (GLint)640, (GLint)480);

	aspect_ratio = (float)640 / (float)480;

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	CMidiPlayer::playMIDIFile(m_hwnd, "Rsrc/bob.mid");

	dl.Load("Rsrc/hotel_extV3.dl");

	float light_radius = 5.0f;

    for (int i = 0; i < 8; i++)
    {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse[i]);
        glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 2.0f/light_radius);
        glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 1.0f/(light_radius*light_radius));
    }

	GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 0.3f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
}

void CGameWorld::Update(int dT)
{
	/*
	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->InternalUpdate(dT);
	}*/
}

void CGameWorld::Draw()
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glColor3d(1.0f, 1.0f, 1.0f);

	//m_player->UpdateCamera();
	m_player->m_position.z += -0.01f;
	m_player->m_position.x += 0.001f;

	gluLookAt(
        m_player->m_position.x, 3, -m_player->m_position.z,
        0,0,0,
        0, 1, 0);
	
	/*
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0,0,1.0f);
		glRotatef(1.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glIndexi(1);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2i(0,  1);
		glIndexi(2);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-1, -1);
		glIndexi(3);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2i(1, -1);
		glEnd();
	glPopMatrix();*/
	
	glPushMatrix();
	//glEnable(GL_LIGHTING);
	glTranslatef(0, -1, 0);
	glScalef(0.1f,0.1f,0.1f);
	//glScalef(3,3,3);
	dl.Draw();
	glPopMatrix();
	
	/*
	glPushMatrix();
	{
		glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MIN, 0.0f, BUILDING_RAND_POS_MIN);

            glTexCoord2f(32.0f, 0);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MAX, 0.0f, BUILDING_RAND_POS_MIN);

            glTexCoord2f(32.0f, 32.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MAX, 0.0f, BUILDING_RAND_POS_MAX);

            glTexCoord2f(0.0f, 32.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(BUILDING_RAND_POS_MIN, 0.0f, BUILDING_RAND_POS_MAX);
        }
        glEnd();
	}
	glPopMatrix();*/

	for(int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Draw();
	}
	
	glFlush();
}