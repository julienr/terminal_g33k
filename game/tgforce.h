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


#ifndef TGFORCE_H
#define TGFORCE_H

#include "tgdefs.h"

/**
 * Represente une force physique
 * - quand 'Duration' arrive a 0, la force n'est plus appliquee, mais pas supprimee automatiquement !
 * - si 'Duration' == -1.0f, la force est applique indefiniment
 */

class tgForce
{
	public:
		tgForce() { vForce.Set(0,0,0); bInfinite = true; fDuration = 0.0f; }
    tgForce(const tgVector3& vec, bool inf=true, float dur=0.0f) { vForce = vec; bInfinite = inf; fDuration = dur;}

		const tgVector3& GetForce() { return vForce; }
		void SetForce(const tgVector3& f) { vForce = f; }

		float GetDuration() { return fDuration; }
		void SetDuration(float f) { bInfinite = false; fDuration = f; }

		bool IsInfinite() { return bInfinite; }
		void SetInfinite(bool b) { bInfinite = b; }

	protected:
		tgVector3 vForce;
		float fDuration;
		bool bInfinite;	
};

#endif

