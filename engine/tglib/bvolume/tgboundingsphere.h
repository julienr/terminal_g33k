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
#ifndef TGBOUNDINGSPHERE_H
#define TGBOUNDINGSPHERE_H

#include "tgboundingvolume.h"
#include "tgaaboundingbox.h"
#include "../tgplane.h"
#include <vector>
#define tgvector std::vector

class tgBoundingSphere : public tgBoundingVolume
{
 public:
	tgBoundingSphere() : vPosition(0,0,0), fRadius(0) {eType = TG_BSPHERE;}
	tgBoundingSphere(const tgVector3& pos, float rad) : vPosition(pos), fRadius(rad) { eType = TG_BSPHERE;}
  tgBoundingSphere(tgBoundingSphere* sphere) : vPosition(sphere->vPosition), fRadius(sphere->fRadius) { eType = TG_BSPHERE; }
	virtual ~tgBoundingSphere() {}
	//Heritées de tgBoundingVolume
 	inline void SetPosition(const tgVector3& pos);
	inline tgVector3 GetPosition () const;
	bool Trace(tgCollisionResult& result, const tgVector3& begin,const tgVector3& end, tgBoundingVolume* volume=NULL/*, PhysicNode* who*/);
	inline bool PointInside (const tgVector3& p);
	bool VolumeInside (tgBoundingVolume* volume);
	bool VolumeInside (tgBoundingVolume* volume, const tgVector3& Start, const tgVector3& End);
	//Fonction particulières
	inline void SetRadius(float r);
	inline float GetRadius () const;
 protected:
	tgVector3 vPosition;
	float fRadius;
};

void tgBoundingSphere::SetPosition(const tgVector3& pos)
{
	vPosition = pos;
}

tgVector3 tgBoundingSphere::GetPosition() const
{
	return vPosition;
}

void tgBoundingSphere::SetRadius(float r)
{
	fRadius = r;
}

float tgBoundingSphere::GetRadius () const
{
	return fRadius;
}

bool tgBoundingSphere::PointInside (const tgVector3& p)
{
	if((p-vPosition).Length() <= fRadius)
		return true;
	
	return false;
}

#endif
