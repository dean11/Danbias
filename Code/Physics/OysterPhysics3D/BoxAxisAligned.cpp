/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "BoxAxisAligned.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

BoxAxisAligned::BoxAxisAligned( ) : ICollideable(Type_box_axis_aligned)
{
	this->minVertex = Float3(-0.5f,-0.5f,-0.5f );
	this->maxVertex = Float3( 0.5f, 0.5f, 0.5f );
}

BoxAxisAligned::BoxAxisAligned( const Float3 &minVertex, const Float3 &maxVertex ) : ICollideable(Type_box_axis_aligned)
{
	this->minVertex = Float4( minVertex, 1.0f );
	this->maxVertex = Float4( maxVertex, 1.0f );
}

BoxAxisAligned::BoxAxisAligned( const Float &leftClip, const Float &rightClip, const Float &topClip, const Float &bottomClip, const Float &nearClip, const Float &farClip ) : ICollideable(Type_box_axis_aligned)
{
	this->minVertex = Float4( leftClip, bottomClip, nearClip, 1.0f );
	this->maxVertex = Float4( rightClip, topClip, farClip, 1.0f );
}

BoxAxisAligned::~BoxAxisAligned( ) {}

BoxAxisAligned & BoxAxisAligned::operator = ( const BoxAxisAligned &box )
{
	this->minVertex = box.minVertex;
	this->maxVertex = box.maxVertex;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> BoxAxisAligned::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new BoxAxisAligned(*this) );
}

bool BoxAxisAligned::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target );
	case Type_plane:			return Utility::Intersect( *this, (const Plane&)target );
	// case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( *this, (const BoxAxisAligned&)target );
	// case Type_box:				return false; // TODO: 
	// case Type_frustrum:			return false; // TODO: 
	default:					return false;
	}
}

bool BoxAxisAligned::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	//! @todo TODO: implement stub properly
	return this->Intersects( target );
}

bool BoxAxisAligned::Contains( const ICollideable &target ) const
{
	//switch( target.type )
	//{
	////case Type_point:			return false; // TODO: 
	////case Type_sphere:			return false; // TODO: 
	////case Type_triangle:		return false; // TODO: 
	////case Type_box_axis_aligned:	return false; // TODO: 
	////case Type_box:			return false; // TODO: 
	////case Type_frustrum:		return false; // TODO: 
	//default:					return false;
	//}
	return false;
}

Float BoxAxisAligned::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}