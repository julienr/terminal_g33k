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

#ifndef TGHUMAN_PLAYER_CONTROLLER_H
#define TGHUMAN_PLAYER_CONTROLLER_H

#include "tgplayer_controller.h"

enum eMoveDirs {
  FORWARD=0,
  BACKWARD,
  STRAFELEFT,
  STRAFERIGHT,
  NUM_MOVE_DIRS
};

class tgHumanPlayerController: public tgPlayerController
{
	public:
		tgHumanPlayerController(tgPlayer*);
		~tgHumanPlayerController();

		void Control();

		void MouseMotion(int rx, int ry);
		void MoveForward(int state);
		void MoveBackward(int state);
		void StrafeLeft(int state);
		void StrafeRight(int state);
    void Jump(int state);
    void Fire (int state);

    void NextWeapon(int state);
		void PrevWeapon(int state);

		void SetNoClip(int b);
		
	protected:
		bool bNoClip;
		bool bMoveDirs[NUM_MOVE_DIRS];
    tgVector3 moveDir;
};

#endif

