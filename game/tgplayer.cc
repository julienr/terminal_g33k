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

#include "tgplayer.h"
#include "tggame.h"
#include "tgmachinegun.h"
#include "tgrocket_launcher.h"


#define NUM_WEAPONS 2

tgPlayer::tgPlayer()
{
  isLocalPlayer = false;

  pModel = pGame->GetModelManager()->GetInstance("player");
  pModel->SetAnimation("idle");
	tgQuaternion q;
	pModel->SetRotation(q);

	pBBox = new tgAABoundingBox(tgVector3(-20,-30,-20), tgVector3(20,30,20));
	pModel->SetBoundingVolume(pBBox);
  pModel->SetPosOffset(tgVector3(0,-30,0));

	pPhysicNode = new tgPhysicNode(pModel);
  pPhysicNode->SetPhysicType(PHYS_WALK);
  pPhysicNode->SetObjectType(OBJ_PLAYER);

  pPhysicNode->SetTouchCallback(tgMakeFunctor((tgFunctor3<const tgVector3&, const tgVector3&, tgPhysicNode* >*)0, *this, &tgPlayer::Touch));
  pPhysicNode->SetUnTouchCallback(tgMakeFunctor((tgFunctor3<const tgVector3&, const tgVector3&, tgPhysicNode* >*)0, *this, &tgPlayer::UnTouch));

  pWeapons = new tgWeapon*[NUM_WEAPONS];
  pWeapons[0] = new tgMachinegun(this);
  pWeapons[1] = new tgRocketLauncher(this);
  currentWeapon = 1;

  light = new tgLight(tgVector3(0,0,0), tgColor(1,0,0), 500.0f);
  pPhysicNode->AttachEntity(light);
  pGame->GetWorldManager()->AddLight(light);
}

tgPlayer::~tgPlayer()
{
  pPhysicNode->DetachEntity(light);
  pGame->GetWorldManager()->RemoveLight(light);
  delete light;
  delete pBBox;
	delete pPhysicNode;
	delete pModel;
  for(int i=0; i<NUM_WEAPONS; i++) {
    delete pWeapons[i];
  }
  delete [] pWeapons;

}

void tgPlayer::SwitchWeapon (int num)
{
  pWeapons[currentWeapon]->UnSelect();
  if (num == -1) { //next
    currentWeapon = (currentWeapon+1)%NUM_WEAPONS;
  } else if (num == -2) { //prev
    currentWeapon--;
    currentWeapon = (currentWeapon < 0)?NUM_WEAPONS-1:currentWeapon;
  } else if (num >= 0 && num < NUM_WEAPONS) {
    currentWeapon = num; 
  } else {
    cerr << "SwitchWeapon, num out of range: " << num << endl;
  }
  pWeapons[currentWeapon]->Select();
}

void tgPlayer::Fire()
{
  if(currentWeapon < 0) return;

  pWeapons[currentWeapon]->Fire(pPhysicNode->GetPosition(), GetViewDirection(), pPhysicNode->GetRotation());
}

tgPhysicNode* tgPlayer::GetPhysicNode()
{
	return pPhysicNode;
}

tgVector3 tgPlayer::GetViewDirection() const
{
  return (pPhysicNode->GetRotation().GetZAxis())*(-1);
}

tgIModel* tgPlayer::GetModel() const 
{
	return pModel;
}

void tgPlayer::Think ()
{
  for(int i=0; i<NUM_WEAPONS; i++) {
    pWeapons[i]->Think();
  }
}

void tgPlayer::TakeDamage (int dam, const tgVector3& vNormal, const tgVector3& vPoint)
{
  cout << "damage taken : " << dam << endl;
  tgIParticleSystem* emitter1 = pGame->GetParticleManager()->GetInstance("rocketexplosion");
  emitter1->SetPosition(vPoint);
  emitter1->Emit();
  pGame->GetWorldManager()->AddParticleSystem(emitter1);

}

void tgPlayer::Touch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other)
{

}

void tgPlayer::UnTouch (const tgVector3& vNormal, const tgVector3& vPoint, tgPhysicNode* other)
{

}


