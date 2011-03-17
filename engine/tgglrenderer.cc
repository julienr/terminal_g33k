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
#include "tgglrenderer.h"

tgGLRenderer::tgGLRenderer ()
{
	pRenderView = NULL;
	bWireframe = false;
	
	cvDebugStencil.Set(1);
	cvToonShading.Set(1);
	cvStencilShadows.Set(0);
	cvShowNormals.Set(0);
	cvShowBVolumes.Set(1);
	cvDebugCollisions.Set(0);
	
	pConsole->Register(tgMakeFunctor((tgFunctor0*)0, *this, &tgGLRenderer::ToggleWireframe), "wireframe");
	pConsole->Register(&cvDebugStencil,"debugstencil");
	pConsole->Register(&cvToonShading, "toonshading");
	pConsole->Register(&cvStencilShadows, "stencilshadows");
	pConsole->Register(&cvShowNormals, "shownormals");
	pConsole->Register(&cvShowBVolumes, "showbvolumes");
	pConsole->Register(&cvDebugCollisions, "debugcollisions");
}

tgGLRenderer::~tgGLRenderer ()
{
	//delete pToonShader;
}

void tgGLRenderer::ToggleWireframe ()
{
	bWireframe = !bWireframe;
}

bool tgGLRenderer::GLHasError(const char* explain)
{
	GLenum error = tgglGetError();
	if(error != GL_NO_ERROR) {
		tgout << explain << ", " << tggluErrorString(error) << " (" << error << ")" << endl;
		return true;
	}
	return false;
}

#define LoadExtension(NAME,STR,BOOL) tggl##NAME = (GL_##NAME##ARB_Func)SDL_GL_GetProcAddress(STR); \
	if (tggl##NAME == NULL) {\
		/*out << "Extension :" << STR << "  non utilisable" << endl;*/\
		BOOL = false; \
		}
		
void tgGLRenderer::InitGL ()
{
	tgglClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	tgglEnable (GL_DEPTH_TEST);
	tgglDepthFunc(GL_LEQUAL);
	tgglDisable(GL_LIGHTING);
	tgglEnable (GL_CULL_FACE);
	
	tgglEnable(GL_TEXTURE_2D);
	
	bool bHasMultiTex = true;
	bool bHasGLSL = true;
  bool bHasPointParameter = true;
	LoadExtension(ActiveTexture,"glActiveTextureARB",bHasMultiTex)
	LoadExtension(MultiTexCoord2f,"glMultiTexCoord2fARB",bHasMultiTex)
	LoadExtension(ClientActiveTexture,"glClientActiveTextureARB",bHasMultiTex)
  LoadExtension(PointParameterf,"glPointParameterfARB",bHasPointParameter)
  LoadExtension(PointParameterfv,"glPointParameterfvARB",bHasPointParameter)
	LoadExtension(CreateShaderObject,"glCreateShaderObjectARB",bHasGLSL)
	LoadExtension(CreateProgramObject,"glCreateProgramObjectARB",bHasGLSL)
	LoadExtension(ShaderSource,"glShaderSourceARB",bHasGLSL)
	LoadExtension(CompileShader,"glCompileShaderARB",bHasGLSL)
	LoadExtension(AttachObject,"glAttachObjectARB",bHasGLSL)
	LoadExtension(LinkProgram,"glLinkProgramARB",bHasGLSL)
	LoadExtension(UseProgramObject,"glUseProgramObjectARB",bHasGLSL)
	LoadExtension(DetachObject,"glDetachObjectARB",bHasGLSL)
	LoadExtension(DeleteObject,"glDeleteObjectARB",bHasGLSL)
	LoadExtension(GetInfoLog,"glGetInfoLogARB",bHasGLSL)
	LoadExtension(GetObjectParameteriv,"glGetObjectParameterivARB",bHasGLSL)
	LoadExtension(GetUniformLocation, "glGetUniformLocationARB",bHasGLSL)
	LoadExtension(Uniform1f,"glUniform1fARB",bHasGLSL)
	LoadExtension(Uniform2f,"glUniform2fARB",bHasGLSL)
	LoadExtension(Uniform3f,"glUniform3fARB",bHasGLSL)
	LoadExtension(Uniform4f,"glUniform4fARB",bHasGLSL)

	if(bHasMultiTex)
		tgout << "MultiTexture\t OK" << endl;
	else
		tgout << "MultiTexture\t ERREUR, impossible de lancer TG sans le support multi-textures" << endl;

	if(bHasPointParameter)
    tgout << "PointParameter\t OK" << endl;
  else
    tgout << "PointParameter\t ERREUR, impossible de lancer TG sans le support pointparameter" << endl;

	if(bHasGLSL)
		tgout << "GLSL\t OK" << endl;
	else {
		tgout << "GLSL\t ERREUR, pas de cel-shading" << endl;
		cvToonShading.Set(0);
	}
}

void tgGLRenderer::EndLoad()
{
	pLightMaterial = pMaterialManager->GetMaterial("textures/playerlight");
	//pLightMaterial = pMaterialManager->GetMaterial("gfx/2d/bigchars");
	/*if(cvToonShading.GetInt()) {
		pToonShader = new tgGLSL("data/programs/toon.vshader");
	}
	pToonMaterial = pMaterialManager->GetMaterial("textures/toon",TG_TEXTURE_1D);*/
}

void tgGLRenderer::ResizeWindow(int w, int h)
{
	GLfloat ratio;
	if (h==0) h=1;
	ratio = (GLfloat)w/(GLfloat)h;
	
	tgglViewport(0,0,(GLsizei)w,(GLsizei)h);
	tgglMatrixMode(GL_PROJECTION);
	tgglLoadIdentity();
	_GLUPerspective(120/2, ratio, 4.0f, 4000.0f);
	
	tgglMatrixMode(GL_MODELVIEW);
	tgglLoadIdentity();
}

void tgGLRenderer::PreRender ()
{
	pSystem->PreRender ();
	tgglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	tgglMatrixMode (GL_MODELVIEW);
	tgglLoadIdentity ();
}

void tgGLRenderer::PostRender ()
{
	glFlush ();
	pSystem->PostRender ();
}

void tgGLRenderer::RenderWorld(tgWorldManager* pWorld)
{
	_SetupView(pRenderView);
	//vLight.Set(0,0,1);
	
	//pTestRenderView = pWorld->GetTestRenderView();
	
	
	
	/*pRenderView = pWorld->GetTestRenderView ();
	
	
	
	glColor3f(1,0,0);
	glPointSize(10.0f);
	glBegin (GL_POINTS);
		glVertex3fv(&pWorld->GetTestRenderView()->vOrigin.x);
	glEnd ();
	glPointSize(1.0f);
	glColor3f(1,1,1);
	RenderDebugFrustum(&pWorld->GetTestRenderView()->frustum);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
	
	
	
	unsigned int size = pWorld->GetModelCount();
	for(unsigned int i=0; i<size; i++) {
		if (pWorld->GetModel(i)->bVisible) {
			_RenderModel(pWorld->GetModel(i));
			/*if(!pWorld->GetModel(i)->IsViewModel() && (cvStencilShadows.GetInt() != 0)) //pas de shadow sur les viewmodels
				_RenderModelShadow(pWorld->GetModel(i));*/
		}
	}
	_RenderQ3Level(pWorld->GetQ3Level());
  size = pWorld->GetLightsCount();
  for(unsigned int i=0; i<size; i++) {
    _DynamicLight(pWorld->GetQ3Level(), pWorld->GetLight(i)); 
  }
