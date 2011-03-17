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

#include "tgshader_parser.h"
#include "tgshader_property.h"
#include "tgshader.h"

//#define DEBUG_SHADER_PARSER
#ifdef DEBUG_SHADER_PARSER
#define coutnl(a)	std::cout << "[TGE][SHADERS] " << a << std::endl;
#define nl		std::cout << std::endl ;
#else
#define coutnl(a) ;
#define nl ;
#endif

tgShaderParser::tgShaderParser (int _file) :
	shaders(), file(_file), old_c(' '), c(' ')
{
	line	= 1;
	error	= false;
	AcceptShaders();
}

tgShaderParser::~tgShaderParser ()
{
	vector<tgShader*>::iterator i;
	for (i=shaders.begin(); i != shaders.end();) {
		delete *i;
		i = shaders.erase(i);
	}

}

void tgShaderParser::GetShaders(vector<tgShader*>& vec)
{
	vec = shaders;
}

tgShader* tgShaderParser::GetShader(string name)
{
	vector<tgShader*>::iterator i;
	for (i=shaders.begin(); i != shaders.end();i++) {
		if( (*i)->GetName() == name) {
			return *i;
		}
	}
	return NULL;
}

void tgShaderParser::AcceptShaders() {
//	coutnl("AcceptShaders()")
	NextToken();
	while( Accept(TGTS_STRING, true) ) { // && !error) {
		AcceptShader();
		NextToken();
	}
//	dcout tgShaderParser::stringShaderTks[tokenRead] << " -- " << t << " error: " << error dendl;
}

void tgShaderParser::AcceptShader()
{
//	coutnl("AcceptShader()")
	tgShader *s = new tgShader();
	s->SetName(stringRead);

	NextToken();
	Accept(TGTS_LCROC);
	AcceptProperties(s);
	Accept(TGTS_RCROC);

	// fix last properties to the shader
	s->SetLoaded(true);
	s->SetHasError(error);
	error = false;

	// add it to the list
	shaders.push_back(s);
	coutnl( "shader read: " << endl <<  *s  );
}

void tgShaderParser::AcceptProperties(tgShader *s)
{
	coutnl("AcceptProperties()")
	NextToken();
	while(tokenRead >= TGTS_STRING || tokenRead == TGTS_LCROC
		|| tokenRead == TGTS_NUMBERI || tokenRead == TGTS_NUMBERF
		) {
		if(tokenRead == TGTS_LCROC) {
			AcceptSubblock(s);
		} else {
			AcceptProperty(s);
		}
		NextToken();
	}
}

void tgShaderParser::AcceptPropertiesBlock(tgShaderBlock *s)
{
	coutnl("AcceptPropertiesBlock()")
	NextToken();
	while(tokenRead >= TGTS_STRING || tokenRead == TGTS_LCROC
		|| tokenRead == TGTS_NUMBERI || tokenRead == TGTS_NUMBERF
		) {
		AcceptProperty(s);
		NextToken();
	}
}

