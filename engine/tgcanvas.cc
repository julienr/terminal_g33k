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
#include "tgcanvas.h"
#include "tgglrenderer.h"

tgCanvas::tgCanvas ()
{
	fOriginX = fOriginY = fCurrentX = fCurrentY = 0.0f;
	DrawColor.Set(1,1,1,1);
	BackgroundColor.Set(0,0,0,0.2f);
	bFontLoaded = false;
}

tgCanvas::~tgCanvas()
{
	pGLRenderer->ReleaseFont(&drawFont);
}

void tgCanvas::SetFont(const tgstring& str)
{
	if(bFontLoaded) {//si on avait deja une font charge
		pGLRenderer->ReleaseFont(&drawFont);
	}
	pGLRenderer->LoadFont(&drawFont, str);
	bFontLoaded = true;
}

void tgCanvas::DisplayText(const tgstring& str, bool CR)
{
	if (!bFontLoaded)
		tgout << "Probleme dans le chargement de la police" << endl;
		
	Text txt;
	txt.str = str;
	txt.posX = fOriginX + fCurrentX;
	txt.posY = fOriginY + fCurrentY;
	txt.FGcolor.Set(DrawColor.r, DrawColor.g, DrawColor.b, DrawColor.a);
	txt.BGcolor.Set(BackgroundColor.r,BackgroundColor.g, BackgroundColor.b, BackgroundColor.a);
	txt.CR = CR;
	TextToDraw.push_back(txt);
}

void tgCanvas::DrawTile (tgIMaterial* mat, float XL, float YL, float U, float V, float UL, float VL)
{

}

void tgCanvas::SetOrigin(float X, float Y)
{
	fOriginX = X;
	fOriginY = Y;
}

void tgCanvas::SetPosition(float X, float Y)
{
	fCurrentX = X;
	fCurrentY = Y;
}

void tgCanvas::SetDrawColor (const tgColor& c)
{
	DrawColor.Set(c.r, c.g, c.b, c.a);
}

tgColor tgCanvas::GetDrawColor ()
{
	return DrawColor;
}

tgColor tgCanvas::GetBackgroundColor ()
{
	return BackgroundColor;
}

void tgCanvas::SetBackgroundColor (const tgColor& c)
{
	BackgroundColor.Set(c.r, c.g, c.b, c.a);
}

void tgCanvas::_Clear ()
{
	TextToDraw.clear();
}
