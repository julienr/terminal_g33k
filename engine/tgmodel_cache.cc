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
#include "tgmodel_cache.h"
#include "tgdefs.h"

tgModelCache::tgModelCache(const char* defFile)
{
	iNumJoints = 0;
	iNumBaseJoints = 0;
	iNumMeshes = 0;
	if (_ExecuteMDF(defFile))
		bLoaded = true;
	else
		bLoaded = false;

}

tgModelCache::~tgModelCache ()
{
	for(int i=0; i<iNumJoints; i++) {
		delete [] pJoints[i].pChildrens;
	}
	delete [] pJoints;

	for (int i=0; i<iNumMeshes; i++)
	{
		delete [] pMeshes[i].pVerts;
		delete [] pMeshes[i].pTriangles;
		delete [] pMeshes[i].pIndexes;
		delete [] pMeshes[i].pWeights;
		delete [] pMeshes[i].pEdges;
		//delete pMeshes[i].pMaterial;
	}
	delete [] pMeshes;
	for (unsigned int i=0; i<pAnimations.size(); i++)
	{
		_MD5Anim& anim = pAnimations[i];
		for (int j=0; j<anim.iNumJoints; j++)
			delete [] anim.pBaseFrame[j];
		delete [] anim.pBaseFrame;
		delete [] anim.pJointInfos;
		
		for (int j=0; j<anim.iNumFrames; j++)
		{
			delete [] anim.pQuaternions[j];
			delete [] anim.pPositions[j];
		}
		delete [] anim.pQuaternions;
		delete [] anim.pPositions;
		//delete [] anim.pBBox;
	}
	delete [] pBaseJoints;
	pAnimations.clear();
	
}

#define NEXT(str) while(buffer.find(str)==tgstring::npos && !pFileManager->EndOfFile(file)) \
			pFileManager->Read(file, buffer);

bool tgModelCache::_ExecuteMDF (const char* mdfFile)
{
	int file = pFileManager->Open(mdfFile, TGFILE_TEXT);
	tgstring buffer;
	tgstring buffer2;
	
	if (file == -1) {
		tgerr << "Erreur lors de l'ouverture de " << mdfFile << endl;
	}
	
	NEXT("mesh")
	pFileManager->Read(file, buffer);
	_LoadMesh(buffer.data());
	//TODO: on ignore les channels pour l'instant
	
	NEXT("anim")
	if (pFileManager->EndOfFile(file)) {
		tgout << "Pas d'animations" << endl;
		return true;
	}
	do { //on boucle sur le chargement des anim
		pFileManager->Read(file, buffer);
		pFileManager->Read(file, buffer2);
		_LoadAnim(buffer, buffer2);

		pFileManager->Read(file, buffer);
	} while (buffer.find("anim") != tgstring::npos);
	
	NEXT("channel")
	if (pFileManager->EndOfFile(file)) {
		tgout << "Pas de channels" << endl;
		return true;
	}
	
	do {
		pFileManager->Read(file, buffer);
		for (int i=0; i<gameImport.iNumChannels; i++) {
			//lecture des channels
			if (buffer.find(gameImport.pChannelsName[i]) != tgstring::npos) {
				NEXT("(")
				while (1) { //on boucle jusqu'� la fin de la parenth�se
					pFileManager->Read(file, buffer);
					if (buffer.find(")") != tgstring::npos)
						break;
					
					if (buffer.data()[0] == '*') { //une etoile signifie qu'on doit mettre le channel sur le joint et tout ses enfants
						buffer.erase(0,1); //on efface l'etoile
						int index = _FindJointIndexByName(buffer);
						if (index == -1)
							continue;
						_RecursiveSetChannel(i, &pJoints[index]);
						//out << "** " << buffer << "\t" << gameImport.pChannelsName[i] << endl;
					} else {
						int index = _FindJointIndexByName(buffer);
						if (index == -1)
							continue;
						pJoints[index].iChannel = i;
						//out << buffer << "\t" << gameImport.pChannelsName[i] << endl;
					}
				}
			NEXT("channel")
			}
		}
	} while (buffer.find("channel") != tgstring::npos);
	
	/*for(int i=0; i<iNumJoints; i++) {
		cout << pJoints[i].sName << "\t" << pJoints[i].iChannel << endl;
	}*/
	
	
	_PreCalculateWeightsNormals();
	return true;
}