void tgShaderParser::AcceptProperty(tgShaderBlock *s)
{
	coutnl("Traitement d'une propriété: " << stringShaderTks[tokenRead]);
	int tk = tokenRead;
	vector<float>		fp;
	vector<tgstring>	sp;
	vector<int>		tp;
//	coutnl("Token: "<< stringRead << " ("
//			<< tgShaderParser::stringShaderTks[tokenRead] << ")");

	switch(tokenRead) {
		// all properties without any parameter:
		case TGTS_NOPICMIP:
		case TGTS_NOMIPMAPS:
		case TGTS_POLYGONOFFSET:
		case TGTS_PORTAL:
		case TGTS_Q3MAP_GLOBALTEXTURE:
		case TGTS_DEPTHWRITE:
		case TGTS_DETAIL: //FIXME
//			p = new tgShaderProperty(tk);
			break;
		// one float
		case TGTS_TESSSIZE:
		case TGTS_Q3MAP_SURFACELIGHT:
		case TGTS_Q3MAP_LIGHTSUBDIVIDE:
			NextToken();
			if(!Accept(TGTS_NUMBERF)) return;
			fp.push_back(floatRead);
			break;
		// one string
		case TGTS_Q3MAP_BACKSHADER:
		case TGTS_Q3MAP_LIGHTIMAGE:
		case TGTS_MAP:
		case TGTS_CLAMPMAP:
			NextToken();
			if(!Accept(TGTS_STRING)) return;
			sp.push_back(stringRead);
			break;
		// one skeyword
		case TGTS_CULL:
			NextToken();
			if(!CheckToken(TGKS_FRONT, TGKS_NONE)) return;
			tp.push_back(tokenRead);
			break;

		// with more specific parameter list......
		case TGTS_SKYPARAMS:
			AcceptNPushBacknFloats(fp, 3, false);
			break;
		case TGTS_DEFORMVERTEXES:
			NextToken();
			if(!CheckToken(TGKS_WAVE, TGKS_AUTOSPRITE2)) return;
			tp.push_back(tokenRead);
			switch(tokenRead) {
				case TGKS_WAVE:
					NextToken();
					if(!Accept(TGTS_NUMBERF)) return;
					fp.push_back(floatRead);
					NextToken();
					if(!CheckToken(TGKS_SIN, TGKS_INVERSESAWTOOTH)) return;
					tp.push_back(tokenRead);
					AcceptNPushBacknFloats(fp, 4, false);
					break;
				case TGKS_NORMAL:
					NextToken();
					if(!Accept(TGTS_NUMBERF)) return;
					fp.push_back(floatRead);
					NextToken();
					if(!CheckToken(TGKS_SIN, TGKS_INVERSESAWTOOTH)) return;
					tp.push_back(tokenRead);
					AcceptNPushBacknFloats(fp, 3, false);
					break;
				case TGKS_BLUGE:
					AcceptNPushBacknFloats(fp, 3, false);
					break;
				case TGKS_MOVE:
					AcceptNPushBacknFloats(fp, 3, false);

					NextToken();
					if(!CheckToken(TGKS_SIN, TGKS_INVERSESAWTOOTH)) return;
					tp.push_back(tokenRead);

					AcceptNPushBacknFloats(fp, 4, false);
					break;
				case TGKS_AUTOSPRITE:
				case TGKS_AUTOSPRITE2:
				default:
					break;
			}
			break;
		case TGTS_FOGPARAMS:
			AcceptNPushBacknFloats(fp, 3, true);
			NextToken();
			if(!Accept(TGTS_NUMBERF)) return;
			fp.push_back(floatRead);
			break;
		case TGTS_SORT:
			NextToken();
			if(tokenRead == TGTS_NUMBERF || tokenRead == TGTS_NUMBERI) {
				fp.push_back(floatRead);
			} else {
				if(!CheckToken(TGKS_PORTAL, TGKS_NEAREST)) return;
				tp.push_back(tokenRead);
			}
			break;
		case TGTS_Q3MAP_SUN:
			AcceptNPushBacknFloats(fp, 3, true);
			AcceptNPushBacknFloats(fp, 3, false);
			break;
		case TGTS_SURFACEPARM:
			NextToken();
			if(!CheckToken(TGKS_ALPHASHADOW, TGKS_WATER)) return;
			tp.push_back(tokenRead);
			break;
		case TGTS_ANIMMAP:
			NextToken();
			if(!Accept(TGTS_NUMBERF)) return;
			fp.push_back(floatRead);
			for(int i = 0; i < 8; ++i) {
        //FIXME: Accept accept tokenRead > TGTS_STRING if t =TGTS_STRING,
        //which makes it consume the blendFunc keyword. Here, we need a strict string test
        if (!StrictAcceptAhead(TGTS_STRING)) return;
        NextToken();
				sp.push_back(stringRead);
			}
			break;
		case TGTS_BLENDFUNC:
			NextToken();
			if(CheckToken(TGKS_ADD, TGKS_BLEND, false)) {
				tp.push_back(tokenRead);
			} else {
				if(!CheckToken(TGKS_GL_DST_COLOR, TGKS_GL_ONE_MINUS_SRC_COLOR)) return;
				tp.push_back(tokenRead);
				NextToken();
				if(!CheckToken(TGKS_GL_DST_COLOR, TGKS_GL_ONE_MINUS_SRC_COLOR)) return;
				tp.push_back(tokenRead);
			}
			break;
		case TGTS_RGBGEN:
    case TGTS_ALPHAGEN:
      //FIXME: rgbgen = alphagen, BUT alphagen may have the "portal" keyword that rgbgen can't, we need to handle it
			NextToken();
			if(!(tokenRead == TGKS_WAVE ||
				CheckToken(TGKS_IDENTITY, TGKS_VERTEX, TGKS_EXACTVERTEX))) return;
			tp.push_back(tokenRead);
			switch(tokenRead) {
				case TGKS_WAVE:
					NextToken();
					if(!CheckToken(TGKS_SIN, TGKS_INVERSESAWTOOTH)) return;
					tp.push_back(tokenRead);
					NextToken();
					if(!AcceptFloat(0.0f, 1.0f)) return;
					fp.push_back(floatRead);
					AcceptNPushBacknFloats(fp, 3, false);
					break;
				case TGKS_IDENTITY:
				case TGKS_IDENTITYLIGHTING:
				case TGKS_ENTITY:
				case TGKS_ONEMINUSENTITY:
				case TGKS_LIGHTINGDIFFUSE:
					break;
				case TGKS_FROMVERTEX:
        case TGKS_VERTEX:
        case TGKS_EXACTVERTEX:
					//FIXME is there VERTEX and ONEMINUSVERTEX instead ???
					break;
				default:
					break;
			}

			break;
		case TGTS_TCGEN:
			NextToken();
			if(CheckToken(TGKS_BASE, TGKS_ENVIRONMENT, false)) {
				tp.push_back(tokenRead);
			} else {
				if(!Accept(TGKS_VECTOR)) return;
				tp.push_back(TGKS_VECTOR);

				NextToken();
				if(!Accept(TGTS_LPAREN)) return;
				AcceptNPushBacknFloats(fp, 3, false);
				NextToken();
				if(!Accept(TGTS_RPAREN)) return;

				NextToken();
				if(!Accept(TGTS_LPAREN)) return;
				AcceptNPushBacknFloats(fp, 3, false);
				NextToken();
				if(!Accept(TGTS_RPAREN)) return;
			}
			break;
		case TGTS_TCMOD:
			NextToken();
			if(!CheckToken(TGKS_ROTATE, TGKS_TURB)) return;
			tp.push_back(tokenRead);
			switch(tokenRead) {
				case TGKS_ROTATE:
					NextToken();
					if(!Accept(TGTS_NUMBERF)) return;
					fp.push_back(floatRead);
					break;
				case TGKS_SCALE:
				case TGKS_SCROLL:
					AcceptNPushBacknFloats(fp, 2, false);
					break;
				case TGKS_STRETCH:
					NextToken();
					if(!CheckToken(TGKS_SIN, TGKS_INVERSESAWTOOTH)) return;
					tp.push_back(tokenRead);
					AcceptNPushBacknFloats(fp, 4, false);
					break;
				case TGKS_TURB:
					AcceptNPushBacknFloats(fp, 4, false);
					break;
				case TGKS_TRANSFORM:
					AcceptNPushBacknFloats(fp, 6, false);
					break;
				default:
					break;
			}
			break;
		case TGTS_DEPTHFUNC:
			NextToken();
			if(!CheckToken(TGKS_LEQUAL, TGKS_EQUAL)) return;
			tp.push_back(tokenRead);
			break;
		case TGTS_ALPHAFUNC:
			NextToken();
			if(!CheckToken(TGKS_GT0, TGKS_GE128)) return;
			tp.push_back(tokenRead);
			break;
		// to ignore.....
		case TGTS_QER_EDITORIMAGE:
			NextToken();
			Accept(TGTS_STRING);
			return;
		case TGTS_QER_NOCARVE:
			return;
		case TGTS_QER_TRANS:
			NextToken();
			Accept(TGTS_NUMBERF);
			break;
		default:
			tgerr << "Unknown property: "<< stringRead << " ("
					<< tgShaderParser::stringShaderTks[tokenRead] << ")" << std::endl;
			return;
	}
	// if we reach this point, no error have been set!
	tgShaderProperty *p = new tgShaderProperty(tk);

	vector<float>::iterator it_f;
	for (it_f=fp.begin(); it_f != fp.end(); ++it_f) {
		p->AddFloat(*it_f);
	}

	vector<tgstring>::iterator it_s;
	for (it_s=sp.begin(); it_s != sp.end(); ++it_s) {
		p->AddString(*it_s);
	}

	vector<int>::iterator it_k;
	for (it_k=tp.begin(); it_k != tp.end(); ++it_k) {
		p->AddKeyword(*it_k);
	}

	s->AddProperty(p);
}

