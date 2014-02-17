/////////////////////////////////////////////////////////////////////
// Created by Pär Hammarstrand 2013
// Edited by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_COLLISION_3D_ICOLLIDEABLE_H
#define OYSTER_COLLISION_3D_ICOLLIDEABLE_H

#include "OysterMath.h"
#include "Utilities.h"

namespace Oyster { namespace Collision3D //! Contains a collection of 3D shapes and intercollission algorithms.
{
	class ICollideable
	{
	public:
		enum Type
		{
			Type_undefined,
			Type_universe,
			Type_point,
			Type_ray,
			Type_sphere,
			Type_plane,
			// Type_triangle,
			Type_box_axis_aligned,
			Type_box,
			Type_frustrum,
			Type_line,
			Type_cone,
		};

		const Type type;

		ICollideable( Type type = Type_undefined );
		virtual ~ICollideable();

		virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const = 0;
		virtual bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const = 0;
		virtual bool Intersects( const ICollideable &target ) const = 0;
		virtual bool Contains( const ICollideable &target ) const = 0;

		virtual ::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const = 0;
	};
} }
#endif