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

#ifndef TGPARTICLE_SYSTEM_H
#define TGPARTICLE_SYSTEM_H

#include "tgparticle_system_cache.h"
#include "tgparticle_emitter.h"
#include "tgiparticle_system.h"

class tgParticleSystem : public tgIParticleSystem
{
  friend class tgGLRenderer;
  friend class tgGLRendererSTD;

  public:
   /**  Herités de tgIEntity  **/
   void SetPosition (const tgVector3& newPos) { vPosition = newPos; if(pBVolume)pBVolume->SetPosition(vPosition); }
   void SetRotation (const tgQuaternion& newRot) { qRotation = newRot; }

   tgBoundingVolume* GetBoundingVolume () { return pBVolume; }
   void SetBoundingVolume (tgBoundingVolume* volume) { pBVolume = volume; }

   const tgVector3& GetPosition () { return vPosition; }
   const tgQuaternion& GetRotation () { return qRotation; }
   const tgVector3& GetOriginPosition () { return tgVector3::ZERO; }
   
   void SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot);
   void SetViewModel(bool b);
   bool IsViewModel();

  protected:
    tgVector3 vPosition;
    tgQuaternion qRotation;
    tgBoundingVolume* pBVolume;

  public:
    tgParticleSystem(tgParticleSystemCache* c);
    ~tgParticleSystem();

    void Emit ();
    void Think (float eTime);
    void DeleteMe (tgParticleEmitter* e);

    //si respawn = true, lorsque l'emitter devrait mourrir, il se recree automatiquement
    void SetRespawn (bool b) { respawn = b; }

    int GetNumEmitters () { return emittersList.size(); }
    tgParticleEmitter* GetEmitter(int i) { return emittersList[i]; }

  protected:
    void _RemoveEmitter (tgParticleEmitter* e);
    void _CreateEmitters ();

    tgParticleSystemCache* cache;
    typedef tgvector<tgParticleEmitter*> t_emittersList;
    t_emittersList emittersList;
    tgset<tgParticleEmitter*> emittersToDelete;

    bool respawn;
};

#endif

