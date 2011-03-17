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
#ifndef TGQUATERNION_H
#define TGQUATERNION_H

#include "tgmath.h"

/**
* Implémentation des quaternion, utilisé pour les rotations
*/
class tgQuaternion
{
 public:
	float r,x,y,z;
	/**
	* Constructeur par défaut
	*/
	inline tgQuaternion (): r(1), x(0), y(0), z(0) {}
 protected:
	/**
	* Constructeur utilisé uniquement dans certaines fonction de Quaternion
	*/
	inline tgQuaternion (float nr, float nx, float ny, float nz): r(nr), x(nx),y(ny),z(nz) {}
 public:
	/**
	* Constructeur de copie
	*/
	inline tgQuaternion (const tgQuaternion& q): r(q.r), x(q.x), y(q.y), z(q.z) {}
	/**
	* Constructeur "standard", créant une rotation d'angle autour de axis
	* @param angle l'angle de rotation
	* @param axis l'axe de rotation
	*/
	inline tgQuaternion (float angle, const tgVector3& axis) { SetRotation(angle,axis); }

	tgQuaternion operator + (const tgQuaternion& q) const;
	tgQuaternion operator - (const tgQuaternion& q) const;
	tgQuaternion operator * (tgQuaternion q) const;
	tgQuaternion operator * (float f) const;
	tgQuaternion operator / (tgQuaternion q)const ;
	
	tgQuaternion operator += (const tgQuaternion& q);
	tgQuaternion operator -= (const tgQuaternion& q);
	tgQuaternion operator *= (tgQuaternion q);
	tgQuaternion operator *= (float f);
	tgQuaternion operator /= (tgQuaternion q);
	
	tgQuaternion Inverse () const;
	tgQuaternion Conjugate () const;
	tgQuaternion Unit () const;
	float Magnitude () const;
	
	/**
	* Donne l'axe des X dans la base du quaternion
	* @return ledit axe
	*/
	tgVector3 GetXAxis() const;
	
	/**
	* Donne l'axe des Y dans la base du quaternion
	* @return ledit axe
	*/
	tgVector3 GetYAxis() const;
	
	/**
	* Donne l'axe des Z dans la base du quaternion
	* @return ledit axe
	*/
	tgVector3 GetZAxis() const;
	
	/**
	* Tourne un vecteur selon ce Quaternion
	* @param v le vecteur a tourner
	* @return le vecteur tourné
	*/
	tgVector3 Rotate (const tgVector3& v) const;
	
	/**
	* Créer une rotation d'angle autour de axis
	* @param angle l'angle de rotation
	* @param axis l'axe de rotation
	*/
	void SetRotation (float angle, const tgVector3& axis);
	
	/**
	* Normalize le quaternion
	*/
	void Normalize ();
	
	/**
	* Met la matrice 4x4 correspondante dans matrix
	* @param matrix un tableau de 4x4 floats
	*/
	void GetMatrix(float* matrix) const;
	
	/**
	* Trouve la valeure de R, utilise par les mesh au format md5
	*/
	inline void ComputeR () { float t = 1.0f-(x*x)-(y*y)-(z*z); r = (t < 0.0f) ? 0.0f : -(float)sqrt(t); }
};

inline tgQuaternion tgQuaternion::operator + (const tgQuaternion& q) const
{
	return tgQuaternion(r+q.r, x+q.x, y+q.y, z+q.z);
}

inline tgQuaternion tgQuaternion::operator - (const tgQuaternion& q) const
{
	return tgQuaternion(r-q.r, x-q.x, y-q.y, z-q.z);
}

inline tgQuaternion tgQuaternion::operator * (tgQuaternion q) const
{
	return tgQuaternion (r*q.r - x*q.x - y*q.y - z*q.z, 
			r*q.x + x*q.r + y*q.z - z*q.y,
			r*q.y + y*q.r + z*q.x - x*q.z,
			r*q.z + z*q.r + x*q.y - y*q.x);
}

inline tgQuaternion tgQuaternion::operator * (float f) const
{
	return tgQuaternion (r*f, x*f, y*f, z*f);
}

inline tgQuaternion tgQuaternion::operator / (tgQuaternion q) const
{
	return (*this) * q.Inverse();
}

