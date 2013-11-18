/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_PHYSICS_3D_FLUID_DRAG_H
#define OYSTER_PHYSICS_3D_FLUID_DRAG_H

#include "OysterMath.h"
#include "OysterPhysics3D.h"

namespace Oyster { namespace Physics3D
{
	struct FluidDrag
	{	/// A struct representing fluids* interaction with objects.
		/// *including gas
	public:
		FluidDrag( ::Oyster::Math::Float density = 1.0f, ::Oyster::Math::Float coeff = 1.0f );

		FluidDrag & operator = ( const FluidDrag &fluid );

		void SetDensity( ::Oyster::Math::Float d );
		void SetDragCoefficient( ::Oyster::Math::Float c );

		/******************************************************************
		 * Returns the resistance force when object is moving through fluid.
		 * @param deltaVelocity: (fluid's velocity) - (the object's velocity)
		 * @param crossSectionalArea: The max area of crosssectional surfaces orthogonal to deltaVelocity
		 * @param targetMem: Allocated memory where result is written. default: temporary allocation
		 * @return targetMem
		 ******************************************************************/
		::Oyster::Math::Float3 & GetDragForce( const ::Oyster::Math::Float3 &deltaVelocity, ::Oyster::Math::Float crossSectionalArea, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;

		/** @todo TODO: Add GetMagnusForce ? */

	private:
		::Oyster::Math::Float density, /// density of the fluid (kg/m^3)
							  dragCoeff;
	};
} }

#endif