#ifndef PHYSICS_FORMULA_H
#define PHYSICS_FORMULA_H

#include "OysterMath.h"
#include "OysterPhysics3D.h"

namespace Oyster { namespace Physics { namespace Formula
{
	namespace MomentOfInertia
	{
		::Oyster::Math::Float4x4 CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		::Oyster::Math::Float4x4 CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius );
		::Oyster::Math::Float4x4 CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );
		::Oyster::Math::Float4x4 CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
		::Oyster::Math::Float4x4 CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
	}

	namespace CollisionResponse
	{
		::Oyster::Math::Float Impulse( ::Oyster::Math::Float coeffOfRestitution,
									   ::Oyster::Math::Float massA, ::Oyster::Math::Float momentumA,
									   ::Oyster::Math::Float massB, ::Oyster::Math::Float momentumB );
	}
} } }

#include "PhysicsFormula-Impl.h"

#endif