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

#ifndef TGLIGHT_H
#define TGLIGHT_H

#include "tgicanvas.h"
#include "tgientity.h"

class tgLight : public tgIEntity
{
  public:
    /**  Herités de tgIEntity  **/
    void SetPosition (const tgVector3& newPos) { position = newPos; }
    void SetRotation (const tgQuaternion& newRot) {}
    void SetJointRotation (const tgstring& jointName, const tgQuaternion& newRot) {}

    tgBoundingVolume* GetBoundingVolume () { return NULL; }
    void SetBoundingVolume (tgBoundingVolume* volume) { }

    const tgVector3& GetPosition () { return position; }
    const tgQuaternion& GetRotation () { return rotation; }

  	void SetViewModel(bool b) {}
	  bool IsViewModel() { return false; }
	
  	const tgVector3& GetOriginPosition() { return tgVector3::ZERO; }
  protected:
    tgVector3 vPosition;
    tgQuaternion qRotation;
    //tgAABoundingBox* pBBox;
    tgBoundingVolume* pBVolume;


  public:
    tgLight (const tgVector3& pos, const tgColor& col, float rad, float life=0.0f, bool inf=true) : position(pos),color(col),radius(rad),lifeTime(life),baseLife(life),infinite(inf) {}
    ~tgLight () {}

    float GetRadius () { return radius; }
    void SetRadius (float f) { radius = f; }
    const tgColor& GetColor() { return color; }
    float GetLifeTime () { return lifeTime; }

    void SetLifeTime(float f) { lifeTime = f; }

    float GetBaseLife () { return baseLife; }

    bool IsInfinite () { return infinite; }
  protected:
    tgVector3 position;
    tgQuaternion rotation;
    tgColor color;
    float radius;
    float lifeTime; //le temps restant a vivre
    float baseLife; //la duree de vie totale
    bool infinite;

};

#endif

