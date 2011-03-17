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
#ifndef TGWORLD_MANAGER_H
#define TGWORLD_MANAGER_H

#include "tgiworld_manager.h"
#include "tgdefs.h"
#include "tgdecalmanager.h"
#include "tgq3level.h"
#include "tgmodel.h"
#include "tgtimer.h"
#include "tgparticle_system.h"

#if 0
class tgDoom3Level;
#endif

/**
* Implémentation de tgIWorldManager, utilisé par Game
*/
class tgWorldManager : public tgIWorldManager
{
 public:
	tgWorldManager ();
	virtual ~tgWorldManager ();
	void Initialize (char* mapName = NULL);
	void Manage (float eTime);
	void AddModel (tgIModel* m);
	void RemoveModel(tgIModel* m);

  void AddParticleSystem (tgIParticleSystem* p);
  void RemoveParticleSystem (tgIParticleSystem* p);
  //fonctione appellee par un particle emitter en fin de vie
  void DeleteMe (tgParticleSystem* p);
	
	void MarkFaces (tgAABoundingBox* pBBox);
	
  const tgCollisionResult& Trace(const tgVector3& Start, const tgVector3& End, tgBoundingVolume* volume, tgIEntity* entity, tgset<tgIEntity*>* ignoreList = NULL);

	void GetTouchingEntities (tgBoundingVolume* volume, tgvector<tgIEntity*>& list);
	void AddDecal(tgBoundingSphere* sphere, const tgVector3& norm, const tgstring& matName);
	
	inline tgModel* GetModel (int i) const { return modelList[i]; }
	inline int GetModelCount () const { return (int)modelList.size(); }

  inline tgParticleSystem* GetParticleSystem (int i) const { return systemsList[i]; }
  inline int GetParticleSystemsCount () const { return (int)systemsList.size(); }

  inline tgLight* GetLight (int i) const { return lightList[i]; }
  inline int GetLightsCount () const { return (int)lightList.size(); }
	
	int GetNumStartLocations() { return pQ3Level->GetNumStartLocations(); }
	const tgVector3& GetStartLocation(int index) { return pQ3Level->GetStartLocation(index); }

  void AddLight (tgLight* l);
  void RemoveLight (tgLight* l);
	
	inline tgQ3Level* GetQ3Level () const { return pQ3Level; }
	inline tgDecalManager* GetDecalManager () const { return pDecalManager; }
 protected:
	tgvector<tgModel*> modelList;
  tgvector<tgParticleSystem*> systemsList;
  tgvector<tgLight*> lightList;
	tgQ3Level* pQ3Level;
	tgDecalManager* pDecalManager;
  tgset<tgParticleSystem*> systemsToDelete;
#if 0
	tgDoom3Level* pDoom3Level;
#endif
};

#endif

