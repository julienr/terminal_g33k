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

//FIXME: Debugging vc++2005
#ifdef TGWIN32
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
//endFIXME

#include "tgdefs.h"

#include "tgdefs.h"
#include "tgconsole.h"
#include "tgworld_manager.h"
#include "tgmodel.h"
#include "tgq3level.h"
#include "tgcanvas.h"
#include "tgdecalmanager.h"
#include "tgmaterial.h"
#include "tgfile_manager.h"
#include "tgsystem.h"
#include "tgmaterial_manager.h"
#include "tgendian.h"
#include "tgtimer.h"
#include "tgbind_manager.h"
#include "tgglrenderer.h"
#include "tgmodel_manager.h"
#include "tgparticle_manager.h"

#ifdef TGLINUX
#include "tgsystem_linux.h"
#endif

#ifdef TGOSX
#include "tgsystem_osx.h"
#endif

#ifdef WIN32
#include "tgsystem_win32.h"
#endif




tgSystem* pSystem = NULL;
tgIGame* pGame = NULL;
tgWorldManager* pWorldManager = NULL;
tgBindManager* pBindManager = NULL;
tgFileManager* pFileManager = NULL;
tgGLRenderer* pGLRenderer = NULL;
tgModelManager* pModelManager = NULL;
tgParticleManager* pParticleManager = NULL;
tgMaterialManager* pMaterialManager = NULL;
tgEndian* pEndian = NULL;
tgTimer* pTimer = NULL;
tgConsole* pConsole = NULL;
tgCanvas* pCanvas = NULL;
tgRenderView* pRenderView = NULL;

tgEngineImport gameImport;

int main (int argc, char** argv)
{
	//FIXME: Debugging vc++2005
	#ifdef TGWIN32
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	#endif

	//endFIXME
	char* mapName = NULL;
	if (argc > 1) {
		mapName = argv[1];
	}


	#ifdef TGLINUX
	tgout << "[TGE] Linux System Version" << endl;
	pSystem = new tgSystemLinux();
	#endif

	#ifdef TGOSX
	tgout << "[TGE] Mac OS X System Version" << endl;
	pSystem = new tgSystemOSX();
	#endif

	#ifdef TGWIN32
	tgout << "[TGE] Win32 System Version" << endl;
	pSystem = new tgSystemWin32();
	#endif

	if (!pSystem) {
		cerr << "[TGE] Erreur lors de l'instanciation de tgSystem" << endl;
		return -1;
	}

	pSystem->Initialize ();

	#ifdef TGLINUX
	pGame = pSystem->GetGameAPI("./libgame.so");
	#endif

	#ifdef TGOSX
	pGame = pSystem->GetGameAPI("./game.dylib");
	#endif

	#ifdef TGWIN32
	pGame = pSystem->GetGameAPI("tggame.dll");
	#endif

	if (!pGame) {
		cerr << "[TGE] Erreur lors de l'obtention du pointeur sur tgIGame" << endl;
		return -1;
	}

	//On determine l'endian du system
	unsigned char swaptest[2] = {1,0};
	if (*(short*)swaptest == 1) //littleendian
	{
		tgout << "Architecture little endian (x86)" << endl;
		pEndian = new tgLittleEndian ();
	}
	else //bigendian
	{
		tgout << "Architecture big endian (PowerPC)" << endl;
		pEndian = new tgBigEndian ();
	}

	//eXPortation des structures du moteur
	pFileManager = new tgFileManager ();
	pTimer = new tgTimer ();
	pBindManager = new tgBindManager ();
	pConsole = new tgConsole();
	pGLRenderer = new tgGLRenderer ();
	pMaterialManager = new tgMaterialManager ();
	pModelManager = new tgModelManager ();
	pWorldManager = new tgWorldManager();
  pParticleManager = new tgParticleManager();
	pCanvas = new tgCanvas ();
	pRenderView = new tgRenderView();


	/* TEST FOR THE SHADERS' PARSER */
	#if 0
	int file = pFileManager->Open("default.mtr", TGFILE_TEXT);
	tgShaderParser *p = new tgShaderParser(file);
	delete(p);
	exit(0);
	#endif
	/* END TEST */

	tgEngineExport export_t;
	export_t.pWorldManager = pWorldManager;
	export_t.pBindManager = pBindManager;
	export_t.pFileManager = pFileManager;
	export_t.pModelManager = pModelManager;
  export_t.pParticleManager = pParticleManager;
	export_t.pTimer = pTimer;
	export_t.pConsole = pConsole;
	export_t.pMaterialManager = pMaterialManager;
	export_t.pCanvas = pCanvas;
	export_t.pRenderView = pRenderView;
	pGame->ImportFromEngine(&export_t);
	pGame->ExportToEngine(&gameImport);


	//Ouverture de la fenêtre
#define X_RES 1024
#define Y_RES 768
	pSystem->OpenWindow("tg", X_RES, Y_RES);
	pGLRenderer->InitGL();
	pGLRenderer->ResizeWindow(X_RES,Y_RES);

	pMaterialManager->Initialize();
	pWorldManager->Initialize(mapName);
	pModelManager->Initialize();
	pGame->Initialize ();
	pCanvas->SetFont("gfx/2d/bigchars");
	pGLRenderer->EndLoad();

	pTimer->Initialize();
	while(!pSystem->IsDone()) {
		pSystem->ReceiveEvents ();
		pRenderView->Update();
		pWorldManager->GetQ3Level()->TraceAgainstTriangleCount = 0;
		pTimer->CalculateFPS();
		pGame->ProcessFrame(pTimer->GetFrameInterval());
		pWorldManager->Manage(pTimer->GetFrameInterval());
		pConsole->Render();
		pGLRenderer->PreRender();
		pGLRenderer->RenderWorld(pWorldManager);
		pGLRenderer->RenderCanvas(pCanvas);
		pGLRenderer->PostRender();
	}


	pSystem->CloseGameAPI ();
	delete pCanvas;
	delete pModelManager;
  delete pParticleManager;
	delete pMaterialManager;
	delete pGLRenderer;
	delete pFileManager;
	delete pConsole;
	delete pBindManager;
	delete pWorldManager;
	delete pTimer;
	delete pEndian;
	delete pRenderView;
	
	if(gameImport.pChannelsName != NULL)
		delete [] gameImport.pChannelsName;

	pSystem->CloseWindow ();

	delete pSystem;
	return 0;
}
