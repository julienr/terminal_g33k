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
#ifndef TGSYSTEM_H
#define TGSYSTEM_H

#include "tgigame.h"
#include "tgkeys.h"

/**
* Classe d'abstraction de l'OS.
* Permet :
*  -le chargement des librairies dynamiques
*  -l'accès au timer
*  -la création de la fenêtre
*/

class tgSystem
{
 public:
	virtual ~tgSystem() {}
 	/**
	* Permet au systeme de réaliser les initialisation qui pourraient être nécessaire...
	*/
	virtual void Initialize () = 0;
	
	/**
	* Retourne un pointeur sur une interface de type tgIGame
	* @param name le nom de la librairie dynamique depuis laquelle on chargera tgIGame
	* @return un pointeur sur ladite interface
	*/
	virtual tgIGame* GetGameAPI (const char* name ) = 0;

	/**
	* S'occupe de detruire l'objet renvoyé par GetGameAPI
	*/
	virtual void CloseGameAPI () = 0;

	/**
	* Ouvre la fenêtre principale du jeu
	* @param name le nom de la fenêtre
	* @param sizeX la largeur
	* @param sizeY la hauteur
	*/
	virtual bool OpenWindow (const char* name, int sizeX, int sizeY) = 0;

	/**
	* Ferme la fenêtre précédemment ouvert par OpenWindow
	*/
	virtual void CloseWindow () = 0;
	
	/**
	* Fonction appelée juste avant le rendu
	*/
	virtual void PreRender () = 0;
	
	/**
	* Fonction appelée juste après le rendu
	*/
	virtual void PostRender () = 0;

	/**
	* Retourne vrai quand l'application doit être quittée (appui sur echap)
	* @return true si l'application doit être quittée, faux sinon
	*/
	virtual bool IsDone () = 0;

	/**
	* Recoit les événements (appui sur touches, mouvements souris)
	*/
	virtual void ReceiveEvents () = 0;

	/**
	* Permet de savoir si une touche est pressée
	* voir interface/keys.h pour les keycodes
	* @return true si la touche demandée a été pressée lors du dernier ReceiveEvents
	*/
	virtual bool IsKeyDown (tgKey k) =0;
	
	/**
	* Permet de connaître le nombe de 'ticks' écoulé depuis le lancement du programme
	* @return le nombre de 'ticks' écoulés
	*/
	virtual unsigned long int GetTicks () = 0;
};

#endif

