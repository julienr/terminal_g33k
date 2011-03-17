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
#ifndef TGIWORLD_MANAGER_H
#define TGIWORLD_MANAGER_H

#include "tglib.h"
#include "tgimodel.h"
#include "tgiparticle_system.h"
#include "tglight.h"

/**
* Gère le chargement de la map, son affichage, ainsi que celui des autres objets (modèles, particules) du monde
*/
class tgIWorldManager
{
 public:
	virtual ~tgIWorldManager() {}
 	virtual void Manage (float eTime) = 0;
	
	virtual void AddModel (tgIModel* m) = 0;
	virtual void RemoveModel (tgIModel* m) = 0;

  virtual void AddParticleSystem (tgIParticleSystem* p) = 0;
  virtual void RemoveParticleSystem (tgIParticleSystem* p) = 0;
  virtual void AddLight (tgLight* l) = 0;
  virtual void RemoveLight (tgLight* l) = 0;
	/**
	*  Trace contre la map
	* @param Start le début du mouvement de la boîte
	* @param End la fin du mouvement
	* @param volume la boundingbox avec laquelle on trace
	* @return le résultat du trace
	*/
	//virtual tgCollisionResult TraceBox(const tgVector3& Start, const tgVector3& End, tgAABoundingBox* volume) = 0;
	
	/**
	* Trace contre la map
	* @param Start le début du rayon
	* @param End la fin du rayon
	* @return le résultat du trace
	*/
	//virtual tgCollisionResult TraceRay(const tgVector3& Start, const tgVector3& End) = 0;
	
  /**
	* Trace contre la map, ignorant une liste d'entites
	* @param Start le début du rayon
	* @param End la fin du rayon
	* @param volume le volume qui suivra le chemin Start - End
  * @param entity l'entity "source" du trace, NULL si aucune
  * @param ignoreList une liste d'entite contre lesquelles ignorer les collisions
	* @return le résultat du trace
	*/
  virtual const tgCollisionResult& Trace(const tgVector3& Start, const tgVector3& End, tgBoundingVolume* volume, tgIEntity* entity, tgset<tgIEntity*>* ignoreList = NULL) = 0;

  /**
  * Remplis une liste d'entites touchees par le boundingVolume
  * @param volume le Bvolume
  * @param list la liste a remplir
  */
  virtual void GetTouchingEntities (tgBoundingVolume* volume, tgvector<tgIEntity*>& list) = 0;
	/**
	* Ajoute un decal avec la position/taille specifie par la sphere
	* @param sphere la sphere qui determinera quelles faces sont touchees
	* @param norm la normale du decal (determine l'orientation du plan)
	* @param matName le nom de la texture a appliquer sur le decal
	*/
	virtual void AddDecal(tgBoundingSphere* sphere, const tgVector3& norm,const tgstring& matName) = 0;

	/**
	 * Applique une marque sur les faces touchees par la Box
	 */ 
	virtual void MarkFaces (tgAABoundingBox* pBBox) = 0;

	/**
	 * retourne le nombre de position de departs definies commes des entity de la map
	 */
	virtual int GetNumStartLocations() = 0;

	/**
	 * retourne la position de depart d'index i 
	 */
	virtual const tgVector3& GetStartLocation(int index) = 0;
	
};

#endif

