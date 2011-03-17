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

#ifndef TG_SHADER_PROPERTY_H
#define TG_SHADER_PROPERTY_H
//#include "defs.h"
#include "tglib.h"
#include "tgshader_parser.h"
//class tgShaderParser;
//class tgShaderParser;

class tgShaderProperty
{
	public:
		tgShaderProperty(/*tgShaderParser::tgShaderToken*/int t);
		virtual ~tgShaderProperty();
		
		/*tgShaderParser::tgShaderToken*/int token;

		void 	AddFloat 	(float f);
		void 	AddString 	(tgstring s);
		void 	AddKeyword 	(/*tgShaderParser::tgShaderToken*/int k);
		
		vector<float> 		floats;
		vector<tgstring> 	strings;
		vector</*tgShaderParser::tgShaderToken*/int> 		keywords;
		
		
		friend inline std::ostream& operator << (std::ostream& output, const tgShaderProperty& p)
		{
			output << "        (tgShaderProperty: " << tgShaderParser::stringShaderTks[p.token] << endl;
			if(p.floats.size()>0) {
				output << "            floats: (";
				for(unsigned int i = 0; i < p.floats.size(); ++i) {
//					output << "                " << p.floats[i] << endl;
					output  << p.floats[i] << ", ";
				}
				output << p.floats[p.floats.size()-1] << ")" << endl;
			}
			
			if(p.strings.size()>0) {
				output << "            strings:" << endl;
				for(unsigned int i = 0; i < p.strings.size(); ++i) {
					output << "                " << p.strings[i] << endl;
				}
			}

			if(p.keywords.size()>0) {
				output << "            keywords:" << endl;
				for(unsigned int i = 0; i < p.keywords.size(); ++i) {
					output << "                " << tgShaderParser::stringShaderTks[p.keywords[i]] << endl;
				}
			}

			output << "        )" << endl;
			return output; 
		}


};

#if 0
class tgShaderProperty
{
	public:
		enum PropertyType {
			S,
			K
		};

		tgShaderProperty(/*tgShaderParser::tgShaderToken*/int t, PropertyType t);
		virtual ~tgShaderProperty();

		PropertyType t;
		/*tgShaderParser::tgShaderToken*/int token;

};

class tgShaderProperty_S : public tgShaderProperty
{
	public:
		tgShaderProperty_S(/*tgShaderParser::tgShaderToken*/int t, tgstring s);
		virtual ~tgShaderProperty_S();

		tgstring _1;
};

class tgShaderProperty_K : public tgShaderProperty
{
	public:
		tgShaderProperty_K(/*tgShaderParser::tgShaderToken*/int t,
				   /*tgShaderParser::tgShaderToken*/int p);
		virtual ~tgShaderProperty_K();

		int _1;
};
#endif

#endif

