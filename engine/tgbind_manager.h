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
#ifndef TGBINDMANAGER_H
#define TGBINDMANAGER_H

#include "tgdefs.h"
#include "tgibind_manager.h"

#define MOUSE_BUTTONS 6
/**
* @see tgIBindManager
*/
class tgBindManager : public tgIBindManager
{
 public:
	tgBindManager ();
	~tgBindManager();
 
	/**
	* @see tgIBindManager::Bind
	*/
	void Bind(tgKey key, const tgFunctor1<int>& func);

	/**
	 * @see tgIBindManager::BindMouseClick
	 */
	void BindMouseClick(int button, const tgFunctor1<int>& func);
	
	/**
	* @see tgIBindManager::Call
	*/
	void Call(tgKey key, int state);
	
	/**
	 * @see tgIBindManager::CallMouseClick
	 */
	void CallMouseClick(int button, int state);
	
	/**
	* @see tgIBindManager::SetUniqueCallback
	*/
	void SetUniqueCallback (const tgFunctor2<tgKey, int>& func);
	
	/**
	* @see tgIBindManager::SwitchToUniqueCallback
	*/
	void SwitchToUniqueCallback ();
	
	/**
	* @see tgIBindManager::SwitchToMultipleCallback
	*/
	void SwitchToMultipleCallback ();
	
	/**
	* @see tgIBindManager::BindMouseMotion
	*/
	void BindMouseMotion(const tgFunctor2<int,int>& func);
	
	/**
	* @see tgIBindManager::CallMouseMotion
	*/
	void CallMouseMotion(int rx, int ry);
 protected:
	tgFunctor1<int> pFunctors[TGKEY_LAST];
	tgFunctor1<int> pMouseButtonFunctors[MOUSE_BUTTONS];
	tgFunctor2<int,int> funcMouseMotion;
	tgFunctor2<tgKey, int> funcUniqueCallback;
	bool bUniqueCallback;
};

#endif 
