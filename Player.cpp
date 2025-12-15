// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "Player.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Input.h"
#include "Log.h"
#include "GameWorld.h"

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

	m_collisionPacket = new CCollisionPacket();
	m_collisionPacket->m_radius = 1.0f;

	m_onGround = true;

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

	if(CInput::IsKeyHeld(VK_SPACE) || CInput::IsKeyHeld(VK_HOME))
	{
		Jump();
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
	if(m_onGround == true)
	{
		m_velocity.y += 0.2f;
		m_onGround = false;
	}
}

void CPlayer::CollideAndSlide()
{
	m_collisionPacket->m_r3Position = m_position;
	m_collisionPacket->m_r3Velocity = m_velocity;

	Vector3 eSpacePosition = m_collisionPacket->m_r3Position /
							 m_collisionPacket->m_radius;
	Vector3 eSpaceVelocity = m_collisionPacket->m_r3Velocity /
							 m_collisionPacket->m_radius;

	m_collisionRecursionDepth = 0;
	Vector3 finalPos = CollideWithWorld(eSpacePosition, eSpaceVelocity);

	if(eSpaceVelocity.y <= -0.005f && eSpaceVelocity.y >= 0.005f) m_onGround = true;

	// TODO: gravity
	m_velocity.y -= 0.001f;
	if(m_velocity.y >= 0.005f) m_velocity.y -= 0.005f;

	finalPos *= m_collisionPacket->m_radius;

	//m_position += m_velocity;
	m_position = finalPos;
}

const float UNITS_PER_METER = 1.0f;

Vector3 CPlayer::CollideWithWorld(const Vector3& pos, const Vector3& vel)
{
	float veryCloseDistance = 0.005f;

	if(m_collisionRecursionDepth > 5) return pos;

	m_collisionPacket->m_velocity = vel;
	m_collisionPacket->m_normalizedVelocity = vel;
	m_collisionPacket->m_normalizedVelocity.GetMagnitude();
	m_collisionPacket->m_basePoint = pos;
	m_collisionPacket->m_foundCollision = false;

	// Parent is very likely our game world...
	CGameWorld *world = (CGameWorld*)m_parent;
	world->CheckWorldCollision(*m_collisionPacket);

	if(m_collisionPacket->m_foundCollision == false) return pos + vel;
	// Collision occured

	Vector3 destPoint = pos + vel;
	Vector3 newBasePoint = pos;

	if(m_collisionPacket->m_nearestDistance >= veryCloseDistance)
	{
		Vector3 v = Vector3(vel.x, vel.y, vel.z);
		v.SetLength(m_collisionPacket->m_nearestDistance - veryCloseDistance);
		newBasePoint = m_collisionPacket->m_basePoint + v;

		v.Normalize();
		m_collisionPacket->m_intersectionPoint -= veryCloseDistance * v;
	}

	Vector3 slidePlaneOrigin = m_collisionPacket->m_intersectionPoint;
	Vector3 slidePlaneNormal = newBasePoint - m_collisionPacket->m_intersectionPoint;

	slidePlaneNormal.Normalize();
	Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	Vector3 newDestPoint = destPoint - slidingPlane.SignedDistanceTo(destPoint)*slidePlaneNormal;
	Vector3 newVelVector = newDestPoint - m_collisionPacket->m_intersectionPoint;

	if(newVelVector.GetMagnitude() < veryCloseDistance) return newBasePoint;

	m_collisionRecursionDepth++;
	return CollideWithWorld(newBasePoint, newVelVector);
}