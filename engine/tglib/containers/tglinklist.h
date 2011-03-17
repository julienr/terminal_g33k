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

#ifndef TGLINKLIST_H
#define TGLINKLIST_H

template <class type>
class tgLinkList 
{
 public:
	tgLinkList ();
	tgLinkList (type* o);
	~tgLinkList ();


	bool IsListEmpty() const;
	/**
	 * Ajoute ce node-ci avant le node passé en parametre
	 */
	void InsertBefore (tgLinkList& node);

	/**
	 * Ajoute ce node-ci après le node passé en parametre
	 */
	void InsertAfter (tgLinkList& node);

	void AddToEnd (tgLinkList& node);
	void AddToFront (tgLinkList& node);

	/**
	 * Retourne le propriétaire de ce node
	 */
	type* GetOwner () const;

	/**
	 * Change le propriétaire de ce node
	 */
	void SetOwner (type* o);

	/**
	 * Retire le node de la liste
	 */
	void Remove ();

	int Count () const;

	void Clear ();

	tgLinkList* GetHead() const;
	tgLinkList* GetNext() const;
	tgLinkList* GetPrev() const;
 protected:
	tgLinkList* pHead;
	tgLinkList* pNext;
	tgLinkList* pPrev;
	type* pOwner;
};

template <class type>
tgLinkList<type>::tgLinkList()
{
	pHead = this;
	pNext = this;
	pPrev = this;
	pOwner = NULL;
}

template <class type>
tgLinkList<type>::tgLinkList(type* o)
{
	pHead = this;
	pNext = this;
	pPrev = this;
	pOwner = o;
}

template <class type>
tgLinkList<type>::~tgLinkList ()
{
	Clear ();
}

template <class type>
bool tgLinkList<type>::IsListEmpty() const {
	return pHead->pNext = pHead;
}

template <class type>
void tgLinkList<type>::InsertBefore(tgLinkList& node)
{
	pNext = &node;
	pPrev = node.pPrev;
	node.pPrev = this;
	pPrev->pNext = this;
	pHead = node.pHead;
}

template <class type>
void tgLinkList<type>::InsertAfter(tgLinkList& node)
{
	pPrev = node;
	pNext = node.pNext;
	pNext->pPrev = this;
	node.pNext = this;
	pHead = node.pHead;
}

template <class type>
void tgLinkList<type>::AddToEnd(tgLinkList& node)
{
	InsertBefore(*node.pHead);
}

template <class type>
void tgLinkList<type>::AddToFront(tgLinkList& node)
{
	InsertAfter(*node.pHead);
}

template <class type>
void tgLinkList<type>::Remove ()
{
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;

	pNext = this;
	pPrev = this;
	pHead = this;
}

template <class type>
void tgLinkList<type>::Clear ()
{
	if(pHead == this) {
		while(pNext != this) {
			pNext->Remove();
		}
	} else {
		Remove ();
	}
}

template <class type>
type* tgLinkList<type>::GetOwner() const
{
	return pOwner;
}

template <class type>
void tgLinkList<type>::SetOwner(type* o)
{
	pOwner = o;
}

template <class type>
tgLinkList<type>* tgLinkList<type>::GetHead() const
{
	return pHead;
}

template <class type>
tgLinkList<type>* tgLinkList<type>::GetNext() const
{
	return pNext;
}

template <class type>
tgLinkList<type>* tgLinkList<type>::GetPrev() const
{
	return pPrev;
}


template <class type>
int tgLinkList<type>::Count () const
{
	tgLinkList<type>* node;
	int num = 1;
	for (node=pHead->pNext; node!=pHead; node=node->pNext) {
		num++;
	}
	return num;
}


#endif

