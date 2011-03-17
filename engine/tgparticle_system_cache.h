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

#ifndef TGPARTICLE_SYSTEM_CACHE_H
#define TGPARTICLE_SYSTEM_CACHE_H

#include "tgparticle_emitter_cache.h"

class tgParticleSystemCache
{
  public:
    tgParticleSystemCache (int file);
    ~tgParticleSystemCache ();

    int GetNumEmitters () { return particleEmittersCache.size(); }
    tgParticleEmitterCache* GetEmitterCache (int i) { return particleEmittersCache[i]; }

    bool IsOk () const { return bOk; }

    const tgstring& GetName() { return name; }
  protected:
    tgvector<tgParticleEmitterCache*> particleEmittersCache;

    bool bOk;

    tgstring name; //le nom du system (lu dans le fichier)
    int depthHack;
};

#endif

