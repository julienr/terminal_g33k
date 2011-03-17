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
#ifndef TGAABOUNDINGBOX_H
#define TGAABOUNDINGBOX_H


#include "tgboundingvolume.h"
#include "tgboundingsphere.h"
#include "../tgplane.h"
#include <vector>
#define tgvector std::vector

class tgAABoundingBox : public tgBoundingVolume
{
 public:
	inline tgAABoundingBox() : vMin(0,0,0), vMax(0,0,0), vPosition(0,0,0), vWorldMin(0,0,0), vWorldMax(0,0,0) { 
		_Initialize();
		_CalculatePlanes();
		eType = TG_AABBOX;
	}
	inline tgAABoundingBox(const tgVector3& min, const tgVector3& max) : vMin(min), vMax(max), vPosition(0,0,0) { 
		_Initialize();
		_CalculateWorldMinMax();
		_CalculatePlanes ();
		eType = TG_AABBOX;
	}
	inline tgAABoundingBox(const tgVector3& min, const tgVector3& max, const tgVector3& pos) : vMin(min), vMax(max), vPosition(pos) { 
		_Initialize();
		_CalculateWorldMinMax();
		_CalculatePlanes (); 
		eType = TG_AABBOX;
	}

  inline tgAABoundingBox(const tgAABoundingBox* bbox) : vMin(bbox->vMin),vMax(bbox->vMax),vPosition(bbox->vPosition) {
 		_Initialize();
		_CalculateWorldMinMax();
		_CalculatePlanes (); 
		eType = TG_AABBOX;
  }
	
	virtual ~tgAABoundingBox() { }
	
	//Heritées de tgBoundingVolume
	inline void SetPosition(const tgVector3& pos);
	inline tgVector3 GetPosition() const;
	bool Trace(tgCollisionResult& result, const tgVector3& begin,const tgVector3& end, tgBoundingVolume* volume=NULL/*, PhysicNode* who*/);
	inline bool PointInside (const tgVector3& p);
	bool Collide (tgAABoundingBox* other);
	bool VolumeInside (tgBoundingVolume* volume, const tgVector3& Start, const tgVector3& End);
	bool VolumeInside (tgBoundingVolume* volume);
	//Fonctions particulières
	inline tgVector3 GetMin () const;
	inline tgVector3 GetMax () const;
	
	inline tgVector3 GetWorldMin () const;
	inline tgVector3 GetWorldMax () const;
	
	inline tgVector3 GetExtents() const;
	
	inline void SetMin(const tgVector3& v);
	inline void SetMax(const tgVector3& v);
	
	inline const tgVector3& GetAxis(int i) const;
	
	inline void GetVertices(tgVector3 corners[8]);
	
	// Cette fonction ne marche pas !!!!!!
	inline tgPlane GetPlane(int i) { return pPlanes[i]; }
	
	// Ajoute le point à la bounding box et étend si nécessaire la bbox
	//inline void AddPoint(const tgVector3& v);
	
	// TODO: à remettre en protected quand la fonction ci-dessus remarchera
	//tgPlane pPlanes[6];
	inline void AddChildren(tgAABoundingBox* child);
	inline tgAABoundingBox* GetChildren (int i);
	inline int GetChildrensCount();

 protected:
	inline void _CalculatePlanes ();
	inline void _CalculateWorldMinMax ();
	inline bool _ClipBox (const tgVector3& E, const tgVector3& P, const tgVector3& D, float& t0, float& t1);
	inline bool _Clip(float denom, float numer, float& t0, float& t1);
	
	inline void _SetParent(tgAABoundingBox* parent);
	inline void _Initialize();
	
	tgVector3 vMin;
	tgVector3 vMax;
	tgVector3 vPosition;
	
	tgVector3 vWorldMin;
	tgVector3 vWorldMax;
	tgPlane pPlanes[6];
	
	tgVector3 pAxis[3];
	tgVector3 vExtents;
	
	tgvector<tgAABoundingBox*> childrens;
	tgAABoundingBox* pParent;
};

void tgAABoundingBox::_SetParent(tgAABoundingBox* parent)
{
	pParent = parent;
}

const tgVector3& tgAABoundingBox::GetAxis(int i) const
{
	return pAxis[i];
}

void tgAABoundingBox::AddChildren(tgAABoundingBox* child)
{
	childrens.push_back(child);
}

tgAABoundingBox* tgAABoundingBox::GetChildren (int i)
{
	return childrens[i];
}

int tgAABoundingBox::GetChildrensCount()
{
	return (int)childrens.size();
}

void tgAABoundingBox::_Initialize()
{
	pParent = NULL;
	childrens.clear();
	//BOX axis-aligned
	pAxis[0].Set(1,0,0);
	pAxis[1].Set(0,1,0);
	pAxis[2].Set(0,0,1);
}

tgVector3 tgAABoundingBox::GetMin() const
{
	return vMin;
}

tgVector3 tgAABoundingBox::GetMax() const
{
	return vMax;
}

tgVector3 tgAABoundingBox::GetWorldMin () const
{
	return vWorldMin;
}

tgVector3 tgAABoundingBox::GetWorldMax () const
{
	return vWorldMax;
}

tgVector3 tgAABoundingBox::GetExtents() const
{
	return vExtents;
}

void tgAABoundingBox::SetPosition(const tgVector3& pos)
{
	vPosition = pos;
	_CalculateWorldMinMax();
	//TODO: on ne doit pas forcément _tout_ recalculer, c'est juste une translation
	_CalculatePlanes ();
}

