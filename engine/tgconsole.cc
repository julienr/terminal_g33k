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
#include "tgconsole.h"

tgConsole::tgConsole ()
{
	iMaxLines = 10;
	//bShow = false;
	
	bShow = false;
	/*static tgCVar var1(10);
	this->Register(&var1, "var1");*/
	this->Register(tgMakeFunctor((tgFunctor0*)0, *this, &tgConsole::CPrint_Test)  , "test");
	this->Register(tgMakeFunctor((tgFunctor1<int>*)0, *this, &tgConsole::CPrint_Test_int)  , "testint");
	this->Register(tgMakeFunctor((tgFunctor1<float>*)0, *this, &tgConsole::CPrint_Test_float)  , "testfloat");
	this->Register(tgMakeFunctor((tgFunctor1<tgstring>*)0, *this, &tgConsole::CPrint_Test_string)  , "teststr");
	this->Register(tgMakeFunctor((tgFunctor3<float,float,float>*)0, *this, &tgConsole::CPrint_Test_floatfloatfloat)  , "testfloatfloatfloat");
}

tgConsole::~tgConsole ()
{

}

void tgConsole::Print (char c)
{
	sCurrentLine += c;
}

void tgConsole::Print (tgstring s)
{
	sCurrentLine += s;
}

void tgConsole::Print (int i)
{
	std::ostringstream stream;
	stream << i;
	sCurrentLine += stream.str();
}

void tgConsole::Print (float f)
{
	std::ostringstream stream;
	stream << f;
	sCurrentLine += stream.str();
}

void tgConsole::CPrint_Test ()
{
	this->Print('p');
	this->Print(29);
	this->Print((float)2.94);
	this->Print("bonjour");
}

void tgConsole::CPrint_Test_int(int i)
{
	this->Print(i);
}

void tgConsole::CPrint_Test_float(float f)
{
	this->Print(f);
}

void tgConsole::CPrint_Test_string(tgstring s)
{
	this->Print(s);
}

void tgConsole::CPrint_Test_floatfloatfloat(float f1 ,float f2 ,float f3)
{
	this->Print(f1);
	this->Print(f2);
	this->Print(f3);
}

void tgConsole::CPrint_RegisteredFunctions ()
{

}

void tgConsole::CPrint_RegisteredVars ()
{

}

void tgConsole::NextLine ()
{
	tgstring temp = sCurrentLine;
	if ((sCurrentLine.size() > 0) && (sCurrentLine.find_first_not_of(" \t\n") != tgstring::npos)) {
		vHistory.push_back(sCurrentLine);
		sCurrentLine.clear();
		_ExecuteLine(temp);
		
	}
}

bool tgConsole::_ExecuteLine(const tgstring& line)
{
	size_t index = line.find_first_not_of (" ");
	
	if (index != tgstring::npos && line[index] == '-') {//si le premier non-espace est un tiret,on a une commande
		return _ParseCommand(line.substr(index+1));
	} else if (index != tgstring::npos && line[index] == '.') { //si le premier non-espace est un point, c'est une assignation de valeur
		return _ParseValue(line.substr(index+1));
	} else {
		return false;
	}
}

bool tgConsole::_ParseValue(const tgstring& command)
{
	tgstring firstWord;
	tgstring args("");
	
	//on cherche le premier mot
	size_t debut = command.find_first_not_of(" \t\n");
	size_t pos = command.find_first_of(" \t\n", debut);
	
	if(pos != tgstring::npos) {//on sépare le premier mot (commande) de la fin de la chaîne (arguments) 
		firstWord = command.substr(debut, pos);
		args = command.substr(pos+1);//pos+1 sinon on prends l'espace devant le nom
	} else {//on a juste un nom de commande, sans arguments
		firstWord = command.substr(debut);
	}
	
	//on cherche l'argument
	if(args.find_first_not_of(" \t\n") == tgstring::npos) { // on a pas de valeur
		this->Print("valeur necessaire");
		return false;
	} else { //assignation
		tgmap< tgstring, tgCVar*>::iterator i = mVars.begin();
		for(; i != mVars.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				i->second->Set(args);
				return true;
			}
		}
		this->Print("variable non trouvee");
		return false;
	}
	
	
	
}