bool tgShaderParser::AcceptNPushBacknFloats (vector<float> &v, int n, bool normalized) {
	if(!normalized) {
		for(int i = 0; i < n; ++i) {
			NextToken();
			if(!Accept(TGTS_NUMBERF)) return false;
			v.push_back(floatRead);
		}
	} else {
		for(int i = 0; i < n; ++i) {
			NextToken();
			if(!AcceptFloat(0.0f, 1.0f)) return false;
			v.push_back(floatRead);
		}
	}
	return true;
}

void tgShaderParser::AcceptSubblock(tgShader *s) {
	coutnl("  AcceptSubblock()")
	// FIXME: we must save each block separated.
	// TODO: so, make a class tgShaderBlock.... and pass a new
	// instance of to AcceptProperties (make a superclass of tgShader with two subclass:
	// tgShader and tgShaderBlock) then add-ti to the shader s
	tgShaderBlock* b = new tgShaderBlock();
	AcceptPropertiesBlock(b);
	Accept(TGTS_RCROC);
	s->AddBlock(b);
}

bool tgShaderParser::Accept(tgShaderToken t) {
//	return Accept(t, false);
  if(!( tokenRead == t ||
        (t == TGTS_STRING && tokenRead > TGTS_STRING) ||
        (t == TGTS_NUMBERF && tokenRead == TGTS_NUMBERI)
      )) {
    return false;
  }
  return true;
}

