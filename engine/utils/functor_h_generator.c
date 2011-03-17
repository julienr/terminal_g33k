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
#include <stdlib.h>
#include <stdio.h>


char* functor_base_def = "/*\n*\n \
* Classe de base de tout les functors\n \
*/ \n\
class tgFunctorBase\n \
{ \n\
	public:\n \
		typedef void (tgFunctorBase::*_MemFunc) (); \n\
		typedef void (*_Func)(); \n\
		inline tgFunctorBase ():pCallee(0),pFunc(0) {} \n\
		inline tgFunctorBase (const void* c, const void* f, size_t sz) \n\
		{ \n\
			if (c) { /*on a c=callee, f = fonction membre*/ \n\
				pCallee = (void*)c; \n\
				memcpy(pMemFunc, f, sz); \n\
			} else { /*un simple pointeur sur fonction statique*/ \n\
				pFunc = f; \n\
			} \n\
		}\n \
\n \
		inline operator bool() const { return pFunc||pCallee; } \n\
\n \
		class DummyInit {\n \
		}; \n\
\n \
		void* pCallee; \n\
		union { \n\
			const void* pFunc; \n\
			char pMemFunc[sizeof(_MemFunc)]; \n\
		}; \n\
};" ;


#define PR_P for(i=1; i<num_args+1; i++) { \
		if (i>1) \
			fprintf(out, ",P%i", i); \
		else \
			fprintf(out, "P%i", i); \
	}
	
#define PR_TP for(i=1; i<num_args+1; i++) { \
	if (i>1) \
		fprintf(out, ",TP%i", i); \
	else \
		fprintf(out, "TP%i", i); \
}
	
#define PR_CLASS_P for(i=1; i<num_args+1; i++) { \
		if (i>1) \
			fprintf(out, ", class P%i", i); \
		else \
			fprintf(out, "class P%i", i); \
	} 
	
#define PR_CLASS_TP for(i=1; i<num_args+1; i++) { \
		if (i>1) \
			fprintf(out, ", class TP%i", i); \
		else \
			fprintf(out, "class TP%i", i); \
	} 
	
#define PR_P_p for(i=1; i<num_args+1; i++) { \
		if (i>1) \
			fprintf(out, ",P%i p%i", i, i); \
		else \
			fprintf(out, "P%i p%i", i, i); \
	} \
	
	
	
#define PR_p for (i=1; i<num_args+1; i++) {\
		if (i>1) \
			fprintf(out, ", p%i", i); \
		else \
			fprintf(out, "p%i", i); \
	}
	
FILE* out;

