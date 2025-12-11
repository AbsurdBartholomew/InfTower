// Node3D.h: interface for the CNode3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE3D_H__6A0B8A23_9B28_4AD5_B848_852ECDDF3F4F__INCLUDED_)
#define AFX_NODE3D_H__6A0B8A23_9B28_4AD5_B848_852ECDDF3F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Vector3.h"
#include "DisplayList.h"

class CCollisionPacket
{
public:
	Vector3 m_radius;

	Vector3 m_r3Velocity;
	Vector3 m_r3Position;

	Vector3 m_velocity;
	Vector3 m_normalizedVelocity;
	Vector3 m_basePoint;

	bool m_foundCollision;
	double m_nearestDistance;
	Vector3 m_intersectionPoint;

	void CheckTriangle(const Vector3& p1, const Vector3& p2, const Vector3& p3);
};

class CNode3D  : public CNode
{
public:
	CNode3D();
	virtual ~CNode3D();
	
	virtual void Init();
	virtual void Update(int dT);
	virtual void Draw();

	virtual void InternalUpdate(int dT)
	{ 
		for(int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->InternalUpdate(dT);
		}

		Update(dT);
	}
	void CollideAndSlide();

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	Vector3 m_velocity;

	bool m_isVisible;
	bool m_canCollide;

	CDisplayList m_list;

	static CNode3D *New() { return new CNode3D(); }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "Node3D"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".no3"; }
};

#endif // !defined(AFX_NODE3D_H__6A0B8A23_9B28_4AD5_B848_852ECDDF3F4F__INCLUDED_)
