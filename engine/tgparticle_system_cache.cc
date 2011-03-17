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

#include "tgparticle_system_cache.h"

#define NEXT(str) while(buffer.find(str)==tgstring::npos && !pFileManager->EndOfFile(file)) \
			pFileManager->Read(file, buffer);

tgParticleSystemCache::tgParticleSystemCache (int file)
{
  bOk = true;

	tgstring buffer;
//tgParticleManager a deja lu "particle"
//  NEXT("particle")
 
  pFileManager->Read(file, name);
//  tgout << "system name : " << name << endl;

  pFileManager->Read(file, buffer);

  if (buffer == "depthHack") {
    pFileManager->Read(file, depthHack);
    pFileManager->Read(file, buffer);
  } 

  while (buffer == "{") {
    tgParticleEmitterCache* emit = new tgParticleEmitterCache(file);
    if (!emit->IsOk()) {
      delete emit;
      break;
    }
    particleEmittersCache.push_back(emit);
    //le constructeur de tgParticleEmitterCache doit s'arreter sur le '}'
    pFileManager->Read(file, buffer);
  }
  
}

tgParticleSystemCache::~tgParticleSystemCache ()
{
  for(unsigned i=0; i<particleEmittersCache.size(); i++) {
    delete particleEmittersCache[i];
  }
}



