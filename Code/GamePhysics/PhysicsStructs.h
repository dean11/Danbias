#ifndef PHYSICS_STRUCTS_H
#define PHYSICS_STRUCTS_H

#include "OysterMath.h"
#include "PhysicsAPI.h"

namespace Oyster { namespace Physics
{
	namespace Struct
	{
		struct SimpleBodyDescription
		{
			::Oyster::Math::Float4x4 rotation;
			::Oyster::Math::Float3 centerPosition;
			::Oyster::Math::Float3 size;
			::Oyster::Math::Float mass;
			::Oyster::Math::Float4x4 inertiaTensor;
			::Oyster::Physics::ICustomBody::EventAction_Collision subscription;
			bool ignoreGravity;

			SimpleBodyDescription();
		};

		struct SphericalBodyDescription
		{
			::Oyster::Math::Float4x4 rotation;
			::Oyster::Math::Float3 centerPosition;
			::Oyster::Math::Float radius;
			::Oyster::Math::Float mass;
			::Oyster::Physics::ICustomBody::EventAction_Collision subscription;
			bool ignoreGravity;

			SphericalBodyDescription();
		};

		struct CustomBodyState
		{
		public:
			CustomBodyState( const ::Oyster::Math::Float3 &reach			= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &centerPos		= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &rotation			= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &linearMomentum	= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &angularMomentum	= ::Oyster::Math::Float3::null );

			CustomBodyState & operator = ( const CustomBodyState &state );

			const ::Oyster::Math::Float4 & GetReach() const;
				  ::Oyster::Math::Float4   GetSize() const;
			const ::Oyster::Math::Float4 & GetCenterPosition() const;
			const ::Oyster::Math::Float4 & GetAngularAxis() const;
				  ::Oyster::Math::Float4x4 GetRotation() const;
			const ::Oyster::Math::Float    GetMass() const;
			const ::Oyster::Math::Float    GetRestitutionCoeff() const;
			const ::Oyster::Math::Float4 & GetLinearMomentum() const;
				  ::Oyster::Math::Float4   GetLinearMomentum( const ::Oyster::Math::Float3 &at ) const;
				  ::Oyster::Math::Float4   GetLinearMomentum( const ::Oyster::Math::Float4 &at ) const;
			const ::Oyster::Math::Float4 & GetAngularMomentum() const;
			

			void SetSize( const ::Oyster::Math::Float3 &size );
			void SetReach( const ::Oyster::Math::Float3 &halfSize );
			void SetCenterPosition( const ::Oyster::Math::Float3 &centerPos );
			void SetRotation( const ::Oyster::Math::Float3 &angularAxis );
			void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
			void SetMass( ::Oyster::Math::Float m );
			void SetRestitutionCoeff( ::Oyster::Math::Float e );
			void SetLinearMomentum( const ::Oyster::Math::Float3 &g );
			void SetAngularMomentum( const ::Oyster::Math::Float3 &h );

			void ApplyImpulse( const ::Oyster::Math::Float4 &j );
			void ApplyImpulse( const ::Oyster::Math::Float4 &j, const ::Oyster::Math::Float4 &at, const ::Oyster::Math::Float4 &normal );

			bool IsSpatiallyAltered() const;
			bool IsDisturbed() const;

		private:
			::Oyster::Math::Float4 reach, centerPos, angularAxis;
			::Oyster::Math::Float4 linearMomentum, angularMomentum;

			bool isSpatiallyAltered, isDisturbed;
		};
	}
} }

#include "PhysicsStructs-Impl.h"

#endif