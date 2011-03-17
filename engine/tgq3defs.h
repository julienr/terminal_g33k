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
#ifndef TGQ3DEFS_H
#define TGQ3DEFS_H

#include "tgdefs.h"
#define MAX_BSP_PLANES          0x20000
#define MAX_CM_PLANES            (MAX_BSP_PLANES<<2)
#define MAX_BSP_BRUSHSIDES 0x20000
#define MAX_CM_BRUSHSIDES  (MAX_BSP_BRUSHSIDES<<1)
#define   MAX_BSP_BRUSHES         0x8000
#define MAX_CM_BRUSHES                  (MAX_BSP_BRUSHES<<1)
/**
* Structures pour charger les maps au format .bsp de quake 3
* Les structures commencant par "Bsp" sont celles qui servent au chargement d'après le fichier binaire. 
* Celles commencant par un "My" ont des informations en plus par rapport aux "Bsp"
*/
class Q3Defs
{
 public:
	enum BspFaceType
	{
		PolygonFace = 1,
		PatchFace,
		MeshFace,
		BillboardFace
	};
	
	struct BspFaceDirectoryEntry
	{
		BspFaceType type;
		int typeIndex; //index dans le tableau des faces de ce type
	};
	
	struct BspHeader
	{
		char strID[4];
		int version;
	};
	
	struct BspLump
	{
		int offset;
		int length;
	};
	
	struct BspLeaf
	{
		int cluster;
		int area;
		tgVector3 min;
		tgVector3 max;
		int leafface;
		int numOfLeafFaces;
		int leafBrush;
		int numOfLeafBrushes;
	};
	//#include <vector>
	struct MyLeaf
	{
		int cluster;
		int area;
		tgAABoundingBox box;
		int leafface;
		int numOfLeafFaces;
		int leafBrush;
		int numOfLeafBrushes;
		int leafPatch;
		int numOfLeafPatches;
		tgvector <int> patchIndex;
		tgvector<tgIEntity*> entityList;
	};
	
	struct BspBrush 
	{
		int brushSides;
		int numOfBrushSides;
		int textureID;
	};
	
	struct BspBrushSide 
	{
		int plane;
		int textureID;
	};
	
	struct BspLightmap
	{
		unsigned char imageBits[128][128][3];
	};
	
	struct BspNode
	{
		int plane;
		int front;
		int back;
		tgVector3 min;
		tgVector3 max;
	};
	
	
	
	struct BspLoadPlane
	{
		tgVector3 vNormal;
		float fD;
	};

	//typedef BspLoadPlane BspPlane;	
	struct BspPlane : public tgPlane
	{
	};
	
	struct BspVisData
	{
		int numOfClusters;
		int bytesPerCluster;
		unsigned char* pBitsets;
	};
	
	struct BspVertex
	{
		tgVector3 vPosition; 
		float vTextureCoord[2];
		float vLightmapCoord[2];
		tgVector3 vNormal;
		unsigned char color[4];
	
		BspVertex operator+(const BspVertex & rhs) const
		{
			BspVertex result;
			result.vPosition=vPosition+rhs.vPosition;
			result.vTextureCoord[0]=vTextureCoord[0]+rhs.vTextureCoord[0];
			result.vTextureCoord[1]=vTextureCoord[1]+rhs.vTextureCoord[1];
			result.vLightmapCoord[0]=vLightmapCoord[0]+rhs.vLightmapCoord[0];
			result.vLightmapCoord[1]=vLightmapCoord[1]+rhs.vLightmapCoord[1];
			return result;
		}
	
		BspVertex operator*(const float rhs) const
		{
			BspVertex result;
			result.vPosition=vPosition*rhs;
			result.vTextureCoord[0]=vTextureCoord[0]*rhs;
			result.vTextureCoord[1]=vTextureCoord[1]*rhs;
			result.vLightmapCoord[0]=vLightmapCoord[0]*rhs;
			result.vLightmapCoord[1]=vLightmapCoord[1]*rhs;
			return result;
		}
	
	};
	
