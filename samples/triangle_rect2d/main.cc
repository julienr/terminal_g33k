/**
* Exemple d'implementation du Separating Axis Theorem pour un triangle vs rect
* le Triangle est statique (en fait deplacable avec clique droit souris, mais il est statique pour l'algo)
* le rectangle est 'trace' entre MStart et MEnd
* MStart est la position du curseur de la souris, MEnd est le petit point bleu qui se deplace avec les touches flechees
* en pressant sur ESPACE, on cree un nouveau triangle aleatoire pour les collisions
*/

#include "tgvector2.h"
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <iostream>
//RANDOM triangles
#include <stdlib.h>
#include <time.h>


using std::cout;
using std::endl;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Surface* pScreen = NULL;
bool bExit = false;
int mouseX = 0;
int mouseY = 0;

tgVector2 MStart;
tgVector2 MEnd;

tgVector2 rectPos;
tgVector2 triPos;
tgVector2 rect[4]; //local coord
tgVector2 tri[3]; //local coord
tgVector2 worldTri[3]; //world coord
tgVector2 worldRect[4]; //world coord

#define COLOR_WHITE 255,255,255,255
#define COLOR_BLUE 0,0,255,255
#define COLOR_GREEN 0,255,0,255
#define COLOR_RED 255,0,0,255

#define SIZE 40
bool RectAgainstTri2D(tgVector2* tri, tgVector2* rect, float& tFirst, float& tLast, const tgVector2& Start, const tgVector2& End);
void InitObjects()
{
	rectPos.Set(60,60);
	triPos.Set(50,50);
	rect[0].Set(0,0);
	rect[1].Set(SIZE,0);
	rect[2].Set(SIZE,SIZE);
	rect[3].Set(0,SIZE);
	
	tri[0].Set(0,0);
	tri[1].Set(SIZE,3*SIZE);
	tri[2].Set(SIZE*2,0.5*SIZE);
}

void DrawTri()
{
	filledTrigonRGBA(pScreen,(int)worldTri[0].x,(int)worldTri[0].y,(int)worldTri[1].x,(int)worldTri[1].y,(int)worldTri[2].x,(int)worldTri[2].y,COLOR_RED);
}

void DrawRect()
{
	tgVector2 vMin,vMax;
	vMin = worldRect[0];
	vMax = worldRect[1];
	for(short i=1;i<4;i++) {
		if(worldRect[i].x < vMin.x)
			vMin.x = worldRect[i].x;
		if(worldRect[i].x > vMax.x)
			vMax.x = worldRect[i].x;
			
		if(worldRect[i].y < vMin.y)
			vMin.y = worldRect[i].y;
		if(worldRect[i].y > vMax.y)
			vMax.y = worldRect[i].y;
	}
	boxRGBA(pScreen,  (int)vMin.x,(int)vMin.y,(int)vMax.x,(int)vMax.y, COLOR_WHITE);
}

void UpdatePos()
{
	if(MEnd.x < 0)
		MEnd.x = 0;
	if(MEnd.y < 0)
		MEnd.y = 0;
	//MEnd.Set(0,0);
	float first,last;
	
	rectPos = MStart;
	for(short i=0; i<4; i++) {
		worldRect[i] = rect[i]+rectPos;
	}
	for(short i=0; i<3; i++) {
		worldTri[i] = tri[i]+triPos;
	}
	
	if(RectAgainstTri2D(worldTri, worldRect, first,last, MStart,MEnd)) {
		rectPos = MStart + (MEnd-MStart)*first;

	} else {
		rectPos = MEnd;
	}
		
	for(short i=0; i<4; i++) {
		worldRect[i] = rect[i]+rectPos;
	}
	
	
}

void Draw()
{	
	lineRGBA(pScreen,0,0,0,SCREEN_HEIGHT-1,COLOR_WHITE);
	lineRGBA(pScreen,0,0,SCREEN_WIDTH-1,0,COLOR_WHITE);
	lineRGBA(pScreen,0,SCREEN_HEIGHT-1,SCREEN_WIDTH-1,SCREEN_HEIGHT-1,COLOR_WHITE);
	lineRGBA(pScreen,SCREEN_WIDTH-1,0,SCREEN_WIDTH-1,SCREEN_HEIGHT-1,COLOR_WHITE);
	
	DrawRect();
	DrawTri();
	filledCircleRGBA(pScreen, (int)MEnd.x, (int)MEnd.y,5,COLOR_BLUE);
	SDL_Flip(pScreen);
}

void RandomizeTriangle()
{
	int seed = clock();
	srand(seed);
	
	/**MAX RAND*/
	#define MR 200
	tri[0].Set(0,0);
	tri[1].Set(rand()%MR , rand()%MR );
	tri[2].Set(rand()%MR , rand()%MR );
}

