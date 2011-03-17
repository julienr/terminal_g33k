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
#include "tgrocket_launcher.h"
#include "tggame.h"

tgRocketLauncher::tgRocketLauncher(tgPlayer* p) : tgWeapon(p)
{
  pModel = pGame->GetModelManager()->GetInstance("rocketlauncher_view");
  pModel->SetAnimation("idle");
  pModel->SetVisible(false);

  pModel->SetRotation(tgQuaternion(90, tgVector3(0,1,0)));
	pModel->SetPosition(tgVector3(4,-55,-7));

  pGame->GetWorldManager()->AddModel(pModel);
}

tgRocketLauncher::~tgRocketLauncher()
{
  for(tgset<tgRocket*>::iterator i=rockets.begin(); i!=rockets.end(); i++) {
    delete *i;
  }
  pGame->GetWorldManager()->RemoveModel(pModel);
  delete pModel;
}

void tgRocketLauncher::UnSelect ()
{
  pModel->SetVisible(false);
}

void tgRocketLauncher::Select ()
{
  if (pPlayer->IsLocalPlayer())
    pModel->SetViewModel(true);
  else {
    //TODO: attach weapon to player
    pModel->SetPosition(pPlayer->GetPhysicNode()->GetPosition());
    pModel->SetViewModel(false);
  }
  pModel->SetVisible(true);
  //FIXME: segfault
//  pModel->SetAnimation("fire", -1, false);
}

void tgRocketLauncher::Fire (const tgVector3& vPosition, const tgVector3& vDirection, const tgQuaternion& qRot)
{
  tgRocket* r = new tgRocket(this);
  tgVector3 tmp = vDirection;
  tmp.Normalize();
  /*FIXME: ugly hack pour éviter les collisions directs avec le lanceur du projectile 
      Peut permettre de tirer à travers un mur!*/
  tgVector3 startPosition = vPosition+tmp*100;
  tgCollisionResult cr;

  r->GetPhysicNode()->SetAllPosition(startPosition);
  r->GetPhysicNode()->SetVelocity(tmp*100);
  r->GetPhysicNode()->SetRotation(qRot*tgQuaternion(90,tgVector3(0,1,0)));
  rockets.insert(r);
  pGame->GetPhysicEngine()->AddNode(r->GetPhysicNode());

  tgset<tgIEntity*> ignoreList;
  ignoreList.insert(pPlayer->GetModel());
  cr = pGame->GetWorldManager()->Trace(vPosition, startPosition, r->GetPhysicNode()->GetBVolume(), r->GetPhysicNode()->GetEntity(), &ignoreList); 

  if (cr.fFraction < 1.0) {
    if (cr.pEntity != NULL)
      pGame->GetPhysicEngine()->HandleTouch(r->GetPhysicNode(), cr);
    else
      r->GetPhysicNode()->HitWall(cr.vColNormal, cr.vEndPoint);
  }
}

void tgRocketLauncher::DeleteMe (tgRocket* r)
{
  rocketsToDelete.insert(r);
}

void tgRocketLauncher::Think ()
{
  for (tgset<tgRocket*>::iterator i=rocketsToDelete.begin(); i!=rocketsToDelete.end(); i++) {
    pGame->GetPhysicEngine()->RemoveNode((*i)->GetPhysicNode());
    rockets.erase(*i);
    delete *i;
  }
  rocketsToDelete.clear();
}

