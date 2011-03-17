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

#ifndef TGPLAYER_H
#define TGPLAYER_H

#include "tgdefs.h"
#include "tgimodel.h"
#include "tgphysic_node.h"
#include "tgforce.h"
#include "tgweapon.h"
#include "tglight.h"

class tgPlayer
{
	public:
		tgPlayer();
		~tgPlayer();

		tgPhysicNode* GetPhysicNode();

    bool IsLocalPlayer() { return isLocalPlayer; }
    void SetIsLocalPlayer(bool b) { isLocalPlayer = b; }

		tgIModel* GetModel() const;

    tgVector3 GetViewDirection() const;
    void Fire ();
    //selectionne l'arme num, -1 pour selectionner la prochaine, -2 pour la precedente
    void SwitchWeapon(int num);

    void Think ();

    void TakeDamage (int dam, const tgVector3& vNormal, const tgVector3& vPoint);

    void Touch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other);
    void UnTouch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other);
	protected:
    bool isLocalPlayer;
    int currentWeapon;
    tgWeapon** pWeapons;
		tgIModel* pModel;
		tgPhysicNode* pPhysicNode;
    tgAABoundingBox* pBBox;
    tgLight* light;
};

#endif

