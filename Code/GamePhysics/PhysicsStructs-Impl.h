#ifndef PHYSICS_STRUCTS_IMPL_H
#define PHYSICS_STRUCTS_IMPL_H

#include "PhysicsStructs.h"
#include "OysterPhysics3D.h"

namespace Oyster 
{ 
	namespace Physics
	{
		namespace Struct
		{
			inline SimpleBodyDescription::SimpleBodyDescription()
			{
				this->rotation = ::Oyster::Math::Float3::null;
				this->centerPosition = ::Oyster::Math::Float3::null;
				this->size = ::Oyster::Math::Float3( 1.0f );
				this->mass = 6.0f;
				this->restitutionCoeff = 1.0f;
				this->frictionCoeff_Dynamic = 0.5f;
				this->frictionCoeff_Static = 0.5f;
				this->inertiaTensor = ::Oyster::Physics3D::MomentOfInertia();
				this->subscription_onCollision = NULL;
				this->subscription_onCollisionResponse = NULL;
				this->subscription_onMovement = NULL;
				this->ignoreGravity = false;
			}

			inline SphericalBodyDescription::SphericalBodyDescription()
			{
				this->rotation = ::Oyster::Math::Float3::null;
				this->centerPosition = ::Oyster::Math::Float3::null;
				this->radius = 0.5f;
				this->mass = 10.0f;
				this->restitutionCoeff = 1.0f;
				this->frictionCoeff_Dynamic = 0.5f;
				this->frictionCoeff_Static = 0.5f;
				this->subscription_onCollision = NULL;
				this->subscription_onCollisionResponse = NULL;
				this->subscription_onMovement = NULL;
				this->ignoreGravity = false;
			}

			inline CustomBodyState::CustomBodyState( ::Oyster::Math::Float mass, ::Oyster::Math::Float restitutionCoeff, ::Oyster::Math::Float staticFrictionCoeff, ::Oyster::Math::Float kineticFrictionCoeff, const ::Oyster::Physics3D::MomentOfInertia &inertiaTensor, const ::Oyster::Math::Float3 &reach, const ::Oyster::Math::Float3 &centerPos, const ::Oyster::Math::Float3 &rotation, const ::Oyster::Math::Float3 &linearMomentum, const ::Oyster::Math::Float3 &angularMomentum, const ::Oyster::Math::Float3 &gravityNormal )
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
				this->linearImpulse = this->angularImpulse = ::Oyster::Math::Float3::null;
				this->deltaPos = this->deltaAxis = ::Oyster::Math::Float3::null;
				this->isSpatiallyAltered = this->isDisturbed = this->isForwarded = false;
				this->gravityNormal = gravityNormal;
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
				this->deltaPos = state.deltaPos;
				this->deltaAxis = state.deltaAxis;
				this->isSpatiallyAltered = state.isSpatiallyAltered;
				this->isDisturbed = state.isDisturbed;
				this->isForwarded = state.isForwarded;
				this->gravityNormal = state.gravityNormal;
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

