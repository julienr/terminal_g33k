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

#ifndef TGPHYSIC_ENGINE_H
#define TGPHYSIC_ENGINE_H

#include "tgdefs.h"
#include "tgphysic_node.h"
#include "tgforce.h"

#define OVERCLIP 1.01f
#define STEPSIZE 15

/**
 * Physic engine
 *
 */
class tgPhysicEngine
{
	public:
		tgPhysicEngine();
		~tgPhysicEngine();
		
		void AddNode(tgPhysicNode*);
		void RemoveNode(tgPhysicNode*);

		void MoveNodes(float);

    /** 
    * s'occupent d'appeler les callbacks des DEUX nodes touchés
    */
    void HandleTouch(tgPhysicNode* node, const tgCollisionResult& cr) const;

    tgPhysicNode* EntityToNode(tgIEntity* ent) const;


  protected:
		bool _PushNode(tgPhysicNode*, tgVector3&, tgVector3&, float);
		void _ClipVelocity(tgVector3& out, const tgVector3& in, const tgVector3& normal, float overbounce = OVERCLIP) const;
    void _MoveNode(tgPhysicNode* node, float eTime);
    void _DoStepSlideMove(tgPhysicNode* node, float eTime, tgVector3& vel, tgVector3& pos);
		void _ClassifyPosition(tgPhysicNode* n);

    #if 0
    //RK4 integrator, see
    //http://www.gaffer.org/game-physics/integration-basics
    struct RK4Derivative {
      tgVector3 dpos; //derivative of position : velocity
      tgVector3 dvel; //derivative of velocity : accel
    };

    struct RK4State {
      tgVector3 pos;
      tgVector3 vel;
    };
    RK4Derivative _RK4Evaluate (tgPhysicNode* node, const RK4State& initial, float t, float dt, const RK4Derivative& d);
    RK4Derivative _RK4Evaluate (tgPhysicNode* node, const RK4State& initial, float t);
    tgVector3 _RK4Acceleration (tgPhysicNode* node, const RK4State& state, float t);
    void _RK4Integrate (tgPhysicNode* node, RK4State& state, float t, float dt);
    #endif

		typedef tgset<tgPhysicNode*> t_nodes;
		t_nodes tNodes;

    typedef tgmap<tgIEntity*, tgPhysicNode*> t_entity_to_node;
    t_entity_to_node mEntityToNode;
	  
    tgForce* pGravity;
};


#endif
