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
#ifndef TGGEOM2D_H
#define TGGEOM2D_H

#include "tgvector2.h"
#include <iostream>
using namespace std;
/**
* une serie de classes pour stocket des figures geometriques a deux dimension
*/


//BEGIN tgRect2D
class tgRect2D
{
 public:
 	tgRect2D() : vPosition(0,0), vMin(0,0), vMax(0,0) { _CalculateWorldMinMax();}
 	
 	const tgVector2& GetPosition() const { return vPosition; }
 	const tgVector2& GetMin() const { return vMin; }
 	const tgVector2& GetMax() const { return vMax; }
 	const tgVector2& GetWorldMin() const { return vWorldMin; }
 	const tgVector2& GetWorldMax() const { return vWorldMax; }
 	tgVector2* GetPoints() { return pPoints; }
 	
 	void SetPosition(const tgVector2& v) { vPosition = v; _CalculateWorldMinMax(); }
 	void SetMin(const tgVector2& v) { vMin = v; _CalculateWorldMinMax(); }
 	void SetMax(const tgVector2& v) { vMax = v; _CalculateWorldMinMax(); }
 	
 protected:
	inline void _CalculateWorldMinMax();
	tgVector2 vPosition;
	tgVector2 vMin;
	tgVector2 vMax;
	tgVector2 vWorldMin;
	tgVector2 vWorldMax;
	tgVector2 pPoints[4];
};

void tgRect2D::_CalculateWorldMinMax()
{
	vWorldMin = vMin + vPosition;
	vWorldMax = vMax + vPosition;
	
	pPoints[0] = vWorldMin;
	pPoints[1].Set(vWorldMin.x,vWorldMax.y);
	pPoints[2] = vWorldMax;
	pPoints[3].Set(vWorldMax.x,vWorldMin.y);
}

//END tgRect2D

//BEGIN tgTriangle2D

class tgTriangle2D
{
 public:
 	tgTriangle2D() {}
 	void Set(const tgVector2& a, const tgVector2& b, const tgVector2& c) { v[0]=a;v[1]=b;v[2]=c; }
 	tgVector2* GetPoints() { return v; }
 	const tgVector2 GetPoint(unsigned short i) { return v[i]; }
 	
 protected:
 	tgVector2 v[3];
};

//END tgTriangle2D

//BEGIN tgLine2D
class tgLine2D
{
 public:
	tgLine2D(): start(0,0),end(0,0) {}
	void Set(const tgVector2& s, const tgVector2& e) { start = s; end = e; }
	const tgVector2& GetStart() { return start; }
	const tgVector2& GetEnd() { return end; }
	
 protected:
 	tgVector2 start, end;
};
//END tgLine2D


#endif
