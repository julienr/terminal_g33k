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
* Abstraction du système de fichier.
*/
class tgIFileManager
{
 public:
	virtual ~tgIFileManager() {}
	/**
	* Ouvre un fichier et retourne son identificateur
	* @param filename le nom du fichier, ainsi que son chemin d'accès
	* @param type le type d'interprétation des données du fichiers (binaire/texte)
	* @return un identificateur unique (utilisé par Read/Write), -1 en cas d'erreur
	*/
	virtual int Open (const char* filename, tgFileType type) = 0;
	
	/**
	* Ferme le fichier
	* @param iD l'identificateur retourné par Open
	*/
	virtual void Close (int iD) = 0;
	
	/**
	* Permet de savoir si un fichier existe
	* @param filename le nom dudit fichier
	* @return vrai si le fichier existe
	*/
	virtual bool Exist (const char* filename) = 0;
	
	/**
	* Est-on à la fin du fichier
	* @return vrai si on est a la fin du fichier
	*/
	virtual bool EndOfFile (int iD) = 0;
	
	/**
	* Clear enlève les flags du stream. Permet notamment de réutiliser seekg après un eof
	*/
	virtual void Clear (int iD) = 0;
	
	/**
	* Lis depuis le fichier dont l'identificateur est donnée en paramètre (TGFILE_BINARY)
	* @param iD l'identificateur retourné par Open
	* @param ptr un pointeur ou on mettra les données lues
	* @param size la taille (en octets) à lire
	*/
	virtual void ReadBinary(int iD, char* ptr, int size) = 0;

  /**
  * Donne l'offset du pointeur de lecture depuis le début du fichier
  * @param iD l'identificateur retourné par Open
  * @return l'offset depuis le début du fichier
  */
  virtual int GetReadOffset(int iD) = 0;

	/**
	* Permet de se déplacer dans un fichier (TGFILE_BINARY)
	* @param iD l'identificateur retourné par Open
	* @param pos la nouvelle position
	*/
	virtual void Seek (int iD, int pos) = 0;
	
	/**
	* Permet de se déplacer dans un fichier (TGFILE_BINARY) depuis la position courante
	* @param iD l'identificateur retourné par Open
	* @param pos la nouvelle position
	*/
	virtual void SeekFromCurrent (int iD, int pos) = 0;
	
	/**
	* Lis une chaîne de caractère (TGFILE_TEXT)
	* @param iD l'identificateur retourné par Open
	* @param buffer la chaîne ou on mettra les données lues
	*/
	virtual void Read(int iD, std::string& buffer) = 0;
	
	/**
	* Lis un float (TGFILE_TEXT)
	* @param iD l'identificateur retourné par Open
	* @param f le float ou on mettra les données lues
	*/
	virtual void Read(int iD, float& f) = 0;
	
	/**
	* Lis un int (TGFILE_TEXT)
	* @param iD l'identificateur retourné par Open
	* @param i l'int ou on mettra les données lues
	*/
	virtual void Read(int iD, int& i) = 0;
	
	/**
	* Lis un int (TGFILE_TEXT)
	* @param iD l'identificateur retourné par Open
	* @param i l'int ou on mettra les données lues
	*/
	virtual void Read(int iD, char& c) = 0;

	/**
	* Lis un unsigned int (TGFILE_TEXT)
	* @param iD l'identificateur retourné par Open
	* @param i l'unsigned int ou on mettra les données lues
	*/
	virtual void Read(int iD, unsigned int& i) = 0;
	
	/**
	* Lis un char NON-FORMATE (lis meme les espaces et les \n)!
	* @param iD l'identificateur retourné par Open
	* @param c le char ou on mettra les données lues
	*/
	virtual void Get(int iD, char& c) = 0;
	
	/**
	* Lis un vector3
	* @param iD l'identificateur retourné par Open
	* @param v le tgVector3 ou seront stockés les données lues
	*/
	virtual void Read(int iD, tgVector3& v) = 0;
};

#endif

