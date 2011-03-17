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
#if 0
#include "doom3level.h"

tgDoom3Level::tgDoom3Level(const char* procname, const char* mapname)
{
	int file = pFileManager->Open(mapname, TGFILE_TEXT);
	_LoadMap(file);
	pFileManager->Close(file);

	file = pFileManager->Open(procname, TGFILE_TEXT);
	_LoadProc(file);
	pFileManager->Close(file);
	out << "Chargement de la map termine" << endl;
	
	_ClassifyModels();
}

tgDoom3Level::~ tgDoom3Level()
{

}

#define COMMENT	0
#define CODE		1

#define SKIP_COMMENT pFileManager->Read(file, buffer); \
		if (buffer.find("/*") != tgstring::npos) \
			state = COMMENT; \
			 \
		while(state == COMMENT) { \
			pFileManager->Read(file, buffer); \
			if (buffer.find("*/") != tgstring::npos) {\
				state = CODE; \
			}\
		} 
		
#define NEXT(str) while(buffer.find(str)==tgstring::npos && !pFileManager->EndOfFile(file)) \
			pFileManager->Read(file, buffer);
bool tgDoom3Level::_LoadMap(int file)
{
	tgstring buffer;
	iNumEntities = 0;
	while (!pFileManager->EndOfFile(file)) {
		pFileManager->Read(file, buffer);
		if (buffer.find("classname") != tgstring::npos) {
			iNumEntities++;
		}
	}
	out << "entites : " << iNumEntities << endl;
	pEntities = new _Entity[iNumEntities];
	pFileManager->Clear(file);
	pFileManager->Seek(file, 0);

	tgstring prop_name, prop_value;
	char c;
	int state = 0;
	int num_open = 0;
	for (int i=0; i<iNumEntities; i++) {
		//out << "------ ENTITE -------  " << i << endl;
		//ne __PAS__ utiliser NEXT, ça fait tout foirer
		while(c!='{' && !pFileManager->EndOfFile(file))
			pFileManager->Read(file, c);
			
		num_open = 1;
		while(num_open && !pFileManager->EndOfFile(file)) {
			pFileManager->Read(file, c);
			if (c == '}')
				num_open--;
			if (c == '{')
				num_open++;
			if (c == '"' && num_open <= 2) { //les proprietes sont dans les premieres accolades. Dans les accolades imbriques viennent les patchDefs/brushesDefs
				tgstring name;
				pFileManager->Read(file, c);
				while(c != '"') {
					name.push_back(c);
					//on veut les espaces dans les guillemets-> utilisation de Get
					pFileManager->Get(file, c);
				}
				if (state == 0) {
					state = 1;
					prop_name = name;
				} else {
					state = 0;
					prop_value = name;
					pEntities[i].mProps[prop_name] = prop_value;
					//out << "num_open: " << num_open << "\t\"" << prop_name << "\"" << pEntities[i].mProps[prop_name] << endl;
				}
			}
		}
		num_open = 0;
		state = 0;
	}
	return true;
}

