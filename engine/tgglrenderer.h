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
#ifndef TGGLRENDERER_H
#define TGGLRENDERER_H

#include "gl/tggl.h"
#include "tgdefs.h"
#include "tgconsole.h"
#include "tgworld_manager.h"
#include "tgmodel.h"
#include "tgq3level.h"
#include "tgcanvas.h"
#include "tgdecalmanager.h"
#include "tgmaterial.h"
#include "tgfile_manager.h"
#include "tgsystem.h"
#include "tgmaterial_manager.h"
#include "tgparticle_emitter.h"
#include "tgparticle_system.h"
#include "tglight.h"

/**
* La classe de base de tout ce qui touche au rendu graphique
* Une sous-class par famille de carte graphique (NV15, R300, ...)
*/
class tgGLRenderer
{
 public:
	tgGLRenderer ();
	~tgGLRenderer ();
	/**
	* Initialize Opengl
	*/
	void InitGL ();
	
	/**
	* Change la taille de la fenêtre de rendu
	* @param w la nouvelle hauteur
	* @param h la nouvelle largeur
	*/
	void ResizeWindow(int w, int h);
	
	/**
	* Effectue le rendu  du WorldManager passé en paramètre
	* @param pWorld un pointeur sur le WorldManager
	*/
	void RenderWorld(tgWorldManager* pWorld);
	
	/**
	* Rend un frustum (debug)
	* @param pFrustum un pointeur sur le frustum
	*/
	void RenderDebugFrustum (tgFrustum* pFrustum);
	
	/**
	* Rend un canevas
	* @param pCanvas un pointeur sur le canevas
	*/
	void RenderCanvas (tgCanvas* pCanvas);
	
	/**
	* Appelée avant le rendu
	*/
	void PreRender ();
	
	/**
	* Appelée après le rendu
	*/
	void PostRender ();
	
	/**
	* Chargement d'une font
	*/
	void LoadFont(tgFont* f, const tgstring& sMatName);
	/**
	* Release d'une font
	*/
	void ReleaseFont(tgFont* f);

	/**
	 * Passage en mode wireframe
	 */ 
	void ToggleWireframe ();
	
	/**
	* Les chargement a effectuer apres tout le reste
	*/
	void EndLoad();

	/**
	* Verifie si openGL a renvoye une erreur via glGetError, le cas echeant,
	* affiche un message d'erreur et retourne true
	* si rien n'est a signaler, retourne false
	* @param const char* est un message explicatif affiche en cas d'erreur
	*/
	static bool GLHasError(const char*);
	
 protected:
	void _GLUPerspective(double fovy, double aspect, double zNear, double zFar);
	void _SetupView(tgRenderView* r);
	void _RenderBVolume(tgBoundingVolume* b);
	void _RenderText (tgFont* f,const char* c, float x, float y, float s, const tgColor& FGcolor, const tgColor& BGcolor);
	void _DrawAxis();

	void _Begin2D();
	void _End2D();
	
	void _RenderModel (tgModel* m);

  void _RenderParticleEmitter (tgParticleEmitter* emit);
	
	void _RenderModelShadow (tgModel* m);
	void _DrawMeshShadowVolume (tgModel* m, const tgVector3& objectLightPos);
	
	void _DebugCollisions();

	//void _DynamicLight(tgQ3Level* l,const tgVector3& pos, const tgVector3& color, float radius);
  void _DynamicLight(tgQ3Level* l,tgLight* light);

#if 0
	void _RenderD3Level (tgDoom3Level* l);
#endif
	void _RenderQ3Level (tgQ3Level* l);
	
	void _RenderDecals (tgDecalManager* m);
	
	void _TransformEntity (tgIEntity* e);
	
	void _ExtractFrustum ();

	tgCVar cvDebugStencil;
	tgCVar cvStencilShadows;
	tgCVar cvToonShading;
	tgCVar cvShowNormals;
	tgCVar cvShowBVolumes;
	tgCVar cvDebugCollisions;
	
	bool bWireframe;

	tgMaterial* pLightMaterial;

