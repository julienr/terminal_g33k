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
#include "tgmodel.h"

tgModel::tgModel(tgModelCache* c)
{
	bVisible = true;
	bViewModel = false;
	pBVolume = NULL;
	
	{
		fPreviousTime = 0;
		fCurrentTime = 0;
	
		bAnimated = false;
		pData = c;
		iNumJoints = pData->GetNumJoints();
		iNumBaseJoints = pData->GetNumBaseJoints();
		iNumMeshes = pData->GetNumMeshes();

		pJoints = new _Joint[iNumJoints];
		pBaseJoints = new _Joint*[iNumBaseJoints];
		
		/*pAnimQueues = new tgqueue<_AnimState>[gameImport.iNumChannels+1];
		pCurrentAnim = new int[gameImport.iNumChannels+1];
		pChannelFrames = new int[gameImport.iNumChannels+1];
		for (int i=0; i<=gameImport.iNumChannels; i++) {
			pCurrentAnim[i] = -1;
			pChannelFrames[i] = -1;
		}*/
		
    //FIXME: On pourrait charger les material dans le model_cache
		pMaterialTab = new tgMaterial*[iNumMeshes];
    pVisibleMeshes = new bool[iNumMeshes];
		for (int i=0; i<iNumMeshes; i++) {
			pMaterialTab[i]=pMaterialManager->GetMaterial(pData->mesh_GetMatName(i));
      //FIXME: ugly hack pour ne pas rendre les effets speciaux tant que la gestion des materials n'est pas 
      // complete
      if (pData->mesh_GetMatName(i).find("flash") != tgstring::npos ||
          pData->mesh_GetMatName(i).find("fx") != tgstring::npos) {
        tgout << "tgModel::tgModel flash or fx material - skipped at rendering - " << pData->mesh_GetMatName(i) << endl; 
        pVisibleMeshes[i] = false;
      } else {
        pVisibleMeshes[i] = true;
      }
		}
		
		pChannels = new _Channel[gameImport.iNumChannels+1];
		for (int i=0; i<=gameImport.iNumChannels; i++) {
			pChannels[i].iFrame = 0;
			pChannels[i].iCurrentAnimation = -1;
		}
		
		pVerticesListTab = new tgVertice*[iNumMeshes];
		pEdgeVerticesListTab = new tgEdgeVertice*[iNumMeshes];
		for (int i=0; i<iNumMeshes; i++) {
			pVerticesListTab[i] = new tgVertice[pData->mesh_GetNumVerts(i)];
			pEdgeVerticesListTab[i] = new tgEdgeVertice[pData->mesh_GetNumEdges(i)*2];
		}
			
		
		//copie des bones de Data
		for (int i=0; i<iNumJoints; i++)
		{
			pJoints[i].sName = pData->GetJointName(i);
			pJoints[i].iNumber = i;
			pJoints[i].iChannel = pData->GetJointChannel(i);
			pJoints[i].iParent = pData->GetJointParent(i);
			pJoints[i].vPosition = pData->GetJointPosition(i);
			pJoints[i].qOrientation = pData->GetJointOrientation(i);
		}
		
		//Creation des liens parents-enfants
		//mise a zero du nombre d'enfants des joints
		int* tmpCount = new int [iNumJoints]; //juste pour savoir combien de joints enfants on deja ete ajoutes a chaque joint
		for (int i=0; i<iNumJoints; i++)
		{
			pJoints[i].iNumChildrens = 0;
			tmpCount[i] = 0;
		}
	
		//recherche des pointeurs sur les joints parents
		//incrementation du nombre de joint enfant sur le parent
		int bIndex = 0;
		for (int i=0; i<iNumJoints; i++)
		{
			if (pJoints[i].iParent < 0)
			{
				pBaseJoints[bIndex] = &pJoints[i];
				pBaseJoints[i]->pParent = NULL;
				bIndex++;
				continue;
			}
			else
			{
				pJoints[i].pParent = &pJoints[pJoints[i].iParent];
				pJoints[i].pParent->iNumChildrens++;
			}
		}
		
		//creation du tableau contenant les pointeurs sur les joints enfant
		for (int i=0; i<iNumJoints; i++) 
				pJoints[i].pChildrens = new _Joint*[pJoints[i].iNumChildrens];
		
		for (int i=0; i<iNumJoints; i++)
		{
			if (pJoints[i].pParent)
			{
				pJoints[i].pParent->pChildrens[tmpCount[pJoints[i].iParent]] = &pJoints[i]; //on ajoute ce bone au tableau d'enfants du parent
				tmpCount[pJoints[i].iParent]++; //on increment tmpCount du bone parent
			}
		}
		delete [] tmpCount;
		_SkinMesh();
	}
	
	{
		vPosition.Set(0,0,0);
    vPosOffset.Set(0,0,0);
		qRotation.SetRotation(0, tgVector3(0,0,1));
	}
}