//Dhenry de www.doom3world.org -->
// der_ton said:
//
// * First you have to get the bind-pose model-space normals by calculating
//   them from the model geometry in bind-pose.
//
// * Then you calculate the weight's normal (which is in bone-space) by
//   invert-transforming the normal by the bone-space matrix.
//
// * So afterwards when animating, you'll transform the weight normal with
//   the animated bone-space matrix and add them all up and you'll get
//   back your animated vertex normal. 
void tgModelCache::_PreCalculateWeightsNormals()
{
	for (int k=0; k<iNumMeshes; k++) {
		const _MD5Mesh& m = pMeshes[k];
		tgvector<tgVector3> bindposVerts (m.iNumVerts);
		tgvector<tgVector3> bindposNorms (m.iNumVerts);
		for (int i=0;i< m.iNumVerts; i++) {
			bindposVerts[i].Set(0,0,0);
			bindposNorms[i].Set(0,0,0);
			
			for(int j=0; j<m.pVerts[i].iNumWeights; j++) {
				const _MD5Weight* pWeight = &m.pWeights[m.pVerts[i].iWeightIndex + j];
				const _MD5Joint* pJoint = &pJoints[pWeight->iBoneIndex];
				// Calculate transformed vertex for this weight 
				tgVector3 wv = pWeight->vWeights;
				wv = pJoint->qOrientation.Rotate(wv);
				bindposVerts[i] += (pJoint->vPosition + wv)*pWeight->fBias;
			}
		}
		
		//Compute triangle normals
		for (int i=0; i<m.iNumTris;i++) {
			const _MD5Triangle* pTriangle = &m.pTriangles[i];
			tgPlane plane;
			plane.SetFromPoints(bindposVerts[pTriangle->pIndex[0]],
					    bindposVerts[pTriangle->pIndex[2]],
					    bindposVerts[pTriangle->pIndex[1]]);
			tgVector3 triNorm (plane.GetNormal());
			
			
			for(int j=0; j<3; j++) {
				bindposNorms[pTriangle->pIndex[j]] += triNorm;
			}
		}
		
		// "Average" the surface normals, by normalizing them
		for(int i=0; i<m.iNumVerts; i++) {
			bindposNorms[i].Normalize();
		} 
		
		//
  		// At this stage we have all vertex normals computed
  		// for the model geometry in bind-pos
		// 
		
		// Zero out all weight normals
		for (int i=0; i<m.iNumWeights; i++) {
			m.pWeights[i].vNormal.Set(0,0,0);
			
		}
		
  		// Compute weight normals by invert-transforming the normal
 		 // by the bone-space matrix 
		
		for(int i=0;i<m.iNumVerts;i++) {
			for(int j=0;j<m.pVerts[i].iNumWeights;j++) {
				_MD5Weight* pWeight = &m.pWeights[m.pVerts[i].iWeightIndex+j];
				const _MD5Joint* pJoint = &pJoints[pWeight->iBoneIndex];
				
				tgVector3 wn = bindposNorms[i];
				//Compute inverse quaternion rotation
				tgQuaternion inv = pJoint->qOrientation.Inverse();
				wn = inv.Rotate(wn);
				
				pWeight->vNormal += wn;
			}
		}
		
		for(int i=0; i<m.iNumWeights; i++) {
			m.pWeights[i].vNormal.Normalize();
		}
	}
}

