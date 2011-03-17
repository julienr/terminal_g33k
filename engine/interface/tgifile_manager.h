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
#ifndef TGIFILE_MANAGER_H
#define TGIFILE_MANAGER_H

#include <string>
#include "tglib.h"

enum tgFileType {
	TGFILE_BINARY,
	TGFILE_TEXT
};

/**
* Abstraction du syst�me de fichier.
*/
class tgIFileManager
{
 public:
	virtual ~tgIFileManager() {}
	/**
	* Ouvre un fichier et retourne son identificateur
	* @param filename le nom du fichier, ainsi que son chemin d'acc�s
	* @param type le type d'interpr�tation des donn�es du fichiers (binaire/texte)
	* @return un identificateur unique (utilis� par Read/Write), -1 en cas d'erreur
	*/
	virtual int Open (const char* filename, tgFileType type) = 0;
	
	/**
	* Ferme le fichier
	* @param iD l'identificateur retourn� par Open
	*/
	virtual void Close (int iD) = 0;
	
	/**
	* Permet de savoir si un fichier existe
	* @param filename le nom dudit fichier
	* @return vrai si le fichier existe
	*/
	virtual bool Exist (const char* filename) = 0;
	
	/**
	* Est-on � la fin du fichier
	* @return vrai si on est a la fin du fichier
	*/
	virtual bool EndOfFile (int iD) = 0;
	
	/**
	* Clear enl�ve les flags du stream. Permet notamment de r�utiliser seekg apr�s un eof
	*/
	virtual void Clear (int iD) = 0;
	
	/**
	* Lis depuis le fichier dont l'identificateur est donn�e en param�tre (TGFILE_BINARY)
	* @param iD l'identificateur retourn� par Open
	* @param ptr un pointeur ou on mettra les donn�es lues
	* @param size la taille (en octets) � lire
	*/
	virtual void ReadBinary(int iD, char* ptr, int size) = 0;

  /**
  * Donne l'offset du pointeur de lecture depuis le d�but du fichier
  * @param iD l'identificateur retourn� par Open
  * @return l'offset depuis le d�but du fichier
  */
  virtual int GetReadOffset(int iD) = 0;

	/**
	* Permet de se d�placer dans un fichier (TGFILE_BINARY)
	* @param iD l'identificateur retourn� par Open
	* @param pos la nouvelle position
	*/
	virtual void Seek (int iD, int pos) = 0;
	
	/**
	* Permet de se d�placer dans un fichier (TGFILE_BINARY) depuis la position courante
	* @param iD l'identificateur retourn� par Open
	* @param pos la nouvelle position
	*/
	virtual void SeekFromCurrent (int iD, int pos) = 0;
	
	/**
	* Lis une cha�ne de caract�re (TGFILE_TEXT)
	* @param iD l'identificateur retourn� par Open
	* @param buffer la cha�ne ou on mettra les donn�es lues
	*/
	virtual void Read(int iD, std::string& buffer) = 0;
	
	/**
	* Lis un float (TGFILE_TEXT)
	* @param iD l'identificateur retourn� par Open
	* @param f le float ou on mettra les donn�es lues
	*/
	virtual void Read(int iD, float& f) = 0;
	
	/**
	* Lis un int (TGFILE_TEXT)
	* @param iD l'identificateur retourn� par Open
	* @param i l'int ou on mettra les donn�es lues
	*/
	virtual void Read(int iD, int& i) = 0;
	
	/**
	* Lis un int (TGFILE_TEXT)
	* @param iD l'identificateur retourn� par Open
	* @param i l'int ou on mettra les donn�es lues
	*/
	virtual void Read(int iD, char& c) = 0;

	/**
	* Lis un unsigned int (TGFILE_TEXT)
	* @param iD l'identificateur retourn� par Open
	* @param i l'unsigned int ou on mettra les donn�es lues
	*/
	virtual void Read(int iD, unsigned int& i) = 0;
	
	/**
	* Lis un char NON-FORMATE (lis meme les espaces et les \n)!
	* @param iD l'identificateur retourn� par Open
	* @param c le char ou on mettra les donn�es lues
	*/
	virtual void Get(int iD, char& c) = 0;
	
	/**
	* Lis un vector3
	* @param iD l'identificateur retourn� par Open
	* @param v le tgVector3 ou seront stock�s les donn�es lues
	*/
	virtual void Read(int iD, tgVector3& v) = 0;
};

#endif

