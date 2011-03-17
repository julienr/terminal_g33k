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
#include "tgdecal.h"

tgDecal::tgDecal(tgMaterial* mat, const tgVector3& norm)
{
	pMaterial = mat;
	vDecalNormal = norm;
	tgVector3::PerpendicularVector(vDecalS, vDecalNormal);
	vDecalT = vDecalS%vDecalNormal;
	vDecalT.Normalize();
	vDecalS.Normalize();
  iNumFaces = 0;
}

tgDecal::~tgDecal()
{
	points.clear();
}

void tgDecal::AddTriangleFace(const tgVector3& v1, const tgVector3& v2, const tgVector3& v3)
{
	_Polygon pol;
	//FIXME: on 'pousse' les points en-dehors de la face pour qu'ils soient dessines devant et eviter les artefacts avec la face original
	//cependant, il faudrait reprendre direct le plan depuis la face originale, plutot que de recalculer
	//Ca cree des bugs sur les escaliers en plus, parce que le plan n'est pas bien pose sur l'escalier, on voit donc un espace entre l'escalier et le decal
	pol.plane.SetFromPoints(v1,v3,v2);
	
	pol.pts.push_back(tgVertice(v1+(pol.plane.vNormal/20), pol.plane.vNormal));
	pol.pts.push_back(tgVertice(v2+(pol.plane.vNormal/20), pol.plane.vNormal));
	pol.pts.push_back(tgVertice(v3+(pol.plane.vNormal/20), pol.plane.vNormal));
	polyList.push_back(pol);
	iNumFaces++;
}


void tgDecal::AddPlane(const tgPlane& p)
{
	supPlanes.push_back(p);
}

/**
* Sutherland-Hodgman Polygon Clipping
* http://www.codeguru.com/Cpp/misc/misc/graphics/article.php/c8965/
*/
enum{
	inside = 0,
	outside = 1
};
void tgDecal::_ClipPolygonAgainstPlane(const tgPlane& plane, const tgvector<tgVertice>& vertex, tgvector<tgVertice>& result)
{
	unsigned int size = (unsigned int)vertex.size();
	for(unsigned int i=0; i<size; i++) {
    //on travaille en modulo size
		int prev = (i==0)?size-1:i-1;
		int loc_current = (plane.ClassifyPoint(vertex[i].vPosition) == POINT_IN_FRONT_OF_PLANE) ?outside:inside;
		int loc_prev = (plane.ClassifyPoint(vertex[prev].vPosition) == POINT_IN_FRONT_OF_PLANE) ?outside:inside;
		if(loc_prev == inside && loc_current == inside) {
			result.push_back(vertex[i]);
		} else if (loc_prev == inside && loc_current == outside) {
			tgVector3 cP;
      //point d'intersection
			plane.IntersectRay(vertex[prev].vPosition, vertex[i].vPosition, cP);
			result.push_back(tgVertice(cP,vertex[i].vNormal));
		} else if (loc_prev == outside && loc_current == outside) {
      //do nothing
		} else if (loc_prev == outside && loc_current == inside) {
			tgVector3 cP;
			plane.IntersectRay(vertex[prev].vPosition, vertex[i].vPosition, cP);
			result.push_back(tgVertice(cP,vertex[i].vNormal));
			result.push_back(vertex[i]);
		}
	}
}

bool VecAlmostEqual(const tgVector3& v1, const tgVector3& v2) 
{
	bool yes = true;
	for(short i=0; i<3; i++) {
		if (!((v1[i] < v2[i]+EPSILON) && (v1[i] > v2[i]-EPSILON)))
			yes = false;
	}
	return yes;
}

void Clamp(float& val, float min, float max) 
{
	if(val < min)
		val = min;
	if(val > max)
		val = max;
}


void tgDecal::Clip()
{
	
	points.clear();
	//on rajoute la box aux autres eventuels plan de clipping
	for(unsigned short i=0; i<6; i++) {
		supPlanes.push_back(BBox.GetPlane(i));
	}
	//On commence par clipper les polygones contre les plans de clipping
	for(unsigned short i=0; i<polyList.size(); i++) { //pour chaque polygone
		_Polygon& poly = polyList[i];
		
		for(unsigned short j=0; j<supPlanes.size(); j++) { //pour chaque plan de clipping
			tgPlane p = supPlanes[j];
			tgvector<tgVertice> result;
			_ClipPolygonAgainstPlane(p, poly.pts, result);
			poly.pts.clear();
			
			for(unsigned short k=0; k<result.size(); k++) {
				poly.pts.push_back(result[k]);
			}
		}
		if(poly.pts.size() > 0) {
			for(unsigned short j=1;j<poly.pts.size()-1; j++) {
				points.push_back(poly.pts[0]);
				points.push_back(poly.pts[j]);
				points.push_back(poly.pts[j+1]);
			}
		}
	}
	if(points.size() < 1)
		return;
	
	//BEGIN suppression des points derriere le plan du decal en les projetants sur ledit plan
	/*const tgVector3& vb = BBox.GetPosition();
	tgPlane decalPlane(vb, vb+vDecalT, vb+vDecalS);
	for(unsigned short k=0;k<points.size(); k++) {
		//FIXME: valeur arbitraire, devrait dependre de la taille du decal, ca pose probleme sur les escaliers par exemple
		if(decalPlane.GetDistance(points[k].vPosition) < -20) {
			// il vaut mieux laisser un peu de marge
		//if(decalPlane.ClassifyPoint(points[k].vPosition) == POINT_BEHIND_PLANE) {
			points[k].vPosition = decalPlane.ProjectPoint(points[k].vPosition);
		}
}*/
	//END
	
	//TODO: suppression des points doublons
	
	//calcul des texcoords
	tgVector3 vMin = points[0].vPosition;
	tgVector3 vMax = points[0].vPosition;
	//on cherche les coordonnes extremes
	for(unsigned short k=0; k<points.size(); k++) {
		tgVector3::AddPointToBounds(points[k].vPosition, vMin, vMax);
	}
	
	const tgVector3 vDistMax = vMax - vMin;
	const float distMaxLength = vDistMax.Length();
	const tgVector3& vCenter = BBox.GetPosition();
	
	for(unsigned short k=0; k<points.size(); k++) {
		tgVector3 vDist = points[k].vPosition - vCenter;
		//alpha represente l'eloignement sur la normal du decal
		float alpha = (vDecalNormal*vDist)/distMaxLength;
		if (alpha > -0.01 && alpha < 0.01) //marge de tolerance sur alpha
			alpha = 0;
		//cout << alpha << endl;
		if(alpha < 0)
			alpha *= -1;
		
		//eloignement sur les vecteur S et T paralelles au plan du decal
		points[k].s = (vDecalS*vDist)/distMaxLength + 0.5f;
		points[k].t = (vDecalT*vDist)/distMaxLength + 0.5f;
		//alpha *= 1.2;
		//alpha a une influence sur ces deux coordonnes,
		//comme dans notre texture le noir est en 0.5/0.5, il faut que les coords s'eloignent de 0.5/0.5 a mesure que alpha augmente
		points[k].s += (points[k].s >= 0.5)?alpha:-alpha;
		points[k].t += (points[k].t >= 0.5)?alpha:-alpha;
		//si on ne clamp pas, on a des repetitions puisque l'addition d'alpha peut amener la coord au-dela de 1
		Clamp(points[k].s,0,1);
		Clamp(points[k].t,0,1);
	}
}

unsigned int tgDecal::GetNumPoints()
{
	return (unsigned int)points.size();
}
