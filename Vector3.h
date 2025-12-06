// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
#define AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	Vector3() { x = 0; y = 0; z = 0; }
};

struct Vector2
{
	float x;
	float y;

	Vector2(float _x, float _y) { x = _x; y = _y; }
	Vector2() { x = 0; y = 0; }
};


#endif // !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
