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

#include "tgparticle_manager.h"

void tgParticleManager::NextEatComment(int file, const tgstring& toRead)
{
  char buf;
  char lastRead=' ';
  bool comment = false;
  unsigned cmpIndex = 0;

  while (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, buf);
    if (buf=='*' && lastRead=='/') comment = true;
    if (buf=='/' && lastRead=='*') comment = false;
    lastRead = buf;
    if (comment) continue;

    if(toRead[cmpIndex] == buf) cmpIndex++;
    else cmpIndex = 0;

    if (cmpIndex == toRead.size()) //found
      return;
  }
}

tgParticleManager::tgParticleManager ()
{

}

tgParticleManager::~tgParticleManager ()
{
	for (systemsMap_t::iterator i = mSystems.begin(); i != mSystems.end(); i++) {
		delete i->second;
	}
}

#define NEXT(str) while(buffer.find(str)==tgstring::npos && !pFileManager->EndOfFile(file)) \
			pFileManager->Read(file, buffer);

void tgParticleManager::PreLoadFile (const tgstring& filename)
{
  int file = pFileManager->Open(filename.data(), TGFILE_TEXT);
  tgstring buffer;
  while (!pFileManager->EndOfFile(file)) {
    NextEatComment(file, "particle");
    tgParticleSystemCache* sys = new tgParticleSystemCache(file);
    if (!sys->IsOk()) {
      tgout << "tgParticleManager::PreLoadFile error loading " << sys->GetName() << endl;
      delete sys;
      continue;
    }
   
    mSystems[sys->GetName()] = sys;
    tgout << "tgParticleManager::PreLoadFile loaded " << sys->GetName() << endl;
  }
  pFileManager->Close(file);
}

tgIParticleSystem* tgParticleManager::GetInstance (const tgstring& partname)
{
	systemsMap_t::iterator i = mSystems.begin ();
	if((i=mSystems.find(partname)) == mSystems.end()) {//non charge
    tgout << "tgParticleManager::GetInstance, ERROR no instance named " << partname << endl;
		return NULL;
	} else {
		return new tgParticleSystem(i->second);
	}
}

