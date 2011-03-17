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

#include "tgphysic_node.h"

tgPhysicNode::tgPhysicNode(tgIEntity* ent)
{
	pEntity = ent;
	fMass = 1.0f;
	bNoClip = false;
	vPosition.Set(0,0,0);
  vPreviousPosition = vPosition;
  interpolation.progress = 0.0;
  interpolation.interpolation = 0.0;
  interpolation.startHeight = 0.0;
  vAccel.Set(0,0,0);
  vVelocity.Set(0,0,0);
  posType = POS_GROUND;
  physType = PHYS_WALK;
  objType = OBJ_UNDEFINED;
}

tgPhysicNode::~tgPhysicNode()
{

}

void tgPhysicNode::AddForce(tgForce* f)
{
	tForces.insert(f);
}

void tgPhysicNode::RemoveForce(tgForce* f)
{
	tForces.erase(f);
}

void tgPhysicNode::AttachEntity(tgIEntity* e)
{
  attachedEntities.insert(e);
}

void tgPhysicNode::DetachEntity(tgIEntity* e)
{
  attachedEntities.erase(e);
}

void tgPhysicNode::SetNoClip(bool b)
{
  bNoClip = b;
  vAccel.Set(0,0,0); //reset accel
}

void tgPhysicNode::UpdateEntity()
{
	pEntity->SetPosition(vPosition);
  pEntity->SetRotation(qRotation);

  for(t_attachedEntities::iterator i = attachedEntities.begin(); i!=attachedEntities.end(); i++) {
    tgIEntity* e = *i;
    e->SetPosition(vPosition);
    e->SetRotation(qRotation);
  }
}

void tgPhysicNode::_UpdateWalkAccel (float eTime)
{
  //FIXME: c'est pas tres malin de diviser par une quantite fixe
  // le 10.0f devrait dependre du eTime
  vAccel /= SPEED*eTime;
/*  vAccel.x = vAccel.x/10.0f;
  vAccel.z = vAccel.z/10.0f;
  vAccel.y = vAccel.y/10.0f;*/

  tgVector3 tmp;
  for(t_forces::iterator f= tForces.begin(); f!=tForces.end(); ) {
    tgForce* force = (*f);
    t_forces::iterator next = f;
    next++;

    tmp += force->GetForce();

    if(!force->IsInfinite()) {
      force->SetDuration(force->GetDuration() - eTime);
      if(force->GetDuration() < 0.0f) {
        //FIXME: c'est cense compenser le fait que si le jeu tourne vite, on va ajouter plus de fois la force
        // -> si elle a une duration de 1.0 et qu'on updateAccel tte les 0.1, on va ajouter 10 fois la force
        // -> si on update tous les 0.6, on va l'ajouter deux fois avant de l'enlever, pourtant la deuxime fois on est a 1.2 secondes
        tmp -= force->GetForce()*force->GetDuration();
        //FIXME: we probably want reference counted forces to avoid problems when a force is applied to more than one object
        delete force;
        tForces.erase(f);
      }
    }
    f = next;
  }
  vAccel += tmp/fMass;

}

void tgPhysicNode::_UpdateProjAccel (float eTime)
{
  tgVector3 tmp;
  for(t_forces::iterator f= tForces.begin(); f!=tForces.end(); ) {
    tgForce* force = (*f);
    t_forces::iterator next = f;
    next++;

    tmp += force->GetForce();

    if(!force->IsInfinite()) {
      force->SetDuration(force->GetDuration() - eTime);
      if(force->GetDuration() < 0.0f) {
        //FIXME: c'est cense compenser le fait que si le jeu tourne vite, on va ajouter plus de fois la force
        // -> si elle a une duration de 1.0 et qu'on updateAccel tte les 0.1, on va ajouter 10 fois la force
        // -> si on update tous les 0.6, on va l'ajouter deux fois avant de l'enlever, pourtant la deuxime fois on est a 1.2 secondes
        tmp -= force->GetForce()*force->GetDuration();
        //FIXME: we probably want reference counted forces to avoid problems when a force is applied to more than one object
        delete force;
        tForces.erase(f);
      }
    }
    f = next;
  }
  vAccel = tmp/fMass;

}

void tgPhysicNode::UpdateAccel (float eTime)
{
  switch(physType) {
    case PHYS_WALK:
      _UpdateWalkAccel(eTime);
      break;
    case PHYS_PROJ:
      _UpdateProjAccel(eTime);
      break;
    default:
      break;
  }
}

void tgPhysicNode::HitWall (const tgVector3& vNormal, const tgVector3& vPoint) const
{
  if (cbHitWall)
    cbHitWall(vNormal, vPoint);
}

void tgPhysicNode::Landed (const tgVector3& vNormal, const tgVector3& vPoint) const
{
  if (cbLanded)
    cbLanded(vNormal, vPoint);
}

void tgPhysicNode::Touch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other) const
{
  if (cbTouch)
    cbTouch(vNormal, vPoint, other);
}

void tgPhysicNode::UnTouch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other) const
{
  if (cbUnTouch)
    cbUnTouch(vNormal, vPoint, other);
}

