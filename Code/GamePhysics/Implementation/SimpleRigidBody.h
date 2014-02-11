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

			State GetState() const;
			State& GetState( State &targetMem ) const;
			void SetState( const State &state );

			void SetCollisionShape(btCollisionShape* shape);
			void SetMotionState(btDefaultMotionState* motionState);
			void SetRigidBody(btRigidBody* rigidBody);
			
			void SetSubscription(EventAction_AfterCollisionResponse function);
			void SetSubscription(EventAction_Move function);

			void SetLinearVelocity(Math::Float3 velocity);
			void SetPosition(Math::Float3 position);
			void SetRotation(Math::Float4 quaternion);
			void SetRotation(Math::Quaternion quaternion);
			void SetRotation(Math::Float3 eulerAngles);
			void SetAngularFactor(Math::Float factor);

			void SetGravity(Math::Float3 gravity);

			void SetUpAndRight(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 right);
			void SetUpAndForward(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 forward);

			Math::Float4x4 GetRotation() const;
			Math::Float4 GetRotationAsAngularAxis();
			Math::Float4x4 GetOrientation() const;
			Math::Float4x4 GetView() const;
			Math::Float4x4 GetView( const ::Oyster::Math::Float3 &offset ) const;

			void CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB, Math::Float kineticEnergyLoss);
			void CallSubscription_Move();

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

			::Oyster::Math::Float3 gravity;
		};
	} 
}

#endif