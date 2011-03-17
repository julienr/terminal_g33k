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
#ifndef TGITIMER_H
#define TGITIMER_H

/**
* Classe g�rant un timer, permet la r�cup�ration d'informations ayant rapport au temps (FPS, ...)
*/
class tgITimer 
{
 public:
 	virtual ~tgITimer() {}
	virtual float GetFPS () = 0;
	virtual float GetFrameInterval () = 0;
	//virtual float GetTime () = 0;
	virtual void CalculateFPS () = 0;
	virtual void Initialize () = 0;
};

#endif
