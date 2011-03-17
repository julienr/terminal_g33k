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
#ifndef TGIGAME_H
#define TGIGAME_H

#include "tgexport.h"
#include "tgimport.h"

/**
* Interface abstraite de la logique du jeu.
* C'est la seule classe utilisé par le moteur pour lancer le jeu
*/

class tgIGame
{
 public:
	virtual ~tgIGame() {}
 	/**
	* Le moteur exporte certaines de ses structures, Game doit obtenir des pointeurs dessus
	* @param e une structure regroupant ces pointeurs 
	* @see engine/interface/export.h
	*/
	virtual void ImportFromEngine (tgEngineExport* e) = 0;
	
	/**
	* Le jeu export certaines structures, le moteur doit en etre informé
	* @param i une structure regroupant les informations sur tggame 
	* @see engine/interface/import.h
	*/
	virtual void ExportToEngine (tgEngineImport* i) = 0;
	
	/**
	* Fonction permettant a Game de s'initialiser _APRES_ ImportFromEngine
	*/
	virtual void Initialize () = 0;
	
	/**
	* Fonction appelée par le moteur à chaque frame
	* @param eTime le temps de la frame, en secondes
	*/
	virtual void ProcessFrame (float eTime) = 0;
 protected:
};

typedef tgIGame* newGame_t();
typedef void deleteGame_t(tgIGame*);

#endif
