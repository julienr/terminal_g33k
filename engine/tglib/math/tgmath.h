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
#ifndef TGMATH_H
#define TGMATH_H

#include <stdlib.h>
#include <string.h>

#define DEGTORAD 3.14/180
#define RADTODEG 180/3.14
#define EPSILON 0.03125f
#define TG_INFINITY 9000000
#define PI 3.1415926535

#include <math.h>

/**
* Diverses fonctions mathématiques
*/
class tgMath
{
 public:
	static float InvSqrt(float x);
	static float Sqrt(float x);
	static float Sin(float x);
	static float Cos(float x);
  static bool FloatEq (float x, float y);
	template <class T>
	static void ChangeCoords(T& t1, T& t2, T& t3);
};

inline float tgMath::InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;       // get bits for floating value 
	i = 0x5f3759df - (i>>1); //
	x = *(float*)&i;         // convert bits back to float
	x = x*(1.5f-xhalf*x*x);  // Newton step, repeating increases accuracy
	return x;
}

inline bool tgMath::FloatEq (float x, float y)
{
  return (x < y+EPSILON && x > y-EPSILON);
}

inline float tgMath::Sqrt(float x)
{
	return x*InvSqrt(x);
}

inline float tgMath::Sin(float x)
{
	return sinf(x);
}

inline float tgMath::Cos(float x)
{
	return cosf(x);
}

template <class T>
inline void tgMath::ChangeCoords(T& x, T& y, T& z)
{
	T temp = y;
	y = z;
	z = -temp;
}

#endif
 
