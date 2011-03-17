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
#ifndef TGTIMER_H
#define TGTIMER_H

#include "tgitimer.h"
#include "tgdefs.h"
#include "tgsystem.h"

/**
* Implémentation du Timer
*/
class tgTimer : public tgITimer
{
 public:
	tgTimer ();
	inline float GetFPS () { return fFPS; }
	inline float GetFrameInterval () { return fFrameInterval; }
	void CalculateFPS ();
	void Initialize ();
 protected:
	float fFPS;
	float fFPSCounter;
	float fFrameInterval;
	float fFrameTime;
	float fLastFrameTime;
	float fCurrentTime;
};

#endif
