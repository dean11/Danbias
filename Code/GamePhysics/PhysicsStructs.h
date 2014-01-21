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
			::Oyster::Math::Float4 centerPosition;
			::Oyster::Math::Float4 size;
			::Oyster::Math::Float mass;
			::Oyster::Math::Float restitutionCoeff;
			::Oyster::Math::Float frictionCoeff_Static;
			::Oyster::Math::Float frictionCoeff_Dynamic;
			::Oyster::Math::Float4x4 inertiaTensor;
			::Oyster::Physics::ICustomBody::EventAction_Collision subscription_onCollision;
			::Oyster::Physics::ICustomBody::EventAction_Move subscription_onMovement;
			bool ignoreGravity;

			SimpleBodyDescription();
		};

		struct SphericalBodyDescription
		{
			::Oyster::Math::Float4x4 rotation;
			::Oyster::Math::Float4 centerPosition;
			::Oyster::Math::Float radius;
			::Oyster::Math::Float mass;
			::Oyster::Math::Float restitutionCoeff;
			::Oyster::Math::Float frictionCoeff_Static;
			::Oyster::Math::Float frictionCoeff_Dynamic;
			::Oyster::Physics::ICustomBody::EventAction_Collision subscription_onCollision;
			::Oyster::Physics::ICustomBody::EventAction_Move subscription_onMovement;
			bool ignoreGravity;

			SphericalBodyDescription();
		};

		struct CustomBodyState
		{
		public:
			CustomBodyState( ::Oyster::Math::Float mass						= 1.0f,
							 ::Oyster::Math::Float restitutionCoeff			= 1.0f,
							 ::Oyster::Math::Float staticFrictionCoeff		= 1.0f,
							 ::Oyster::Math::Float kineticFrictionCoeff		= 1.0f,
							 const ::Oyster::Math::Float4x4 &inertiaTensor	= ::Oyster::Math::Float4x4::identity,
							 const ::Oyster::Math::Float4 &reach			= ::Oyster::Math::Float4::null,
							 const ::Oyster::Math::Float4 &centerPos		= ::Oyster::Math::Float4::standard_unit_w,
							 const ::Oyster::Math::Float4 &rotation			= ::Oyster::Math::Float4::null,
							 const ::Oyster::Math::Float4 &linearMomentum	= ::Oyster::Math::Float4::null,
							 const ::Oyster::Math::Float4 &angularMomentum	= ::Oyster::Math::Float4::null );

			CustomBodyState & operator = ( const CustomBodyState &state );

			const ::Oyster::Math::Float		 GetMass() const;
			const ::Oyster::Math::Float		 GetRestitutionCoeff() const;
			const ::Oyster::Math::Float		 GetFrictionCoeff_Static() const;
			const ::Oyster::Math::Float		 GetFrictionCoeff_Kinetic() const;
			const ::Oyster::Math::Float4x4 & GetMomentOfInertia() const;
			const ::Oyster::Math::Float4 &	 GetReach() const;
				  ::Oyster::Math::Float4	 GetSize() const;
			const ::Oyster::Math::Float4 &	 GetCenterPosition() const;
			const ::Oyster::Math::Float4 &	 GetAngularAxis() const;
				  ::Oyster::Math::Float4x4	 GetRotation() const;
				  ::Oyster::Math::Float4x4	 GetOrientation() const;
				  ::Oyster::Math::Float4x4	 GetOrientation( const ::Oyster::Math::Float4 &offset ) const;
				  ::Oyster::Math::Float4x4	 GetView() const;
				  ::Oyster::Math::Float4x4	 GetView( const ::Oyster::Math::Float4 &offset ) const;
			const ::Oyster::Math::Float4 &	 GetLinearMomentum() const;
				  ::Oyster::Math::Float4	 GetLinearMomentum( const ::Oyster::Math::Float4 &at ) const;
			const ::Oyster::Math::Float4 &	 GetAngularMomentum() const;
			const ::Oyster::Math::Float4 &	 GetLinearImpulse() const;
			const ::Oyster::Math::Float4 &	 GetAngularImpulse() const;
			const ::Oyster::Math::Float4 &	 GetForward_DeltaPos() const;
			const ::Oyster::Math::Float4 &	 GetForward_DeltaAxis() const;
			
			void SetMass_KeepMomentum( ::Oyster::Math::Float m );
			void SetMass_KeepVelocity( ::Oyster::Math::Float m );
			void SetRestitutionCoeff( ::Oyster::Math::Float e );
			void SetFrictionCoeff( ::Oyster::Math::Float staticU, ::Oyster::Math::Float kineticU );
			void SetMomentOfInertia_KeepMomentum( const ::Oyster::Math::Float4x4 &tensor );
			void SetMomentOfInertia_KeepVelocity( const ::Oyster::Math::Float4x4 &tensor );
			void SetSize( const ::Oyster::Math::Float4 &size );
			void SetReach( const ::Oyster::Math::Float4 &halfSize );
			void SetCenterPosition( const ::Oyster::Math::Float4 &centerPos );
			void SetRotation( const ::Oyster::Math::Float4 &angularAxis );
			void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
			void SetOrientation( const ::Oyster::Math::Float4x4 &orientation );
			void SetLinearMomentum( const ::Oyster::Math::Float4 &g );
			void SetAngularMomentum( const ::Oyster::Math::Float4 &h );
			void SetLinearImpulse( const ::Oyster::Math::Float4 &j );
			void SetAngularImpulse( const ::Oyster::Math::Float4 &j );

			void AddRotation( const ::Oyster::Math::Float4 &angularAxis );
			void AddTranslation( const ::Oyster::Math::Float4 &deltaPos );

			void ApplyLinearImpulse( const ::Oyster::Math::Float4 &j );
			void ApplyAngularImpulse( const ::Oyster::Math::Float4 &j );
			void ApplyImpulse( const ::Oyster::Math::Float4 &j, const ::Oyster::Math::Float4 &at, const ::Oyster::Math::Float4 &normal );
			void ApplyForwarding( const ::Oyster::Math::Float4 &deltaPos, const ::Oyster::Math::Float4 &deltaAxis );

			bool IsSpatiallyAltered() const;
			bool IsDisturbed() const;
			bool IsForwarded() const;

		private:
			::Oyster::Math::Float mass, restitutionCoeff, staticFrictionCoeff, kineticFrictionCoeff;
			::Oyster::Math::Float4x4 inertiaTensor;
			::Oyster::Math::Float4 reach, centerPos, angularAxis;
			::Oyster::Math::Float4 linearMomentum, angularMomentum;
			::Oyster::Math::Float4 linearImpulse, angularImpulse;
			::Oyster::Math::Float4 deltaPos, deltaAxis; // Forwarding data sum

			bool isSpatiallyAltered, isDisturbed, isForwarded;
		};

		/**
		###############################################################################
		Can't define structs inside structs in a union therefor they are declared here.
		###############################################################################
		*/
		struct GravityWell
		{
			::Oyster::Math::Float3 position;
			::Oyster::Math::Float mass;

			GravityWell( );
			GravityWell( const GravityWell &gravityWell );
			GravityWell & operator = ( const GravityWell &gravityWell );

			bool operator == ( const GravityWell &gravity ) const;
			bool operator != ( const GravityWell &gravity ) const;
		};

		struct GravityDirected
		{
			::Oyster::Math::Float3 impulse;

			GravityDirected( );
			GravityDirected( const GravityDirected &gravityDirected );
			GravityDirected & operator = ( const GravityDirected &gravityDirected );

			bool operator == ( const GravityDirected &gravity ) const;
			bool operator != ( const GravityDirected &gravity ) const;
		};

		struct GravityDirectedField
		{
			::Oyster::Math::Float3 normalizedDirection;
			::Oyster::Math::Float mass;
			::Oyster::Math::Float magnitude;

			GravityDirectedField( );
			GravityDirectedField( const GravityDirectedField &gravityDirectedField );
			GravityDirectedField & operator = ( const GravityDirectedField &gravityDirectedField );

			bool operator == ( const GravityDirectedField &gravity ) const;
			bool operator != ( const GravityDirectedField &gravity ) const;
		};

		struct Gravity
		{
			enum GravityType
			{
				GravityType_Undefined		= -1,
				GravityType_Well			= 0,
				GravityType_Directed		= 1,
				GravityType_DirectedField	= 2,
			} gravityType;

			union
			{
				struct
				{
					GravityWell well;
				};

				struct
				{
					GravityDirected directed;
				};

				struct
				{
					GravityDirectedField directedField;
				};
			};

			Gravity( );
			Gravity( const Gravity &gravity );
			Gravity & operator = ( const Gravity &gravity );

			bool operator == ( const Gravity &gravity ) const;
			bool operator != ( const Gravity &gravity ) const;
		};
	}
} }


#include "PhysicsStructs-Impl.h"

#endif