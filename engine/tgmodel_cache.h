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
#ifndef TGMODEL_CACHE_H
#define TGMODEL_CACHE_H

#include "tgdefs.h"
#include "tgfile_manager.h"
#include "tgimport.h"

/**
* Classe s'occupant de gérer le cache des données d'un modèle.
* Utilisée par le ModelManager
*/
class tgModelCache
{
 protected:
	struct _MD5Joint;
	struct _MD5Mesh;
	struct _MD5Vert;
	struct _MD5Weight;
	struct _MD5Triangle;
	struct _MD5Anim;
	struct _MD5Edge;
	
 public:
	tgModelCache(const char* mdfFile);
	~tgModelCache ();
	
	/**
	* Permet de savoir si le modèle a été correctement chargé
	* @return true s'il a été correctement chargé
	*/
	inline bool IsLoaded () const { return bLoaded; }
	
	/**
	* Retourne l'index de l'animation nommé "name"
	* @param name le nom de l'animation
	* @return l'index, utilisable pour les fonctions d'accès  (anim_XXX)
	*/
	int GetAnimIndex(const std::string& name);
	
 protected:
	bool _ExecuteMDF (const char* mdfFile);
	bool _LoadMesh (const char* filename);
	bool _LoadAnim(const tgstring& name, const tgstring& filename);
	int _FindJointIndexByName(const tgstring& name);
	void _RecursiveSetChannel(int channel, _MD5Joint* joint);
	void _PreCalculateWeightsNormals();
	
	
	int iNumJoints;
	int iNumBaseJoints;
	int iNumMeshes;
	int** pIndexes;
	_MD5Joint* pJoints;
	_MD5Joint** pBaseJoints;
	_MD5Mesh* pMeshes;
	bool bLoaded;
	//typedef tgmap<tgstring,_MD5Anim> animMap_t;
	//animMap_t mAnimations;
	tgvector<_MD5Anim> pAnimations;
 public:
	//Joint informations
	inline int GetNumJoints() { return iNumJoints; }
	inline int GetJointParent(int i) { return pJoints[i].iParent; }
	inline tgVector3 GetJointPosition(int i) { return pJoints[i].vPosition; }
	inline tgQuaternion& GetJointOrientation(int i) { return pJoints[i].qOrientation; }
	inline int GetNumBaseJoints() { return iNumBaseJoints; }
	inline tgstring GetJointName(int i) { return pJoints[i].sName; }
	inline int GetJointChannel(int i) { return pJoints[i].iChannel; }
	
	//Edges informations
	//inline int GetNumEdges () { return edges.size(); }
	//inline Edge* GetEdge(int i) { return &edges[i]; }
	//Meshes informations
	inline int GetNumMeshes () { return iNumMeshes; }
	inline int mesh_GetNumVerts (int i) { return pMeshes[i].iNumVerts; }
	inline int mesh_GetNumWeights (int i) { return pMeshes[i].iNumWeights; }
	inline int mesh_GetNumTris (int i) { return pMeshes[i].iNumTris; }
	inline int mesh_GetNumEdges (int i) { return pMeshes[i].iNumEdges; }
	inline unsigned int* mesh_GetIndexes(int i) { return pMeshes[i].pIndexes; }
	inline tgFace* mesh_GetFace(int i, int j) { return &pMeshes[i].pTriangles[j]; }
	inline tgstring mesh_GetMatName(int i)  { return pMeshes[i].sMatName; }
	//Vertices informations
	inline int vert_GetWeightIndex(int i, int j) { return pMeshes[i].pVerts[j].iWeightIndex; }
	inline int vert_GetNumWeights (int i, int j) { return pMeshes[i].pVerts[j].iNumWeights; }
	inline float* vert_GetTexCoords (int i, int j) { return pMeshes[i].pVerts[j].pTexCoords; }
	//Weights informations
	inline int weight_GetBoneIndex(int i, int j) { return pMeshes[i].pWeights[j].iBoneIndex; }
	inline float weight_GetBias (int i, int j) { return pMeshes[i].pWeights[j].fBias; }
	inline tgVector3& weight_GetWeight (int i, int j) { return pMeshes[i].pWeights[j].vWeights; }
	inline tgVector3& weight_GetNormal (int i, int j) { return pMeshes[i].pWeights[j].vNormal; }
	//Edges informations	
	inline int* edge_GetVertIndexes(int i, int j) { return pMeshes[i].pEdges[j].pVertIndexes; }
	inline int* edge_GetTriangleIndexes(int i, int j) { return pMeshes[i].pEdges[j].pTriangleIndexes; }
	//Triangle informations
	inline unsigned int* triangle_GetIndexes(int i, int j) { return pMeshes[i].pTriangles[j].pIndex; }
	
