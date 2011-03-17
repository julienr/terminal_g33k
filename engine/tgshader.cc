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

#include "tgshader.h"

tgShaderBlock::tgShaderBlock() //: properties()
	: properties()
{
	hasError	= false;
	isLoaded	= false;
//	error		= 0;
}

tgShaderBlock::~tgShaderBlock()
{
	vector<tgShaderProperty*>::iterator i;
	for (i=properties.begin(); i != properties.end();) {
		delete *i;
		i = properties.erase(i);
	}
}

void tgShaderBlock::AddProperty(tgShaderProperty *p)
{
	properties.push_back(p);
//	cout << "ok, property added" << endl;
}


void tgShaderBlock::GetProperties(vector<tgShaderProperty*>& vec)
{
	vec = properties;
}

tgShaderProperty* tgShaderBlock::GetProperty(int property)
{
	vector<tgShaderProperty*>::iterator i;
	for (i=properties.begin(); i != properties.end();i++) {
		if ((*i)->token == property)
			return *i;
	}
	return NULL;
}

/*****************************************************/

tgShader::tgShader() //: properties()
	: blocks()
{
}

tgShader::~tgShader()
{
	vector<tgShaderBlock*>::iterator i;
	for (i=blocks.begin(); i != blocks.end();) {
		delete *i;
		i = blocks.erase(i);
	}
}

void tgShader::SetName(tgstring s)
{
	name = s;
}

string tgShader::GetName()
{
	return name;
}

void tgShader::AddBlock(tgShaderBlock* b)
{
	blocks.push_back(b);
}

tgShaderBlock* tgShader::GetBlock(int i)
{
	return blocks[i];
}

void tgShader::GetBlocks(vector<tgShaderBlock*>& vec)
{
	vec = blocks;
}


