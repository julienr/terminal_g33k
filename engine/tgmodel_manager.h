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
#ifndef TGMODEL_MANAGER_H
#define TGMODEL_MANAGER_H

#include "tgimodel_manager.h"
#include "tgdefs.h"
#include "tgmodel_cache.h"
#include "tgmodel.h"

/**
* Impl�mentation du gestionnaire de mod�le
*/
class tgModelManager: public tgIModelManager
{
 public:
	tgModelManager ();
	~tgModelManager ();
	void Initialize ();
	/**
	* @see tgIModelManager::PreLoadModel
	*/
	void PreLoadModel (const tgstring& modelName, const tgstring& filename);
	
	/**
	* @see tgIModelManager::GetInstance
	*/
	tgIModel* GetInstance (const tgstring& modelName);
 protected:
	typedef tgmap<tgstring, tgModelCache*> ModelsMap;
	ModelsMap mModels;
};

#endif
