/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "FluidDrag.h"

using namespace ::Oyster::Physics3D;
using namespace ::Oyster::Math3D;

FluidDrag::FluidDrag( Float _density, Float _dragCoeff )
		: density(_density), dragCoeff(_dragCoeff) {}

FluidDrag & FluidDrag::operator = ( const FluidDrag &fluid )
{
	this->density = fluid.density;
	this->dragCoeff = fluid.dragCoeff;
	return *this;
}
void FluidDrag::SetDensity( ::Oyster::Math::Float d )
{ this->density = d; }

void FluidDrag::SetDragCoefficient( ::Oyster::Math::Float c )
{ this->dragCoeff = c; }

Float3 & FluidDrag::GetDragForce( const Float3 &deltaVelocity, Float crossSectionalArea, Float3 &targetMem ) const
{ return targetMem = (0.5f * this->density * this->dragCoeff * crossSectionalArea * deltaVelocity.GetMagnitude() ) * deltaVelocity; }