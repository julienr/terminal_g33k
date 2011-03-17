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
#ifndef TGBOUNDINGVOLUME_H
#define TGBOUNDINGVOLUME_H

#include "../math/tgvector3.h"


enum eBVolumeTypes
{
	TG_BSPHERE,
	TG_AABBOX
};

/**
 * Structure regroupant des informations sur les "trace"
 */
class tgIEntity;
struct tgCollisionResult
{
	float fFraction;
	tgVector3 vEndPoint;
	tgVector3 vColNormal;
	bool bStartsOut;
	bool bAllSolid;
	tgIEntity* pEntity;
	//FIXME ugly hack to avoid bezier bugs
	bool bBezier;
};

class tgBoundingVolume 
{
 public:
	virtual ~tgBoundingVolume() {}
	eBVolumeTypes GetType () const { return eType; }
	virtual void SetPosition(const tgVector3& pos) = 0;
	virtual tgVector3 GetPosition() const = 0;
	virtual bool Trace(tgCollisionResult& result, const tgVector3& begin,const tgVector3& end, tgBoundingVolume* volume=NULL/*, PhysicNode* who*/) = 0;
	virtual bool PointInside (const tgVector3& p) = 0;
	virtual bool VolumeInside (tgBoundingVolume* volume, const tgVector3& Start, const tgVector3& End) = 0;
  virtual bool VolumeInside (tgBoundingVolume* volume) = 0;
 protected:
	eBVolumeTypes eType;
};

#endif