	//OPENGL EXTENSIONS
	typedef void (APIENTRY * GL_ActiveTextureARB_Func)(GLenum);
	typedef void (APIENTRY * GL_MultiTexCoord2fARB_Func)(GLenum, GLfloat, GLfloat);
	typedef void (APIENTRY * GL_ClientActiveTextureARB_Func)(GLenum);
  //Point sprites
  typedef void (APIENTRY * GL_PointParameterfARB_Func) (GLenum pname, GLfloat param);
  typedef void (APIENTRY * GL_PointParameterfvARB_Func) (GLenum pname, const GLfloat *params);
	//GLSL
	typedef GLhandleARB (APIENTRY * GL_CreateShaderObjectARB_Func)(GLenum shaderType);
	typedef GLhandleARB (APIENTRY * GL_CreateProgramObjectARB_Func) (void);
	typedef void (APIENTRY * GL_ShaderSourceARB_Func)(GLhandleARB shader, int numOfStrings, const char** strings, int* lenOfStrings);
	typedef void (APIENTRY * GL_CompileShaderARB_Func)(GLhandleARB shader);
	typedef void (APIENTRY * GL_AttachObjectARB_Func)(GLhandleARB program, GLhandleARB shader);
	typedef void (APIENTRY * GL_LinkProgramARB_Func)(GLhandleARB program);
	typedef void (APIENTRY * GL_UseProgramObjectARB_Func)(GLhandleARB program);
	typedef void (APIENTRY * GL_DetachObjectARB_Func)(GLhandleARB program, GLhandleARB shader);
	typedef void (APIENTRY * GL_DeleteObjectARB_Func)(GLhandleARB id);
	typedef void (APIENTRY * GL_GetInfoLogARB_Func)(GLhandleARB object, int maxlength, int* len, char* log);
	typedef void (APIENTRY * GL_GetObjectParameterivARB_Func)(GLhandleARB object, GLenum type, int* param);
	typedef GLint (APIENTRY * GL_GetUniformLocationARB_Func)(GLhandleARB program, const char* name);
	typedef void (APIENTRY * GL_Uniform1fARB_Func)(GLint , GLfloat);
	typedef void (APIENTRY * GL_Uniform2fARB_Func)(GLint , GLfloat,GLfloat);	
	typedef void (APIENTRY * GL_Uniform3fARB_Func)(GLint , GLfloat,GLfloat,GLfloat);
	typedef void (APIENTRY * GL_Uniform4fARB_Func)(GLint , GLfloat,GLfloat,GLfloat, GLfloat);

	

	GL_ActiveTextureARB_Func tgglActiveTexture;
	GL_MultiTexCoord2fARB_Func tgglMultiTexCoord2f;
	GL_ClientActiveTextureARB_Func tgglClientActiveTexture;
  GL_PointParameterfARB_Func tgglPointParameterf;
  GL_PointParameterfvARB_Func tgglPointParameterfv;
	//GLSL
	GL_CreateShaderObjectARB_Func tgglCreateShaderObject;
	GL_CreateProgramObjectARB_Func tgglCreateProgramObject;
	GL_ShaderSourceARB_Func tgglShaderSource;
	GL_CompileShaderARB_Func tgglCompileShader;
	GL_AttachObjectARB_Func tgglAttachObject;
	GL_LinkProgramARB_Func tgglLinkProgram;
	GL_UseProgramObjectARB_Func tgglUseProgramObject;
	GL_DetachObjectARB_Func tgglDetachObject;
	GL_DeleteObjectARB_Func tgglDeleteObject;
	GL_GetInfoLogARB_Func tgglGetInfoLog;
	GL_GetObjectParameterivARB_Func tgglGetObjectParameteriv;
	GL_GetUniformLocationARB_Func tgglGetUniformLocation;
	GL_Uniform1fARB_Func tgglUniform1f;
	GL_Uniform2fARB_Func tgglUniform2f;
	GL_Uniform3fARB_Func tgglUniform3f;
	GL_Uniform4fARB_Func tgglUniform4f;
	
	
 protected:
	 class tgGLSL {
		 public:
			 tgGLSL(const tgstring& filename);
			 ~tgGLSL();
			 void Enable ();
			 void Disable ();
			 void SetUniform(const char* name, float, float, float,float);
			 void SetUniform(const char* name, float, float, float);
			 void SetUniform(const char* name, float, float);
			 void SetUniform(const char* name, float);
		 protected:
			 void _PrintLog(GLhandleARB);
			 tgstring _ReadFromTextFile(int file);
			 GLhandleARB vertexShader;
			 GLhandleARB program;
	 };
	 
	 
	 tgGLSL* pToonShader;
	 tgMaterial* pToonMaterial;
	 tgVector3 vLight;

};


#endif
