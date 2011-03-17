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

#include "tghuman_player_controller.h"


tgHumanPlayerController::tgHumanPlayerController(tgPlayer*p) : tgPlayerController(p)
{
	bMoveDirs[0] = bMoveDirs[1] = bMoveDirs[2] = bMoveDirs[3] = false;

	SetNoClip(false);

  p->SetIsLocalPlayer(true);
  p->SwitchWeapon(0);
  moveDir.Set(0,0,0);
}

tgHumanPlayerController::~tgHumanPlayerController()
{

}

void tgHumanPlayerController::SetNoClip(int b)
{
	bNoClip = (bool)b;
	pPlayer->GetPhysicNode()->SetNoClip((bool)b);
}
#undef TEST_VEL
#ifdef TEST_VEL
#define NVEL 1.0
void HandleMoveAxis (float* vel, bool forward, bool backward)
{
  *vel -= (*vel)/2;

  if (forward)
    *vel += NVEL;
  if (backward)
    *vel -= NVEL;

  if (*vel < -NVEL)
    *vel = -NVEL;
  if (*vel > NVEL)
    *vel = NVEL;
}
#endif
void tgHumanPlayerController::Control()
{
  #ifdef TEST_VEL
  HandleMoveAxis(&moveDir.z, bMoveDirs[BACKWARD], bMoveDirs[FORWARD]);
  HandleMoveAxis(&moveDir.x, bMoveDirs[STRAFERIGHT], bMoveDirs[STRAFELEFT]);

  tgVector3 tmp(0,0,0);
  tmp += pPlayer->GetPhysicNode()->GetRotation().GetZAxis()*moveDir.z;
  tmp += pPlayer->GetPhysicNode()->GetRotation().GetXAxis()*moveDir.x;

	if(bNoClip || pPlayer->GetPhysicNode()->GetPhysicType() == PHYS_PROJ) {
		tmp *= SPEED;
		movementForce.SetForce(tmp);
	} else {
		//permet d'annuler la force sur les y (pas de deplacement verticaux, sauf saut) tout en gardant les proportions sur les autres axes
		tmp.y = 0;
		tmp.Normalize();
		tmp *= SPEED;
		movementForce.SetForce(tmp);
	}
  #else
	tgVector3 tmp(0,0,0);
	
	if(bMoveDirs[FORWARD])
		tmp -= pPlayer->GetPhysicNode()->GetRotation().GetZAxis();
	if(bMoveDirs[BACKWARD])
		tmp += pPlayer->GetPhysicNode()->GetRotation().GetZAxis();
	if(bMoveDirs[STRAFELEFT])
		tmp -= pPlayer->GetPhysicNode()->GetRotation().GetXAxis();
	if(bMoveDirs[STRAFERIGHT])
		tmp += pPlayer->GetPhysicNode()->GetRotation().GetXAxis();
	if(bNoClip || pPlayer->GetPhysicNode()->GetPhysicType() == PHYS_PROJ) {
		tmp *= SPEED;
		movementForce.SetForce(tmp);
	} else {
		//permet d'annuler la force sur les y (pas de deplacement verticaux, sauf saut) tout en gardant les proportions sur les autres axes
		tmp.y = 0;
		tmp.Normalize();
		tmp *= SPEED;
		movementForce.SetForce(tmp);
	}
  #endif
}

void tgHumanPlayerController::MouseMotion(int rx, int ry)
{
	static float totX = 0.0f;
	static float totY = 0.0f;
	
	float fSensivity = 0.2f;

	totX -= ((float)ry)*fSensivity;
	totY -= ((float)rx)*fSensivity;

	tgQuaternion qY = tgQuaternion(totY, tgVector3(0,1,0));
	tgQuaternion qX = tgQuaternion(totX, tgVector3(1,0,0));
	tgQuaternion q = qY*qX;
	q.Normalize();
	pPlayer->GetPhysicNode()->SetRotation(q);	

}

void tgHumanPlayerController::MoveForward(int state)
{
	if (state)
		bMoveDirs[FORWARD] = true;
	else
		bMoveDirs[FORWARD] = false;

}

void tgHumanPlayerController::MoveBackward(int state)
{
	if (state)	
		bMoveDirs[BACKWARD] = true;
	else
		bMoveDirs[BACKWARD] = false;

}

void tgHumanPlayerController::StrafeLeft(int state)
{
	if (state)
		bMoveDirs[STRAFELEFT] = true;
	else
		bMoveDirs[STRAFELEFT] = false;

}

void tgHumanPlayerController::StrafeRight(int state)
{
	if (state)
		bMoveDirs[STRAFERIGHT] = true;
	else
		bMoveDirs[STRAFERIGHT] = false;

}

void tgHumanPlayerController::Jump(int state)
{
  //don't jump if we are in air
  if (pPlayer->GetPhysicNode()->GetPositionType() == POS_AIR)
    return;

  tgForce *force = new tgForce(tgVector3(0,2*SPEED,0), false, 0.3f);
  pPlayer->GetPhysicNode()->AddForce(force);
  pPlayer->GetPhysicNode()->SetPositionType(POS_AIR);
}

void tgHumanPlayerController::Fire (int state)
{
  if (!state) 
    return;
  pPlayer->Fire();
}

void tgHumanPlayerController::NextWeapon(int state)
{
  if (!state)
    return;
  pPlayer->SwitchWeapon(-1);
}

void tgHumanPlayerController::PrevWeapon(int state)
{
  if (!state)
    return;
  pPlayer->SwitchWeapon(-2);
}
