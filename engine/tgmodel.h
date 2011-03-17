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
#ifndef TGMODEL_H
#define TGMODEL_H

#include "tgimodel.h"
#include "tgdefs.h"
#include "tgmaterial.h"
#include "tgmodel_cache.h"
#include "tgmaterial_manager.h"
#include "tgimport.h"
#include "tgfile_manager.h"

/**
* Implémentation de la gestion des modèles
*/
class tgModel : public tgIModel
{
	friend class tgGLRenderer;
	friend class tgGLRendererSTD;

 public:
	/**  Herités de tgIEntity  **/
 	void SetPosition (const tgVector3& newPos) { vPosition = newPos; if(pBVolume)pBVolume->SetPosition(vPosition); }
	void SetRotation (const tgQuaternion& newRot) { qRotation = newRot; }
	void SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot);
	
	tgBoundingVolume* GetBoundingVolume () { return pBVolume; }
	void SetBoundingVolume (tgBoundingVolume* volume) { pBVolume = volume; }
	
	const tgVector3& GetPosition () { return vPosition; }
	const tgQuaternion& GetRotation () { return qRotation; }

  void SetPosOffset (const tgVector3& v) { vPosOffset = v; }
  const tgVector3& GetPosOffset () { return vPosOffset; }
 protected:
	tgVector3 vPosition;
	tgQuaternion qRotation;
  tgVector3 vPosOffset;
	//tgAABoundingBox* pBBox;
	tgBoundingVolume* pBVolume;
 
 
 
 protected:
	struct _Joint;
 public:
	tgModel(tgModelCache* c);
	~tgModel ();
	
	/**
	* @see tgIModel::SetVisible
	*/ 
	void SetVisible(bool b) { bVisible = b; }
	
	/**
	* @see tgIModel::SetAnimation
	*/
	void SetAnimation(const tgstring& animName, int channel=-1, bool repeat=true, bool breakCurrent=true);
	/**
	 * @see tgIModel::SetViewModel
	 */
	void SetViewModel(bool b);
	bool IsViewModel();
	
	const tgVector3& GetOriginPosition();

	/**
	* Anime le modèle pour un temps de eTime
	* @param eTime le temps écoulé
	*/
	void Animate(float eTime);
	
	/**
	* Permet de savoir si l'animation en cours est finie
	* @return true si l'animation est finie
	*/
	inline bool IsCurrentAnimationFinished (int i) { return (pChannels[i].iFrame >= pData->anim_GetNumFrames(pChannels[i].iCurrentAnimation)); }
	
	inline int GetJointIndexByName(const tgstring& jName) {
		for(int i=0; i<iNumJoints; i++) {
			if (pJoints[i].sName == jName)
				return i;
		}
		return -1;
	}
	
	inline tgVector3 GetJointPosition (const tgstring& jName) { if (GetJointIndexByName(jName) == -1) return tgVector3(); else return pJoints[GetJointIndexByName(jName)].vPosition; }
	inline tgQuaternion GetJointOrientation (const tgstring& jName) { if (GetJointIndexByName(jName) == -1) return tgQuaternion(); else return pJoints[GetJointIndexByName(jName)].qOrientation; }
	//Utile pour calculer la silhouette via des shaders
	//void FillEdgeModel();
	void ComputeSilhouette(const tgVector3& lightPos);
	
	
 protected:
 	bool bVisible;
	bool bViewModel;
 
	void _BuildBone (_Joint*, const tgQuaternion&, const tgVector3&);
	void _SkinMesh ();
	
	bool bAnimated;
	
	tgModelCache* pData;
	
	//std::vector<Edge *> visibleEdges;
	
	_Joint* pJoints;
	_Joint** pBaseJoints;
	int iNumJoints;
	int iNumBaseJoints;
	
	tgVertice**  pVerticesListTab;
	tgEdgeVertice**  pEdgeVerticesListTab;

  //indique si un mesh doit etre affiche ou non - utilise pour ne pas afficher les fx/flash sur les armes
  bool* pVisibleMeshes;
	
	struct _Edge
	{
		int pVertIndexes[2];
		int pTriangleIndexes[2];
		bool bCCW;
		int iMesh;
	};
	
	tgvector<_Edge> visibleEdges;
	tgMaterial** pMaterialTab;
	int** pIndexesListTab;
	
	int iNumEdgesVerts;
	
	int iNumMeshes;
	
	//animation
	float fPreviousTime;
	float fCurrentTime;
	
	struct _AnimState
	{
		_AnimState():iAnimationIndex(-1), bRepeat(false) {}
		_AnimState(int iA, bool bR):iAnimationIndex(iA), bRepeat(bR) {}
		int iAnimationIndex;
		bool bRepeat;
	};

	/**
	* Chaque bone est animé par un seul channel (spécifié dans le .mdf)
	*/
	struct _Channel
	{
		int iFrame;
		int iCurrentAnimation;
		tgstack<_AnimState> animStack;
	};
	
	_Channel* pChannels; //tableau de gameImport.iNumChannels+1 (le +1 c'est pour le channel 'all' qui est forcement present)
	//int* pChannelFrames; //un tableau de gameImport.iNumChannels, donnant le nombre de frame courant de chaque channel
	//int* pCurrentAnim; //un tableau de gameImport.iNumChannels, donnant les index des animations des différents channels
	
	
	//tgstack<_AnimState>* pAnimQueues; //on utilise une pile LIFO pour nos animations(une pile par channel) + une pour le channel général
 
 
	struct _Joint
	{
		std::string sName;
		int iNumber;
		int iParent;
		int iChannel;
		tgVector3 vPosition;
		tgQuaternion qOrientation;
		_Joint* pParent;
		_Joint** pChildrens;
		int iNumChildrens;
		//tgvector<SceneNode*> vNodes;
	};
};

#endif