inline tgQuaternion tgQuaternion::operator += (const tgQuaternion& q)
{
	r += q.r;
	x += q.x;
	y += q.y;
	z += q.z;
	return (*this);
}

inline tgQuaternion tgQuaternion::operator -= (const tgQuaternion& q)
{
	r -= q.r;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return (*this);
}

inline tgQuaternion tgQuaternion::operator *= (tgQuaternion q)
{
	(*this) = q * (*this);
	return (*this);
}

inline tgQuaternion tgQuaternion::operator /= (tgQuaternion q)
{
	(*this) = (*this)*q.Inverse();
	return (*this);
}

inline tgQuaternion tgQuaternion::operator *= (float f)
{
	(*this) = (*this)*f;
	return (*this);
}

inline tgQuaternion tgQuaternion::Inverse () const
{
	return Conjugate()*(1/(r*r+x*x+y*y+z*z));
}

inline tgQuaternion tgQuaternion::Conjugate () const
{
	return tgQuaternion (r, -x, -y, -z);
}

inline tgQuaternion tgQuaternion::Unit () const
{
	return (*this)*(1/(this->Magnitude()));
}

inline float tgQuaternion::Magnitude () const
{
	return tgMath::Sqrt(r*r + x*x + y*y + z*z);
}

inline void tgQuaternion::SetRotation (float angle, const tgVector3& axis)
{
	tgVector3 tmp = axis/axis.Length();
	angle *= (float)DEGTORAD;
	float sin_a = tgMath::Sin(angle/2);
	float cos_a = tgMath::Cos(angle/2);
	x = tmp.x*sin_a;
	y = tmp.y*sin_a;
	z = tmp.z*sin_a;
	r = cos_a;
}

inline void tgQuaternion::Normalize ()
{
	float mag = this->Magnitude();
	if (mag) {
		r /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

inline void tgQuaternion::GetMatrix(float* matrix) const
{
	matrix[0]  = 1.0f - 2.0f * (y * y + z * z);
	matrix[1]  = 2.0f * (x * y + z * r);
	matrix[2]  = 2.0f * (x * z - y * r);
	matrix[3]  = 0.0f;

	matrix[4]  = 2.0f * (x * y - z * r);
	matrix[5]  = 1.0f - 2.0f * (x * x + z * z);
	matrix[6]  = 2.0f * (z * y + x * r);
	matrix[7]  = 0.0f;

	matrix[8]  = 2.0f * (x * z + y * r);
	matrix[9]  = 2.0f * (y * z - x * r);
	matrix[10] = 1.0f - 2.0f * (x * x + y * y);
	matrix[11] = 0.0f;

	matrix[12] = 0; 
	matrix[13] = 0;  
	matrix[14] = 0;
	matrix[15] = 1.0f;
}

inline tgVector3 tgQuaternion::Rotate (const tgVector3& v) const
{
	/*Quaternion vec(0, v.x, v.y, v.z);
		Quaternion q = *this;
		Quaternion qinv = q.Conjugate();

		Quaternion vec2 = q*vec*qinv;

		return Vector3 (vec2.x, vec2.y, vec2.z);*/
	tgVector3 uv, uuv;
	tgVector3 qvec(x,y,z);
	uv = qvec%v;
	uuv = qvec%uv;
	uv *= (2.0f*r);
	uuv *= 2.0f;

	return v+uv+uuv;
}

inline tgVector3 tgQuaternion::GetXAxis () const
{
	//float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twy = ty*r;
	float twz = tz*r;
	float txy = ty*x;
	float txz = tz*x;
	float tyy = ty*y;
	float tzz = tz*z;

	return tgVector3 (1.0f-(tyy+tzz), txy+twz, txz-twy);
}

inline tgVector3 tgQuaternion::GetYAxis () const
{
	float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twx = tx*r;
	float twz = tz*r;
	float txx = tx*x;
	float txy = ty*x;
	float tyz = tz*y;
	float tzz = tz*z;

	return tgVector3(txy-twz, 1.0f-(txx+tzz), tyz+twx);
}

inline tgVector3 tgQuaternion::GetZAxis () const
{
	float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twx = tx*r;
	float twy = ty*r;
	float txx = tx*x;
	float txz = tz*x;
	float tyy = ty*y;
	float tyz = tz*y;

	return tgVector3(txz+twy, tyz-twx, 1.0f-(txx+tyy));
}

#endif
