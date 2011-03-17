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
#ifndef TGIMODEL_H
#define TGIMODEL_H

#include "tglib.h"
#include "tgientity.h"

/**
* Classe gérant un modèle, permettant de changer son animation
*/
class tgIModel : public tgIEntity
{
 public:
 	/**  Herités de tgIEntity  **/
	virtual void SetPosition (const tgVector3& newPos) = 0;
	virtual void SetRotation (const tgQuaternion& newRot) = 0;
	virtual void SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot) = 0;
	
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

 public:
	virtual ~tgIModel () {}
	
	/**
	* Permet de dire si le modèle doit ou non être affiché
	*/
	virtual void SetVisible(bool b) = 0;
	
	/**
	* Change l'animation du modèle. Les animations sont empilées sur une pile FIFO
	* @param animName le nom de l'animation
	* @param channel le channel d'animation (les 'joints' qui seront affectés par l'animation)
	* @param repeat vrai si l'animation doit de répéter en boucle
	* @param breakCurrent vrai si on doit stopper l'animation courante. Si faux, on ajoute cette animation à la liste des prochaines animations
	*/
	virtual void SetAnimation(const tgstring& animName, int channel=-1, bool repeat=true, bool breakCurrent=true) = 0;
	
	virtual tgVector3 GetJointPosition (const tgstring& jName) = 0;
	virtual tgQuaternion GetJointOrientation (const tgstring& jName) = 0;

};

#endif