bool tgDoom3Level::_LoadProc(int file)
{
	tgstring buffer;
	int state = 1;
	iNumStaticModels = 0;
	int iNumLoadModels = 0;
	while (!pFileManager->EndOfFile(file)) { //on compte le nombre de models
		pFileManager->Read(file, buffer);
		if (buffer == "model") {
			/*NEXT("{")
			SKIP_COMMENT
			pFileManager->Read(file, buffer);*/
			iNumLoadModels++;
			NEXT("{")
			SKIP_COMMENT
			tgstring name;
			pFileManager->Read(file, name);
			name.erase(0,1);
			name.erase(name.size()-1, name.size());
			if(name.find("_area") == tgstring::npos)  //ça n'est pas une area -> static model
				iNumStaticModels++;
			
		}
	}
	pFileManager->Clear(file);
	pFileManager->Seek(file, 0);
	
	
	
	
	//BEGIN Portals
	pFileManager->Clear(file);
	pFileManager->Seek(file, 0);
	
	NEXT("interAreaPortals")
	if (pFileManager->EndOfFile(file)) { //pas d'areas definie pour cette map
	
	} else {
		NEXT("{")
		SKIP_COMMENT
		pFileManager->Read(file, iNumAreas);
		SKIP_COMMENT
		pFileManager->Read(file, iNumPortals);
		pPortals = new _Portal[iNumPortals];
		SKIP_COMMENT
		for(int i=0; i<iNumPortals; i++) {
			SKIP_COMMENT
			pFileManager->Read(file, pPortals[i].iNumPoints);
			pFileManager->Read(file, pPortals[i].iPositiveArea);
			pFileManager->Read(file, pPortals[i].iNegativeArea);
			pPortals[i].pPoints = new tgVector3[pPortals[i].iNumPoints];
			for (int j=0; j<pPortals[i].iNumPoints; j++) {
				NEXT("(")
				pFileManager->Read(file, pPortals[i].pPoints[j]);
				pPortals[i].pPoints[j].ChangeCoords();
				NEXT(")")
			}
			//out << "numPoints : " << pPortals[i].iNumPoints  << "\tpositive area : " << pPortals[i].iPositiveArea << "\tnegative area : " << pPortals[i].iNegativeArea << endl;
		}
	}
	//END Portals
	
	//BEGIN Nodes
	pFileManager->Clear(file);
	pFileManager->Seek(file, 0);
	
	NEXT("nodes")
	if (pFileManager->EndOfFile(file)) { //pas d'areas definie pour cette map
	
	} else {
		NEXT("{")
		SKIP_COMMENT
		pFileManager->Read(file, iNumNodes);
		pNodes = new _Node[iNumNodes];
		SKIP_COMMENT
		SKIP_COMMENT
		SKIP_COMMENT
		for(int i=0; i<iNumNodes; i++) {
			SKIP_COMMENT
			tgVector3 temp;
			float f;
			NEXT("(")
			pFileManager->Read(file, temp);
			pFileManager->Read(file, f);
			NEXT(")")
			temp.ChangeCoords();
			pNodes[i].plane = tgPlane(temp, f);
			pFileManager->Read(file, pNodes[i].iPositiveChild);
			pFileManager->Read(file, pNodes[i].iNegativeChild);
		}
		
		
	}
	//END Nodes
	
	//BEGIN Modèles
	pFileManager->Clear(file);
	pFileManager->Seek(file, 0);
	
	int areaCount = 0;
	int staticCount = 0;
	pAreas = new _Area [iNumAreas];
	pStaticModels = new _StaticModel [iNumStaticModels];
	
	for(int k=0; k<iNumLoadModels; k++) {
		//_Model& model = pModels[k];
		
		NEXT("model")
		NEXT("{")
		SKIP_COMMENT
		tgstring name;
		pFileManager->Read(file, name);
		name.erase(0,1);
		name.erase(name.size()-1, name.size());
		//out << model.sName << endl;
		bool bArea;
		_Model* model;
		if(name.find("_area") != tgstring::npos) { //on a une area
			//pAreas[areaCount] = &model;
			bArea = true;
			model = &pAreas[areaCount];
			areaCount++;
		} else { //on a un modèle "static";
			bArea = false;
			model = &pStaticModels[staticCount];
			staticCount++;
		}
		model->sName = name;
	
		//On fait correspondre les entites definie dans le .map au modèles (les modèles static ont leur origine définit dans une entite)
		if (!bArea) {
			_StaticModel* staticModel = static_cast<_StaticModel*>(model);
			for(int i=0; i<iNumEntities; i++) {
				if (staticModel->sName == pEntities[i].mProps["name"]) {
					tgstring temp = pEntities[i].mProps["origin"];
					//out << temp << endl;
					int spaces[3];
					spaces[0] = temp.find_first_of(' ');
					spaces[1] = temp.find_first_of(' ', spaces[0]+1);
					//spaces[2] = temp.find_first_of(' ', spaces[1]);
					tgstring sx = temp.substr(0,spaces[0]);
					tgstring sy = temp.substr(spaces[0], spaces[1]-spaces[0]);
					tgstring sz = temp.substr(spaces[1]);
					
					staticModel->vOrigin.Set((float)atof(sx.data()), (float)atof(sy.data()), (float)atof(sz.data()));
					staticModel->vOrigin.ChangeCoords();
				}
			}
		}
		
		//Lecture des infos sur les surfaces
		SKIP_COMMENT
		pFileManager->Read(file, model->numSurfaces);
		model->pSurfaces = new _Surface[model->numSurfaces];
		
		//AABB du modèle
		tgVector3 boxMin(0,0,0);
		tgVector3 boxMax(0,0,0);
		
		for(int i=0; i<model->numSurfaces; i++) {
			_Surface& surface = model->pSurfaces[i];
			NEXT("{")
			pFileManager->Read(file, surface.shaderName);
			surface.shaderName.erase(0,1);
			surface.shaderName.erase(surface.shaderName.size()-1, surface.shaderName.size());
			
			SKIP_COMMENT
			pFileManager->Read(file, surface.numVertices);
			SKIP_COMMENT
			pFileManager->Read(file, surface.numIndexes);
			
			surface.pMaterial = pMaterialManager->GetMaterial(surface.shaderName);
			surface.pVertices = new tgVertice[surface.numVertices];
			surface.pIndexes = new int[surface.numIndexes];
			for (int j=0; j<surface.numVertices; j++) {
				tgVertice& vert = surface.pVertices[j];
				NEXT("(")
				pFileManager->Read(file, vert.vPosition.x);
				pFileManager->Read(file, vert.vPosition.y);
				pFileManager->Read(file, vert.vPosition.z);
				vert.vPosition.ChangeCoords();
				pFileManager->Read(file, vert.s);
				pFileManager->Read(file, vert.t);
				pFileManager->Read(file, vert.vNormal.x);
				pFileManager->Read(file, vert.vNormal.y);
				pFileManager->Read(file, vert.vNormal.z);
				vert.vNormal.ChangeCoords();
				//AABB
				tgVector3& v = vert.vPosition;
				if (v.x < boxMin.x)
					boxMin.x = v.x;
				if (v.y < boxMin.y)
					boxMin.y = v.y;
				if (v.z < boxMin.z)
					boxMin.z = v.z;
					
				if (v.x > boxMax.x)
					boxMax.x = v.x;
				if (v.y > boxMax.y)
					boxMax.y = v.y;
				if (v.z > boxMax.z)
					boxMax.z = v.z;
				
				
				//out << vert.vPosition.x << "\t" << vert.vPosition.y << "\t" << vert.vPosition.z << endl;
				NEXT(")")
			}
			
			for(int j=0; j<surface.numIndexes; j++) {
				pFileManager->Read(file, surface.pIndexes[j]);
			}
		}
		NEXT("}")	
		model->pBBox = new tgAABoundingBox(boxMin, boxMax);
	}
	//END modèles
	
	//Connection des areas avec les portals
	for(int i=0; i<iNumPortals; i++) {
		//on augment le nombre de portal dans les modèles concernes
		pAreas[pPortals[i].iPositiveArea].iNumPortals++;
		pAreas[pPortals[i].iNegativeArea].iNumPortals++;
		
		pPortals[i].visible = false;
	}
	
	int* pCount = new int[iNumAreas];
	for(int i=0; i<iNumAreas; i++) {
		pAreas[i].pPortals = new _Portal*[pAreas[i].iNumPortals];
		pAreas[i].visible = false;
		pCount[i] = 0;
	}
	
	for(int i=0; i<iNumPortals; i++) {
		int index = pPortals[i].iPositiveArea;
		pAreas[index].pPortals[pCount[index]] = &pPortals[i];
		pCount[index]++;
		
		index = pPortals[i].iNegativeArea;
		pAreas[index].pPortals[pCount[index]] = &pPortals[i];
		pCount[index]++;
	}
	delete [] pCount;
	
	return true;
}
void tgDoom3Level::_RenderArea (const tgFrustum& frustum, int areaNum, int fromArea)
{
	if (areaNum == -1 || areaNum > iNumAreas) {
		return;
	}
	if (pAreas[areaNum].visible) 
		return;
	pAreas[areaNum].visible = true;
		
	for(int i=0; i<pAreas[areaNum].iNumPortals; i++) {
		_Portal& portal = *pAreas[areaNum].pPortals[i];
		if (!portal.visible) {
			//portal.visible = false;
			if(frustum.PolygonInside(portal.pPoints, portal.iNumPoints)) {
				portal.visible = true;
				
				if(portal.iPositiveArea != areaNum && portal.iPositiveArea != fromArea)
					_RenderArea(frustum, portal.iPositiveArea, areaNum);
				if(portal.iNegativeArea != areaNum && portal.iNegativeArea!= fromArea)
					_RenderArea(frustum, portal.iNegativeArea, areaNum);
			}
			/*for(int j=0; j<portal.iNumPoints; j++)
				if (frustum.PointInside(portal.pPoints[j]))
					portal.visible = true;*/
			
		}
		
		/*if (!portal.visible)
			continue;
			
		
		
		//out << portal.iPositiveArea << "\t" << portal.iNegativeArea << endl;
		if (portal.iPositiveArea != areaNum && portal.iPositiveArea != fromArea)
			_RenderArea(newFrustum, portal.iPositiveArea, areaNum);
		if (portal.iNegativeArea != areaNum && portal.iNegativeArea != fromArea)
			_RenderArea(newFrustum, portal.iNegativeArea, areaNum);*/
	}
}



