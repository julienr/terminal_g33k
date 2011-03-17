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
/** Template de functor, utilises pour les callback
* Ce fichier a été généré automatiquement
*/

#ifndef TGFUNCTOR_H
#define TGFUNCTOR_H
#include "tglib.h"
/*#include <stdlib.h>
#include <string.h>*/

/*
*
 * Classe de base de tout les functors
 */ 
class tgFunctorBase
 { 
	public:
 		typedef void (tgFunctorBase::*_MemFunc) (); 
		typedef void (*_Func)(); 
		inline tgFunctorBase ():pCallee(0),pFunc(0) {} 
		inline tgFunctorBase (const void* c, const void* f, size_t sz) 
		{ 
			if (c) { /*on a c=callee, f = fonction membre*/ 
				pCallee = (void*)c; 
				memcpy(pMemFunc, f, sz); 
			} else { /*un simple pointeur sur fonction statique*/ 
				pFunc = f; 
			} 
		}
 
 		inline operator bool() const { return pFunc||pCallee; } 

 		class DummyInit {
 		}; 

 		void* pCallee; 
		union { 
			const void* pFunc; 
			char pMemFunc[sizeof(_MemFunc)]; 
		}; 
};


/****************************************Functor sans arguments, sans retour****************************************/


/** 
* Classe gérant les functor
*/
class tgFunctor0: protected tgFunctorBase
{
	public:
		tgFunctor0(DummyInit* = 0) :tgFunctorBase() {}
		void operator ()(void) const
		{
			thunk(*this);
		}
		tgFunctorBase::operator bool;
	protected:
		typedef void (*Thunk) (const tgFunctorBase&);
		tgFunctor0(Thunk t, const void* c, const void* f, size_t sz):
 tgFunctorBase(c,f,sz),thunk(t) {}
	private:
		Thunk thunk;
};

/**
 * Traducteur de fonction statiques
 */
template <class Func>
class tgFunctionTranslator0:public tgFunctor0
{
	public:
		tgFunctionTranslator0(Func f):tgFunctor0(thunk,0,(const void*)f,0) {}
		static void thunk(const tgFunctorBase& ftor)
		{
			(Func(ftor.pFunc))();
		}
};

/**
 * Traducteur de fonctions-membres
 */
template <class Callee, class MemFunc>
class tgMemberTranslator0: public tgFunctor0
{
	public:
		tgMemberTranslator0(Callee& c, const MemFunc& m): tgFunctor0(thunk, &c, &m, sizeof(MemFunc)) {}
		static void thunk (const tgFunctorBase& ftor)
		{
			(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))();
		}
};

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/

template <class TRT>
inline tgFunctionTranslator0<TRT (*) (void)> 
tgMakeFunctor(tgFunctor0*, TRT (*f)(void))
{
	return tgFunctionTranslator0<TRT (*)(void)>(f);
}

template <class Callee,class TRT,class CallType>
inline tgMemberTranslator0<Callee,TRT (CallType::*)(void)>
 tgMakeFunctor(tgFunctor0*,Callee &c,TRT (CallType::* const &f)(void))
{
	typedef TRT (CallType::*MemFunc)(void);
	return tgMemberTranslator0<Callee,MemFunc>(c,f);
}

template <class Callee,class TRT,class CallType>
inline tgMemberTranslator0<const Callee,TRT (CallType::*)(void)const> 
tgMakeFunctor(tgFunctor0*,const Callee &c,TRT (CallType::* const &f)(void)const)
{
	typedef TRT (CallType::*MemFunc)(void)const;
	return tgMemberTranslator0<const Callee,MemFunc>(c,f);
}


/****************************************Functor sansarguments, avec retour****************************************/


/** 
* Classe gérant les functor
*/
template <class RT>
 class tgFunctor0wRet: protected tgFunctorBase
{
	public:
		tgFunctor0wRet(DummyInit* = 0) :tgFunctorBase(){}
		RT operator ()(void) const
		{
			return thunk(*this);
		}
		tgFunctorBase::operator bool;
	protected:
		typedef RT (*Thunk) (const tgFunctorBase&);
		tgFunctor0wRet(Thunk t, const void* c, const void* f, size_t sz):
 tgFunctorBase(c,f,sz),thunk(t) {}
	private:
		Thunk thunk;
};

/**
 * Traducteur de fonction statiques
 */
template <class Func, class RT>
class tgFunctionTranslator0wRet:public tgFunctor0wRet<RT>
{
	public:
		tgFunctionTranslator0wRet(Func f):tgFunctor0wRet<RT>(thunk,0,(const void*)f,0) {}
		static RT thunk(const tgFunctorBase& ftor)
		{
			return (Func(ftor.pFunc))();
		}
};

