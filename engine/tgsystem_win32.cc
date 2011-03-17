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
#ifdef TGWIN32
#include "tgsystem_win32.h"
#include "tgbind_manager.h"



void tgSystemWin32::Initialize ()
{
	bDone = false;
	for (int i=0; i<TGKEY_LAST; i++) bKeyDown[i] = false;

	newGame = NULL;
	deleteGame = NULL;
	pDLHandle = NULL;
}

tgIGame* tgSystemWin32::GetGameAPI (const char* name)
{
	pDLHandle = LoadLibrary(name);
	pGame = NULL;
	
	if (pDLHandle == NULL) {
		cerr << "[TGE] Erreur LoadLibrary : " << GetLastError() << endl;
		return NULL;
	}

	newGame = (newGame_t*)GetProcAddress(pDLHandle, "newGame");
	deleteGame = (deleteGame_t*)GetProcAddress(pDLHandle, "deleteGame");

	if((newGame == NULL) || (deleteGame == NULL)) {
		cerr << "[TGE] Erreur GetProcAddress : " << GetLastError() << endl;
		return NULL;
	}

	pGame = newGame ();

//	deleteGame (pGame);
//	dlclose (handle);
	return pGame;
}

void tgSystemWin32::CloseGameAPI ()
{
	deleteGame (pGame);
	FreeLibrary (pDLHandle);
}


bool tgSystemWin32::OpenWindow (const char* name, int sizeX, int sizeY)
{
	iScreenWidth = sizeX;
	iScreenHeight = sizeY;

	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		cerr << "[TGE] Erreur lors de l'initialisation de SDL_VIDEO" << endl;
		return false;		
	}

	pVidInfo = SDL_GetVideoInfo();

	if (!pVidInfo) {
		cerr << "[TGE] Impossible d'obtenir des informations via SDL_GetVideoInfo" << endl;
		return false;
	}
	
	iScreenBpp = pVidInfo->vfmt->BitsPerPixel;
	
	iVideoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE;
	
	pScreen = SDL_SetVideoMode (iScreenWidth, iScreenHeight, iScreenBpp, iVideoFlags);

	int dbuff;
	
	//On passe la profondeur des buffers en 32 bits
	//TODO: ça demanderait quelques benchmarks
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); 
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   32);
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 32); 
	SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 32 );
	
	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &dbuff);

	if(!dbuff) {
		cerr << "[TGE] Impossible d'obtenir le double tampon" << endl;
		return false;
	}

	/*if(!GetProcAdresses()) {
		cerr << "[TGE] Opengl wglGetProcAddress() Problem ! " << endl;
		return false;
	}*/
	
	SDL_ShowCursor(SDL_DISABLE);

	SDL_WM_SetCaption(name, NULL);
	return true;
}

void tgSystemWin32::CloseWindow ()
{
	SDL_Quit ();
}

void tgSystemWin32::PreRender ()
{

}

void tgSystemWin32::PostRender ()
{
	SDL_GL_SwapBuffers();
}

void tgSystemWin32::ReceiveEvents ()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_ACTIVEEVENT:
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					bDone = true;
				} else {
					bKeyDown[event.key.keysym.sym] = true;
					if (pBindManager)
						pBindManager->Call((tgKey)event.key.keysym.sym, 1);
				}
				break;
			case SDL_KEYUP:
				bKeyDown[event.key.keysym.sym] = false;
				if (pBindManager)
					pBindManager->Call((tgKey)event.key.keysym.sym,0);
				break;
			default:
				break;
		}
	}
	
	int mX, mY;
	int middleX=iScreenWidth/2, middleY=iScreenHeight/2;
	SDL_GetMouseState(&mX, &mY);
	if (mX == middleX && mY == middleY) 
		return;
	
	pBindManager->CallMouseMotion(mX-middleX, mY-middleY);
	SDL_WarpMouse(middleX, middleY);
}

#endif

