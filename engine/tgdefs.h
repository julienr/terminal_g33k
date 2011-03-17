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
#ifndef TGDEFS_H
#define TGDEFS_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <sys/stat.h>
#ifdef TGLINUX
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif
#ifdef TGWIN32
#include <SDL.h>
#include <SDL_image.h>
#endif

//BEGIN DEFINES
//file_manager.h
#define DEBUG_STREAMS
#define MAX_STREAMS 10




//END DEFINES

#define TG_PI 3.1415

#include "tglib.h"

#if 0
#include "tglib.h"
#include "memory_manager.h"
#include "world_manager.h"
#include "bind_manager.h"
#include "file_manager.h"
#include "system.h"
#include "igame.h"
#include "glrenderer.h"
#include "model_cache.h"
#include "model_manager.h"
#include "model.h"
#include "doom3level.h"
#include "decal.h"
#include "decalmanager.h"
#include "q3level.h"
#include "material.h"
#include "image.h"
#include "material_manager.h"
#include "endian.h"
#include "timer.h"
#include "console.h"
#include "canvas.h"
#include "shader.h"
#include "shader_parser.h"
#include "shader_property.h"


class tgBindManager;
class tgFileManager;
class tgGLRenderer;
class tgModelManager;
class tgWorldManager;
class tgSystem;
class tgMaterialManager;
class tgTimer;
class tgConsole;
class tgCanvas;
class tgMemoryManager;
class tgDecalManager;

class tgShaderParser;
class tgShader;
class tgShaderProperty;

//class tgShaderProperty_S;

class tgDecal;
#endif

class tgIGame;
class tgWorldManager;
class tgSystem;
class tgBindManager;
class tgFileManager;
class tgGLRenderer;
class tgModelManager;
class tgMaterialManager;
class tgEndian;
class tgTimer;
class tgConsole;
class tgCanvas;
struct tgEngineImport;
class tgRenderView;
class tgParticleManager;

extern tgIGame* pGame;
extern tgWorldManager* pWorldManager;
extern tgSystem* pSystem;
extern tgBindManager* pBindManager;
extern tgFileManager* pFileManager;
extern tgGLRenderer* pGLRenderer;
extern tgModelManager* pModelManager;
extern tgMaterialManager* pMaterialManager;
extern tgEndian* pEndian;
extern tgTimer* pTimer;
extern tgConsole* pConsole;
extern tgCanvas* pCanvas;
extern tgRenderView* pRenderView;
extern tgParticleManager* pParticleManager;

extern tgEngineImport gameImport;

#define SWAPFLOAT(name) name=pEndian->Float(name);
#define SWAPLONG(name) name=pEndian->Long(name);
#define SWAPSHORT(name) name=pEndian->Short(name);

using std::cout;
using std::cerr;
using std::endl;

/*#define tgerr cerr << "[TGE]" << __FILE__ << "::" << __LINE__ << ", "
#define tgout cout << "[TGE]" << __FILE__ << "::" << __LINE__ << ", "*/
#define tgout cout
#define tgerr cerr


#endif