tgModel::~tgModel ()
{
	for (int i=0; i<iNumJoints; i++)
		delete [] pJoints[i].pChildrens;
	delete [] pJoints;
	delete [] pBaseJoints;
	for (int i=0; i<pData->GetNumMeshes(); i++) {
		delete [] pVerticesListTab[i];
		delete [] pEdgeVerticesListTab[i];
	}
	
	delete [] pVerticesListTab;
	delete [] pEdgeVerticesListTab;
	delete [] pMaterialTab;
	delete [] pChannels;
  delete [] pVisibleMeshes;
	
	/*delete [] pAnimQueues;
	delete [] pCurrentAnim;
	delete [] pChannelFrames;*/
}

void tgModel::SetViewModel(bool b)
{
	bViewModel = b;
}

bool tgModel::IsViewModel()
{
	return bViewModel;
}

const tgVector3& tgModel::GetOriginPosition()
{
	//On considere que le bone d'index 0 represente l'origin du model
	if(iNumJoints > 0) {
		return pJoints[0].vPosition;
	} else {
		return tgVector3::ZERO;
	}
}

void tgModel::SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot)
{
	int jointIndex = GetJointIndexByName(jointName);
	if (jointIndex >= 0) { //le joint existe
		//on fixe la rotation, on devrait peut-etre plutot multiplier 
		pJoints[jointIndex].qOrientation = newRot;
	}
	
}

void tgModel::SetAnimation(const tgstring& animName, int channel, bool repeat, bool breakCurrent)
{
	bAnimated = true;
	_AnimState state(pData->GetAnimIndex(animName), repeat);

	if (channel == -1) //on bind sur le dernier channel, à savoir le 'all' par défaut
			channel = gameImport.iNumChannels;
			
	_Channel& chan = pChannels[channel];
	if (breakCurrent) { //si on doit arrêter l'animation en cours
		while(!chan.animStack.empty())
			chan.animStack.pop(); //on vide
		
		chan.animStack.push(state); //on ajoute l'animation
		chan.iCurrentAnimation = state.iAnimationIndex; //on met cette animation comme courante
		chan.iFrame = 0;
	} else { //sinon, on se contente de rajouter sur la pile
		chan.animStack.push(state);
		
		if (chan.iCurrentAnimation == -1 || chan.iCurrentAnimation == pChannels[gameImport.iNumChannels].iCurrentAnimation) { //si il n'y a pas d'animation en cours ou que l'animation en cours est celle du chan general
			chan.iCurrentAnimation = state.iAnimationIndex;
			chan.iFrame = 0;
		}
	}
}

void tgModel::_SkinMesh ()
{
	tgVector3 pos;
	tgVector3 norm;
	for (int i=0; i<pData->GetNumMeshes(); i++)
	{
		for (int j=0; j<pData->mesh_GetNumVerts(i); j++)
		{
			pVerticesListTab[i][j].vPosition.Set(0,0,0);
			pVerticesListTab[i][j].vNormal.Set(0,0,0);
			//assignation des texcoords.
			//FIXME: On pourrait éviter ça et ne le faire qu'une seule fois
			pVerticesListTab[i][j].s = pData->vert_GetTexCoords(i,j)[0];
			pVerticesListTab[i][j].t = pData->vert_GetTexCoords(i,j)[1];

			for (int k=0; k<pData->vert_GetNumWeights(i, j); k++) 
			{
				int baseIndex = pData->vert_GetWeightIndex(i,j);
				int boneIndex = pData->weight_GetBoneIndex(i, baseIndex+k);
			
				pos = pJoints[boneIndex].qOrientation.Rotate(pData->weight_GetWeight(i, baseIndex+k));
				norm = pJoints[boneIndex].qOrientation.Rotate(pData->weight_GetNormal(i, baseIndex+k));
				
 				pVerticesListTab[i][j].vPosition += (pos + pJoints[boneIndex].vPosition) * pData->weight_GetBias(i, baseIndex+k);
				pVerticesListTab[i][j].vNormal += (norm)*pData->weight_GetBias(i, baseIndex+k);
			}
		}
	}
}

