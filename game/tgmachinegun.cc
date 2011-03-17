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
#include "tgmachinegun.h"
#include "tggame.h"

tgMachinegun::tgMachinegun(tgPlayer* p) : tgWeapon(p)
{
  pModel = pGame->GetModelManager()->GetInstance("machinegun_view");
  pModel->SetAnimation("idle");
  pModel->SetVisible(false);

  pModel->SetRotation(tgQuaternion(90, tgVector3(0,1,0)));
	pModel->SetPosition(tgVector3(2,-2,-10));

  pGame->GetWorldManager()->AddModel(pModel);
}

tgMachinegun::~tgMachinegun()
{
  pGame->GetWorldManager()->RemoveModel(pModel);
  delete pModel;
}

void tgMachinegun::UnSelect ()
{
  pModel->SetVisible(false);
}

void tgMachinegun::Select ()
{
  if (pPlayer->IsLocalPlayer())
    pModel->SetViewModel(true);
  else  {
    //TODO: attach weapon to player
    pModel->SetPosition(pPlayer->GetPhysicNode()->GetPosition());
    pModel->SetViewModel(false);
  }
  pModel->SetVisible(true);
}

void tgMachinegun::Fire(const tgVector3& vPosition, const tgVector3& vDirection, const tgQuaternion& qRot)
{
  tgCollisionResult col;
  col = pGame->GetWorldManager()->Trace(vPosition, vPosition+vDirection*50000,
                                        NULL,  pPlayer->GetPhysicNode()->GetEntity());
  if (col.fFraction >= 1.0f) 
    return;
 
  tgBoundingSphere temp(col.vEndPoint,25);
  pGame->GetWorldManager()->AddDecal(&temp, col.vColNormal, "textures/decal"); 

  if (col.pEntity) {
    tgPhysicNode* tmpNode = pGame->GetPhysicEngine()->EntityToNode(col.pEntity);
    if (tmpNode && tmpNode->GetObjectType() == OBJ_PLAYER) {
      tgPlayer* player = pGame->PhysicNodeToPlayer(tmpNode);
      if (player) {
        player->TakeDamage(MACHINEGUN_DAMAGE, col.vColNormal, col.vEndPoint);
      }
    }
  }
}

void tgMachinegun::Think ()
{

}
