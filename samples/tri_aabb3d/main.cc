/*
 * Demonstration of SAT for aaBB vs triangle 3D
 * Julien Rebetez
 * Based on OpenGL/SDL initialisation: 
 * This code was created by Jeff Molofee '99 
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 * 
 * or for port-specific comments, questions, bugreports etc. 
 * email to leggett@eecs.tulane.edu
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include "interface/tglib.h"
//RANDOM triangles
#include <stdlib.h>
#include <time.h>
#include <iostream>

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16


tgAABoundingBox bbox;
tgVector3 triangle[3];
tgVector3 worldTri[3];
tgVector3 triPos;
tgVector3 MStart;
tgVector3 MEnd;

float dist;
bool bDir[4];

using std::cout;
using std::endl;

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* This is our SDL surface */
SDL_Surface *surface;

bool RectAgainstTri2D(tgVector2* , tgVector2* , float& , float& , const tgVector2& , const tgVector2& );
bool BoxAgainstTri3D(tgVector3* , tgAABoundingBox* , float& , float& , const tgVector3& , const tgVector3& );

void UpdatePos()
{
	float first,last;
	
	
	bbox.SetPosition(MStart);
	for(short i=0;i<3; i++) {
		worldTri[i] = triPos + triangle[i];
	}
	
	if(BoxAgainstTri3D(worldTri, &bbox, first,last, MStart,MEnd)) {
		bbox.SetPosition(MStart + (MEnd-MStart)*first);
	} else {
		bbox.SetPosition(MEnd);
	}
}

void RandomizeTriangle()
{
	int seed = clock();
	srand(seed);
	
	/**MAX RAND*/
	#define MR 10
	triangle[0].Set(-rand()%MR,rand()%MR,-rand()%MR);
	triangle[1].Set(rand()%MR , 0,0 );
	triangle[2].Set(0,0, rand()%MR );
}

void DrawBox()
{
	glColor3f(0,0,1);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glVertex3f(MStart.x,MStart.y,MStart.z);
	glEnd();
	glPointSize(1.0f);
	glColor3f(1,0,0);
	glPushMatrix();
	//glTranslatef(boxPos.x,boxPos.y,boxPos.z);
	glBegin (GL_TRIANGLES);
	for (int i=0; i<6; i++)
	{
		glVertex3fv (&(bbox.GetPlane(i).v[0].x));
		glVertex3fv (&(bbox.GetPlane(i).v[1].x));
		glVertex3fv (&(bbox.GetPlane(i).v[2].x));
	}
	glEnd ();
	glPopMatrix();
}

void DrawTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex3fv(&worldTri[0].x);
	glVertex3fv(&worldTri[1].x);
	glVertex3fv(&worldTri[2].x);
	glEnd();
}

void Init()
{	
	bbox.SetMax(tgVector3(1,1,1));
	bbox.SetMin(tgVector3(-1,-1,-1));
	triangle[0].Set(4,0,0);
	triangle[1].Set(0,0,4);
	triangle[2].Set(-2,2,-2);
	//RandomizeTriangle();
	triPos.Set(0,2,-1);
	
	dist = -40;
	MStart.Set(0,10,0);
	MEnd.Set(0,0,0);
}

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
    exit( returnCode );
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;
 
    /* Protect against a divide by zero */
   if ( height == 0 )
	height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return( TRUE );
}

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    /* ESC key was pressed */
	    Quit( 0 );
	    break;
	case SDLK_F1:
	    /* F1 key was pressed
	     * this toggles fullscreen mode
	     */
	    SDL_WM_ToggleFullScreen( surface );
	    break;
	case SDLK_SPACE:
		RandomizeTriangle();
		break;
	case SDLK_UP:
		bDir[0] = true;
		break;
	case SDLK_DOWN:
		bDir[1] = true;
		break;
	case SDLK_RIGHT:
		bDir[2] = true;
		break;
	case SDLK_LEFT:
		bDir[3] = true;
		break;
	default:
	    break;
	}

    return;
}

