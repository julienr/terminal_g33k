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
#ifndef TGGL_H
#define TGGL_H

//on veut inclure notre glext, sinon GL/gl.h inclu celui du système
#define GL_GLEXT_LEGACY 
//on veut que glext nous donne les prototypes de fonctions
#define GL_GLEXT_PROTOTYPES

#ifdef TGOSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#ifdef TGLINUX
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "gl.h"
#include "glext.h"
#include "glu.h"
#endif
#ifdef TGWIN32
#include <windows.h>
#include "gl\gl.h"
#include "gl\glext.h"
#include "gl\glu.h"
#include <SDL.h>
#endif



//#include "glext.h"
#include "tggl_linked.h"

#endif
 
