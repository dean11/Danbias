/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "BoxAxisAligned.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;

BoxAxisAligned::BoxAxisAligned( ) : ICollideable(Type_box_axis_aligned), minVertex(-0.5f,-0.5f,-0.5f), maxVertex(0.5f,0.5f,0.5f) {}
BoxAxisAligned::BoxAxisAligned( const Float3 &_minVertex, const Float3 &_maxVertex ) : ICollideable(Type_box_axis_aligned), minVertex(_minVertex), maxVertex(_maxVertex) {}
BoxAxisAligned::BoxAxisAligned( const Float &leftClip, const Float &rightClip, const Float &topClip, const Float &bottomClip, const Float &nearClip, const Float &farClip )
	: ICollideable(Type_box_axis_aligned), minVertex(leftClip, bottomClip, nearClip), maxVertex(rightClip, topClip, farClip) {}
BoxAxisAligned::~BoxAxisAligned( ) {}

BoxAxisAligned & BoxAxisAligned::operator = ( const BoxAxisAligned &box )
{
	this->minVertex = box.minVertex;
	this->maxVertex = box.maxVertex;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> BoxAxisAligned::Clone( ) const
{ return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new BoxAxisAligned(*this) ); }

bool BoxAxisAligned::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case Type_universe: return true;
	case Type_point: return Utility::Intersect( *this, *(Point*)target );
	case Type_ray: return Utility::Intersect( *this, *(Ray*)target, ((Ray*)target)->collisionDistance );
	case Type_sphere: return Utility::Intersect( *this, *(Sphere*)target );
	case Type_plane: return Utility::Intersect( *this, *(Plane*)target );
	// case Type_triangle: return false; // TODO: 
	case Type_box_axis_aligned: return Utility::Intersect( *this, *(BoxAxisAligned*)target );
	// case Type_box: return false; // TODO: 
	// case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}

bool BoxAxisAligned::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	// case Type_point: return false; // TODO: 
	// case Type_sphere: return false; // TODO: 
	// case Type_triangle: return false; // TODO: 
	// case Type_box_axis_aligned: return false; // TODO: 
	// case Type_box: return false; // TODO: 
	// case Type_frustrum: return false; // TODO: 
	default: return false;
	}
}