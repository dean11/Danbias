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

			void ApplyImpulse(Math::Float3 impulse);
			
			void SetSubscription(EventAction_AfterCollisionResponse function);
			void SetSubscription(EventAction_Move function);

			void SetLinearVelocity(Math::Float3 velocity);
			void SetPosition(Math::Float3 position);
			void SetRotation(Math::Float4 quaternion);
			void SetRotation(Math::Quaternion quaternion);
			void SetRotation(Math::Float3 eulerAngles);
			void SetRotation(::Oyster::Math::Float4x4 rotation);
			void SetRotationAsAngularAxis(Math::Float4 angularAxis);
			void SetAngularFactor(Math::Float factor);
			void SetMass(Math::Float mass);

			void SetGravity(Math::Float3 gravity);

			void SetUpAndRight(Math::Float3 up, Math::Float3 right);
			void SetUpAndForward(Math::Float3 up, Math::Float3 forward);
			void SetUp(Math::Float3 up);

			Math::Float4x4 GetRotation() const;
			Math::Float4 GetRotationAsAngularAxis();
			Math::Float4x4 GetOrientation() const;
			Math::Float4x4 GetView() const;
			Math::Float4x4 GetView( const Math::Float3 &offset ) const;

			Math::Float3 GetGravity() const;
			::Oyster::Math::Float3 GetLinearVelocity() const;

			void CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB, Math::Float kineticEnergyLoss);
			void CallSubscription_Move();

			btCollisionShape* GetCollisionShape() const;
			btDefaultMotionState* GetMotionState() const;
			btRigidBody* GetRigidBody() const;

			void SetCustomTag( void *ref );
			void* GetCustomTag() const;

			// Class specific
			void SetCollisionShape(btCollisionShape* shape);
			void SetMotionState(btDefaultMotionState* motionState);
			void SetRigidBody(btRigidBody* rigidBody);

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