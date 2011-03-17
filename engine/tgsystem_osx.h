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
#ifdef TGOSX
#ifndef TGSYSTEM_OSX_H
#define TGSYSTEM_OSX_H

#include "tgsystem.h"
#include <dlfcn.h> //dlopen(), dlsym()
#include "tgdefs.h"

#include <SDL/SDL.h>

/*
* tgSystemOSX, couche d'abstraction de GNU/Linux
* Utilise SDL pour gérer les événements et la création de la fenêtre
*/

class tgSystemOSX : public tgSystem
{
 public:
 	/**
	* @see tgSystem::Initialize
	*/
	void Initialize ();
	/**
	* @see tgSystem::GetGameAPI
	*/
	tgIGame* GetGameAPI (const char* name);

	/**
	* @see tgSystem::CloseGameAPI
	*/
	void CloseGameAPI ();

	/**
	* @see tgSystem::OpenWindow
	*/
	bool OpenWindow (const char* name, int sizeX, int sizeY);

	/**
	* @see tgSystem::CloseWindow
	*/
	void CloseWindow ();
	
	/**
	* @see tgSystem::PreRender
	*/
	void PreRender ();
	
	/**
	* @see tgSystem::PostRender
	*/
	void PostRender ();

	/**
	* @see tgSystem::ReceiveEvents
	*/
	void ReceiveEvents ();

	/**
	* @see tgSystem::IsDone
	*/
	inline bool IsDone () { return bDone; }

	/**
	* @see tgSystem::IsKeyDown
	*/
	inline bool IsKeyDown (tgKey k) { return bKeyDown[k] ; }
	
	/**
	* @see tgSystem::GetTicks
	*/
	unsigned long int GetTicks () { return SDL_GetTicks(); }

	
 protected:
	bool bDone;
	
	bool bKeyDown[TGKEY_LAST];
 
	void *pDLHandle;
	tgIGame* pGame;
	//fonction de création/destruction de tgIGame chargée depuis la DLL
	newGame_t* newGame;
	deleteGame_t* deleteGame;

	//variables liées a SDL
	SDL_Surface* pScreen;
	const SDL_VideoInfo* pVidInfo;
	int iScreenWidth;
	int iScreenHeight;
	int iScreenBpp;
	int iVideoFlags;
};

#endif

#endif
