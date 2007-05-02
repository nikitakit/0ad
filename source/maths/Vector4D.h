//***********************************************************
//
// Name:		CVector4D.h
// Last Update:	02/11/03
// Author:		Rich Cross
//
// Description: Provides an interface for a vector in R4 and
//				allows vector and scalar operations on it
//
//***********************************************************

#ifndef _VECTOR4D_H
#define _VECTOR4D_H


#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// CVector4D:
class CVector4D 
{
public:
	CVector4D() {}
    CVector4D(const float f[4]) { m_X=f[0]; m_Y=f[1]; m_Z=f[2]; m_W=f[3]; }
    CVector4D(float x,float y,float z,float w) { m_X=x; m_Y=y; m_Z=z; m_W=w; }
	CVector4D(const CVector4D& p) { m_X=p.m_X; m_Y=p.m_Y; m_Z=p.m_Z; m_W=p.m_W; }

	operator float*()
	{
		return &m_X;
	}

	operator const float*() const
	{
		return &m_X;
	}

	CVector4D operator-() const
	{
	    return CVector4D(-m_X,-m_Y,-m_Z,-m_W);
	}

    CVector4D operator+(const CVector4D& t) const
	{
    	return CVector4D(m_X+t.m_X,m_Y+t.m_Y,m_Z+t.m_Z,m_W+t.m_W);
	}

	CVector4D operator-(const CVector4D& t) const
	{
	    return CVector4D(m_X-t.m_X,m_Y-t.m_Y,m_Z-t.m_Z,m_W-t.m_W);
	}

    CVector4D operator*(const CVector4D& t) const
	{
	    return CVector4D(m_X*t.m_X,m_Y*t.m_Y,m_Z*t.m_Z,m_W*t.m_W);
	}

	CVector4D operator*(float f) const
	{
	    return CVector4D(m_X*f,m_Y*f,m_Z*f,m_W*f);
	}

	CVector4D operator/(float f) const
	{
	    float inv_f = 1.0f/f;
		return CVector4D(m_X*inv_f,m_Y*inv_f,m_Z*inv_f,m_W*inv_f);
	}

	CVector4D& operator+=(const CVector4D& t)
	{
		m_X += t.m_X; m_Y += t.m_Y; m_Z += t.m_Z; m_W += t.m_W;
	    return *this;
	}

	CVector4D& operator-=(const CVector4D& t)
	{
		m_X -= t.m_X; m_Y -= t.m_Y; m_Z -= t.m_Z; m_W -= t.m_W;
	    return *this;
	}

	CVector4D& operator*=(const CVector4D& t)
	{
		m_X *= t.m_X; m_Y *= t.m_Y; m_Z *= t.m_Z; m_W *= t.m_W;
		return *this;
	}

	CVector4D& operator*=(float f)
	{
	    m_X *= f; m_Y *= f; m_Z *= f; m_W *= f;
	    return *this;
	}

	CVector4D& operator/=(float f)
	{
		float inv_f = 1.0f / f;
	    m_X *= inv_f; m_Y *= inv_f; m_Z *= inv_f; m_W *= inv_f;
    	return *this;
	}

    float Dot(const CVector4D& a) const
	{
		return m_X*a.m_X + m_Y*a.m_Y + m_Z*a.m_Z + m_W*a.m_W;
	}

	float LengthSquared() const
	{
	    return Dot(*this);
	}

    float Length() const
	{
		return sqrtf(LengthSquared());
	}

    void Normalize()
	{
		const float inv_mag = 1.0f / Length();
        m_X *= inv_mag; m_Y *= inv_mag; m_Z *= inv_mag; m_W *= inv_mag;
	}

public:
	float m_X, m_Y, m_Z, m_W;
};
//////////////////////////////////////////////////////////////////////////////////


#endif
