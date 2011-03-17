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

/*
 * This class implements shaders
 */

#ifndef TGSHADER_H
#define TGSHADER_H

#include "tgdefs.h"

#include "tgshader_property.h"

class tgShaderBlock
{
	public:
		tgShaderBlock		();
		virtual ~tgShaderBlock	();

		void AddProperty	(tgShaderProperty *p);

		void SetLoaded		(bool l) { isLoaded = l; }
		void SetHasError	(bool e) { hasError = e; }
//		void SetError		(int er) { error = er; }

		void GetProperties	(vector<tgShaderProperty*>& vec);
		tgShaderProperty* GetProperty(int property);

		friend inline std::ostream& operator << (std::ostream& output, const tgShaderBlock& p)
		{
			output << "    (tgShaderBlock: ";
			if (p.hasError) {
				output << " - HAS ERRORS" << endl;
			} else {
				output << endl;
			}
			for(unsigned int i = 0; i < p.properties.size(); ++i) {
				output << *p.properties[i];
			}
			output << "    )" << endl;
			return output;
		}

	protected:
		/* information */
		bool hasError;
		bool isLoaded;
//		unsigned int error;

		/* shader's properties */
		vector<tgShaderProperty*> properties;
};

class tgShader : public tgShaderBlock
{

	public:
		tgShader		();
		virtual ~tgShader	();

		void SetName		(tgstring s);
		string GetName		();

		void AddBlock		(tgShaderBlock* b);
		tgShaderBlock* GetBlock (int i);
		void GetBlocks		(vector<tgShaderBlock*>& blocks);
		
		friend inline std::ostream& operator << (std::ostream& output, const tgShader& p)
		{
			output << "(tgShader:" << p.name;
			if (p.hasError) {
				output << " - HAS ERRORS" << endl;
			} else {
				output << endl;
			}
			for(unsigned int i = 0; i < p.properties.size(); ++i) {
				output << *p.properties[i];
			}
			for(unsigned int i = 0; i < p.blocks.size(); ++i) {
				output << *p.blocks[i];
			}
			output << ")" << endl;
			return output;
		}

	protected:
		vector<tgShaderBlock*> blocks;

		/* shader's name */
		tgstring name;

	private:

};


#endif

