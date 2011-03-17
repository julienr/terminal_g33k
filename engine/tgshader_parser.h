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

#ifndef TGSHADER_PARSER
#define TGSHADER_PARSER

#include "tgdefs.h"
#include "tgfile_manager.h"

class tgShader;
class tgShaderBlock;


/** implements the shader parser.
 * can read a file from a descriptor obtained from the file manager.
 * then Parse the file, and store each read shader in a vector.
 * WARNING: The file is not close at the end.
 */
class tgShaderParser
{
	public:
		/** create the parser for the given file descriptor */
		tgShaderParser		(int file);
		virtual ~tgShaderParser ();

		/** return the list of parsed shaders */
		void GetShaders(vector<tgShader*>& vec);

		tgShader* GetShader(string name);

		/** token that can be found during the parsing of any shader */
		enum tgShaderToken {
			TGTS_NULL = 0,
			TGTS_UNKNOWN,	// we don't knwo....	1
			TGTS_EOF,	// End Of File		2

			TGTS_LCROC,	// {			3
			TGTS_RCROC,	// }			4
			TGTS_LPAREN,
			TGTS_RPAREN,

			/*
			 * from here, they are all "subclass" of TGTS_STRING....
			 * so we can use >= TGTS_STRING to match a string...
			 */

			TGTS_STRING,	// a string which doesn't
			// match any of reserved words
			//			7

			TGTS_NUMBERI,	// int			8
			TGTS_NUMBERF,	// float		9

			//TGTS_DOLLARD,	// $
			//FIXME: I think that $ is accepted as
			//       normal string element..?

			TGTS_MINUS,	// -			10
			TGTS_SLASH,	// /			11
//			TGTS_STAR,	// *			12


			TGTS___FIRST_TOKEN, // NOT A TOKEN, JUST A MARK IN THE LIST
			////////////////////////
			// possibles keywords //
			////////////////////////
			// keywords for TGTS_CULL
			TGKS_FRONT,
			TGKS_BACK,
			TGKS_DISABLE,
			TGKS_NONE,

			//keywords for TGTS_DEFORMVERTEXES
			TGKS_WAVE,
			TGKS_NORMAL,
			TGKS_BLUGE,
			TGKS_MOVE,
			TGKS_AUTOSPRITE,
			TGKS_AUTOSPRITE2,

			// keywords for TGTS_SORT
			TGKS_PORTAL,
			TGKS_SKY,
			TGKS_OPAQUE,
			TGKS_BANNER,
			TGKS_UNDERWATER,
			TGKS_ADDITIVE,
			TGKS_NEAREST,

			// keywords for TGTS_SURFACEPARM
			TGKS_ALPHASHADOW,
			TGKS_AREAPORTAL,
			TGKS_CLUSTERPORTAL,
			TGKS_DONOTENTER,
			TGKS_FLESH,
			TGKS_FOG,
			TGKS_LAVA,
			TGKS_METALSTEPS,
			TGKS_NODAMAGE,
			TGKS_NODLIGHT,
			TGKS_NODRAW,
			TGKS_NODROP,
			TGKS_NOIMPACT,
			TGKS_NOMARKS,
			TGKS_NOLIGHTMAP,
			TGKS_NOSTEPS,
			TGKS_NONSOLID,
			TGKS_ORIGIN,
			TGKS_PLAYERCLIP,
			TGKS_SLICK,
			TGKS_SLIME,
			TGKS_STRUCTURAL,
			TGKS_TRANS,
			TGKS_WATER,

			// keywords for TGTS_BLENDFUNC (with one arg)
			TGKS_ADD,
			TGKS_FILTER,
			TGKS_BLEND,

			// keywords for TGTS_BLENDFUNC,
			// psossible in both arguments
			TGKS_GL_DST_COLOR,
			TGKS_GL_ONE_MINUS_DST_COLOR,
			TGKS_GL_ONE,
			TGKS_GL_ZERO,
			TGKS_GL_SRC_ALPHA,
			TGKS_GL_ONE_MINUS_SRC_ALPHA,
      TGKS_GL_ONE_MINUS_DST_ALPHA,
			TGKS_GL_SRC_COLOR,
			TGKS_GL_ONE_MINUS_SRC_COLOR,

			//keywords for TGTS_RGBGEN_W and TGTS_TCMOD_ST
			TGKS_SIN,
			TGKS_TRIANGLE,
			TGKS_SQUARE,
			TGKS_SAWTOOTH,
			TGKS_INVERSESAWTOOTH,

			//keywords for TGTS_TCGEN
			TGKS_BASE,
			TGKS_LIGHTMAP,
			TGKS_ENVIRONMENT,
			TGKS_VECTOR,	// special: change the keyword to TGTS_TCGEN_V

			//keyword for TGTS_TCMOD*
			TGKS_ROTATE,
			TGKS_SCALE,
			TGKS_SCROLL,
			TGKS_STRETCH,
			TGKS_TRANSFORM,
			TGKS_TURB,

			// keyword for TGTS_DEPTHFUNC
			TGKS_LEQUAL,
			TGKS_EQUAL,