bool tgModelCache::_LoadMesh(const char* filename)
{
	int file = pFileManager->Open(filename, TGFILE_TEXT);
	tgstring buffer;
	int version;
	if (file == -1) {
		tgerr << "Erreur lors de l'ouverture de " << filename << endl;
		return false;
	}
	
	pFileManager->Read(file, buffer);
	pFileManager->Read(file, version);
	if (version != 10) {
		tgerr << "Mauvaise version : " << version << endl;
		return false;
	}
	
	NEXT("numJoints")
	pFileManager->Read(file, iNumJoints);
	
	NEXT("numMeshes")
	pFileManager->Read(file, iNumMeshes);
	
	iNumBaseJoints = 0;
	pJoints = new _MD5Joint[iNumJoints];

	
	for (int i=0; i<iNumJoints; i++) {
		NEXT("\"")
		pJoints[i].sName = buffer;
		pJoints[i].iChannel = /*gameImport.iNumChannels*/-1; //on met sur le channel "all" par defaut
		pJoints[i].sName.erase(0,1); //on efface le premier "
		size_t last = pJoints[i].sName.find_last_of('\"');
		pJoints[i].sName.erase(last);
		
		
		pFileManager->Read(file, pJoints[i].iParent);
		if (pJoints[i].iParent < 0)
			iNumBaseJoints++;
		NEXT("(")
		pFileManager->Read(file, pJoints[i].vPosition.x);
		pFileManager->Read(file, pJoints[i].vPosition.y);
		pFileManager->Read(file, pJoints[i].vPosition.z);
	
		pJoints[i].vPosition.ChangeCoords();
		
		NEXT(")")
		NEXT("(")
		tgQuaternion& q = pJoints[i].qOrientation;
		pFileManager->Read(file, q.x);
		pFileManager->Read(file, q.y);
		pFileManager->Read(file, q.z);
		tgMath::ChangeCoords<float>(q.x,q.y,q.z);
		
		q.ComputeR();
	}
	
	//BEGIN Creation des liens parents-enfant
	pBaseJoints = new _MD5Joint*[iNumBaseJoints];
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
	
	//creation du tableau contenant les pointeurs sur les joint enfant
	for (int i=0; i<iNumJoints; i++) 
			pJoints[i].pChildrens = new _MD5Joint*[pJoints[i].iNumChildrens];
	
	for (int i=0; i<iNumJoints; i++)
	{
		if (pJoints[i].pParent)
		{
			pJoints[i].pParent->pChildrens[tmpCount[pJoints[i].iParent]] = &pJoints[i]; //on ajoute ce bone au tableau d'enfants du parent
			tmpCount[pJoints[i].iParent]++; //on increment tmpCount du bone parent
		}
	}
	delete [] tmpCount;
	//END Creation des liens parents-enfant
	
	pMeshes = new _MD5Mesh[iNumMeshes];
	for (int i=0; i<iNumMeshes; i++)
	{
		NEXT("shader")
		tgstring sTmp;
		pFileManager->Read(file, sTmp);
		
		size_t iFirstQuote = sTmp.find_first_of("\"");
		size_t iLastQuote = sTmp.find_last_of("\"");
		iFirstQuote += 1; //on veut le caractere juste apres le dernier slash
		sTmp.erase(iLastQuote);
    pMeshes[i].sMatName = sTmp.substr(iFirstQuote);
		/*pMeshes[i].sNormal = "models/textures/"+sTmp.substr(iLastSlash);
		pMeshes[i].sColor = pMeshes[i].sNormal+"_color";
		pMeshes[i].sNormal += "_normal";*/
		//out << pMeshes[i].sColor << "\t" << pMeshes[i].sNormal << endl;
		/*
		pMeshes[i].pMaterial = new Material ();
		Texture* tmpTex = Engine::pEngine->pTextureManager->LoadTexture((char*)pMeshes[i].sNormal.data(), TG_TEXTURE_2D);
		
		pMeshes[i].pMaterial->SetNormalTexture(tmpTex);
		tmpTex = Engine::pEngine->pTextureManager->LoadTexture((char*)pMeshes[i].sColor.data(), TG_TEXTURE_2D);
		pMeshes[i].pMaterial->SetFirstTexture(tmpTex);*/
		
		
		NEXT("numverts")
		pFileManager->Read(file, pMeshes[i].iNumVerts);
		pFileManager->Read(file, buffer); //sans ca, le NEXT("vert") ne va pas bouger car vert est deja dans numverts
		
		pMeshes[i].pVerts = new _MD5Vert[pMeshes[i].iNumVerts];
		for (int j=0; j<pMeshes[i].iNumVerts; j++)
		{
			_MD5Vert& vert = pMeshes[i].pVerts[j];
			NEXT("vert")
			NEXT("(")
			pFileManager->Read(file, vert.pTexCoords[0]);
			pFileManager->Read(file, vert.pTexCoords[1]);
			vert.pTexCoords[1] = 1 - vert.pTexCoords[1];
			NEXT(")")
			pFileManager->Read(file, vert.iWeightIndex);
			pFileManager->Read(file, vert.iNumWeights);
			
		}
		NEXT("numtris")
		pFileManager->Read(file, pMeshes[i].iNumTris);
		
		pMeshes[i].pTriangles = new _MD5Triangle[pMeshes[i].iNumTris];
		pMeshes[i].pIndexes = new unsigned int[pMeshes[i].iNumTris*3];
		int iC = 0;
		pFileManager->Read(file, buffer); //sans ca, le NEXT("tri") ne va pas bouger car tri est deja dans numtris
		for (int j=0; j<pMeshes[i].iNumTris; j++)
		{
			_MD5Triangle& tri = pMeshes[i].pTriangles[j];
			NEXT("tri")
			pFileManager->Read(file, buffer);
			pFileManager->Read(file, tri.pIndex[0]);
			pFileManager->Read(file, tri.pIndex[1]);
			pFileManager->Read(file, tri.pIndex[2]);
			
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[0];
			iC++;
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[1];
			iC++;
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[2];
			iC++;
		}
		
		NEXT("numweights")
		pFileManager->Read(file, pMeshes[i].iNumWeights);
		pFileManager->Read(file, buffer); // sans ca, le NEXT("weight") foire
		pMeshes[i].pWeights = new _MD5Weight[pMeshes[i].iNumWeights];
		for (int j=0; j<pMeshes[i].iNumWeights; j++)
		{
			_MD5Weight& w = pMeshes[i].pWeights[j];
			NEXT("weight")
			pFileManager->Read(file, buffer);
			pFileManager->Read(file, w.iBoneIndex);
			pFileManager->Read(file, w.fBias);
			NEXT("(")
			pFileManager->Read(file, w.vWeights.x);
			pFileManager->Read(file, w.vWeights.y);
			pFileManager->Read(file, w.vWeights.z);
			w.vWeights.ChangeCoords();
		}
		
		pMeshes[i].SetConnectivity();
	}
	pFileManager->Close(file);
	tgout << "Chargement de " << filename << "  termine" << endl;
	return true;
}

