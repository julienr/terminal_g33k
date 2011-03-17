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
#include "tgmaterial_manager.h"

tgMaterialManager::tgMaterialManager ()
{
	
}

tgMaterialManager::~tgMaterialManager ()
{
	tgvector<materialMap_t::iterator> toErase;
	for(materialMap_t::iterator i = matMap.begin(); i != matMap.end(); i++) {
		toErase.push_back(i);
	}
	for(unsigned int j =0; j<toErase.size(); j++) {	
		delete toErase[j]->second;
		matMap.erase(toErase[j]);
	}
}

void tgMaterialManager::Initialize ()
{
}

tgMaterial* tgMaterialManager::GetMaterial(const tgstring& name)
{
	materialMap_t::iterator i = matMap.find(name);
	
	if (i != matMap.end()) { //le material est dans notre liste
		if(!i->second->Loaded()) {//il n'a pas encore ete charge
			i->second->Load();
			if (i->second->Loaded()) //le chargement s'est-il bien passe ?
				return i->second;
			else
				return matMap["white"];
		} else { //il est deja charge
			return i->second;
		}
	} else { //sinon on retourne la texture blanche
		tgout << "Material pas dans la liste -> notexture : " << name << endl;
		return matMap["white"];
	}
}

tgIMaterial* tgMaterialManager::GetGenericMaterial(const tgstring& name)
{
	return GetMaterial(name);
}

void tgMaterialManager::AddFile(const tgstring& filename)
{
  tgout << "loading shader : " << filename << endl;
	int file = pFileManager->Open(filename.data(), TGFILE_TEXT);
	tgShaderParser* parser = new tgShaderParser(file);
	tgvector<tgShader*> shadersVec;
	parser->GetShaders(shadersVec);

	/**
	 * Un shader est structure de la facon suivante
	 *
	 * nom_shader
	 * {
	 *	instruction args
	 *	instruction args
	 *	{
	 *		instruction args
	 *		instruction args
	 *	}
	 *	{
	 *		instruction args
	 *		instruction args
	 *	}
	 *
	 * }
	 *
	 */
	tgvector<tgShader*>::iterator i;
	for(i=shadersVec.begin(); i!=shadersVec.end(); i++) {
		//on cree un nouveau material pour chaque shader
		tgMaterial* mat = new tgMaterial(*i);
    cout << "mat name : " << mat->GetName() << endl;
		materialMap_t::iterator i = matMap.find(mat->GetName());
		if (i!=matMap.end()) { //on ne remplace pas l'ancien, sinon ca invalide les anciens pointeurs
			tgout << "Double chargement du material : " << mat->GetName() << endl;
			delete mat;
		} else {
			matMap[mat->GetName()] = mat;
		}
	
	}
	delete parser;	
	pFileManager->Close(file);
	tgout << "shader : " << filename << " -- OK" << endl;
}
