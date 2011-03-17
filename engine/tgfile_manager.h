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
#ifndef TGFILE_MANAGER_H
#define TGFILE_MANAGER_H

#include "tgifile_manager.h"
#include "tgdefs.h"

/**
* Implémentation de tgIFileManager
*/
class tgFileManager : public tgIFileManager
{
 public:
	/**
	* Constructeur par défaut
	*/
	tgFileManager ();
	
	/**
	* Destructeur par défaut
	*/
	virtual ~tgFileManager ();
	
	/**
	* @see tgIFileManager::Open
	*/
	int Open (const char* filename, tgFileType type);
	
	/**
	* @see tgIFileManager::Close
	*/
	void Close (int iD);
	
	/**
	* @see tgIFileManager::Exist
	*/
	bool Exist (const char* filename);
	
	/**
	* @see tgIFileManager::EndOfFile
	*/
	bool EndOfFile (int iD);
	
	/**
	* @see tgIFileManager::Clear
	*/
	void Clear (int iD);
	
	/**
	* @see tgIFileManager::ReadBinary
	*/
	void ReadBinary(int iD, char* ptr, int size);

  /**
  * @see tgIFileManager::GetReadOffset
  */
  int GetReadOffset(int iD);
	
	/**
	* @see tgIFileManager::Seek
	*/
	void Seek (int iD, int pos);
	
	/**
	* @see tgIFileManager::SeekFromCurrent
	*/
	void SeekFromCurrent (int iD, int pos);
	
	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, tgstring& buffer);
	
	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, float& f);
	
	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, int& i);
	
	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, char& c);

	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, unsigned int& i);
	
	/**
	* @see tgIFileManager::Read
	*/
	void Read(int iD, tgVector3& v);
	
	/**
	* @see tgIFileManager::Get
	*/
	void Get(int iD, char& c);
 protected:
	bool _ValidateID (int iD);
	bool _ValidateStream (int iD);
	
	//FILE* pStreams[MAX_STREAMS];
	std::fstream pStreams[MAX_STREAMS];
};

#endif