	//Animations informations
	inline int anim_GetNumFrames(int i) { return pAnimations[i].iNumFrames; }
	inline int anim_GetNumJoints(int i) { return pAnimations[i].iNumJoints; }
	inline int anim_GetFrameRate(int i) { return pAnimations[i].iFrameRate; }
	inline tgQuaternion anim_GetQuaternion (int i, int j, int k) { return pAnimations[i].pQuaternions[j][k]; }
	inline tgVector3 anim_GetPosition (int i, int j, int k) { return pAnimations[i].pPositions[j][k]; }
		
	//inline BoundingBox* anim_GetBBox(int i,int j) { return &pAnimations[i].pBBox[j]; }
	inline int anim_joint_GetParent (int i, int j) { return pAnimations[i].pJointInfos[j].iParent; }
	inline int anim_joint_GetFlags (int i, int j) { return pAnimations[i].pJointInfos[j].iFlags; }
	
	inline int anim_joint_GetStartIndex (int i, int j) { return pAnimations[i].pJointInfos[j].iStartIndex; }
	
	
 protected:
 
	struct _MD5Anim
	{
		std::string sName;
		int iNumFrames;
		int iNumJoints;
		int iFrameRate;
		int iNumAnimatedComponents;
		tgQuaternion** pQuaternions; //un tableau de [iNumFrames][iNumJoints] Qx,Qy,Qz
		tgVector3** pPositions; //un tableau de [iNumFrames][iNumJoints] Tx,Ty,Tz
		float** pBaseFrame; //un tableau de [iNumJoints][6], stockant, dans l'ordre, Tx, Ty, Tz, Qx, Qy, Qz a la frame de base de l'animation
		struct _MD5JointInfos
		{
			int iParent;
			int iFlags;
			int iStartIndex;
		};
		_MD5JointInfos* pJointInfos; //un tableau de iNumJoints

		//BoundingBox* pBBox; //un tableau de [iNumFrames] BoundingBox, une pour chaque frame
	};

	
	
	struct _MD5Joint
	{
		std::string sName;
		int iNumber;
		int iParent;
		int iChannel;
		tgVector3 vPosition;
		tgQuaternion qOrientation;
		
		//hiérarchie
		_MD5Joint* pParent;
		_MD5Joint** pChildrens;
		int iNumChildrens;
	};
	
	struct _MD5Mesh
	{
		public:
			void SetConnectivity();
			
			int iNumVerts;
			_MD5Vert* pVerts;
			
			int iNumWeights;
			_MD5Weight* pWeights;
			
			int iNumTris;
			_MD5Triangle* pTriangles;
			
			unsigned int* pIndexes;
			
			int iNumEdges;
			_MD5Edge* pEdges;
			//Material* pMaterial;
			tgstring sMatName;
			//tgstring sNormal;
		protected:
			void _AddEdge(int v1, int v2, int tri, tgvector<_MD5Edge>& edgesList);
			void _FindCreateMatchingEdge(int v1, int v2, int tri, tgvector<_MD5Edge>& edgesList);
	};
	
	struct _MD5Vert
	{
		int iWeightIndex;
		int iNumWeights;
		float pTexCoords[2];
	};
	
	struct _MD5Weight
	{
		int iBoneIndex;
		float fBias;
		tgVector3 vWeights;
		tgVector3 vNormal;
	};
	
	struct _MD5Triangle : public tgFace
	{
	
	};
	
	struct _MD5Edge
	{
		int pVertIndexes[2];
		int pTriangleIndexes[2];
	};
};

#endif