bool tgModelCache::_LoadAnim(const tgstring& name, const tgstring& filename)
{
	_MD5Anim anim;
	
	int file = pFileManager->Open(filename.data(), TGFILE_TEXT);
	tgstring buffer;
	int version;
	if (file == -1) {
		tgerr << "Erreur lors de l'ouverture de " << filename << endl;
		return false;
	}
	
	pFileManager->Read(file, buffer);
	pFileManager->Read(file, version);
	if (version != 10) {
		tgerr << "Mauvaise version : " << version << endl;
		return false;
	}
	
	anim.sName = name;
	
	NEXT("numFrames")
	pFileManager->Read(file, anim.iNumFrames);
	NEXT("numJoints")
	pFileManager->Read(file, anim.iNumJoints);
	NEXT("frameRate")
	pFileManager->Read(file, anim.iFrameRate);
	NEXT("numAnimatedComponents")
	pFileManager->Read(file, anim.iNumAnimatedComponents);
	
	float** pFrames;
	pFrames = new float*[anim.iNumFrames];
	for (int i=0; i<anim.iNumFrames; i++)
		pFrames[i] = new float[anim.iNumAnimatedComponents];
		
	anim.pJointInfos = new _MD5Anim::_MD5JointInfos[anim.iNumJoints];
	anim.pBaseFrame = new float*[anim.iNumJoints];
	
	//FIXME: a priori, il n'y a pas de md5mesh ou la hierarchie de l'anim est different du md5mesh. Ceci dit, on peut peut-etre utiliser ca pour demembrer ou autre
	NEXT("hierarchy")
	for (int i=0; i<anim.iNumJoints; i++)
	{
		NEXT("\"")
		buffer = "";
		pFileManager->Read(file, anim.pJointInfos[i].iParent);
		pFileManager->Read(file, anim.pJointInfos[i].iFlags);
		pFileManager->Read(file, anim.pJointInfos[i].iStartIndex);
	}
	/*
	anim.pBBox = new BoundingBox[anim.iNumFrames];
	NEXT("bounds")
	for (int i=0; i<anim.iNumFrames; i++)
	{
		NEXT("(")
		pFD >> anim.pBBox[i].vMin.x >> anim.pBBox[i].vMin.y >> anim.pBBox[i].vMin.z;
		Util::ChangeCoords(anim.pBBox[i].vMin);
		NEXT(")")
		NEXT("(")
		pFD >> anim.pBBox[i].vMax.x >> anim.pBBox[i].vMax.y >> anim.pBBox[i].vMax.z;
		Util::ChangeCoords(anim.pBBox[i].vMax);
		NEXT(")")	
	}*/
	
	//chargement de baseframe
	NEXT("baseframe")
	for (int i=0; i<anim.iNumJoints; i++)
	{
		anim.pBaseFrame[i] = new float[6];
		NEXT("(")
		pFileManager->Read(file, anim.pBaseFrame[i][0]);
		pFileManager->Read(file, anim.pBaseFrame[i][1]);
		pFileManager->Read(file, anim.pBaseFrame[i][2]);
		NEXT(")")
		NEXT("(")
		pFileManager->Read(file, anim.pBaseFrame[i][3]);
		pFileManager->Read(file, anim.pBaseFrame[i][4]);
		pFileManager->Read(file, anim.pBaseFrame[i][5]);
		NEXT(")")
	}
	
	//chargement des frames
	for (int i=0; i<anim.iNumFrames; i++)
	{
		NEXT("frame")
		NEXT("{")
		for (int j=0; j<anim.iNumAnimatedComponents; j++)
			pFileManager->Read(file, pFrames[i][j]);
		NEXT("}")
	}
	
	anim.pQuaternions = new tgQuaternion*[anim.iNumFrames];
	for (int i=0; i<anim.iNumFrames; i++)
		anim.pQuaternions[i] = new tgQuaternion[anim.iNumJoints];
	
	anim.pPositions = new tgVector3*[anim.iNumFrames];
	for (int i=0; i<anim.iNumFrames; i++)
		anim.pPositions[i] = new tgVector3[anim.iNumJoints];

	for (int i=0; i<anim.iNumFrames; i++)
		for (int j=0; j<anim.iNumJoints; j++)
		{
			int n = anim.pJointInfos[j].iStartIndex;
			int flags = anim.pJointInfos[j].iFlags;

			//Tx
			if (flags & 1)
			{
				anim.pPositions[i][j].x = pFrames[i][n];
				n++;
			}
			else
				anim.pPositions[i][j].x = anim.pBaseFrame[j][0];

			//Ty
			if (flags & 2)
			{
				anim.pPositions[i][j].y = pFrames[i][n];
				n++;
			}
			else
				anim.pPositions[i][j].y = anim.pBaseFrame[j][1];

			//Tz
			if (flags & 4)
			{
				anim.pPositions[i][j].z = pFrames[i][n];
				n++;
			}
			else
				anim.pPositions[i][j].z = anim.pBaseFrame[j][2];

			//Qx
			if (flags & 8)
			{
				anim.pQuaternions[i][j].x = pFrames[i][n];
				n++;
			}
			else
				anim.pQuaternions[i][j].x = anim.pBaseFrame[j][3];
			
			//Qy
			if (flags & 16)
			{
				anim.pQuaternions[i][j].y = pFrames[i][n];
				n++;
			}
			else
				anim.pQuaternions[i][j].y = anim.pBaseFrame[j][4];

			//Qz
			if (flags & 32)
			{
				anim.pQuaternions[i][j].z = pFrames[i][n];
				n++;
			}
			else
				anim.pQuaternions[i][j].z = anim.pBaseFrame[j][5];

			tgMath::ChangeCoords<float>(anim.pQuaternions[i][j].x, anim.pQuaternions[i][j].y, anim.pQuaternions[i][j].z);
			anim.pPositions[i][j].ChangeCoords();
			anim.pQuaternions[i][j].ComputeR();
				
		}
	
	for (int i=0; i<anim.iNumFrames; i++)
		delete [] pFrames[i];
	delete [] pFrames;
	pFileManager->Close(file);
	
	pAnimations.push_back(anim);
	
	tgout << "Chargement de " << filename << "  termine" << endl;

	return true;
}

