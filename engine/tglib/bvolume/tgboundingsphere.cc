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
#include "tgboundingsphere.h"


bool tgBoundingSphere::Trace(tgCollisionResult& result, const tgVector3& begin,const tgVector3& end, tgBoundingVolume* volume/*, PhysicNode* who*/)
{
	if(volume == NULL) {//on a un rayon
		//FIXME : écrire une fonction :P
		/*float tcol;
		//calcul de l'intersection
		tgVector3 D =(end-begin).GetNormalized();
		tcol = vPosition*vPosition + fRadius*fRadius - begin*begin;
		if (tcol < 0)
			return;
		result.fFraction = tcol;
		return;*/
		/*tgVector3 D = (end-begin).GetNormalized();
		tgVector3 EO = vPosition - begin;
		float v = EO*D;
		float disc = fRadius*fRadius - (EO*EO - v*v);
		if (disc<0.0f)//pas de collision
			return;

		float d = sqrt (disc);
		result.fFraction = v-d;
		tgVector3 colPoint = begin+D*(v-d);
		result.vColNormal = (vPosition-colPoint).GetNormalized();
		return;*/
		
	} else if (volume->GetType() == TG_AABBOX) { 
		tgAABoundingBox* box = static_cast<tgAABoundingBox*>(volume);
		/*tgAABoundingBox* box = (tgAABoundingBox*)volume;
		tgAABoundingBox* box = new tgAABoundingBox();*/
		return box->Trace(result, begin,end,this);
		
	} else if (volume->GetType() == TG_BSPHERE) {
		tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(volume);
		//calcul de l'intersection
		tgVector3 D = (end-begin).GetNormalized();
		tgVector3 EO = vPosition - begin;
		float v = EO*D+sphere->GetRadius();
		float disc = fRadius*fRadius - (EO*EO - v*v);
		if (disc<0.0f)//pas de collision
			return false;

		float d = sqrt (disc);
		result.fFraction = v-d;
		tgVector3 colPoint = begin+D*(v-d);
		result.vColNormal = (vPosition-colPoint).GetNormalized();
		return true;
	}
	return false;
}

bool tgBoundingSphere::VolumeInside (tgBoundingVolume* volume, const tgVector3& Start, const tgVector3& End)
{
  //FIXME: optimise
  tgCollisionResult cr;
  return Trace(cr, Start, End, volume);
}

bool tgBoundingSphere::VolumeInside (tgBoundingVolume* volume)
{
	if(volume->GetType() == TG_BSPHERE) {
		tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(volume);
		tgVector3 dist = sphere->GetPosition() - vPosition;
		if(dist.Length() > (sphere->GetRadius() + fRadius))
			return false;
		return true;
	} else if (volume->GetType() == TG_AABBOX) {
		tgAABoundingBox* box = static_cast<tgAABoundingBox*>(volume);
		return box->VolumeInside(this);
	} else {
  	return VolumeInside(volume, vPosition, vPosition);
  }
}


