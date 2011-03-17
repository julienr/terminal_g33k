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
#include "tgq3level.h"

tgQ3Level::tgQ3Level (const char* levelname)
{
  _LoadBSP(levelname);
  _FillLeavesPatchInfo();
}

tgQ3Level::~tgQ3Level()
{
  delete [] pEntities;
  delete [] pVerts;
  delete [] pFaces;
  delete [] pVisibleFaces;
  delete [] pDirectory;
  delete [] pTexturesInfos;
  delete [] pMaterials;
  delete [] pContents;
  delete [] pLeafs;
  delete [] pNodes;
  delete [] pLeafFaces;
  delete [] pPlanes;
  delete [] pLightMapsInfos;
  delete [] pLightMaps;
  delete [] pLeafBrushes;
  delete [] pBrushes;
  delete [] pBrushSides;
  delete [] pMeshIndices;
  delete [] pShaders;
  delete [] pPatches;
  delete [] pPolygonFaces;
  delete [] pMeshFaces;
  delete [] clusters.pBitsets;
}

void tgQ3Level::_LoadTextures ()
{
  for (int i =0;i<numOfTextures;i++) {
    pMaterials[i] = pMaterialManager->GetMaterial(pTexturesInfos[i].strName);
  }
    
}

void ChangeGamma(unsigned char *pImage, int size, float factor)
{
    // Go through every pixel in the lightmap
    for(int i = 0; i < size / 3; i++, pImage += 3)
    {
        float scale = 1.0f, temp = 0.0f;
        float r = 0, g = 0, b = 0;

        // extract the current RGB values
        r = (float)pImage[0];
        g = (float)pImage[1];
        b = (float)pImage[2];

        // Multiply the factor by the RGB values, while keeping it to a 255 ratio
        r = r * factor / 255.0f;
        g = g * factor / 255.0f;
        b = b * factor / 255.0f;

        // Check if the the values went past the highest value
        if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;
        if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;
        if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;

        // Get the scale for this pixel and multiply it by our pixel values
        scale*=255.0f;
        r*=scale;   g*=scale;   b*=scale;

        // Assign the new gamma'nized RGB values to our image
        pImage[0] = (unsigned char)r;
        pImage[1] = (unsigned char)g;
        pImage[2] = (unsigned char)b;
    }
}

