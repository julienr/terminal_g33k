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
#ifndef TGMATRIX_H
#define TGMATRIX_H

#include "tgmath.h"
/**
 * ligne-colonne -> m[0][1] = ligne 0, colonne 1
 */
class tgMatrix3 
{
 public:
	tgMatrix3 () {
		m[0][0] = 1;
		m[0][1] = 0;
		m[0][2] = 0;
		m[1][0] = 0;
		m[1][1] = 1;
		m[1][2] = 0;
		m[2][0] = 0;
		m[2][1] = 0;
		m[2][2] = 1;
	}
	tgMatrix3 (const float arr[3][3]) { memcpy(m,arr,9*sizeof(float)); }
	tgMatrix3 (float e00,float e01,float e02, float e10,float e11,float e12,float e20, float e21,float e22) {
		m[0][0] = e00;
		m[0][1] = e01;
		m[0][2] = e02;
		m[1][0] = e10;
		m[1][1] = e11;
		m[1][2] = e12;
		m[2][0] = e20;
		m[2][1] = e21;
		m[2][2] = e22;
	}

	/**
	 * permet la syntax Matrix[row][col]
	 */
	float* operator[] (size_t row) const {
		return (float*)m[row];
	}

	tgVector3 operator* (const tgVector3& in) const;
 protected:
	float m[3][3];
};

tgVector3 tgMatrix3::operator* (const tgVector3& in) const
{
	tgVector3 prod;
	for(unsigned short r=0;r<3;r++) {
		prod[r] = m[r][0]*in[0] +
			  m[r][1]*in[1] +
			  m[r][2]*in[2];
	}
	return prod;
}

#endif

