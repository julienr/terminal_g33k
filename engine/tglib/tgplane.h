
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
#ifndef TGPLANE_H
#define TGPLANE_H
#include "math/tgvector3.h"
#include "math/tgvector2.h"
#include "math/tggeom2d.h"

enum {
	POINT_ON_PLANE,
	POINT_BEHIND_PLANE,
	POINT_IN_FRONT_OF_PLANE
};

enum {
	BOX_INTERSECT_PLANE,
	BOX_BEHIND_PLANE,
	BOX_IN_FRONT_OF_PLANE
};

enum {
	POLY_INTERSECT_PLANE,
	POLY_BEHIND_PLANE,
	POLY_IN_FRONT_OF_PLANE
};

enum {
	SPHERE_INTERSECT_PLANE,
	SPHERE_BEHIND_PLANE,
	SPHERE_IN_FRONT_OF_PLANE
};


enum {
	//plans axial
	PLANE_X = 0,
	PLANE_Y = 1,
	PLANE_Z = 2,
	
	//plans non-axial
	PLANE_ANYX = 3,
	PLANE_ANYY = 4,
	PLANE_ANYZ = 5
};

class tgAABoundingBox;
class tgBoundingSphere;

class tgPlane
{
 public:
	inline tgPlane () : vNormal(0,0,0), fD(0.0f)
	{ /*CalculatePoints();*/ }
	inline tgPlane (const tgVector3& v, float f) : vNormal(v), fD(f)
	{ /*CalculatePoints();*/ }
	
	inline tgPlane (const tgPlane& p) : vNormal(p.vNormal), fD(p.fD)
	{ v[0] = p.v[0]; v[1] = p.v[1]; v[2] = p.v[2]; }
	
	inline tgPlane (const tgVector3& v1, const tgVector3& v2, const tgVector3& v3) {
		SetFromPoints(v1,v2,v3);
	}
	
	inline ~tgPlane () {}

	//TODO: Cette méthode n'est utile que pour le déverminage. Elle est mal codée et peut amener des bugs. A enlever dès que possible
	inline void CalculatePoints ();
	
	inline void SetNormal (tgVector3 v) { vNormal = v; }
	inline void SetD (float f) { fD = f; }
	inline tgVector3 GetNormal () const { return vNormal; }
	inline float GetD () const { return fD; }
	
	void SetFromPoints (const tgVector3& v1, const tgVector3& v2, const tgVector3& v3);
	void SetFromPointAndNormal (const tgVector3& point, const tgVector3& normal);

	//retourne le vecteur perpendiculaire a la normal
	tgVector3 ProjectVector(const tgVector3& in);
	//retourne la projection du point sur le plan
	tgVector3 ProjectPoint(const tgVector3& in);
	
	float GetDistance (const tgVector3& v) const;
	int ClassifyPoint (const tgVector3& v) const;
	int ClassifyBox (tgAABoundingBox* box) const;
	int ClassifyBox(const tgVector3& vMin, const tgVector3& vMax) const;
	int ClassifyPolygon(tgVector3* points, int num) const;
	int ClassifySphere (const tgBoundingSphere* s) const;
	
	bool IntersectRay (const tgVector3& R0, const tgVector3& end, tgVector3& cP) const;
	
	void Normalize ();
	void Negate ();
	void FindType();
	//DEBUG
	tgVector3 v[3];
	tgVector3 vNormal;
	float fD;
	int iType;
};

#include "bvolume/tgaaboundingbox.h"

inline void tgPlane::SetFromPoints (const tgVector3& v1, const tgVector3& v2, const tgVector3& v3)
{
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
	
	vNormal = (v2-v1)%(v3-v1);
	vNormal.Normalize();
	
	fD = vNormal*v1*(-1);
}

inline void tgPlane::SetFromPointAndNormal (const tgVector3& point, const tgVector3& normal)
{
	vNormal = normal;
	fD = -(vNormal*point);
}

inline void tgPlane::FindType ()
{
	if((vNormal.x < 1+EPSILON) && (vNormal.x > 1-EPSILON)) {
		iType = PLANE_X;
		return;
	}
	if((vNormal.y < 1+EPSILON) && (vNormal.y > 1-EPSILON)) {
		iType = PLANE_Y;
		return;
	}
	if((vNormal.z < 1+EPSILON) && (vNormal.z > 1-EPSILON)){
		iType = PLANE_Z;
		return;
	}
	
	float ax = fabs(vNormal.x);
	float ay = fabs(vNormal.y);
	float az = fabs(vNormal.z);
	
	if(ax >= ay && ax >= az) {
		iType = PLANE_ANYX;
		return;
	}
	if (ay >= ax && ay >= az) {
		iType = PLANE_ANYY;
		return;
	}
	iType = PLANE_ANYZ;
}

inline void tgPlane::Normalize()
{
	float magn = vNormal.Length();
	vNormal.Normalize();
	fD /= magn;
}

inline void tgPlane::Negate()
{
	fD = -fD;
	vNormal.Negate();
}

