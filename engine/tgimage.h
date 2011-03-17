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
#ifndef TGIMAGE_H
#define TGIMAGE_H

#include "tgdefs.h"
#include "gl/tggl.h"

/**
* Classe supportant le chargement du format TGA
* Convertit _tout_ en RGBA
* 
*/
class tgImage
{
 public:
	tgImage (const tgstring& filename);
	~tgImage ();
	//unsigned short int GetAlphaDepth () { return iAlphaDepth; }
	//unsigned short int GetPixelDepth () { return iPixelDepth; }
	unsigned short GetColorMode () { return /*iColorMode;*/type; }
	unsigned int GetWidth () { return /*iWidth;*/w; }
	unsigned int GetHeight () { return /*iHeight;*/h; }
	unsigned char* GetPixels () {return /*pData;*/ data;}
	
	inline operator bool() const { return bLoaded; } 
 protected:
	/*struct TGAHeader {
		uint8	IDLength;
		uint8	colorMapType;
		uint8	imageType;
		uint16	colorMapEntry;
		uint16	colorMapLength;
		uint8	colorMapSize;
		uint16	Xorigin;
		uint16	Yorigin;
		uint16	width;
		uint16	height;
		uint8	pixelDepth;
		uint8	imageDescriptor;
	};
	
	
	//bool _LoadFromFile(const tgstring& filename);
	bool _LoadTGA(const tgstring& filename);
	bool _LoadUncompressedTGA(const tgstring& filename, int file, TGAHeader& header);
	bool _LoadCompressedTGA(const tgstring& filename, int file, TGAHeader& header);
	
	
	
	unsigned char* pData;
	//unsigned short int iAlphaDepth;
	//unsigned short int iPixelDepth;
	unsigned short iColorMode;
	unsigned int iBpp;
	unsigned int iWidth;
	unsigned int iHeight;*/
	//SDL_Surface* image;
	unsigned char* data;
	int w;
	int h;
	int type;
	bool bLoaded;
};

#endif
