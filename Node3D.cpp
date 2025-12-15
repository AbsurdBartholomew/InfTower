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
	m_name = GetResourceName();

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

void CCollisionPacket::CheckTriangle(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Plane triPlane(p1, p2, p3);

	if(triPlane.IsFrontFacingTo(m_normalizedVelocity))
	{
		double t0, t1;
		bool embeddedInPlane = false;

		double signedDistToTriPlane = triPlane.SignedDistanceTo(m_basePoint);
		float normalDotVelocity = triPlane.m_normal.Dot(m_velocity);

		if(normalDotVelocity == 0.0f)
		{
			if(fabs(signedDistToTriPlane) >= 1.0f)
			{
				return;
			}
			else
			{
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			t0 = (-1.0 - signedDistToTriPlane) / normalDotVelocity;
			t1 = ( 1.0 - signedDistToTriPlane) / normalDotVelocity;

			if(t0 > t1)
			{
				double temp = t1;
				t1 = t0;
				t0 = temp;
			}

			if(t0 > 1.0f || t1 < 0.0f)
			{
				return;
			}

			if (t0 < 0.0) t0 = 0.0;
			if (t1 < 0.0) t1 = 0.0;
			if (t0 > 1.0) t0 = 1.0;
			if (t1 > 1.0) t1 = 1.0;
		}

		Vector3 collisionPoint;
		bool foundCollision = false;
		float t = 1.0f;

		if(!embeddedInPlane)
		{
			Vector3 planeIntersectionPoint = (m_basePoint - triPlane.m_normal) + t0 * m_velocity;
			if(CheckPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				foundCollision = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}	
		}

		if(foundCollision == false)
		{
			float velocitySquaredLength = m_velocity.SquaredMagnitude();
			float a,b,c;
			float newT;
			
			// check against points
			a = velocitySquaredLength;
			
			// p1
			b = 2.0 * (m_velocity.Dot(m_basePoint - p1));
			c = (p1 - m_basePoint).SquaredMagnitude() - 1.0;
			if(GetLowestRoot(a,b,c,t,&newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p1;
			}
			
			// p2
			b = 2.0 * (m_velocity.Dot(m_basePoint - p2));
			c = (p2 - m_basePoint).SquaredMagnitude() - 1.0;
			if(GetLowestRoot(a,b,c,t,&newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p2;
			}

			// p3
			b = 2.0 * (m_velocity.Dot(m_basePoint - p3));
			c = (p3 - m_basePoint).SquaredMagnitude() - 1.0;
			if(GetLowestRoot(a,b,c,t,&newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p3;
			}

			// edges
			// p1 -> p2
			Vector3 edge = p2 - p1;
			Vector3 baseToVtx = p1 - m_basePoint;
			float edgeSquaredLength = edge.SquaredMagnitude();
			float edgeDotVelocity = edge.Dot(m_velocity);
			float edgeDotBaseToVtx = edge.Dot(baseToVtx);

			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * m_velocity.Dot(baseToVtx)) - 2.0 * edgeDotVelocity * edgeDotBaseToVtx;
			c = edgeSquaredLength * (1 - baseToVtx.SquaredMagnitude()) + edgeDotBaseToVtx * edgeDotBaseToVtx;

			if(GetLowestRoot(a,b,c,t,&newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVtx) / edgeSquaredLength;

				if(f >= 0.0 && f <= 1.0)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p1 + f * edge;
				}
			}

			// p2 -> p3
			edge = p3 - p2;
			baseToVtx = p2 - m_basePoint;
			edgeSquaredLength = edge.SquaredMagnitude();
			edgeDotVelocity = edge.Dot(m_velocity);
			edgeDotBaseToVtx = edge.Dot(baseToVtx);

			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * m_velocity.Dot(baseToVtx)) - 2.0 * edgeDotVelocity * edgeDotBaseToVtx;
			c = edgeSquaredLength * (1 - baseToVtx.SquaredMagnitude()) + edgeDotBaseToVtx * edgeDotBaseToVtx;

			if(GetLowestRoot(a,b,c,t,&newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVtx) / edgeSquaredLength;

				if(f >= 0.0 && f <= 1.0)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p2 + f * edge;
				}
			}

			// p3 -> p1
			edge = p1 - p3;
			baseToVtx = p3 - m_basePoint;
			edgeSquaredLength = edge.SquaredMagnitude();
			edgeDotVelocity = edge.Dot(m_velocity);
			edgeDotBaseToVtx = edge.Dot(baseToVtx);

			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * m_velocity.Dot(baseToVtx)) - 2.0 * edgeDotVelocity * edgeDotBaseToVtx;
			c = edgeSquaredLength * (1 - baseToVtx.SquaredMagnitude()) + edgeDotBaseToVtx * edgeDotBaseToVtx;

			if(GetLowestRoot(a,b,c,t,&newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVtx) / edgeSquaredLength;

				if(f >= 0.0 && f <= 1.0)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}
		if(foundCollision == true)
		{
			float distToColl = t * m_velocity.GetMagnitude();
			if(m_foundCollision == false || distToColl < m_nearestDistance)
			{
				// information for sliding
				m_nearestDistance = distToColl;
				m_intersectionPoint = collisionPoint;
				m_foundCollision = true;
			}
		}
	}	
}