			inline const ::Oyster::Physics3D::MomentOfInertia & CustomBodyState::GetMomentOfInertia() const
			{
				return this->inertiaTensor;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetReach() const
			{
				return this->reach;
			}

			inline ::Oyster::Math::Float3 CustomBodyState::GetSize() const
			{
				return 2.0f * this->GetReach();
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetCenterPosition() const
			{
				return this->centerPos;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetAngularAxis() const
			{
				return this->angularAxis;
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetRotation() const
			{
				return ::Oyster::Math3D::RotationMatrix( this->GetAngularAxis() );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetOrientation() const
			{
				return ::Oyster::Math3D::OrientationMatrix( this->angularAxis, this->centerPos );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetOrientation( const ::Oyster::Math::Float3 &offset ) const
			{
				return ::Oyster::Math3D::OrientationMatrix( this->angularAxis, (this->centerPos + offset) );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetView() const
			{
				return ::Oyster::Math3D::ViewMatrix( this->angularAxis, this->centerPos );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetView( const ::Oyster::Math::Float3 &offset ) const
			{
				return ::Oyster::Math3D::ViewMatrix( this->angularAxis, (this->centerPos + offset) );
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetLinearMomentum() const
			{
				return this->linearMomentum;
			}

			inline ::Oyster::Math::Float3 CustomBodyState::GetLinearMomentum( const ::Oyster::Math::Float3 &at ) const
			{
				return this->linearMomentum + ::Oyster::Physics3D::Formula::TangentialLinearMomentum( this->angularMomentum, at - this->centerPos );
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetAngularMomentum() const
			{
				return this->angularMomentum;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetLinearImpulse() const
			{
				return this->linearImpulse;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetAngularImpulse() const
			{
				return this->angularImpulse;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetForward_DeltaPos() const
			{
				return this->deltaPos;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetForward_DeltaAxis() const
			{
				return this->deltaAxis;
			}

			inline const ::Oyster::Math::Float3 & CustomBodyState::GetGravityNormal() const
			{
				return this->gravityNormal;
			}

			inline void CustomBodyState::SetMass_KeepMomentum( ::Oyster::Math::Float m )
			{
				this->mass = m;
			}

			inline void CustomBodyState::SetMass_KeepVelocity( ::Oyster::Math::Float m )
			{
				if( m != 0.0f )
				{ // sanity block!
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

			inline void CustomBodyState::SetMomentOfInertia_KeepMomentum( const ::Oyster::Physics3D::MomentOfInertia &tensor )
			{
				this->inertiaTensor = tensor;
			}

			inline void CustomBodyState::SetMomentOfInertia_KeepVelocity( const ::Oyster::Physics3D::MomentOfInertia &tensor )
			{
				::Oyster::Math::Quaternion rotation = ::Oyster::Math3D::Rotation(this->angularAxis);
				::Oyster::Math::Float3 w = this->inertiaTensor.CalculateAngularVelocity( rotation, this->angularMomentum );
				this->inertiaTensor = tensor;
				this->angularMomentum = this->inertiaTensor.CalculateAngularMomentum( rotation, w );
			}
		
			inline void CustomBodyState::SetSize( const ::Oyster::Math::Float3 &size )
			{
				this->SetReach( 0.5f * size );
			}

			inline void CustomBodyState::SetReach( const ::Oyster::Math::Float3 &halfSize )
			{
				this->reach = halfSize;
				this->reach = ::Utility::Value::Max( this->reach, ::Oyster::Math::Float3::null );
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetCenterPosition( const ::Oyster::Math::Float3 &centerPos )
			{
				this->centerPos = centerPos;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetRotation( const ::Oyster::Math::Float3 &angularAxis )
			{
				this->angularAxis = angularAxis;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetOrientation( const ::Oyster::Math::Float3 &angularAxis, const ::Oyster::Math::Float3 &translation )
			{
				this->angularAxis = angularAxis ;
				this->centerPos = translation;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::SetLinearMomentum( const ::Oyster::Math::Float3 &g )
			{
				this->linearMomentum = g;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::SetAngularMomentum( const ::Oyster::Math::Float3 &h )
			{
				this->angularMomentum = h;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::SetLinearImpulse( const ::Oyster::Math::Float3 &j )
			{
				this->linearImpulse = j;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::SetAngularImpulse( const ::Oyster::Math::Float3 &j )
			{
				this->angularImpulse = j;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::SetGravityNormal( const ::Oyster::Math::Float3 &gravityNormal )
			{
				this->gravityNormal = gravityNormal;
			}

			inline void CustomBodyState::AddRotation( const ::Oyster::Math::Float3 &angularAxis )
			{
				this->angularAxis += angularAxis;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::AddTranslation( const ::Oyster::Math::Float3 &deltaPos )
			{
				this->centerPos += deltaPos;
				this->isSpatiallyAltered = this->isDisturbed = true;
			}

			inline void CustomBodyState::ApplyLinearImpulse( const ::Oyster::Math::Float3 &j )
			{
				this->linearImpulse += j;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::ApplyAngularImpulse( const ::Oyster::Math::Float3 &j )
			{
				this->angularImpulse += j;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::ApplyImpulse( const ::Oyster::Math::Float3 &j, const ::Oyster::Math::Float3 &at, const ::Oyster::Math::Float3 &normal )
			{
				::Oyster::Math::Float3 offset = at - this->centerPos;
				::Oyster::Math::Float3 deltaAngularImpulse = ::Oyster::Physics3D::Formula::AngularMomentum( j, offset );
				this->linearImpulse += j - ::Oyster::Physics3D::Formula::TangentialLinearMomentum( deltaAngularImpulse, offset );

				this->angularImpulse += deltaAngularImpulse;
				this->isDisturbed = true;
			}

			inline void CustomBodyState::ApplyForwarding( const ::Oyster::Math::Float3 &deltaPos, const ::Oyster::Math::Float3 &deltaAxis )
			{
				this->deltaPos += deltaPos;
				this->deltaAxis += deltaAxis;
				this->isDisturbed = this->isForwarded = true;
			}

			inline bool CustomBodyState::IsSpatiallyAltered() const
			{
				return this->isSpatiallyAltered;
			}

			inline bool CustomBodyState::IsDisturbed() const
			{
				return this->isDisturbed;
			}

			inline bool CustomBodyState::IsForwarded() const
			{
				return this->isForwarded;
			}

			inline GravityWell::GravityWell( )
			{
				this->position	= ::Oyster::Math::Float3::null;
				this->mass		= 0.0f;
			}
			
			inline GravityWell::GravityWell( const GravityWell &gravityWell )
			{
				this->position	= gravityWell.position;
				this->mass		= gravityWell.mass;
			}

			inline GravityWell & GravityWell::operator = ( const GravityWell &gravityWell )
			{
				this->position	= gravityWell.position;
				this->mass		= gravityWell.mass;

				return *this;
			}

			inline bool GravityWell::operator == ( const GravityWell &gravity ) const
			{
				if( this->position == gravity.position )
				if( this->mass == gravity.mass )
				{
					return true;
				}
				return false;
			}

			inline bool GravityWell::operator != ( const GravityWell &gravity ) const
			{
				if( this->position == gravity.position )
				if( this->mass == gravity.mass )
				{
					return false;
				}
				return true;
			}

			inline GravityDirected::GravityDirected( )
			{
				this->impulse = ::Oyster::Math::Float3::null;
			}

			inline GravityDirected::GravityDirected( const GravityDirected &gravityDirected )
			{
				this->impulse = gravityDirected.impulse;
			}

			inline GravityDirected & GravityDirected::operator = ( const GravityDirected &gravityDirected )
			{
				this->impulse = gravityDirected.impulse;

				return *this;
			}

			inline bool GravityDirected::operator == ( const GravityDirected &gravity ) const
			{
				return this->impulse == gravity.impulse;
			}

			inline bool GravityDirected::operator != ( const GravityDirected &gravity ) const
			{
				return this->impulse != gravity.impulse;
			}

			inline GravityDirectedField::GravityDirectedField( )
			{
				this->normalizedDirection	= ::Oyster::Math::Float3::null;
				this->mass					= 0.0f;
				this->magnitude				= 0.0f;
			}

			inline GravityDirectedField::GravityDirectedField( const GravityDirectedField &gravityDirectedField )
			{
				this->normalizedDirection	= gravityDirectedField.normalizedDirection;
				this->mass					= gravityDirectedField.mass;
				this->magnitude				= gravityDirectedField.magnitude;
			}

			inline GravityDirectedField & GravityDirectedField::operator = ( const GravityDirectedField &gravityDirectedField )
			{
				this->normalizedDirection	= gravityDirectedField.normalizedDirection;
				this->mass					= gravityDirectedField.mass;
				this->magnitude				= gravityDirectedField.magnitude;

				return *this;
			}

			inline bool GravityDirectedField::operator == ( const GravityDirectedField &gravity ) const
			{
				if( this->normalizedDirection == gravity.normalizedDirection )
				if( this->mass == gravity.mass )
				if( this->magnitude == gravity.magnitude )
				{
					return true;
				}
				return false;
			}

			inline bool GravityDirectedField::operator != ( const GravityDirectedField &gravity ) const
			{
				if( this->normalizedDirection == gravity.normalizedDirection )
				if( this->mass == gravity.mass )
				if( this->magnitude == gravity.magnitude )
				{
					return false;
				}
				return true;
			}

			inline Gravity::Gravity()
			{
				this->gravityType = GravityType_Undefined;
			}

			inline Gravity::Gravity( const Gravity &gravity )
			{
				this->gravityType = gravity.gravityType;

				switch( gravity.gravityType )
				{
				case GravityType_Well:
					this->well = gravity.well;
					break;
				case GravityType_Directed:
					this->directed = gravity.directed;
					break;
				case GravityType_DirectedField:
					this->directedField = gravity.directedField;
					break;
				default: break;
				}
			}
		
			inline Gravity & Gravity::operator = ( const Gravity &gravity )
			{
				this->gravityType = gravity.gravityType;

				switch( gravity.gravityType )
				{
				case GravityType_Well:
					this->well = gravity.well;
					break;
				case GravityType_Directed:
					this->directed = gravity.directed;
					break;
				case GravityType_DirectedField:
					this->directedField = gravity.directedField;
					break;
				default: break;
				}

				return *this;
			}

			inline bool Gravity::operator == ( const Gravity &gravity ) const
			{
				if( this->gravityType == gravity.gravityType )
				{
					switch( this->gravityType )
					{
					case GravityType_Well:			return this->well == gravity.well;
					case GravityType_Directed:		return this->directed == gravity.directed;
					case GravityType_DirectedField:	return this->directedField == gravity.directedField;
					default: return true;
					}
				}
				return false;
			}

			inline bool Gravity::operator != ( const Gravity &gravity ) const
			{
				if( this->gravityType == gravity.gravityType )
				{
					switch( this->gravityType )
					{
					case GravityType_Well:			return this->well != gravity.well;
					case GravityType_Directed:		return this->directed != gravity.directed;
					case GravityType_DirectedField:	return this->directedField != gravity.directedField;
					default: return false;
					}
				}
				return true;
			}
		}
	} 
}

#endif