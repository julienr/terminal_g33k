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
#ifndef TGIMODEL_MANAGER_H
#define TGIMODEL_MANAGER_H

#include "tglib.h"
#include "tgimodel.h"

/**
* Class gérant le chargement des modèles, ainsi que le cache (on ne charge pas un modèle deux fois depuis le disque et on évite les accès disques pendant le jeu
*/
class tgIModelManager
{
 public:
	virtual ~tgIModelManager () {} 
	/**
	* Précharge un modèle, de façon à ce que les prochain appels à GetInstance n'engendrent pas d'accès disque
	* @param modelName le nom identifiant le modèle pour tout ce qui touche au ModelManager
	* @param filename le nom du fichier .mdf comprenant la definition du model (model definition)
	*/
	virtual void PreLoadModel (const tgstring& modelName, const tgstring& filename) = 0;
	
	/**
	* Permet d'obtenir une instance d'un modèle. Si le modèle a été préchargé, ne génère aucun accès disque
	* @param modelName le nom identifiant le modèle
	* @return un pointeur sur un objet tgIModel permettant d'accéder aux informations de ce modèle
	*/
	virtual tgIModel* GetInstance (const tgstring& modelName) = 0;
};

#endif
