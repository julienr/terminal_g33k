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
#ifndef TGTRIANGLE_H
#define TGTRIANGLE_H

#include "tgmath.h"
#include "../tgplane.h"

class tgTriangle 
{
 public:
	tgTriangle () { Set(tgVector3(0,0,0), tgVector3(0,0,0), tgVector3(0,0,0)); }
	tgTriangle (const tgVector3& p1, const tgVector3& p2, const tgVector3& p3) { Set(p1,p2,p3); }
	
	void Set(const tgVector3& p1, const tgVector3& p2, const tgVector3& p3){
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		plane.SetFromPoints(p1,p2,p3);
	}
	bool PointInside (const tgVector3& p) const {
		return ((((points[2]-points[1])%(p-points[1])) * ((points[2]-points[1])%(points[0]-points[1]))) >= 0 && 
			(((points[2]-points[0])%(p-points[0])) * ((points[2]-points[0])%(points[1]-points[0]))) >= 0 && 
			(((points[1]-points[0])%(p-points[0])) * ((points[1]-points[0])%(points[2]-points[0]))) >= 0);
	}
	
	tgPlane GetPlane() const { return plane; }
	tgVector3 GetPoint(int i) const { if(i>0 && i<3) return points[i]; else return tgVector3(0,0,0); }
 protected:
	tgVector3 points[3];
	tgPlane plane;
};

#endif 
