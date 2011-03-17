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
#ifndef TGWEAPON_H
#define TGWEAPON_H

#include "tgdefs.h"
#include "tgimodel.h"

class tgPlayer;

class tgWeapon
{
  public:
    tgWeapon(tgPlayer* p) { pPlayer = p; }
    virtual ~tgWeapon() {}
    virtual void Fire (const tgVector3& vPosition, const tgVector3& vDirection, const tgQuaternion& qRot) = 0;
    virtual void Think () = 0;
    virtual void UnSelect () = 0;
    virtual void Select () = 0;
  protected:
    tgPlayer* pPlayer;
    tgIModel* pModel;
};

#endif

