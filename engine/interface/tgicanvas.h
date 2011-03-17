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
#ifndef TGICANVAS_H
#define TGICANVAS_H

#include "tgimaterial.h"
#include "tglib.h"

struct tgColor
{
	tgColor() { Set(1,1,1,1); }
	tgColor(float pr, float pg, float pb, float pa=1.0f) { Set(pr,pg,pb,pa); }
	void Set(float pr, float pg, float pb, float  pa=1.0f) { r=pr; g=pg; b=pb; a=pa; }
	float r,g,b,a;
};

/**
* Classe gérant un canevas sur lequel il est possible d'afficher du texte/images
*/
class tgICanvas
{
 public:
	 virtual ~tgICanvas() {}
 	/**
 	* Affiche du texte à l'écran
 	* @param tgstring le texte qui sera affiché
 	* @param CR un booléen qui dit si il faudra aller à la ligne après l'affichage du texte (par défaut, true)
 	*/
	 virtual void DisplayText (const tgstring& str, bool CR=true) = 0;
	
	/**
	* Affiche une partie d'un material à l'écran
	* @param tgIMaterial* un pointeur sur le material qui va être affiché
	* @param float la longueur du carré dans lequel on va afficher le material
	* @param float la hauteur du carré dans lequel on va afficher le material
	* @param float la position en X de la partie du material à rendre
	* @param float la position en Y de la partie du material à rendre
	* @param float la position en X de la fin de la partie du material à rendre
	* @param float la position en Y de la fin de la partie du material à rendre
	*/
	virtual void DrawTile (tgIMaterial* mat, float XL, float YL, float U, float V, float UL, float VL) = 0;
	
	/**
	* Change l'origine du Canvans. Dans la majorité des cas (0,0) convient très bien
	*/
	virtual void SetOrigin(float X, float Y) = 0;
	
	/**
	* Change la position du dessin du prochain élément
	*/
	virtual void SetPosition(float X, float Y) = 0;
	
	/**
	* Change la couleur du dessin
	*/
	virtual void SetDrawColor (const tgColor& c) = 0;
	
	/**
	* Change la couleur de fond du dessin
	*/
	virtual void SetBackgroundColor (const tgColor& c) = 0;
	
	/**
	* Retourne la couleur en cours pour le fond du dessin
	*/
	virtual tgColor GetBackgroundColor () = 0;
	
	/**
	* Retourne la couleur en cours pour le dessin
	*/
	virtual tgColor GetDrawColor () = 0;
};

#endif
