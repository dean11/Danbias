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
			CustomBodyState( const ::Oyster::Math::Float3 &reach		= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &centerPos	= ::Oyster::Math::Float3::null,
							 const ::Oyster::Math::Float3 &rotation		= ::Oyster::Math::Float3::null );

			CustomBodyState & operator = ( const CustomBodyState &state );

			const ::Oyster::Math::Float4 & GetReach() const;
				  ::Oyster::Math::Float4   GetSize() const;
			const ::Oyster::Math::Float4 & GetCenterPosition() const;
			const ::Oyster::Math::Float4 & GetAngularAxis() const;
				  ::Oyster::Math::Float4x4 GetRotation() const;

			void SetSize( const ::Oyster::Math::Float3 &size );
			void SetReach( const ::Oyster::Math::Float3 &halfSize );
			void SetCenterPosition( const ::Oyster::Math::Float3 &centerPos );
			void SetRotation( const ::Oyster::Math::Float3 &angularAxis );
			void SetRotation( const ::Oyster::Math::Float4x4 &rotation );

			bool IsSpatiallyAltered() const;
			bool IsDisturbed() const;

		private:
			::Oyster::Math::Float4 reach, centerPos, angularAxis;

			bool isSpatiallyAltered, isDisturbed;
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
			GravityWell( const GravityWell& gravityWell );
			GravityWell& operator=( const GravityWell& gravityWell );
		};

		struct GravityDirected
		{
			::Oyster::Math::Float3 impulse;

			GravityDirected( );
			GravityDirected( const GravityDirected& gravityDirected );
			GravityDirected& operator=( const GravityDirected& gravityDirected );
		};

		struct GravityDirectedField
		{
			::Oyster::Math::Float3 normalizedDirection;
			::Oyster::Math::Float mass;
			::Oyster::Math::Float magnitude;

			GravityDirectedField( );
			GravityDirectedField( const GravityDirectedField& gravityDirectedField );
			GravityDirectedField& operator=( const GravityDirectedField& gravityDirectedField );
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
			Gravity( const Gravity& gravity );
			Gravity& operator=( const Gravity& gravity );
			
		};
	}
} }


#include "PhysicsStructs-Impl.h"

#endif