/**
 * Traducteur de fonctions-membres
 */
template <class Callee, class MemFunc, class RT>
class tgMemberTranslator0wRet: public tgFunctor0wRet<RT>
{
	public:
		tgMemberTranslator0wRet(Callee& c, const MemFunc& m): tgFunctor0wRet<RT>(thunk, &c, &m, sizeof(MemFunc)) {}
		static RT thunk (const tgFunctorBase& ftor)
		{
			return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))();
		}
};

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/

template <class TRT>
inline tgFunctionTranslator0wRet<TRT (*) (void), TRT> 
tgMakeFunctor(tgFunctor0wRet<TRT>*, TRT (*f)(void))
{
	return tgFunctionTranslator0wRet<TRT (*)(void), TRT>(f);
}

template <class Callee,class TRT,class CallType>
inline tgMemberTranslator0wRet<Callee,TRT (CallType::*)(void), TRT>
 tgMakeFunctor(tgFunctor0wRet<TRT>*,Callee &c,TRT (CallType::* const &f)(void))
{
	typedef TRT (CallType::*MemFunc)(void);
	return tgMemberTranslator0wRet<Callee,MemFunc, TRT>(c,f);
}

template <class Callee,class TRT,class CallType>
inline tgMemberTranslator0wRet<const Callee,TRT (CallType::*)(void)const, TRT> 
tgMakeFunctor(tgFunctor0wRet<TRT>*,const Callee &c,TRT (CallType::* const &f)(void)const)
{
	typedef TRT (CallType::*MemFunc)(void)const;
	return tgMemberTranslator0wRet<const Callee,MemFunc, TRT>(c,f);
}