bool tgConsole::_ParseCommand(const tgstring& command)
{
	tgstring firstWord;
	tgstring args("");
	
	//on cherche le premier mot
	size_t debut = command.find_first_not_of(" \t\n");
	size_t pos = command.find_first_of(" \t\n", debut);
	
	if(pos != tgstring::npos) {//on sépare le premier mot (commande) de la fin de la chaîne (arguments) 
		firstWord = command.substr(debut, pos);
		args = command.substr(pos+1);//pos+1 sinon on prends l'espace devant le nom
	} else {//on a juste un nom de commande, sans arguments
		firstWord = command.substr(debut);
	}
	//separation des arguments
	tgvector<tgstring> vArgs;
	_GetArgs(&vArgs, args);
	/*for(size_t i=0; i<args.size(); i++) {
		out << args[i] << endl;
	}
	
	for(size_t i=0; i<vArgs.size(); i++) {
		this->Print(vArgs[i]);
	}*/
	//Fonction à 0 argument
	{
		tgmap< tgstring, tgFunctor0>::iterator i = Func.begin();
		for(; i != Func.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				i->second ();
				return true;
			}
		}
	}
	//Fonctions à 1 argument
	{ //int
		tgmap< tgstring, tgFunctor1<int> >::iterator i = intFunc.begin();
		for(; i != intFunc.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				if(vArgs.size() < 1) {
					this->Print("need 1 arg");
					return true;
				}
				i->second (atoi(vArgs[0].data()));
				return true;
			}
		}
	}
	{ //float
		tgmap< tgstring, tgFunctor1<float> >::iterator i = floatFunc.begin();
		for(; i != floatFunc.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				if(vArgs.size() < 1) {
					this->Print("need 1 arg");
					return true;
				}
				i->second ((float)atof(vArgs[0].data()));
				return true;
			}
		}
	}
	{ //str
		tgmap< tgstring, tgFunctor1<tgstring> >::iterator i = strFunc.begin();
		for(; i != strFunc.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				if(vArgs.size() < 1) {
					this->Print("need 1 arg");
					return true;
				}
				i->second (vArgs[0]);
				return true;
			}
		}
	}
	//Fonctions à 3 argument
	{ //float float float
		tgmap< tgstring, tgFunctor3<float,float,float> >::iterator i = floatfloatfloatFunc.begin();
		for(; i != floatfloatfloatFunc.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				if(vArgs.size() < 3) {
					this->Print("need 3 args");
					return true;
				}
				i->second ((float)atof(vArgs[0].data()), (float)atof(vArgs[1].data()),(float)atof(vArgs[2].data()));
				return true;
			}
		}
	}
	
	//Fonctions a 1 argument de type vector
	{
		tgmap< tgstring, tgFunctor1<const tgVector3&> >::iterator i = vectorFunc.begin();
		for(; i != vectorFunc.end(); i++) {
			if (firstWord.compare(i->first) == 0) {
				if(vArgs.size() < 3) {
					this->Print("need 3 args");
					return true;
				}
				//i->second (atof(vArgs[0].data()), atof(vArgs[1].data()),atof(vArgs[2].data()));
				i->second(tgVector3((float)atof(vArgs[0].data()),(float)atof(vArgs[1].data()),(float)atof(vArgs[2].data())));
				return true;
			}
		}
	}

	return false;
}

void tgConsole::_GetArgs(tgvector<tgstring>* args, const tgstring& line)
{
	size_t count = 0;
	do {
		size_t debut = line.find_first_not_of(" \t\n", count);
		if(debut == tgstring::npos)
			return;
		size_t fin = line.find_first_of(" \t\n", debut);
		count = fin;
		if(fin == tgstring::npos)
			(*args).push_back(line.substr(debut));
			//out << "debut :" << debut << "\tfin :" << "npos" << "\tvaleur :" << line.substr(debut) << endl;
		else
			(*args).push_back(line.substr(debut,fin-debut));
			//out << "debut :" << debut << "\tfin :" << fin << "\tvaleur :" << line.substr(debut,fin-debut) << endl;
		
	} while(count != tgstring::npos);
}

void tgConsole::BackSpace ()
{
	if (sCurrentLine.size() > 0 )
		sCurrentLine.erase(--sCurrentLine.end(), sCurrentLine.end());
}

void tgConsole::Toggle()
{
	(bShow) ? bShow = false : bShow = true;
}

void tgConsole::Render()
{
	if (!bShow)
		return;
	tgColor tempDraw = pCanvas->GetDrawColor();
	tgColor tempBG = pCanvas->GetBackgroundColor();
	
	pCanvas->SetDrawColor(tgColor(1,1,1));
	pCanvas->SetBackgroundColor(tgColor(0,0.5f,0,0.8f));
	size_t length = vHistory.size();
	//on affiche au maximum iMaxLines
	size_t begin;
	if (length < iMaxLines)
		begin = 0;
	else
		begin = length-(size_t)iMaxLines;
		
	for(size_t i=begin;i<length;i++){
		pCanvas->DisplayText(vHistory[i]);
	}
	
	pCanvas->DisplayText("$" + sCurrentLine);
	
	pCanvas->SetBackgroundColor(tempBG);
	pCanvas->SetDrawColor(tempDraw);
}

void tgConsole::Register (tgCVar* var, const tgstring& name)
{
	mVars[name] = var;
}

void tgConsole::Register (const tgFunctor0& func, const tgstring& name)
{
	Func[name] = func;
}

void tgConsole::Register (const tgFunctor1<int>& func, const tgstring& name)
{
	intFunc[name] = func;
}

void tgConsole::Register (const tgFunctor1<float>& func, const tgstring& name)
{
	floatFunc[name] = func;
}

void tgConsole::Register (const tgFunctor1<tgstring>& func, const tgstring& name)
{
	strFunc[name] = func;
}

void tgConsole::Register (const tgFunctor3<float,float,float>& func, const tgstring& name)
{
	floatfloatfloatFunc[name] = func;
}
void tgConsole::Register (const tgFunctor1<const tgVector3&>& func, const tgstring& name)
{
	vectorFunc[name] = func;
}

tgCVar* tgConsole::Get (const tgstring& name)
{
	tgmap<tgstring, tgCVar*>::iterator i = mVars.find(name);
	if (i==mVars.end()) {
		tgout << "Variable inexistante : " << name << endl;
		return NULL;
	} else {
		return i->second;
	}
}

