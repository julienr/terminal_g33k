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

#include "tgparticle_emitter_cache.h"
#include "tgmaterial_manager.h"

// tgParticleDistribution


tgParticleBoxDistribution::tgParticleBoxDistribution (float l, float w, float h) : length(l), height(h), width(w)
{
  
}

tgVector3 tgParticleBoxDistribution::GetRandomPosition ()
{
  return tgVector3(RAND_1*length, RAND_1*width, RAND_1*height);  
}

tgParticleSphereDistribution::tgParticleSphereDistribution (float r) : radius(r)
{
}

tgVector3 tgParticleSphereDistribution::GetRandomPosition ()
{
  //x²+y²+z²=r²
  float x = sqrt(radius*radius*RAND_1); //x entre 0 et r
  float y = sqrt((radius*radius - x*x)*RAND_1); //y entre 0 et r²-x²
  float z = sqrt(radius*radius - x*x - y*y); // z = r²-x²-y²
  return tgVector3(x,y,z);
}

// tgParticleEmitterCache
#define NEXT(str) { \
      buffer.clear(); \
      while(buffer.find(str)==tgstring::npos && !pFileManager->EndOfFile(file)) \
			pFileManager->Read(file, buffer); \
      }

#define NEXTCHAR(c) { \
    do{ pFileManager->Read(file, charbuffer); }while (charbuffer != c && !pFileManager->EndOfFile(file));  \
   }

//FIXME si on met a 0.6, le muzzle des rockets s'affiche
#define SCALE_FACTOR 0.5
//#define SCALE_FACTOR 0.06
//des que size passe a > 2.0f (> 1.0f dans renderer puisque divise par 2), ca donne correct
//#define SCALE_FACTOR 0.6

//comparer les tailles dans les fichiers, on ne devrait pas avoir plus d'un facteur 3-4 entre les deux particules..

//l'erreur pourrait etre que la size lue est la mauvaise

tgParticleEmitterCache::tgParticleEmitterCache(int file) : direction(0,0,1),
                                                           bDirCone(false), 
                                                           count(0)