void tgQ3Level::_LoadLightmaps ()
{
  tgglGenTextures(numOfLightmaps, (GLuint*)pLightMaps);
  for (int i=0; i<numOfLightmaps; i++)
  {
    tgglPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    tgglBindTexture(GL_TEXTURE_2D, pLightMaps[i]);
    ChangeGamma((unsigned char*)pLightMapsInfos[i].imageBits, 128*128*3, 2);
    tggluBuild2DMipmaps(GL_TEXTURE_2D, 3, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, pLightMapsInfos[i].imageBits);

    //Mipmap
    tgglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    tgglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    tgglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
}


void tgQ3Level::FindSphereContainedFaces (const tgBoundingSphere& sphere)
{
  tgvector<int> leavesList;
  for(int i=0; i<numOfFaces; i++)
    pVisibleFaces[i] = false;

  //recherche des leaves qui sont touches par la sphere
  _RecursiveSphereLeaves(0, &sphere, &leavesList);
  int size = (int)leavesList.size();
  for(int i=0; i<size; i++) {
    Q3Defs::MyLeaf* leaf = &pLeafs[leavesList[i]];
    for(int j=0; j<leaf->numOfLeafFaces; j++) {
      int faceIndex = pLeafFaces[leaf->leafface+j];
      pVisibleFaces[faceIndex] = true;
    }
  }
}

void tgQ3Level::FindVisibleFaces (const tgRenderView& rView)
{
  int camLeaf = _FindLeaf(rView.GetFinalPosition());
  int camCluster = pLeafs[camLeaf].cluster;
  //remise à zéro du tableau
  for(int i=0; i<numOfFaces; i++)
    pVisibleFaces[i] = false;
  //calcul des faces visibles
  for(int i=0; i<numOfLeafs; i++) {
    Q3Defs::MyLeaf* leaf = &pLeafs[i];
    if(!_IsClusterVisible(camCluster, leaf->cluster))
      continue;
      
    //TODO: Frustum culling sur les BBox des leafs
    for(int j=0; j<leaf->numOfLeafFaces; j++)
      pVisibleFaces[pLeafFaces[leaf->leafface+j]] = true;
  }
}

int tgQ3Level::_FindLeaf(const tgVector3& pos)
{
  int index = 0;
  while(index >= 0) {
    const Q3Defs::BspNode& node = pNodes[index];
    const Q3Defs::BspPlane& plane = pPlanes[node.plane];
    if (plane.GetDistance(pos) > 0)
      index = node.front;
    else
      index = node.back;
  }
  //-index-1
  return ~index;
}

bool tgQ3Level::_IsClusterVisible (int from, int test)
{
  //si on a pas de donnees de visibilite 
  if (!clusters.pBitsets || (from < 0))
    return true;
  
  int i = (from*clusters.bytesPerCluster) + (test/8);
  uint8 visSet = clusters.pBitsets[i];
  
  return (visSet & ( 1 << (test & 7))) != 0;
}

void tgQ3Level::_FillLeavesPatchInfo ()
{
  for(int i=0; i<numOfLeafs; i++) {
    Q3Defs::MyLeaf* leaf = &pLeafs[i];
    //bool bIndexFixed = false;
    leaf->numOfLeafPatches = 0;
    for(int j=0; j<leaf->numOfLeafFaces; j++) {
      int faceIndex = pLeafFaces[leaf->leafface+j];
      if(pFaces[faceIndex].type == Q3Defs::PatchFace) { //on a une patch
        /*if (!bIndexFixed) {
          leaf->leafPatch = pDirectory[faceIndex].typeIndex;
          bIndexFixed = true;
        }
        leaf->numOfLeafPatches++;*/
        leaf->patchIndex.push_back(pDirectory[faceIndex].typeIndex);
      }
    }
    //out << leaf->leafPatch << "\t num :" << leaf->numOfLeafPatches << endl;
  }
}

void tgQ3Level::_LoadPatch(Q3Defs::BspPatch* p, int tesselation)
{
  //le nombre de points de contrôles
  int controlWidth = p->size[0];
  int controlHeight = p->size[1];
  //le nombre de bezier patch que produira notre patch
  int biquadWidth = (controlWidth-1)/2;
  int biquadHeight = (controlHeight-1)/2;
  //le nombre de vertices
  int vertexWidth = biquadWidth*tesselation+1;
  int vertexHeight = biquadHeight*tesselation+1;
  
  p->pVertices = new Q3Defs::BspVertex[vertexWidth*vertexHeight];
  
  //pour stocker temporairement les vertices de la surface de bezier qu'on traite
  Q3Defs::BspVertex* biquadVertices = new Q3Defs::BspVertex[(tesselation+1)*(tesselation+1)];
  //pour stocke temporairement les points de controle de la surface de bezier en cours de traitement
  Q3Defs::BspVertex biquadControlPoints[9];
  
  for(int j=0; j<biquadHeight; ++j)
  {
    for(int k=0; k<biquadWidth; ++k)
    {
      biquadControlPoints[0]=p->controls[j*controlWidth*2 + k*2];
      biquadControlPoints[1]=p->controls[j*controlWidth*2 + k*2 +1];
      biquadControlPoints[2]=p->controls[j*controlWidth*2 + k*2 +2];

      biquadControlPoints[3]=p->controls[j*controlWidth*2 + controlWidth + k*2];
      biquadControlPoints[4]=p->controls[j*controlWidth*2 + controlWidth + k*2 +1];
      biquadControlPoints[5]=p->controls[j*controlWidth*2 + controlWidth + k*2 +2];

      biquadControlPoints[6]=p->controls[j*controlWidth*2 + controlWidth*2 + k*2];
      biquadControlPoints[7]=p->controls[j*controlWidth*2 + controlWidth*2 + k*2 +1];
      biquadControlPoints[8]=p->controls[j*controlWidth*2 + controlWidth*2 + k*2 +2];

      //On applique la tesselation a cette surface de bezier
      _TesselateBiquadraticPatch(biquadControlPoints, biquadVertices, tesselation);

      //On rajoute les vertices calcules (biquadVertices) dans le tableau de vertice de la patch
      for(int l=0; l<tesselation+1; ++l)
      {
        for(int m=0; m<tesselation+1; ++m)
        {
          int index= (j*vertexWidth+k)*tesselation + l*vertexWidth + m;
          p->pVertices[index]=biquadVertices[l*(tesselation+1)+m];
        }
      }
      
    }
  }
  
  //allocation memoire pour les indices
  p->iNumIndices = (vertexWidth-1)*(vertexHeight-1)*6;
  p->pIndices = new unsigned int[p->iNumIndices];
  
  //on calcul les indices
  for(int j=0; j<vertexHeight-1; ++j) {
    for(int k=0; k<vertexWidth-1; ++k) {
      p->pIndices[(j*(vertexWidth-1)+k)*6+0]=j*vertexWidth+k;
      p->pIndices[(j*(vertexWidth-1)+k)*6+2]=j*vertexWidth+k+1;
      p->pIndices[(j*(vertexWidth-1)+k)*6+1]=(j+1)*vertexWidth+k;

      p->pIndices[(j*(vertexWidth-1)+k)*6+3]=(j+1)*vertexWidth+k;
      p->pIndices[(j*(vertexWidth-1)+k)*6+5]=j*vertexWidth+k+1;
      p->pIndices[(j*(vertexWidth-1)+k)*6+4]=(j+1)*vertexWidth+k+1;
    }
  }
  
  //Calcule de la bounding box de la patch
  if(vertexWidth*vertexHeight <= 0){
    tgout << "Erreur, vertexWidth*vertexHeight <=0 !" << endl;
  }
  
  
  tgVector3 boxMin = p->pVertices[0].vPosition;
  tgVector3 boxMax = p->pVertices[0].vPosition;
  
  for(int i=0; i<vertexWidth*vertexHeight; i++) {
    const tgVector3& v = p->pVertices[i].vPosition;
    if (v.x < boxMin.x)
      boxMin.x = v.x;
    if (v.y < boxMin.y)
      boxMin.y = v.y;
    if (v.z < boxMin.z)
      boxMin.z = v.z;
      
    if (v.x > boxMax.x)
      boxMax.x = v.x;
    if (v.y > boxMax.y)
      boxMax.y = v.y;
    if (v.z > boxMax.z)
      boxMax.z = v.z;
  }
  p->pBBox = new tgAABoundingBox(boxMin, boxMax);
  p->sphere.SetPosition((boxMin+boxMax)/2);
  p->sphere.SetRadius((boxMax-boxMin).Length()/2);

  p->pFaces = new Q3Defs::BspPatchFace[p->iNumIndices/3];
  unsigned int triCount = 0;
  for (unsigned int i=0; i<p->iNumIndices; i+=3) { 
    const tgVector3& point1 = p->pVertices[p->pIndices[i]].vPosition;
    const tgVector3& point2 = p->pVertices[p->pIndices[i+1]].vPosition;
    const tgVector3& point3 = p->pVertices[p->pIndices[i+2]].vPosition;
    p->pFaces[triCount].plane.SetFromPoints(point1,point3,point2);
    p->pFaces[triCount].points[0] = point1;
    p->pFaces[triCount].points[2] = point2;
    p->pFaces[triCount].points[1] = point3;
    triCount++;
  }


  delete [] biquadVertices;
}

void tgQ3Level::_TesselateBiquadraticPatch (Q3Defs::BspVertex* controlPoints, Q3Defs::BspVertex* vertices, int tess)
{
  //le nombre de points sur une ligne
  int vertexWidth = tess + 1;
  
  Q3Defs::BspVertex* column0 = new Q3Defs::BspVertex[vertexWidth];
  Q3Defs::BspVertex* column1 = new Q3Defs::BspVertex[vertexWidth];
  Q3Defs::BspVertex* column2 = new Q3Defs::BspVertex[vertexWidth];
  //tesselation le long des colonnes
  for (int i=0; i<vertexWidth; i++)
  {
    float factor = float(i)/(vertexWidth-1);
    column0[i] = _QuadraticInterpolate(controlPoints[0], controlPoints[3], controlPoints[6], factor);
    column1[i] = _QuadraticInterpolate(controlPoints[1], controlPoints[4], controlPoints[7], factor);
    column2[i] = _QuadraticInterpolate(controlPoints[2], controlPoints[5], controlPoints[8], factor);
  }
  
  //tesselation le long des lignes
  for (int i=0; i<vertexWidth; i++)
  {
    for (int j=0; j<vertexWidth; j++)
    {
      vertices[i*vertexWidth+j] = _QuadraticInterpolate(column0[i], column1[i], column2[i], float(j)/(vertexWidth-1));
    }
  }
  delete [] column0;
  delete [] column1;
  delete [] column2;
  
  
}

Q3Defs::BspVertex tgQ3Level::_QuadraticInterpolate (const Q3Defs::BspVertex& v1, const Q3Defs::BspVertex& v2, const Q3Defs::BspVertex& v3, float f)
{
  Q3Defs::BspVertex result;
  result = v1*(1.0f-f)*(1.0f-f) + v2*2*f*(1.0f-f) + v3*f*f;
  return result;
}


const tgCollisionResult& tgQ3Level::Trace(const tgVector3& Start, const tgVector3& End, tgBoundingVolume* volume, tgIEntity* entity, tgset<tgIEntity*>* ignoreList)
{
  pTraceEntity = entity;
  pIgnoreList = ignoreList;
  if (volume == NULL) {
    pTraceVolume = NULL;
    traceType = TRACE_RAY;
  } else if(volume->GetType() == TG_AABBOX) {
    pTraceVolume = new tgAABoundingBox(static_cast<tgAABoundingBox*>(volume));
    traceType = TRACE_BOX;
    //on change la position du bounding volume sur start (utilise par le SAT vs Nurbs)
    //FIXME: on ne devrait PAS TOUCHER a pTraceVolume
    pTraceVolume->SetPosition(Start);
  } else if (volume->GetType() == TG_BSPHERE) {
    pTraceVolume = new tgBoundingSphere(static_cast<tgBoundingSphere*>(volume));
    traceType = TRACE_SPHERE;
    //on change la position du bounding volume sur start (utilise par le SAT vs Nurbs)
    pTraceVolume->SetPosition(Start);
  } else {
    tgerr << "volume->GetType () inconnu : " << volume->GetType() << endl;
    traceType = TRACE_RAY;
    pTraceVolume = NULL;
  }
  const tgCollisionResult& cr = _Trace(Start, End);
  delete pTraceVolume;
  return cr;
}

const tgCollisionResult& tgQ3Level::_Trace(const tgVector3& Start, const tgVector3& End)
{
  traceResult.bStartsOut = true;
  traceResult.bAllSolid = false;
  traceResult.fFraction = 1.0f;
  traceResult.vColNormal.Set(0,0,0);
  traceResult.pEntity = NULL;
  traceResult.bBezier = false;
  
  MStart = Start;
  MEnd = End;
  
  _TraceAgainstNode( 0 , 0.0f, 1.0f, Start, End);

  if (traceResult.fFraction == 1.0f) { // le trace n'a rien rencontré
    traceResult.vEndPoint = End;
  }
  else { //on a eu une collision avec quelque chose
    traceResult.vEndPoint = Start + (End-Start)*traceResult.fFraction;
    traceResult.vColNormal.Normalize();
    //FIXME: ugly hack pour eviter qu'on s'arrete dans les beziers
    //=> ugly hack etendu a toutes les collisions pour eviter qu'one ne reste coince dans
    //les murs, le probleme est que ca cree des 'tremblements'
    //if(traceResult.bBezier)
      traceResult.vEndPoint += traceResult.vColNormal;
  }
  return traceResult;
}

void tgQ3Level::_TraceAgainstNode (int nodeIndex, float startFraction, float endFraction, const tgVector3& Start, const tgVector3& End)
{
  if (traceResult.fFraction <= startFraction) 
    return;
  
  if (nodeIndex < 0) { // c'est un LEAF
    Q3Defs::MyLeaf* leaf = &pLeafs[~nodeIndex]; 
    // le leafIndex est égal à -(nodeIndex+1)=~nodeIndex
    
    //BRUSHES
    for (int i = 0; i < leaf->numOfLeafBrushes; i++) {
      Q3Defs::BspBrush *brush = &pBrushes[pLeafBrushes[leaf->leafBrush + i]];
      //si le brush a des côtés et qu'il est solid, alors on le test
      if ( brush->numOfBrushSides > 0 && pContents[brush->textureID] & 1)
      {
        _TraceAgainstBrush(brush);
      }
    }
    
    //PATCHES
    unsigned int numPatches = (unsigned int)leaf->patchIndex.size();
    for(size_t i=0; i<numPatches; i++) {
      Q3Defs::BspPatch* patch = &pPatches[leaf->patchIndex[i]];
      if(!(pContents[patch->textureID] & 1)) //la patch n'est pas solide
        continue;
      if (!patch->pBBox->VolumeInside(pTraceVolume, MStart,MEnd))
        continue;
      _TraceAgainstPatch(patch);
    }
    
    //ENTITES
    for(tgset<tgIEntity*>::iterator i=pEntityList.begin(); i!=pEntityList.end(); i++) {
      tgIEntity* entity = *i;
      if(entity == pTraceEntity)
        continue;
      if(pIgnoreList && pIgnoreList->find(entity) != pIgnoreList->end())
        continue;

      if(entity->GetBoundingVolume() != NULL) {
        if(entity->GetBoundingVolume()->Trace(traceResult, MStart, MEnd, pTraceVolume)) {
          traceResult.pEntity = entity;
        }
      }
    }
    
    return;
  }
  
  
  //c'est un NODE
  Q3Defs::BspNode  *node  = &pNodes[nodeIndex];
  Q3Defs::BspPlane *plane = &pPlanes[node->plane];
  
  float startDistance = plane->GetDistance(Start);
  float endDistance = plane->GetDistance(End);
  float offset = 0;
  
  if (traceType == TRACE_RAY) {
    offset = 0;
  } else if (traceType == TRACE_BOX) {
    tgAABoundingBox* box = static_cast<tgAABoundingBox*>(pTraceVolume);
    offset = (float)(fabs(box->GetExtents().x*plane->vNormal.x)+
                     fabs(box->GetExtents().y*plane->vNormal.y)+
         fabs(box->GetExtents().z*plane->vNormal.z));
  } else if (traceType == TRACE_SPHERE) {
    tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(pTraceVolume);
    offset = sphere->GetRadius();
  } else { 
    tgerr << "traceType Inconnu : " << traceType << endl;
  }
  
  //les deux points sont devant le plan -> on passe au front child
  if (startDistance >= offset && endDistance >= offset) {
    _TraceAgainstNode( node->front, startFraction, endFraction, Start, End);
    return;
  }
  
  //les deux points sont derrière le plan -> on passe au back child
  else if (startDistance < -offset && endDistance < -offset) {
    _TraceAgainstNode( node->back, startFraction, endFraction, Start, End);
    return;
  }
  
  else { //la ligne coupe le plan
    int side1, side2;
    float fraction1, fraction2;
    tgVector3 middle;
    
    //si on sait que l'un des deux est negative et l'autre positive, et que startDistance < endDistance, alors startDistance est negative et endDistance positive
    if (startDistance < endDistance) {
      side1 = node->back;    //le côté "back" contient le point de départ, il sera donc testé en premier
      side2 = node->front;
      float inverseDistance = 1.0f / (startDistance - endDistance);
      fraction1 = (startDistance + EPSILON - offset) * inverseDistance;
      fraction2 = (startDistance + EPSILON + offset) * inverseDistance;
    }
    //l'autre cas
    else if (endDistance < startDistance) {
      side1 = node->front;  //le côté "front" contient le point de départ, il sera donc testé en premier
      side2 = node->back;
      float inverseDistance = 1.0f / (startDistance - endDistance);
      fraction1 = (startDistance + EPSILON + offset) * inverseDistance;
      fraction2 = (startDistance - EPSILON - offset) * inverseDistance;
    }
    else { //endDistance == startDistance 
      side1 = node->front;
      side2 = node->back;
      fraction1 = 1.0f;
      fraction2 = 0.0f;
    }

    if (fraction1 < 0.0f) fraction1 = 0.0f;
    else if (fraction1 > 1.0f) fraction1 = 1.0f;
    if (fraction2 < 0.0f) fraction2 = 0.0f;
    else if (fraction2 > 1.0f) fraction2 = 1.0f;

    middle = Start + (End - Start)*fraction1;
    float middleFraction = startFraction + (endFraction - startFraction) * fraction1;

    // test du premier côté
    _TraceAgainstNode( side1, startFraction, middleFraction, Start, middle);


    middle = Start + (End - Start)*fraction2;
    middleFraction = startFraction + (endFraction - startFraction) * fraction2;

    // test du deuxième côté
    _TraceAgainstNode( side2, middleFraction, endFraction, middle, End);
  }
}

bool SameSide (const tgVector3& p1, const tgVector3& p2, const tgVector3& a, const tgVector3& b)
{
  /*tgVector3 cp1 ((b-a)%(p1-a));
  tgVector3 cp2 ((b-a)%(p2-a));
  return (cp1*cp2 >= 0);*/
  return ((((b-a)%(p1-a)) * ((b-a)%(p2-a))) >= 0);
}


bool PointInTriangle (const tgVector3& p,const tgVector3& a,const tgVector3& b,const tgVector3& c)
{
  //return (SameSide(p, a, b, c) && SameSide (p, b, a, c) && SameSide(p, c, a, b));
  return ((((c-b)%(p-b)) * ((c-b)%(a-b))) >= 0 && //SameSide (p,a,b,c)
       (((c-a)%(p-a)) * ((c-a)%(b-a))) >= 0 && //SameSide (p,b,a,c)
       (((b-a)%(p-a)) * ((b-a)%(c-a))) >= 0); //SameSide (p,c,a,b)
}

/**
* Retourne via min/max la longueur de la projection d'une box sur un vecteur
*/
void ProjectBox(const tgVector3& vec, tgAABoundingBox* box, float& min, float& max) 
{
  float pos = vec*box->GetPosition();
  float dist = box->GetExtents().x*fabs(vec*box->GetAxis(0)) + 
      box->GetExtents().y*fabs(vec*box->GetAxis(1)) +
      box->GetExtents().z*fabs(vec*box->GetAxis(2));
  min = pos - dist;
  max = pos + dist;
}

/**
* Retourne via min/max la longueur de la projection d'un triangle sur un vecteur
*/
void ProjectTriangle(const tgVector3& vec, const tgVector3* tri, float& min, float& max) 
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


/**
* tri = min0,max0
* box = min1,max1
*/
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
      TFirst -= EPSILON;
    } else if (speed < 0) {
      float T = (min0-max1)/speed; if(T<TLast) TLast = T;
      if(TFirst>TLast) return true;
      TFirst += EPSILON;
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

bool BoxAgainstTri3D(const tgVector3* tri, tgAABoundingBox* box, float& TFirst, float& TLast, const tgVector3& Start, const tgVector3& End)
{
  tgVector3 W = End-Start;//on veut la vitesse en coord objects, comme Start == MStart == rectPos, on simplifie W = (end-rectPos)-(start-rectPos)
  
  TFirst = 0;
  TLast = TG_INFINITY;
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
  /*if(triPlane.ClassifyBox(box) != BOX_INTERSECT_PLANE)
    return false;*/
  direction = triPlane.GetNormal();
  
  //on est sur la normal du triangle => tous les points sont projetes sur le meme 
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


void tgQ3Level::_CollideAgainstTriangle(const Q3Defs::BspPatchFace& face)
{
  const tgPlane& triPlane = face.plane;
  /* ----------- BOX => Separating Axis Theorem ----------- */
  if(traceType == TRACE_BOX) {
    tgAABoundingBox* box = static_cast<tgAABoundingBox*>(pTraceVolume);
    float TFirst,TLast;
    if(BoxAgainstTri3D(face.points, box,TFirst, TLast, MStart, MEnd)) {
      //if(TFirst < TLast) {
        if(TFirst < traceResult.fFraction) {
          //tgVector3 colPoint = MStart + (MEnd-MStart)*TFirst;
          traceResult.fFraction = TFirst;
          traceResult.vColNormal = triPlane.GetNormal();
          traceResult.bBezier = true;
          //out << traceResult.vColNormal << endl;
          traceResult.bStartsOut = true;
          traceResult.bAllSolid = false;
        }
      //}
    }
    return;
  }
  
  /* ----------- SPHERE/RAY => collision contre plan + 2D point in triangle ----------- */
  float startFraction = -1.0f;
  float endFraction = 1.0f;
  bool startsOut = false;
  tgVector3 CandidateToHitNormal;
  float startDistance=0, endDistance=0;
  tgVector3 offset(0,0,0);
  
  if(traceType == TRACE_RAY) {
    startDistance = triPlane.GetDistance(MStart);
    endDistance = triPlane.GetDistance(MEnd);
  } else if(traceType == TRACE_SPHERE) {
    tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(pTraceVolume);
    offset.Set(sphere->GetRadius(),sphere->GetRadius(),sphere->GetRadius());
    startDistance = triPlane.GetDistance(MStart) - sphere->GetRadius();
    endDistance = triPlane.GetDistance(MEnd) - sphere->GetRadius();
  } else {
    tgout << "traceType incconu : " << traceType << endl;
  }
      
  if (startDistance > 0) startsOut = true;
      
  if (startDistance > 0 && endDistance > 0) //devant le plan
    return;
  if (startDistance <= 0 && endDistance <= 0) //derriere le plan
    return;
      
  if (startDistance > endDistance) {  
    float fraction = (startDistance - EPSILON) / (startDistance - endDistance);
    if(!PointInTriangle(MStart+(MEnd-MStart)*fraction, face.points[0]-offset, face.points[1]-offset, face.points[2]-offset)) 
      return;
        
    if (fraction > startFraction) {
      startFraction = fraction;
      CandidateToHitNormal = triPlane.GetNormal();
    }
  } else {   
    float fraction = (startDistance + EPSILON) / (startDistance - endDistance);
    if(!PointInTriangle(MStart+(MEnd-MStart)*fraction, face.points[0]-offset, face.points[1]-offset, face.points[2]-offset))
      return;
        
    if (fraction < endFraction ) {
      endFraction = fraction;
    }
  }
      
  if (startFraction < endFraction) {
    if (startFraction > -1 && startFraction < traceResult.fFraction) {
      if (startFraction < 0) startFraction = 0;
      traceResult.fFraction = startFraction;
      traceResult.vColNormal = CandidateToHitNormal; 
      traceResult.bBezier = false;  
      /*if (startsOut == false) {
        traceResult.bStartsOut = false;
        if (endsOut == false)
          traceResult.bAllSolid = true;
      }*/
    }
  }     
      
}

void tgQ3Level::_TraceAgainstPatch(Q3Defs::BspPatch* patch)
{
  if (!patch->pBBox->VolumeInside(pTraceVolume, MStart,MEnd))
    return;
  for (unsigned int i=0; i<patch->iNumIndices/3; i++) { //collision triangles par triangles
    _CollideAgainstTriangle(patch->pFaces[i]);
    
  }
}

void tgQ3Level::_TraceAgainstBrush(Q3Defs::BspBrush* brush)
{
  float startFraction = -1.0f;
  float endFraction = 1.0f;
  bool  startsOut = false;
  bool  endsOut = false;

  tgVector3 CandidateToHitNormal;
  //tgPlane CandidateToHitPlane;

  for (int i = 0; i < brush->numOfBrushSides; i++) {
    Q3Defs::BspBrushSide *brushSide = &pBrushSides[brush->brushSides + i];
    Q3Defs::BspPlane     *plane  = &pPlanes[brushSide->plane];
    
    float startDistance=0, endDistance=0;
    if (traceType == TRACE_RAY) { 
      startDistance = plane->GetDistance(MStart);
      endDistance = plane->GetDistance(MEnd);
    }
    else if (traceType == TRACE_SPHERE) 
    {
      tgBoundingSphere* sphere = static_cast<tgBoundingSphere*>(pTraceVolume);
      startDistance = plane->GetDistance(MStart) - sphere->GetRadius();
      endDistance = plane->GetDistance(MEnd) - sphere->GetRadius();
    }
    else if (traceType == TRACE_BOX) {
      tgAABoundingBox* box = static_cast<tgAABoundingBox*>(pTraceVolume);
      tgVector3 offset;
      offset.x = plane->vNormal.x < 0 ? box->GetMax().x : box->GetMin().x;
      offset.y = plane->vNormal.y < 0 ? box->GetMax().y : box->GetMin().y;
      offset.z = plane->vNormal.z < 0 ? box->GetMax().z : box->GetMin().z;
      startDistance = plane->GetDistance(MStart+offset);
      endDistance = plane->GetDistance(MEnd+offset);
    }
    else {
      tgout << "Mauvais traceType : " << traceType << endl;
    }
    
    if (startDistance > 0)  startsOut = true;

    if (endDistance > 0)  endsOut = true;

    if (startDistance > 0 && endDistance > 0) //les deux points sont devant le plan -> pas dans le brush
      return;
    
    if (startDistance <= 0 && endDistance <= 0)  //les deux points sont derrière le plan -> collision avec un autre plan
      continue;

    if (startDistance > endDistance) {  
      float fraction = (startDistance - EPSILON) / (startDistance - endDistance);
      if (fraction < 0)
        fraction = 0;

      if (fraction > startFraction) {
        startFraction = fraction;
        CandidateToHitNormal = plane->vNormal;
      }
    }
    else {   
      float fraction = (startDistance + EPSILON) / (startDistance - endDistance);
      if (fraction > 1)
        fraction = 1;
      if (fraction < endFraction) {
        endFraction = fraction;
        //on update pas CandidateToHitNormal
      }
    }
  }

  if (startsOut == false) {
    traceResult.bStartsOut = false;

    if (endsOut == false)
      traceResult.bAllSolid = true;
    return;
  }
  
  //il y a eu une collision contre le brush
  if (startFraction < endFraction) {
    if (startFraction > -1 && startFraction < traceResult.fFraction) {
      if (startFraction < 0) startFraction = 0;
      traceResult.fFraction    = startFraction;
      traceResult.vColNormal = CandidateToHitNormal;
      traceResult.bStartsOut = startsOut;
      traceResult.bBezier = false;
      traceResult.bAllSolid = startsOut & endsOut;
    }
  }
}

void tgQ3Level::Think()
  {
  /*pEntityList.clear();
  for(int i=0; i<numOfLeafs; i++) {
    pLeafs[i].entityList.clear();
  }*/
}

void tgQ3Level::RemoveEntity(tgIEntity* entity)
{
  pEntityList.erase(entity);
}

void tgQ3Level::AddEntity(tgIEntity* entity)
{
  pEntityList.insert(entity);
/*  tgvector<int>* pVector = &entityMap[entity];
  pVector->clear();
  int num = _FindBoxLeaves(entity->GetBoundingBox()->GetWorldMin(), entity->GetBoundingBox()->GetWorldMax(), pVector, 0);
  
  
  for(int i=0; i<num; i++) {
    cout << (*pVector)[i] << "\t";
    pLeafs[(*pVector)[i]].entityList.push_back(entity);
  }
  cout << endl;*/
}

void tgQ3Level::GetTouchingEntities (tgBoundingVolume* volume, tgvector<tgIEntity*>& list)
{
  for(tgset<tgIEntity*>::iterator i=pEntityList.begin(); i!=pEntityList.end(); i++) {
    if ((*i)->GetBoundingVolume() && volume->VolumeInside((*i)->GetBoundingVolume())) {
      list.push_back((*i));
    }
  }
}

/**
 * On cree trois plans parallelles aux aretes (et avec des normales perpendiculaires aux aretes) et si la sphere est en-dehors de l'un de ces plans, elle n'entre pas en collision avec le triangle. De meme, si elle n'intersect pas avec le plan principal du triangle, elle est trop au-dessus ou au-dessous
 *     2
 *    - -
 *   -   -
 *  -     -
 * 1-------3  
 */
bool SphereIntersectTriangle(tgBoundingSphere* pBSphere, const tgVector3& v1, const tgVector3& v2, const tgVector3& v3)
{
  tgPlane trianglePlane;
  trianglePlane.SetFromPoints(v1,v2,v3);

  //plan principal
  if(trianglePlane.ClassifySphere(pBSphere) != SPHERE_INTERSECT_PLANE)
    return false;
  
  tgPlane p12, p23, p31;
  int c12,c23,c31;
  tgVector3 vTmp;

  //p12
  vTmp = v2 + trianglePlane.vNormal*10;
  p12.SetFromPoints(v1,v2,vTmp);
  c12 = p12.ClassifySphere(pBSphere);
  //p23
  vTmp = v3 + trianglePlane.vNormal*10;
  p23.SetFromPoints(v2,v3,vTmp);
  c23 = p23.ClassifySphere(pBSphere);
  //p31
  vTmp = v1 + trianglePlane.vNormal*10;
  p31.SetFromPoints(v3,v1,vTmp);
  c31 = p31.ClassifySphere(pBSphere);
  if ((c12 == SPHERE_IN_FRONT_OF_PLANE) || (c23 == SPHERE_IN_FRONT_OF_PLANE) || (c31 == SPHERE_IN_FRONT_OF_PLANE))
    return false;
  return true;
}

void tgQ3Level::GetFacesMarked(tgBoundingSphere* pBSphere, tgDecal* decal)
{
  tgvector<int> leavesList;
  _RecursiveSphereLeaves(0, pBSphere, &leavesList);
  
  //retrouve la liste des faces
  for (unsigned int i=0; i<leavesList.size(); i++) {
    Q3Defs::MyLeaf* leaf = &pLeafs[leavesList[i]];
    for(int j=0; j<leaf->numOfLeafFaces; j++) {
      int faceIndex = pLeafFaces[leaf->leafface+j];

      if(pDirectory[faceIndex].type == Q3Defs::PolygonFace) {
        Q3Defs::BspPolygonFace* pof = &pPolygonFaces[pDirectory[faceIndex].typeIndex];
        if ((pof->plane.ClassifySphere(pBSphere) == SPHERE_INTERSECT_PLANE)) {
          //FIXME: 5 est-elle une bonne valeur ?
          //le but est de ne pas appliquer le decal sur les deux cotes d'un mur par example
          //FIXME: pour check la double-application sur un mur, il vaut mieux comparer les normales
          if(pof->plane.GetDistance(pBSphere->GetPosition()) > /*-5*/0 ){
            for(int k=0; k<pof->numOfMeshVerts; k+=3) {
              int tmp = pMeshIndices[pof->firstMeshIndex+k];
              const tgVector3& point1 = pVerts[pof->startVertIndex+tmp].vPosition;
              
              tmp = pMeshIndices[pof->firstMeshIndex+k+1];
              const tgVector3& point2 = pVerts[pof->startVertIndex+tmp].vPosition;
              
              tmp = pMeshIndices[pof->firstMeshIndex+k+2];
              const tgVector3& point3 = pVerts[pof->startVertIndex+tmp].vPosition;
              if(SphereIntersectTriangle(pBSphere, point1, point2, point3)) {
                decal->AddTriangleFace(point1, point2, point3);
                //decal->AddPlane(/*pof->plane*/tgPlane(point1,point2,point3));
              }
            }
          }
        }
        
      } else if (pDirectory[faceIndex].type == Q3Defs::PatchFace) {
        #if 1
        Q3Defs::BspPatch* patch = &pPatches[pDirectory[faceIndex].typeIndex];
        for (unsigned int k=0; k<patch->iNumIndices; k+=3) { //collision triangles par triangles
          const tgVector3& point1 = patch->pVertices[patch->pIndices[k]].vPosition;
          const tgVector3& point2 = patch->pVertices[patch->pIndices[k+1]].vPosition;
          const tgVector3& point3 = patch->pVertices[patch->pIndices[k+2]].vPosition;
          
          if(SphereIntersectTriangle(pBSphere, point1, point2, point3)) {
            decal->AddTriangleFace(point1,point2,point3);
            //decal->AddPlane(tgPlane(point1,point2,point3));
          }
        }
        #endif
        //out <<"patch :(" << endl;
      } else if (pDirectory[faceIndex].type == Q3Defs::MeshFace) {
        //out << "mesh :(" << endl;
      }
    }
  }
  float radius = pBSphere->GetRadius();
  decal->SetBox(tgAABoundingBox(tgVector3(-radius,-radius,-radius), tgVector3(radius,radius,radius), pBSphere->GetPosition()));
  decal->Clip();
}

void tgQ3Level::_RecursiveSphereLeaves(int nodenum,const tgBoundingSphere* pBSphere, tgvector<int>* pList)
{
  while(nodenum >= 0) {
    Q3Defs::BspNode* node = &pNodes[nodenum];
    Q3Defs::BspPlane* bspPlane = &pPlanes[node->plane];
    int s = bspPlane->ClassifySphere(pBSphere);

    if (s==SPHERE_BEHIND_PLANE) nodenum = node->back;
    else if (s==SPHERE_IN_FRONT_OF_PLANE) nodenum = node->front;
    else {//SPHERE_INTERSECT_PLANE
      _RecursiveSphereLeaves(node->back, pBSphere, pList);
      nodenum = node->front;
    }
  }
  pList->push_back(~nodenum);
}

void tgQ3Level::_LoadEntities(int file)
{
  pFileManager->Seek(file, lumps[Q3Defs::kEntities].offset);
  char* entityString = new char[numOfLoadEntities];
  pFileManager->ReadBinary(file, (char*)entityString, numOfLoadEntities);
//  out << "entites -------------------: " << endl << entityString << endl;

  //on va compter le nombre de structs 
  numOfEntities = 0;
  char* c = entityString;
  tgstring buffer;
  //on parcourt chaque caractere de la chaine entites
  while(*c != '\0') {
    //on remplis notre buffer tant qu'on a pas un separateur
    buffer.clear();
    while (!(*c=='\n' || *c==' ' || *c=='\t')) {
      buffer += *c;
      c++;
      if(*c == '\0')
        break;
    }
    
    if (buffer.find("classname") != tgstring::npos) {
      numOfEntities++;
    }

    if(*c == '\0')
      break;
    c++;
  }

  //allocation memoire entites
  tgout << "nombre de entities: \t" << numOfEntities++ << endl;
  pEntities = new Q3Defs::BspEntity[numOfEntities];


  //parsing entites
  c = entityString;
  for (int i=0; i<numOfEntities; i++) {
    //cout << "--- entite: " << i << " ---" << endl;
    buffer.clear();
    while(!(*c=='{' || *c=='\0')) {
      buffer += *c;
      c++;
    }
    //en theorie, il n'y a jamais de {} imbriques dans les entites, mais sait-on jamais
    int num_open = 1;
    int state = 0;
    tgstring prop_name,prop_value;
    while(num_open && *c!='\0') {
      c++;
      if(*c=='}')
        num_open--;
      if(*c=='{')
        num_open++;
      tgstring name;
      if(*c=='"') {
        c++;
        //tant qu'on a pas les guillemets finaux, on est toujours dans le nom
        while(*c!='"') {
          name += *c;
          c++;
        }
        if(state == 0) {
          state = 1;
          prop_name = name;
        } else {
          state = 0;
          prop_value = name;
          pEntities[i].mProperties[prop_name] = prop_value;
          //cout << prop_name << "\t" << prop_value << endl;
        }
      }
    }
  }
  delete [] entityString;

  //on remplis les structs associes
  for(int i=0; i<numOfEntities; i++) {
    if(pEntities[i].mProperties["classname"].find("info_player_deathmatch") != tgstring::npos) {
      _entInfoPlayerDeathmatch info;
      info.vOrigin.FromString(pEntities[i].mProperties["origin"]);
      info.vOrigin.ChangeCoords();
      cout << "info_player_deathmatch origin : " << info.vOrigin << endl;
      infoPlayerDeathmatch.push_back(info);
    }
  }

}

bool tgQ3Level::_LoadBSP(const char* levelname)
{
  int file = pFileManager->Open(levelname, TGFILE_BINARY);
  if (file == -1) {
    //on a deja un message d'erreur dans tgFileManager
    return false;
  }
  pFileManager->ReadBinary(file, (char*)&header, sizeof(Q3Defs::BspHeader));
  
  /*TODO: décommentez apres l'introduction de la gestion endian
  if (!Engine::pEndian->bLittle)
  {
    char orig[4];
    for (int i=0; i<4; i++)
      orig[i] = header.strID[i];
    header.strID[0] = orig[3];
    header.strID[1] = orig[2];
    header.strID[2] = orig[1];
    header.strID[3] = orig[0];
  }*/
  SWAPLONG(header.version)
  
  pFileManager->ReadBinary(file, (char*)&lumps, sizeof(Q3Defs::BspLump)*Q3Defs::kMaxLumps);
  for (int i=0; i<Q3Defs::kMaxLumps; i++)
  {
    SWAPLONG(lumps[i].offset)
    SWAPLONG(lumps[i].length)
  }
  
  numOfVerts = lumps[Q3Defs::kVertices].length / sizeof (Q3Defs::BspVertex);
  pVerts = new Q3Defs::BspVertex [numOfVerts];
  //pVertices = new tgVertice[numOfVerts];
  //allocation memoire pour les faces
  numOfFaces = lumps[Q3Defs::kFaces].length / sizeof(Q3Defs::BspFace);
  pFaces = new Q3Defs::BspFace [numOfFaces];
  pVisibleFaces = new bool[numOfFaces];
  pDirectory = new Q3Defs::BspFaceDirectoryEntry[numOfFaces];
  //allocation memoire pour les textures
  numOfTextures = lumps[Q3Defs::kTextures].length /sizeof (Q3Defs::BspTexture);
  pTexturesInfos = new Q3Defs::BspTexture [numOfTextures];
  pMaterials = new tgMaterial*[numOfTextures];
  pContents = new int [numOfTextures];
  //allocation memoire pour les leafs
  numOfLeafs = lumps[Q3Defs::kLeafs].length/sizeof (Q3Defs::BspLeaf);
  pLeafs = new Q3Defs::MyLeaf [numOfLeafs];
  //allocation memoire pour les nodes
  numOfNodes = lumps[Q3Defs::kNodes].length / sizeof(Q3Defs::BspNode);
  pNodes     = new Q3Defs::BspNode [numOfNodes];
  //allocation memoire pour les leafFace
  numOfLeafFaces = lumps[Q3Defs::kLeafFaces].length / sizeof(int);
  pLeafFaces     = new int [numOfLeafFaces];
  //allocation memoire pour les plans
  numOfPlanes = lumps[Q3Defs::kPlanes].length / sizeof(Q3Defs::BspLoadPlane);
  pPlanes     = new Q3Defs::BspPlane [MAX_CM_PLANES]; //on garde de la place pour les éventuels plans supplémentaires (ceux des patches)
  if (numOfPlanes > MAX_CM_PLANES)
    tgerr << "numOfPlanes > MAX_CM_PLANES" << endl;
  planeCount = numOfPlanes;
  
  //allocation memoire pour les lightmaps
  numOfLightmaps = lumps[Q3Defs::kLightmaps].length / sizeof(Q3Defs::BspLightmap);
  pLightMapsInfos = new Q3Defs::BspLightmap [numOfLightmaps];
  pLightMaps = new unsigned int[numOfLightmaps];
  //allocation memoire pour les leafBrushes
  numOfLeafBrushes = lumps[Q3Defs::kLeafBrushes].length / sizeof(int);
  pLeafBrushes = new int [numOfLeafBrushes];
  //allocation memoire pour les brushes
  numOfBrushes = lumps[Q3Defs::kBrushes].length / sizeof(Q3Defs::BspBrush);
  pBrushes = new Q3Defs::BspBrush[MAX_CM_BRUSHES]; //on garde de la place pour les éventuels brushes supplémentaires (ceux des patches),
  if (numOfBrushes > MAX_CM_BRUSHES)
    tgerr << "numOfBrushes > MAX_CM_BRUSHES" << endl;
  brushesCount = numOfBrushes;
  //allocation memoire pour les brushes sides
  numOfBrushSides = lumps[Q3Defs::kBrushSides].length / sizeof(Q3Defs::BspBrushSide);
  pBrushSides = new Q3Defs::BspBrushSide[MAX_CM_BRUSHSIDES]; //on garde de la place pour les éventuels brushsides supplémentaires (ceux des patches)
  if (numOfBrushSides > MAX_CM_BRUSHSIDES)
    tgerr << "numOfBrushSides > MAX_CM_BRUSHSIDES" << endl;
  brushSidesCount = numOfBrushSides;

  //allocation memoire pour les indices de mesh
  numOfMeshIndices = lumps[Q3Defs::kMeshVerts].length / sizeof(int);
  pMeshIndices = new int[numOfMeshIndices];
  //allocation memoire pour les effets
  numOfShaders = lumps[Q3Defs::kShaders].length / sizeof(Q3Defs::BspShader);
  pShaders = new Q3Defs::BspShader[numOfShaders];
  //allocation memoire pour les entites
  numOfLoadEntities = lumps[Q3Defs::kEntities].length;
  //pEntities = new Q3Defs::BspEntity[numOfEntities];
  
  #ifdef DEBUG
  tgout << "nombre de vertices :\t" << numOfVerts << endl;
  tgout << "nombre de faces :\t" << numOfFaces << endl;
  tgout << "nombre de textures:\t" << numOfTextures << endl;
  tgout << "nombre de nodes:\t" << numOfNodes << endl;
  tgout << "nombre de leafs:\t" << numOfLeafs << endl;
  tgout << "nombre de leaffaces:\t" << numOfLeafFaces << endl;
  tgout << "nombre de plans:\t" << numOfPlanes << endl;
  tgout << "nombre de leafbrushes:\t" << numOfLeafBrushes << endl;
  tgout << "nombre de brushes:\t" << numOfBrushes << endl;
  tgout << "nombre de brushsides:\t" << numOfBrushSides << endl;
  tgout << "nombre de meshindices:\t" << numOfMeshIndices << endl;
  tgout << "nombre de shaders:\t" << numOfShaders << endl;
  tgout << "nombre de lightmaps:\t" << numOfLightmaps << endl;
  #endif
  
  //TODO: charger les entites !
  _LoadEntities (file);
  
  //positionement sur le debut de la section vertices
  pFileManager->Seek(file, lumps[Q3Defs::kVertices].offset);
  pFileManager->ReadBinary(file, (char*)pVerts, sizeof(Q3Defs::BspVertex)*numOfVerts);
  for (int i=0; i<numOfVerts; i++)
  {
    SWAPFLOAT(pVerts[i].vPosition.x)
    SWAPFLOAT(pVerts[i].vPosition.y)
    SWAPFLOAT(pVerts[i].vPosition.z)
    SWAPFLOAT(pVerts[i].vTextureCoord[0])
    SWAPFLOAT(pVerts[i].vTextureCoord[1])
    SWAPFLOAT(pVerts[i].vLightmapCoord[0])
    SWAPFLOAT(pVerts[i].vLightmapCoord[1])
    SWAPFLOAT(pVerts[i].vNormal.x)
    SWAPFLOAT(pVerts[i].vNormal.y)
    SWAPFLOAT(pVerts[i].vNormal.z)
  }
  //lecture des donnes sur les vertices
  for ( int i=0; i<numOfVerts; i++)
  { 
    //pVerts[i].vTextureCoord[0] = 1 - pVerts[i].vTextureCoord[0];
    pVerts[i].vTextureCoord[1] = 1 - pVerts[i].vTextureCoord[1];
    
    pVerts[i].vPosition.ChangeCoords();
    pVerts[i].vNormal.ChangeCoords();
    
    //remplissage du tableau pVertices, utilise lors du bump-mapping
    /*pVertices[i].vPosition = pVerts[i].vPosition;
    pVertices[i].vNormal = pVerts[i].vNormal;
    pVertices[i].s = pVerts[i].vTextureCoord[0];
    pVertices[i].t = pVerts[i].vTextureCoord[1];*/
  }
  
  
  //chargement des mesh indices
  pFileManager->Seek(file, lumps[Q3Defs::kMeshVerts].offset);
  pFileManager->ReadBinary(file, (char*)pMeshIndices, sizeof(int)*numOfMeshIndices);
  for (int i=0; i<numOfMeshIndices; i++)
    SWAPLONG(pMeshIndices[i])
  
  //chargement des textures
  pFileManager->Seek(file, lumps[Q3Defs::kTextures].offset);
  pFileManager->ReadBinary(file, (char*)pTexturesInfos, sizeof(Q3Defs::BspTexture)*numOfTextures);
  for (int i=0; i<numOfTextures; i++)
  {
    SWAPLONG(pTexturesInfos[i].flags)
    SWAPLONG(pTexturesInfos[i].contents)
    pContents[i] = pTexturesInfos[i].contents;
  }
  _LoadTextures ();
  //chargement des lightmaps
  pFileManager->Seek(file, lumps[Q3Defs::kLightmaps].offset);
  pFileManager->ReadBinary(file, (char*)pLightMapsInfos, sizeof(Q3Defs::BspLightmap)*numOfLightmaps);
  _LoadLightmaps ();
  
  //chargement des shaders
  pFileManager->Seek(file, lumps[Q3Defs::kShaders].offset);
  pFileManager->ReadBinary(file, (char*)pShaders, sizeof(Q3Defs::BspShader)*numOfShaders);
  for (int i=0; i<numOfShaders; i++)
  {
    SWAPLONG(pShaders[i].brush);
    SWAPLONG(pShaders[i].unknown);
  }
  
  //chargement des nodes
  pFileManager->Seek(file, lumps[Q3Defs::kNodes].offset);
  pFileManager->ReadBinary(file, (char*)pNodes, sizeof(Q3Defs::BspNode)*numOfNodes);
  for (int i=0; i<numOfNodes; i++)
  {
    SWAPLONG(pNodes[i].front)
    SWAPLONG(pNodes[i].back)
    SWAPLONG(pNodes[i].plane)
    SWAPFLOAT(pNodes[i].min.x)
    SWAPFLOAT(pNodes[i].min.y)
    SWAPFLOAT(pNodes[i].min.z)
    SWAPFLOAT(pNodes[i].max.x)
    SWAPFLOAT(pNodes[i].max.y)
    SWAPFLOAT(pNodes[i].max.z)
    pNodes[i].min.ChangeCoords();
    pNodes[i].max.ChangeCoords();
  }
  
  //chargement des leafs
  pFileManager->Seek(file, lumps[Q3Defs::kLeafs].offset);
  Q3Defs::BspLeaf* pReadedLeaves = new Q3Defs::BspLeaf[numOfLeafs];
  pFileManager->ReadBinary(file, (char*)pReadedLeaves, sizeof(Q3Defs::BspLeaf)*numOfLeafs);
  
  for (int i=0; i<numOfLeafs; i++)
  {
    SWAPLONG(pReadedLeaves[i].cluster)
    SWAPLONG(pReadedLeaves[i].area)
    SWAPFLOAT(pReadedLeaves[i].min.x)
    SWAPFLOAT(pReadedLeaves[i].min.y)
    SWAPFLOAT(pReadedLeaves[i].min.z)
    SWAPFLOAT(pReadedLeaves[i].max.x)
    SWAPFLOAT(pReadedLeaves[i].max.y)
    SWAPFLOAT(pReadedLeaves[i].max.z)
    SWAPLONG(pReadedLeaves[i].leafface)
    SWAPLONG(pReadedLeaves[i].numOfLeafFaces)
    SWAPLONG(pReadedLeaves[i].leafBrush)
    SWAPLONG(pReadedLeaves[i].numOfLeafBrushes)
    pReadedLeaves[i].min.ChangeCoords();
    pReadedLeaves[i].max.ChangeCoords();
    
    //copie dans la structure finale
    pLeafs[i].cluster = pReadedLeaves[i].cluster;
    pLeafs[i].area = pReadedLeaves[i].area;
    pLeafs[i].box.SetMin(pReadedLeaves[i].min);
    pLeafs[i].box.SetMax(pReadedLeaves[i].max);
    pLeafs[i].box.SetPosition(tgVector3(0,0,0));
    pLeafs[i].leafface = pReadedLeaves[i].leafface;
    pLeafs[i].numOfLeafFaces = pReadedLeaves[i].numOfLeafFaces;
    pLeafs[i].leafBrush = pReadedLeaves[i].leafBrush;
    pLeafs[i].numOfLeafBrushes = pReadedLeaves[i].numOfLeafBrushes;
    pLeafs[i].leafPatch = 0;
    pLeafs[i].numOfLeafPatches = 0;
  }
  delete [] pReadedLeaves;
  
  //chargement des faces des leafs
  pFileManager->Seek(file, lumps[Q3Defs::kLeafFaces].offset);
  pFileManager->ReadBinary(file, (char*)pLeafFaces, sizeof(int)*numOfLeafFaces);
  for (int i=0; i<numOfLeafFaces; i++)
    SWAPLONG(pLeafFaces[i])
  
  //chargement des plans
  Q3Defs::BspLoadPlane* pLoadPlanes = new Q3Defs::BspLoadPlane[numOfPlanes];
  pFileManager->Seek(file, lumps[Q3Defs::kPlanes].offset);
  pFileManager->ReadBinary(file, (char*)pLoadPlanes, sizeof(Q3Defs::BspLoadPlane)*numOfPlanes);
  for (int i=0; i<numOfPlanes; i++)
  {
    SWAPFLOAT(pLoadPlanes[i].fD)
    SWAPFLOAT(pLoadPlanes[i].vNormal.x)
    SWAPFLOAT(pLoadPlanes[i].vNormal.y)
    SWAPFLOAT(pLoadPlanes[i].vNormal.z)
    pLoadPlanes[i].vNormal.ChangeCoords();
    pLoadPlanes[i].fD = -pLoadPlanes[i].fD;
    
    pPlanes[i].vNormal = pLoadPlanes[i].vNormal;
    pPlanes[i].fD = pLoadPlanes[i].fD;
    /*SWAPFLOAT(pPlanes[i].fD)
    SWAPFLOAT(pPlanes[i].vNormal.x)
    SWAPFLOAT(pPlanes[i].vNormal.y)
    SWAPFLOAT(pPlanes[i].vNormal.z)
    pPlanes[i].vNormal.ChangeCoords();*/
  }
  delete [] pLoadPlanes;
    
  //chargement des donnes de visibilite
  pFileManager->Seek(file, lumps[Q3Defs::kVisData].offset);
  if (lumps[Q3Defs::kVisData].length) //on verifie qu'il y ait des infos de visibilite
  {
    pFileManager->ReadBinary(file, (char*)&clusters.numOfClusters, sizeof(int));
    SWAPLONG(clusters.numOfClusters)
    pFileManager->ReadBinary(file, (char*)&clusters.bytesPerCluster, sizeof(int));
    SWAPLONG(clusters.bytesPerCluster)
    int size = clusters.numOfClusters*clusters.bytesPerCluster;
    clusters.pBitsets = new unsigned char[size];
    
    pFileManager->ReadBinary(file, (char*)clusters.pBitsets, sizeof(unsigned char)*size);
  }
  else //sinon, on a pas d'infos de visibilite
    clusters.pBitsets = NULL;
    
  //chargement des Brushes
  pFileManager->Seek(file, lumps[Q3Defs::kBrushes].offset);
  pFileManager->ReadBinary(file, (char*)pBrushes, sizeof(Q3Defs::BspBrush)*numOfBrushes);
  for (int i=0; i<numOfBrushes; i++)
  {
    SWAPLONG(pBrushes[i].brushSides)
    SWAPLONG(pBrushes[i].numOfBrushSides)
    SWAPLONG(pBrushes[i].textureID)
  }
  
  //chargement des LeafBrushes
  pFileManager->Seek(file, lumps[Q3Defs::kLeafBrushes].offset);
  pFileManager->ReadBinary(file, (char*)pLeafBrushes, sizeof(int)*numOfLeafBrushes);
  for (int i=0; i<numOfLeafBrushes; i++)
    SWAPLONG(pLeafBrushes[i])
  
  //chargement des BrushSides
  pFileManager->Seek(file, lumps[Q3Defs::kBrushSides].offset);
  pFileManager->ReadBinary(file, (char*)pBrushSides, sizeof(Q3Defs::BspBrushSide)*numOfBrushSides);
  for (int i=0; i<numOfBrushSides; i++)
  {
    SWAPLONG(pBrushSides[i].plane)
    SWAPLONG(pBrushSides[i].textureID)
  }
  

  //positionement sur le debut de la section faces
  pFileManager->Seek(file, lumps[Q3Defs::kFaces].offset);
  pFileManager->ReadBinary(file, (char*)pFaces, sizeof(Q3Defs::BspFace)*numOfFaces);
  for (int i=0; i<numOfFaces; i++)
  {
    SWAPLONG(pFaces[i].textureID)
    SWAPLONG(pFaces[i].effect)
    SWAPLONG(pFaces[i].type)
    SWAPLONG(pFaces[i].startVertIndex)
    SWAPLONG(pFaces[i].numOfVerts)
    SWAPLONG(pFaces[i].meshVertIndex)
    SWAPLONG(pFaces[i].numMeshVerts)
    SWAPLONG(pFaces[i].lightmapID)
    SWAPLONG(pFaces[i].lMapCorner[0])
    SWAPLONG(pFaces[i].lMapCorner[1])
    SWAPLONG(pFaces[i].lMapSize[0])
    SWAPLONG(pFaces[i].lMapSize[1])
    SWAPFLOAT(pFaces[i].lMapPos[0])
    SWAPFLOAT(pFaces[i].lMapPos[1])
    SWAPFLOAT(pFaces[i].lMapPos[2])
    SWAPFLOAT(pFaces[i].lMapVecs[0][0])
    SWAPFLOAT(pFaces[i].lMapVecs[0][1])
    SWAPFLOAT(pFaces[i].lMapVecs[0][2])
    SWAPFLOAT(pFaces[i].lMapVecs[1][0])
    SWAPFLOAT(pFaces[i].lMapVecs[1][1])
    SWAPFLOAT(pFaces[i].lMapVecs[1][2])
    SWAPFLOAT(pFaces[i].vNormal.x)
    SWAPFLOAT(pFaces[i].vNormal.y)
    SWAPFLOAT(pFaces[i].vNormal.z)
    SWAPLONG(pFaces[i].size[0])
    SWAPLONG(pFaces[i].size[1])
  }   
  
  
  numOfPatches = 0;
  numOfPolygonFaces = 0;
  numOfMeshFaces = 0;
  for (int i=0; i<numOfFaces; i++)
  {
    pFaces[i].vNormal.ChangeCoords();
    if (pFaces[i].type == Q3Defs::PolygonFace)
      numOfPolygonFaces++;
    else if (pFaces[i].type == Q3Defs::PatchFace) 
      numOfPatches++;
    else if (pFaces[i].type == Q3Defs::MeshFace)
      numOfMeshFaces++;
  }
      
  tgout << "nombre de patches:\t" << numOfPatches << std::endl;
  tgout << "nombre de faces polygonales:\t" << numOfPolygonFaces << std::endl;
  
  pPatches = new Q3Defs::BspPatch[numOfPatches];
  pPolygonFaces = new Q3Defs::BspPolygonFace[numOfPolygonFaces];
  pMeshFaces = new Q3Defs::BspMeshFace[numOfMeshFaces];
  
  Q3Defs::BspPolygonFace* pof;
  Q3Defs::BspPatch* pat;
  Q3Defs::BspMeshFace* mes;
  
  int patchesCount = 0;
  int polyCount = 0;
  int meshCount = 0;
  
  for (int i=0; i<numOfFaces; i++)
  {
    if (pFaces[i].type == Q3Defs::PolygonFace)
    {
      pof = &pPolygonFaces[polyCount];
      
      pDirectory[i].type = Q3Defs::PolygonFace;
      pDirectory[i].typeIndex = polyCount;
      
      pof->textureID = pFaces[i].textureID;
      pof->startVertIndex = pFaces[i].startVertIndex;
      pof->numOfVerts = pFaces[i].numOfVerts;
      pof->lightmapID = pFaces[i].lightmapID;
      pof->numOfMeshVerts = pFaces[i].numMeshVerts;
      pof->firstMeshIndex = pFaces[i].meshVertIndex;
      
      tgVector3 points[3];
      points[0] = pVerts[pof->startVertIndex].vPosition;
      points[1] = pVerts[pof->startVertIndex+1].vPosition;
      points[2] = pVerts[pof->startVertIndex+2].vPosition;

      pof->plane.SetFromPoints(points[0], points[2], points[1]);  
      //Calcul du centre et du "rayon" de la face
      tgVector3 vMin (-TG_INFINITY, -TG_INFINITY, -TG_INFINITY);
      tgVector3 vMax (TG_INFINITY, TG_INFINITY, TG_INFINITY);
            
      for (int j=pof->startVertIndex; j < pof->startVertIndex + pof->numOfVerts; j++)
      {
        tgVector3& v = pVerts[j].vPosition;
        tgVector3::AddPointToBounds(v,vMin,vMax);
      }
      pof->sphere.SetPosition((vMax+vMin)/2);
      pof->sphere.SetRadius((vMax-vMin).Length()/2);

      polyCount++;
    }
    else if (pFaces[i].type == Q3Defs::PatchFace)
    {
      pat = &pPatches[patchesCount];
      
      pDirectory[i].type = Q3Defs::PatchFace;
      pDirectory[i].typeIndex = patchesCount;
      
      pat->size[0] = pFaces[i].size[0];
      pat->size[1] = pFaces[i].size[1];
      pat->textureID = pFaces[i].textureID;
      pat->lightmapID = pFaces[i].lightmapID;
      
      
      pat->controls = new Q3Defs::BspVertex[pFaces[i].numOfVerts];
      
      for (int j=0; j<pFaces[i].numOfVerts; j++)
      {
        pat->controls[j] = pVerts[pFaces[i].startVertIndex+j];
      }
      
      _LoadPatch(pat, 3);
      
      patchesCount++;
    }
    else if (pFaces[i].type == Q3Defs::MeshFace)
    {
      mes = &pMeshFaces[meshCount];
      
      pDirectory[i].type = Q3Defs::MeshFace;
      pDirectory[i].typeIndex = meshCount;
      
      mes->textureID = pFaces[i].textureID;
      mes->firstMeshIndex = pFaces[i].meshVertIndex;
      mes->lightmapID = pFaces[i].lightmapID;
      mes->numOfMeshVerts = pFaces[i].numMeshVerts;
      mes->numOfVerts = pFaces[i].numOfVerts;
      mes->startVertIndex = pFaces[i].startVertIndex;
    

      tgVector3 vMin (-TG_INFINITY, -TG_INFINITY, -TG_INFINITY);
      tgVector3 vMax (TG_INFINITY, TG_INFINITY, TG_INFINITY);
            
      for (int j=mes->startVertIndex; j < mes->startVertIndex + mes->numOfVerts; j++)
      {
        tgVector3& v = pVerts[j].vPosition;
        tgVector3::AddPointToBounds(v,vMin,vMax);
      }
      mes->sphere.SetPosition((vMax+vMin)/2);
      mes->sphere.SetRadius((vMax-vMin).Length()/2);

      meshCount++;
    }
  }



  
  pFileManager->Close(file);
  return true;
}
