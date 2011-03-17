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
#ifndef TGQ3LEVEL_H
#define TGQ3LEVEL_H

#include "tgdefs.h"
#include "tgq3defs.h"
#include "tgendian.h"
#include "tgfile_manager.h"
#include "tgiworld_manager.h"
#include "gl/tggl.h"
#include "tgmaterial.h"
#include "tgdecal.h"
#include "tgmaterial_manager.h"
#include "tgrenderview.h"

enum eTraceType
{
  TRACE_BOX,
  TRACE_SPHERE,
  TRACE_RAY
};


class tgQ3Level
{
  friend class tgGLRenderer;
  friend class tgGLRendererSTD;
 public:
  tgQ3Level (const char* levelname);
  ~tgQ3Level ();

  //remplis le tableau pVisibleFaces en explorant l'arbre BSP a partir d'une renderview 
  //ou d'une sphere (renvoit les faces contenues dans la sphere)
  void FindVisibleFaces (const tgRenderView& rView);
  void FindSphereContainedFaces (const tgBoundingSphere& sphere);

  const tgCollisionResult& Trace(const tgVector3& Start, const tgVector3& End, tgBoundingVolume* volume, tgIEntity* entity, tgset<tgIEntity*>* ignoreList = NULL);

  void AddEntity(tgIEntity* entity);
  void RemoveEntity(tgIEntity* entity);

  void GetFacesMarked (tgBoundingSphere* pBSphere, tgDecal* decal);

  void GetTouchingEntities (tgBoundingVolume* volume, tgvector<tgIEntity*>& list);
  
  /**
  * Appelée à chaque frame pour faire divers traitements internes
  * Notamment pour remettre à jour la liste des entitées
  */
  void Think ();

  int GetNumStartLocations() { return (int)infoPlayerDeathmatch.size(); }
  const tgVector3& GetStartLocation(int index) { return infoPlayerDeathmatch[index].vOrigin; }
  
  int TraceAgainstTriangleCount;
 protected:
  struct _entInfoPlayerDeathmatch
  {
    tgVector3 vOrigin;
    float angle;
  };

  //entities information
  tgvector<_entInfoPlayerDeathmatch> infoPlayerDeathmatch;
  
  const tgCollisionResult& _Trace (const tgVector3& Start, const tgVector3& End);
  void _TraceAgainstNode (int nodeIndex, float startFraction, float endFraction, const tgVector3& Start, const tgVector3& End);
  void _TraceAgainstBrush (Q3Defs::BspBrush* brush);
  void _TraceAgainstPatch (Q3Defs::BspPatch* patch);
  //void _CollideAgainstTriangle(const tgPlane& triPlane, const tgVector3& point1, const tgVector3& point2, const tgVector3& point3);
  void _CollideAgainstTriangle(const Q3Defs::BspPatchFace& face);
  
  void _RecursiveSphereLeaves(int nodenum,const tgBoundingSphere* pBBox, tgvector<int>* pList);
  
  int _FindLeaf (const tgVector3& pos);
  bool _IsClusterVisible (int from, int test);
  bool _LoadBSP (const char* levelname);
  void _LoadTextures ();
  void _LoadLightmaps ();
  void _LoadPatch(Q3Defs::BspPatch* p, int tesselation);
  void _FillLeavesPatchInfo ();
  void _LoadEntities(int file);
  void _TesselateBiquadraticPatch (Q3Defs::BspVertex* controlPoints, Q3Defs::BspVertex* vertices, int tess);
  Q3Defs::BspVertex _QuadraticInterpolate (const Q3Defs::BspVertex& v1, const Q3Defs::BspVertex& v2, const Q3Defs::BspVertex& v3, float f);
  
  Q3Defs::BspHeader header;
  Q3Defs::BspLump lumps[Q3Defs::kMaxLumps];
  
  //information temporaires pour les 'trace'
  tgCollisionResult traceResult;
  tgBoundingVolume* pTraceVolume;
  tgset<tgIEntity*>* pIgnoreList;
  tgVector3 MStart;
  tgVector3 MEnd;
  tgIEntity* pTraceEntity;
  eTraceType traceType;
  
  
  tgset<tgIEntity*> pEntityList;

  //information pour stocker les entites
  tgmap<tgIEntity*, tgvector<int> > entityMap;
  
  int numOfVerts;
  int numOfFaces;
  int numOfTextures;
  int numOfLeafs;
  int numOfNodes;
  int numOfLeafFaces;
  int numOfPlanes;
  int numOfLightmaps;
  int numOfMeshIndices;
  int numOfEntities;
  int numOfLoadEntities;
  int numOfShaders;
  int numOfLeafBrushes;
  int numOfBrushSides;
  int numOfBrushes;
  int numOfPatches;
  int numOfPolygonFaces;
  int numOfMeshFaces;
  
  int planeCount;
  int brushSidesCount;
  int brushesCount;

  
  Q3Defs::BspFace* pFaces;
  Q3Defs::BspVertex* pVerts;
  
  tgMaterial**      pMaterials;
  Q3Defs::BspTexture*   pTexturesInfos;
  Q3Defs::MyLeaf*     pLeafs;
  Q3Defs::BspNode*    pNodes;
  Q3Defs::BspPlane*   pPlanes;
  Q3Defs::BspLightmap*    pLightMapsInfos;
  int*        pLeafFaces;
  Q3Defs::BspBrushSide*   pBrushSides;
  int*        pLeafBrushes;
  Q3Defs::BspBrush*   pBrushes;
  Q3Defs::BspShader*    pShaders;
  bool*         pVisibleFaces;
  int*        pMeshIndices;
  int*        pContents;
  Q3Defs::BspVisData    clusters;
  Q3Defs::BspPatch*     pPatches;
  Q3Defs::BspPolygonFace*   pPolygonFaces;
  Q3Defs::BspMeshFace*    pMeshFaces;
  Q3Defs::BspFaceDirectoryEntry*  pDirectory;
  unsigned int*       pLightMaps;
  Q3Defs::BspEntity*    pEntities;
};

#endif
