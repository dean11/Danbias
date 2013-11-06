/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "BoxAxisAligned.h"
#include "Collision.h"

using namespace ::Oyster::Collision;
using namespace ::Oyster::Math;

BoxAxisAligned::BoxAxisAligned( ) : ICollideable(ICollideable::BoxAxisAligned), minVertex(-0.5f,-0.5f,-0.5f), maxVertex(0.5f,0.5f,0.5f) {}
BoxAxisAligned::BoxAxisAligned( const BoxAxisAligned &box ) : ICollideable(ICollideable::BoxAxisAligned), minVertex(box.minVertex), maxVertex(box.maxVertex) {}
BoxAxisAligned::BoxAxisAligned( const Float3 &_minVertex, const Float3 &_maxVertex ) : ICollideable(ICollideable::BoxAxisAligned), minVertex(_minVertex), maxVertex(_maxVertex) {}
BoxAxisAligned::BoxAxisAligned( const Float &leftClip, const Float &rightClip, const Float &topClip, const Float &bottomClip, const Float &nearClip, const Float &farClip )
	: ICollideable(ICollideable::BoxAxisAligned), minVertex(leftClip, bottomClip, nearClip), maxVertex(rightClip, topClip, farClip) {}
BoxAxisAligned::~BoxAxisAligned( ) { /*Nothing needs to be done here*/ }

BoxAxisAligned & BoxAxisAligned::operator = ( const BoxAxisAligned &box )
{
	this->minVertex = box.minVertex;
	this->maxVertex = box.maxVertex;
	return *this;
}

ICollideable* BoxAxisAligned::clone( ) const
{ return new BoxAxisAligned( *this ); }

bool BoxAxisAligned::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: return Utility::intersect( *this, *(Collision::Sphere*)target );
	case ICollideable::Plane: return Utility::intersect( *this, *(Collision::Plane*)target );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *this, *(Collision::BoxAxisAligned*)target );
	case ICollideable::Box: return false; // TODO
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

bool BoxAxisAligned::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return false; // TODO
	case ICollideable::Sphere: return false; // TODO
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return false; // TODO
	case ICollideable::Box: return false; // TODO
	case ICollideable::Frustrum: return false; // TODO
	default: return false;
	}
}

ICollideable::State BoxAxisAligned::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0