//	_DynamicLight(pWorld->GetQ3Level(),pRenderView->GetFinalPosition(),tgVector3(1,0,0),500.0f);
	_RenderDecals(pWorld->GetDecalManager());

  size = pWorld->GetParticleSystemsCount();
  for (unsigned int i=0; i<size; i++) {
    tgParticleSystem* sys = pWorld->GetParticleSystem(i);


    tgglPushMatrix();
    _TransformEntity(sys);

    int numEmitters = sys->GetNumEmitters();
    for(int j=0; j<numEmitters; j++) {
      _RenderParticleEmitter(sys->GetEmitter(j));
    }

    tgglPopMatrix();
  }



	//_RenderD3Level(pWorld->GetLevel());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void tgGLRenderer::RenderCanvas (tgCanvas* pCanvas)
{
	_Begin2D();
	//tgglColor4fv(&pCanvas->DrawColor.r);
	size_t size = pCanvas->TextToDraw.size();
	for(size_t i=0; i<size; i++) {
	//	out << pCanvas->TextToDraw[i].str.data() << endl;
		//tgglColor4fv(&pCanvas->TextToDraw[i].FGcolor.r);
		//_RenderText(&pCanvas->drawFont, pCanvas->TextToDraw[i].str.data(), 50,50,1);
		_RenderText(&pCanvas->drawFont, pCanvas->TextToDraw[i].str.data(), 50, 50, 1, pCanvas->TextToDraw[i].FGcolor, pCanvas->TextToDraw[i].BGcolor);
		if(pCanvas->TextToDraw[i].CR)
			glTranslatef(0,16,0);
	}
	pCanvas->_Clear();
	tgglColor4f(1,1,1,1);
	_End2D();
}

/*void tgGLRenderer::_RenderText (tgFont* f,const char* c, float x, float y, float s)
{
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glEnable( GL_BLEND );
	tgglActiveTexture(GL_TEXTURE0);
	f->pMat->BindColor();
	
	int set = 0;
	glListBase( f->dListId - 32 + ( 128 * set ) ); // we substract 32 because the 32 first character of ASCII table are not     displayable and not part of         our texture :-P
	
	glPushMatrix();
		glTranslatef(x, y, 0 );
		glScalef (s,s,s);
		glCallLists( strlen(c), GL_BYTE, c );
	glPopMatrix();
	glDisable (GL_BLEND);
}*/

void tgGLRenderer::_RenderText (tgFont* f,const char* c, float x, float y, float s, const tgColor& FGcolor, const tgColor& BGcolor)
{
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	tgglEnable(GL_BLEND);
	tgglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int set = 0;
	//on soustrait suivant les caracteres qui manquent au debut de la font (les 32 premiers caracter ASCII ne sont pas affiches et pas inclus dans certaines fonts -> -32
	glListBase( f->dListId - 0 + ( 128 * set ) );
	
	glPushMatrix();
		glTranslatef(x, y, 0 );
		glScalef (s,s,s);
		
	//	tgglColor4fv(&BGcolor.r);
	//ARRIERE-PLAN
		if(BGcolor.a != 0) {
			tgglActiveTexture(GL_TEXTURE1);
			tgglDisable(GL_TEXTURE_2D);
			
			tgglActiveTexture(GL_TEXTURE0);
			tgglDisable(GL_TEXTURE_2D);
			
			tgglColor4fv(&BGcolor.r);
			int length = (GLint)strlen(c)*16;
			tgglBegin(GL_QUADS);
				glVertex2i( 0, 0 );
				glVertex2i( length, 0 );
				glVertex2i( length, 16);
				glVertex2i( 0, 16 );
			tgglEnd();
		}
	//TEXTE
		//on change de blending, le noir des fonts ne devant pas être affiché
		tgglActiveTexture(GL_TEXTURE0);
		f->pMat->BindStage(0);
		
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		tgglColor4fv(&FGcolor.r);
		glCallLists( (GLsizei)strlen(c), GL_BYTE, c );
		tgglDisable(GL_TEXTURE_2D);
	tgglPopMatrix();
	tgglDisable (GL_BLEND);
}


void tgGLRenderer::_Begin2D ()
{
	tgglDisable (GL_DEPTH_TEST);
	tgglDisable (GL_CULL_FACE);
	tgglMatrixMode (GL_PROJECTION);
	tgglPushMatrix ();
	tgglLoadIdentity ();
	tgglOrtho (0, 1024, 768, 0, -1, 1);
	tgglMatrixMode (GL_MODELVIEW);
	tgglPushMatrix ();
	tgglLoadIdentity ();
}

void tgGLRenderer::_End2D()
{
	tgglMatrixMode( GL_PROJECTION );
	tgglPopMatrix( );
	tgglMatrixMode( GL_MODELVIEW );
	tgglPopMatrix ();
	
	tgglEnable (GL_DEPTH_TEST);
	tgglEnable (GL_CULL_FACE);
}

void tgGLRenderer::LoadFont(tgFont* f, const tgstring& sMatName)
{
	f->pMat = pMaterialManager->GetMaterial(sMatName);

	float cx;
	float cy;
	f->dListId = glGenLists (256);
	
	for (int i=0; i<256;i++)
	{
		cx = 1 - ( float )( i % 16 ) / 16.0f; //Ca depend de la façon dont est stockée la texture
		cy = ( float )( i / 16 ) / 16.0f;
		//cx = ( float )( i % 16 ) / 16.0f;
		//cy = ( float )( i / 16 ) / 16.0f;
		
		tgglNewList( f->dListId +  (255 - i) , GL_COMPILE );
			tgglBegin( GL_QUADS );
				tgglMultiTexCoord2f(GL_TEXTURE0, cx - 0.0625f, cy + 0.0625f );
				glVertex2i( 0, 0 );
				tgglMultiTexCoord2f(GL_TEXTURE0, cx, cy + 0.0625f );
				glVertex2i( 16, 0 );
				tgglMultiTexCoord2f(GL_TEXTURE0, cx, cy  );
				glVertex2i( 16, 16 );
				tgglMultiTexCoord2f(GL_TEXTURE0, cx - 0.0625f, cy);
				glVertex2i( 0, 16 );
			glEnd( );
			glTranslated( 15, 0, 0 );
		glEndList( );
	}
}

void tgGLRenderer::ReleaseFont(tgFont* f)
{
	tgglDeleteLists(f->dListId,256);
}

void tgGLRenderer::_TransformEntity (tgIEntity* e)
{
	if (e->IsViewModel()) { //on doit rendre le model sur la viewport
		tgglLoadIdentity();
	} 
	//FIXME: tout en un ?
	tgQuaternion quat = e->GetRotation();
	
	//TRES IMPORTANT, sans ca, ca peut partir en couille, notamment pasque la matrice commence a scaller l'anim. 
	//En normalizant, on s'assure qu'on ne fait QUE des rotations
	float matrix[16];
	quat.Normalize();
	e->SetRotation(quat);
	quat.GetMatrix(matrix);
	//on soustrait la translation du bone 'origin' (qui est exprime en coordonnes models, donc on le tourne), car les animations de marche par exemple, font avancer le model.
	tgVector3 toTranslate = e->GetPosOffset() + e->GetPosition() - quat.Rotate(e->GetOriginPosition());

	tgglTranslatef(toTranslate.x,toTranslate.y,toTranslate.z);
	
	
	tgglMultMatrixf(matrix);
}

void tgGLRenderer::_DrawAxis ()
{
	//X axis
	tgglColor3f(1,0,0);
	tgglBegin(GL_LINES);
	tgglVertex3f(0,0,0);
	tgglVertex3f(10,0,0);
	tgglEnd();
	
	//Y axis
	tgglColor3f(0,1,0);
	tgglBegin(GL_LINES);
	tgglVertex3f(0,0,0);
	tgglVertex3f(0,10,0);
	tgglEnd();
	
	//Z axis
	tgglColor3f(0,0,1);
	tgglBegin(GL_LINES);
	tgglVertex3f(0,0,0);
	tgglVertex3f(0,0,10);
	tgglEnd();
}