			// keyword for TGTS_ALPHAFUNC
			TGKS_GT0,
			TGKS_LT128,
			TGKS_GE128,

			// keywords for TGTS_RGBGEN
			TGKS_IDENTITY,
			TGKS_IDENTITYLIGHTING,
			TGKS_ENTITY,
			TGKS_ONEMINUSENTITY,
			TGKS_FROMVERTEX, //FIXME is there VERTEX and ONEMINUSVERTEX instead ???
                       // I think this vertex = fromvertex = 1-oneminusvertex
			TGKS_LIGHTINGDIFFUSE,
      TGKS_EXACTVERTEX, //used in some q3 shader, but not documented
      TGKS_VERTEX, //section 6.3.6 q3 shader manual

			//keyword for TGTS_ALPHAGEN
			//TGKS_PORTAL, // PORTAL ever defned. use it...

			/* TODO: add here all of the shaders' keywords */
			// detail for arguments:
			// S = String
			// I = Int
			// F = Float, Fn = Float normalized
			// Kz..y = keyword >=z <=y
			// 7F 8S 2K ok

			/* General shaders keywirds */
			TGTS_SKYPARAMS,	// SIF (float or int ?)
			TGTS_CULL,		// K front..none
			TGTS_DEFORMVERTEXES,	// wave: FKFFFF
			//TGTS_DEFORMVERTEXES_N,	// FSFFF
			//TGTS_DEFORMVERTEXES_B,	// FFF		???
			//TGTS_DEFORMVERTEXES_M,	// FFFSFFFF
			//TGTS_DEFORMVERTEXES_A,	//
			//TGTS_DEFORMVERTEXES_A2, //
			TGTS_FOGPARAMS,	// FnFnFnF
			TGTS_NOPICMIP,		//
			TGTS_NOMIPMAPS,		//
			TGTS_POLYGONOFFSET,	//
			TGTS_PORTAL,		//
			TGTS_SORT,		// K (or S in the file)
			//   portal-nearest
			/* Q3MAP Specific Shader Keywords */
			TGTS_TESSSIZE,		// F
			TGTS_Q3MAP_BACKSHADER,	// S
			TGTS_Q3MAP_GLOBALTEXTURE, //
			TGTS_Q3MAP_SUN,		// Fn?Fn?Fn?I?I?I?
			TGTS_Q3MAP_SURFACELIGHT,// F?
			TGTS_Q3MAP_LIGHTIMAGE,	// S
			TGTS_Q3MAP_LIGHTSUBDIVIDE, // I?
			TGTS_SURFACEPARM,	// K alphashadow..water

			/* editor options .... ignore... */
			TGTS_QER_EDITORIMAGE,	// S
			TGTS_QER_NOCARVE,	//
			TGTS_QER_TRANS,	// F

			/* Stage Specific Keywords */
			TGTS_MAP,		// S
			TGTS_CLAMPMAP,		// S
			TGTS_ANIMMAP,		// FS*
			TGTS_BLENDFUNC,	// K (shortcuts)
			//TGTS_BLENDFUNC,	// KK
			TGTS_RGBGEN/*_IL,		//
				     TGTS_RGBGEN_I,		//
				     TGTS_RGBGEN_W,		// KFFFF
				     TGTS_RGBGEN_E,		//
				     TGTS_RGBGEN_OME,	//
				     TGTS_RGBGEN_V,		// ????
				     TGTS_RGBGEN_OMV,	// ???? and fromVertex ???
				     TGTS_RGBGEN_LD*/,	//
				TGTS_ALPHAGEN,		// K
			TGTS_TCGEN,		// K
			//TGTS_TCGEN_V,		// FFFFFF
			TGTS_TCMOD/*_R,		// F
				    TGTS_TCMOD_SCA,	// FF
				    TGTS_TCMOD_SCR,	// FF
				    TGTS_TCMOD_ST,		// KFFFF
				    TGTS_TCMOD_TR,		// FFFFFF FIXME
				    TGTS_TCMOD_TU*/,		// FFFF
				TGTS_DEPTHFUNC,		// K lequal..equal
			TGTS_DEPTHWRITE,	//
			TGTS_DETAIL,		// FIXME
			TGTS_ALPHAFUNC,	// K

			/* NOT FOUND IN THE DOC */
			TGTS_COLORMAP,		// S
			TGTS_TEXTURE,		//

			TGTS___LAST_TOKEN
		};

		/** string representation of all of the tokens */
		static const char*	stringShaderTks[];

	private:
		/** shaders read in the file */
		vector<tgShader*>	shaders;
		/** reference to the file */
		int			file;
		/** true if an error has been found */
		bool			error;


		/* main functions =================================================== */

		/** read the next char in the file, and increment the position. also
		 * set tokenRead to TGTS_EOF if necessary.
		 * @return true if the char is read, false if EOF reached
		 */
		bool ReadNextChar	();

		/** read the token when we know that we are at the begining of a token
		 * @return the token read
		 */
		tgShaderToken ReadToken ();