inline void tgPlane::CalculatePoints ()
{
	tgVector3 vec1, vec2, vec3;
	tgVector3::PerpendicularVector(vec1, vNormal);
	vec2 = vec1%vNormal;
	vec3 = vec1+vec2;
	//FIXME: La méthode CalculatePoints ne fonctionnant pas correctement, il ne faut PAS redéfinir le plan. Ca provoque des bugs :)
	//SetFromPoints(vec1, vec2, vec3);
	//SetFromPoints(tgVector3(-fD/vNormal.x, 0, 0), tgVector3(0, -fD/vNormal.y, 0), tgVector3(0, -fD/vNormal.z, 0));
}

inline float tgPlane::GetDistance (const tgVector3& v) const
{
	return v*vNormal + fD;
}

inline int tgPlane::ClassifyPoint (const tgVector3& v) const
{
	float distance = GetDistance(v);
	
	if (distance > EPSILON)
		return POINT_IN_FRONT_OF_PLANE;
	if (distance < EPSILON)
		return POINT_BEHIND_PLANE;
	
	return POINT_ON_PLANE;
}


inline int tgPlane::ClassifyPolygon(tgVector3* points, int num) const
{
	if(num < 1)
		return POLY_INTERSECT_PLANE;
	if (num == 1)
		return ClassifyPoint(points[0]);
		
	int side = ClassifyPoint(points[0]);
	for(int i=1; i<num; i++) {
		if(ClassifyPoint(points[i]) != side)
			return POLY_INTERSECT_PLANE;
	}
	if (side == POINT_BEHIND_PLANE)
		return POLY_BEHIND_PLANE;
		
	return POLY_IN_FRONT_OF_PLANE;
}


/*
inline int tgPlane::ClassifyBox (const tgVector3& vMin, const tgVector3& vMax) const
{
	float dist1, dist2;
	int sides;
	
	int signbits = 0;
	
	//signbits = (vNormal.x < 0) ? 1 : 0 | (vNormal.y < 0) ? 1<<1 : 0 | (vNormal.z < 0) ? 1<<2 : 0; //signbits = signx + (signy<<1) + (signz<<2)
	if (vNormal.x < 0) signbits |= 1 << 0;
	if (vNormal.y < 0) signbits |= 1 << 1;
	if (vNormal.z < 0) signbits |= 1 << 2;
	
	
	switch (signbits) {
		case 0:
			// 000 -> PPP
			dist1 = vNormal.x*vMax.x + vNormal.y*vMax.y + vNormal.z*vMax.z;
			dist2 = vNormal.x*vMin.x + vNormal.y*vMin.y + vNormal.z*vMin.z;
			break;
		case 1:
			// 001 -> PPN
			dist1 = vNormal.x*vMin.x + vNormal.y*vMax.y + vNormal.z*vMax.z;
			dist2 = vNormal.x*vMax.x + vNormal.y*vMin.y + vNormal.z*vMin.z;
			break;
		case 2:
			// 010 -> PNP
			dist1 = vNormal.x*vMax.x + vNormal.y*vMin.y + vNormal.z*vMax.z;
			dist2 = vNormal.x*vMin.x + vNormal.y*vMax.y + vNormal.z*vMin.z;
			break;
		case 3:
			// 011 -> PNN
			dist1 = vNormal.x*vMin.x + vNormal.y*vMin.y + vNormal.z*vMax.z;
			dist2 = vNormal.x*vMax.x + vNormal.y*vMax.y + vNormal.z*vMin.z;
			break;
		case 4:
			// 100 -> NPP
			dist1 = vNormal.x*vMax.x + vNormal.y*vMax.y + vNormal.z*vMin.z;
			dist2 = vNormal.x*vMin.x + vNormal.y*vMin.y + vNormal.z*vMax.z;
			break;
		case 5:
			// 101 -> NPN
			dist1 = vNormal.x*vMin.x + vNormal.y*vMax.y + vNormal.z*vMin.z;
			dist2 = vNormal.x*vMax.x + vNormal.y*vMin.y + vNormal.z*vMax.z;
			break;
		case 6:
			// 110 -> NNP
			dist1 = vNormal.x*vMax.x + vNormal.y*vMin.y + vNormal.z*vMin.z;
			dist2 = vNormal.x*vMin.x + vNormal.y*vMax.y + vNormal.z*vMax.z;
			break;
		case 7:
			// 111 -> NNN
			dist1 = vNormal.x*vMin.x + vNormal.y*vMin.y + vNormal.z*vMin.z;
			dist2 = vNormal.x*vMax.x + vNormal.y*vMax.y + vNormal.z*vMax.z;
			break;
		default:
			dist1 = dist2 = 0;		// shut up compiler
			break;
	}
	
	sides = 0;
	if (dist1 >= fD) sides = 1;
	if (dist2 < fD) sides |= 2;

	return sides;
	
}*/

inline bool tgPlane::IntersectRay (const tgVector3& R0, const tgVector3& end, tgVector3& cP) const
{
	tgVector3 V = end - R0;
	//TODO: S'assuer que CalculatePoints fonctionne normalement. Si cette fonction est uniquement utilisée avec des patchs c'est
	// bon parce que les patchs sont crées via SetFromPoints, mais si on la rend plus général ça va plus.
	float t = (vNormal*(v[0] - R0))/(vNormal*V);
	cP = R0 + V*t;
	if (t >= 0 && t <= 1)
		return true;
	else
		return false;
}

#endif
