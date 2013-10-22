/////////////////////////////////////////////////////////////////////
// Created by Pär Hammarstrand 2013
// Edited by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_ICOLLIDEABLE_H
#define OYSTER_COLLISION_ICOLLIDEABLE_H

namespace Oyster { namespace Collision
{
	class ICollideable
	{
	public:
		enum CollisionType
		{
			Point,
			Ray,
			Sphere,
			Plane,
			Triangle,
			BoxAxisAligned,
			Box,
			Frustrum,
			Line
		};

		enum State
		{
			Missed = 0,
			Contained,
			Intersected
		};

		ICollideable( CollisionType _type ) : type(_type) {};

		const CollisionType type;

		virtual ICollideable* clone( ) const = 0;

		virtual bool Intersects( const ICollideable *target ) const = 0;

		virtual bool Contains( const ICollideable *target ) const = 0;

		//Not supported returns 0;
		virtual State Advanced( const ICollideable *target ) const = 0;
	};
} }
#endif