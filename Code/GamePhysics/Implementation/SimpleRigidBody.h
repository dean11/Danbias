#ifndef OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H
#define OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H

#include "..\PhysicsAPI.h"
#include <btBulletDynamicsCommon.h>

namespace Oyster { namespace Physics
{
	class SimpleRigidBody : public ICustomBody
	{
	public:
		SimpleRigidBody();
		SimpleRigidBody( const API::SimpleBodyDescription &desc );
		virtual ~SimpleRigidBody();

		void SetCollisionShape(btCollisionShape* shape);
		void SetMotionState(btDefaultMotionState* motionState);
		void SetRigidBody(btRigidBody* rigidBody);
		
		void SetSubscription(EventAction_AfterCollisionResponse function);
		void CallSubsciptMessage(ICustomBody* bodyA, ICustomBody* bodyB, Math::Float kineticEnergyLoss);

		State GetState() const;
		State & GetState( State &targetMem ) const;
		void SetState( const State &state );

		btDefaultMotionState* GetMotionState() const;

		void SetCustomTag( void *ref );
		void* GetCustomTag() const;

	private:
		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;

		Struct::CustomBodyState state;

		EventAction_AfterCollisionResponse afterCollision;

		void *customTag;
	};
} }

#endif