void tgDoom3Level::RenderAreas(tgRenderView* rView)
{
	//Remise a zéro
	for(int i=0; i<iNumAreas; i++) {
		pAreas[i].visible = false;
	}
	for(int i=0; i<iNumPortals; i++) {
		pPortals[i].visible = false;
	}
	for(int i=0; i<iNumStaticModels; i++) {
		pStaticModels[i].visible = false;
	}

	
	int area = _FindArea(rView->vOrigin);
	//cas spécial
	if (area == -1) { //on est en dehors de la map -> on affiche tout
		for(int i=0; i<iNumAreas; i++) {
			pAreas[i].visible = true;
		}
		for(int i=0; i<iNumPortals; i++) {
			pPortals[i].visible = true;
		}
		return;
	} 
	
	_RenderArea (rView->frustum, area, -1);
}

void tgDoom3Level::MarkVisibleModels ()
{
	/*for(int i=0; i<iNumAreas; i++) {
		if(pAreas[i]->visible) {
			
		}
	}*/
	for(int i=0; i<iNumStaticModels; i++) {
		_StaticModel& model = pStaticModels[i];
		model.visible = false;
		for(unsigned int j=0;j<model.areas.size(); j++) {
			//out << model.areas[j] << endl;
			int index = model.areas[j];
			if (index < 0) {
				out << "index < 0" << endl;
				model.visible = true;
				break;
			} else if(pAreas[index].visible) {
				model.visible = true;
				break;
			}
		}
	}
}

