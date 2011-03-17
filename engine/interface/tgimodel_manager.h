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
* Class g�rant le chargement des mod�les, ainsi que le cache (on ne charge pas un mod�le deux fois depuis le disque et on �vite les acc�s disques pendant le jeu
*/
class tgIModelManager
{
 public:
	virtual ~tgIModelManager () {} 
	/**
	* Pr�charge un mod�le, de fa�on � ce que les prochain appels � GetInstance n'engendrent pas d'acc�s disque
	* @param modelName le nom identifiant le mod�le pour tout ce qui touche au ModelManager
	* @param filename le nom du fichier .mdf comprenant la definition du model (model definition)
	*/
	virtual void PreLoadModel (const tgstring& modelName, const tgstring& filename) = 0;
	
	/**
	* Permet d'obtenir une instance d'un mod�le. Si le mod�le a �t� pr�charg�, ne g�n�re aucun acc�s disque
	* @param modelName le nom identifiant le mod�le
	* @return un pointeur sur un objet tgIModel permettant d'acc�der aux informations de ce mod�le
	*/
	virtual tgIModel* GetInstance (const tgstring& modelName) = 0;
};

#endif