int tgModelCache::GetAnimIndex(const std::string& name)
{
	unsigned int size = (unsigned int)pAnimations.size();
	for (unsigned int i=0; i<size; i++) {
		if (pAnimations[i].sName == name)
			return (int)i;
	}
	return -1;
}

int tgModelCache::_FindJointIndexByName(const tgstring& name)
{
	for (int i=0; i<iNumJoints; i++) {
		if (pJoints[i].sName.find(name) != tgstring::npos)
			return i;
	}
	return -1;
}

void tgModelCache::_RecursiveSetChannel(int channel, _MD5Joint* joint)
{
	joint->iChannel = channel;
	for (int i=0; i<joint->iNumChildrens; i++) {
		_RecursiveSetChannel(channel, joint->pChildrens[i]);
	}
}

void tgModelCache::_MD5Mesh::SetConnectivity()
{
	tgvector<_MD5Edge> edgesList;
	_MD5Triangle* tri;
	for (int i=0; i<iNumTris; i++)
	{
		tri = &pTriangles[i];
		if (tri->pIndex[0] < tri->pIndex[1])
			_AddEdge (tri->pIndex[0], tri->pIndex[1], i,edgesList);
			
		if (tri->pIndex[1] < tri->pIndex[2])
			_AddEdge (tri->pIndex[1], tri->pIndex[2], i,edgesList);

		if (tri->pIndex[2] < tri->pIndex[0])
			_AddEdge (tri->pIndex[2], tri->pIndex[0], i,edgesList);

	}
		
	for (int i=0; i<iNumTris; i++)
	{
		tri = &pTriangles[i];
		if (tri->pIndex[0] > tri->pIndex[1])
			_FindCreateMatchingEdge (tri->pIndex[0], tri->pIndex[1], i,edgesList);
		if (tri->pIndex[1] > tri->pIndex[2])
			_FindCreateMatchingEdge (tri->pIndex[1], tri->pIndex[2], i,edgesList);
		if (tri->pIndex[2] > tri->pIndex[0])
			_FindCreateMatchingEdge (tri->pIndex[2], tri->pIndex[0], i,edgesList);
	}
	
	iNumEdges = (unsigned int)edgesList.size();
	pEdges = new _MD5Edge[iNumEdges];
	for(int i=0; i<iNumEdges; i++)
		pEdges[i] = edgesList[i];
	edgesList.clear();
}

