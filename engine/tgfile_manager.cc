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
#include "tgfile_manager.h"

tgFileManager::tgFileManager ()
{
	/*for(int i=0; i<MAX_STREAMS; i++) {
		pStreams[i] = NULL;
	}*/
}

tgFileManager::~tgFileManager ()
{
}

int tgFileManager::Open (const char* filename, tgFileType type)
{
	for(int i=0;i<MAX_STREAMS;i++) {
		if (/*pStreams[i] == NULL*/!pStreams[i].is_open()) {//stream inutilisé
			if (type == TGFILE_BINARY)
				//pStreams[i] = fopen(filename, "rb");
				pStreams[i].open(filename,ios::in | ios::binary);
			else if (type == TGFILE_TEXT)
				//pStreams[i] = fopen(filename, "rt");
				pStreams[i].open(filename,ios::in);
			else {
				tgerr << "type de fichier inconnu:" << type << endl;
				return -1;
			}
			if (/*!pStreams[i]*/pStreams[i].fail()) {
				tgerr << "impossible d'ouvrir " << filename << endl;
				//pStreams[i].close();
				//fclose(pStreams[i]);
				return -1;
			}
			return i;
		}
	}
	tgerr << "tout les flux sont utilisés" << endl;
	return -1;
}

void tgFileManager::Close (int iD)
{
	if (!_ValidateID(iD)) return;
	
	if (/*pStreams[iD] != NULL*/pStreams[iD].is_open()) {
		pStreams[iD].close ();
		//fclose(pStreams[iD]);
		//pStreams[iD] = NULL;
	}
	else {
		tgerr << "flux iD non-utilisé : " << iD << endl;
	}
}

bool tgFileManager::Exist (const char* filename)
{
	struct stat mystat;
	return(stat(filename, &mystat)==0);
/*	FILE* f = fopen (filename, "rb");
	if (f == NULL) {
		return false;
	}
	else {
		fclose(f);
		return true;
	}*/
}

bool tgFileManager::EndOfFile (int iD)
{
	if (!_ValidateStream(iD)) return true;
	return pStreams[iD].eof ();
	//return (feof(pStreams[iD]) == 0)?false:true;
}

void tgFileManager::Clear (int iD)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD].clear();
	//clearerr(pStreams[iD]);
}

void tgFileManager::ReadBinary(int iD, char* ptr, int size)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD].read(ptr, size);
	//fread(ptr, size, 1, pStreams[iD]);
}

int tgFileManager::GetReadOffset(int iD)
{
  if (!_ValidateStream(iD)) return 0;
  return pStreams[iD].tellg();
}

void tgFileManager::Seek (int iD, int pos)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD].seekg(pos);
	//fseek(pStreams[iD], pos, SEEK_SET);
}

void tgFileManager::SeekFromCurrent (int iD, int pos)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD].seekg(pos, ios::cur);
	//fseek(pStreams[iD], pos, SEEK_CUR);
}

void tgFileManager::Read(int iD, tgstring& buffer)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> buffer;
	/*char buf[2048];
	memset(buf,'\0',2048);
	fscanf(pStreams[iD], "%s", buf);
	buffer = tgstring(buf);*/
}

void tgFileManager::Read(int iD, float& f)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> f;
	//fscanf(pStreams[iD], "%f", &f);
}

void tgFileManager::Read(int iD, char& c)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> c;
	//fscanf(pStreams[iD], "%c", &c);
}

void tgFileManager::Read(int iD, int& i)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> i;
	//fscanf(pStreams[iD], "%i", &i);
}

void tgFileManager::Read(int iD, unsigned int& i)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> i;
	//fscanf(pStreams[iD], "%i", &i);
}

void tgFileManager::Read(int iD, tgVector3& v)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD] >> v.x >> v.y >> v.z;
	//fscanf(pStreams[iD], "%f %f %f", &v.x, &v.y, &v.z);
}

void tgFileManager::Get(int iD, char& c)
{
	if (!_ValidateStream(iD)) return;
	pStreams[iD].get(c);
	//fscanf(pStreams[iD], "%c", &c);
}

bool tgFileManager::_ValidateID (int iD)
{
	if (!((iD>=0) && (iD<MAX_STREAMS))) {
		tgerr << "iD hors limites : " << iD << endl;
		return false;
	}
	return true;
}

bool tgFileManager::_ValidateStream (int iD)
{
	if (!_ValidateID(iD)) return false;
	if (!pStreams[iD].is_open()/*pStreams[iD] == NULL*/) {
		tgerr << "flux non-ouvert : " << iD << endl;
		return false;
	}
	return true;
}