#if 0
void tgGLRenderer::_RenderModel(tgModel* m)
{
	
	tgglPointSize(20.0f);
	tgglColor3f(1,0,0);
	tgglBegin(GL_POINTS);
	tgglVertex3f(0,0,0);
	tgglEnd();
	tgglPointSize(1.0f);
		
	if(cvShowBVolumes.GetInt())
		_RenderBVolume(m->GetBoundingVolume());

	if (bWireframe) {
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		tgglLineWidth(1.0f);
	}

	static float counter = 45;
#define VAL 200
	
	vLight.Set(VAL*sin(counter), VAL*cos(counter) ,0);
	//counter += 0.01f;
	
	//vLight.Set(0,1,0);
	//pToonShader->SetUniform("lightDir",vLight.x,vLight.y,vLight.z);
	float pos[] = {vLight.x,vLight.y,vLight.z};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	
	
	tgglPushMatrix ();
	_TransformEntity(m);
	if(cvShowBVolumes.GetInt())
		_DrawAxis();
	
	tgglColor3f(1,1,1);
	
	tgglFrontFace(GL_CW);
	//m->FillEdgeModel();
	
	for (int i=0; i<m->iNumMeshes; i++) {
		
		const int count = m->pData->mesh_GetNumTris(i)*3;
		const unsigned int* indices = m->pData->mesh_GetIndexes(i);
		
		//m->pMaterialTab[i]->BindColor();

		
		//BEGIN Affichage des normals
		if(cvShowNormals.GetInt()) {
			tgglLineWidth(1.0f);
			tgglColor3f(0,0,1);
			for(int k=0; k<count; k++) {
				const tgVector3& position = m->pVerticesListTab[i][indices[k]].vPosition;
					tgglPushMatrix();
					tgglTranslatef(position.x,position.y,position.z);
					const tgVector3& normal = m->pVerticesListTab[i][indices[k]].vNormal;
				
				
					tgglBegin(GL_LINES);
						tgglVertex3f(0,0,0);
						tgglVertex3fv(&normal.x);
					tgglEnd();
				tgglPopMatrix();
			}
		}
		//END
		
		tgglColor3f(1,1,1);
		tgglActiveTexture(GL_TEXTURE0);
		
		pToonMaterial->BindColor();
		
		tgglActiveTexture(GL_TEXTURE1);
		tgglDisable(GL_TEXTURE_2D);
		if(cvToonShading.GetInt()) {
			pToonShader->Enable();
		}
		tgglEnableClientState(GL_VERTEX_ARRAY);
		tgglEnableClientState(GL_NORMAL_ARRAY);
		//tgglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		tgglVertexPointer(3, GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].vPosition);
		tgglNormalPointer(GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].vNormal);
		
		//tgglClientActiveTexture(GL_TEXTURE1);
		//tgglTexCoordPointer(2, GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].s);
		
		tgglDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,indices);
		//tgglDisableClientState(GL_TEXTURE_COORD_ARRAY);
		tgglDisableClientState(GL_NORMAL_ARRAY);
		tgglDisableClientState(GL_VERTEX_ARRAY);
		if(cvToonShading.GetInt()) {
			pToonShader->Disable();
		}
		
		
		 // NEHE STYLE CEL-SHADING, beaucoup plus rapide mais les outline ne bougent pas suivant la position de la lumiere
		//BEGIN OUTLINE
		tgglColor3f(0,0,0);
		tgglActiveTexture(GL_TEXTURE0);
		tgglDisable(GL_TEXTURE_2D);
		tgglDisable(GL_TEXTURE_1D);
		tgglActiveTexture(GL_TEXTURE1);
		tgglDisable(GL_TEXTURE_2D);
		tgglDisable(GL_TEXTURE_1D);
		
		glPolygonMode (GL_BACK, GL_LINE);
		glCullFace (GL_FRONT);
		
		tgglEnableClientState(GL_VERTEX_ARRAY);
		
		tgglVertexPointer(3, GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].vPosition);
		tgglDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,indices);
		tgglDisableClientState(GL_VERTEX_ARRAY);
		
		
		glPolygonMode (GL_BACK, GL_FILL);
		glCullFace(GL_BACK);
		//END OUTLINE
	}
		
	
	tgglFrontFace(GL_CCW);
	tgglPopMatrix ();
	
	if(bWireframe)
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void tgGLRenderer::_DrawMeshShadowVolume (tgModel* m,const tgVector3& objectLightPos)
{
	glBegin (GL_QUADS);
	for (unsigned int i=0; i<m->visibleEdges.size(); i++)
	{
		tgModel::_Edge& e = m->visibleEdges[i];
		tgVertice* pVerts = m->pVerticesListTab[e.iMesh];
		
		const tgVector3& v1 = pVerts[e.pVertIndexes[0]].vPosition;
		const tgVector3& v2 = pVerts[e.pVertIndexes[1]].vPosition;
		tgVector3 v3 ((v1 - objectLightPos)*TG_INFINITY);
		tgVector3 v4 ((v2 - objectLightPos)*TG_INFINITY);

		if (!e.bCCW)
		{
			glVertex3f( v2.x, v2.y, v2.z );
			glVertex3f( v1.x, v1.y, v1.z );
			glVertex3f( v1.x+v3.x, v1.y+v3.y, v1.z+v3.z );
			glVertex3f( v2.x+v4.x, v2.y+v4.y, v2.z+v4.z );
		}
		else
		{
			glVertex3f( v1.x, v1.y, v1.z );
			glVertex3f( v2.x, v2.y, v2.z );
			glVertex3f( v2.x+v4.x, v2.y+v4.y, v2.z+v4.z );
			glVertex3f( v1.x+v3.x, v1.y+v3.y, v1.z+v3.z );
		}
	}
	glEnd ();
}

