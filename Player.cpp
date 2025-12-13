// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "Player.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Input.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameGlobals CPlayer::globals;

inline float rad2deg(float rad)
{
	return rad * (180.0f / 3.14159);
}

static bool hasRegistered = false;

CPlayer::CPlayer()
{
	m_name = GetResourceName();

	m_id = Hash((unsigned char*)GetResourceName());
	printf("resource ID %s hashes to %d\n", GetResourceName(), m_id);
	m_version = 0;

	if(hasRegistered == false)
	{
		Register(0);
		hasRegistered = true;
	}

	m_currentWalkSpeed = 0.0f;
	m_angle = 0.0f;
	m_verticalLookOffset = 0.0f;

	Log::Print("I'm at %f %f %f\n", m_position.x, m_position.y, m_position.z);

	globals.Serialize("SaveData.glb");
	Log::Print("My name is %s\n", globals.m_name);
}

CPlayer::~CPlayer()
{

}

void CPlayer::UpdateCamera()
{
	float walkOffset = sin(m_walkFrames / 6) / 4;
	glLoadIdentity();

	gluLookAt(
        0, 0, 0,
        0,m_verticalLookOffset,-15,
        0, 1, 0);
	glRotatef(rad2deg(m_angle), 0, 1, 0);
	glTranslatef(-m_position.x, -(m_position.y + 3) + walkOffset, -m_position.z);

	//Log::Print("I'm at %f %f %f\n", m_position.x, m_position.y, m_position.z);
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

const float MAX_SPEED = 0.05f;
const float SPEEDUP = 0.008f;

void CPlayer::Update(int dT)
{
	m_velocity.x *= 0.92f;
	m_velocity.z *= 0.92f;

	if(CInput::IsKeyHeld(VK_HOME))
	{
		Log::Print("Position is %f %f %f\n", m_position.x, m_position.y, m_position.z);
	}

	if(CInput::IsKeyHeld(VK_UP))
	{
		MoveForward();
	} else if(CInput::IsKeyHeld(VK_DOWN))
	{
		MoveBack();
	} else
	{
		m_walkFrames = 0;
	}

	if(CInput::IsKeyHeld(VK_LEFT))
	{
		MoveLeft();
	}
	if(CInput::IsKeyHeld(VK_RIGHT))
	{
		MoveRight();
	}

	m_currentWalkSpeed -= SPEEDUP;
	if(m_currentWalkSpeed < 0) m_currentWalkSpeed = 0;

	if((int)m_walkFrames % 30 == 0 && m_walkFrames > 0)
	{
		PlaySound("Rsrc/step.wav", NULL, SND_ASYNC);
	}
	
	CollideAndSlide();
}

void CPlayer::MoveForward()
{
	m_currentWalkSpeed += SPEEDUP;

	m_velocity.x += sin(m_angle) * m_currentWalkSpeed;
	m_velocity.z -= cos(m_angle) * m_currentWalkSpeed;

	m_walkFrames++;
}

void CPlayer::MoveBack()
{
	m_currentWalkSpeed -= SPEEDUP;

	m_velocity.x += sin(m_angle) * m_currentWalkSpeed;
	m_velocity.z -= cos(m_angle) * m_currentWalkSpeed;

	m_walkFrames += 0.5f;
}

void CPlayer::MoveLeft()
{
	m_angle -= 0.03f;
}

void CPlayer::MoveRight()
{
	m_angle += 0.03f;
}

void CPlayer::Jump()
{

}