bool tgShaderParser::StrictAccept(tgShaderToken t) {
  if (tokenRead == t)
    return true;
  else
    return false;
}

bool tgShaderParser::AcceptAhead(tgShaderToken t) {
  //save
  int savePos = pFileManager->GetReadOffset(file);
  tgstring saveStr = stringRead;
  tgShaderToken token = tokenRead;

  bool result;
  NextToken();
  result = Accept(t);

  //restore
  tokenRead = token;
  stringRead = saveStr;
  pFileManager->Seek(file, savePos);
  return result;
}

bool tgShaderParser::StrictAcceptAhead(tgShaderToken t) {
  //save
  int savePos = pFileManager->GetReadOffset(file);
  tgstring saveStr = stringRead;
  tgShaderToken token = tokenRead;

  bool result;
  NextToken();
  result = StrictAccept(t);

  //restore
  tokenRead = token;
  stringRead = saveStr;
  pFileManager->Seek(file, savePos);
  return result;
}

bool tgShaderParser::Accept(tgShaderToken t, bool AcceptEOF) {
//	tgShaderToken tk = NextToken();
	if(!( tokenRead == t ||
	      (t == TGTS_STRING && tokenRead > TGTS_STRING) ||
	      (t == TGTS_NUMBERF && tokenRead == TGTS_NUMBERI)
	    ) && (!AcceptEOF || tokenRead != TGTS_EOF)) {
		tgerr << "\nShader parser error (1) found:\n" <<
			"        Token expected: " << tgShaderParser::stringShaderTks[t] << "\n" <<
			"        Token found   : " << tgShaderParser::stringShaderTks[tokenRead] <<
			" (" << stringRead << ")"
			<< "\n" <<
			"        At line " << line << ", column " << column << std::endl;
		error = true;
		return false;
	} else if( tokenRead == TGTS_EOF) {
		if(!AcceptEOF) {
			tgerr << "End of file reached, but token " << tgShaderParser::stringShaderTks[t] <<
				"was expected. (Error 2)" << std::endl;
			error = true;
		}
		return false;
	}
	return true;
}

