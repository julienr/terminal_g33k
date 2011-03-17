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
#include "tgplane.h"
#include "math/tgmatrix3.h"
/*#include <iostream>
using namespace std;*/

int tgPlane::ClassifyBox (tgAABoundingBox* box) const
{
	tgVector3 corners[8];
	box->GetVertices(corners);
	int side = ClassifyPoint(corners[0]);
	for(int i=1; i<8; i++) {
		if (ClassifyPoint(corners[i]) != side)
			return BOX_INTERSECT_PLANE;
	}
	if(side == POINT_BEHIND_PLANE)
		return BOX_BEHIND_PLANE;
		
	return BOX_IN_FRONT_OF_PLANE;

}


tgVector3 tgPlane::ProjectVector(const tgVector3& in)
{
	//vNormal est de longueur unitaire, on utilise la methode simplifiee
	vNormal.Normalize();
	tgMatrix3 mat;
	mat[0][0] = 1.0f-vNormal.x*vNormal.x;
	mat[0][1] = -vNormal.x*vNormal.y;
	mat[0][2] = -vNormal.x*vNormal.z;
	mat[1][0] = -vNormal.y*vNormal.x;
	mat[1][1] = 1.0f-vNormal.y*vNormal.y;
	mat[1][2] = -vNormal.y*vNormal.z;
	mat[2][0] = -vNormal.z*vNormal.x;
	mat[2][1] = -vNormal.z*vNormal.y;
	mat[2][2] = 1.0f-vNormal.z*vNormal.z;
	return mat*in;
}


tgVector3 tgPlane::ProjectPoint(const tgVector3& in)
{
	tgVector3 tmp = ProjectVector(in);
	tmp -= vNormal*fD;
	return tmp;

}


//FIXME: Version non-optimisée ;)
int tgPlane::ClassifyBox(const tgVector3& vMin, const tgVector3& vMax) const
{
	tgAABoundingBox* temp = new tgAABoundingBox(vMin, vMax);
	int ret = ClassifyBox(temp);
	delete temp;
	return ret;
}

int tgPlane::ClassifySphere (const tgBoundingSphere* s) const
{
	float distance = GetDistance(s->GetPosition());
	if (distance > s->GetRadius())
		return SPHERE_IN_FRONT_OF_PLANE;
	else if (distance < -(s->GetRadius()))
		return SPHERE_BEHIND_PLANE;
	else
		return SPHERE_INTERSECT_PLANE;
}