void tgModel::_BuildBone (_Joint* pJoint,const tgQuaternion& q, const tgVector3& v)
{
	_Channel& chan = pChannels[(pJoint->iChannel == -1)?gameImport.iNumChannels:pJoint->iChannel];
	
	if (chan.iCurrentAnimation == -1) { //si le channel de ce bone n'a pas d'animation, on ne return _PAS_, on passe simplement l'animation de ce bone. Il est possible que des enfants de ce bone aient une animation
	} else {
		tgVector3 animatedPosition = pData->anim_GetPosition(chan.iCurrentAnimation, chan.iFrame, pJoint->iNumber);
		tgQuaternion animatedOrientation = pData->anim_GetQuaternion(chan.iCurrentAnimation, chan.iFrame, pJoint->iNumber);
		if (pJoint->iParent < 0) //pas de parent
		{
			pJoint->vPosition = animatedPosition;
			pJoint->qOrientation = animatedOrientation;
		}
		else //parent
		{
			pJoint->vPosition = q.Rotate(animatedPosition);
			pJoint->vPosition += v;
			pJoint->qOrientation = q*animatedOrientation;
		}
	}
	/*for (unsigned int i=0; i<pJoint->vNodes.size(); i++)
	{
		pJoint->vNodes[i]->SetPosition(vFinalPosition+pJoint->vPosition);
		pJoint->vNodes[i]->SetRotation(qFinalRotation*pJoint->qOrientation);
	}*/
	
	for (int i = 0; i < pJoint->iNumChildrens; i++)
	{
		_BuildBone(pJoint->pChildrens[i], pJoint->qOrientation, pJoint->vPosition);
	}
		
}

void tgModel::Animate(float eTime)
{
	fCurrentTime += eTime;

	/*for (unsigned int i=0; i<vNodes.size(); i++)
	{
		vNodes[i]->SetPosition(vFinalPosition);
		vNodes[i]->SetRotation(qFinalRotation);
	}*/
	
	if (!bAnimated)
		return;
 
  if ((fCurrentTime - fPreviousTime) > /*41.66/1000*/0.041) // 1/24 = 0.0.41666667. Un md5 est anime 24 fois par secondes
	{
		fPreviousTime = fCurrentTime;
		for (int i=0; i<=gameImport.iNumChannels; i++) {  //boucle sur les channels
			_Channel& chan = pChannels[i];
			chan.iFrame++;
			//si le channel est synchronise sur le general, on synchronise les frames, sinon on a des jolis bugs
			if ((i<gameImport.iNumChannels) && (chan.iCurrentAnimation == pChannels[gameImport.iNumChannels].iFrame)) {
				chan.iFrame = pChannels[gameImport.iNumChannels].iFrame;
				continue;
			}
			if (chan.iCurrentAnimation == -1) {// si on a pas d'animation en cours, on ne continue pas sur ce channel
				chan.iCurrentAnimation = pChannels[gameImport.iNumChannels].iCurrentAnimation;
				chan.iFrame = pChannels[gameImport.iNumChannels].iFrame;
				continue;
			}
			if(chan.animStack.empty()) { //si on a plus d'anim, on met a -1 pour eviter d'aller chercher des infos sur les anims
				chan.iCurrentAnimation = -1;
				chan.iFrame = 0;
				continue;
			}
			if (IsCurrentAnimationFinished(i)) { //l'animation en cours est termine
				if (chan.animStack.top().bRepeat) { //on doit la répéter
					//out << "repeating" << endl;
					chan.iFrame = 0;
				}
				else { //sinon, on la supprime
					//out << "sup" << endl;
					if (!chan.animStack.empty())
						chan.animStack.pop();
					
					if (chan.animStack.empty()) { //si on a plus d'animation sur le channel particulier
						//on bind l'animation generale; et on reprends a la frame en cours
						chan.iCurrentAnimation = pChannels[gameImport.iNumChannels].iCurrentAnimation; 
						chan.iFrame = pChannels[gameImport.iNumChannels].iFrame;
					} else { //sinon, on met l'animation suivante
						chan.iCurrentAnimation  = chan.animStack.top().iAnimationIndex;
						chan.iFrame = 0;
					}
				}
			}
		}
			
		for (int i=0; i<iNumBaseJoints; i++)
			_BuildBone(pBaseJoints[i], tgQuaternion(), tgVector3());

		_SkinMesh();

		//FIXME:Doit-on le faire ici ??
		//pBBox = pData->anim_GetBBox(iCurrentAnimation, iFrame);
	}
}
/*
* Utile pour calculer les silhouette via des shaders, pas implemente
void tgModel::FillEdgeModel()
{
	int count = 0;
	for (int i=0; i<pData->GetNumMeshes(); i++)
	{
		tgEdgeVertice* vert = pEdgeVerticesListTab[i];
		for(int j=0; j<pData->mesh_GetNumEdges(i); j++) {
			if (pData->edge_GetTriangleIndexes(i,j)[1] != -1) { //on check que l'edge est bien partage entre deux faces
				vert[0].vPosition = pVerticesListTab[i][pData->edge_GetVertIndexes(i,j)[0]].vPosition;
				vert[1].vPosition = pVerticesListTab[i][pData->edge_GetVertIndexes(i,j)[1]].vPosition;
				
				//Comme texcoord1 et texcoord2, on veut mettre les normals des deux faces qui partange l'edge. Ca nous permet d'extraire la silhouette en hardware via un vertex shader
				//On calcule donc cette normal en creant les deux plans correspondant aux deux faces, on ne peut pas precalculer etant donne que les vertices bougent
				
				tgPlane p1,p2;
				unsigned int* index1 = pData->triangle_GetIndexes(i,pData->edge_GetTriangleIndexes(i,j)[0]);
				unsigned int* index2 = pData->triangle_GetIndexes(i,pData->edge_GetTriangleIndexes(i,j)[1]);
				p1.SetFromPoints(pVerticesListTab[i][index1[0]].vPosition,
						pVerticesListTab[i][index1[1]].vPosition,
						pVerticesListTab[i][index1[2]].vPosition);
				p2.SetFromPoints(pVerticesListTab[i][index2[0]].vPosition,
						pVerticesListTab[i][index2[1]].vPosition,
						pVerticesListTab[i][index2[2]].vPosition);
				vert[0].vNormal1 = p1.GetNormal();
				vert[1].vNormal1 = p1.GetNormal();
				vert[0].vNormal1 = p2.GetNormal();
				vert[1].vNormal1 = p2.GetNormal();
				vert += 2;
				count += 2;
			}
		}
	}
	iNumEdgesVerts = count;
}*/

