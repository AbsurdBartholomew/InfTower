// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__5C8F184D_30D0_4A78_9823_516C973CCC1F__INCLUDED_)
#define AFX_PLAYER_H__5C8F184D_30D0_4A78_9823_516C973CCC1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node3D.h"
#include "GameGlobals.h"

class CPlayer : public CNode3D  
{
public:
	CPlayer();
	virtual ~CPlayer();

	void UpdateCamera();
	void Update(int dT);

	void CollideAndSlide();
	
	static CPlayer *New() { return new CPlayer(); }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "Player"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".ply"; }

	static CGameGlobals globals;

	float m_verticalLookOffset;
	CDisplayList *m_map;

private:
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();

	void Jump();

	bool m_onGround;

	Vector3 CollideWithWorld(const Vector3& pos, const Vector3& vel);
	int m_collisionRecursionDepth;

	float m_currentWalkSpeed;
	float m_angle;

	float m_walkFrames;

	Vector3 m_direction;

	CCollisionPacket *m_collisionPacket;
};

#endif // !defined(AFX_PLAYER_H__5C8F184D_30D0_4A78_9823_516C973CCC1F__INCLUDED_)
