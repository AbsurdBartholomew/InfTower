// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
#define AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

inline float lerp(float start, float goal, float percent)
{
	return start + (percent * 0.3f) * (goal - start);
}

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	Vector3() { x = 0; y = 0; z = 0; }

	Vector3 &operator +=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3 &operator -=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3 &operator -(const Vector3 &rhs)
	{
		Vector3 r = Vector3(x,y,z);
		r.x -= rhs.x;
		r.y -= rhs.y;
		r.z -= rhs.z;
		return *this;
	}

	Vector3 &operator *=(const Vector3 &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	float GetMagnitude()
	{
		float dist = (x * x) + (y * y) + (z * z);
		return sqrtf(dist);
	}
};

struct Vector2
{
	float x;
	float y;

	Vector2(float _x, float _y) { x = _x; y = _y; }
	Vector2() { x = 0; y = 0; }
};


#endif // !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
