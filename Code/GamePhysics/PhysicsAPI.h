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
		class ICustomBody;

		enum UpdateState
		{
			resting,
			altered
		};

		namespace Constant
		{
			const float gravity_constant = (const float)6.67284e-11; // The _big_G_! ( N(m/kg)^2 ) Used in real gravityforcefields.
		}


		class MomentOfInertia
		{
		public:
			static ::Oyster::Math::Float4x4 & CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);
				
			static ::Oyster::Math::Float4x4 & CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);

			static ::Oyster::Math::Float4x4 & CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );

			static ::Oyster::Math::Float4x4 & CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
	
			static ::Oyster::Math::Float4x4 & CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
		};


		class API
		{
		public:
			typedef void (*EventAction_Collision)( unsigned int, unsigned int );
			typedef void (*EventAction_Destruction)( unsigned int, ::Utility::DynamicMemory::UniquePointer<ICustomBody> );

			static API & Instance();

			virtual void SetDeltaTime( float deltaTime ) = 0;
			virtual void SetGravityConstant( float g ) = 0;
			virtual void SetAction( EventAction_Collision functionPointer ) = 0;
			virtual void SetAction( EventAction_Destruction functionPointer ) = 0;

			virtual void Update() = 0;

			virtual bool IsInLimbo( unsigned int objRef ) = 0;
			virtual void MoveToLimbo( unsigned int objRef ) = 0;
			virtual void ReleaseFromLimbo( unsigned int objRef ) = 0;

			virtual unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle ) = 0;
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( unsigned int objRef ) = 0;
			virtual void DestroyObject( unsigned int objRef ) = 0;

			virtual const ICustomBody & Peek( unsigned int objRef ) const = 0;

			virtual void ApplyForceAt( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF ) = 0;
			virtual void ApplyCollisionResponse( unsigned int objRefA, unsigned int objRefB, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) = 0;

			virtual void SetMomentOfInertiaTensor_KeepVelocity( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMomentOfInertiaTensor_KeepMomentum( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMass_KeepVelocity( unsigned int objRef, ::Oyster::Math::Float m ) = 0;
			virtual void SetMass_KeepMomentum( unsigned int objRef, ::Oyster::Math::Float m ) = 0;
			virtual void SetCenter( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos ) = 0;
			virtual void SetRotation( unsigned int objRef, const ::Oyster::Math::Float4x4 &rotation ) = 0;
			virtual void SetOrientation( unsigned int objRef, const ::Oyster::Math::Float4x4 &orientation ) = 0;

			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateSimpleRigidBody() const = 0;
		
		protected:
			virtual ~API() {}
		};
	
		class ICustomBody
		{
		public:
			virtual ~ICustomBody() {};

			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const = 0;
			
			virtual bool IsSubscribingCollisions() const = 0;
			virtual bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const = 0;
			virtual bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const = 0;

			virtual unsigned int					GetReference() const = 0;
			virtual ::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const = 0;
			virtual ::Oyster::Math::Float3 &		GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			virtual ::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;

			virtual UpdateState Update( ::Oyster::Math::Float timeStepLength ) = 0;

			virtual void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMass_KeepVelocity( ::Oyster::Math::Float m ) = 0;
			virtual void SetMass_KeepMomentum( ::Oyster::Math::Float m ) = 0;
			virtual void SetCenter( const ::Oyster::Math::Float3 &worldPos ) = 0;
			virtual void SetRotation( const ::Oyster::Math::Float4x4 &rotation ) = 0;
			virtual void SetOrientation( const ::Oyster::Math::Float4x4 &orientation ) = 0;
		};

		namespace Error
		{
			const unsigned int not_a_reference = ::Utility::Value::numeric_limits<unsigned int>::max();

			class NullBody : public ICustomBody
			{
			public:
				virtual ~NullBody();

				::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const;
			
				bool IsSubscribingCollisions() const;
				bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const;
				bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const;

				unsigned int					GetReference() const;
				::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const;
				::Oyster::Math::Float3 &		GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
				::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
				::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
				::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
				::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

				UpdateState Update( ::Oyster::Math::Float timeStepLength );

				void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI );
				void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI );
				void SetMass_KeepVelocity( ::Oyster::Math::Float m );
				void SetMass_KeepMomentum( ::Oyster::Math::Float m );
				void SetCenter( const ::Oyster::Math::Float3 &worldPos );
				void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
				void SetOrientation( const ::Oyster::Math::Float4x4 &orientation );

			} const nobody;
		}
	}
}
#endif