	struct BspFace //les faces telles qu'elles sont chargees
	{
		int textureID;                              // The index into the texture array
		int effect;                                 // The index for the effects (or -1 = n/a)
		int type;                                   // 1=polygon, 2=patch, 3=mesh, 4=billboard
		int startVertIndex;                 // The starting index into this face's first vertex
		int numOfVerts;                             // The number of vertices for this face
		int meshVertIndex;                  // The index into the first meshvertex
		int numMeshVerts;                   // The number of mesh vertices
		int lightmapID;                             // The texture index for the lightmap
		int lMapCorner[2];                  // The face's lightmap corner in the image
		int lMapSize[2];                    // The size of the lightmap section
		float lMapPos[3];                   // The 3D origin of lightmap.
		float lMapVecs[2][3];               // The 3D space for s and t unit vectors.
		tgVector3 vNormal;                   // The face normal.
		int size[2];                                // The bezier patch dimensions.
	};
	
	struct BspPolygonFace
	{
		int startVertIndex;
		int numOfVerts;
		int textureID;
		int lightmapID;
		/*tgVector3 vCenter;
		float fRadius;*/
		int firstMeshIndex;
		int numOfMeshVerts;
		tgPlane plane;
		tgBoundingSphere sphere;
	};
	
	struct BspMeshFace
	{
		int startVertIndex;
		int numOfVerts;
		int textureID;
		int lightmapID;
		int firstMeshIndex;
		int numOfMeshVerts;
		tgBoundingSphere sphere;
	};
	
	// Article excellent : http://www.gamasutra.com/features/19990611/bezier_01.htm
	// Code sur la subdivision des patches tire de la

	struct BspPatchFace
	{
		tgVector3 points[3];
		tgPlane plane;
	};
	
	struct BspPatch
	{
		BspVertex* pVertices;
		unsigned int* pIndices;
		unsigned int iNumIndices;

		BspPatchFace* pFaces;
		
		int iTesselation;
	
		int textureID;
		int lightmapID;
		tgVector3 vNormal;
	
		int size[2];
		BspVertex* controls;
		
		tgAABoundingBox* pBBox;
		tgBoundingSphere sphere;
	
		~BspPatch () 
		{
			if(pFaces)
				delete [] pFaces;
			if (pVertices)
				delete [] pVertices;
			if (pIndices)
				delete [] pIndices;
			if (controls)
				delete [] controls;
			if (pBBox)
				delete pBBox;
		}
	};

	struct BspEntity
	{
		std::map<std::string, std::string> mProperties;
	};
	
	enum eLumps
	{
		kEntities = 0,                              // Stores player/object positions, etc...
		kTextures,                                  // Stores texture information
		kPlanes,                                // Stores the splitting planes
		kNodes,                                             // Stores the BSP nodes
		kLeafs,                                             // Stores the leafs of the nodes
		kLeafFaces,                                 // Stores the leaf's indices into the faces
		kLeafBrushes,                               // Stores the leaf's indices into the brushes
		kModels,                                    // Stores the info of world models
		kBrushes,                                   // Stores the brushes info (for collision)
		kBrushSides,                                // Stores the brush surfaces info
		kVertices,                                  // Stores the level vertices
		kMeshVerts,                                 // Stores the model vertices offsets
		kShaders,                                   // Stores the shader files (blending, anims..)
		kFaces,                                             // Stores the faces for the level
		kLightmaps,                                 // Stores the lightmaps for the level
		kLightVolumes,                              // Stores extra world lighting information
		kVisData,                                   // Stores PVS and cluster info (visibility)
		kMaxLumps                                   // A constant to store the number of lumps
	};
	
	struct BspTexture
	{
		char strName[64];                   // The name of the texture w/o the extension
		int flags;                                  // The surface flags (unknown)
		int contents;                               // The content flags (unknown)
	};
	
	struct BspShader
	{
		char strName[64]; //le nom du shader
		int brush; //le brush qui génère cet effet
		int unknown; // toujours 5, sauf q3dm8 qui en a un à -1	
	};
};

#endif
