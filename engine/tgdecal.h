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
#ifndef TGDECAL_H
#define TGDECAL_H

#include "tgdefs.h"

class tgMaterial;
class tgDecal
{
public:
	tgDecal(tgMaterial* mat, const tgVector3& norm);
	~tgDecal();
	/**
	* Ajoute une face triangulaire en prenant en argument les 3 points qui la composent
	*/
	void AddTriangleFace(const tgVector3& v1, const tgVector3& v2, const tgVector3& v3);
	//sans effet
	void AddPlane(const tgPlane& p);
	
	void SetBox(tgAABoundingBox box) { BBox = box;}
	void Clip();
	
	tgMaterial* GetMaterial() { return pMaterial; }

	unsigned int GetNumTriangleFaces () { return iNumFaces; }
	unsigned int GetNumPoints();
	tgVertice& GetVertice(unsigned int i) { return points[i]; }
protected:
	void _ClipPolygonAgainstPlane(const tgPlane& plane, const tgvector<tgVertice>& vertex, tgvector<tgVertice>& result);
	struct _Polygon {
		tgvector<tgVertice> pts;
		tgPlane plane;
	};
	tgvector<_Polygon> polyList;
	tgvector<tgVertice> points;
	tgvector<tgPlane> supPlanes;
	unsigned int iNumFaces;
	tgAABoundingBox BBox;
	tgMaterial* pMaterial;
	tgVector3 vDecalNormal;
	tgVector3 vDecalS;
	tgVector3 vDecalT;
};

#endif
