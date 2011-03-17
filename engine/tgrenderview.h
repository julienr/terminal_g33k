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

#include "tgirenderview.h"
#include "tgdefs.h"

#ifndef TGRENDERVIEW_H
#define TGRENDERVIEW_H

class tgRenderView: public tgIRenderView
{
 public:
	tgRenderView();
	~tgRenderView();
	void Update();
	
	void SetEntity(tgIEntity* ent) { pEntity = ent; }
	
	void SetRelPosition (const tgVector3& pos) { vRelPosition = pos;}
	const tgVector3& GetRelPosition() const { return vRelPosition; }

	//we need to give a non-constant reference in order to allow glrenderer to update it
	tgFrustum& GetFrustum() { return frustum; }
	
	float* GetMatrix() { return matrix; }
	void SetMatrix(float m[16]) { for(short i=0; i<16; i++) matrix[i] = m[i];}

	void SetRelRotation (const tgQuaternion& rot) { qRelRotation = rot;}
	const tgQuaternion& GetRelRotation() const { return qRelRotation; }
	
	const tgVector3& GetFinalPosition () const { return vFinalPosition; }
	const tgQuaternion& GetFinalRotation () const { return qFinalRotation; }
 protected:
	tgIEntity* pEntity;
	tgFrustum frustum;
	float matrix[16];
	tgVector3 vRelPosition;
	tgQuaternion qRelRotation;
	tgVector3 vFinalPosition;
	tgQuaternion qFinalRotation;
};


#endif