void print_zero_args ()
{
	fprintf(out, "\n\n\n");
	fprintf(out, "/****************************************Functor sans arguments, sans retour****************************************/");
	fprintf(out, "\n\n\n");
	fprintf(out, "/** \n\
* Classe gérant les functor\n\
*/\n\
class tgFunctor0: protected tgFunctorBase\n\
{\n\
	public:\n\
		tgFunctor0(DummyInit* = 0) :tgFunctorBase() {}\n\
		void operator ()(void) const\n\
		{\n\
			thunk(*this);\n\
		}\n\
		tgFunctorBase::operator bool;\n\
	protected:\n\
		typedef void (*Thunk) (const tgFunctorBase&);\n\
		tgFunctor0(Thunk t, const void* c, const void* f, size_t sz):\n\ tgFunctorBase(c,f,sz),thunk(t) {}\n\
	private:\n\
		Thunk thunk;\n\
};\n\
\n\
/**\n\
 * Traducteur de fonction statiques\n\
 */\n\
template <class Func>\n\
class tgFunctionTranslator0:public tgFunctor0\n\
{\n\
	public:\n\
		tgFunctionTranslator0(Func f):tgFunctor0(thunk,0,(const void*)f,0) {}\n\
		static void thunk(const tgFunctorBase& ftor)\n\
		{\n\
			(Func(ftor.pFunc))();\n\
		}\n\
};\n\
\n\
/**\n\
 * Traducteur de fonctions-membres\n\
 */\n\
template <class Callee, class MemFunc>\n\
class tgMemberTranslator0: public tgFunctor0\n\
{\n\
	public:\n\
		tgMemberTranslator0(Callee& c, const MemFunc& m): tgFunctor0(thunk, &c, &m, sizeof(MemFunc)) {}\n\
		static void thunk (const tgFunctorBase& ftor)\n\
		{\n\
			(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))();\n\
		}\n\
};\n\
\n\
/**\n\
* Fonction s'occupant de créer un functor\n\
* utilisation : \n\
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a\n\
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test\n\
*/\n\
\n\
template <class TRT>\n\
inline tgFunctionTranslator0<TRT (*) (void)> \n\
tgMakeFunctor(tgFunctor0*, TRT (*f)(void))\n\
{\n\
	return tgFunctionTranslator0<TRT (*)(void)>(f);\n\
}\n\
\n\
template <class Callee,class TRT,class CallType>\n\
inline tgMemberTranslator0<Callee,TRT (CallType::*)(void)>\n\
 tgMakeFunctor(tgFunctor0*,Callee &c,TRT (CallType::* const &f)(void))\n\
{\n\
	typedef TRT (CallType::*MemFunc)(void);\n\
	return tgMemberTranslator0<Callee,MemFunc>(c,f);\n\
}\n\
\n\
template <class Callee,class TRT,class CallType>\n\
inline tgMemberTranslator0<const Callee,TRT (CallType::*)(void)const> \n\
tgMakeFunctor(tgFunctor0*,const Callee &c,TRT (CallType::* const &f)(void)const)\n\
{\n\
	typedef TRT (CallType::*MemFunc)(void)const;\n\
	return tgMemberTranslator0<const Callee,MemFunc>(c,f);\n\
}");
fprintf(out, "\n\n\n");
fprintf(out, "/****************************************Functor sansarguments, avec retour****************************************/");
fprintf(out, "\n\n\n");
fprintf(out, "/** \n\
* Classe gérant les functor\n\
*/\n\
template <class RT>\n \
class tgFunctor0wRet: protected tgFunctorBase\n\
{\n\
	public:\n\
		tgFunctor0wRet(DummyInit* = 0) :tgFunctorBase(){}\n\
		RT operator ()(void) const\n\
		{\n\
			return thunk(*this);\n\
		}\n\
		tgFunctorBase::operator bool;\n\
	protected:\n\
		typedef RT (*Thunk) (const tgFunctorBase&);\n\
		tgFunctor0wRet(Thunk t, const void* c, const void* f, size_t sz):\n\ tgFunctorBase(c,f,sz),thunk(t) {}\n\
	private:\n\
		Thunk thunk;\n\
};\n\
\n\
/**\n\
 * Traducteur de fonction statiques\n\
 */\n\
template <class Func, class RT>\n\
class tgFunctionTranslator0wRet:public tgFunctor0wRet<RT>\n\
{\n\
	public:\n\
		tgFunctionTranslator0wRet(Func f):tgFunctor0wRet<RT>(thunk,0,(const void*)f,0) {}\n\
		static RT thunk(const tgFunctorBase& ftor)\n\
		{\n\
			return (Func(ftor.pFunc))();\n\
		}\n\
};\n\
\n\
/**\n\
 * Traducteur de fonctions-membres\n\
 */\n\
template <class Callee, class MemFunc, class RT>\n\
class tgMemberTranslator0wRet: public tgFunctor0wRet<RT>\n\
{\n\
	public:\n\
		tgMemberTranslator0wRet(Callee& c, const MemFunc& m): tgFunctor0wRet<RT>(thunk, &c, &m, sizeof(MemFunc)) {}\n\
		static RT thunk (const tgFunctorBase& ftor)\n\
		{\n\
			return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))();\n\
		}\n\
};\n\
\n\
/**\n\
* Fonction s'occupant de créer un functor\n\
* utilisation : \n\
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a\n\
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test\n\
*/\n\
\n\
template <class TRT>\n\
inline tgFunctionTranslator0wRet<TRT (*) (void), TRT> \n\
tgMakeFunctor(tgFunctor0wRet<TRT>*, TRT (*f)(void))\n\
{\n\
	return tgFunctionTranslator0wRet<TRT (*)(void), TRT>(f);\n\
}\n\
\n\
template <class Callee,class TRT,class CallType>\n\
inline tgMemberTranslator0wRet<Callee,TRT (CallType::*)(void), TRT>\n\
 tgMakeFunctor(tgFunctor0wRet<TRT>*,Callee &c,TRT (CallType::* const &f)(void))\n\
{\n\
	typedef TRT (CallType::*MemFunc)(void);\n\
	return tgMemberTranslator0wRet<Callee,MemFunc, TRT>(c,f);\n\
}\n\
\n\
template <class Callee,class TRT,class CallType>\n\
inline tgMemberTranslator0wRet<const Callee,TRT (CallType::*)(void)const, TRT> \n\
tgMakeFunctor(tgFunctor0wRet<TRT>*,const Callee &c,TRT (CallType::* const &f)(void)const)\n\
{\n\
	typedef TRT (CallType::*MemFunc)(void)const;\n\
	return tgMemberTranslator0wRet<const Callee,MemFunc, TRT>(c,f);\n\
}");
}

