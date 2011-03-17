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

#include "tgparticle_emitter.h"
#include "tgworld_manager.h"
#include "tgparticle_system.h"

tgParticleEmitter::tgParticleEmitter(tgParticleEmitterCache* c, tgParticleSystem* parent) 
{
  cache = c;  
  pParent = parent;
}

tgParticleEmitter::~tgParticleEmitter()
{
  for (particlesList_t::iterator i=particlesList.begin(); i!=particlesList.end(); i++) {
    delete *i;
  }
}

void tgParticleEmitter::_InitParticle (tgParticle* p)
{
  *p = cache->GetRefParticle();
  if (cache->GetDistribution())
    p->vPosition = cache->GetDistribution()->GetRandomPosition();

  if (cache->HasDirCone()) {
    /*
        angle (zOv) = alpha
        angle (vOx) = beta = 90 - alpha
        angle (xOy) = gamma
        v un vecteur aleatoire pris dans une cone d'angle par rapport au z max alpha_max
        => 0 < alpha < alpha_max
        => 0 < gamma < 360

        v.x = cos(beta)
        v.y = cos(gamma)
        v.z = sin(beta)

        z
        ^
        |        v
        |      /
        |    /
        |  /
        |/  
        |------------------>x
        /
        /
        /
        /
        /
        <
        y
     */
    float beta = 90 - RAND_1*cache->GetDirConeAngle();
    float gamma = RAND_1*360;
    p->vVelocity.Set(cosf(beta), cosf(gamma), sinf(beta));
    p->vVelocity *= cache->GetStartSpeed();
  } else {
    cache->GetDirection(p->vVelocity);
    p->vVelocity *= cache->GetStartSpeed();
  }
}

void tgParticleEmitter::Emit ()
{
  for (int i=0; i<cache->GetCount(); i++) {
    tgParticle* p = new tgParticle();
    _InitParticle(p);
    particlesList.insert(p);
  }
}

void tgParticleEmitter::Think (float eTime)
{
  //particules
  for (particlesList_t::iterator i=particlesList.begin(); i!=particlesList.end(); ) {
    tgParticle* p = *i;
    particlesList_t::iterator next = i;
    next++;

    if (p->isAlive) {
      p->vPosition += p->vVelocity*eTime;
      p->size += cache->GetSizeGrowth()*eTime;
      
      p->lifeTime -= eTime;
      //verification de la duree de vie
      if (p->lifeTime <= 0) {
        p->isAlive = false;
        p->cycles -= 1.0f;
      }
      if (p->cycles <= 0 && !cache->IsInfinite()) {
        delete p;
        particlesList.erase(i);
      }

      //fadeIn
      /*if (p->lifeTime >= cache->GetFadeInTime()) {
       for (int i=0; i<4; i++) {
        p->color[i] = (cache->GetFadeColor() - cache->GetRefPart().color[i])/cache->GetRefPart().lifetime;
       }
      }

      //fadeOut
      if (p->lifeTime <= cache->GetFadeOutTime()) {
       for (int i=0; i<4; i++) {
        p->color[i] = (1 - p->lifeTime + cache->GetFadeOutTime())*cache->GetFadeColor()[i];
       }
      }*/

    } else { 
      p->deadTime -= eTime;
      _InitParticle(p);
    }

    i = next;
  }
  //l'emitter meurt quand toutes les particules ont epuise leurs cycles
  if (particlesList.empty()) {
    pParent->DeleteMe(this);
  }
}


