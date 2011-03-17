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
#ifndef TGENTITY_H
#define TGENTITY_H

#include "tglib.h"



/**
* Classe abstraite de base de toute les entitées.
* Une entitée est tout ce qui a une position/rotation -> modèles, particules, lumières, téléporteur ou autre
*/

class tgIEntity
{
 public:
	virtual ~tgIEntity() {}
	virtual void SetPosition (const tgVector3& newPos) = 0;
	virtual void SetRotation (const tgQuaternion& newRot) = 0;

	/**
	* Fonction utilisée pour tourner un joint de l'entité. Peut être ignorée si l'entité n'a pas de joint.
	* Utilisée par exemple pour tourner la tête du modèle
	*/
	virtual void SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot) = 0;
	
	/**
	* Toute entitée a une bounding box qui lui permet notamment d'être "classée" dans la map pour savoir si elle doit être rendue.
	* Peut également être utilisé pour les collisions.
	*/
	virtual tgBoundingVolume* GetBoundingVolume () = 0;
	virtual void SetBoundingVolume (tgBoundingVolume* volume) = 0;
	
	virtual const tgVector3& GetPosition () = 0;
	virtual const tgQuaternion& GetRotation () = 0;
	/**
	 * Un 'viewmodel' est affiche en mode premiere personne, une arme par exemple
	 */
	virtual void SetViewModel(bool b) = 0;
	virtual bool IsViewModel() = 0;
	virtual const tgVector3& GetOriginPosition() = 0;
  // permet de specifier un offset par rapport a la position, utilise uniquement pour le rendu
  virtual void SetPosOffset (const tgVector3& v) { }
  virtual const tgVector3& GetPosOffset () { return tgVector3::ZERO; }

};

#endif
