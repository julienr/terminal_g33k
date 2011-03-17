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
#ifndef TGMATERIAL_MANAGER_H
#define TGMATERIAL_MANAGER_H

#include "tgdefs.h"
#include "tgimaterial_manager.h"
#include "tgshader_parser.h"
#include "tgmaterial.h"
#include "tgfile_manager.h"

/**
* Class s'occupant de faire un "cache" des matériaux
*/
class tgMaterialManager : public tgIMaterialManager
{
 public:
	tgMaterialManager ();
	virtual ~tgMaterialManager ();
	
	void Initialize ();
	
	tgMaterial* GetMaterial(const tgstring& name);
	tgIMaterial* GetGenericMaterial(const tgstring& name);
	
	void AddFile(const tgstring& filename);
 protected:
 	typedef tgmap<tgstring, tgMaterial*> materialMap_t;
	materialMap_t matMap;
};

#endif
