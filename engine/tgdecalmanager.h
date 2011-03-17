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
#ifndef TGDECAL_MANAGER_H
#define TGDECAL_MANAGER_H

#include "tgdefs.h"
#include "tgdecal.h"
#include "tgconsole.h"

class tgDecalManager
{
	public:
		tgDecalManager(unsigned int maxDecals);
		~tgDecalManager();
		
		tgDecal* NewDecal(tgMaterial* mat, const tgVector3& norm);
		//void Think();
		//void SetMaxDecals(unsigned int max);
		
		unsigned int GetNumDecals() const;
		tgDecal* GetDecal(unsigned int i) const;
		
	protected:
		tgdeque<tgDecal*> decalsList;
		//unsigned int iMaxDecals;
		tgCVar cvMaxDecals;
		
};

#endif
