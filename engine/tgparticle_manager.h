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

#ifndef TGPARTICLE_MANAGER_H
#define TGPARTICLE_MANAGER_H

#include "tgdefs.h"
#include "tgiparticle_manager.h"
#include "tgparticle_system.h"

class tgParticleManager : public tgIParticleManager
{
  public:
    tgParticleManager ();
    ~tgParticleManager ();
    
    void PreLoadFile (const tgstring& filename);

    //avance jusqu'a toRead en skippant les commentaires
    void NextEatComment(int file, const tgstring& toRead);
    
    tgIParticleSystem* GetInstance (const tgstring& partname);
  protected:
  	typedef tgmap<tgstring, tgParticleSystemCache*> systemsMap_t;
  	systemsMap_t mSystems;

};

#endif

