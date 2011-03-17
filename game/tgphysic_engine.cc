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

#include "tgphysic_engine.h"
#include "tggame.h"

tgPhysicEngine::tgPhysicEngine()
{
  pGravity = new tgForce(tgVector3(0,-10000,0)); 
}

tgPhysicEngine::~tgPhysicEngine()
{
  delete pGravity;
}

void tgPhysicEngine::AddNode(tgPhysicNode* node)
{
  tNodes.insert(node);
  if (node->GetEntity() != NULL)
    mEntityToNode.insert(std::make_pair(node->GetEntity(), node));
}

void tgPhysicEngine::RemoveNode(tgPhysicNode* node)
{
  tNodes.erase(node);	
  if (node->GetEntity() != NULL)
    mEntityToNode.erase(node->GetEntity());
}


/** uses interpolation, see
 * http://www.gaffer.org/game-physics/fix-your-timestep/
 */
void tgPhysicEngine::MoveNodes(float eTime)
{
  const float dt = 0.01f;
  static float accumulator = 0.0f;
 
  for(t_nodes::iterator i	= tNodes.begin(); i!= tNodes.end(); i++) {
    tgPhysicNode* node = (*i);		
    node->SetPosition(node->GetPreviousPosition());
  }
 
  accumulator += eTime;
  while (accumulator >= dt) {

    for(t_nodes::iterator i	= tNodes.begin(); i!= tNodes.end(); i++) {
      tgPhysicNode* node = (*i);
      node->SetPreviousPosition(node->GetPosition());

      if(node->bNoClip) {
        node->UpdateAccel(dt);
        node->SetPosition(node->GetPosition()+node->GetAccel()*dt*dt+node->GetVelocity()*dt);
      } else {
        switch(node->GetPhysicType()) {
          case PHYS_WALK:
            //FIXME: pas besoin de faire un AddForce a chaque fois, il suffit de l'ajouter a la creation du physicNode
            node->AddForce(pGravity);
            node->UpdateAccel(dt);
            _MoveNode(node, dt);
            node->RemoveForce(pGravity);
            break;
          case PHYS_PROJ:
            node->UpdateAccel(dt);
            _MoveNode(node, dt);
            break;
          default:
            break;
        }
      }      
    }

    accumulator -= dt;
  }
  for(t_nodes::iterator i	= tNodes.begin(); i!= tNodes.end(); i++) {
    tgPhysicNode* node = (*i);	
    const tgVector3 tmp = node->GetPosition();
    const float alpha = accumulator / dt;

    node->SetPosition(node->GetPosition()*alpha + node->GetPreviousPosition()*(1.0f-alpha));
    node->SetPreviousPosition(tmp);
    //apres tous les mouvements, on met a jour la position de l'entity
    node->UpdateEntity();

  }


}
/**
 * _DoStepSlideMove effectue un deplacement qui essaie d'etre le plus fluide possible en montant les 
 * escaliers et en glissant contre les obstacles.
 * @return true si on a monte ou glisse contre un obstacle
 * @return false si le mouvement s'est fait d'une traite
 */

void tgPhysicEngine::_MoveNode(tgPhysicNode* node, float eTime)
{
  tgVector3 vNewPosition = node->GetPosition();
  tgVector3 vNewVelocity = node->GetAccel()*eTime + node->GetVelocity();
  //on travaille en temps normalise
  vNewVelocity *= eTime;
  //_PushNode(node, vNewPosition, vNewVelocity, 1.0f);
  _DoStepSlideMove(node, 1.0f, vNewVelocity, vNewPosition);

  #define INTERPOLATION_SPEED 15
  //FIXME: ne fonctionne pas bien dans curvy
  #if 0
  //interpolation uniquement sur les nodes au sol, pas sur les projectiles
  if (node->GetPhysicType() == PHYS_WALK) {
    float deltaY = vNewPosition.y - node->GetPosition().y;
    //interpolation du deplacement sur les y => smooth stairs climbing
    tgPhysicNode::Interpolation* interp = node->GetInterpolation();
    if (deltaY < 20 && deltaY > 1) {
      if(interp->progress < 1) { //deja une interpolation en cours
        interp->startHeight += interp->interpolation*interp->progress;
        interp->interpolation = vNewPosition.y - interp->startHeight;
      } else { //on en commence une nouvelle
        interp->startHeight = node->GetPosition().y;
        interp->interpolation = deltaY;
      }
      interp->progress = 0;
    }

    //pas d'interpolation sur les nodes en l'air
    if (node->GetPositionType() == POS_AIR) 
      interp->progress = 1;

    if (interp->progress < 1) {
      interp->progress += INTERPOLATION_SPEED*eTime;
      if (interp->progress > 1) 
        interp->progress = 1;
      vNewPosition.y = interp->startHeight + interp->progress*interp->interpolation;
    }
  }
  #endif
  node->SetAccel((vNewPosition-node->GetPosition())*node->GetMass()/(eTime*eTime));
  node->SetPosition(vNewPosition);
  _ClassifyPosition(node);
}

