/////////////////////////////////////////////////////////////////////
// Created by Erik Persson 2014
/////////////////////////////////////////////////////////////////////

#include "Cone.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;


Cone::Cone( ) : ICollideable(Type_cone)
{
	this->radius = 1;
	this->height = Oyster::Math::Float3(0,0,0);
}

Cone::Cone( const ::Oyster::Math::Float3 &height, const Oyster::Math::Float3 &position, const ::Oyster::Math::Float &radius ) : ICollideable(Type_cone)
{
	this->radius = radius;
	this->height = height;
	this->position = position;
}

Cone::Cone( const ::Oyster::Math::Float4 &height, const Oyster::Math::Float4 &position, const ::Oyster::Math::Float &radius ) : ICollideable(Type_cone)
{
	this->radius = radius;
	this->height = (Oyster::Math::Float3)height;
	this->position = (Oyster::Math::Float3)position;
}

Cone::~Cone( )
{
	
}

Cone & Cone::operator = ( const Cone &cone )
{
	this->radius = cone.radius;
	this->height = cone.height;
	this->position = cone.position;
	return *this;
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Cone::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Cone(*this) );
}