tgVector3 tgAABoundingBox::GetPosition() const
{
	return vPosition;
}

void tgAABoundingBox::SetMin(const tgVector3& v)
{
	vMin = v;
	_CalculateWorldMinMax();
	_CalculatePlanes ();
}

void tgAABoundingBox::SetMax(const tgVector3& v)
{
	vMax = v;
	_CalculateWorldMinMax();
	_CalculatePlanes ();
}

void tgAABoundingBox::_CalculateWorldMinMax ()
{
	vWorldMin = vMin + vPosition;
	vWorldMax = vMax + vPosition;
	//FIXME: L'extent DOIT etre en OBJECT SPACE !
	//mais ca cree des bugs dans les deplacements apparement
	/*vExtents.x = -vWorldMin.x > vWorldMax.x ? -vWorldMin.x : vWorldMax.x;
	vExtents.y = -vWorldMin.y > vWorldMax.y ? -vWorldMin.y : vWorldMax.y;
	vExtents.z = -vWorldMin.z > vWorldMax.z ? -vWorldMin.z : vWorldMax.z;*/
	//l'extent est en object space
	vExtents.x = -vMin.x > vMax.x ? -vMin.x : vMax.x;
	vExtents.y = -vMin.y > vMax.y ? -vMin.y : vMax.y;
	vExtents.z = -vMin.z > vMax.z ? -vMin.z : vMax.z;
}

/*void tgAABoundingBox::AddPoint(const tgVector3& v)
{
	if (v.x < vMin.x)
		vMin.x = v.x;
	if (v.y < vMin.y)
		vMin.y = v.y;
	if (v.z < vMin.z)
		vMin.z = v.z;
		
	if (v.x > vMax.x)
		vMax.x = v.x;
	if (v.y > vMax.y)
		vMax.y = v.y;
	if (v.z > vMax.z)
		vMax.z = v.z;
	
		
	_CalculateWorldMinMax();
	_CalculatePlanes();
}*/

void tgAABoundingBox::_CalculatePlanes()
{
	const tgVector3& vi = vWorldMin;
	const tgVector3& va = vWorldMax;
	
	pPlanes[0].SetFromPoints(tgVector3(va.x, vi.y, vi.z),vi, tgVector3(va.x, va.y, vi.z));
	pPlanes[1].SetFromPoints(va,tgVector3(va.x, vi.y, va.z), tgVector3(va.x, va.y, vi.z));
	pPlanes[2].SetFromPoints(va, tgVector3(va.x, va.y, vi.z), tgVector3(vi.x, va.y, va.z));
	pPlanes[3].SetFromPoints(tgVector3(vi.x, va.y, vi.z), vi, tgVector3(vi.x, va.y, va.z));
	pPlanes[4].SetFromPoints(tgVector3(vi.x, vi.y, va.z), tgVector3(va.x, vi.y, va.z), va);
	pPlanes[5].SetFromPoints( tgVector3(vi.x, vi.y, va.z), vi, tgVector3(va.x, vi.y, va.z));
}

bool tgAABoundingBox::PointInside (const tgVector3& p)
{
	if (p.x > vWorldMin.x && p.y > vWorldMin.y && p.y > vWorldMin.z && p.x < vWorldMax.x && p.y < vWorldMax.y && p.z < vWorldMax.z)
		return true;
	else
		return false;
}
/*
bool tgAABoundingBox::RayInside (const tgVector3& begin, const tgVector3& end)
{
	for(short int i=0; i<6; i++) {
		float startDistance, endDistance;
		startDistance = pPlanes[i].GetDistance(begin);
		endDistance = pPlanes[i].GetDistance(end);
		
		if (startDistance > 0 && endDistance > 0) //les deux points sont à l'extérieur du même plan
			return false;
	}
	return true;
}

bool tgAABoundingBox::BoxInside (const tgVector3& vOtherMin, const tgVector3& vOtherMax,const tgVector3& Start, const tgVector3& End)
{
	for(short int i=0; i<6; i++) {
		tgVector3 offset;
		offset.x = pPlanes[i].vNormal.x < 0 ? vOtherMax.x : vOtherMin.x;
		offset.y = pPlanes[i].vNormal.y < 0 ? vOtherMax.y : vOtherMin.y;
		offset.z = pPlanes[i].vNormal.z < 0 ? vOtherMax.z : vOtherMin.z;
		
		float startDistance, endDistance;
		startDistance = pPlanes[i].GetDistance(Start+offset);
		endDistance = pPlanes[i].GetDistance(End+offset);
		
		if (startDistance > 0 && endDistance > 0) //les deux points sont à l'extérieur du même plan
			return false;
	}
	return true;
}*/

void tgAABoundingBox::GetVertices(tgVector3 corners[8])
{
	corners[0] = vWorldMin;
	corners[1].Set(vWorldMax.x, vWorldMin.y, vWorldMin.z);
	corners[2].Set(vWorldMax.x, vWorldMax.y, vWorldMin.z);
	corners[3].Set(vWorldMin.x, vWorldMax.y, vWorldMin.z);
	corners[4].Set(vWorldMin.x, vWorldMin.y, vWorldMax.z);
	corners[5].Set(vWorldMin.x, vWorldMax.y, vWorldMax.z);
	corners[6] = vWorldMax;
	corners[7].Set(vWorldMax.x, vWorldMin.y, vWorldMax.z);
}


#endif