bool tgShaderParser::CheckToken(tgShaderToken min, tgShaderToken max, bool setError) {
	bool b = Accept(TGTS_STRING) && tokenRead >= min && tokenRead <= max;
	if (!b && setError) {
		tgerr << "\nShader parser error (3) found:\n" <<
			"        Token expected: '" << tgShaderParser::stringShaderTks[min] << "' ---> '" <<
							tgShaderParser::stringShaderTks[max] << "'" << endl <<
			"        Token found   : " << tgShaderParser::stringShaderTks[tokenRead] <<
			" (" << stringRead << ")"
			<< endl <<
			"        At line " << line << ", column " << column << std::endl;
		error = true;
	}
	return b;
}

bool tgShaderParser::AcceptInt(int min, int max) {
	bool b = Accept(TGTS_NUMBERI) && intRead >= min && intRead <= max;
	if (!b) {
		error = true;
		tgerr << "\nShader parser error (4) found:\n" <<
			"        Int expected: >= " << min << " and <= " << max << endl <<
			"        Token found   : " << tgShaderParser::stringShaderTks[tokenRead] <<
			" (" << stringRead << ")"
			<< endl <<
			"        At line " << line << ", column " << column << std::endl;
	}
	return b;
}

bool tgShaderParser::AcceptFloat(float min, float max) {
	bool b = Accept(TGTS_NUMBERF) && floatRead >= min && floatRead <= max;
	if (!b) {
		error = true;
		tgerr << "\nShader parser error (5) found:\n" <<
			"        Float expected: >= " << min << " and <= " << max << endl <<
			"        Token found   : " << tgShaderParser::stringShaderTks[tokenRead] <<
			" (" << stringRead << ")"
			<< endl <<
			"        At line " << line << ", column " << column << std::endl;
	}
	return b;
}

bool
tgShaderParser::GoToNextToken()
{
	bool noteof = true;
	while((noteof = ReadNextChar())) {
//		while(isWhiteSpace()) readNextChar();
		if(IsWhiteSpace()) {
			continue;
		} else if(c == '/') {
			ReadNextChar();
			if(c == '/') {
				do {
					noteof = ReadNextChar();
				} while (c != '\n' && noteof);
			} else {
				break;
			}
		} else {
			break;
		}
	}
  return noteof;
}

tgShaderParser::tgShaderToken
tgShaderParser::NextToken ()
{
//	coutnl( "nextToken" )
  bool noteof = GoToNextToken();
	if(noteof) {
		tokenRead = ReadToken();
		return tokenRead;
	} else {
		return TGTS_EOF;
	}
}

tgShaderParser::tgShaderToken
tgShaderParser::ReadToken ()
{
//	coutnl( "readToken" )
	if ( old_c == '/' || IsStringAllowedChar() ) {
		bool isDigit = true;
		int dotRead = 0;
		tgstring s;
		if(old_c == '/' ) {
			if(!IsStringAllowedChar()) {
				return TGTS_SLASH;
			}
			isDigit = false;
			s.append(1, old_c);
		}

//		do {
			// append c to the string
		s.append(1, c);
		isDigit = isDigit && (IsDigitOrDot() || c == '-') ;
		if(c == '.') {
			if(dotRead > 1) isDigit = false;
			else ++dotRead;
		}
		ReadNextChar();

		while(IsStringAllowedChar()) {
			s.append(1, c);
			isDigit = isDigit && IsDigitOrDot();
			if(c == '.') {
				if(dotRead > 1) isDigit = false;
				else ++dotRead;
			}
			ReadNextChar();
		}


		stringRead = s;

		if(isDigit) {
			if(dotRead == 0) {
				intRead = atoi(s.c_str());
				floatRead = (float)intRead;
				return TGTS_NUMBERI;
			} else if (dotRead == 1) {
				floatRead = (float)atof(s.c_str());
				// also set the int, even if it's shouldn't be used
				intRead = (int)(floatRead);
				return TGTS_NUMBERF;
			}
		}

		return CheckReservedWord(s);

	} else {
//		coutnl( "tokenOther  : " << (old_c == '/' ? old_c : c))
		stringRead = ""+c;
		switch(c) {
		case '{' :
			return TGTS_LCROC;
		case '}' :
			return TGTS_RCROC;
		case '(' :
			return TGTS_LPAREN;
		case ')' :
			return TGTS_RPAREN;
		case '-' :
			return TGTS_MINUS;
//		case '*' :
//			return TGTS_STAR;
		default :
			if(old_c == '/') return TGTS_SLASH;
			return TGTS_UNKNOWN;
	}}
	return TGTS_NULL;
}

