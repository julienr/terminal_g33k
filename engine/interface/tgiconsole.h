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
#ifndef TGICONSOLE_H
#define TGICONSOLE_H

#include "tgfunctor.h"
#include "tglib.h"
#include <sstream>

/**
* Classe implémentant une variable de la console
* @see tgIConsole
*/

class tgCVar
{
 public:
	inline tgCVar() { intVal = 0; _UpdateInt(); }
	inline tgCVar(const tgCVar& var) { intVal = var.GetInt(); floatVal = var.GetFloat(); strVal = var.GetStr(); }
	inline tgCVar(int var) { intVal = var; _UpdateInt (); }
	inline tgCVar(float var) { floatVal = var; _UpdateFloat (); }
	inline tgCVar(const tgstring& var) { strVal = var; _UpdateStr (); }
	
	inline void Set(int var) { intVal = var; _UpdateInt (); }
	inline void Set(float var) { floatVal = var; _UpdateFloat (); }
	inline void Set(const tgstring& var) { strVal = var; _UpdateStr (); }
	
	inline int GetInt () const { return intVal; }
	inline float GetFloat() const { return floatVal; }
	inline tgstring GetStr() const { return strVal; }
	
 protected:
	int intVal;
	float floatVal;
	tgstring strVal;
	
	//update en prenant intVal comme valeur
	inline void _UpdateInt () {
		std::ostringstream s;
		floatVal = (float)intVal;
		s << intVal;
		strVal = s.str();
	}
	//update en prenant floatVal comme valeur
	inline void _UpdateFloat () {
		std::ostringstream s;
		intVal = (int)floatVal;
		s << floatVal;
		strVal = s.str();
	}
	//update en prenant strVal comme valeur
	inline void _UpdateStr() {
		intVal = atoi(strVal.data());
		floatVal = (float)atof(strVal.data());
	}
};

/**
* Classe implémentant une console.
* Elle gère l'enregistrement des variables/fonctions
*/
class tgIConsole
{
 public:
 	virtual ~tgIConsole() {}
 	virtual void Print (char c) = 0;
	virtual void Print (tgstring s) = 0;
	virtual void Print (int i) = 0;
	virtual void Print (float f) = 0;
	virtual void NextLine () = 0;
	virtual void BackSpace () = 0;
	virtual void Toggle() = 0;
	virtual void Render () = 0;
 
	virtual bool IsActive() = 0;
 
	/**
	* Enregistrement d'une variable/fonction auprès de la console
	*/
	virtual void Register (tgCVar* var, const tgstring& name) = 0;
	//0 argument
	virtual void Register (const tgFunctor0& func, const tgstring& name) = 0;
	//1 argument
	virtual void Register (const tgFunctor1<int>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<float>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<tgstring>& func, const tgstring& name) = 0;

	
	//2 arguments
	/*virtual void Register (const tgFunctor1<int,int>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<float, float>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<tgstring, tgstring>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<int,float>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<float, int>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<int, tgstring>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<tgstring,int>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<float, tgstring>& func, const tgstring& name) = 0;
	virtual void Register (const tgFunctor1<tgstring, float>& func, const tgstring& name) = 0;*/
	
	//3 arguments
	virtual void Register (const tgFunctor3<float,float,float>& func, const tgstring& name) = 0;

	//1 argument vector
	virtual void Register (const tgFunctor1<const tgVector3&>& func, const tgstring& name) = 0;
	
	/**
	* Récupération d'une variable/fonction préalablement enregistrée
	*/
	virtual tgCVar* Get (const tgstring& name) =0;
};

#endif
