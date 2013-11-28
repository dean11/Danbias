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

			void Init( unsigned int numObjects, unsigned int numGravityWells , const ::Oyster::Math::Float3 &worldSize );

			void SetDeltaTime( float deltaTime );
			void SetGravityConstant( float g );
			void SetSubscription( EventAction_Destruction functionPointer );

			void Update();

			bool IsInLimbo( const ICustomBody* objRef );
			void MoveToLimbo( const ICustomBody* objRef );
			void ReleaseFromLimbo( const ICustomBody* objRef );

			void AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle );
			::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( const ICustomBody* objRef );
			void DestroyObject( const ICustomBody* objRef );

			void ApplyForceAt( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF );
			void ApplyCollisionResponse( const ICustomBody* objRefA, const ICustomBody* objRefB, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact );

			void SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI );
			void SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI );
			void SetMass_KeepVelocity( const ICustomBody* objRef, ::Oyster::Math::Float m );
			void SetMass_KeepMomentum( const ICustomBody* objRef, ::Oyster::Math::Float m );
			void SetCenter( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos );
			void SetRotation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &rotation );
			void SetOrientation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &orientation );
			void SetSize( const ICustomBody* objRef, const ::Oyster::Math::Float3 &size );

			::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SimpleBodyDescription &desc ) const;
			::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SphericalBodyDescription &desc ) const;

		private:
			::Oyster::Math::Float gravityConstant, updateFrameLength;
			EventAction_Destruction destructionAction;
		};

		namespace Default
		{
			void EventAction_Destruction( ::Utility::DynamicMemory::UniquePointer<::Oyster::Physics::ICustomBody> proto );
			::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_Collision( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter );
		}
	}
}

#endif