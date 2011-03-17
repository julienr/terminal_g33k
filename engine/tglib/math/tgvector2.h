/*
-----------------------------------------------------------------------------
This source file is part of Terminal_G33k
Copyright (c) 2005 The Terminal_G33k Team
Also see acknowledgements in Readme.txt

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
or go to http://www.gnu.org/copyleft/gpl.txt
-----------------------------------------------------------------------------
*/
#ifndef TGMATHVECTOR2_H
#define TGMATHVECTOR2_H

#include "tgmath.h"
#include <iostream>

class tgVector2
{
 public:
	float x,y;
	inline tgVector2(): x(0),y(0) {}
	inline tgVector2(float nx, float ny):x(nx), y(ny) {}
	inline tgVector2(const tgVector2& v):x(v.x), y(v.y) {}
	
	~tgVector2() {}

	/**
	* Norme de ce vecteur
	*/
	float Length () const;
	void Normalize ();
	tgVector2 GetNormalized() const;
	void Set(float nx, float ny);

	/**
	* Retourne la longueur de la projection de v (Vu dans le calcul ci-dessous) sur ce vecteur .
	* ->   ->            ->
	* U  * V    = Vu * ||U||
	*/
	/*float ProjectionLength(const tgVector3& v) const;
	
	void ChangeCoords ();
	void Negate();*/

	//inline static void AddPointToBounds(const tgVector2& point, tgVector2& min, tgVector2& max);

	tgVector2 operator * (const float a) const;
	tgVector2 operator / (const float a) const;
	tgVector2 operator + (const tgVector2& a) const;
	tgVector2 operator - (const tgVector2& a) const;
	float operator * (const tgVector2& a) const;

	tgVector2& operator *= (const float a);
	tgVector2& operator /= (const float a);
	tgVector2& operator += (const tgVector2& a);
	tgVector2& operator -= (const tgVector2& a);

	float& operator [] (size_t i);
	float operator [] (size_t i) const;

	bool operator != (const tgVector2& a) const;
	bool operator == (const tgVector2& a) const;
	
	
	friend inline std::ostream& operator << (std::ostream& output, const tgVector2& v)
	{
		output << "(" << v.x << ", " << v.y << ")";
		return output; 
	}
};

inline float tgVector2::Length() const
{
	return tgMath::Sqrt(x*x+y*y);
}

inline void tgVector2::Normalize() 
{
	float l = this->Length();
	if (l > EPSILON) //On évite les divisions par zéro
		(*this)/=l;
	else
		return;
}

inline tgVector2 tgVector2::GetNormalized() const
{
	float l = this->Length();
	if (l > EPSILON)
		return (*this)/l;
	else
		return (*this);
}

inline void tgVector2::Set(float nx,float ny)
{
	x = nx; 
	y = ny;
}


inline tgVector2 tgVector2::operator * (const float a) const
{
	return tgVector2(x*a, y*a);
}

inline tgVector2 tgVector2::operator / (const float a) const
{
	return tgVector2(x/a, y/a);
}

inline tgVector2 tgVector2::operator + (const tgVector2& a) const
{
	return tgVector2(x+a.x, y+a.y);
}

inline tgVector2 tgVector2::operator - (const tgVector2& a) const
{
	return tgVector2(x-a.x, y-a.y);
}

inline float tgVector2::operator * (const tgVector2& a) const
{
	return (x * a.x) + (y * a.y);
}


inline tgVector2& tgVector2::operator *= (const float a)
{
	x *= a;
	y *= a;
	return (*this);
}

inline tgVector2& tgVector2::operator /= (const float a)
{
	x /= a;
	y /= a;
	return (*this);
}

inline tgVector2& tgVector2::operator += (const tgVector2& a)
{
	x += a.x;
	y += a.y;
	return (*this);
}

inline tgVector2& tgVector2::operator -= (const tgVector2& a)
{
	x -= a.x;
	y -= a.y;
	return (*this);
}

inline float& tgVector2::operator [] (size_t i)
{
	if (i<0 || i>2)
		i  = 0;
		
	return *(&x+i);

	/*switch(i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
	}*/
}

inline float tgVector2::operator [] (size_t i) const
{
	if (i<0 || i>2)
		i  = 0;
	
	return *(&x+i);
	/*switch(i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
	}*/
}

inline bool tgVector2::operator != (const tgVector2& a) const
{
	return (a.x != x || a.y != y);
}

inline bool tgVector2::operator == (const tgVector2& a) const
{
	return (a.x == x && a.y == y);
}


#endif

