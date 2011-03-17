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

#include "tggame.h"

void tgGame::ImportFromEngine (tgEngineExport* e)
{
	pWorldManager = e->pWorldManager;
	pBindManager = e->pBindManager;
	pFileManager = e->pFileManager;
	pModelManager = e->pModelManager;
	pMaterialManager = e->pMaterialManager;
	pCanvas = e->pCanvas;
	pTimer = e->pTimer;
	pConsole = e->pConsole;
	pRenderView = e->pRenderView;
  pParticleManager = e->pParticleManager;
}

void tgGame::ExportToEngine (tgEngineImport* i)
{
	//Channels d'animation
	i->iNumChannels = 2;
	i->pChannelsName = new tgstring[2];
	i->pChannelsName[0] = "torso";
	i->pChannelsName[1] = "legs";
}

tgGame::~tgGame()
{
	delete pPhysicEngine;

  delete pBotPlayerController;
  delete pBotPlayer;
	delete pHumanPlayerController;
	delete pHumanPlayer;
}

void tgGame::Initialize ()
{
  pModelManager->PreLoadModel("player", "data/models/berserker.mdf");
  pModelManager->PreLoadModel("rocket", "data/models/rocket.mdf");
  pModelManager->PreLoadModel("rocketlauncher_view", "data/models/rocketlauncher_view.mdf");
  pModelManager->PreLoadModel("machinegun_view", "data/models/machinegun_view.mdf");

  pParticleManager->PreLoadFile("data/particles/explosions.prt");
  pParticleManager->PreLoadFile("data/particles/weapons.prt");

	//PLAYERS
	pHumanPlayer = new tgPlayer();
	pHumanPlayerController = new tgHumanPlayerController(pHumanPlayer);

  pBotPlayer = new tgPlayer();
  pBotPlayerController = new tgAIPlayerController(pBotPlayer);

  pWorldManager->AddModel(pBotPlayer->GetModel());
	pWorldManager->AddModel(pHumanPlayer->GetModel());

  players.push_back(pHumanPlayer);
  players.push_back(pBotPlayer);


  //hide player's model
	pHumanPlayer->GetModel()->SetVisible(false);

	//BIND
	pBindManager->Bind(TGKEY_TAB, tgMakeFunctor((tgFunctor1<int>*)0, *this, &tgGame::ToggleConsole));
	pBindManager->SetUniqueCallback (tgMakeFunctor((tgFunctor2<tgKey, int>*)0, *this, &tgGame::UniqueKeyCallback));
	
	pBindManager->BindMouseMotion(tgMakeFunctor((tgFunctor2<int,int>*)0, *pHumanPlayerController, &tgHumanPlayerController::MouseMotion));
	
	pBindManager->Bind(TGKEY_a, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::StrafeLeft));
	pBindManager->Bind(TGKEY_d, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::StrafeRight));
	pBindManager->Bind(TGKEY_w, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::MoveForward));
	pBindManager->Bind(TGKEY_s, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::MoveBackward));
  pBindManager->Bind(TGKEY_SPACE, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::Jump));

  pBindManager->BindMouseClick(TG_BUTTON_LEFT, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::Fire));
  pBindManager->BindMouseClick(TG_BUTTON_WHEELUP, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::NextWeapon));
  pBindManager->BindMouseClick(TG_BUTTON_WHEELDOWN, tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::PrevWeapon));


	//CONSOLE REGISTER
	pConsole->Register(tgMakeFunctor((tgFunctor1<int>*)0, *pHumanPlayerController, &tgHumanPlayerController::SetNoClip), "noclip");	
	pConsole->Register(tgMakeFunctor((tgFunctor0*)0,*this, &tgGame::NextStartPosition),"nextstartposition");
	pConsole->Register(tgMakeFunctor((tgFunctor1<const tgVector3&>*)0, *pHumanPlayer->GetPhysicNode(), &tgPhysicNode::SetAllPosition), "forceposition");
	//PHYSIC
	pPhysicEngine = new tgPhysicEngine();
	pPhysicEngine->AddNode(pHumanPlayer->GetPhysicNode());
  pPhysicEngine->AddNode(pBotPlayer->GetPhysicNode());
	
	//RENDERVIEW
	pRenderView->SetEntity(pHumanPlayer->GetModel());

	//START POSITION
	if(pWorldManager->GetNumStartLocations() > 1) {
    cout << "Level has start locations " << endl;
		pHumanPlayer->GetPhysicNode()->SetAllPosition(pWorldManager->GetStartLocation(0)+tgVector3(30,30,30));
    pBotPlayer->GetPhysicNode()->SetAllPosition(pWorldManager->GetStartLocation(1));
  }
	//pHumanPlayerController->SetNoClip(1);


}

