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

#include "tgbind_manager.h"

tgBindManager::tgBindManager ()
{
	bUniqueCallback = false;
}

tgBindManager::~tgBindManager()
{

}

void tgBindManager::Bind (tgKey key, const tgFunctor1<int>& func) 
{
	if ((key>=0) && (key<TGKEY_LAST))
		pFunctors[key] = func;
	else
		cerr << "[TGE]BindManager::Bind - Hors Limite" << endl;
}

void tgBindManager::BindMouseClick(int button, const tgFunctor1<int>& func)
{
	if((button>=0) && (button<MOUSE_BUTTONS))
		pMouseButtonFunctors[button] = func;
	else
		cerr << "[TGE]BindManager::BindMouseClick - Hors Limite" << endl;
}

void tgBindManager::Call (tgKey key, int state)
{
	if (bUniqueCallback) {
		funcUniqueCallback(key, state);
		return;
	}
	else {
		if ((key>=0) && (key<TGKEY_LAST)) {
			if (pFunctors[key]) {
				pFunctors[key](state);
			}
		} else
			cerr << "[TGE]BindManager::Call - Hors Limite" << endl;
	}
}

void tgBindManager::CallMouseClick (int button, int state)
{
	if ((button>=0) && (button<MOUSE_BUTTONS)) {
		if (pMouseButtonFunctors[button]) {
			pMouseButtonFunctors[button](state);
		}
	} else {
		cerr << "[TGE]BindManager::CallMouseClick - Hors Limite" << endl;
	}
}

void tgBindManager::SetUniqueCallback (const tgFunctor2<tgKey, int>& func)
{
	funcUniqueCallback = func;
}

void tgBindManager::SwitchToUniqueCallback ()
{
	bUniqueCallback = true;
}

void tgBindManager::SwitchToMultipleCallback ()
{
	bUniqueCallback = false;
}

void tgBindManager::BindMouseMotion(const tgFunctor2<int,int>& func)
{
	funcMouseMotion = func;
}

void tgBindManager::CallMouseMotion(int rx, int ry)
{
	if(funcMouseMotion)
		funcMouseMotion(rx,ry);
}