    /**
     * set the parser at the beginning of the next token, without reading it
     * @return false if EOF
     */
    bool GoToNextToken ();

		/** set the parser at the begining of the next token and read it
		 * skip comments and whitespace
		 * @return the token read
		 */
		tgShaderToken NextToken ();

		/** check if the given string is a known reserved word.
		 * @param token the token to check
		 * @return the given token id if the token is known, TGTS_STRING if not
		 */
		tgShaderToken CheckReservedWord(tgstring &token);


		/* helpers functions =============================================== */

		/** check if the char read is a white space or not
		 * @return true if c is "\n", " " or "\t"
		 */
		bool IsWhiteSpace	();

		/** check if the read char is a character allowed to be in a string or not
		 * @return true if c can be in a string, else, return false
		 */
		bool IsStringAllowedChar();

		/** check if the character is a digit, or a dot.
		 * @return true if c is 0..9 or "."
		 */
		bool IsDigitOrDot	();

		/** check if the character read is a digit.
		 * @return true if c is between 0 and 9, else false
		 */
		bool IsDigit		();

		/** turn into lowrecase each character of the given string
		 * @param string the string to lowerize
		 * @return a lowerized representation of the given string
		 */
		tgstring Lowerize	(tgstring &string);

		/** Check if the token read is of the given type
		 * @param t the expected token
		 * @return true if the token read is the expected one, or of his type (for TGTS_STRING)
		 * @see Accept(tgShaderToken, bool)
		 */
		bool Accept		(tgShaderToken t);

    /**
     * Same as Accept(), but doesn't let other tokens pass as strings 
     */
    bool StrictAccept (tgShaderToken t);

    /**
     * Check if the next token that will be read will be of the given type
     * __doesn't__ read the next token and doesn't consume anything in the file
     */
    bool AcceptAhead (tgShaderToken t);

    /**
     * Same as AcceptAhead(), but calls StrictAccept instead of Accept
     */
    bool StrictAcceptAhead (tgShaderToken t);

		/** Check if the token read is of the given type t. t can be an exact token
		 * or TGTS_STRING. TGTS_STRING accept all of the token values >= TGTS_STRING
		 * @param t the expected token
		 * @param AcceptEOF if true, TGTS_EOF is accepted
		 * @return true if the token read is the expected one, or of his type (for TGTS_STRING)
		 */
		bool Accept		(tgShaderToken t, bool AcceptEOF);

		/** Check if the token read is an int and if it's between min and max (included)
		 * @param min the minimal value accepted
		 * @param max the maximal value accepted
		 * @return true if the token is an int AND his value is >= min and <= max
		 */
		bool AcceptInt		(int min, int max);

		/** Check if the token read is a float and if it's between min and max (included)
		 * @param min the minimal value accepted
		 * @param max the maximal value accepted
		 * @return true if the token is a float AND his value is >= min and <= max
		 */
		bool AcceptFloat	(float min, float max);

		/** check if the token read is between min and max.
		 * min and max are the min and max values of accepted tokens
		 * @param min the minimal token accepted
		 * @param max the maximal token accepted
		 * @param setError if true and the checked token is not in the range, an error is set
		 * @return true if the token value is >= min and <= max
		 */
		bool CheckToken		(tgShaderToken min, tgShaderToken max, bool setError = true);

		/** Accept a list of shaders in the file
		 */
		void AcceptShaders	();

		/** Accept one and only one shader. */
		void AcceptShader	();

		/** Accept a list of properties.
		 * @param s the shader owner of the properties to accept
		 */
		void AcceptProperties	(tgShader *s);
		void AcceptPropertiesBlock	(tgShaderBlock *s);

		/** Accept one and only one property
		 * @param s the shader owner of the property to accept
		 */
		void AcceptProperty	(tgShaderBlock *s);

		/** Accept a sub block in the shader (delimited by { and } )
		 * @param s the shader owner of the block to accept
		 */
		void AcceptSubblock	(tgShader *s);

		/** Accept n floats (normalized or not), and push values on the vector v
		 * If a float is not read, the function stops and return false
		 * @param v the vector where to push the values
		 * @param n the number of repetitions
		 * @param normalized true if we have to check for normalized floats (0.0 to 1.0)
		 * @return true if n floats have been read
		 */
		bool AcceptNPushBacknFloats (vector<float> &v, int n, bool normalized);

		/** old read char */
		char		old_c;
		/** curent read char */
		char		c;

		/** curent token read */
		tgShaderToken	tokenRead;

		/** string read. Ever contains a valid value */
		tgstring	stringRead;

		/** value of the read int.
		 * Contains a valid value iif the token read is an int or a float
		 * Shouldn't be used if tokenRead is TGTS_NUMBERF
		 */
		int		intRead;

		/** value of the read float.
		 * Contains a valid value iif the token read is an int or a float
		 * */
		float		floatRead;


		/* position in the file (to return very coool errors) ============ */

		/** curent column in the shader file */
		int		column;
		/** curent line in the shader file */
		int		line;


};

#endif

