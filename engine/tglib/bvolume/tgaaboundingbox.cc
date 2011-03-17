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


#include "tgaaboundingbox.h"


bool tgAABoundingBox::Trace(tgCollisionResult& result, const tgVector3& begin, const tgVector3& end, tgBoundingVolume* volume)
{
	float startFraction = -1.0f;
	float endFraction = 1.0f;
	float startDistance=0, endDistance=0;
	bool bStartsOut = false;
	bool bEndsOut = false;
	int colPlane=-1;
	
	for (int j=0;j<6;j++) {
		if(volume == NULL) { //on trace avec un rayon
			startDistance = pPlanes[j].GetDistance(begin);
			endDistance = pPlanes[j].GetDistance(end);
		} else if (volume->GetType() == TG_AABBOX) {
			tgVector3 offset;
			tgAABoundingBox* box = static_cast<tgAABoundingBox*>(volume);
			offset.x = pPlanes[j].vNormal.x < 0 ? box->GetMax().x : box->GetMin().x;
			offset.y = pPlanes[j].vNormal.y < 0 ? box->GetMax().y : box->GetMin().y;
			offset.z = pPlanes[j].vNormal.z < 0 ? box->GetMax().z : box->GetMin().z;
			startDistance = pPlanes[j].GetDistance(begin+offset);
			endDistance = pPlanes[j].GetDistance(end+offset);
		} else if (volume->GetType() == TG_BSPHERE) {
			tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(volume);
			startDistance = pPlanes[j].GetDistance(begin) + sphere->GetRadius();
			endDistance = pPlanes[j].GetDistance(end) + sphere->GetRadius();
		} else {
			return false;
		}

		if (startDistance > 0 ) bStartsOut = true;
		if (endDistance > 0 ) bEndsOut = true;
		
		if ((startDistance > 0) && (endDistance > 0)) {//les deux sont devant le plan, donc dehors
			return false;
		}
		
		if ((startDistance <= 0) && (endDistance <= 0)) //les deux sont dedans, seront coupé par un autre plan
			continue;

		if (startDistance>endDistance) {
			float fraction = (startDistance - EPSILON)/(startDistance - endDistance);
			if (fraction > startFraction) {
				startFraction = fraction;
				colPlane = j;
			}
		}
		else {
			float fraction = (startDistance + EPSILON)/(startDistance - endDistance);
			if (fraction < endFraction) {
				endFraction = fraction;
			}
		}
	}
	
	if (bStartsOut == false) {
		result.bStartsOut = false;

		if (bEndsOut == false)
			result.bAllSolid = true;
		return true;
	}

	if (startFraction < endFraction) {
		if (startFraction > -1 && startFraction < result.fFraction) {
			// si on met startFraction = 0 ça pose des problèmes 
			// la, on a juste le cas ou on est dans la boite ou la collision sera ignorée
				if (startFraction < 0) return false;
				result.fFraction = startFraction;
			
				if (bStartsOut == false)
					if (bEndsOut == false)
						result.bAllSolid = true;
				result.bStartsOut = bStartsOut;
			
				if (colPlane != -1)
					result.vColNormal = pPlanes[colPlane].vNormal;
				else
					result.vColNormal.Set(0,0,0);
				
				return true;
		}
	}
	return false;
}

bool tgAABoundingBox::Collide (tgAABoundingBox* other)
{
	//FIXME: c'est faux => recoder avec SAT 
	/*
	dans ce cas-la, ca foire, aucune vertice n'overlap
   +--+
   ||
+---+  +---+
|   :  :   |
+---+  +---+
   ||
   +--+
	*/
	if(vWorldMin.x > other->vWorldMax.x || other->vWorldMin.x > vWorldMax.x) return false;
	if(vWorldMin.y > other->vWorldMax.y || other->vWorldMin.y > vWorldMax.y) return false;
	if(vWorldMin.z > other->vWorldMax.z || other->vWorldMin.z > vWorldMax.z) return false;
	return true;
}

bool tgAABoundingBox::VolumeInside (tgBoundingVolume* volume, const tgVector3& Start, const tgVector3& End)
{
	for(short int i=0; i<6; i++) {
		float startDistance, endDistance;
		if(volume == NULL) {
			startDistance = pPlanes[i].GetDistance(Start);
			endDistance = pPlanes[i].GetDistance(End);
			
		} else if (volume->GetType() == TG_AABBOX) {
			tgAABoundingBox* box = static_cast<tgAABoundingBox*>(volume);
			tgVector3 offset;
			offset.x = pPlanes[i].vNormal.x < 0 ? box->GetMax().x : box->GetMin().x;
			offset.y = pPlanes[i].vNormal.y < 0 ? box->GetMax().y : box->GetMin().y;
			offset.z = pPlanes[i].vNormal.z < 0 ? box->GetMax().z : box->GetMin().z;
			startDistance = pPlanes[i].GetDistance(Start+offset);
			endDistance = pPlanes[i].GetDistance(End+offset);
			
		} else if (volume->GetType() == TG_BSPHERE) {
			tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(volume);
			startDistance = pPlanes[i].GetDistance(Start) + sphere->GetRadius();
			endDistance = pPlanes[i].GetDistance(End) + sphere->GetRadius();
			
		} else {
			//Probleme
			return false;
		}
		
		if (startDistance > 0 && endDistance > 0) //les deux points sont à l'extérieur du même plan
			return false;
	}
	return true;
}

bool tgAABoundingBox::VolumeInside (tgBoundingVolume* volume)
{
	if (volume->GetType() == TG_BSPHERE) {
		/* J. Arvo. A simple method for box-sphere intersection testing. 
			 In A. Glassner, editor, Graphics Gems, pp. 335-339, Academic Press, Boston, MA, 1990. */
		tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(volume);
		float s,d = 0;
		for(int i=0; i<3; i++) {
			if (sphere->GetPosition()[i] < vWorldMin[i]) {
				s = sphere->GetPosition()[i] - vWorldMin[i];
				d += s*s;
			} else if (sphere->GetPosition()[i] > vWorldMax[i]) {
				s = sphere->GetPosition()[i] - vWorldMax[i];
				d += s*s;
			}
		}
		return d <= sphere->GetRadius()*sphere->GetRadius();
	} else {
  	//FIXME: optimise
  	return VolumeInside(volume, vPosition, vPosition);
	}
}

