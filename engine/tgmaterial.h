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
#ifndef TGMATERIAL_H
#define TGMATERIAL_H

#include "tgdefs.h"
#include "tgimaterial.h"
#include "tgshader.h"
#include "gl/tggl.h"
#include "tgimage.h"

class tgShader;
class tgShaderBlock;

class tgMaterial : public tgIMaterial
{
 friend class tgGLRenderer;
 public:
	tgMaterial(tgShader* s);
	virtual ~tgMaterial ();
	
	/**
	* Un material ne va pas charger les textures si la map n'en a pas besoin. Cette fonction permet de dire à ce
	* material de charger ses textures
	*/
	void Load ();
	
	/**
	* Permet de savoir si la texture du material a été chargée. 
	* Appelée juste après un Load(), cette fonction permet de savoir si ledit Load() a réussi à charger la texture.
	*/
	inline bool Loaded () { return bLoaded; }
	
	/**
	* @see tgMaterial::Load
	*/
	void UnLoad ();

	/**
	 * retourne le nombre de stages que contient le material, pour chaque stage, le renderer doit rendre la 
	 * geometrie sur laquelle est appique le material
	 * un material contient au minimum 1 stage
	 */
	unsigned short GetNumStages() { return iNumStages; }
	
	/**
	 * bind le 'stage' donne en parametre
	 */
	void BindStage(unsigned short stage);

	tgstring GetName() { return sName; }

	bool NoPicmip() { return bNoPicmip; }
	bool NoMipmaps() { return bNoMipmaps; }

 protected:
	class _Stage
	{
		public:
			_Stage(tgShaderBlock*,tgMaterial*);
			~_Stage();
			void Load();
			void Bind();
			tgstring* pTexNames;
			unsigned int* pTexturesID;
			unsigned int iNumTextures;
			bool bLoaded;

			tgMaterial* pParent;
	};

	tgstring sName;
	void _LoadFromFile(int file);
	_Stage** pStages;
	//la fonction Load de ce material a-t-elle déjà été appelée
	bool bLoaded;
	unsigned int iNumStages;


	//shader properties
	bool bNoPicmip;
	bool bNoMipmaps;

	
};

#endif