{
  size[0] = size[1] = 1.0f;
  speed[0] = speed[1] = 0.0f;
  fadeIn = 0.0f;
  fadeOut = 0.0f;
  bOk = true;
  distribution = NULL;
  infinite = true;

  int beginOffset = pFileManager->GetReadOffset(file); //utilise pour retourner au debut du bloc
  //cout << beginOffset << endl;
	tgstring buffer;
  char charbuffer = ' ';
  
  NEXT("material")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, buffer);
    material = pMaterialManager->GetMaterial(buffer);
  }
  pFileManager->Seek(file, beginOffset); 

  NEXT("count")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, count); 
  }
  pFileManager->Seek(file, beginOffset); 

  NEXT("time")
  if (!pFileManager->EndOfFile(file)) { 
    pFileManager->Read(file, refPart.lifeTime); 
  }
  pFileManager->Seek(file, beginOffset); 

  NEXT("deadTime")
  if (!pFileManager->EndOfFile(file)) { 
    pFileManager->Read(file, refPart.deadTime); 
  }
  pFileManager->Seek(file, beginOffset); 

  NEXT("cycles")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, refPart.cycles);
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("bunching")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, bunching);
  }
  pFileManager->Seek(file, beginOffset);
 

  NEXT("distribution")
  if (!pFileManager->EndOfFile(file)) { 
    pFileManager->Read(file, buffer);
    if (buffer.find("rect") != tgstring::npos) {
     float l,w,h;
     pFileManager->Read(file, l);
     pFileManager->Read(file, w);
     pFileManager->Read(file, h);
     distribution = new tgParticleBoxDistribution(l*SCALE_FACTOR,w*SCALE_FACTOR,h*SCALE_FACTOR);
    } else if (buffer.find("sphere") != tgstring::npos) {
     float values[3];
     pFileManager->Read(file, values[0]);
     pFileManager->Read(file, values[1]);
     pFileManager->Read(file, values[2]);
     //TODO: use the yaxis and zaxis size too
     distribution = new tgParticleSphereDistribution(values[0]*SCALE_FACTOR);
    } else if (buffer.find("cylinder") != tgstring::npos) {
      float values[4];
      pFileManager->Read(file, values[0]);
      pFileManager->Read(file, values[1]);
      pFileManager->Read(file, values[2]);
      pFileManager->Read(file, values[3]);
      //TODO: do something =)
      distribution = new tgParticleSphereDistribution(values[0]*SCALE_FACTOR);
    }
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("randomDistribution")
  if (!pFileManager->EndOfFile(file)) {
    int tmp;
    pFileManager->Read(file, tmp);
    randomDistribution = tmp;
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("direction")
  if (!pFileManager->EndOfFile(file)) { 
    pFileManager->Read(file, buffer);
    if (buffer.find("outward") != tgstring::npos) {
      direction.Set(0,1,0); 
      NEXTCHAR('"')
      pFileManager->Read(file, zbias);
    } else if (buffer.find("cone") != tgstring::npos) {
      bDirCone = true;
      NEXTCHAR('"')
      pFileManager->Read(file, dirConeAngle);
    }
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("orientation")
  if (!pFileManager->EndOfFile(file)) {
    //may be aimed/view/x/y/z
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("speed")
  if (!pFileManager->EndOfFile(file)) {
    NEXTCHAR('"')
    pFileManager->Read(file, speed[0]);
    speed[0] *= SCALE_FACTOR;
    NEXTCHAR('"')
    pFileManager->Read(file, buffer);
    if (buffer.find("to") != tgstring::npos) { //speed intervall
      NEXTCHAR('"')
      pFileManager->Read(file, speed[1]);
      speed[1] *= SCALE_FACTOR;
    } else {
      speed[1] = speed[0];
    }
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("size")
  if (!pFileManager->EndOfFile(file)) { 
    NEXTCHAR('"')
    pFileManager->Read(file, size[0]);
    size[0] *= SCALE_FACTOR;
    //cout << "size[0] : " << size[0] << endl;
    NEXTCHAR('"')
    pFileManager->Read(file, buffer);
    if (buffer.find("to") != tgstring::npos) { //speed intervall
      //tgout << "intervall" << endl;
      NEXTCHAR('"')
      pFileManager->Read(file, size[1]);
      size[1] *= SCALE_FACTOR;
    } else {
      size[1] = size[0];
    }
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("fadeIn")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, fadeIn);
  }
  pFileManager->Seek(file, beginOffset);

  NEXT("fadeOut")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, fadeOut);
  }
  pFileManager->Seek(file, beginOffset);
 
  NEXT("color")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, refPart.color[0]);
    pFileManager->Read(file, refPart.color[1]);
    pFileManager->Read(file, refPart.color[2]);
    pFileManager->Read(file, refPart.color[3]);
  } 
  pFileManager->Seek(file, beginOffset);
  
  NEXT("fadeColor")
  if (!pFileManager->EndOfFile(file)) {
    pFileManager->Read(file, fadeColor[0]);
    pFileManager->Read(file, fadeColor[1]);
    pFileManager->Read(file, fadeColor[2]);
    pFileManager->Read(file, fadeColor[3]);
  } 
  pFileManager->Seek(file, beginOffset);

  NEXTCHAR('}')
  if (pFileManager->EndOfFile(file)) {
    tgout << "tgParticleEmitterCache::tgParticleEmitterCache unexcepted end of file" << endl;
    bOk = false;
  }

  //initialisations
  fadeIn *= refPart.lifeTime;
  fadeOut *= refPart.lifeTime;
  speed[2] = (speed[1]-speed[0])/refPart.lifeTime;
  size[2] = (size[1]-size[0])/refPart.lifeTime;
  //cout << "size growth : " << size[0] << " to " << size[1] << " : " << size[2] << endl;
  refPart.size = size[0];

  if (refPart.cycles > 0.0)
    infinite = false;

}

tgParticleEmitterCache::~tgParticleEmitterCache()
{
  delete distribution;
}

void tgParticleEmitterCache::GetDirection (tgVector3& result) const
{ 
  result.Set(RAND_1, RAND_1+zbias, RAND_1);
  result.Normalize();
}
