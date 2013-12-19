#ifndef PHYSICS_STRUCTS_IMPL_H
#define PHYSICS_STRUCTS_IMPL_H

#include "PhysicsStructs.h"

namespace Oyster 
{ 
	namespace Physics
	{
		namespace Struct
		{
			inline SimpleBodyDescription::SimpleBodyDescription()
			{
				this->rotation = ::Oyster::Math::Float4x4::identity;
				this->centerPosition = ::Oyster::Math::Float3::null;
				this->size = ::Oyster::Math::Float3( 1.0f );
				this->mass = 12.0f;
				this->inertiaTensor = ::Oyster::Math::Float4x4::identity;
				this->subscription = NULL;
				this->ignoreGravity = false;
			}

			inline SphericalBodyDescription::SphericalBodyDescription()
			{
				this->rotation = ::Oyster::Math::Float4x4::identity;
				this->centerPosition = ::Oyster::Math::Float3::null;
				this->radius = 0.5f;
				this->mass = 10.0f;
				this->subscription = NULL;
				this->ignoreGravity = false;
			}


			inline CustomBodyState::CustomBodyState( const ::Oyster::Math::Float3 &reach, const ::Oyster::Math::Float3 &centerPos, const ::Oyster::Math::Float3 &rotation )
			{
				this->reach = ::Oyster::Math::Float4( reach, 0.0f );
				this->centerPos = ::Oyster::Math::Float4( centerPos, 1.0f );
				this->angularAxis = ::Oyster::Math::Float4( rotation, 0.0f );
				this->isSpatiallyAltered = this->isDisturbed = false;
			}

			inline CustomBodyState & CustomBodyState::operator = ( const CustomBodyState &state )
			{
				this->reach = state.reach;
				this->centerPos = state.centerPos;
				this->angularAxis = state.angularAxis;

				this->isSpatiallyAltered = state.isSpatiallyAltered;
				this->isDisturbed = state.isDisturbed;
				return *this;
			}

			inline const ::Oyster::Math::Float4 & CustomBodyState::GetReach() const
			{
				return this->reach;
			}

			inline ::Oyster::Math::Float4 CustomBodyState::GetSize() const
			{
				return 2.0f * this->GetReach();
			}

			inline const ::Oyster::Math::Float4 & CustomBodyState::GetCenterPosition() const
			{
				return this->centerPos;
			}

			inline const ::Oyster::Math::Float4 & CustomBodyState::GetAngularAxis() const
			{
				return this->angularAxis;
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetRotation() const
			{
				return ::Oyster::Math3D::RotationMatrix( this->GetAngularAxis().xyz );
			}

			inline void CustomBodyState::SetSize( const ::Oyster::Math::Float3 &size )
			{
				this->SetReach( 0.5f * size );
			}

			inline void CustomBodyState::SetReach( const ::Oyster::Math::Float3 &halfSize )
			{
				this->reach.xyz = halfSize;
				this->reach = ::Utility::Value::Max( this->reach, ::Oyster::Math::Float4::null );
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetCenterPosition( const ::Oyster::Math::Float3 &centerPos )
			{
				this->centerPos.xyz = centerPos;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetRotation( const ::Oyster::Math::Float3 &angularAxis )
			{
				this->angularAxis.xyz = angularAxis;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetRotation( const ::Oyster::Math::Float4x4 &rotation )
			{
				this->SetRotation( ::Oyster::Math3D::AngularAxis(rotation).xyz );
			}

			inline bool CustomBodyState::IsSpatiallyAltered() const
			{
				return this->isSpatiallyAltered;
			}

			inline bool CustomBodyState::IsDisturbed() const
			{
				return this->isDisturbed;
			}

			inline GravityWell::GravityWell( )
			{
				this->position	= ::Oyster::Math::Float3::null;
				this->mass		= 0.0f;
			}
			
			inline GravityWell::GravityWell( const GravityWell& gravityWell )
			{
				this->position	= gravityWell.position;
				this->mass		= gravityWell.mass;
			}

			GravityWell& GravityWell::operator=( const GravityWell& gravityWell )
			{
				this->position	= gravityWell.position;
				this->mass		= gravityWell.mass;

				return *this;
			}

			inline GravityDirected::GravityDirected( )
			{
				this->impulse = ::Oyster::Math::Float3::null;
			}

			inline GravityDirected::GravityDirected( const GravityDirected& gravityDirected )
			{
				this->impulse = gravityDirected.impulse;
			}

			inline GravityDirected& GravityDirected::operator=( const GravityDirected& gravityDirected )
			{
				this->impulse = gravityDirected.impulse;

				return *this;
			}

			inline GravityDirectedField::GravityDirectedField( )
			{
				this->normalizedDirection	= ::Oyster::Math::Float3::null;
				this->mass					= 0.0f;
				this->magnitude				= 0.0f;
			}

			inline GravityDirectedField::GravityDirectedField( const GravityDirectedField& gravityDirectedField )
			{
				this->normalizedDirection	= gravityDirectedField.normalizedDirection;
				this->mass					= gravityDirectedField.mass;
				this->magnitude				= gravityDirectedField.magnitude;
			}
			inline GravityDirectedField& GravityDirectedField::operator=( const GravityDirectedField& gravityDirectedField )
			{
				this->normalizedDirection	= gravityDirectedField.normalizedDirection;
				this->mass					= gravityDirectedField.mass;
				this->magnitude				= gravityDirectedField.magnitude;

				return *this;
			}

			inline Gravity::Gravity()
			{
				this->gravityType = GravityType_Undefined;
			}

			inline Gravity::Gravity( const Gravity& gravity )
			{
				switch(gravity.gravityType)
				{
				case GravityType_Well:
					this->well.position = gravity.well.position;
					this->well.mass		 = gravity.well.mass;
					break;
				case GravityType_Directed:
					this->directed.impulse = gravity.directed.impulse;
					break;
				case GravityType_DirectedField:
					this->directedField.normalizedDirection = gravity.directedField.normalizedDirection;
					this->directedField.magnitude			= gravity.directedField.magnitude;
					this->directedField.mass					= gravity.directedField.mass;
					break;
				}
			}
		
			inline Gravity& Gravity::operator=( const Gravity& gravity )
			{
				switch(gravity.gravityType)
				{
				case GravityType_Well:
					this->well.position	= gravity.well.position;
					this->well.mass		= gravity.well.mass;
					break;
				case GravityType_Directed:
					this->directed.impulse = gravity.directed.impulse;
					break;
				case GravityType_DirectedField:
					this->directedField.normalizedDirection = gravity.directedField.normalizedDirection;
					this->directedField.magnitude			= gravity.directedField.magnitude;
					this->directedField.mass					= gravity.directedField.mass;
					break;
				}

				return *this;
			}
		}
	} 
}

#endif