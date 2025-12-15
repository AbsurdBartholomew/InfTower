// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
#define AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/******************************************************************/
struct Vector3
{
	union
	{
		float m_vec[3];

		struct
		{
			float x, y, z;
		};
	};
	
	Vector3(float f) { x = f; y = f; z = f; }
	Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	Vector3(const Vector3& _vec) { x = _vec.x; y = _vec.y; z = _vec.z; }
	Vector3() { x = 0; y = 0; z = 0; }

	inline Vector3 &operator +=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline Vector3 &operator -=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline Vector3 &operator *=(const Vector3 &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	inline Vector3 &operator /=(const Vector3 &rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	inline double Dot(const Vector3 &b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}

	inline Vector3 Cross(const Vector3 &b)
	{
		Vector3 temp;
		temp.x = y * b.z - z * b.y;
		temp.y = z * b.x - x * b.z;
		temp.z = x * b.y - y * b.x;
		return temp;
	}

	inline float GetMagnitude()
	{
		float dist = (x * x) + (y * y) + (z * z);
		return sqrtf(dist);
	}

	inline float SquaredMagnitude()
	{
		return x*x + y*y + z*z;
	}

	inline void Normalize()
	{
		float mag = GetMagnitude();
		x /= mag;
		y /= mag;
		z /= mag;
	}

	inline void SetLength(float length)
	{
		float factor;
		factor = length / GetMagnitude();
		
		x *= factor;
		y *= factor;
		z *= factor;
	}

	friend inline Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs);
	friend inline Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs);
	friend inline Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs);
	inline Vector3 operator /(const float &rhs)
	{
		Vector3 r = Vector3(x,y,z);
		r.x /= rhs;
		r.y /= rhs;
		r.z /= rhs;
		return r;
	}
	friend inline Vector3 operator/(const Vector3 &lhs, const Vector3 &rhs);
};

/******************************************************************/
inline Vector3 operator +(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 r = Vector3(lhs.x,lhs.y,lhs.z);
	r.x += rhs.x;
	r.y += rhs.y;
	r.z += rhs.z;
	return r;
}

inline Vector3 operator -(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 r = Vector3(lhs.x,lhs.y,lhs.z);
	r.x -= rhs.x;
	r.y -= rhs.y;
	r.z -= rhs.z;
	return r;
}

inline Vector3 operator *(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 r = Vector3(lhs.x,lhs.y,lhs.z);
	r.x *= rhs.x;
	r.y *= rhs.y;
	r.z *= rhs.z;
	return r;
}

inline Vector3 operator /(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 r = Vector3(lhs.x,lhs.y,lhs.z);
	r.x /= rhs.x;
	r.y /= rhs.y;
	r.z /= rhs.z;
	return r;
}
/******************************************************************/
struct Vector2
{
	union
	{
		float m_vec[2];

		struct
		{
			float x, y;
		};
	};

	Vector2(float _x, float _y) { x = _x; y = _y; }
	Vector2() { x = 0; y = 0; }
};
/******************************************************************/

/******************************************************************/
// https://www.peroxide.dk/papers/collision/collision.pdf
struct Plane
{
	float m_equation[4];
	Vector3 m_origin;
	Vector3 m_normal;

	Plane(const Vector3 &origin, const Vector3 &normal)
	{
		m_normal = normal;
		m_origin = origin;

		m_equation[0] = normal.x;
		m_equation[1] = normal.y;
		m_equation[2] = normal.z;
		m_equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
	}

	Plane(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
	{
		m_normal = (p2 - p1).Cross(p3 - p1);
		m_normal.Normalize();
		m_origin = p1;

		m_equation[0] = m_normal.x;
		m_equation[1] = m_normal.y;
		m_equation[2] = m_normal.z;
		m_equation[3] = -(m_normal.x * m_origin.x + m_normal.y * m_origin.y + m_normal.z * m_origin.z);
	}

	bool IsFrontFacingTo(const Vector3 &direction)
	{
		double dot = m_normal.Dot(direction);
		return (dot <= 0);
	}

	double SignedDistanceTo(const Vector3 &point)
	{
		return (point.Dot(m_normal)) + m_equation[3];
	}
};
/******************************************************************/
// some math & collision functions since I don't know where else to keep them

inline float lerp(float start, float goal, float percent)
{
	return start + (percent * 0.3f) * (goal - start);
}

#define in(a) ((unsigned int&) a)

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800)

inline bool CheckPointInTriangle(const Vector3 &point, const Vector3 &pa, const Vector3 &pb, const Vector3 &pc)
{
	Vector3 e10 = pb - pa;
	Vector3 e20 = pc - pa;

	float a = e10.Dot(e10);
	float b = e10.Dot(e20);
	float c = e20.Dot(e20);
	float ac_bb = (a * c)-(b * b);
	Vector3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);

	float d = vp.Dot(e10);
	float e = vp.Dot(e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;

	return (( in(z)& ~(in(x)|in(y)) ) & 0x80000000);
}

inline bool GetLowestRoot(float a, float b, float c, float maxR, float *root)
{
	// Check if a solution exists
	float determinant = b * b - 4.0f * a * c;

	// If determinant is negative it means no solutions.
	if(determinant < 0.0f) return false;

	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);

	// Sort so x1 <= x2
	if (r1 > r2) 
	{
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}

	// Get lowest root:
	if (r1 > 0 && r1 < maxR) 
	{
		*root = r1;
		return true;
	}

	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) 
	{
		*root = r2;
		return true;
	}

	// No (valid) solutions
	return false;
}

#pragma warning(pop)
/******************************************************************/
#endif // !defined(AFX_VECTOR3_H__63762BF2_A91D_49BF_999D_7106B937F268__INCLUDED_)