int main()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Init Video " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(pScreen == NULL) {
		cout << "pScreen " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	InitObjects();
	SDL_Event event;
	bool bDir[4] = {false,false,false,false};
	
	bool bRClick = false;
	while(!bExit) {
		SDL_FillRect(pScreen,NULL,0);
		if(bDir[0])
			MEnd.x -= 1;
		if(bDir[1])
			MEnd.x += 1;
		if(bDir[2])
			MEnd.y -= 1;
		if(bDir[3])
			MEnd.y += 1;
	
		if(bRClick) triPos.Set(mouseX,mouseY);
		else MStart.Set(mouseX,mouseY);
		
		UpdatePos();
		Draw();
		while(SDL_PollEvent(&event)) {
		switch( event.type ) {
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_RIGHT)
						bRClick = true;
					break;
					
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_RIGHT)
						bRClick = false;
					break;
					
				case SDL_QUIT:
					bExit = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							bExit = true;
							break;
						case SDLK_LEFT:
							bDir[0] = true;
							break;
						case SDLK_RIGHT:
							bDir[1] = true;
							break;
						case SDLK_UP:
							bDir[2] = true;
							break;
						case SDLK_DOWN:
							bDir[3] = true;
							break;
						case SDLK_SPACE:
							RandomizeTriangle();
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT:
							bDir[0] = false;
							break;
						case SDLK_RIGHT:
							bDir[1] = false;
							break;
						case SDLK_UP:
							bDir[2] = false;
							break;
						case SDLK_DOWN:
							bDir[3] = false;
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					mouseX = event.motion.x;
					mouseY = event.motion.y;
					break;
			}
		}
	}
}


#define FILL_BOX_MIN_MAX(min,max) float min = axis[i][0]*rect[0]; \
		float max = axis[i][0]*rect[0]; \
		for(short j=1;j<4;j++) { \
			float tmp = axis[i][0]*rect[j]; \
			if (tmp<min) \
				min = tmp; \
			if (tmp>max) \
				max = tmp; \
		}
		
#define FILL_TRI_MIN_MAX(min,max) float min = axis[i][0]*tri[0]; \
		float max = axis[i][0]*tri[0]; \
		for(short j=1;j<3;j++) {\
			float tmp = axis[i][0]*tri[j]; \
			if(tmp<min) \
				min = tmp; \
			if(tmp>max) \
				max = tmp; \
		}
		
#define DONT_OVERLAP(min0,max0,min1,max1) ((min0>max1 && max0>max1) || (min0<min1 && max0<min1))

bool RectAgainstTri2D(tgVector2* tri, tgVector2* rect, float& TFirst, float& TLast, const tgVector2& Start, const tgVector2& End)
{
	tgVector2 W = End-Start;//on veut la vitesse en coord objects, comme Start == MStart == rectPos, on simplifie W = (end-rectPos)-(start-rectPos)
	
	TFirst = 0;
	TLast = 100000;
	float TMax = 1;
	//AXIS
	/*on prend des tableaux en 2D, dans la deuxieme colonne, on utilise la [i][1] 
	pour stocker la position du centre de l'edge, histoire de pouvoir dessiner les normals*/
	tgVector2 triEdge[3][2];
	triEdge[0][0] = tri[0]-tri[1];
	triEdge[0][1] = (tri[1]+tri[0])/2;
	
	triEdge[1][0] = tri[1]-tri[2];
	triEdge[1][1] = (tri[2]+tri[1])/2;
	
	triEdge[2][0] = tri[2]-tri[0];
	triEdge[2][1] = (tri[0]+tri[2])/2;
	
	tgVector2 rectEdge[4][2];
	rectEdge[0][0] = rect[1]-rect[0];
	rectEdge[0][1] = (rect[1]+rect[0])/2;
	
	rectEdge[1][0] = rect[2]-rect[1];
	rectEdge[1][1] = (rect[2]+rect[1])/2;
	
	rectEdge[2][0] = rect[3]-rect[2];
	rectEdge[2][1] = (rect[3]+rect[2])/2;
	
	rectEdge[3][0] = rect[0]-rect[3];
	rectEdge[3][1] = (rect[0]+rect[3])/2;
	
	tgVector2 axis[7][2];
	
	for(short i=0; i<3;i++) {
		axis[i][0].Set(triEdge[i][0].y, -triEdge[i][0].x);
		axis[i][0].Normalize();
		axis[i][1] = triEdge[i][1];
	}
	for(short i=0;i<4;i++) {
		axis[i+3][0].Set(rectEdge[i][0].y,-rectEdge[i][0].x);
		axis[i+3][0].Normalize();
		axis[i+3][1] = rectEdge[i][1];
	}
	///AXIS
	
	for(short i=0; i<7; i++) {
		lineRGBA(pScreen, axis[i][1].x,axis[i][1].y,axis[i][1].x+axis[i][0].x*5,axis[i][1].y+axis[i][0].y*5,COLOR_GREEN);
		float speed = axis[i][0]*W;
		
		FILL_TRI_MIN_MAX(min0,max0)
		FILL_BOX_MIN_MAX(min1,max1)
		
		if(max1 < min0) { //la boite est a gauche du tri
			if(speed <= 0) return false;
			float T = (min0-max1)/speed; if(T>TFirst) TFirst = T;
			if(TFirst > TMax) return false;
			T = (max0-min1)/speed; if(T<TLast)TLast = T;
			if(TFirst>TLast) return false;
		} else if (max0<min1) { //la boite est a droite du tri
			if(speed >=0) return false;
			float T = (max0-min1)/speed; if(T>TFirst)TFirst = T;
			if(TFirst>TMax) return false;
			T = (min0-max1)/speed; if(T<TLast)TLast = T;
			if (TFirst>TLast) return false;
		} else {
			if(speed >= 0) {
				float T=(max0-min1)/speed; if(T<TLast) TLast = T;
				if(TFirst>TLast) return false;
			} else if (speed < 0) {
				float T = (min0-max1)/speed; if(T<TLast) TLast = T;
				if(TFirst>TLast) return false;
			}
		}
	}
	
	return true;
	
}
