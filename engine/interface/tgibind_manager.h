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
#ifndef TGIBIND_MANAGER_H
#define TGIBIND_MANAGER_H

#include "tgfunctor.h"
#include "tgkeys.h"

/**
* Gère les bind (association d'une touche du clavier à une action)
* Maximum une action (Callback via Functor) par touche
*/
class tgIBindManager
{
 public:
	virtual ~tgIBindManager () {}
	/**
	* Associe une touche a une fonction
	* @param key la touche en question
	* @param func un functor obtenu par appel a tgMakeFunctor
	*/
	virtual void Bind(tgKey key, const tgFunctor1<int>& func) = 0;
	
	/**
	 * Associe un bouton de souris a une fonction
	 * @param button le numero du boutton
	 * @param func un functor obtenu par appel a tgMakeFunctor
	 */
	virtual void BindMouseClick(int button, const tgFunctor1<int>& func) = 0;
	
	/**
	* Appelle la fonction associee a la touche
	* @param key la touche en question
	* @param state l'état de la touche (1 = enfoncée, 0 = relachée)
	*/
	virtual void Call(tgKey key, int state) = 0;

	/**
	 * Appelle la fonction associe au bouton
	 * @param button le numero du boutton
	 * @param state l'etat du bouton (1=enfonce, 0=relache)
	 */
	virtual void CallMouseClick(int button, int state) = 0;
	
	/**
	* Permet d'associer une callback générale qui sera appelée lorsque n'importe quelle touche est pressée, pour autant 
	* que le BindManager soit en mode "UniqueCallback" (pour récupérer les entrées pour la console par exemple)
	* @param func un functor obtenu par appel a tgMakeFunctor
	*/
	virtual void SetUniqueCallback (const tgFunctor2<tgKey, int>& func) = 0;
	
	/**
	* Passe en mode Unique -> quand une touche est pressée, la fonction associée par SetUniqueCallback est appelée
	* @see SetUniqueCallback()
	*/
	virtual void SwitchToUniqueCallback () = 0;
	
	/**
	* Passe en mode Multiple -> quand une touche est pressée, les fonction associées par Bind sont appelées
	* @see Bind()
	*/
	virtual void SwitchToMultipleCallback () = 0;
	
	/**
	* Associe une fonction aux mouvements de la souris
	* @param func un functor obtenu par appel a tgMakeFunctor
	*/
	virtual void BindMouseMotion(const tgFunctor2<int,int>& func) = 0;
	
	
	/**
	* Appelle la fonction associée aux mouvements de la souris
	* @param rx relative x motion
	* @param ry relative y motion
	*/ 
	virtual void CallMouseMotion(int rx, int ry) = 0;
};

#endif
 
