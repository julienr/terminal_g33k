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
#ifndef TGLIB_H
#define TGLIB_H

/// Unsigned 1-byte integer.
typedef unsigned char uint8;
/// Signed 1-byte integer.
typedef signed char sint8;
/// Unsigned 2-byte integer.
typedef unsigned short uint16;
/// Signed 2-byte integer.
typedef signed short sint16;
/// Unsigned 4-byte integer.
typedef unsigned int uint32;
/// Signed 4-byte integer.
typedef signed int sint32;

//#include "stdlib.h"


#include "../tglib/math/tgvector3.h"
#include "../tglib/math/tgvector2.h"
#include "../tglib/math/tggeom2d.h"
#include "../tglib/math/tgmath.h" 
#include "../tglib/math/tgquaternion.h"
#include "../tglib/math/tgfrustum.h"
#include "../tglib/math/tgtriangle.h"
#include "../tglib/tgplane.h"
#include "../tglib/tgface.h"
#include "../tglib/tgvertice.h"
#include "../tglib/bvolume/tgaaboundingbox.h"
#include "../tglib/bvolume/tgboundingsphere.h"
#include "../tglib/bvolume/tgboundingvolume.h"
#include "../tglib/containers/tglinklist.h"

#include <cstring>
#include <stdlib.h>

#include <string>
#define tgstring std::string
#include <map>
#define tgmap std::map
#include <vector>
#define tgvector std::vector
#include <list>
#define tglist std::list
/*#include "../tglib/stl/stack.h"
#define tgstack tgStack*/
#include <deque>
#define tgdeque std::deque
#include <queue>
#define tgqueue std::queue
#include <stack>
#define tgstack std::stack
#include <set>
#define tgset std::set


#endif
