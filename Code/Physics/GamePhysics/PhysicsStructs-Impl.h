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
			inline CustomBodyState::CustomBodyState( ::Oyster::Math::Float mass, ::Oyster::Math::Float3 reach, ::Oyster::Math::Float restitutionCoeff, ::Oyster::Math::Float staticFrictionCoeff, ::Oyster::Math::Float dynamicFrictionCoeff, const ::Oyster::Math::Float3 &centerPos, const ::Oyster::Math::Quaternion& quaternion, ::Oyster::Math::Float3 previousVelocity)
			{
				this->mass = mass;
				this->reach = reach;
				this->restitutionCoeff = restitutionCoeff;
				this->staticFrictionCoeff = staticFrictionCoeff;
				this->dynamicFrictionCoeff = dynamicFrictionCoeff;
				this->centerPos = centerPos;
				this->quaternion = quaternion;
				this->previousVelocity = ::Oyster::Math::Float3::null;
			}

			inline CustomBodyState & CustomBodyState::operator = ( const CustomBodyState &state )
			{
				this->mass = state.mass;
				this->restitutionCoeff = state.restitutionCoeff;
				this->reach = state.reach;
				this->staticFrictionCoeff = state.staticFrictionCoeff;
				this->dynamicFrictionCoeff = state.dynamicFrictionCoeff;
				this->centerPos = state.centerPos;
				this->quaternion = state.quaternion;
				this->previousVelocity = state.previousVelocity;

				return *this;
			}


			inline ::Oyster::Math::Float4x4 CustomBodyState::GetRotation() const
			{
				return ::Oyster::Math3D::RotationMatrix( this->quaternion );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetOrientation() const
			{
				return ::Oyster::Math3D::OrientationMatrix( this->quaternion, this->centerPos );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetView() const
			{
				return ::Oyster::Math3D::ViewMatrix( this->quaternion, this->centerPos );
			}

			inline ::Oyster::Math::Float4x4 CustomBodyState::GetView( const ::Oyster::Math::Float3 &offset ) const
			{
				return ::Oyster::Math3D::ViewMatrix( this->quaternion, (this->centerPos + offset) );
			}

		}
	} 
}

#endif