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
#ifndef TGMATHVECTOR3_H
#define TGMATHVECTOR3_H

#include "tgmath.h"
#include <iostream>
#include <string>
#include <sstream>
typedef std::stringstream tgstringstream;
typedef std::string tgstring;

/**
* Classe implémentant un vecteur tridimensionnel
*/
class tgVector3
{
 public:
  //FIXME: cf definition de fonction
	static void ProjectPointOnPlane(tgVector3& dst, tgVector3 p, tgVector3 normal);
	static void PerpendicularVector(tgVector3& dst, const tgVector3& src);

	float x,y,z;
	inline tgVector3(): x(0),y(0),z(0) {}
	inline tgVector3(float nx, float ny, float nz):x(nx), y(ny), z(nz) {}
	inline tgVector3(const tgVector3& v):x(v.x), y(v.y), z(v.z) {}
	~tgVector3() {}

	/**
	* Norme de ce vecteur
	*/
	float Length () const;
	void Normalize ();
	tgVector3 GetNormalized() const;
	void Set(float x, float y, float z);

	/**
	* Retourne la longueur de la projection de v (Vu dans le calcul ci-dessous) sur ce vecteur .
	* ->   ->            ->
	* U  * V    = Vu * ||U||
	*/
	float ProjectionLength(const tgVector3& v) const;
	
	void ChangeCoords ();
	void Negate();

	inline static void AddPointToBounds(const tgVector3& point, tgVector3& min, tgVector3& max);

	friend tgVector3 operator * (const tgVector3& v, float a);
	friend tgVector3 operator / (const tgVector3& v, float a);
  friend tgVector3 operator * (float a, const tgVector3& v);
  friend tgVector3 operator / (float a, const tgVector3& v);
	tgVector3 operator + (const tgVector3& a) const;
	tgVector3 operator - (const tgVector3& a) const;
	float operator * (const tgVector3& a) const;
	tgVector3 operator % (const tgVector3& a) const;

	tgVector3& operator *= (const float a);
	tgVector3& operator /= (const float a);
	tgVector3& operator += (const tgVector3& a);
	tgVector3& operator -= (const tgVector3& a);

	float& operator [] (size_t i);
	float operator [] (size_t i) const;

	bool operator != (const tgVector3& a) const;
	bool operator == (const tgVector3& a) const;
	
	
	friend inline std::ostream& operator << (std::ostream& output, const tgVector3& v)
	{
		output << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return output; 
	}
	/**
	 * cree le vecteur d'apres une string contenant les coordonnes separes par des espaces, example: "0.23 10.5 40"
	 */
	void FromString(const tgstring& str);

	static tgVector3 ZERO;
	static tgVector3 XNORM;
	static tgVector3 YNORM;
	static tgVector3 ZNORM;
};

inline void tgVector3::FromString(const tgstring& str)
{
	tgstringstream(str) >> x >> y >> z;
}

//FIXME: wtf ?? c'est nimp cette fonction ! (le plan est defini uniquement par une normale ? et fD ?!
inline void tgVector3::ProjectPointOnPlane(tgVector3& dst, tgVector3 p, tgVector3 normal)
{
	float   d, invDenom;
	invDenom = 1.0f / (normal*normal);
	d = (normal*p) * invDenom;
	dst = p-(normal*invDenom)*d;
}

inline void tgVector3::PerpendicularVector(tgVector3& dst, const tgVector3& src)
{
	int i, pos = 0;
	float val, min = 1.0;
	tgVector3 tmp;
	// Find the smallest magnitude axially aligned vector
	for (i = 0; i < 3; i++){
		val = fabs(src[i]);
		if (val < min){
			min = val;
			pos = i;
		}
	}
	tmp[pos] = 1.0;
	// Project the point onto the plane defined by src
	ProjectPointOnPlane(dst, tmp, src);
	// Normalize the result
	dst.Normalize();
}

inline void tgVector3::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

inline float tgVector3::Length () const
{
	return tgMath::Sqrt(x*x+y*y+z*z);
}

inline void tgVector3::Normalize ()
{
	float l = this->Length();
	if (l > EPSILON) //On évite les divisions par zéro
		(*this)/=l;
	else
		return;
}

inline tgVector3 tgVector3::GetNormalized() const
{
	float l = this->Length();
	if (l > EPSILON)
		return (*this)/l;
	else
		return (*this);
}

inline void tgVector3::Set (float nx, float ny, float nz)
{
	x = nx; y=ny; z=nz;
}

inline void tgVector3::ChangeCoords()
{
	tgMath::ChangeCoords<float>(x,y,z);
}

inline tgVector3 operator * (const tgVector3& v, float a)
{
	return tgVector3(v.x*a, v.y*a, v.z*a);
}

inline tgVector3 operator / (const tgVector3& v, float a)
{
	return tgVector3(v.x/a, v.y/a, v.z/a);
}

inline tgVector3 tgVector3::operator + (const tgVector3& a) const
{
	return tgVector3(x+a.x, y+a.y, z+a.z);
}

inline tgVector3 tgVector3::operator - (const tgVector3& a) const
{
	return tgVector3(x-a.x, y-a.y, z-a.z);
}

inline float tgVector3::operator * (const tgVector3& a) const
{
	return (x * a.x) + (y * a.y) + (z * a.z);
}

inline tgVector3 tgVector3::operator % (const tgVector3& a) const
{
	return tgVector3((y*a.z)-(z*a.y), (z*a.x)-(x*a.z), (x*a.y)-(y*a.x));
}

inline tgVector3& tgVector3::operator *= (const float a)
{
	x *= a;
	y *= a;
	z *= a;
	return (*this);
}

inline tgVector3& tgVector3::operator /= (const float a)
{
	x /= a;
	y /= a;
	z /= a;
	return (*this);
}

inline tgVector3& tgVector3::operator += (const tgVector3& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return (*this);
}

inline tgVector3& tgVector3::operator -= (const tgVector3& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return (*this);
}

inline float& tgVector3::operator [] (size_t i)
{
	if (i<0 || i>3)
		i  = 0;
		
	return *(&x+i);

	/*switch(i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
	}*/
}

inline float tgVector3::operator [] (size_t i) const
{
	if (i<0 || i>3)
		i  = 0;
	
	return *(&x+i);
	/*switch(i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
	}*/
}

inline bool tgVector3::operator != (const tgVector3& a) const
{
	return (a.x != x || a.y != y || a.z != z);
}

inline float tgVector3::ProjectionLength(const tgVector3& v) const
{
	return ((*this)*v)/this->Length();
}

inline void tgVector3::AddPointToBounds(const tgVector3& point, tgVector3& min, tgVector3& max)
{
	if (point.x < min.x)
		min.x = point.x;
	else if (point.x > max.x)
		max.x = point.x;

	if (point.y < min.y)
		min.y = point.y;
	else if (point.y > max.y)
		max.y = point.y;

	if (point.z < min.z)
		min.z = point.z;
	else if (point.z > max.z)
		max.z = point.z;
}
#endif