void tgGLRenderer::_RenderModelShadow (tgModel* m)
{
	tgglPushMatrix ();
	_TransformEntity(m);
	//_DrawAxis();
	
	m->ComputeSilhouette(vLight);
	//BEGIN EDGE DEBUG
	#if 0
	glColor3f(1,0,0);
	glBegin (GL_LINES);
	
	for (unsigned int i=0; i<m->visibleEdges.size(); i++)
	{
		if (m->visibleEdges[i].pTriangleIndexes[1] == -1)
			glColor3f(0, 0, 1);
		else
			glColor3f(1, 0, 0);
		glVertex3fv( &m->pVerticesListTab[m->visibleEdges[i].iMesh][m->visibleEdges[i].pVertIndexes[0]].vPosition.x );
		glVertex3fv( &m->pVerticesListTab[m->visibleEdges[i].iMesh][m->visibleEdges[i].pVertIndexes[1]].vPosition.x );
	}
	
	glEnd ();
	#endif
	//END EDGE DEBUG
	
	//BEGIN SHADOW
	
	/**
	* FIXME : ON DOIT CAPER LES VOLUMES POUR QUE LA METHODE CARMACK FONCTIONNE
	*/
	glColor4f(0.7f,0.4f,0.0f,1.0f);

	if(cvDebugStencil.GetInt()) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1,0,0,0.5f);
		glCullFace(GL_BACK);
		_DrawMeshShadowVolume(m, vLight);
		glColor4f(0,1,0,0.5f);
		glCullFace(GL_FRONT);
		_DrawMeshShadowVolume(m, vLight);
		glDisable (GL_BLEND);
		glCullFace(GL_BACK);
	} else {
		glEnable(GL_CULL_FACE);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glClear(GL_STENCIL_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilMask(GL_TRUE);

		glFrontFace(GL_CCW);
		glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
		_DrawMeshShadowVolume(m, vLight);
		
		glFrontFace(GL_CW);
		glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
		_DrawMeshShadowVolume(m, vLight);
		
		glFrontFace(GL_CCW);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glDepthMask(GL_TRUE);
		glColor4f(0.0f,0.0f,0.0f,0.4f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glStencilFunc(GL_NOTEQUAL,0,0xffffffff);
		glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
		glPushMatrix();
			glLoadIdentity();
			glBegin(GL_TRIANGLE_STRIP);
				glVertex3f( -1.1f,  1.1f, -2.5f );
				glVertex3f( -1.1f, -1.1f, -2.5f );
				glVertex3f(  1.1f,  1.1f, -2.5f );
				glVertex3f(  1.1f, -1.1f, -2.5f );
			glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		/*glDisable(GL_STENCIL_TEST);
		glDepthMask(GL_TRUE);*/
		glPopAttrib();
	}

	
	
	//END SHADOW
	
	
	tgglPopMatrix();
}

#else
void tgGLRenderer::_RenderModel (tgModel* m)
{
	/*if (!pRenderView->frustum.PointInside(tgVector3(0,0,0)))
		return;*/
	//out << "Rendering model" << endl;

	
	
	tgglPointSize(20.0f);
	tgglColor3f(1,0,0);
	tgglBegin(GL_POINTS);
	tgglVertex3f(0,0,0);
	tgglEnd();
	tgglPointSize(1.0f);
		
	if(cvShowBVolumes.GetInt())
		_RenderBVolume(m->GetBoundingVolume());

	if (bWireframe) {
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		tgglLineWidth(1.0f);
	}

	tgglActiveTexture(GL_TEXTURE1);
	tgglDisable(GL_TEXTURE_2D);
	tgglActiveTexture(GL_TEXTURE0);
	tgglClientActiveTexture(GL_TEXTURE0);
	
	
	
	tgglPushMatrix ();
		_TransformEntity(m);
		if(cvShowBVolumes.GetInt())
			_DrawAxis();
	
		tgglColor3f(1,1,1);
		tgglFrontFace(GL_CW);
		tgglEnableClientState(GL_VERTEX_ARRAY);
		tgglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		for (int i=0; i<m->iNumMeshes; i++) {
      if (!m->pVisibleMeshes[i])
        continue;

			m->pMaterialTab[i]->BindStage(0);

			tgglVertexPointer(3, GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].vPosition);

			tgglTexCoordPointer(2, GL_FLOAT, sizeof(tgVertice), &m->pVerticesListTab[i][0].s);

			tgglDrawElements(GL_TRIANGLES, m->pData->mesh_GetNumTris(i)*3, GL_UNSIGNED_INT,m->pData->mesh_GetIndexes(i));
		}
		
		tgglDisableClientState(GL_VERTEX_ARRAY);
		tgglDisableClientState(GL_TEXTURE_COORD_ARRAY);
		tgglFrontFace(GL_CCW);
	tgglPopMatrix ();
	
	if(bWireframe)
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	tgglDisable(GL_TEXTURE_2D);
}
#endif

