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

#ifndef TGIRENDERVIEW_H
#define TGIRENDERVIEW_H

#include "tgientity.h"
#include "tglib.h"

/**
* Structure regroupant des informations sur la vue d'un joueur ou d'un monstre.
* Lors du rendu, une vue de ce type est utilisée (on peut ainsi facilement changer de point de vue.
*/
class tgIRenderView
{
 public:
	virtual ~tgIRenderView() {}
	virtual void SetEntity(tgIEntity* ) = 0;
	 
	virtual void SetRelPosition (const tgVector3& pos) = 0;
	virtual const tgVector3& GetRelPosition() const = 0;

	virtual void SetRelRotation (const tgQuaternion& rot) = 0;
	virtual const tgQuaternion& GetRelRotation() const = 0;
};

#endif

