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

#include "tgparticle_system.h"
#include "tgworld_manager.h"

tgParticleSystem::tgParticleSystem(tgParticleSystemCache* c) : vPosition(0,0,0), qRotation(0,tgVector3::ZNORM)
{
  cache = c;
  pBVolume = NULL;
  respawn = false;

  _CreateEmitters();
}

void tgParticleSystem::_CreateEmitters ()
{
  for(int i=0; i<cache->GetNumEmitters(); i++) {
    tgParticleEmitter* e = new tgParticleEmitter(cache->GetEmitterCache(i), this);
    emittersList.push_back(e);
  }
}

tgParticleSystem::~tgParticleSystem()
{
  for(t_emittersList::iterator i=emittersList.begin(); i!=emittersList.end(); i++) {
    delete *i;
  }
}

void tgParticleSystem::Emit ()
{
  for(t_emittersList::iterator i=emittersList.begin(); i!=emittersList.end(); i++) {
    (*i)->Emit();
  }
}

void tgParticleSystem::Think (float eTime)
{
  for(t_emittersList::iterator i=emittersList.begin(); i!=emittersList.end(); i++) {
    (*i)->Think(eTime);
  }

  for (tgset<tgParticleEmitter*>::iterator i=emittersToDelete.begin(); i!=emittersToDelete.end(); i++) {
    _RemoveEmitter(*i);
    delete *i;
  }
  emittersToDelete.clear();
  if (emittersList.empty()) {
    if (respawn) {
      _CreateEmitters();
      Emit();
    } else {
      pWorldManager->DeleteMe(this);
    }
  }
}

void tgParticleSystem::_RemoveEmitter (tgParticleEmitter* e)
{
	bool found = false;
	t_emittersList::iterator i;
	for(i=emittersList.begin(); i!=emittersList.end(); i++) {
		if((*i) == e) {
			found = true;
			break;
		}
	}
	if(!found)
		return;

	emittersList.erase(i);
}

void tgParticleSystem::DeleteMe (tgParticleEmitter* e)
{
  emittersToDelete.insert(e);
}

/* tgIEntity */
void tgParticleSystem::SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot)
{
  tgout << "tgParticleEmitter::SetJointRotation irrelevant" << endl;
}

void tgParticleSystem::SetViewModel(bool b)
{
  tgout << "tgParticleEmitter::SetViewModel irrelevant" << endl;
}

bool tgParticleSystem::IsViewModel()
{
  return false;
}


