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

#include "tgshader_property.h"

tgShaderProperty::tgShaderProperty(/*tgShaderParser::tgShaderToken*/int t) :
	token(t), floats(), strings(), keywords()
{
}

void tgShaderProperty::AddFloat (float f)
{
	floats.push_back(f);
}

void tgShaderProperty::AddString (tgstring s)
{
	strings.push_back(s);
}

void tgShaderProperty::AddKeyword (/*tgShaderParser::tgShaderToken*/int k)
{
	keywords.push_back(k);
}

tgShaderProperty::~tgShaderProperty()
{
}

#if 0
tgShaderProperty::tgShaderProperty(/*tgShaderParser::tgShaderToken*/int t, PropertyType p) :
	t(p), token(t)
{
}


tgShaderProperty::~tgShaderProperty()
{
}

/*----------------------- tgShaderProperty_S ----------------------*/

tgShaderProperty_S::tgShaderProperty_S(/*tgShaderParser::tgShaderToken*/int t, tgstring s)
	: tgShaderProperty(t, S), _1(s)
{
}

tgShaderProperty_S::~tgShaderProperty_S()
{
}

/*----------------------- tgShaderProperty_K ----------------------*/

tgShaderProperty_K::tgShaderProperty_K(/*tgShaderParser::tgShaderToken*/int t,
				       /*tgShaderParser::tgShaderToken*/int  p)
	: tgShaderProperty(t, K), _1(p)
{
}

tgShaderProperty_K::~tgShaderProperty_K()
{
}
#endif

