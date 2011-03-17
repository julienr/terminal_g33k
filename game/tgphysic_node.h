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

#ifndef TGPHYSIC_NODE_H
#define TGPHYSIC_NODE_H

#include "tgdefs.h"
#include "tgforce.h"
#include "tgientity.h"
#include "tgfunctor.h"

class tgPhysicEngine;

enum ePositionType{
	POS_WATER,
	POS_AIR,
	POS_GROUND
};

enum ePhysicType{
  PHYS_WALK,
  PHYS_PROJ
};

enum eObjectType {
  OBJ_UNDEFINED,
  OBJ_PROJECTILE,
  OBJ_PLAYER
};

/**
 * abstract class
 *  - define the base object used by tgPhysicEngine
 */
class tgPhysicNode
{
	friend class tgPhysicEngine;
	public:
		tgPhysicNode(tgIEntity*);
		virtual ~tgPhysicNode();

		const tgVector3& GetPosition() const { return vPosition; }
    //sets vPosition AND vPreviousPosition
    //ALWAYS use this function if you want to set the node position
    void SetAllPosition (const tgVector3& pos) { vPosition = vPreviousPosition = pos; }
		
		void SetRotation(const tgQuaternion& rot) { qRotation = rot; }
		const tgQuaternion& GetRotation() const { return qRotation; }

    const tgVector3& GetAccel() const { return vAccel; }
    void SetAccel(const tgVector3& v) { vAccel = v; }

		tgBoundingVolume* GetBVolume() const { return pEntity->GetBoundingVolume(); }

		tgIEntity* GetEntity() const { return pEntity; }

		void AddForce(tgForce* f);
		void RemoveForce(tgForce* f);

		void SetMass(float f) { fMass = f; }
		float GetMass() const { return fMass; }

    void SetVelocity(const tgVector3& vel) { vVelocity = vel; }
    const tgVector3& GetVelocity() const { return vVelocity; }

		void UpdateEntity();

    void AttachEntity (tgIEntity* e);
    void DetachEntity (tgIEntity* e);

		void SetNoClip(bool b);

		void SetPositionType (ePositionType type) { posType = type; }
		ePositionType GetPositionType() const { return posType; }

    void SetPhysicType (ePhysicType type) { physType = type; }
    ePhysicType GetPhysicType() const { return physType; }

    void SetObjectType (eObjectType type) { objType = type; }
    eObjectType GetObjectType() const { return objType; }

    void UpdateAccel (float eTime);
    
    struct Interpolation 
    {
      float progress;
      float interpolation;
      float startHeight;
    };

    Interpolation* GetInterpolation() { return &interpolation; }

    void SetGroundPlane (const tgVector3& v) { vGroundPlane = v; }
    const tgVector3& GetGroundPlane () const { return vGroundPlane; }

    /* Collisions callbacks */
    void HitWall (const tgVector3& vNormal, const tgVector3& vPoint) const;
    void Landed (const tgVector3& vNormal, const tgVector3& vPoint) const;
    void Touch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other) const;
    void UnTouch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other) const;

    void SetHitWallCallback(const tgFunctor2 <const tgVector3&, const tgVector3& > cb) { cbHitWall = cb; }
    void SetLandedCallback(const tgFunctor2 <const tgVector3&, const tgVector3& > cb) { cbLanded = cb; }
    void SetTouchCallback(const tgFunctor3 <const tgVector3&, const tgVector3&, tgPhysicNode* > cb) { cbTouch = cb; }
    void SetUnTouchCallback(const tgFunctor3 <const tgVector3&, const tgVector3&, tgPhysicNode* > cb) { cbUnTouch = cb; }

	protected:
    //should ONLY be used by tgphysic_engine
		void SetPosition(const tgVector3& pos) { vPosition = pos; }
   //previousPosition is stored to avoid 'steps' in physic rendering through interpolation
    void SetPreviousPosition (const tgVector3& pos) { vPreviousPosition = pos; }
    const tgVector3& GetPreviousPosition() const { return vPreviousPosition; }


    void _UpdateProjAccel (float eTime);
    void _UpdateWalkAccel (float eTime);

		typedef tgset<tgForce*> t_forces;
		t_forces tForces;

    typedef tgset<tgIEntity*> t_attachedEntities;
    t_attachedEntities attachedEntities;
		
		tgVector3 vPosition;
    tgVector3 vAccel;
    tgVector3 vVelocity;
		tgQuaternion qRotation;
    tgVector3 vPreviousPosition;


		tgIEntity* pEntity;

    //movement interpolation
    Interpolation interpolation;

    tgVector3 vGroundPlane;

		float fMass;

		bool bNoClip;
		ePositionType posType;
    ePhysicType physType;
    eObjectType objType;

    tgFunctor2<const tgVector3&, const tgVector3& > cbHitWall;
    tgFunctor2<const tgVector3&, const tgVector3& > cbLanded;
    tgFunctor3<const tgVector3&, const tgVector3&, tgPhysicNode* > cbTouch;
    tgFunctor3<const tgVector3&, const tgVector3&, tgPhysicNode* > cbUnTouch;
};

#endif

