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
			this->centerPosition = ::Oyster::Math::Float4::null;
			this->size = ::Oyster::Math::Float4( 1.0f );
			this->mass = 12.0f;
			this->inertiaTensor = ::Oyster::Math::Float4x4::identity;
			this->subscription = NULL;
			this->ignoreGravity = false;
		}

		inline SphericalBodyDescription::SphericalBodyDescription()
		{
			this->rotation = ::Oyster::Math::Float4x4::identity;
			this->centerPosition = ::Oyster::Math::Float4::null;
			this->radius = 0.5f;
			this->mass = 10.0f;
			this->subscription = NULL;
			this->ignoreGravity = false;
		}

		inline CustomBodyState::CustomBodyState( ::Oyster::Math::Float mass, ::Oyster::Math::Float restitutionCoeff, ::Oyster::Math::Float staticFrictionCoeff, ::Oyster::Math::Float kineticFrictionCoeff, const ::Oyster::Math::Float4x4 &inertiaTensor, const ::Oyster::Math::Float4 &reach, const ::Oyster::Math::Float4 &centerPos, const ::Oyster::Math::Float4 &rotation, const ::Oyster::Math::Float4 &linearMomentum, const ::Oyster::Math::Float4 &angularMomentum )
		{
			this->mass = mass;
			this->restitutionCoeff = restitutionCoeff;
			this->staticFrictionCoeff = staticFrictionCoeff;
			this->kineticFrictionCoeff = kineticFrictionCoeff;
			this->inertiaTensor = inertiaTensor;
			this->reach = reach;
			this->centerPos = centerPos;
			this->angularAxis = rotation;
			this->linearMomentum = linearMomentum;
			this->angularMomentum = angularMomentum;
			this->linearImpulse = this->angularImpulse = ::Oyster::Math::Float4::null;
			this->isSpatiallyAltered = this->isDisturbed = false;
		}

		inline CustomBodyState & CustomBodyState::operator = ( const CustomBodyState &state )
		{
			this->mass = state.mass;
			this->restitutionCoeff = state.restitutionCoeff;
			this->staticFrictionCoeff = state.staticFrictionCoeff;
			this->kineticFrictionCoeff = state.kineticFrictionCoeff;
			this->inertiaTensor = state.inertiaTensor;
			this->reach = state.reach;
			this->centerPos = state.centerPos;
			this->angularAxis = state.angularAxis;
			this->linearMomentum = state.linearMomentum;
			this->angularMomentum = state.angularMomentum;
			this->linearImpulse = state.linearImpulse;
			this->angularImpulse = state.angularImpulse;
			this->isSpatiallyAltered = state.isSpatiallyAltered;
			this->isDisturbed = state.isDisturbed;
			return *this;
		}

		inline const ::Oyster::Math::Float CustomBodyState::GetMass() const
		{
			return this->mass;
		}

		inline const ::Oyster::Math::Float CustomBodyState::GetRestitutionCoeff() const
		{
			return this->restitutionCoeff;
		}

		inline const ::Oyster::Math::Float CustomBodyState::GetFrictionCoeff_Static() const
		{
			return this->staticFrictionCoeff;
		}

		inline const ::Oyster::Math::Float CustomBodyState::GetFrictionCoeff_Kinetic() const
		{
			return this->kineticFrictionCoeff;
		}

		inline const ::Oyster::Math::Float4x4 & CustomBodyState::GetMomentOfInertia() const
		{
			return this->inertiaTensor;
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

		inline ::Oyster::Math::Float4x4 CustomBodyState::GetOrientation() const
		{
			return ::Oyster::Math3D::OrientationMatrix( this->angularAxis.xyz, this->centerPos.xyz );
		}

		inline ::Oyster::Math::Float4x4 CustomBodyState::GetView() const
		{
			return ::Oyster::Math3D::ViewMatrix( this->angularAxis.xyz, this->centerPos.xyz );
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetLinearMomentum() const
		{
			return this->linearMomentum;
		}

		inline ::Oyster::Math::Float4 CustomBodyState::GetLinearMomentum( const ::Oyster::Math::Float4 &at ) const
		{
			//return this->linearMomentum + ::Oyster::Physics3D::Formula::TangentialLinearMomentum( this->angularMomentum, at - this->centerPos ); // C3083 error?
			return this->linearMomentum + ::Oyster::Math::Float4( this->angularMomentum.xyz.Cross((at - this->centerPos).xyz), 0.0f );
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetAngularMomentum() const
		{
			return this->angularMomentum;
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetLinearImpulse() const
		{
			return this->linearImpulse;
		}

		inline const ::Oyster::Math::Float4 & CustomBodyState::GetAngularImpulse() const
		{
			return this->angularImpulse;
		}

		inline void CustomBodyState::SetMass_KeepMomentum( ::Oyster::Math::Float m )
		{
			this->mass = m;
		}

		inline void CustomBodyState::SetMass_KeepVelocity( ::Oyster::Math::Float m )
		{
			if( m != 0.0f )
			{ // sanity block!
				// Formula::LinearMomentum( m, Formula::LinearVelocity(this->mass, this->linearMomentum) )
				// is the same as (this->linearMomentum / this->mass) * m = (m / this->mass) * this->linearMomentum
				this->linearMomentum *= (m / this->mass);
				this->mass = m;
			}
		}

		inline void CustomBodyState::SetRestitutionCoeff( ::Oyster::Math::Float e )
		{
			this->restitutionCoeff = e;
		}

		inline void CustomBodyState::SetFrictionCoeff( ::Oyster::Math::Float staticU, ::Oyster::Math::Float kineticU )
		{
			this->staticFrictionCoeff = staticU;
			this->kineticFrictionCoeff = kineticU;
		}

		inline void CustomBodyState::SetMomentOfInertia_KeepMomentum( const ::Oyster::Math::Float4x4 &tensor )
		{
			this->inertiaTensor = tensor;
		}

		inline void CustomBodyState::SetMomentOfInertia_KeepVelocity( const ::Oyster::Math::Float4x4 &tensor )
		{
			if( tensor.GetDeterminant() != 0.0f )
			{ // sanity block!
				::Oyster::Math::Float4x4 rotation = ::Oyster::Math3D::RotationMatrix(this->angularAxis.xyz);
				//::Oyster::Math::Float4 w = ::Oyster::Physics3D::Formula::AngularVelocity( (rotation * this->inertiaTensor).GetInverse(), this->angularMomentum ); // C3083 error?
				::Oyster::Math::Float4 w = (rotation * this->inertiaTensor).GetInverse() * this->angularMomentum;
				this->inertiaTensor = tensor;
				//this->angularMomentum = ::Oyster::Physics3D::Formula::AngularMomentum( rotation * tensor, w ); // C3083 error?
				this->angularMomentum = rotation * tensor * w;
			}
		}
		
		inline void CustomBodyState::SetSize( const ::Oyster::Math::Float4 &size )
		{
			this->SetReach( 0.5f * size );
		}

		inline void CustomBodyState::SetReach( const ::Oyster::Math::Float4 &halfSize )
		{
			this->reach.xyz = halfSize;
			this->reach = ::Utility::Value::Max( this->reach, ::Oyster::Math::Float4::null );
			this->isSpatiallyAltered = this->isDisturbed = true;
		}

		inline void CustomBodyState::SetCenterPosition( const ::Oyster::Math::Float4 &centerPos )
		{
			this->centerPos.xyz = centerPos;
			this->isSpatiallyAltered = this->isDisturbed = true;
		}

		inline void CustomBodyState::SetRotation( const ::Oyster::Math::Float4 &angularAxis )
		{
			this->angularAxis.xyz = angularAxis;
			this->isSpatiallyAltered = this->isDisturbed = true;
		}

		inline void CustomBodyState::SetRotation( const ::Oyster::Math::Float4x4 &rotation )
		{
			this->SetRotation( ::Oyster::Math3D::AngularAxis(rotation) );
		}

		inline void CustomBodyState::SetOrientation( const ::Oyster::Math::Float4x4 &orientation )
		{
			this->SetRotation( ::Oyster::Math3D::ExtractAngularAxis(orientation) );
			this->SetCenterPosition( orientation.v[3] );
		}

		inline void CustomBodyState::SetLinearMomentum( const ::Oyster::Math::Float4 &g )
		{
			this->linearMomentum.xyz = g;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::SetAngularMomentum( const ::Oyster::Math::Float4 &h )
		{
			this->angularMomentum.xyz = h;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::SetLinearImpulse( const ::Oyster::Math::Float4 &j )
		{
			this->linearImpulse.xyz = j;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::SetAngularImpulse( const ::Oyster::Math::Float4 &j )
		{
			this->angularImpulse.xyz = j;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::AddRotation( const ::Oyster::Math::Float4 &angularAxis )
		{
			this->angularAxis += angularAxis;
			this->isSpatiallyAltered = this->isDisturbed = true;
		}

		inline void CustomBodyState::AddTranslation( const ::Oyster::Math::Float4 &deltaPos )
		{
			this->centerPos += deltaPos;
			this->isSpatiallyAltered = this->isDisturbed = true;
		}

		inline void CustomBodyState::ApplyLinearImpulse( const ::Oyster::Math::Float4 &j )
		{
			this->linearImpulse += j;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::ApplyAngularImpulse( const ::Oyster::Math::Float4 &j )
		{
			this->angularImpulse += j;
			this->isDisturbed = true;
		}

		inline void CustomBodyState::ApplyImpulse( const ::Oyster::Math::Float4 &j, const ::Oyster::Math::Float4 &at, const ::Oyster::Math::Float4 &normal )
		{
			//::Oyster::Math::Float4 tangentialImpulse = ::Oyster::Physics3D::Formula::AngularMomentum( j, at - this->centerPos ); // C3083 error?
			::Oyster::Math::Float4 tangentialImpulse = ::Oyster::Math::Float4( (at - this->centerPos).xyz.Cross(j.xyz), 0.0f );
			this->linearImpulse += j - tangentialImpulse;
			this->angularImpulse += tangentialImpulse;

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