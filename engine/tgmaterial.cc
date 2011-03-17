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
#include "tgmaterial.h"
#include "tgglrenderer.h"

tgMaterial::tgMaterial(tgShader* s)
{
	bLoaded = false;
	sName = s->GetName();

	bNoPicmip = false;
	bNoMipmaps = false;

	tgvector<tgShaderProperty*> propsList;
	s->GetProperties(propsList);

	for(tgvector<tgShaderProperty*>::iterator i=propsList.begin(); i!=propsList.end(); i++) {
		tgShaderProperty* prop = *i;
		switch(prop->token) {
			case tgShaderParser::TGTS_NOPICMIP:
				bNoPicmip = true;
				break;
			case tgShaderParser::TGTS_NOMIPMAPS:
				bNoMipmaps = true;
				break;
			default:
				break;
		}
	}
	
	//on obtient la liste des blocs du shader	
	tgvector<tgShaderBlock*> blocksList;
	s->GetBlocks(blocksList);
	//on cree chaque stage
  iNumStages = (unsigned int)blocksList.size(); 
  //cout << "\tnumstages : " << iNumStages << endl;
  pStages = new _Stage*[iNumStages];
  for(unsigned int i=0; i<iNumStages; i++) {
    pStages[i] = new _Stage(blocksList[i],this);
  }
}



tgMaterial::~tgMaterial()
{
	for(unsigned int i=0; i<iNumStages; i++) {
		delete pStages[i];
	}
	delete [] pStages;
}

	
tgMaterial::_Stage::_Stage(tgShaderBlock* block,tgMaterial* p)
{
	bLoaded = false;
	pTexNames = NULL;
	pTexturesID = NULL;
	pParent = p;
  iNumTextures = 0;

	tgvector<tgShaderProperty*> propsList;
	block->GetProperties(propsList);
	//pour chaque propriete, on regarde si on a une image ou autre chose a precalculer
	for (tgvector<tgShaderProperty*>::iterator k=propsList.begin(); k!=propsList.end(); k++) {
		tgShaderProperty* prop = *k;
		switch(prop->token) {
			case tgShaderParser::TGTS_MAP: //on a une texture unique sur ce stage
        //cout << "found texture : " << prop->strings[0] << endl;
				iNumTextures = 1;
				pTexNames = new tgstring[iNumTextures];
				pTexNames[0] = "data/" + prop->strings[0];
				pTexturesID = new unsigned int[1];
				break;
			default:
				break;
		}
	}
}

tgMaterial::_Stage::~_Stage()
{
	if(pTexNames)
		delete [] pTexNames;
	if(bLoaded)
		glDeleteTextures(iNumTextures,pTexturesID);
	delete [] pTexturesID;

}

void tgMaterial::_Stage::Load()
{
  tgglGenTextures(iNumTextures,pTexturesID);
	for(unsigned int i=0; i<iNumTextures; i++) {
		tgImage* img = new tgImage(pTexNames[i]);
		if(*img) {
      cout << "Loaded texture : " << pTexNames[i] << endl;
			tgglBindTexture(GL_TEXTURE_2D,pTexturesID[i]);
			if(pParent->NoPicmip()) { //on garde une qualite maximum sur la texture
				tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			} else {
				tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			}

			if(pParent->NoMipmaps()) {
				tgglTexImage2D(GL_TEXTURE_2D,0,img->GetColorMode(),img->GetWidth(), img->GetHeight(),0,img->GetColorMode(),GL_UNSIGNED_BYTE,img->GetPixels());
			} else {
				tggluBuild2DMipmaps(GL_TEXTURE_2D,img->GetColorMode(), img->GetWidth(), img->GetHeight(), img->GetColorMode(), GL_UNSIGNED_BYTE, img->GetPixels());
			}
			
			if(tgGLRenderer::GLHasError(pTexNames[i].data())) {
        delete [] pTexturesID;
        iNumTextures = 0;
        delete img;
        break;
			}
			delete img;
		} else {
      delete [] pTexturesID;
      iNumTextures = 0;
			cout << "Erreur lors du chargement de " << pTexNames[i] << endl;
			delete img;
      break;
		}
	}
	bLoaded = true;
}

void tgMaterial::_Stage::Bind()
{
  if(iNumTextures > 0 && pTexturesID && pTexturesID[0] != -1) {
  	tgglEnable(GL_TEXTURE_2D);
  	tgglBindTexture(GL_TEXTURE_2D,pTexturesID[0]);
  } else {
    tgglDisable(GL_TEXTURE_2D);
  }
}

void tgMaterial::Load ()
{
	if(bLoaded)
		return;
	for(unsigned short i=0; i<iNumStages; i++) {
		pStages[i]->Load();	
	}
	bLoaded = true;
}

void tgMaterial::UnLoad()
{
	//TODO: implement
}

void tgMaterial::BindStage(unsigned short stage)
{
  if (!pStages[stage]->bLoaded) {
    tgglDisable(GL_TEXTURE_2D);
    return;
  }
	pStages[stage]->Bind();
}

