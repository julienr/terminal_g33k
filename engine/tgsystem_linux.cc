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
#ifdef TGLINUX
#include "tgsystem_linux.h"

void tgSystemLinux::Initialize ()
{
	bDone = false;
	for (int i=0; i<TGKEY_LAST; i++) bKeyDown[i] = false;
}

tgIGame* tgSystemLinux::GetGameAPI (const char* name)
{
	pDLHandle = dlopen(name, RTLD_LAZY);
	pGame = NULL;
	
	if (!pDLHandle) {
		cerr << "[TGE] dlopen : " << dlerror() << endl;
		return NULL;
	}

	newGame = (newGame_t*)dlsym(pDLHandle, "newGame");
	deleteGame = (deleteGame_t*)dlsym(pDLHandle, "deleteGame");

	if (!newGame || !deleteGame) {
		cerr << "[TGE] dlsym : " << dlerror() << endl;
	}

	pGame = newGame ();

//	deleteGame (pGame);
//	dlclose (handle);
	return pGame;
}

void tgSystemLinux::CloseGameAPI ()
{
	deleteGame (pGame);
	dlclose (pDLHandle);
}

bool tgSystemLinux::OpenWindow (const char* name, int sizeX, int sizeY)
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
	
	SDL_SetVideoMode (iScreenWidth, iScreenHeight, iScreenBpp, iVideoFlags);

	int dbuff;
	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);*/
	
	//On passe la profondeur des buffers en 32 bits
	//TODO: ça demanderait quelques benchmarks
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); 
	/*SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   32);
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 32); 
	SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 32 );*/
	
	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &dbuff);

	if(!dbuff) {
		cerr << "[TGE] Impossible d'obtenir le double tampon" << endl;
		return false;
	}
	
	SDL_ShowCursor(SDL_DISABLE);

	SDL_WM_SetCaption(name, NULL);
	return true;
}

void tgSystemLinux::CloseWindow ()
{
	//SDL_Quit ();
}

void tgSystemLinux::PreRender ()
{

}

void tgSystemLinux::PostRender ()
{
	SDL_GL_SwapBuffers();
}

void tgSystemLinux::ReceiveEvents ()
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
			case SDL_MOUSEBUTTONDOWN:
				if (pBindManager)
					pBindManager->CallMouseClick(event.button.button, 0);
				break;
			case SDL_MOUSEBUTTONUP:
				if (pBindManager)
					pBindManager->CallMouseClick(event.button.button, 1);
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

