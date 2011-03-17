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
#ifndef TGMACHINEGUN_H
#define TGMACHINEGUN_H

#include "tgweapon.h"
#include "tgdefs.h"

#define MACHINEGUN_DAMAGE 5

class tgMachinegun : public tgWeapon
{
  public:
    tgMachinegun(tgPlayer* p);
    ~tgMachinegun();
    void Fire (const tgVector3& vPosition, const tgVector3& vDirection, const tgQuaternion& qRot);
    void Think ();

    // appelées lorsque l'arme est selectionnée/déselectionnée 
    void UnSelect ();
    void Select ();

  protected:
};

#endif