void tgGLRenderer::_RenderBVolume(tgBoundingVolume* b)
{
	if(b == NULL)
		return;
	tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/*tgglActiveTexture(GL_TEXTURE0);
	tgglDisable(GL_TEXTURE_2D);
	
	tgglActiveTexture(GL_TEXTURE1);
	tgglDisable(GL_TEXTURE_2D);*/
	tgglDisable(GL_CULL_FACE);
	
	tgglColor3f(0.5,0.5,0.5);
		tgglLineWidth(5.0f);
	
	if(b->GetType() == TG_AABBOX) {
		tgAABoundingBox* box = static_cast<tgAABoundingBox*>(b);
		
		tgglBegin (GL_TRIANGLES);
		for (int i=0; i<6; i++)
		{
			tgglVertex3fv (&(box->GetPlane(i).v[0].x));
			tgglVertex3fv (&(box->GetPlane(i).v[1].x));
			tgglVertex3fv (&(box->GetPlane(i).v[2].x));
		}
		tgglEnd ();
		
	} else if (b->GetType() == TG_BSPHERE) {
		tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(b);
		tgglPushMatrix();
		tgglTranslatef(sphere->GetPosition().x,sphere->GetPosition().y,sphere->GetPosition().z);
		GLUquadric* myQuad = gluNewQuadric();
		gluSphere(myQuad, sphere->GetRadius(), 10, 10);
		gluDeleteQuadric(myQuad);
		tgglPopMatrix();
	} 
	tgglColor3f(1,1,1);
	/*tgglActiveTexture(GL_TEXTURE0);
	tgglEnable(GL_TEXTURE_2D);
	
	tgglActiveTexture(GL_TEXTURE1);
	tgglEnable(GL_TEXTURE_2D);*/
	
	tgglEnable(GL_CULL_FACE);
	tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void tgGLRenderer::_DebugCollisions()
{
	tgglActiveTexture(GL_TEXTURE1);
	tgglDisable(GL_TEXTURE_2D);
	tgglActiveTexture(GL_TEXTURE0);
	tgglDisable(GL_TEXTURE_2D);

	tgCollisionResult trace;

	/*trace = pWorldManager->Trace(pRenderView->GetOrigin(), pRenderView->GetOrigin()-pRenderView->qRotation.GetZAxis()*500000,NULL,NULL);

	tgglColor3f(1,0,0);
	tgglPushMatrix();
	tgglTranslatef(trace.vEndPoint.x, trace.vEndPoint.y, trace.vEndPoint.z);
	GLUquadric* pquad =  gluNewQuadric();
	tgglFrontFace(GL_CCW);
	if (pquad) {
		gluSphere(pquad, 10, 5, 5);
		gluDeleteQuadric(pquad);
	}

	tgglPopMatrix();*/
	
	tgAABoundingBox box(tgVector3(-30,-30,-30), tgVector3(30,30,30));
	//box.SetPosition(pRenderView->GetOrigin());
	trace = pWorldManager->Trace(pRenderView->GetFinalPosition(), pRenderView->GetFinalPosition()-pRenderView->GetFinalRotation().GetZAxis()*500000,&box,NULL);
	box.SetPosition(trace.vEndPoint);
	_RenderBVolume(&box);
	//Render collision Normal
	tgglPushMatrix();
		tgglTranslatef(trace.vEndPoint.x, trace.vEndPoint.y, trace.vEndPoint.z);
		tgglBegin(GL_LINES);
		tgglVertex3f(0,0,0);
		tgVector3 scaledNormal = trace.vColNormal*10;
		//out << trace.vColNormal << endl;
		tgglVertex3fv(&scaledNormal.x);
		tgglEnd();
	tgglPopMatrix();
	/**FACE MARKINGS
	 *
	 */
	//tgBoundingSphere* temp = new tgBoundingSphere(trace.vEndPoint, 50);
	/*l->GetFacesMarked(temp);
	_RenderBVolume(temp);*/
	//delete temp;
	/** */
}

#if 0
void tgGLRenderer::_RenderParticleEmitter (tgParticleEmitter* emit)
{
  tgout << "rendering emitter" << endl;
  //FIXME: doit etre gere comme une entity (_TransformEntity)
  //attenuation quadratic pour les points
  /*			                  1
	dist_atten(d) = -------------------
			            a + b * d + c * d^2
  */
  const float quadratic[] =  { 0.0f, 0.0f, 0.01f };
  tgglPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

  tgglTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

  float maxSize = 0.0f;
  glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
  glPointSize( maxSize );
  glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
  glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
  glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
  cout << maxSize << endl;

  tgglPushMatrix();


  emit->GetMaterial()->BindStage(0);

  tgglEnable (GL_POINT_SPRITE_ARB); 
  tgglBegin(GL_POINTS);
  tgout << emit->particlesList.size() << endl;
  for (tgParticleEmitter::particlesList_t::iterator i=emit->particlesList.begin(); 
       i!= emit->particlesList.end(); i++) {
    tgParticle* p = *i;
    tgglPointSize(p->size);
    tgglColor4fv (p->color);
    tgglVertex3f(p->vPosition.x, p->vPosition.y, p->vPosition.z);
  }
  tgglEnd();
  tgglDisable (GL_POINT_SPRITE_ARB);

  tgglPopMatrix();

}
#else
//TODO il faut desactiver le depth test et rendre les particules dans l'ordre avec le reste
//de la geometrie si on veut eviter le clipping moche
// eventuellement des point sprites pour les petites particules
void tgGLRenderer::_RenderParticleEmitter (tgParticleEmitter* emit)
{
  //TODO: Le rendu des particules en fil de fer texture donne qq chose de vraiment pas mal !
  //les particles "regardent" toujours la camera
  tgVector3 campos = pRenderView->GetFinalPosition();
  tgVector3 view = pRenderView->GetFinalRotation().GetZAxis(); //pCamera->GetForward();
  tgVector3 up = pRenderView->GetFinalRotation().GetYAxis(); //pCamera->GetUp();
  //en fait, on veut dessiner le carre de la particule sur des axes parallelles aux bords de l'¿cran
  // le produit vectoriel entre le vector up et view nous donne l'axe horizontal
  //le produi vectoriel entre la vue et l'horizontal nous donne la vertical
  //dans notre nouvelle base, view est l'axe des z, horiz les x et vert les y
  tgVector3 oHoriz = up % view;
  oHoriz.Normalize();
  tgVector3 oVert = oHoriz % view;
  oVert.Normalize();

  //on multiplie view par -1 pour avoir la bonne normale (view va dans le sens de la "vue")
  view *= -1;

  tgglDepthMask(GL_FALSE);
  tgglDisable(GL_CULL_FACE);
  tgglEnable(GL_BLEND);
  tgglBlendFunc( GL_SRC_ALPHA, GL_ONE );

  emit->GetMaterial()->BindStage(0);

  for (tgParticleEmitter::particlesList_t::iterator i=emit->particlesList.begin(); 
      i!= emit->particlesList.end(); i++) {
    tgParticle* p = *i;
    if (!p->isAlive) 
      continue;

    tgglPushMatrix ();

    tgVector3 horiz = oHoriz*0.5*p->size;
    tgVector3 vert = oVert*0.5*p->size;

    tgglColor4fv (p->color);
    glTranslatef(p->vPosition.x, p->vPosition.y, p->vPosition.z);

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glNormal3f(view.x, view.y, view.z);glVertex3f(horiz.x+vert.x, horiz.y+vert.y, horiz.z+vert.z);
    glTexCoord2f(1,0);glNormal3f(view.x, view.y, view.z);glVertex3f(horiz.x-vert.x, horiz.y-vert.y, horiz.z-vert.z);
    glTexCoord2f(1,1);glNormal3f(view.x, view.y, view.z);glVertex3f(-horiz.x-vert.x, -horiz.y-vert.y, -horiz.z-vert.z);
    glTexCoord2f(0,1);glNormal3f(view.x, view.y, view.z);glVertex3f(-horiz.x+vert.x, -horiz.y+vert.y,-horiz.z+vert.z);
    glEnd();
    glPopMatrix ();
  }


  tgglDepthMask (GL_TRUE);
  tgglEnable(GL_CULL_FACE);
  tgglDisable(GL_BLEND);
}
#endif

void _DLightFace(tgVector3 verts[3], const tgVector3& position, const tgColor& color, float radius) 
{
//TODO: vertex-shader version
	float iradius = 1.0f/radius;
	float u[3];
	float v[3];
	
	float floatColor[3];
	floatColor[0] = color.r*255.0f;
	floatColor[1] = color.g*255.0f;
	floatColor[2] = color.b*255.0f;

	GLubyte pcolor[3][4];
	int clip[3] = {0,0,0};
	//pour chaque vertex de la face
	for(int j=0; j<3; j++) {
		float modulate;
		//on calcul la distance light - face
		tgVector3 vDist = position - verts[j];
		//TODO: Si la light est derriere la surface, il ne faut pas eclairer (simple test dist, normal face)
		//les coords de texture en u/v sont la distance sur les x/z divise par le rayon
		u[j] = 0.5f + vDist.x*iradius;
		v[j] = 0.5f + vDist.z*iradius;

		if(u[j] < 0.0f) clip[j] |= 1;
		else if (u[j] > 1.0f) clip[j] |= 2;

		if(v[j] < 0.0f) clip[j] |= 4;
		else if (v[j] > 1.0f) clip[j] |= 8;

		//la distance sur les y influence la modulation de la couleur
		if(vDist.y > radius) {
			clip[j] |= 16;
			modulate = 0.0f;
		} else if (vDist.y < -radius) {
			clip[j] |= 32;
			modulate = 0.0f;
		} else {
			vDist.y = fabs(vDist.y);
			if(vDist.y < radius*0.5f) {
				modulate = 1.0f;
			} else {
				modulate = 2.0f*(radius-vDist.y)*iradius;
			}
		}
		pcolor[j][0] = (GLubyte)(floatColor[0]*modulate);
		pcolor[j][1] = (GLubyte)(floatColor[1]*modulate);
		pcolor[j][2] = (GLubyte)(floatColor[2]*modulate);
		pcolor[j][3] = 255;
	}

	if(clip[0] & clip[1] & clip[2]) return;
//TODO: Optimizer ca, en rendant le tout en une seul passe via des vertex array et en evitant le remplissage d'un tableau 'tab'
	glBegin(GL_TRIANGLES);
	glTexCoord2f(u[0],v[0]);
	tgglColor4ubv(pcolor[0]);
	glVertex3fv(&verts[0].x);

	glTexCoord2f(u[1],v[1]);
	tgglColor4ubv(pcolor[1]);
	glVertex3fv(&verts[1].x);

	glTexCoord2f(u[2],v[2]);
	tgglColor4ubv(pcolor[2]);
	glVertex3fv(&verts[2].x);
	glEnd();

}

void tgGLRenderer::_DynamicLight(tgQ3Level* l,tgLight* light)
{
  tgglFrontFace(GL_CW);
	tgglActiveTexture(GL_TEXTURE1);
	tgglDisable(GL_TEXTURE_2D);
	tgglActiveTexture(GL_TEXTURE0);
	tgglDisable(GL_TEXTURE_2D);
	pLightMaterial->BindStage(0);
	tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	tgglEnable(GL_BLEND);
//	tgglBlendFunc(GL_DST_COLOR,GL_ONE);
	tgglBlendFunc(GL_ONE,GL_ONE);

	tgBoundingSphere lightSphere(light->GetPosition(),light->GetRadius());	

	//on marque les faces qui sont contenues dans la lightsphere
	l->FindSphereContainedFaces(lightSphere);

	for(int i=0; i<l->numOfFaces; i++) {
		if(!l->pVisibleFaces[i])
			continue;
		if(l->pDirectory[i].type == Q3Defs::PatchFace) {
			Q3Defs::BspPatch* p = &l->pPatches[l->pDirectory[i].typeIndex];
			if(!lightSphere.VolumeInside(&p->sphere)) continue;

			for(unsigned int k=0;k<p->iNumIndices; k+=3) {
				tgVector3 tab[3];
				tab[0] = p->pVertices[p->pIndices[k]].vPosition;
				tab[1] = p->pVertices[p->pIndices[k+1]].vPosition;
				tab[2] = p->pVertices[p->pIndices[k+2]].vPosition;
				_DLightFace(tab,light->GetPosition(),light->GetColor(),light->GetRadius());


			}
		} else if(l->pDirectory[i].type == Q3Defs::PolygonFace) {
			Q3Defs::BspPolygonFace* pFace = &l->pPolygonFaces[l->pDirectory[i].typeIndex];
			if(!lightSphere.VolumeInside(&pFace->sphere)) continue;

			for(int k=0; k<pFace->numOfMeshVerts; k+=3) {
				tgVector3 tab[3];
				tab[0] = l->pVerts[pFace->startVertIndex+l->pMeshIndices[pFace->firstMeshIndex+k]].vPosition;
				tab[1] = l->pVerts[pFace->startVertIndex+l->pMeshIndices[pFace->firstMeshIndex+k+1]].vPosition;
				tab[2] = l->pVerts[pFace->startVertIndex+l->pMeshIndices[pFace->firstMeshIndex+k+2]].vPosition;
				_DLightFace(tab,light->GetPosition(),light->GetColor(),light->GetRadius());
			}
		} else if(l->pDirectory[i].type ==  Q3Defs::MeshFace) {
			Q3Defs::BspMeshFace* pMesh = &l->pMeshFaces[l->pDirectory[i].typeIndex];
			if(!lightSphere.VolumeInside(&pMesh->sphere)) continue;

			for(int k=0; k<pMesh->numOfMeshVerts; k+=3) {
				tgVector3 tab[3];
				tab[0] = l->pVerts[pMesh->startVertIndex+l->pMeshIndices[pMesh->firstMeshIndex+k]].vPosition;
				tab[1] = l->pVerts[pMesh->startVertIndex+l->pMeshIndices[pMesh->firstMeshIndex+k+1]].vPosition;
				tab[2] = l->pVerts[pMesh->startVertIndex+l->pMeshIndices[pMesh->firstMeshIndex+k+2]].vPosition;
				_DLightFace(tab,light->GetPosition(),light->GetColor(),light->GetRadius());
			}
		}

	}
	tgglColor4f(1,1,1,1);
	tgglDisable(GL_BLEND);
  tgglFrontFace(GL_CCW);
}

void tgGLRenderer::_RenderQ3Level (tgQ3Level* l)
{
	l->FindVisibleFaces(*pRenderView);

	if(cvDebugCollisions.GetInt())
		_DebugCollisions();

	tgglColor3f(1,1,1);
	tgglFrontFace(GL_CW);
	tgglEnableClientState(GL_VERTEX_ARRAY);
	
	tgglClientActiveTexture(GL_TEXTURE0);
	tgglActiveTexture(GL_TEXTURE0);
	tgglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	tgglEnable(GL_TEXTURE_2D);
	
	tgglClientActiveTexture(GL_TEXTURE1);
	tgglActiveTexture(GL_TEXTURE1);
	tgglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	tgglEnable(GL_TEXTURE_2D);
	
	int numPolyRend = 0;
	int numPatchRend = 0;
	int numMeshRend = 0;

	if(bWireframe) {
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		tgglLineWidth(1.0f);
	}
	
	for(int i=0; i<l->numOfFaces; i++) {
		if(!l->pVisibleFaces[i])
			continue;
		
		if (l->pDirectory[i].type == Q3Defs::PolygonFace) {
			Q3Defs::BspPolygonFace* pFace = &l->pPolygonFaces[l->pDirectory[i].typeIndex];
			
			tgglActiveTexture(GL_TEXTURE0);
			l->pMaterials[pFace->textureID]->BindStage(0);
			tgglActiveTexture(GL_TEXTURE1);
      if (pFace->lightmapID != -1)
  			tgglBindTexture(GL_TEXTURE_2D,  l->pLightMaps[pFace->lightmapID]);
			
			tgglClientActiveTexture(GL_TEXTURE0);
			tgglTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), l->pVerts[pFace->startVertIndex].vTextureCoord);
			tgglClientActiveTexture(GL_TEXTURE1);
			tgglTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), l->pVerts[pFace->startVertIndex].vLightmapCoord);
			
			glVertexPointer(3, GL_FLOAT, sizeof(Q3Defs::BspVertex), &l->pVerts[pFace->startVertIndex].vPosition);
			tgglDrawElements (GL_TRIANGLES, pFace->numOfMeshVerts, GL_UNSIGNED_INT, &l->pMeshIndices[pFace->firstMeshIndex]);

			
			numPolyRend++;
			tgglColor3f(1,1,1);
			continue;
		}
		else if (l->pDirectory[i].type == Q3Defs::PatchFace)
		{
		//	tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	tgglLineWidth(1.0f);
			Q3Defs::BspPatch* p = &l->pPatches[l->pDirectory[i].typeIndex];
			
			tgglActiveTexture(GL_TEXTURE0);
			l->pMaterials[p->textureID]->BindStage(0);
			tgglActiveTexture(GL_TEXTURE1);
      if (p->lightmapID != -1)
  			glBindTexture(GL_TEXTURE_2D,  l->pLightMaps[p->lightmapID]);
			
			tgglClientActiveTexture(GL_TEXTURE0);
			tgglTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), p->pVertices[0].vTextureCoord);
			tgglClientActiveTexture(GL_TEXTURE1);
			tgglTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), p->pVertices[0].vLightmapCoord);
			
			tgglVertexPointer(3, GL_FLOAT, sizeof(Q3Defs::BspVertex), &p->pVertices[0].vPosition);
			tgglDrawElements(GL_TRIANGLES, p->iNumIndices, GL_UNSIGNED_INT, &p->pIndices[0]);

		/*	tgglActiveTexture(GL_TEXTURE0);
			tgglDisable(GL_TEXTURE_2D);
			tgglActiveTexture(GL_TEXTURE1);
			tgglDisable(GL_TEXTURE_2D);
			tgglColor3f(0,1,0);
			tgglPointSize(10.0f);
			tgglBegin(GL_POINTS);
			for(int j=0; j<p->size[0]*p->size[1]; j++) {
				tgglVertex3fv(&p->controls[j].vPosition.x);
			}
			tgglEnd();
			
			tgglActiveTexture(GL_TEXTURE1);
			tgglEnable(GL_TEXTURE_2D);
			tgglActiveTexture(GL_TEXTURE0);
			tgglEnable(GL_TEXTURE_2D);
			tgglColor3f(1,1,1);*/
		//	tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//_RenderBVolume(p->pBBox);
			numPatchRend++;
			continue;
		}
		else if (l->pDirectory[i].type ==  Q3Defs::MeshFace)
		{
			Q3Defs::BspMeshFace* pMesh = &l->pMeshFaces[l->pDirectory[i].typeIndex];
			tgglActiveTexture(GL_TEXTURE0);
			l->pMaterials[pMesh->textureID]->BindStage(0);
			tgglActiveTexture(GL_TEXTURE1);
			if (pMesh->lightmapID>=0)
				tgglBindTexture (GL_TEXTURE_2D, l->pLightMaps[pMesh->lightmapID]);
			else {
			//	glBindTexture (GL_TEXTURE_2D, pTexManager->GetWhite());
				tgglDisable(GL_TEXTURE_2D);
			}
			
			tgglClientActiveTexture(GL_TEXTURE0_ARB);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), l->pVerts[pMesh->startVertIndex].vTextureCoord);
			tgglClientActiveTexture(GL_TEXTURE1_ARB);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Q3Defs::BspVertex), l->pVerts[pMesh->startVertIndex].vLightmapCoord);
			
			glVertexPointer(3, GL_FLOAT, sizeof(Q3Defs::BspVertex), &l->pVerts[pMesh->startVertIndex].vPosition);
			
			glDrawElements (GL_TRIANGLES, pMesh->numOfMeshVerts, GL_UNSIGNED_INT, &l->pMeshIndices[pMesh->firstMeshIndex]);
			numMeshRend++;
			//au cas ou on l'aurait disable ci-dessus
			if (pMesh->lightmapID<0) {
				tgglActiveTexture(GL_TEXTURE1);
				tgglEnable(GL_TEXTURE_2D);
			}
		}
	}

	
	if(bWireframe)
		tgglPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	//rendered poly stats
	std::ostringstream stream;
	stream << "poly: " << numPolyRend << ", patch: " << numPatchRend << ", mesh: " << numMeshRend << endl;
	pCanvas->DisplayText(stream.str());
	std::ostringstream stream2;
	stream2 << "total: " << numPolyRend+numPatchRend+numMeshRend << endl;
	pCanvas->DisplayText(stream2.str());
	std::ostringstream stream3;
	stream3 << "TraceAgainstTriangle: " << l->TraceAgainstTriangleCount << endl;
	pCanvas->DisplayText(stream3.str());
	
	
	
	tgglDisableClientState(GL_VERTEX_ARRAY);
	tgglDisableClientState(GL_TEXTURE_COORD_ARRAY);
	tgglClientActiveTexture(GL_TEXTURE0);
	tgglDisableClientState(GL_TEXTURE_COORD_ARRAY);

	tgglFrontFace(GL_CCW);
}

