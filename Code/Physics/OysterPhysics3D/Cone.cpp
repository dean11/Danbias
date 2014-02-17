/////////////////////////////////////////////////////////////////////
// Created by Erik Persson 2014
/////////////////////////////////////////////////////////////////////

#include "Cone.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Collision3D;
using namespace ::Oyster::Math3D;


Cone::Cone( ) : ICollideable(Type_cone)
{
	this->center = Float3(0, 0, 0);
	this->quaternion = Float4(0, 0, 0, 1);
	this->radius = 1;
	this->length = 0;
}

Cone & Cone::operator = ( const Cone &Cone )
{
	this->center = Cone.center;
	this->quaternion = Cone.quaternion;
	this->radius = Cone.radius;
	this->length = Cone.length;

	return *this;
}

Cone::Cone( const ::Oyster::Math::Float &height, const Oyster::Math::Float3 &position, const Oyster::Math::Float4 &quaternion, const ::Oyster::Math::Float &radius ) : ICollideable(Type_cone)
{
	this->center = position;
	this->quaternion = quaternion;
	this->radius = radius;
	this->length = height;
}

Cone::Cone( const ::Oyster::Math::Float &height, const Oyster::Math::Float4 &position, const Oyster::Math::Float4 &quaternion, const ::Oyster::Math::Float &radius ) : ICollideable(Type_cone)
{
	this->center = position;
	this->quaternion = quaternion;
	this->radius = radius;
	this->length = height;
}

Cone::~Cone( )
{
	
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Cone::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Cone(*this) );
}