void tgDoom3Level::_ClassifyModels()
{
	for(int i=0; i<iNumStaticModels; i++) {
		_StaticModel& model = pStaticModels[i];
		_FindBoxAreas(model.pBBox, model.areas);
		//cout << model.areas.size() << endl;
		/*for(int j=0; j<model.areas.size(); j++)
			cout << model.areas[j] << "\t";
		cout << endl;*/
	}
}

void tgDoom3Level::_FindBoxAreas(tgAABoundingBox* box, tgvector<int>& areas)
{
	areas.clear();
	_FindBoxAreas_r(box, 0, areas);
}

void tgDoom3Level::_FindBoxAreas_r(tgAABoundingBox* box, int index, tgvector<int>& areas)
{
	if (index < 0) {
		int areanum = ~index;
		if (areanum < 0 || areanum > iNumAreas)
			cerr << "Mauvais areanum : " << areanum << endl;
		
		//on vérifie qu'il n'y soit pas déjà
		tgvector<int>::iterator ir = find(areas.begin(), areas.end(), areanum);
		if(ir == areas.end())
			areas.push_back(areanum);
		return;
	}


	const _Node& node = pNodes[index];
	int side = node.plane.ClassifyBox(box);
	if (side == BOX_IN_FRONT_OF_PLANE) {
		if(node.iPositiveChild)
			_FindBoxAreas_r(box, node.iPositiveChild,areas);
	}
	else if (side == BOX_BEHIND_PLANE) {
		if(node.iNegativeChild)
			_FindBoxAreas_r(box, node.iNegativeChild,areas);
	}
	else if (side == BOX_INTERSECT_PLANE) {
		
		if (node.iPositiveChild) //on continue sur les nodes si on ne repart pas sur 0
			_FindBoxAreas_r(box, node.iPositiveChild, areas);
		if (node.iNegativeChild) //on continue sur les nodes si on ne repart pas sur 0
			_FindBoxAreas_r(box, node.iNegativeChild, areas);
	}
}

int tgDoom3Level::_FindArea(const tgVector3& pos)
{
	return _FindArea_r(pos, 0);
}

int tgDoom3Level::_FindArea_r(const tgVector3& pos, int index)
{
	const _Node& node = pNodes[index];
	int side = node.plane.ClassifyPoint(pos);
	if (side == POINT_IN_FRONT_OF_PLANE)
		index = node.iPositiveChild;
	else 
		index = node.iNegativeChild;
		
	if (index <= 0)
		return ~index;
		
	return _FindArea_r(pos, index);
}



#endif

