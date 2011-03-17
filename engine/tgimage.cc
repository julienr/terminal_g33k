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
#include "tgimage.h"
#include <assert.h>

Uint32 getPixel (SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    assert(bpp>=1 && bpp<=4);
    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putPixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color)
{
	/* Nombre de bits par pixels de la surface d'écran */
	Uint8 bpp = surface->format->BytesPerPixel;
	/* Pointeur vers le pixel à remplacer (pitch correspond à la taille 
	   d'une ligne d'écran, c'est à dire (longueur * bitsParPixel) 
	   pour la plupart des cas) */
	Uint8 * p = ((Uint8 *)surface->pixels) + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = (Uint8) color;
			break;
		case 2:
			*(Uint16 *)p = (Uint16) color;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				*(Uint16 *)p = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
				*(p + 2) = color & 0xff;
			}
			else
			{
				*(Uint16 *)p = color & 0xffff;
				*(p + 2) = ((color >> 16) & 0xff) ;
			}
			break;
		case 4:
			*(Uint32 *)p = color;
			break;
	}
}


tgImage::tgImage(const tgstring& filename)
{
	data = NULL;
	//special white material
	if (filename.find("*white") != tgstring::npos) {
		w = 16;
		h = 16;
		type = GL_RGBA;
		data = new unsigned char[w*h*4]; //generate a RGBA 16*16 white opaque texture
		for(int i=0; i<w*h*4; i++) {
			data[i] = 255;
		}
		bLoaded = true;
		return;
		
	}
	//generation d'un degrade central blanc->noir pour la lumiere
	#define LIGHT_SIZE 16
	if (filename.find("*light") != tgstring::npos) {
		int b;
		w = LIGHT_SIZE;
		h = LIGHT_SIZE;
		type = GL_RGBA;
		data = new unsigned char[LIGHT_SIZE*LIGHT_SIZE*4];
		int pos = 0;
		for(int y=0; y<h; y++) {
			for(int x=0; x<w; x++) {
				float d;
				d = (LIGHT_SIZE/2-0.5f-x)*(LIGHT_SIZE/2-0.5f-x) + 
					(LIGHT_SIZE/2-0.5f-y)*(LIGHT_SIZE/2-0.5f-y);
				b = (int)(4000/d);
				if (b>255) {
					b = 255;
				} else if (b<75) {
					b = 0;
				}
				data[pos] = b; pos++;
				data[pos] = b; pos++;
				data[pos] = b; pos++;
				data[pos] = 255; pos++;
			}
		}
		bLoaded = true;
		return;
	}

	//tga texture loading
	SDL_Surface* image = IMG_Load(filename.data());
	if(!image) {
		bLoaded = false;
		return;
	}
	bLoaded = true;
	w = image->w;
	h = image->h;
	type = GL_RGBA;
	int dim = w*h*4; // RGBA
	data = new unsigned char[dim];
	int pos = 0;
	for(int y=(h-1); y>-1; y--) {
		for(int x=0; x<w; x++) {
			Uint8 r,g,b,a;
			Uint32 color = getPixel(image,x,y);

/*			if(!alpha)
				SDL_GetRGB(color, img->format, &r,&g,&b);
			else*/
				SDL_GetRGBA(color, image->format, &r,&g,&b,&a);

			data[pos] = r; pos++;
			data[pos] = g; pos++;
			data[pos] = b; pos++;
		  data[pos] = a; pos++;
		}
	}
	SDL_FreeSurface(image);

/*	pData = NULL;
	iColorMode = 0;
	iWidth = 0;
	iHeight = 0;
	bLoaded = false;
	if (_LoadTGA(filename)) {
		bLoaded = true;
		//out << "chargement reussi" << endl;
	}
	else {
		bLoaded = false;
	}
	*/
}

