#ifndef PHYSICS_API_H
#define PHYSICS_API_H

#include "OysterCollision3D.h"
#include "OysterMath.h"
#include "Utilities.h"

namespace Oyster
{
	namespace Physics
	{
		class API;
		class IRigidBody;
		class IParticle;	

		enum UpdateState
		{
			resting,
			altered
		};

		namespace Constant
		{
			const float gravity_constant = (const float)6.67284e-11; // The _big_G_! ( N(m/kg)^2 ) Used in real gravityforcefields.
		}

		class API
		{
		public:
			typedef void (*EventAction_Collision)( unsigned int, unsigned int );
			typedef void (*EventAction_Destruction)( unsigned int, ::Utility::DynamicMemory::UniquePointer<IRigidBody> );

			static API & Instance();

			virtual void SetDeltaTime( float deltaTime ) = 0;
			virtual void SetGravityConstant( float g ) = 0;
			virtual void SetAction( EventAction_Collision functionPointer ) = 0;
			virtual void SetAction( EventAction_Destruction functionPointer ) = 0;

			virtual void Update() = 0;

			virtual bool IsInLimbo( unsigned int objRef ) = 0;
			virtual void MoveToLimbo( unsigned int objRef ) = 0;
			virtual void ReleaseFromLimbo( unsigned int objRef ) = 0;

			virtual unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<IRigidBody> handle ) = 0;
			virtual ::Utility::DynamicMemory::UniquePointer<IRigidBody> ExtractObject( unsigned int objRef ) = 0;
			virtual void DestroyObject( unsigned int objRef ) = 0;
		};
	
		class IRigidBody
		{
		public:
			virtual ~IRigidBody() {};

			virtual UpdateState Update( ::Oyster::Math::Float timeStepLength ) = 0;
		
			virtual bool IsSubscribingCollisions() const = 0;
			virtual bool IsIntersecting( const IRigidBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) = 0;

			virtual unsigned int GetReference() const = 0;
			virtual ::Oyster::Collision3D::Sphere & GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const = 0;
			virtual ::Oyster::Math::Float3 & GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
		};

		class IParticle
		{
		public:

		};
	}

	namespace Collision
	{}
}
#endif