void tgGame::NextStartPosition()
{
	static int current = 0;
	current++;
	if(current >= pWorldManager->GetNumStartLocations())
		current = 0;
	pHumanPlayer->GetPhysicNode()->SetAllPosition(pWorldManager->GetStartLocation(current));
}


void tgGame::UniqueKeyCallback (tgKey key, int state)
{
	if(state) return;
	if (key == TGKEY_TAB) {
		ToggleConsole ();
		return;
	}
	if (key == TGKEY_RETURN) {
		pConsole->NextLine();
		return;
	}
	if (key == TGKEY_BACKSPACE) {
		pConsole->BackSpace();
		return;
	}
	pConsole->Print((char)key);
	//cout << (char)key << endl;
}

void tgGame::ToggleConsole (int state)
{
	if (state) return;
	pConsole->Toggle();

	if (pConsole->IsActive()) {
		pBindManager->SwitchToUniqueCallback();
	} else {
		pBindManager->SwitchToMultipleCallback();
	}
}

tgPlayer* tgGame::PhysicNodeToPlayer (tgPhysicNode* node)
{
  for(t_playersList::iterator i = players.begin(); i != players.end(); i++) {
    if ((*i)->GetPhysicNode() == node)
      return (*i);
  }
  return NULL;
}

void tgGame::ProcessFrame (float eTime)
{
  //BEGIN MAX_FREQ
  #define PHYS_MAX_FREQ 0.016666667 // 1/60 => 60hz
  /* On bouge/think les objets max 60 fois par secondes, sinon on a des mouvements trop petits qui ressemblent a rien*/
/*  static float totalETime = 0;

  totalETime += eTime;*/
  /**
   * FIXME: en procedant comme ici pour le 'capping' a 60hz, des saccades
   * apparaissent si on est a 61fps:
   * On devra 'skipper' une frame et on va se retrouver avec la frame suivante
   * avec presque 2* le temps moyen des autres frames.
   * Si on monte a 70fps, on a 10 fois ce cas-la, donc y'a plusieurs saccades qui
   * s'intercalent, c'est assez horrible
   */
//  if(totalETime > PHYS_MAX_FREQ) {
    pHumanPlayerController->Control();
    pHumanPlayer->Think();
    pBotPlayer->Think();
    //FIXME: il faut soustraire le max de PHYS_MAX_FREQ pour eviter qu'a 30 fps, totalETime
    //ne monte pasqu'on ne peut pas tout supprimer
    // => genre totalETime -= division_entiere totalETime/PHYS_MAX_FREQ
    // mais ca fait revenir les saccades
    //float tmp = floor(totalETime/PHYS_MAX_FREQ);
    pPhysicEngine->MoveNodes(/*totalETime*/eTime);
//    totalETime = 0;
    //totalETime -= PHYS_MAX_FREQ;
//  }

  {
    std::ostringstream stream;
    stream << "Player position : " << pHumanPlayer->GetPhysicNode()->GetPosition() << endl;
    pCanvas->DisplayText(stream.str());
  }
  {
    std::ostringstream stream;
    stream << pTimer->GetFPS() << " FPS" << endl;
    pCanvas->DisplayText(stream.str());
  }


}