tgImage::~ tgImage()
{
	if(data)
		delete [] data;	
	/*	delete [] pData;*/
}
/*
bool tgImage::_LoadTGA(const tgstring& filename)
{
	TGAHeader header;
	
	int file = pFileManager->Open(filename.data(), TGFILE_BINARY);
	if (file == -1) {
		//on a deja un message d'erreur dans tgFileManager
		return false;
	}
	uint8 tmp[18];
	pFileManager->ReadBinary(file, (char*)tmp, 18);
	
	header.IDLength = tmp[0];
	header.colorMapType = tmp[1];
	header.imageType = tmp[2];
	header.colorMapEntry = tmp[3] + tmp[4]*256;
	header.colorMapLength = tmp[5] + tmp[6]*256;
	header.colorMapSize = tmp[7];
	header.Xorigin = tmp[8] + tmp[9]*256;
	header.Yorigin = tmp[10] + tmp[1]*256;
	header.width = tmp[12] + tmp[13]*256;
	header.height = tmp[14] + tmp[15]*256;
	header.pixelDepth = tmp[16];
	header.imageDescriptor = tmp[17];
	
	if (header.imageType == 2)
		return _LoadUncompressedTGA(filename, file, header);
	else if (header.imageType == 10)
		return _LoadCompressedTGA(filename, file, header);
	else {
		err << "TGA doit etre de type 2 ou 10, type : " << header.imageType << endl;
		pFileManager->Close(file);
		return false;
	}
}

bool tgImage::_LoadUncompressedTGA(const tgstring& filename, int file, TGAHeader& header)
{
	GLuint cswap;
	
	iWidth = header.width;
	iHeight = header.height;
	iBpp = header.pixelDepth;
	if((iWidth <= 0) || (iHeight <= 0) || ((iBpp != 24) && (iBpp !=32)))
	{
		err << "Informations de texture invalides : " << (int)iBpp << endl;
		pFileManager->Close(file);
		return false;
	}
	
	if (iBpp == 24)
		iColorMode = GL_RGB;
	else
		iColorMode = GL_RGBA;
	
	unsigned int bytesPerPixel = iBpp/8;
	unsigned int imageSize = (bytesPerPixel*iWidth*iHeight);
	pData = new GLubyte[imageSize];
	
	pFileManager->ReadBinary(file, (char*)pData, imageSize);
	
	for(cswap = 0; cswap < (unsigned int)imageSize; cswap += bytesPerPixel)
		pData[cswap] ^= pData[cswap+2] ^= pData[cswap] ^= pData[cswap+2];
	
	pFileManager->Close(file);
	return true;
}

bool tgImage::_LoadCompressedTGA(const tgstring& filename, int file, TGAHeader& header)
{
	//GLuint cswap;
	
	iWidth = header.width;
	iHeight = header.height;
	iBpp = header.pixelDepth;
	
	GLuint pixelcount	= iWidth*iHeight;
	GLuint currentpixel	= 0;
	GLuint currentbyte	= 0;
	
	
	if((iWidth <= 0) || (iHeight <= 0) || ((iBpp != 24) && (iBpp !=32)))
	{
		err << "Informations de texture invalides" << endl;
		pFileManager->Close(file);
		return false;
	}
	
	if (iBpp == 24)
		iColorMode = GL_RGB;
	else
		iColorMode = GL_RGBA;
		
	unsigned int bytesPerPixel = iBpp/8;
	unsigned int imageSize = (bytesPerPixel*iWidth*iHeight);
	pData = new GLubyte[imageSize];
	GLubyte* colorbuffer  = new GLubyte[bytesPerPixel];
	
	do {
		GLubyte chunkheader = 0;
		pFileManager->ReadBinary(file, (char*)&chunkheader, 1);
		if (chunkheader == 0) {
			out << "chunkheader == 0 ! Fichier endommage ?" << endl;
			pFileManager->Close(file);
			delete [] pData;
			return false;
		}
		if (chunkheader < 128) {
			for(int counter = 0; counter<chunkheader; counter++) {
				pFileManager->ReadBinary(file, (char*)colorbuffer, bytesPerPixel);
				pData[currentbyte] = colorbuffer[2];
				pData[currentbyte+1] = colorbuffer[1];
				pData[currentbyte+2] = colorbuffer[0];
				
				if (bytesPerPixel == 4)
					pData[currentbyte+3] = colorbuffer[3];
				
				currentbyte += bytesPerPixel;
				currentpixel++;
				
				if (currentpixel>pixelcount) {
					err << "Trop de pixels lus dans " << filename << endl;
					pFileManager->Close(file);
					delete [] pData;
					return false;
				}
			}
		} else {
			pFileManager->ReadBinary(file, (char*)colorbuffer, bytesPerPixel);
			for(int counter = 0; counter<chunkheader; counter++) {
				pData[currentbyte] = colorbuffer[2];
				pData[currentbyte+1] = colorbuffer[1];
				pData[currentbyte+2] = colorbuffer[0];
				
				if (bytesPerPixel == 4)
					pData[currentbyte+3] = colorbuffer[3];
					
				currentbyte += bytesPerPixel;
				currentpixel++;
				
				if (currentpixel>pixelcount) {
					err << "Trop de pixels lus dans " << filename << endl;
					pFileManager->Close(file);
					delete [] pData;
					return false;
				}
			}
		}
	
	} while(currentpixel < pixelcount);
	pFileManager->Close(file);
	delete [] colorbuffer;
	return true;
}*/