/****************************************Functor a 1 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1>
 class tgFunctor1:protected tgFunctorBase 
{ 
public:
 tgFunctor1(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1) const { thunk(*this, p1); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1); 
tgFunctor1(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class Func>
class tgFunctionTranslator1 :public tgFunctor1<P1>
{
 public: tgFunctionTranslator1(Func f):tgFunctor1<P1>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1)
 {
 (Func(ftor.pFunc))(p1);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1,class Callee, class MemFunc>
class tgMemberTranslator1: public tgFunctor1<P1>
{
public:tgMemberTranslator1(Callee& c, const MemFunc& m): tgFunctor1<P1>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class TRT, class TP1>
 inline tgFunctionTranslator1<P1, TRT(*)(TP1)>
tgMakeFunctor(tgFunctor1<P1>*, TRT(*f)(TP1)) 
 {
return tgFunctionTranslator1<P1, TRT(*)(TP1)>(f);
 } 

template <class P1,class Callee,class TRT,class CallType,class TP1>
inline tgMemberTranslator1<P1,Callee,TRT (CallType::*)(TP1)>tgMakeFunctor(tgFunctor1<P1>*, Callee &c,TRT (CallType::* const &f)(TP1))
{
typedef TRT (CallType::*MemFunc)(TP1);
return tgMemberTranslator1<P1 ,Callee,MemFunc>(c,f);
}
template<class P1,class Callee,class TRT,class CallType,class TP1>
inline tgMemberTranslator1<P1,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor1<P1>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator1<P1,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 1 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class RT>
 class tgFunctor1wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor1wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1) const { return thunk(*this, p1); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1); 
tgFunctor1wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class Func, class RT>
class tgFunctionTranslator1wRet :public tgFunctor1wRet<P1, RT>
{
 public: tgFunctionTranslator1wRet(Func f):tgFunctor1wRet<P1, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1)
 {
 return (Func(ftor.pFunc))(p1);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1,class Callee, class MemFunc, class RT>
class tgMemberTranslator1wRet: public tgFunctor1wRet<P1, RT>
{
public:tgMemberTranslator1wRet(Callee& c, const MemFunc& m): tgFunctor1wRet<P1, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class TP1, class RT>
 inline tgFunctionTranslator1wRet<P1, RT(*)(TP1), RT>
tgMakeFunctor(tgFunctor1wRet<P1, RT>*, RT(*f)(TP1)) 
 {
return tgFunctionTranslator1wRet<P1, RT(*)(TP1), RT>(f);
 } 

template <class P1,class Callee,class CallType,class TP1, class RT>
inline tgMemberTranslator1wRet<P1,Callee,RT (CallType::*)(TP1), RT>tgMakeFunctor(tgFunctor1wRet<P1, RT>*, Callee &c,RT (CallType::* const &f)(TP1))
{
typedef RT (CallType::*MemFunc)(TP1);
return tgMemberTranslator1wRet<P1 ,Callee,MemFunc, RT>(c,f);
}
template<class P1,class Callee,class CallType,class TP1, class RT>
inline tgMemberTranslator1wRet<P1,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor1wRet<P1, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator1wRet<P1,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 2 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2>
 class tgFunctor2:protected tgFunctorBase 
{ 
public:
 tgFunctor2(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2) const { thunk(*this, p1, p2); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2); 
tgFunctor2(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class Func>
class tgFunctionTranslator2 :public tgFunctor2<P1,P2>
{
 public: tgFunctionTranslator2(Func f):tgFunctor2<P1,P2>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2)
 {
 (Func(ftor.pFunc))(p1, p2);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2,class Callee, class MemFunc>
class tgMemberTranslator2: public tgFunctor2<P1,P2>
{
public:tgMemberTranslator2(Callee& c, const MemFunc& m): tgFunctor2<P1,P2>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class TRT, class TP1, class TP2>
 inline tgFunctionTranslator2<P1,P2, TRT(*)(TP1,TP2)>
tgMakeFunctor(tgFunctor2<P1,P2>*, TRT(*f)(TP1,TP2)) 
 {
return tgFunctionTranslator2<P1,P2, TRT(*)(TP1,TP2)>(f);
 } 

template <class P1, class P2,class Callee,class TRT,class CallType,class TP1, class TP2>
inline tgMemberTranslator2<P1,P2,Callee,TRT (CallType::*)(TP1,TP2)>tgMakeFunctor(tgFunctor2<P1,P2>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2);
return tgMemberTranslator2<P1,P2 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2,class Callee,class TRT,class CallType,class TP1, class TP2>
inline tgMemberTranslator2<P1,P2,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor2<P1,P2>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator2<P1,P2,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 2 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class RT>
 class tgFunctor2wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor2wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2) const { return thunk(*this, p1, p2); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2); 
tgFunctor2wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class Func, class RT>
class tgFunctionTranslator2wRet :public tgFunctor2wRet<P1,P2, RT>
{
 public: tgFunctionTranslator2wRet(Func f):tgFunctor2wRet<P1,P2, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2)
 {
 return (Func(ftor.pFunc))(p1, p2);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2,class Callee, class MemFunc, class RT>
class tgMemberTranslator2wRet: public tgFunctor2wRet<P1,P2, RT>
{
public:tgMemberTranslator2wRet(Callee& c, const MemFunc& m): tgFunctor2wRet<P1,P2, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class TP1, class TP2, class RT>
 inline tgFunctionTranslator2wRet<P1,P2, RT(*)(TP1,TP2), RT>
tgMakeFunctor(tgFunctor2wRet<P1,P2, RT>*, RT(*f)(TP1,TP2)) 
 {
return tgFunctionTranslator2wRet<P1,P2, RT(*)(TP1,TP2), RT>(f);
 } 

template <class P1, class P2,class Callee,class CallType,class TP1, class TP2, class RT>
inline tgMemberTranslator2wRet<P1,P2,Callee,RT (CallType::*)(TP1,TP2), RT>tgMakeFunctor(tgFunctor2wRet<P1,P2, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2))
{
typedef RT (CallType::*MemFunc)(TP1,TP2);
return tgMemberTranslator2wRet<P1,P2 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2,class Callee,class CallType,class TP1, class TP2, class RT>
inline tgMemberTranslator2wRet<P1,P2,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor2wRet<P1,P2, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator2wRet<P1,P2,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 3 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3>
 class tgFunctor3:protected tgFunctorBase 
{ 
public:
 tgFunctor3(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3) const { thunk(*this, p1, p2, p3); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3); 
tgFunctor3(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class Func>
class tgFunctionTranslator3 :public tgFunctor3<P1,P2,P3>
{
 public: tgFunctionTranslator3(Func f):tgFunctor3<P1,P2,P3>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3)
 {
 (Func(ftor.pFunc))(p1, p2, p3);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3,class Callee, class MemFunc>
class tgMemberTranslator3: public tgFunctor3<P1,P2,P3>
{
public:tgMemberTranslator3(Callee& c, const MemFunc& m): tgFunctor3<P1,P2,P3>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class TRT, class TP1, class TP2, class TP3>
 inline tgFunctionTranslator3<P1,P2,P3, TRT(*)(TP1,TP2,TP3)>
tgMakeFunctor(tgFunctor3<P1,P2,P3>*, TRT(*f)(TP1,TP2,TP3)) 
 {
return tgFunctionTranslator3<P1,P2,P3, TRT(*)(TP1,TP2,TP3)>(f);
 } 

template <class P1, class P2, class P3,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3>
inline tgMemberTranslator3<P1,P2,P3,Callee,TRT (CallType::*)(TP1,TP2,TP3)>tgMakeFunctor(tgFunctor3<P1,P2,P3>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
return tgMemberTranslator3<P1,P2,P3 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3>
inline tgMemberTranslator3<P1,P2,P3,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor3<P1,P2,P3>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator3<P1,P2,P3,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 3 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class RT>
 class tgFunctor3wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor3wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3) const { return thunk(*this, p1, p2, p3); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3); 
tgFunctor3wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class Func, class RT>
class tgFunctionTranslator3wRet :public tgFunctor3wRet<P1,P2,P3, RT>
{
 public: tgFunctionTranslator3wRet(Func f):tgFunctor3wRet<P1,P2,P3, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3)
 {
 return (Func(ftor.pFunc))(p1, p2, p3);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3,class Callee, class MemFunc, class RT>
class tgMemberTranslator3wRet: public tgFunctor3wRet<P1,P2,P3, RT>
{
public:tgMemberTranslator3wRet(Callee& c, const MemFunc& m): tgFunctor3wRet<P1,P2,P3, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class TP1, class TP2, class TP3, class RT>
 inline tgFunctionTranslator3wRet<P1,P2,P3, RT(*)(TP1,TP2,TP3), RT>
tgMakeFunctor(tgFunctor3wRet<P1,P2,P3, RT>*, RT(*f)(TP1,TP2,TP3)) 
 {
return tgFunctionTranslator3wRet<P1,P2,P3, RT(*)(TP1,TP2,TP3), RT>(f);
 } 

template <class P1, class P2, class P3,class Callee,class CallType,class TP1, class TP2, class TP3, class RT>
inline tgMemberTranslator3wRet<P1,P2,P3,Callee,RT (CallType::*)(TP1,TP2,TP3), RT>tgMakeFunctor(tgFunctor3wRet<P1,P2,P3, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3);
return tgMemberTranslator3wRet<P1,P2,P3 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3,class Callee,class CallType,class TP1, class TP2, class TP3, class RT>
inline tgMemberTranslator3wRet<P1,P2,P3,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor3wRet<P1,P2,P3, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator3wRet<P1,P2,P3,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 4 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4>
 class tgFunctor4:protected tgFunctorBase 
{ 
public:
 tgFunctor4(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4) const { thunk(*this, p1, p2, p3, p4); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4); 
tgFunctor4(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class Func>
class tgFunctionTranslator4 :public tgFunctor4<P1,P2,P3,P4>
{
 public: tgFunctionTranslator4(Func f):tgFunctor4<P1,P2,P3,P4>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4,class Callee, class MemFunc>
class tgMemberTranslator4: public tgFunctor4<P1,P2,P3,P4>
{
public:tgMemberTranslator4(Callee& c, const MemFunc& m): tgFunctor4<P1,P2,P3,P4>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class TRT, class TP1, class TP2, class TP3, class TP4>
 inline tgFunctionTranslator4<P1,P2,P3,P4, TRT(*)(TP1,TP2,TP3,TP4)>
tgMakeFunctor(tgFunctor4<P1,P2,P3,P4>*, TRT(*f)(TP1,TP2,TP3,TP4)) 
 {
return tgFunctionTranslator4<P1,P2,P3,P4, TRT(*)(TP1,TP2,TP3,TP4)>(f);
 } 

template <class P1, class P2, class P3, class P4,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4>
inline tgMemberTranslator4<P1,P2,P3,P4,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4)>tgMakeFunctor(tgFunctor4<P1,P2,P3,P4>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4);
return tgMemberTranslator4<P1,P2,P3,P4 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4>
inline tgMemberTranslator4<P1,P2,P3,P4,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor4<P1,P2,P3,P4>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator4<P1,P2,P3,P4,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 4 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class RT>
 class tgFunctor4wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor4wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4) const { return thunk(*this, p1, p2, p3, p4); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4); 
tgFunctor4wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class Func, class RT>
class tgFunctionTranslator4wRet :public tgFunctor4wRet<P1,P2,P3,P4, RT>
{
 public: tgFunctionTranslator4wRet(Func f):tgFunctor4wRet<P1,P2,P3,P4, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4,class Callee, class MemFunc, class RT>
class tgMemberTranslator4wRet: public tgFunctor4wRet<P1,P2,P3,P4, RT>
{
public:tgMemberTranslator4wRet(Callee& c, const MemFunc& m): tgFunctor4wRet<P1,P2,P3,P4, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class TP1, class TP2, class TP3, class TP4, class RT>
 inline tgFunctionTranslator4wRet<P1,P2,P3,P4, RT(*)(TP1,TP2,TP3,TP4), RT>
tgMakeFunctor(tgFunctor4wRet<P1,P2,P3,P4, RT>*, RT(*f)(TP1,TP2,TP3,TP4)) 
 {
return tgFunctionTranslator4wRet<P1,P2,P3,P4, RT(*)(TP1,TP2,TP3,TP4), RT>(f);
 } 

template <class P1, class P2, class P3, class P4,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class RT>
inline tgMemberTranslator4wRet<P1,P2,P3,P4,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4), RT>tgMakeFunctor(tgFunctor4wRet<P1,P2,P3,P4, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4);
return tgMemberTranslator4wRet<P1,P2,P3,P4 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class RT>
inline tgMemberTranslator4wRet<P1,P2,P3,P4,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor4wRet<P1,P2,P3,P4, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator4wRet<P1,P2,P3,P4,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 5 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5>
 class tgFunctor5:protected tgFunctorBase 
{ 
public:
 tgFunctor5(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) const { thunk(*this, p1, p2, p3, p4, p5); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5); 
tgFunctor5(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class Func>
class tgFunctionTranslator5 :public tgFunctor5<P1,P2,P3,P4,P5>
{
 public: tgFunctionTranslator5(Func f):tgFunctor5<P1,P2,P3,P4,P5>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5,class Callee, class MemFunc>
class tgMemberTranslator5: public tgFunctor5<P1,P2,P3,P4,P5>
{
public:tgMemberTranslator5(Callee& c, const MemFunc& m): tgFunctor5<P1,P2,P3,P4,P5>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5>
 inline tgFunctionTranslator5<P1,P2,P3,P4,P5, TRT(*)(TP1,TP2,TP3,TP4,TP5)>
tgMakeFunctor(tgFunctor5<P1,P2,P3,P4,P5>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5)) 
 {
return tgFunctionTranslator5<P1,P2,P3,P4,P5, TRT(*)(TP1,TP2,TP3,TP4,TP5)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5>
inline tgMemberTranslator5<P1,P2,P3,P4,P5,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5)>tgMakeFunctor(tgFunctor5<P1,P2,P3,P4,P5>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5);
return tgMemberTranslator5<P1,P2,P3,P4,P5 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5>
inline tgMemberTranslator5<P1,P2,P3,P4,P5,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor5<P1,P2,P3,P4,P5>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator5<P1,P2,P3,P4,P5,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 5 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class RT>
 class tgFunctor5wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor5wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) const { return thunk(*this, p1, p2, p3, p4, p5); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5); 
tgFunctor5wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class Func, class RT>
class tgFunctionTranslator5wRet :public tgFunctor5wRet<P1,P2,P3,P4,P5, RT>
{
 public: tgFunctionTranslator5wRet(Func f):tgFunctor5wRet<P1,P2,P3,P4,P5, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5,class Callee, class MemFunc, class RT>
class tgMemberTranslator5wRet: public tgFunctor5wRet<P1,P2,P3,P4,P5, RT>
{
public:tgMemberTranslator5wRet(Callee& c, const MemFunc& m): tgFunctor5wRet<P1,P2,P3,P4,P5, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class TP1, class TP2, class TP3, class TP4, class TP5, class RT>
 inline tgFunctionTranslator5wRet<P1,P2,P3,P4,P5, RT(*)(TP1,TP2,TP3,TP4,TP5), RT>
tgMakeFunctor(tgFunctor5wRet<P1,P2,P3,P4,P5, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5)) 
 {
return tgFunctionTranslator5wRet<P1,P2,P3,P4,P5, RT(*)(TP1,TP2,TP3,TP4,TP5), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class RT>
inline tgMemberTranslator5wRet<P1,P2,P3,P4,P5,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5), RT>tgMakeFunctor(tgFunctor5wRet<P1,P2,P3,P4,P5, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5);
return tgMemberTranslator5wRet<P1,P2,P3,P4,P5 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class RT>
inline tgMemberTranslator5wRet<P1,P2,P3,P4,P5,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor5wRet<P1,P2,P3,P4,P5, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator5wRet<P1,P2,P3,P4,P5,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 6 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6>
 class tgFunctor6:protected tgFunctorBase 
{ 
public:
 tgFunctor6(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6) const { thunk(*this, p1, p2, p3, p4, p5, p6); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6); 
tgFunctor6(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class Func>
class tgFunctionTranslator6 :public tgFunctor6<P1,P2,P3,P4,P5,P6>
{
 public: tgFunctionTranslator6(Func f):tgFunctor6<P1,P2,P3,P4,P5,P6>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6,class Callee, class MemFunc>
class tgMemberTranslator6: public tgFunctor6<P1,P2,P3,P4,P5,P6>
{
public:tgMemberTranslator6(Callee& c, const MemFunc& m): tgFunctor6<P1,P2,P3,P4,P5,P6>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6>
 inline tgFunctionTranslator6<P1,P2,P3,P4,P5,P6, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6)>
tgMakeFunctor(tgFunctor6<P1,P2,P3,P4,P5,P6>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5,TP6)) 
 {
return tgFunctionTranslator6<P1,P2,P3,P4,P5,P6, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6>
inline tgMemberTranslator6<P1,P2,P3,P4,P5,P6,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6)>tgMakeFunctor(tgFunctor6<P1,P2,P3,P4,P5,P6>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6);
return tgMemberTranslator6<P1,P2,P3,P4,P5,P6 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6>
inline tgMemberTranslator6<P1,P2,P3,P4,P5,P6,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor6<P1,P2,P3,P4,P5,P6>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator6<P1,P2,P3,P4,P5,P6,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 6 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class RT>
 class tgFunctor6wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor6wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6) const { return thunk(*this, p1, p2, p3, p4, p5, p6); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6); 
tgFunctor6wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class Func, class RT>
class tgFunctionTranslator6wRet :public tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>
{
 public: tgFunctionTranslator6wRet(Func f):tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6,class Callee, class MemFunc, class RT>
class tgMemberTranslator6wRet: public tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>
{
public:tgMemberTranslator6wRet(Callee& c, const MemFunc& m): tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class RT>
 inline tgFunctionTranslator6wRet<P1,P2,P3,P4,P5,P6, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6), RT>
tgMakeFunctor(tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5,TP6)) 
 {
return tgFunctionTranslator6wRet<P1,P2,P3,P4,P5,P6, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class RT>
inline tgMemberTranslator6wRet<P1,P2,P3,P4,P5,P6,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6), RT>tgMakeFunctor(tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6);
return tgMemberTranslator6wRet<P1,P2,P3,P4,P5,P6 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class RT>
inline tgMemberTranslator6wRet<P1,P2,P3,P4,P5,P6,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor6wRet<P1,P2,P3,P4,P5,P6, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator6wRet<P1,P2,P3,P4,P5,P6,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 7 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
 class tgFunctor7:protected tgFunctorBase 
{ 
public:
 tgFunctor7(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7) const { thunk(*this, p1, p2, p3, p4, p5, p6, p7); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7); 
tgFunctor7(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class Func>
class tgFunctionTranslator7 :public tgFunctor7<P1,P2,P3,P4,P5,P6,P7>
{
 public: tgFunctionTranslator7(Func f):tgFunctor7<P1,P2,P3,P4,P5,P6,P7>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee, class MemFunc>
class tgMemberTranslator7: public tgFunctor7<P1,P2,P3,P4,P5,P6,P7>
{
public:tgMemberTranslator7(Callee& c, const MemFunc& m): tgFunctor7<P1,P2,P3,P4,P5,P6,P7>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7>
 inline tgFunctionTranslator7<P1,P2,P3,P4,P5,P6,P7, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7)>
tgMakeFunctor(tgFunctor7<P1,P2,P3,P4,P5,P6,P7>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7)) 
 {
return tgFunctionTranslator7<P1,P2,P3,P4,P5,P6,P7, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7>
inline tgMemberTranslator7<P1,P2,P3,P4,P5,P6,P7,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7)>tgMakeFunctor(tgFunctor7<P1,P2,P3,P4,P5,P6,P7>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7);
return tgMemberTranslator7<P1,P2,P3,P4,P5,P6,P7 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7>
inline tgMemberTranslator7<P1,P2,P3,P4,P5,P6,P7,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor7<P1,P2,P3,P4,P5,P6,P7>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator7<P1,P2,P3,P4,P5,P6,P7,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 7 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class RT>
 class tgFunctor7wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor7wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7) const { return thunk(*this, p1, p2, p3, p4, p5, p6, p7); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7); 
tgFunctor7wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class Func, class RT>
class tgFunctionTranslator7wRet :public tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>
{
 public: tgFunctionTranslator7wRet(Func f):tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee, class MemFunc, class RT>
class tgMemberTranslator7wRet: public tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>
{
public:tgMemberTranslator7wRet(Callee& c, const MemFunc& m): tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class RT>
 inline tgFunctionTranslator7wRet<P1,P2,P3,P4,P5,P6,P7, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7), RT>
tgMakeFunctor(tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7)) 
 {
return tgFunctionTranslator7wRet<P1,P2,P3,P4,P5,P6,P7, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class RT>
inline tgMemberTranslator7wRet<P1,P2,P3,P4,P5,P6,P7,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7), RT>tgMakeFunctor(tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7);
return tgMemberTranslator7wRet<P1,P2,P3,P4,P5,P6,P7 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class RT>
inline tgMemberTranslator7wRet<P1,P2,P3,P4,P5,P6,P7,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor7wRet<P1,P2,P3,P4,P5,P6,P7, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator7wRet<P1,P2,P3,P4,P5,P6,P7,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 8 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
 class tgFunctor8:protected tgFunctorBase 
{ 
public:
 tgFunctor8(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8) const { thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8); 
tgFunctor8(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class Func>
class tgFunctionTranslator8 :public tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>
{
 public: tgFunctionTranslator8(Func f):tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee, class MemFunc>
class tgMemberTranslator8: public tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>
{
public:tgMemberTranslator8(Callee& c, const MemFunc& m): tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8>
 inline tgFunctionTranslator8<P1,P2,P3,P4,P5,P6,P7,P8, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8)>
tgMakeFunctor(tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8)) 
 {
return tgFunctionTranslator8<P1,P2,P3,P4,P5,P6,P7,P8, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8>
inline tgMemberTranslator8<P1,P2,P3,P4,P5,P6,P7,P8,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8)>tgMakeFunctor(tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8);
return tgMemberTranslator8<P1,P2,P3,P4,P5,P6,P7,P8 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8>
inline tgMemberTranslator8<P1,P2,P3,P4,P5,P6,P7,P8,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor8<P1,P2,P3,P4,P5,P6,P7,P8>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator8<P1,P2,P3,P4,P5,P6,P7,P8,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 8 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class RT>
 class tgFunctor8wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor8wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8) const { return thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8); 
tgFunctor8wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class Func, class RT>
class tgFunctionTranslator8wRet :public tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>
{
 public: tgFunctionTranslator8wRet(Func f):tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee, class MemFunc, class RT>
class tgMemberTranslator8wRet: public tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>
{
public:tgMemberTranslator8wRet(Callee& c, const MemFunc& m): tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class RT>
 inline tgFunctionTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8), RT>
tgMakeFunctor(tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8)) 
 {
return tgFunctionTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class RT>
inline tgMemberTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8), RT>tgMakeFunctor(tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8);
return tgMemberTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class RT>
inline tgMemberTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor8wRet<P1,P2,P3,P4,P5,P6,P7,P8, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator8wRet<P1,P2,P3,P4,P5,P6,P7,P8,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 9 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
 class tgFunctor9:protected tgFunctorBase 
{ 
public:
 tgFunctor9(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9) const { thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8,P9); 
tgFunctor9(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class Func>
class tgFunctionTranslator9 :public tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>
{
 public: tgFunctionTranslator9(Func f):tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee, class MemFunc>
class tgMemberTranslator9: public tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>
{
public:tgMemberTranslator9(Callee& c, const MemFunc& m): tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9>
 inline tgFunctionTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9)>
tgMakeFunctor(tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9)) 
 {
return tgFunctionTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9>
inline tgMemberTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9)>tgMakeFunctor(tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9);
return tgMemberTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9>
inline tgMemberTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor9<P1,P2,P3,P4,P5,P6,P7,P8,P9>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator9<P1,P2,P3,P4,P5,P6,P7,P8,P9,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 9 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class RT>
 class tgFunctor9wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor9wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9) const { return thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8,P9); 
tgFunctor9wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class Func, class RT>
class tgFunctionTranslator9wRet :public tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>
{
 public: tgFunctionTranslator9wRet(Func f):tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee, class MemFunc, class RT>
class tgMemberTranslator9wRet: public tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>
{
public:tgMemberTranslator9wRet(Callee& c, const MemFunc& m): tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class RT>
 inline tgFunctionTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9), RT>
tgMakeFunctor(tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9)) 
 {
return tgFunctionTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class RT>
inline tgMemberTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9), RT>tgMakeFunctor(tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9);
return tgMemberTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class RT>
inline tgMemberTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator9wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,const Callee,MemFunc, RT>(c,f);
}


/****************************************Functor a 10 arguments, sans retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
 class tgFunctor10:protected tgFunctorBase 
{ 
public:
 tgFunctor10(DummyInit* = 0) :tgFunctorBase(){}
 void operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10) const { thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
 tgFunctorBase::operator bool; 
protected:
 typedef void(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8,P9,P10); 
tgFunctor10(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class Func>
class tgFunctionTranslator10 :public tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>
{
 public: tgFunctionTranslator10(Func f):tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>(thunk,0,(const void*)f,0) {}
 static void thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
 {
 (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee, class MemFunc>
class tgMemberTranslator10: public tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>
{
public:tgMemberTranslator10(Callee& c, const MemFunc& m): tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>(thunk, &c, &m, sizeof(MemFunc)) {}
static void thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
{
(((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class TRT, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10>
 inline tgFunctionTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10)>
tgMakeFunctor(tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>*, TRT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10)) 
 {
return tgFunctionTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, TRT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10)>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10>
inline tgMemberTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,Callee,TRT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10)>tgMakeFunctor(tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>*, Callee &c,TRT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10))
{
typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10);
return tgMemberTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10 ,Callee,MemFunc>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee,class TRT,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10>
inline tgMemberTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,const Callee,TRT (CallType::*)(TP1)const> 
tgMakeFunctor(tgFunctor10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>*,const Callee &c,TRT (CallType::* const &f)(TP1)const)
{
typedef TRT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator10<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,const Callee,MemFunc>(c,f);
}


/****************************************Functor a 10 arguments, avec retour****************************************/


