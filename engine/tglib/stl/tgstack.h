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
#ifndef TGSTACK_H
#define TGSTACK_H

template <class T>
class tgStack
{
 public:
	tgStack(int size=0);
	~tgStack ();

	void push(T);
	void pop ();
	void erase ();
	int size ();
	bool empty ();
	T top ();
	
	void resize(int newsize);
	
 protected:
	int iSize;
	int iElem;
	T* pElements;
};

template <class T>
inline tgStack<T>::tgStack(int size)
{
	iSize = size;
	iElem = -1;
	pElements = new T[size];
}

template <class T>
inline tgStack<T>::~tgStack ()
{
	delete [] pElements;
}

template <class T>
inline void tgStack<T>::push (T elem)
{
	if (iElem == iSize-1)
		resize(iSize+1);
	iElem++;
	pElements[iElem] = elem;
}

template <class T>
inline void tgStack<T>::pop ()
{
	iElem--;
}

template <class T>
inline T tgStack<T>::top ()
{
	if (iElem <= -1)
		return T();
	return pElements[iElem];
}

template <class T>
inline void tgStack<T>::resize(int newsize)
{
	T* pTemp = new T[newsize];
	for(int i=0; i<iSize; i++) {
		pTemp[i] = pElements[i];
	}
	delete [] pElements;
	pElements = pTemp;
}

template <class T>
inline int tgStack<T>::size()
{
	return iElem+1;
}

template <class T>
inline void tgStack<T>::erase()
{
	iElem = -1;
}

template <class T>
inline bool tgStack<T>::empty()
{
	return (size()==0);
}

#endif
