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

#ifndef TGPROJECTILE_H
#define TGPROJECTILE_H

#include "tgdefs.h"
#include "tgimodel.h"
#include "tgphysic_node.h"
#include "tgforce.h"

class tgWeapon;

class tgProjectile
{
  public: 
    virtual ~tgProjectile () { }

		tgPhysicNode* GetPhysicNode() const { return pPhysicNode; }

		tgIModel* GetModel() const { return pModel; }

  protected:
    tgProjectile (tgWeapon* p);

    tgWeapon* pParent;
    tgIModel* pModel;
    tgPhysicNode* pPhysicNode;
};

#endif