/**
 *Classe gérant les functor 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class RT>
 class tgFunctor10wRet:protected tgFunctorBase 
{ 
public:
 tgFunctor10wRet(DummyInit* = 0):tgFunctorBase() {}
 RT operator () (P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10) const { return thunk(*this, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
 tgFunctorBase::operator bool; 
protected:
 typedef RT(*Thunk)(const tgFunctorBase&, P1,P2,P3,P4,P5,P6,P7,P8,P9,P10); 
tgFunctor10wRet(Thunk t, const void* c, const void* f, size_t sz):tgFunctorBase(c,f,sz),thunk(t) {}
 private: 
Thunk thunk; };

/** 
* Traducteur de fonction statiques 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class Func, class RT>
class tgFunctionTranslator10wRet :public tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>
{
 public: tgFunctionTranslator10wRet(Func f):tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>(thunk,0,(const void*)f,0) {}
 static RT thunk(const tgFunctorBase& ftor,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
 {
 return (Func(ftor.pFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
}
 };

/** 
* Traducteur de fonctions-membres 
*/
 template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee, class MemFunc, class RT>
class tgMemberTranslator10wRet: public tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>
{
public:tgMemberTranslator10wRet(Callee& c, const MemFunc& m): tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>(thunk, &c, &m, sizeof(MemFunc)) {}
static RT thunk (const tgFunctorBase& ftor, P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
{
return (((Callee*)ftor.pCallee)->**(MemFunc*)(void*)(ftor.pMemFunc))(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); 
}
 };

/**
* Fonction s'occupant de créer un functor
* utilisation : 
*   #makeFunctor((Functor1<int>*)0, &a, &A::Test); //pour une fonction A::Test de l'instance a
*   #makeFunctor((Functor1<int>*)0, &Test); //pour une fonction non-membre Test
*/
template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10, class RT>
 inline tgFunctionTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10), RT>
tgMakeFunctor(tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>*, RT(*f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10)) 
 {
return tgFunctionTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT(*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10), RT>(f);
 } 

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10, class RT>
inline tgMemberTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,Callee,RT (CallType::*)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10), RT>tgMakeFunctor(tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>*, Callee &c,RT (CallType::* const &f)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10))
{
typedef RT (CallType::*MemFunc)(TP1,TP2,TP3,TP4,TP5,TP6,TP7,TP8,TP9,TP10);
return tgMemberTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10 ,Callee,MemFunc, RT>(c,f);
}
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10,class Callee,class CallType,class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10, class RT>
inline tgMemberTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,const Callee,RT (CallType::*)(TP1)const, RT> 
tgMakeFunctor(tgFunctor10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10, RT>*,const Callee &c,RT (CallType::* const &f)(TP1)const)
{
typedef RT (CallType::*MemFunc)(TP1)const;
return tgMemberTranslator10wRet<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,const Callee,MemFunc, RT>(c,f);
}
#endif
