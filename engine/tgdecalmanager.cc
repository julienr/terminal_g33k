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
#include "tgdecalmanager.h"

tgDecalManager::tgDecalManager(unsigned int maxDecals)
{
	cvMaxDecals.Set((int)maxDecals);
	pConsole->Register(&cvMaxDecals,"maxdecals");
}

tgDecalManager::~tgDecalManager()
{
	for(unsigned i=0; i<decalsList.size(); i++) {
		delete decalsList[i];
	}
}

/*void tgDecalManager::SetMaxDecals(unsigned int max)
{
	iMaxDecals = max;
}*/

unsigned int tgDecalManager::GetNumDecals() const
{
	return (unsigned int)decalsList.size();
}

tgDecal* tgDecalManager::GetDecal(unsigned int i) const
{
	return decalsList[i];
}


tgDecal* tgDecalManager::NewDecal(tgMaterial* mat, const tgVector3& norm)
{
	if (decalsList.size() < (unsigned int)cvMaxDecals.GetInt()) { //on peut en creer un nouveau
		tgDecal* d = new tgDecal(mat,norm);
		decalsList.push_back(d);
		return d;
	} else { //on doit en remplacer un
		tgDecal* d;
		while(decalsList.size() > (unsigned int)cvMaxDecals.GetInt()) {//suppression du surplus
			d = decalsList[0];
			decalsList.pop_front();
			delete d;
		}
		/*tgDecal* d = decalsList[0];
		decalsList.pop_front();
		delete d;*/
		d = new tgDecal(mat,norm);
		decalsList.push_back(d);
		return d;
	}
	
}

/*void tgDecalManager::Think()
{
	
}*/
