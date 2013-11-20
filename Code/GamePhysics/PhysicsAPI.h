#ifndef PHYSICS_API_H
#define PHYSICS_API_H

#include "OysterMath.h"
#include "Utilities.h"

namespace Oyster
{
	namespace Physics
	{
		class API;
		class IRigidBody;
		class IParticle;	

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

			virtual void MoveToLimbo( unsigned int objRef );
			virtual void ReleaseFromLimbo( unsigned int objRef );

			virtual unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<IRigidBody> handle );
			virtual void DestroyObject( unsigned int objRef );
		};
	
		class IRigidBody
		{
		public:

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