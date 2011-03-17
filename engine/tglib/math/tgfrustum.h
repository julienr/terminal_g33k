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
#ifndef TGFRUSTUM_H
#define TGFRUSTUM_H

#include "../tgplane.h"
#include "../bvolume/tgaaboundingbox.h"

class tgFrustum 
{
 friend class tgGLRenderer;
 public:
	inline tgFrustum () { pPlanes = new tgPlane[6]; iNumPlanes = 6; }
	inline tgFrustum (int numPlanes) { pPlanes = new tgPlane[numPlanes]; iNumPlanes = numPlanes; }
	inline ~tgFrustum () { delete [] pPlanes; }
	
	void SetPlane(int n, const tgPlane& p) { pPlanes[n] = p; }
	void SetOrigin(const tgVector3& v) { vOrigin = v; }
	tgVector3 GetOrigin () const { return vOrigin; }
	inline bool PointInside(const tgVector3& v) const;
	inline bool BoxInside(tgAABoundingBox* box) const;
	inline bool PolygonInside(tgVector3* points, int num) const;
	
 //protected:
	tgVector3 vOrigin;
 	tgPlane* pPlanes;
	int iNumPlanes;
};


bool tgFrustum::PointInside(const tgVector3& v) const
{
	bool inside = true;
	for(int i=0; i<iNumPlanes; i++) {
		if (pPlanes[i].ClassifyPoint(v) == POINT_BEHIND_PLANE) {
			inside = false;
		}
	}
	return inside;
}

bool tgFrustum::PolygonInside(tgVector3* points, int num) const
{
	for(int i=0; i<iNumPlanes; i++) {
		if(pPlanes[i].ClassifyPolygon(points, num) == POLY_BEHIND_PLANE)
			return false;
	}
	return true;
}

bool tgFrustum::BoxInside(tgAABoundingBox* box) const
{
	//FIXME: A tester
	for(int i=0; i<iNumPlanes; i++) {
		if(pPlanes[i].ClassifyBox(box) == BOX_BEHIND_PLANE) //si la boite est derriere a un plan, elle est forcément en dehors
			return false;
	}
	return true;
	/*
	int iTotalIn = 0;
	for(int i=0; i<iNumPlanes; i++) {
		tgVector3 corners[8];
		box->GetVertices(corners);
		
		int iInCount = 8;
		int iPtIn = 1;
		for(int j=0; j<8; j++) {
			if(pPlanes[i].ClassifyPoint(corners[j]) == POINT_BEHIND_PLANE) {
				iPtIn = 0;
				iInCount--;
			}
		}
		
		if(iInCount == 0)
			return false;
		iTotalIn += iPtIn;
	}
	
	//if(iTotalIn == 6)
	//	return INSIDE;
	return true; */
}

#endif

