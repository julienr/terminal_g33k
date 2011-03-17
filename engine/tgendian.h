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
#ifndef TGENDIAN_H
#define TGENDIAN_H

/**
* tgEndian : Gestion de la conversion little endian/big endian
*/
class tgEndian
{
 public:
 	virtual ~tgEndian() {}
	virtual short Short (short l) = 0;
	virtual int Long (int l) = 0;
	virtual float Float (float f) = 0;
	bool bLittle;
};

/**
* tgLittleEndian : conversion pour les architectures x86. Les .bmp et .bsp sont en littleendian -> pas de conversion
*/
class tgLittleEndian : public tgEndian
{
 public:
	tgLittleEndian () { bLittle=true; }
	short Short (short l) { return l; }
	int Long (int l) { return l; }
	float Float (float f) { return f; }

};

/**
* tgBigEndian: conversion pour les architectures MAC. Les .bmp et .bsp sont en littleendian -> conversion
*/
class tgBigEndian : public tgEndian
{
 public:
	tgBigEndian () { bLittle=false; }
	short Short (short l) 
	{ 
		unsigned char b1, b2;
		b1 = l&255;
		b2 = (l>>8)&255;
		return (b1<<8) + b2;
	}
	
	int Long (int l)
	{
		unsigned char b1,b2,b3,b4;
		b1=l&255;
		b2=(l>>8)&255;
		b3=(l>>16)&255;
		b4=(l>>24)&255;
		return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
	}
	
	float Float (float f)
	{
		union
		{
			float f;
			unsigned char b[4];
		}dat1, dat2;
		dat1.f = f;
		dat2.b[0] = dat1.b[3];
		dat2.b[1] = dat1.b[2];
		dat2.b[2] = dat1.b[1];
		dat2.b[3] = dat1.b[0];
		return dat2.f;
	}
	
};

#endif