bool tgShaderParser::ReadNextChar ()
{
	old_c = c;
	if(pFileManager->EndOfFile(file)) {
		tokenRead = TGTS_EOF;
		stringRead = "--end of file--";
		return false;
	}

	pFileManager->Get(file, c);
//	cout << c << endl;
	switch(c) {
		case '\n' :
			line++;
			column = 1;
			break;
		default:
			column++;
	}
	return true;
}

bool tgShaderParser::IsWhiteSpace()
{
	return  (
		c == '\r' ||
		c == '\t' ||
		c == ' ' ||
		c == '\n'
	       );
}

bool tgShaderParser::IsStringAllowedChar()
{
	// try to keep them in the "most-frequently-used" order....
	return (
		( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ) ||	// leter
		  IsDigitOrDot() ||
		  c == '/' ||
		  c == '_' ||
		  c == '-' ||
		  c == '*' ||
		  c == '$'		// FIXME: is $ correct here ?
	       );
}

bool tgShaderParser::IsDigitOrDot()
{
	return ( IsDigit() || c == '.' );
}

bool tgShaderParser::IsDigit()
{
	return ( c >= '0' && c <= '9' );

}

tgShaderParser::tgShaderToken
tgShaderParser::CheckReservedWord(tgstring &token)
{
	tgstring s = Lowerize(token);

	for(int i = TGTS___FIRST_TOKEN+1; i < TGTS___LAST_TOKEN; ++i) {
		if(s == tgShaderParser::stringShaderTks[i]) {
      coutnl("CheckReservedToken("<<token << ") = " << i << "("<<stringShaderTks[i]<<")");
			return (tgShaderToken)i;
    }
	}
	return TGTS_STRING;
}

tgstring tgShaderParser::Lowerize(tgstring &string)
{
	tgstring s;
	tgstring::iterator it;
	it = string.begin();
	for( it = string.begin(); it != string.end(); it++ ) {
		if(*it >= 'A' && *it <= 'Z') {
			s.append(1, *it-'A'+'a');
		} else {
			s.append(1, *it);
		}
	}
	return s;
}