void handleKeyUp(SDL_keysym* keysym)
{
 	switch ( keysym->sym )
	{
		case SDLK_UP:
			bDir[0] = false;
			break;
		case SDLK_DOWN:
			bDir[1] = false;
			break;
		case SDLK_RIGHT:
			bDir[2] = false;
			break;
		case SDLK_LEFT:
			bDir[3] = false;
			break;
		default:
			break;
	}
}

int initGL()
{
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    return( TRUE );
}

void DrawAxis ()
{
	//X axis
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(10,0,0);
	glEnd();
	
	//Y axis
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,10,0);
	glEnd();
	
	//Z axis
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,10);
	glEnd();
}

/* Here goes our drawing code */
int drawGLScene()
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;


    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	UpdatePos();

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    glTranslatef( 0,-5, dist );

    /* Rotate The Triangle On The Y axis ( NEW ) */
    glRotatef( rtri, 0.0f, 1.0f, 0.0f );
	DrawAxis();
	glColor3f(1,0,0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    DrawBox();
    	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    	glColor3f(0,1,0);
    DrawTriangle();
    
    /* Draw it to the screen */
    SDL_GL_SwapBuffers( );

    /* Gather our frames per second */
    /*Frames++;
    {
	GLint t = SDL_GetTicks();
	if (t - T0 >= 5000) {
	    GLfloat seconds = (t - T0) / 1000.0;
	    GLfloat fps = Frames / seconds;
	    printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
	    T0 = t;
	    Frames = 0;
	}
    }*/

    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    rtri  += 0.2f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    rquad -=0.15f;
    
    if(bDir[0]) dist += 0.5f;
    if(bDir[1]) dist -= 0.5f;
    if(bDir[2]) MStart.x += 0.5f;
    if(bDir[3]) MStart.x -= 0.5f;

    return( TRUE );
}

int main( int argc, char **argv )
{
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    /* main loop variable */
    int done = FALSE;
    /* used to collect events */
    SDL_Event event;
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;

    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}

    /* initialize OpenGL */
    initGL( );
	Init();
    /* resize the initial window */
    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

    /* wait for events */ 
    while ( !done )
	{
	    /* handle the events in the queue */

	    while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
			case SDL_VIDEORESIZE:
			    /* handle resize event */
			    surface = SDL_SetVideoMode( event.resize.w,
							event.resize.h,
							16, videoFlags );
			    if ( !surface )
				{
				    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				    Quit( 1 );
				}
			    resizeWindow( event.resize.w, event.resize.h );
			    break;
			case SDL_KEYDOWN:
			    /* handle key presses */
			    handleKeyPress( &event.key.keysym );
			    break;
			case SDL_KEYUP:
				handleKeyUp(&event.key.keysym);
				break;
			case SDL_QUIT:
			    /* handle quit requests */
			    done = TRUE;
			    break;
			default:
			    break;
			}
		}
		drawGLScene( );
	}

    /* clean ourselves up and exit */
    Quit( 0 );

    /* Should never get here */
    return( 0 );
}

/**
* Retourne via min/max la longueur de la projection d'une box sur un vecteur
*/
void ProjectBox(const tgVector3& vec, tgAABoundingBox* box, float& min, float& max) 
{
	float pos = vec*box->GetPosition();
	//TODO: a inclure dans box
	//comme on bosse avec des axis aligned, les axis sont fixe
	//mais avec des box non-aligned, il faudrait calculer les axis dans la box et remplacer ici
	float dist = box->GetExtents().x*fabs(vec*box->GetAxis(0)) + 
			box->GetExtents().y*fabs(vec*box->GetAxis(1)) +
			box->GetExtents().z*fabs(vec*box->GetAxis(2));
	min = pos - dist;
	max = pos + dist;
}

/**
* Retourne via min/max la longueur de la projection d'un triangle sur un vecteur
*/
void ProjectTriangle(const tgVector3& vec, tgVector3* tri, float& min, float& max) 
{
	min = vec*tri[0];
	max = min;
	
	float dot = vec*tri[1];
	if(dot < min)
		min = dot;
	else if (dot > max)
		max = dot;
	
	dot = vec*tri[2];
	if(dot < min)
		min = dot;
	else if (dot > max)
		max = dot;
}

