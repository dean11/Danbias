#ifndef PHYSICS_STRUCTS_IMPL_H
#define PHYSICS_STRUCTS_IMPL_H

#include "PhysicsStructs.h"

namespace Oyster { namespace Physics
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

		inline CustomBodyState::CustomBodyState( const ::Oyster::Math::Float3 &reach, const ::Oyster::Math::Float3 &centerPos, const ::Oyster::Math::Float3 &rotation, const ::Oyster::Math::Float3 &linearMomentum, const ::Oyster::Math::Float3 &angularMomentum )
		{
			this->reach = ::Oyster::Math::Float4( reach, 0.0f );
			this->centerPos = ::Oyster::Math::Float4( centerPos, 1.0f );
			this->angularAxis = ::Oyster::Math::Float4( rotation, 0.0f );
			this->linearMomentum = ::Oyster::Math::Float4( linearMomentum, 0.0f );
			this->angularMomentum = ::Oyster::Math::Float4( angularMomentum, 0.0f );
			this->isSpatiallyAltered = this->isDisturbed = false;
		}

		inline CustomBodyState & CustomBodyState::operator = ( const CustomBodyState &state )
		{
			this->reach = state.reach;
			this->centerPos = state.centerPos;
			this->angularAxis = state.angularAxis;
			this->linearMomentum = state.linearMomentum;
			this->angularMomentum = state.angularMomentum;

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

		inline const ::Oyster::Math::Float CustomBodyState::GetMass() const
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::GetMass)
			return 1.0f;
		}

		inline const ::Oyster::Math::Float CustomBodyState::GetRestitutionCoeff() const
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::GetRestitutionCoeff)
			return 1.0f;
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetLinearMomentum() const
		{
			return this->linearMomentum;
		}

		inline ::Oyster::Math::Float4 CustomBodyState::GetLinearMomentum( const ::Oyster::Math::Float3 &at ) const
		{
			return this->GetLinearMomentum( ::Oyster::Math::Float4(at, 1.0f) );
		}

		inline ::Oyster::Math::Float4 CustomBodyState::GetLinearMomentum( const ::Oyster::Math::Float4 &at ) const
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::GetLinearMomentum)
			return ::Oyster::Math::Float4::null;
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetAngularMomentum() const
		{
			return this->angularMomentum;
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

		inline void CustomBodyState::SetMass( ::Oyster::Math::Float m )
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::SetRestitutionCoeff)
		}

		inline void CustomBodyState::SetRestitutionCoeff( ::Oyster::Math::Float e )
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::SetRestitutionCoeff)
		}

		inline void CustomBodyState::SetLinearMomentum( const ::Oyster::Math::Float3 &g )
		{
			this->linearMomentum = ::Oyster::Math::Float4( g, 0.0f );
			this->isDisturbed = true;
		}

		inline void CustomBodyState::SetAngularMomentum( const ::Oyster::Math::Float3 &h )
		{
			this->angularMomentum = ::Oyster::Math::Float4( h, 0.0f );
			this->isDisturbed = true;
		}

		inline void CustomBodyState::ApplyImpulse( const ::Oyster::Math::Float4 &j )
		{
			this->linearMomentum += j;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::ApplyImpulse( const ::Oyster::Math::Float4 &j, const ::Oyster::Math::Float4 &at, const ::Oyster::Math::Float4 &normal )
		{
			//! @todo TODO: stub to be implemented (CustomBodyState::ApplyImpulse)

			this->linearMomentum += j.Dot( normal ) * normal; // perpendicular impulse component
			//this->angularImpulse += Formula:: ...

			this->isDisturbed = true;
		}

		inline bool CustomBodyState::IsSpatiallyAltered() const
		{
			return this->isSpatiallyAltered;
		}

		inline bool CustomBodyState::IsDisturbed() const
		{
			return this->isDisturbed;
		}
	}
} }

#endif