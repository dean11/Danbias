/////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATIONS
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_RIGIDBODY_INLINE_H
#define OYSTER_PHYSICS_3D_RIGIDBODY_INLINE_H

#include "RigidBody.h"

namespace Oyster { namespace Physics3D
{
	inline void RigidBody::ApplyImpulse_Linear( const ::Oyster::Math::Float3 &worldJ )
	{
		this->impulse_Linear += worldJ;
	}

	inline void RigidBody::ApplyImpulse_Angular( const ::Oyster::Math::Float3 &worldJ )
	{
		this->impulse_Angular += worldJ;
	}

	inline void RigidBody::ApplyForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength )
	{
		this->impulse_Linear += worldF * updateFrameLength;
	}

	inline void RigidBody::ApplyForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float3 &atWorldPos )
	{
		this->ApplyImpulse( worldF * updateFrameLength, atWorldPos );
	}

	inline ::Oyster::Math::Float4x4 RigidBody::GetToWorldMatrix() const
	{
		return this->GetOrientation();
	}

	inline ::Oyster::Math::Float4x4 RigidBody::GetToLocalMatrix() const
	{
		return this->GetView();
	}

	inline ::Oyster::Math::Float3 RigidBody::GetSize() const
	{
		return 2.0f * this->boundingReach;
	}

	inline void RigidBody::SetSize( const ::Oyster::Math::Float3 &widthHeight )
	{
		this->boundingReach = ::Utility::Value::Abs( 0.5f * widthHeight );
	}

	//inline void RigidBody::SetForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength )
	//{
	//	this->impulse_Linear = worldF * updateFrameLength;
	//}

	//inline void RigidBody::SetForce( const ::Oyster::Math::Float3 &worldF, ::Oyster::Math::Float updateFrameLength, const ::Oyster::Math::Float3 &atWorldPos )
	//{
	//	this->SetImpulse_Linear( worldF * updateFrameLength, atWorldPos );
	//}
} }

#endif