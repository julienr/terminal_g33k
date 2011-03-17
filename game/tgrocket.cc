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

#include "tgrocket.h"
#include "tggame.h"
#include "tgrocket_launcher.h"
#include "tgiparticle_system.h"

tgRocket::tgRocket (tgRocketLauncher* p) : tgProjectile(p)
{
  pModel = pGame->GetModelManager()->GetInstance("rocket");
	tgQuaternion q;
	pModel->SetRotation(q);
  pGame->GetWorldManager()->AddModel(pModel);

	pBBox = new tgAABoundingBox(tgVector3(-10,-10,-10), tgVector3(10,10,10));
	pModel->SetBoundingVolume(pBBox);

  pPhysicNode = new tgPhysicNode(pModel);
  pPhysicNode->SetPhysicType(PHYS_PROJ);
  pPhysicNode->SetObjectType(OBJ_PROJECTILE);
  pPhysicNode->SetTouchCallback(tgMakeFunctor((tgFunctor3<const tgVector3&, const tgVector3&, tgPhysicNode* >*)0, *this, &tgRocket::Touch));
  pPhysicNode->SetHitWallCallback(tgMakeFunctor((tgFunctor2<const tgVector3&, const tgVector3& >*)0, *this, &tgRocket::HitWall));

  light = new tgLight(tgVector3(0,0,0), tgColor(0,0,1), 150.0f);
  pPhysicNode->AttachEntity(light);
  pGame->GetWorldManager()->AddLight(light);

  trailEmitter = pGame->GetParticleManager()->GetInstance("grenadetrail");
  trailEmitter->SetRespawn(true);
  trailEmitter->Emit();
  pPhysicNode->AttachEntity(trailEmitter);
  pGame->GetWorldManager()->AddParticleSystem(trailEmitter);
}

tgRocket::~tgRocket ()
{
  pPhysicNode->DetachEntity(light);
  pGame->GetWorldManager()->RemoveLight(light);
  delete light;

  pPhysicNode->DetachEntity(trailEmitter);
  pGame->GetWorldManager()->RemoveParticleSystem(trailEmitter);
  delete trailEmitter;

  pGame->GetWorldManager()->RemoveModel(pModel);
  delete pPhysicNode;
  delete pBBox;
  delete pModel;
}

void tgRocket::HitWall (const tgVector3& vNormal, const tgVector3& vPoint)
{
  _Explosion(vNormal, vPoint);
}

void tgRocket::Touch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other)
{
  if (other->GetObjectType() == OBJ_PROJECTILE)
    return;
  _Explosion(vNormal, vPoint);
}

void tgRocket::_Explosion (const tgVector3& vNormal, const tgVector3& vPoint)
{
  static_cast<tgRocketLauncher*>(pParent)->DeleteMe(this);

  //emitter & light
  tgIParticleSystem* emitter1 = pGame->GetParticleManager()->GetInstance("rocketexplosion");
  emitter1->SetPosition(vPoint);
  emitter1->Emit();
  pGame->GetWorldManager()->AddParticleSystem(emitter1);

  tgLight* explLight = new tgLight(vPoint, tgColor(1,1,1), 300.0f, 5.0f, false);
  pGame->GetWorldManager()->AddLight(explLight);

  //add force to entity touched by explosion
  //FIXME: doesn't work !
  tgvector<tgIEntity*> touchedEntities;
  tgBoundingSphere* sphere = new tgBoundingSphere(vPoint, 150);
  pGame->GetWorldManager()->GetTouchingEntities(sphere, touchedEntities);
  for(unsigned i=0; i<touchedEntities.size(); i++) {
    tgPhysicNode* tmpNode = pGame->GetPhysicEngine()->EntityToNode(touchedEntities[i]);
//		cout << tmpNode << endl;
    if (tmpNode && tmpNode->GetObjectType() == OBJ_PLAYER) {
	    tgVector3 dir = tmpNode->GetPosition()-vPoint;
	    tmpNode->AddForce(new tgForce(dir*1000, false, 0.3f));

      tgPlayer* player = pGame->PhysicNodeToPlayer(tmpNode);
      if (player) {
        player->TakeDamage(ROCKET_DAMAGE, vNormal, vPoint);
      }
    }
  }
  delete sphere;
}