void tgGLRenderer::_RenderDecals (tgDecalManager* m)
{
	tgglFrontFace(GL_CW);
	tgglDisable(GL_CULL_FACE);
		
	tgglColor3f(1,1,1);
	if (bWireframe) {
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		tgglLineWidth(1.0f);
	} else {
		tgglEnable(GL_BLEND);
    tgglBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
		tgglActiveTexture(GL_TEXTURE1);
		tgglDisable(GL_TEXTURE_2D);
		tgglActiveTexture(GL_TEXTURE0);
		tgglEnable(GL_TEXTURE_2D);
	}
	
	
	for(unsigned i=0; i<m->GetNumDecals(); i++) {
		tgDecal* d = m->GetDecal(i);
		if(!bWireframe)
			d->GetMaterial()->BindStage(0);
		tgglBegin(GL_TRIANGLES);
		for(unsigned j=0; j<d->GetNumPoints(); j++) {
			if(!bWireframe)
				tgglMultiTexCoord2f(GL_TEXTURE0,d->GetVertice(j).s,d->GetVertice(j).t);
			tgglVertex3fv(&d->GetVertice(j).vPosition.x);
		}
		tgglEnd();
	}
	
	
	if (bWireframe) {
		tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {
		tgglDisable(GL_BLEND);
		tgglDisable(GL_TEXTURE_2D);
		tgglActiveTexture(GL_TEXTURE0);
		tgglDisable(GL_TEXTURE_2D);
	}
	tgglEnable(GL_CULL_FACE);
	tgglFrontFace(GL_CW);
}

#if 0
void tgGLRenderer::_RenderD3Level(tgDoom3Level* l)
{
	/*if(pRenderView)
		l->MarkVisibleModels(pTestRenderView->vOrigin);*/
	l->RenderAreas(pTestRenderView);
	l->MarkVisibleModels();
		
	tgglActiveTexture(GL_TEXTURE1);
	tgglDisable(GL_TEXTURE_2D);
	tgglActiveTexture(GL_TEXTURE0);
	
	tgglFrontFace(GL_CW);
	//glDisable(GL_CULL_FACE);

	tgglLineWidth(1.0f);
	//tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	tgglColor3f(1,1,1);
	tgglEnableClientState(GL_VERTEX_ARRAY);
	tgglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	for(int i=0; i<l->iNumStaticModels; i++) {
		tgDoom3Level::_StaticModel& model = l->pStaticModels[i];
		if(!model.visible)
			continue;
		tgglPushMatrix ();
	
		tgglTranslatef(model.vOrigin.x, model.vOrigin.y, model.vOrigin.z);
		for (int k=0; k<model.numSurfaces; k++) {
			tgDoom3Level::_Surface& surf = model.pSurfaces[k];
			
			surf.pMaterial->BindColor();
			
			/*tgglBegin(GL_TRIANGLES);
			for (int i=0; i<surf.numIndexes; i++) {
				//float c = (float)i/surf.numIndexes;
				//tgglColor3f(c,c/3,c/5);
				glTexCoord2fv(&surf.pVertices[surf.pIndexes[i]].s);
				//glTexCoord2f(c, c/5);
				tgglVertex3fv(&surf.pVertices[surf.pIndexes[i]].vPosition.x);
			}
			tgglEnd();*/
			tgglTexCoordPointer(2, GL_FLOAT, sizeof(tgVertice), &surf.pVertices[0].s);
			tgglVertexPointer(3, GL_FLOAT, sizeof(tgVertice), &surf.pVertices[0].vPosition );
			tgglDrawElements(GL_TRIANGLES, surf.numIndexes, GL_UNSIGNED_INT, surf.pIndexes);
		}
		tgglPopMatrix ();
		//glColor3f(1,1,1);
	}
	tgglColor3f(1,1,1);
	tgglDisableClientState(GL_VERTEX_ARRAY);
	tgglDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	tgglDisable(GL_TEXTURE_2D);
	tgglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	tgglDisable(GL_CULL_FACE);
	tgglLineWidth(3.0f);
	
	//Rendu des portals
	/*int area =l->_FindArea(pTestRenderView->vOrigin);
	RenderDebugFrustum(&pTestRenderView->frustum);
	l->_RenderArea(pTestRenderView->frustum, area, -1);*/
	int cnt =0;
	for(int i=0; i<l->iNumPortals; i++) {
		tgDoom3Level::_Portal& portal = l->pPortals[i];
		if (portal.visible) {
			tgglColor3f(0,1,0);
			cnt++;
		} else {
			tgglColor3f(1,0,0);
		}
		tgglBegin(GL_QUADS);
		for (int j=0;j<portal.iNumPoints; j++) {
			tgglVertex3fv(&portal.pPoints[j].x);
		}
		tgglEnd();
	}
	//cout << "visible : " << cnt << endl;
	tgglLineWidth(1.0f);
	tgglEnable(GL_CULL_FACE);
	tgglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
}
#endif
void tgGLRenderer::RenderDebugFrustum (tgFrustum* pFrustum)
{
	/*glPointSize(10.0f);
	tgglBegin(GL_POINTS);
	for(int i=0; i<pFrustum->iNumPlanes; i++) {
			tgglVertex3fv(&pFrustum->pPlanes[i].v[0].x);
			tgglVertex3fv(&pFrustum->pPlanes[i].v[1].x);
			tgglVertex3fv(&pFrustum->pPlanes[i].v[2].x);
	}
	tgglEnd();*/
	glPointSize(10.0f);
	for(int i=0; i<6; i++) {
		glBegin(GL_POINTS);
			glVertex3fv(&pFrustum->pPlanes[i].v[0].x);
			glVertex3fv(&pFrustum->pPlanes[i].v[1].x);
			glVertex3fv(&pFrustum->pPlanes[i].v[2].x);
		glEnd();
	}
	glBegin(GL_TRIANGLES);
	glVertex3fv(&pFrustum->pPlanes[4].v[0].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[1].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[0].x);
	
	glVertex3fv(&pFrustum->pPlanes[4].v[0].x);
	glVertex3fv(&pFrustum->pPlanes[4].v[1].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[1].x);
	
	
	glVertex3fv(&pFrustum->pPlanes[4].v[1].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[1].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[2].x);
	
	glVertex3fv(&pFrustum->pPlanes[4].v[1].x);
	glVertex3fv(&pFrustum->pPlanes[4].v[2].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[2].x);
	
	glVertex3fv(&pFrustum->pPlanes[4].v[2].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[2].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[0].x);
	
	glVertex3fv(&pFrustum->pPlanes[4].v[2].x);
	glVertex3fv(&pFrustum->pPlanes[4].v[0].x);
	glVertex3fv(&pFrustum->pPlanes[5].v[0].x);
	glEnd();
}

void tgGLRenderer::_SetupView (tgRenderView* r)
{
	tgglMultMatrixf(r->GetMatrix());
	tgglTranslatef(-r->GetFinalPosition().x, -r->GetFinalPosition().y, -r->GetFinalPosition().z);
	//tgVector3 viewDiff = r->GetOrigin() - r->GetLastOrigin();
	//FIXME: c'est sense 'amortir' les montes des escaliers
	//tgglTranslatef(0,viewDiff.y,0);
	//out << viewDiff << endl;
	
	_ExtractFrustum();
	
	/*glPushMatrix ();
		tgglTranslatef(r->vOrigin.x, r->vOrigin.y, r->vOrigin.z);
		glMultMatrixf(r->matrix);
		glPointSize(10.0f);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
			glVertex3f(0,0,0);
		glEnd ();
		glBegin(GL_LINES);
			glColor3f(0,1,0);
			glVertex3f(0,0,0);
			glVertex3f(10,0,0);
			glColor3f(0,0,1);
			glVertex3f(0,0,0);
			glVertex3f(0,0,10);
		glEnd();
		glColor3f(1,1,1);
	glPopMatrix ();*/
}

void tgGLRenderer::_GLUPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xmin, xmax, ymin, ymax;
	
	ymax = zNear * tan( fovy * TG_PI / 360.0 );
	ymin = -ymax;

	xmin = ymin * aspect;
	xmax = ymax * aspect;
	
	/*xmin += -( 2 * gl_state.camera_separation ) / zNear;
	xmax += -( 2 * gl_state.camera_separation ) / zNear;*/
	
	tgglFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void tgGLRenderer::_ExtractFrustum ()
{
   float   proj[16];
   float   modl[16];
   float   clip[16];
   float   t;
   float   frustum[6][4];

   /* Get the current PROJECTION matrix from OpenGL */
   glGetFloatv( GL_PROJECTION_MATRIX, proj );

   /* Get the current MODELVIEW matrix from OpenGL */
   glGetFloatv( GL_MODELVIEW_MATRIX, modl );

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   frustum[0][0] = clip[ 3] - clip[ 0];
   frustum[0][1] = clip[ 7] - clip[ 4];
   frustum[0][2] = clip[11] - clip[ 8];
   frustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip[ 3] + clip[ 0];
   frustum[1][1] = clip[ 7] + clip[ 4];
   frustum[1][2] = clip[11] + clip[ 8];
   frustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   frustum[2][0] = clip[ 3] + clip[ 1];
   frustum[2][1] = clip[ 7] + clip[ 5];
   frustum[2][2] = clip[11] + clip[ 9];
   frustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;

   /* Extract the TOP plane */
   frustum[3][0] = clip[ 3] - clip[ 1];
   frustum[3][1] = clip[ 7] - clip[ 5];
   frustum[3][2] = clip[11] - clip[ 9];
   frustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;

   /* Extract the FAR plane */
   frustum[4][0] = clip[ 3] - clip[ 2];
   frustum[4][1] = clip[ 7] - clip[ 6];
   frustum[4][2] = clip[11] - clip[10];
   frustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;

   /* Extract the NEAR plane */
   frustum[5][0] = clip[ 3] + clip[ 2];
   frustum[5][1] = clip[ 7] + clip[ 6];
   frustum[5][2] = clip[11] + clip[10];
   frustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;
   
   tgPlane pPlanes[6];
   for(int i=0; i<6; i++) {
   	pPlanes[i] = tgPlane(tgVector3(frustum[i][0], frustum[i][1], frustum[i][2]), frustum[i][3]);
   }
   
   tgFrustum& f = pRenderView->GetFrustum();
   f.SetPlane(0, pPlanes[0]);
   f.SetPlane(1, pPlanes[1]);
   f.SetPlane(2, pPlanes[2]);
   f.SetPlane(3, pPlanes[3]);
   f.SetPlane(4, pPlanes[4]);
   f.SetPlane(5, pPlanes[5]);
}



tgGLRenderer::tgGLSL::tgGLSL(const tgstring& filename)
{
	int file = 0;
	tgstring vertexShaderSource;
	if ((file = pFileManager->Open(filename.data(),TGFILE_TEXT)) == -1)
		    tgout << "impossible de trouver le fichier: " << filename << endl;
	
	vertexShaderSource = _ReadFromTextFile(file);
	vertexShader = pGLRenderer->tgglCreateShaderObject(GL_VERTEX_SHADER_ARB);
	
	const char* src = vertexShaderSource.data();
	pGLRenderer->tgglShaderSource(vertexShader, 1, &src,NULL);
	
	pGLRenderer->tgglCompileShader(vertexShader);
	
	program = pGLRenderer->tgglCreateProgramObject();
	pGLRenderer->tgglAttachObject(program, vertexShader);
	pGLRenderer->tgglLinkProgram(program);	
	
	_PrintLog(vertexShader);
	_PrintLog(program);
}
tgGLRenderer::tgGLSL::~tgGLSL()
{
	pGLRenderer->tgglDetachObject(program,vertexShader);
	pGLRenderer->tgglDeleteObject(vertexShader);
	pGLRenderer->tgglDeleteObject(program);
}

void tgGLRenderer::tgGLSL::_PrintLog(GLhandleARB obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	pGLRenderer->tgglGetObjectParameteriv(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
					&infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		pGLRenderer->tgglGetInfoLog(obj, infologLength, &charsWritten, infoLog);
		tgout << "Shader info log : " << infoLog << endl;
		delete [] infoLog;
	}

}

void tgGLRenderer::tgGLSL::Enable ()
{
	pGLRenderer->tgglUseProgramObject(program);
}

void tgGLRenderer::tgGLSL::Disable ()
{
	pGLRenderer->tgglUseProgramObject(0);	
}

void tgGLRenderer::tgGLSL::SetUniform(const char* name, float p1, float p2, float p3,float p4)
{
	GLint tmp = pGLRenderer->tgglGetUniformLocation(program, name);
	//TODO REturn value check
	//TODO cache des GLint retournes par GetUniformLocation
	pGLRenderer->tgglUniform4f(tmp,p1,p2,p3,p4);
}

void tgGLRenderer::tgGLSL::SetUniform(const char* name, float p1, float p2, float p3)
{
	GLint tmp = pGLRenderer->tgglGetUniformLocation(program, name);
	//TODO REturn value check
	//TODO cache des GLint retournes par GetUniformLocation
	pGLRenderer->tgglUniform3f(tmp,p1,p2,p3);
}

void tgGLRenderer::tgGLSL::SetUniform(const char* name, float p1, float p2)
{
	GLint tmp = pGLRenderer->tgglGetUniformLocation(program, name);
	//TODO REturn value check
	//TODO cache des GLint retournes par GetUniformLocation
	pGLRenderer->tgglUniform2f(tmp,p1,p2);
}

void tgGLRenderer::tgGLSL::SetUniform(const char* name, float p1)
{
	GLint tmp = pGLRenderer->tgglGetUniformLocation(program, name);
	//TODO REturn value check
	//TODO cache des GLint retournes par GetUniformLocation
	pGLRenderer->tgglUniform1f(tmp,p1);
}

tgstring tgGLRenderer::tgGLSL::_ReadFromTextFile(int file)
{
	tgstring buffer;
	while(!pFileManager->EndOfFile(file)) {
		char c;
		pFileManager->Get(file,c);
		buffer += c;
	}
	tgout << buffer << endl;
	return buffer;
}