void tgModel::ComputeSilhouette(const tgVector3& lightPos)
{
	visibleEdges.clear();
	for (int i=0; i<pData->GetNumMeshes(); i++)
	{
		int numTris = pData->mesh_GetNumTris(i);
		bool* triVisible = new bool[numTris];
		for (int j=0; j<numTris; j++)
		{
			unsigned int* indexes = pData->triangle_GetIndexes(i,j);
			tgVector3 vCross1 = pVerticesListTab[i][indexes[2]].vPosition - pVerticesListTab[i][indexes[1]].vPosition;
			tgVector3 vCross2 = pVerticesListTab[i][indexes[1]].vPosition - pVerticesListTab[i][indexes[0]].vPosition;
			tgVector3 vNormal = vCross1 % vCross2;
			vNormal.Normalize();
			
			float angle = vNormal*lightPos;
			triVisible[j] = 0.0f <= angle;
			
			//std::cout << tri.visible << "angle : " << angle << std::endl;
		}
		
		
		for(int j=0; j<pData->mesh_GetNumEdges(i); j++) {
#define e pData->edge_GetTriangleIndexes(i,j)
			//FIXME REVOIR LES CRITERES ?
			if (/*(e[1]<0 && triVisible[e[0]]) ||*/ (e[1]>=0 && (triVisible[e[0]] != triVisible[e[1]]))) {
				_Edge edge;
				if(triVisible[e[0]])
					edge.bCCW = true;
				else
					edge.bCCW = false;
				edge.iMesh = i;
				edge.pTriangleIndexes[0] = pData->edge_GetTriangleIndexes(i,j)[0];
				edge.pTriangleIndexes[1] = pData->edge_GetTriangleIndexes(i,j)[1];
				edge.pVertIndexes[0] = pData->edge_GetVertIndexes(i,j)[0];
				edge.pVertIndexes[1] = pData->edge_GetVertIndexes(i,j)[1];
				visibleEdges.push_back(edge);
			}
		}
		delete [] triVisible;	
	}
}