void tgModelCache::_MD5Mesh::_AddEdge(int v1, int v2, int tri, tgvector<_MD5Edge>& edgesList)
{
	_MD5Edge edge;
	edge.pTriangleIndexes[0] = tri;
	edge.pTriangleIndexes[1] = -1;
	edge.pVertIndexes[0] = v1;
	edge.pVertIndexes[1] = v2;
	edgesList.push_back(edge);
}

void tgModelCache::_MD5Mesh::_FindCreateMatchingEdge(int v1, int v2, int tri, tgvector<_MD5Edge>& edgesList)
{
	//std::cout << "recherche pour : \t" << v1 << "\t" << v2 << std::endl;
	for (unsigned int i=0; i<edgesList.size(); i++)
	{
		//std::cout << edges[i].pVertIndexes[0] << "\t" << edges[i].pVertIndexes[1] << std::endl;
		if ((edgesList[i].pVertIndexes[0] == v2 && edgesList[i].pVertIndexes[1] == v1))
		{
			if (edgesList[i].pTriangleIndexes[1] == -1 /*|| edges[i].pTriangleIndexes[1]==tri*/)
			{
				edgesList[i].pTriangleIndexes[1] = tri;
				return;
			}
			#ifdef DEBUG
			else
			{
				//std::cout << "edge > 2 triangles : v1=" << v1 << ", v2=" << v2 << " ,tri=" << tri << std::endl;
			}
			#endif
		}
	}
	//on a pas trouv� d'edge correspondant, donc on en cree un nouveau
	_AddEdge (v1, v2, tri,edgesList);
}