#define STEP_SIZE 20
void tgPhysicEngine::_DoStepSlideMove(tgPhysicNode* node, float eTime, tgVector3& vel, tgVector3& pos)
{
  tgVector3 start_o, start_v;
  tgVector3 down, up;
  tgVector3 down_o;
  tgCollisionResult trace;
  float stepSize;

  start_o = pos;
  start_v = vel;

  if(!_PushNode(node, pos, vel, eTime)) { // pas de collision, pas besoin de monter
    return;
  }

  down_o = pos;

  //trace vers le bas pour verifier qu'on est pas en l'air
  down = start_o;
  down.y -= STEP_SIZE;

  trace = pGame->GetWorldManager()->Trace(start_o, down, node->GetBVolume(), node->GetEntity());
  up.Set(0,1,0);

  //on ne monte pas si on a encore une vitesse vertical et que rien ne bloque verticalement (si fFraction == 1.0) ou que la pente est trop raide
  //trace.vColNormal est de longueur 1, up aussi -> trace.vColNormal*up = ||trace.vColNormal||*||up||*cos(angle) -> si cos(angle)<0.7, angle>45, pente trop raide
  if(vel.y > 0 && (trace.fFraction == 1.0 || trace.vColNormal*up < 0.7)) {
    return;
  }

  //on trace entre la position du joueur et la position STEPSIZE plus haut, pour eviter de monter a travers un plafond
  up = start_o;
  up.y += STEP_SIZE;

  trace = pGame->GetWorldManager()->Trace(start_o, up, node->GetBVolume(), node->GetEntity());
  if (trace.bAllSolid) {
    return;
  }

  //on monte le node au point STEPSIZE plus haut et on push avec la vitesse originale
  stepSize = trace.vEndPoint.y - start_o.y;
  pos = trace.vEndPoint;
  vel = start_v;

  _PushNode(node, pos, vel, eTime);
  //on redescend le node sur le sol
  down = pos;
  down.y -= stepSize;
  trace = pGame->GetWorldManager()->Trace(pos, down, node->GetBVolume(), node->GetEntity());
  if(!trace.bAllSolid) 
    pos = trace.vEndPoint;
  //on verifie qu'on ne redescend pas par rapport a un mouvement sans step
  if(trace.vEndPoint.y < down_o.y)
    pos = down_o;
  if(trace.fFraction < 1.0f)
    _ClipVelocity(vel, vel, trace.vColNormal);
  return;

}


#define MAX_BOUNCES 5
#if 1
bool tgPhysicEngine::_PushNode(tgPhysicNode* node, tgVector3& pos, tgVector3& vel, float time)
{
  int normalsCount = 0;
  tgVector3 normalsList[MAX_BOUNCES]; //les normales des plans qu'on a deja touche
  int bouncesCount = 0;

  // pour ne pas revenir contre la vitesse
  normalsList[normalsCount++] = vel.GetNormalized();

  for (bouncesCount = 0; (vel != tgVector3::ZERO) && (bouncesCount < MAX_BOUNCES); bouncesCount++) {
    tgVector3 eP = pos + vel; 
    tgCollisionResult cr = pGame->GetWorldManager()->Trace(pos, eP, node->GetBVolume(), node->GetEntity());

    if (cr.fFraction < 1.0f) {
      if (cr.pEntity != NULL)
        HandleTouch(node, cr);
      else
        node->HitWall(cr.vColNormal, cr.vEndPoint);
    }

    tgPlane colPlane;
    colPlane.SetFromPointAndNormal(cr.vEndPoint, cr.vColNormal);
    tgVector3 pP = colPlane.ProjectPoint(cr.vEndPoint);
    tgVector3 pV = colPlane.ProjectPoint(eP);

    vel = (pV-pP)*(1-cr.fFraction);

    pos = cr.vEndPoint;

    //on ne rajoute pas ce plan a la liste des normales s'il est deja dedans
    bool alreadyIn = false;
    for (int i=0; i<normalsCount; i++) {
      if ( cr.vColNormal*normalsList[i] > 0.99 ) {
        alreadyIn = true;
        vel += cr.vColNormal;
        break;
      }
    }
    if (alreadyIn)
      continue;
    normalsList[normalsCount++] = cr.vColNormal;

    if (normalsCount >= MAX_BOUNCES) {
      //FIXME: ce cas arrive souvent lorsqu'on slide contre le bord d'un portique en bezier
      // parce que les beziers utilisees pour les collisions sont les memes que pour le rendu 
      // et elles contiennent beaucoup trop de triangles 
      // => il faut creer un set de bezier qui contient nettement moins de triangles pour la collision
      //cerr << "normalsCount >= MAX_BOUNCES" << endl;
      //break;
      vel.Set(0,0,0);
      return true;
    }

    //on verifie que notre nouvelle vitesse ne revienne pas dans un plan touche precedemment
    for (int i=0; i<normalsCount; i++) {
      //si cos(angle forme par Vel et la Normal) est entre 0 et 1, on a un angle
      //entre -90 et 90 => la vitesse "sort" du plan => pas besoin de clipper
      //cos = (vel*planesList[i]) / (||vel||*||planesList[i]||)
      //mais puisqu'on s'occupe que du signe, on peut oublier la division
      if(vel*normalsList[i] >= 0)
        continue;

      //si on arrive ici, c'est que la vitesse coupe le plan normalsList[i] 
      _ClipVelocity(vel, vel, normalsList[i]);

      //on verifie que la nouvelle vitesse ne coupe pas encore un autre plan
      for (int j=0; j<normalsCount; j++) {
        if (j==i) 
          continue;
        if (vel*normalsList[j] >= 0)
          continue;

        //on coupe un deuxieme plane => on essaie de reclipper
        _ClipVelocity(vel, vel, normalsList[j]);

        //on reteste par rapport au plan precedent
        if (vel*normalsList[i] >= 0)
          continue;

        //si on arrive ici, la vitesse reentre dans le plan precedent => on calcule
        //une vitesse qui nous fais sortir des deux plans
        tgVector3 outDir = normalsList[i]%normalsList[j];
        outDir.Normalize();
        vel = outDir*vel.Length();

        //on reverifie une fois que la vitesse n'entre dans aucun plan
        for (int k=0; k<normalsCount; k++) {
          if (k==i || k==j)
            continue;
          if (vel*normalsList[k] >= 0)
            continue;
          //impossible de sortir de cette situation
          //cerr << "bizarre" << endl;
          vel.Set(0,0,0);
          return true;
        }
      }
    }
  }
  return (bouncesCount != 0);
  }
