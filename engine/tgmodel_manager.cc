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
#include "tgmodel_manager.h"

tgModelManager::tgModelManager ()
{
	
}

void tgModelManager::Initialize()
{
	tgModelCache* m = new tgModelCache ("data/models/default.mdf");
	mModels["nomodel"] = m;
}

tgModelManager::~ tgModelManager()
{
	tgvector<ModelsMap::iterator> toErase;
	for (ModelsMap::iterator i = mModels.begin(); i != mModels.end(); i++) {
		//delete i->second;
		//mModels.erase(i);
		toErase.push_back(i);
	}
	for(unsigned int i = 0; i<toErase.size(); i++) {
		delete toErase[i]->second;
		mModels.erase(toErase[i]);
	}
	
	//delete mModels["nomodel"];
}

void tgModelManager::PreLoadModel (const tgstring& modelName, const tgstring& filename)
{
	ModelsMap::iterator i = mModels.find(modelName);
	if (i == mModels.end()) { // non trouve
		tgModelCache* m = new tgModelCache(filename.data());
		if (m->IsLoaded()) {
			mModels[modelName] = m;
		} else {
			mModels[modelName] = mModels["nomodel"];
			delete m;
		}
	}
}

tgIModel* tgModelManager::GetInstance (const tgstring& modelName)
{
	ModelsMap::iterator i = mModels.begin ();
	if((i=mModels.find(modelName)) == mModels.end()) {//mesh non chargé
		return new tgModel(mModels["nomodel"]);
	} else {
		return new tgModel(i->second);
		
	}
}


