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

#ifndef TGGAME_H
#define TGGAME_H

#include "tgdefs.h"
#include "interface/tgigame.h"

#include "tgphysic_engine.h"
#include "tghuman_player_controller.h"
#include "tgai_player_controller.h"
#include "tgplayer.h"


class tgGame: public tgIGame
{
	public:
		~tgGame();
		/**
		* @see tgIGame::ProcessFrame
		*/
		void ProcessFrame (float eTime);

		/**
		* @see tgIGame::ImportFromEngine
		*/
		void ImportFromEngine (tgEngineExport* e);

		/**
		*  @see tgIGame::ExportToEngine
		*/
		void ExportToEngine (tgEngineImport* i);

		/**
		* @see tgIGame::Initialize
		*/
		void Initialize ();

    tgPlayer* PhysicNodeToPlayer (tgPhysicNode* node);
		
		void ToggleConsole (int state=0);
		void UniqueKeyCallback (tgKey key, int state);

		tgIModelManager* GetModelManager() const { return pModelManager; }
		tgIWorldManager* GetWorldManager() const { return pWorldManager; }
    tgPhysicEngine* GetPhysicEngine () const { return pPhysicEngine; }
    tgIParticleManager* GetParticleManager () const { return pParticleManager; }

		void NextStartPosition ();

		
	protected:
		tgIWorldManager* pWorldManager;
		tgIBindManager* pBindManager;
		tgIFileManager* pFileManager;
		tgIModelManager* pModelManager;
		tgIMaterialManager* pMaterialManager;
		tgICanvas* pCanvas;
		tgITimer* pTimer;
		tgIConsole* pConsole;
		tgIRenderView* pRenderView;
    tgIParticleManager* pParticleManager;
		
		tgPhysicEngine* pPhysicEngine;

    //PLAYERS
		tgHumanPlayerController* pHumanPlayerController;
		tgPlayer* pHumanPlayer;
    tgAIPlayerController* pBotPlayerController;
    tgPlayer *pBotPlayer;
    typedef tgvector<tgPlayer*> t_playersList;
    t_playersList players;
};

#endif