//ALL entry MUST be lowerized since comparison are done with lowerized strings
const char* tgShaderParser::stringShaderTks[TGTS___LAST_TOKEN+1] = {
			"TGTS_NULL",
			"TGTS_UNKNOWN",	// we don't knwo....	1
			"TGTS_EOF",	// End Of File		2
			"TGTS_LCROC {",	// {			3
			"TGTS_RCROC }",	// }			4
			"TGTS_LPAREN (",	// {			5
			"TGTS_RPAREN )",	// }			6

			"TGTS_STRING",	// a string which doesn't
			"TGTS_NUMBERI",	// int			8
			"TGTS_NUMBERF",	// float		9

			"TGTS_MINUS -",	// -			10
			"TGTS_SLASH /",	// /			11
//			"TGTS_STAR *",	// *			12

			"TGTS___FIRST_TOKEN",



			"front",
			"back",
			"disable",
			"none",

			//keywords for TGTS_DEFORMVERTEXES
			"wave",
			"normal",
			"bluge",
			"move",
			"autosprite",
			"autosprite2",

			// keywords for "sort
			"portal",
			"sky",
			"opaque",
			"banner",
			"underwater",
			"additive",
			"nearest",

			// keywords for "surfaceparm
			"alphashadow",
			"areaportal",
			"clusterportal",
			"donotenter",
			"flesh",
			"fog",
			"lava",
			"metalsteps",
			"nodamage",
			"nodlight",
			"nodraw",
			"nodrop",
			"noimpact",
			"nomarks",
			"nolightmap",
			"nosteps",
			"nonsolid",
			"origin",
			"playerclip",
			"slick",
			"slime",
			"structural",
			"trans",
			"water",

			// keywords for "blendfunc1
			"add",
			"filter",
			"blend",

			// keywords for "blendfunc",
			// first arg only
			"gl_dst_color",
			"gl_one_minus_dst_color",
			// psossible in both arguments
			"gl_one",
			"gl_zero",
			"gl_src_alpha",
			"gl_one_minus_src_alpha",
      "gl_one_minus_dst_alpha",

			// second argument only
			"gl_src_color",
			"gl_one_minus_src_color",

			//keywords for "rgbgen_w and "tcmod_st
			"sin",
			"triangle",
			"square",
			"sawtooth",
			"inversesawtooth",

			//keywords for "tcgen
			"base",
			"lightmap",
			"environment",
			"vector",	// special: change the keyword to "tcgen_v

			//keyword for "tcmod*
			"rotate",
			"scale",
			"scroll",
			"stretch",
			"transform",
			"turb",

			// keyword for "depthfunc
			"lequal",
			"equal",

			// keyword for "alphafunc
			"gt0",
			"lt128",
			"ge128",
      // keyword for "rgbgen
			"identity",
			"identitylighting",
			"entity",
			"oneminusentity",
			"fromVertex",
			"lightingdiffuse",
      "exactvertex",
      "vertex",

			//keyword for "alphagen
			//tgks_portal", // portal ever defned. use it...

			/* todo: add here all of the shaders' keywords */
			// detail for arguments:
			// s = string
			// i = int
			// f = float", fn = float normalized
			// kz..y = keyword >=z <=y

			/* general shaders keywirds */
			"skyparams",	// sif (float or int ?)
			"cull",		// k front..none
			"deformvertexes",	// fsffff
			"fogparams",	// fnfnfnf
			"nopicmip",		//
			"nomipmaps",		//
			"polygonoffset",	//
			"portal",		//
			"sort",		// k (or s in the file)
						//   portal-nearest
			/* q3map specific shader keywords */
			"tesssize",		// f
			"q3map_backshader",	// s
			"q3map_globaltexture", //
			"q3map_sun",		// fn?fn?fn?i?i?i?
			"q3map_surfacelight",// f?
			"q3map_lightimage",	// s
			"q3map_lightsubdivide", // i?
			"surfaceparm",	// k alphashadow..water

			/* editor options .... ignore... */
			"qer_editorimage",	// s
			"qer_nocarve",	//
			"qer_trans",	// f

			/* stage specific keywords */
			"map",		// s
			"clampmap",		// s
			"animmap",		// fs*
			//"blendfunc1",	// k (shortcuts)
			"blendfunc",	// kk
			"rgbgen",
			//"rgbgen_il",		//
			//"rgbgen_i",		//
			//"rgbgen_w",		// kffff
			//"rgbgen_e",		//
			//"rgbgen_ome",	//
			//"rgbgen_v",		// ????
			//"rgbgen_omv",	// ???? and fromvertex ???
			//"rgbgen_ld",	//
			"alphagen",		// k
			"tcgen",		// k
			//"tcgen_v",		// ffffff
			"tcmod",
			//"tcmod_r",		// f
			//"tcmod_sca",	// ff
			//"tcmod_scr",	// ff
			//"tcmod_st",		// kffff
			//"tcmod_tr",		// ffffff
			//"tcmod_tu",		// ffff
			"depthfunc",		// k lequal..equal
			"depthwrite",	//
			"detail",		//
			"alphafunc",	// k

			/* not found in the doc */
			"colormap",		// s
			"texture",		//

			"TGTS___LAST_TOKEN"
		};


