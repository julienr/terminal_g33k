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
#ifndef TGCONSOLE_H
#define TGCONSOLE_H

#include "tgiconsole.h"
#include "tgcanvas.h"
#include "tgdefs.h"

class tgConsole : public tgIConsole
{
 public:
	tgConsole ();
	virtual ~tgConsole();

	void Print (char c);
	void Print (tgstring s);
	void Print (int i);
	void Print (float f);
	void NextLine ();
	void BackSpace ();
	void Toggle();
	
	void Render ();
	
	inline bool IsActive() { return bShow; }
	
	/**
	* @see tgIConsole::Register
	*/
	void Register (tgCVar* var, const tgstring& name);
	//0 argument
	void Register (const tgFunctor0& func, const tgstring& name);
	//1 argument
	void Register (const tgFunctor1<int>& func, const tgstring& name);
	void Register (const tgFunctor1<float>& func, const tgstring& name);
	void Register (const tgFunctor1<tgstring>& func, const tgstring& name);
	//3 arguments
	void Register (const tgFunctor3<float,float,float>& func, const tgstring& name);

	//vector argument
	void Register (const tgFunctor1<const tgVector3&>& func, const tgstring& name);
	
	/**
	* @see tgIConsole::Get
	*/
	tgCVar* Get (const tgstring& name) ;
	
	
	void CPrint_Test();
	void CPrint_Test_int(int i);
	void CPrint_Test_float(float f);
	void CPrint_Test_string(tgstring s);
	void CPrint_Test_floatfloatfloat(float,float,float);
	void CPrint_RegisteredFunctions ();
	void CPrint_RegisteredVars ();
 protected:
	/**
	* Renvoie true si la fonction correspondante est trouvée, false sinon
	*/
	bool _ExecuteLine(const tgstring& line);
	
	/**
	* Rempli un vecteur avec les arguments
	*/
	void _GetArgs(tgvector<tgstring>* args, const tgstring& line);
	
	/**
	* Sépare les arguments et assigne la valeur a la variable en question
	*/
	bool _ParseValue(const tgstring& command);
	
	/**
	* Sépare les arguments et appel la fonction en question
	*/
	bool _ParseCommand(const tgstring& command);
	
	tgvector<tgstring> vHistory;
	tgstring sCurrentLine;
	tgstring sBuff;
	
	unsigned int iMaxLines;
	bool bShow;
	
	


	
	tgmap<tgstring, tgCVar*> mVars;
	//0 argument
	tgmap< tgstring, tgFunctor0> Func;
	//1 argument
	tgmap<tgstring, tgFunctor1<int> > intFunc;
	tgmap<tgstring, tgFunctor1<float> > floatFunc;
	tgmap<tgstring, tgFunctor1<tgstring> > strFunc;
	//3 argument -> vector
	tgmap<tgstring, tgFunctor3<float,float,float> > floatfloatfloatFunc;

	//vector argument
	tgmap<tgstring, tgFunctor1<const tgVector3&> > vectorFunc;
};
#endif
