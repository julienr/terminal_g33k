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
#ifndef TGCANVAS_H
#define TGCANVAS_H

#include "tgdefs.h"
#include "tgmaterial.h"
#include "tgicanvas.h"


struct tgFont
{
	tgMaterial* pMat;
	int dListId;
};

class tgCanvas : public tgICanvas
{
	friend class tgGLRenderer;
 public:
 	tgCanvas ();
 	~tgCanvas ();
 
 	/**
 	* @see tgICanvas::DisplayText
 	*/
	void DisplayText(const tgstring& str, bool CR=true);
	
	/**
	* @see tgICanvas::DrawTile
	*/
	void DrawTile (tgIMaterial* mat, float XL, float YL, float U, float V, float UL, float VL);
	
	/**
	* @see tgICanvas::SetOrigin
	*/
	void SetOrigin(float X, float Y);
	
	/**
	* @see tgICanvas::SetPosition
	*/
	void SetPosition(float X, float Y);
	
	/**
	* @see tgICanvas::SetDrawColor
	*/
	void SetDrawColor (const tgColor& c);
	
	/**
	* @see tgICanvas::GetDrawColor
	*/
	tgColor GetDrawColor ();
	
	/**
	* @see tgICanvas::SetBackgroundColor
	*/
	void SetBackgroundColor (const tgColor& c);
	
	/**
	* @see tgICanvas::GetBackgroundColor
	*/
	tgColor GetBackgroundColor ();
	
	/**
	* Change la police utilisée pour le rendu du texte
	*/
	void SetFont(const tgstring& name);
 protected:
 	/**
 	* Efface tout TextToDraw.
 	* Appelée à la fin du rendu par tgGLRenderer
 	*/
 	void _Clear ();
 
	struct Text
	{
		tgstring str;
		float posX, posY;
		tgColor FGcolor;
		tgColor BGcolor;
		bool CR;
	};
	
	//l'origin ou on va commencer à dessiner
	float fOriginX, fOriginY;
	//la positiona actuelle
	float fCurrentX, fCurrentY;
	tgColor DrawColor;
	tgColor BackgroundColor;
	//le texte à dessiner au prochain render
	tgvector <Text> TextToDraw;
	//la police utilisée pour dessiner le texte
	tgFont drawFont;
	//la police a-t-elle été chargée correctement ?
	bool bFontLoaded;
};

#endif