bool NoIntersect(float TMax, float speed, float min0, float max0, float min1, float max1, float& TFirst, float& TLast)
{
	if(max1 < min0) { //la boite est a gauche du tri
		if(speed <= 0) return true;
		float T = (min0-max1)/speed; if(T>TFirst) TFirst = T;
		if(TFirst > TMax) return true;
		T = (max0-min1)/speed; if(T<TLast)TLast = T;
		if(TFirst>TLast) return true;
	} else if (max0<min1) { //la boite est a droite du tri
		if(speed >=0) return true;
		float T = (max0-min1)/speed; if(T>TFirst)TFirst = T;
		if(TFirst>TMax) return true;
		T = (min0-max1)/speed; if(T<TLast)TLast = T;
		if (TFirst>TLast) return true;
	} else { //les deux intervalles sont superposes
		if(speed >= 0) {
			float T=(max0-min1)/speed; if(T<TLast) TLast = T;
			if(TFirst>TLast) return true;
		} else if (speed < 0) {
			float T = (min0-max1)/speed; if(T<TLast) TLast = T;
			if(TFirst>TLast) return true;
		}
	}
	return false;
}

#define FILL_BOX_MIN_MAX3D(min,max) float min = axis[i]*verts[0]; \
		float max = axis[i]*verts[0]; \
		for(short j=1;j<8;j++) { \
			float tmp = axis[i]*verts[j]; \
			if (tmp<min) \
				min = tmp; \
			if (tmp>max) \
				max = tmp; \
		}
		
#define FILL_TRI_MIN_MAX3D(min,max) float min = axis[i]*tri[0]; \
		float max = axis[i]*tri[0]; \
		for(short j=1;j<3;j++) {\
			float tmp = axis[i]*tri[j]; \
			if(tmp<min) \
				min = tmp; \
			if(tmp>max) \
				max = tmp; \
		}

bool BoxAgainstTri3D(tgVector3* tri, tgAABoundingBox* box, float& TFirst, float& TLast, const tgVector3& Start, const tgVector3& End)
{
	tgVector3 W = End-Start;//on veut la vitesse en coord objects, comme Start == MStart == rectPos, on simplifie W = (end-rectPos)-(start-rectPos)
	
	TFirst = 0;
	TLast = 10000000;
	float TMax = 1;
	
	float min0,max0,min1,max1,speed;
	tgVector3 direction;
	
	//les edges du triangle
	tgVector3 triEdges[3];
	triEdges[0] = tri[0]-tri[1];
	triEdges[1] = tri[1]-tri[2];
	triEdges[2] = tri[2]-tri[0];
	//on test la normale du triangle
	tgPlane triPlane(tri[0],tri[1],tri[2]);
	direction = triPlane.GetNormal();
	min0 = direction*tri[0];
	max0 = min0;
	ProjectBox(direction, box, min1, max1);
	speed = direction*W;
	if(NoIntersect(TMax, speed, min0,max0,min1,max1,TFirst,TLast))
		return false;

	//on test les directions des faces de la box
	for(short i=0; i<3; i++)
	{
		direction = box->GetAxis(i);
		ProjectTriangle(direction,tri,min0,max0);
		float pos = direction*box->GetPosition();
		min1 = pos - box->GetExtents()[i];
		max1 = pos + box->GetExtents()[i];
		speed = direction*W;
		if(NoIntersect(TMax,speed,min0,max0,min1,max1,TFirst,TLast))
			return false;
	}
	
	//on test la direction des produits vectoriels triangle-box
	for(short i=0; i<3; i++) {
		for(short j=0; j<3; j++) {
			direction = triEdges[j]%box->GetAxis(i);
			ProjectTriangle(direction, tri, min0, max0);
			ProjectBox(direction, box, min1, max1);
			speed = direction*W;
			if(NoIntersect(TMax,speed,min0,max0,min1,max1,TFirst,TLast))
				return false;
		}
	}
	return true;
}

