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

#ifndef TGPARTICLE_EMITTER_CACHE_H
#define TGPARTICLE_EMITTER_CACHE_H

#include "tgdefs.h"
#include "tgmaterial.h"
#include <stdlib.h> //rand
#define RAND_1 (double(rand())/RAND_MAX)

struct tgParticle
{ 
  tgParticle() : vPosition(0,0,0),vVelocity(0,0,0),lifeTime(0),deadTime(0),size(1.0f),
                 cycles(0.0),isAlive(true)
  { }
                 

  tgVector3 vPosition;
  tgVector3 vVelocity;
  float lifeTime;
  float deadTime;
  float size;
/*  float fadeIn;
  float fadeOut;*/
  float cycles;
  bool isAlive;
  float color[4];
};

struct tgParticleDistribution
{
  virtual tgVector3 GetRandomPosition () = 0;
  virtual ~tgParticleDistribution() {}
};

struct tgParticleBoxDistribution : public tgParticleDistribution
{
  tgParticleBoxDistribution(float l, float w, float h);
  virtual ~tgParticleBoxDistribution() {}
  tgVector3 GetRandomPosition ();
  protected:
    float length, height, width; //size on x,z,y
};

struct tgParticleSphereDistribution : public tgParticleDistribution
{
  tgParticleSphereDistribution(float r);
  virtual ~tgParticleSphereDistribution() {}
  tgVector3 GetRandomPosition ();
  protected:
    float radius;
};

/**
    Doom3 particles syntax : 
    http://iddevnet.com/doom3/editor_particle.php
*/

class tgParticleEmitterCache
{
  public:
    tgParticleEmitterCache(int file);
    ~tgParticleEmitterCache();

    inline const tgParticle& GetRefParticle () { return refPart; }

    inline tgMaterial* GetMaterial () const { return material; }
    inline tgParticleDistribution* GetDistribution () const { return distribution; }
    void GetDirection (tgVector3& result) const;
    inline bool HasDirCone () const { return bDirCone; }
    inline float GetDirConeAngle () const { return dirConeAngle; }
    inline float GetStartSize () const { return size[0]; }
    inline float GetEndSize () const { return size[1]; }
    inline float GetSizeGrowth () const { return size[2]; }
    inline float GetStartSpeed () const { return speed[0]; }
    inline float GetEndSpeed () const { return speed[1]; }
    inline float GetSpeedGrowth () const { return speed[2]; }

    inline float GetFadeInTime () const { return fadeIn; }
    inline float GetFadeOutTime () const { return fadeOut; }

    inline const float* GetFadeColor () const { return fadeColor; }
    inline int GetCount () const { return count; }

    inline bool IsInfinite () { return infinite; }

    bool IsOk () { return bOk; }


  protected:
    bool bOk;
    tgParticle refPart;
    tgMaterial* material;
    tgParticleDistribution* distribution;
    tgVector3 direction; //direction de la vitesse initiale des particules
    bool bDirCone; //la direction doit-elle etre donne par un cone aleatoire ?
    float dirConeAngle; //l'angle du cone dans lequel on selectionne une direction au bol

    //pour tous les intervalles, [0]=>debut, [1]=>fin, [2]=>gagne/seconde
    float size[3];
    float fadeIn;
    float fadeOut;
    float fadeColor[4];
    int count;
    bool randomDistribution;
    float bunching;
    float speed[3];
    float zbias;

    bool infinite;
};

#endif
