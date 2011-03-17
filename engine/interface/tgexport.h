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
#ifndef TGEXPORT_H
#define TGEXPORT_H

//comptabilite Win32


#include "tgiworld_manager.h"
#include "tgibind_manager.h"
#include "tgifile_manager.h"
#include "tgimodel_manager.h"
#include "tgiparticle_manager.h"
#include "tgitimer.h"
#include "tgiconsole.h"
#include "tgicanvas.h"
#include "tgimaterial_manager.h"
#include "tgirenderview.h"

struct tgEngineExport
{
	tgIWorldManager* pWorldManager;
	tgIBindManager* pBindManager;
	tgIFileManager* pFileManager;
	tgIModelManager* pModelManager;
  tgIParticleManager* pParticleManager;
	tgITimer* pTimer;
	tgIConsole* pConsole;
	tgICanvas* pCanvas;
	tgIMaterialManager* pMaterialManager;
	tgIRenderView* pRenderView;
};

#endif
