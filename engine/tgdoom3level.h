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
#ifndef TGDOOM3LEVEL_H
#define TGDOOM3LEVEL_H

#include "defs.h"

class tgMaterial;

class tgDoom3Level
{
 friend class tgGLRenderer;
 public:
	tgDoom3Level (const char* procname, const char* mapname);
	~tgDoom3Level();
	
	
	void MarkVisibleModels ();
	void RenderAreas(tgRenderView* rView);
 protected:
	bool _LoadProc(int file);
	bool _LoadMap(int file);
	int _FindArea(const tgVector3& pos);
	int _FindArea_r(const tgVector3& box, int index);
	void _FindBoxAreas(tgAABoundingBox* box, tgvector<int>& areas);
	void _FindBoxAreas_r(tgAABoundingBox* box, int index, tgvector<int>& areas);
	
	void _RenderArea (const tgFrustum& frustum, int areaNum, int fromArea);
	void _ClassifyModels ();
	
	struct _Surface
	{
		tgstring shaderName;
		int numVertices;
		int numIndexes;
		tgVertice* pVertices;
		int* pIndexes;
		tgMaterial* pMaterial;
	};

	struct _Entity
	{
		/*tgstring sName;
		tgVector3 vOrigin;*/
		tgmap<tgstring, tgstring> mProps;
	};
	
	struct _Portal
	{
		int iNumPoints;
		int iPositiveArea;
		int iNegativeArea;
		tgVector3* pPoints;
		
		bool visible;
	};
	
	struct _Model
	{
		tgstring sName;
		int numSurfaces;
		_Surface* pSurfaces;
		tgAABoundingBox* pBBox;
		bool visible; //utilise par les areas
		
	};
	
	struct _StaticModel : public _Model
	{
		tgVector3 vOrigin;
		tgvector<int> areas;
	};
	
	struct _Area : public _Model
	{
		int iNumPortals;
		_Portal** pPortals; // des pointeurs vers les portals auquelles cette area est connectee
	};
	
	struct _Node
	{
		tgPlane plane; //un child negatif sera une area d'index (-1-child)
		int iPositiveChild;
		int iNegativeChild;
	};
	
	int iNumStaticModels;
	int iNumEntities;
	int iNumPortals;
	int iNumAreas;
	int iNumNodes;
	_StaticModel* pStaticModels;
	_Entity* pEntities;
	_Portal* pPortals;
	_Area* pAreas;
	_Node* pNodes;
};

#endif


#endif

