#ifndef PHYSICS_API_IMPL_H
#define PHYSICS_API_IMPL_H

#include "../PhysicsAPI.h"

namespace Oyster
{
	namespace Physics
	{
		class API_Impl : public API
		{
		public:
			API_Impl();
			virtual ~API_Impl();

			void SetDeltaTime( float deltaTime );
			void SetGravityConstant( float g );
			void SetAction( EventAction_Collision functionPointer );
			void SetAction( EventAction_Destruction functionPointer );

			void Update();

			bool IsInLimbo( unsigned int objRef );
			void MoveToLimbo( unsigned int objRef );
			void ReleaseFromLimbo( unsigned int objRef );

			unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle );
			::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( unsigned int objRef );
			void DestroyObject( unsigned int objRef );

			const ICustomBody & Peek( unsigned int objRef ) const;

			void ApplyForceAt( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF );
			void ApplyCollisionResponse( unsigned int objRefA, unsigned int objRefB, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact );

			void SetMomentOfInertiaTensor_KeepVelocity( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI );
			void SetMomentOfInertiaTensor_KeepMomentum( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI );
			void SetMass_KeepVelocity( unsigned int objRef, ::Oyster::Math::Float m );
			void SetMass_KeepMomentum( unsigned int objRef, ::Oyster::Math::Float m );
			void SetCenter( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos );
			void SetRotation( unsigned int objRef, const ::Oyster::Math::Float4x4 &rotation );
			void SetOrientation( unsigned int objRef, const ::Oyster::Math::Float4x4 &orientation );

			::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateSimpleRigidBody() const;
		};
	
	}

}

#endif