#endif

void tgPhysicEngine::_ClipVelocity(tgVector3& out, const tgVector3& in, const tgVector3& normal, float overbounce) const
{
  float backoff = in*normal;
  if(backoff < 0)
    backoff *= overbounce;
  else
    backoff /= overbounce;
  out = in-(normal*backoff);
}

void tgPhysicEngine::_ClassifyPosition(tgPhysicNode* n)
{
  tgCollisionResult trace;
  tgVector3 down = n->GetPosition()-tgVector3::YNORM*STEPSIZE;
  trace = pGame->GetWorldManager()->Trace(n->GetPosition(), down, n->GetBVolume(), n->GetEntity());
  if(trace.fFraction < 1.0f) {
    n->SetPositionType(POS_GROUND);
    n->SetGroundPlane(trace.vColNormal);
  } else
    n->SetPositionType(POS_AIR);
}

tgPhysicNode* tgPhysicEngine::EntityToNode(tgIEntity* ent) const
{
  t_entity_to_node::const_iterator i = mEntityToNode.find(ent);
  if (i == mEntityToNode.end() || i->second == NULL)
    return NULL;
  return i->second;
}

void tgPhysicEngine::HandleTouch (tgPhysicNode* node, const tgCollisionResult& cr) const
{
  if(!cr.pEntity) {
    cerr << "tgPhysicEngine::HandleTouch appelle avec une entite NULL" << endl;
    return;
  }

  tgPhysicNode* tmpNode = EntityToNode(cr.pEntity);
  if (tmpNode == NULL) {
    cerr << "tgPhysicEngine::HandleTouch node introuvable" << endl;
    return;
  } 

  tmpNode->Touch(cr.vColNormal, cr.vEndPoint, node);
  node->Touch(cr.vColNormal, cr.vEndPoint, tmpNode);
}

#if 0
tgPhysicEngine::RK4Derivative tgPhysicEngine::_RK4Evaluate (tgPhysicNode* node, const RK4State& initial, float t)
{
  RK4Derivative output;
  output.dpos = initial.vel;
  output.dvel = _RK4Acceleration(node, initial, t);
  return output;
}

tgPhysicEngine::RK4Derivative tgPhysicEngine::_RK4Evaluate (tgPhysicNode* node, const RK4State& initial, float t, float dt, const RK4Derivative& d)
{
  RK4State state;
  state.pos = initial.pos + d.dpos*dt;
  state.vel = initial.vel + d.dvel*dt;

  RK4Derivative output;
  output.dpos = state.vel;
  output.dvel = _RK4Acceleration(node, state, t+dt);
  return output;
}

tgVector3 tgPhysicEngine::_RK4Acceleration (tgPhysicNode* node, const RK4State& state, float t)
{
  //TODO: change
/*  const float k = 10;
  const float b = 1;
  return -k * state.pos - b * state.vel;*/
  node->UpdateAccel(t);
  return node->GetAccel();
}

void tgPhysicEngine::_RK4Integrate (tgPhysicNode* node, RK4State& state, float t, float dt)
{
  RK4Derivative a = _RK4Evaluate (state, t);
  RK4Derivative b = _RK4Evaluate (state, t, dt*0.5f, a);
  RK4Derivative c = _RK4Evaluate (state, t, dt*0.5f, b);
  RK4Derivative d = _RK4Evaluate (state, t, dt, c);

  const tgVector3 dposdt = 1.0f/6.0f * (a.dpos + 2.0f*(b.dpos + c.dpos) + d.dpos);
  const tgVector3 dveldt = 1.0f/6.0f * (a.dvel + 2.0f*(b.dvel + c.dvel) + d.dvel);

  state.pos += dposdt*dt;
  state.vel += dveldt*dt;
}
#endif
