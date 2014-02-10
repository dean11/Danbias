#ifndef OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H
#define OYSTER_PHYSICS_SIMPLE_RIGIDBODY_H

#include "..\PhysicsAPI.h"
#include <btBulletDynamicsCommon.h>

namespace Oyster 
{ 
	namespace Physics
	{
		class SimpleRigidBody : public ICustomBody
		{
		public:
			SimpleRigidBody();
			virtual ~SimpleRigidBody();

			void SetCollisionShape(btCollisionShape* shape);
			void SetMotionState(btDefaultMotionState* motionState);
			void SetRigidBody(btRigidBody* rigidBody);
			
			void SetSubscription(EventAction_AfterCollisionResponse function);
			void SetSubscription(EventAction_Move function);

			void SetLinearVelocity(Math::Float3 velocity);
			void SetRotation(Math::Float4 quaternion);
			void SetRotation(Math::Float3 eulerAngles);

			void CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB, Math::Float kineticEnergyLoss);
			void CallSubscription_Move();

			State GetState() const;
			State & GetState( State &targetMem ) const;
			void SetState( const State &state );

			btCollisionShape* GetCollisionShape() const;
			btDefaultMotionState* GetMotionState() const;
			btRigidBody* GetRigidBody() const;

			void SetCustomTag( void *ref );
			void* GetCustomTag() const;

			private:
			btCollisionShape* collisionShape;
			btDefaultMotionState* motionState;
			btRigidBody* rigidBody;

			Struct::CustomBodyState state;

			EventAction_AfterCollisionResponse afterCollision;
			EventAction_Move onMovement;

			void *customTag;
		};
	} 
}

#endif