void print_tgFunctor(const int num_args)
{
	int i=0;
	fprintf(out, "/**\n *Classe gérant les functor \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ">\n class tgFunctor%i:protected tgFunctorBase \n{ \npublic:\n tgFunctor%i(DummyInit* = 0) :tgFunctorBase(){}\n void operator () (",num_args,num_args);
		PR_P_p
	fprintf(out, ") const { thunk(*this, ");
		PR_p
	fprintf(out, "); }\n tgFunctorBase::operator bool; \nprotected:\n typedef void(*Thunk)(const tgFunctorBase&, ");
		PR_P
	fprintf(out, "); \ntgFunctor%i(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}\n private: \nThunk thunk; };", num_args);
}

void print_tgFunctorwRet(const int num_args)
{
	int i=0;
	fprintf(out, "/**\n *Classe gérant les functor \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ", class RT>\n class tgFunctor%iwRet:protected tgFunctorBase \n{ \npublic:\n tgFunctor%iwRet(DummyInit* = 0):tgFunctorBase() {}\n RT operator () (",num_args,num_args);
		PR_P_p
	fprintf(out, ") const { return thunk(*this, ");
		PR_p
	fprintf(out, "); }\n tgFunctorBase::operator bool; \nprotected:\n typedef RT(*Thunk)(const tgFunctorBase&, ");
		PR_P
	fprintf(out, "); \ntgFunctor%iwRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}\n private: \nThunk thunk; };", num_args);
}

void print_tgFunctionTranslator(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/** \n* Traducteur de fonction statiques \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ", class Func>\nclass tgFunctionTranslator%i :public tgFunctor%i<",num_args,num_args);
		PR_P
	fprintf(out, ">\n{\n public: tgFunctionTranslator%i(Func f):tgFunctor%i<",num_args,num_args);
		PR_P
	fprintf(out, ">(thunk,0,(const void*)f,0) {}\n static void thunk(const tgFunctorBase& ftor,");
		PR_P_p
	fprintf(out, ")\n {\n (Func(ftor.pFunc))(");
		PR_p;
	fprintf(out, ");\n}\n };");
}

void print_tgFunctionTranslatorwRet(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/** \n* Traducteur de fonction statiques \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ", class Func, class RT>\nclass tgFunctionTranslator%iwRet :public tgFunctor%iwRet<",num_args,num_args);
		PR_P
	fprintf(out, ", RT>\n{\n public: tgFunctionTranslator%iwRet(Func f):tgFunctor%iwRet<",num_args,num_args);
		PR_P
	fprintf(out, ", RT>(thunk,0,(const void*)f,0) {}\n static RT thunk(const tgFunctorBase& ftor,");
		PR_P_p
	fprintf(out, ")\n {\n return (Func(ftor.pFunc))(");
		PR_p;
	fprintf(out, ");\n}\n };");
}

void print_tgMemberTranslator(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/** \n* Traducteur de fonctions-membres \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ",class Callee, class MemFunc>\nclass tgMemberTranslator%i: public tgFunctor%i<",num_args,num_args);
		PR_P
	fprintf(out, ">\n{\npublic:tgMemberTranslator%i(Callee& c, const MemFunc& m): tgFunctor%i<", num_args, num_args);
		PR_P
	fprintf(out, ">(thunk, &c, &m, sizeof(MemFunc)) {}\nstatic void thunk (const tgFunctorBase& ftor, ");
		PR_P_p
	fprintf(out, ")\n{\n(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(");
		PR_p
	fprintf(out, "); \n}\n };");
}

void print_tgMemberTranslatorwRet(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/** \n* Traducteur de fonctions-membres \n*/\n template <");
		PR_CLASS_P
	fprintf(out, ",class Callee, class MemFunc, class RT>\nclass tgMemberTranslator%iwRet: public tgFunctor%iwRet<",num_args,num_args);
		PR_P
	fprintf(out, ", RT>\n{\npublic:tgMemberTranslator%iwRet(Callee& c, const MemFunc& m): tgFunctor%iwRet<", num_args, num_args);
		PR_P
	fprintf(out, ", RT>(thunk, &c, &m, sizeof(MemFunc)) {}\nstatic RT thunk (const tgFunctorBase& ftor, ");
		PR_P_p
	fprintf(out, ")\n{\nreturn (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(");
		PR_p
	fprintf(out, "); \n}\n };");
}

void print_tgMakeFunctor(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/**\n* Fonction s'occupant de créer un functor\n* utilisation : \n*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a\n*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test\n*/\ntemplate <");
		PR_CLASS_P
	fprintf(out, ", class TRT, ");
		PR_CLASS_TP
	fprintf(out, ">\n inline tgFunctionTranslator%i<");
		PR_P
	fprintf(out, ", TRT(*)(");
		PR_TP
	fprintf(out, ")>\ntgMakeFunctor(tgFunctor%i<", num_args);
		PR_P
	fprintf(out, ">*, TRT(*f)(");
		PR_TP
	fprintf(out, ")) \n {\nreturn tgFunctionTranslator%i<", num_args);
		PR_P
	fprintf(out, ", TRT(*)(");
		PR_TP
	fprintf(out, ")>(f);\n } \n\ntemplate <");
		PR_CLASS_P
	fprintf(out, ",class Callee,class TRT,class CallType,");
		PR_CLASS_TP
	fprintf(out, ">\ninline tgMemberTranslator%i<", num_args);
		PR_P
	fprintf(out, ",Callee,TRT (CallType::*)(");
		PR_TP
	fprintf(out, ")>tgMakeFunctor(tgFunctor%i<");
		PR_P
	fprintf(out, ">*, Callee &c,TRT (CallType::* const &f)(");
		PR_TP
	fprintf(out, "))\n{\ntypedef TRT (CallType::*MemFunc)(");
		PR_TP
	fprintf(out, ");\nreturn tgMemberTranslator%i<", num_args);
		PR_P
	fprintf(out, " ,Callee,MemFunc>(c,f);\n}");
	
	fprintf(out, "\ntemplate<");
		PR_CLASS_P
	fprintf(out, ",class Callee,class TRT,class CallType,");
		PR_CLASS_TP
	fprintf(out, ">\ninline tgMemberTranslator%i<", num_args);
		PR_P
	fprintf(out, ",const Callee,TRT (CallType::*)(TP1)const> \ntgMakeFunctor(tgFunctor%i<");
		PR_P
	fprintf(out, ">*,const Callee &c,TRT (CallType::* const &f)(TP1)const)\n{\ntypedef TRT (CallType::*MemFunc)(TP1)const;\nreturn tgMemberTranslator%i<", num_args);
		PR_P
	fprintf(out, ",const Callee,MemFunc>(c,f);\n}");
}

void print_tgMakeFunctorwRet(const int num_args)
{
	int i=0;
	fprintf(out, "\n\n/**\n* Fonction s'occupant de créer un functor\n* utilisation : \n*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a\n*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test\n*/\ntemplate <");
		PR_CLASS_P
	fprintf(out, ", ");
		PR_CLASS_TP
	fprintf(out, ", class RT>\n inline tgFunctionTranslator%iwRet<");
		PR_P
	fprintf(out, ", RT(*)(");
		PR_TP
	fprintf(out, "), RT>\ntgMakeFunctor(tgFunctor%iwRet<", num_args);
		PR_P
	fprintf(out, ", RT>*, RT(*f)(");
		PR_TP
	fprintf(out, ")) \n {\nreturn tgFunctionTranslator%iwRet<", num_args);
		PR_P
	fprintf(out, ", RT(*)(");
		PR_TP
	fprintf(out, "), RT>(f);\n } \n\ntemplate <");
		PR_CLASS_P
	fprintf(out, ",class Callee,class CallType,");
		PR_CLASS_TP
	fprintf(out, ", class RT>\ninline tgMemberTranslator%iwRet<", num_args);
		PR_P
	fprintf(out, ",Callee,RT (CallType::*)(");
		PR_TP
	fprintf(out, "), RT>tgMakeFunctor(tgFunctor%iwRet<");
		PR_P
	fprintf(out, ", RT>*, Callee &c,RT (CallType::* const &f)(");
		PR_TP
	fprintf(out, "))\n{\ntypedef RT (CallType::*MemFunc)(");
		PR_TP
	fprintf(out, ");\nreturn tgMemberTranslator%iwRet<", num_args);
		PR_P
	fprintf(out, " ,Callee,MemFunc, RT>(c,f);\n}");
	
	fprintf(out, "\ntemplate<");
		PR_CLASS_P
	fprintf(out, ",class Callee,class CallType,");
		PR_CLASS_TP
	fprintf(out, ", class RT>\ninline tgMemberTranslator%iwRet<", num_args);
		PR_P
	fprintf(out, ",const Callee,RT (CallType::*)(TP1)const, RT> \ntgMakeFunctor(tgFunctor%iwRet<");
		PR_P
	fprintf(out, ", RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)\n{\ntypedef RT (CallType::*MemFunc)(TP1)const;\nreturn tgMemberTranslator%iwRet<", num_args);
		PR_P
	fprintf(out, ",const Callee,MemFunc, RT>(c,f);\n}");
}


#define NUM_ARGS 10

int main(int argc, char** argv)
{
	int argsCount = 0;
	int i=0;
	out = fopen(argv[1], "wt");
	fprintf(out, "/** Template de functor, utilises pour les callback\n* Ce fichier a été généré automatiquement\n*/\n\n");
	fprintf(out, "#ifndef TGFUNCTOR_H\n#define TGFUNCTOR_H\n#include <stdlib.h>\n#include <string.h>\n\n");
	fprintf(out, "%s", functor_base_def);
	
	print_zero_args ();
	
	for(i=1; i<NUM_ARGS+1;i++) {
		fprintf(out, "\n\n\n");
		fprintf(out, "/****************************************Functor a %i arguments, sans retour****************************************/",i);
		fprintf(out, "\n\n\n");
		print_tgFunctor(i);
		print_tgFunctionTranslator(i);
		print_tgMemberTranslator(i);
		print_tgMakeFunctor(i);
		
		fprintf(out, "\n\n\n");
		fprintf(out, "/****************************************Functor a %i arguments, avec retour****************************************/",i);
		fprintf(out, "\n\n\n");
		print_tgFunctorwRet(i);
		print_tgFunctionTranslatorwRet(i);
		print_tgMemberTranslatorwRet(i);
		print_tgMakeFunctorwRet(i);
		
	}
	
	fprintf(out, "\n#endif\n");

	return 0;
}
