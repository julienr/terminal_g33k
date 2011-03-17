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
#include "tgworld_manager.h"

tgWorldManager::tgWorldManager ()
{
	pRenderView = NULL;
/*	int material = pFileManager->Open("data/d3_maps/materials/base_wall.mtr", TGFILE_TEXT);
	tgMaterial* pMat = new tgMaterial(material);
	tgMaterial* pMat2 = new tgMaterial(material);
	pFileManager->Close(material);*/
//	tgImage* image = new tgImage("data/textures/base_wall/a_sopanel2c_d01e.tga");
	/*tgCVar deux(2);
	pConsole->Register(&deux, "deux");
	out << pConsole->Get("deux")->GetStr() << endl;*/
	pDecalManager = new tgDecalManager(10);
}

tgWorldManager::~tgWorldManager ()
{
  //delete particles systems
	for(tgvector<tgParticleSystem*>::iterator i=systemsList.begin(); i!=systemsList.end(); i++) {
    delete *i;
	}

	for(tgvector<tgLight*>::iterator i=lightList.begin(); i!=lightList.end(); i++) {
    delete *i;
	}

	if(pQ3Level != NULL)
		delete pQ3Level;
	delete pDecalManager;
}
void tgWorldManager::Initialize(char* mapName)
{
	#if 0
	pMaterialManager->AddFile("data/scripts/base_button.shader");
	pMaterialManager->AddFile("data/scripts/base_floor.shader");
	pMaterialManager->AddFile("data/scripts/base_light.shader");
	pMaterialManager->AddFile("data/scripts/base_object.shader");
	pMaterialManager->AddFile("data/scripts/base.shader");
	pMaterialManager->AddFile("data/scripts/base_support.shader");
	pMaterialManager->AddFile("data/scripts/base_trim.shader");
	pMaterialManager->AddFile("data/scripts/base_wall.shader");
	pMaterialManager->AddFile("data/scripts/common.shader");
	pMaterialManager->AddFile("data/scripts/ctf.shader");
	pMaterialManager->AddFile("data/scripts/eerie.shader");
	pMaterialManager->AddFile("data/scripts/gothic_block.shader");
	pMaterialManager->AddFile("data/scripts/gothic_floor.shader");
	pMaterialManager->AddFile("data/scripts/gothic_light.shader");
	pMaterialManager->AddFile("data/scripts/gothic_trim.shader");
	pMaterialManager->AddFile("data/scripts/gothic_wall.shader");
	pMaterialManager->AddFile("data/scripts/hell.shader");
	pMaterialManager->AddFile("data/scripts/liquid.shader");
	pMaterialManager->AddFile("data/scripts/menu.shader");
	pMaterialManager->AddFile("data/scripts/models.shader");
	pMaterialManager->AddFile("data/scripts/organics.shader");
	pMaterialManager->AddFile("data/scripts/sfx.shader");
	pMaterialManager->AddFile("data/scripts/shrine.shader");
	pMaterialManager->AddFile("data/scripts/skin.shader");
	pMaterialManager->AddFile("data/scripts/sky.shader");
	#endif
//	pMaterialManager->AddFile("data/scripts/gfx.shader");
  pMaterialManager->AddFile("data/scripts/tg.shader");

	if (mapName == NULL)
		pQ3Level = new tgQ3Level("data/maps/q3dm1.bsp");
	else
		pQ3Level = new tgQ3Level(mapName);
}

const tgCollisionResult& tgWorldManager::Trace(const tgVector3& Start, const tgVector3& End, tgBoundingVolume* volume, tgIEntity* entity, tgset<tgIEntity*>* ignoreList)
{
	return pQ3Level->Trace(Start, End, volume,entity, ignoreList);
}

void tgWorldManager::AddDecal(tgBoundingSphere* sphere, const tgVector3& norm, const tgstring& matName)
{
	tgDecal* d = pDecalManager->NewDecal(pMaterialManager->GetMaterial(matName), norm);
	pQ3Level->GetFacesMarked(sphere, d);
}

void tgWorldManager::GetTouchingEntities (tgBoundingVolume* volume, tgvector<tgIEntity*>& list)
{
  pQ3Level->GetTouchingEntities(volume, list);
}

void tgWorldManager::MarkFaces (tgAABoundingBox* pBBox)
{
//	pQ3Level->GetFacesMarked(pBBox);	
}

void tgWorldManager::Manage (float eTime)
{
	size_t size = modelList.size();
	for(size_t i=0; i<size; i++) {
		//FIXME: cf FIXME model.cc:181
		modelList[i]->Animate(eTime);
	}

  //suppression des lights en fin de vie
  for(tgvector<tgLight*>::iterator i=lightList.begin(); i!=lightList.end(); ) {
    tgLight* light = *i;
    tgvector<tgLight*>::iterator next = i;
    next++;

    if (light->IsInfinite()) {
      i = next;
      continue;
    }

    light->SetLifeTime(light->GetLifeTime()-eTime);

    light->SetRadius(light->GetRadius()*(light->GetLifeTime()/light->GetBaseLife()));

    if (light->GetLifeTime() < 0.0f) {
      delete *i;
      next = lightList.erase(i);
    }
    i = next;
	}


  size = systemsList.size();
  for(size_t i=0; i<size; i++) {
    systemsList[i]->Think(eTime);
  }

  //supression des emitters l'ayant demande
  for (tgset<tgParticleSystem*>::iterator i=systemsToDelete.begin(); i!=systemsToDelete.end(); i++) {
    RemoveParticleSystem(*i);
    delete *i;
  }
  systemsToDelete.clear();
	
	pQ3Level->Think();
}

void tgWorldManager::AddModel (tgIModel* m)
{
	modelList.push_back(static_cast<tgModel*>(m));
  pQ3Level->AddEntity(m);
}

void tgWorldManager::RemoveModel (tgIModel* m)
{
	bool found = false;
	tgvector<tgModel*>::iterator i;
	for(i=modelList.begin(); i!=modelList.end(); i++) {
		if((*i) == m) {
			found = true;
			break;
		}
	}
	if(!found)
		return;

	modelList.erase(i);
  pQ3Level->RemoveEntity(m);
}

void tgWorldManager::AddParticleSystem (tgIParticleSystem* p)
{
  systemsList.push_back(static_cast<tgParticleSystem*>(p));
}

void tgWorldManager::RemoveParticleSystem (tgIParticleSystem* p)
{
	bool found = false;
	tgvector<tgParticleSystem*>::iterator i;
	for(i=systemsList.begin(); i!=systemsList.end(); i++) {
		if((*i) == p) {
			found = true;
			break;
		}
	}
	if(!found)
		return;

	systemsList.erase(i);

}

void tgWorldManager::AddLight (tgLight* l)
{
  lightList.push_back(static_cast<tgLight*>(l));
}

void tgWorldManager::RemoveLight (tgLight* l)
{
	bool found = false;
	tgvector<tgLight*>::iterator i;
	for(i=lightList.begin(); i!=lightList.end(); i++) {
		if((*i) == l) {
			found = true;
			break;
		}
	}
	if(!found)
		return;

	lightList.erase(i);
}

void tgWorldManager::DeleteMe (tgParticleSystem* p)
{
  systemsToDelete.insert(p);
}


