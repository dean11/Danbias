#ifndef PHYSICS_API_IMPL_H
#define PHYSICS_API_IMPL_H

#include "../PhysicsAPI.h"
#include "Octree.h"

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

			void SetFrameTimeLength( float deltaTime );
			void SetGravityConstant( float g );
			void SetSubscription( EventAction_Destruction functionPointer );

			float GetFrameTimeLength() const;

			void Update();

			bool IsInLimbo( const ICustomBody* objRef );
			void MoveToLimbo( const ICustomBody* objRef );
			void ReleaseFromLimbo( const ICustomBody* objRef );

			void AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle );
			::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( const ICustomBody* objRef );
			void DestroyObject( const ICustomBody* objRef );

			void AddGravity( const API::Gravity &g );
			void RemoveGravity( const API::Gravity &g );

			void ApplyEffect( const Oyster::Collision3D::ICollideable& collideable, void* args, void(hitAction)(ICustomBody*, void*) );

			//void ApplyForceAt( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF );

			//void SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI );
			//void SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI );
			//void SetMass_KeepVelocity( const ICustomBody* objRef, ::Oyster::Math::Float m );
			//void SetMass_KeepMomentum( const ICustomBody* objRef, ::Oyster::Math::Float m );
			//void SetCenter( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos );
			//void SetRotation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &rotation );
			//void SetOrientation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &orientation );
			//void SetSize( const ICustomBody* objRef, const ::Oyster::Math::Float3 &size );

			::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SimpleBodyDescription &desc ) const;
			::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SphericalBodyDescription &desc ) const;

		private:
			::Oyster::Math::Float gravityConstant, updateFrameLength;
			EventAction_Destruction destructionAction;
			::std::vector<API::Gravity> gravity;
			Octree worldScene;
		};

		namespace Default
		{
			void EventAction_Destruction( ::Utility::DynamicMemory::UniquePointer<::Oyster::Physics::ICustomBody> proto );
			::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_BeforeCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter );
			void EventAction_AfterCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter, ::Oyster::Math::Float kineticEnergyLoss );
			void EventAction_Move( const ::Oyster::Physics::ICustomBody *object );
		}
	}
}

#endif