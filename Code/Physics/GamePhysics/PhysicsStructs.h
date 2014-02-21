#ifndef PHYSICS_STRUCTS_H
#define PHYSICS_STRUCTS_H

#include "OysterMath.h"
#include "PhysicsAPI.h"
#include "Inertia.h"

namespace Oyster 
{ 
	namespace Physics
	{
		namespace Struct
		{
			struct CustomBodyState
			{
			public:
				// Default constructor
				CustomBodyState( ::Oyster::Math::Float mass						= 1.0f,
							     ::Oyster::Math::Float3 reach					= ::Oyster::Math::Float3::null,
								 ::Oyster::Math::Float restitutionCoeff			= 0.5f,
								 ::Oyster::Math::Float staticFrictionCoeff		= 1.0f,
								 ::Oyster::Math::Float dynamicFrictionCoeff		= 1.0f,
								 const ::Oyster::Math::Float3 &centerPos		= ::Oyster::Math::Float3::null,
								 const ::Oyster::Math::Quaternion &quaternion	= ::Oyster::Math::Quaternion(::Oyster::Math::Float3(0, 0, 0), 1),
								 ::Oyster::Math::Float3 previousVelocity		= ::Oyster::Math::Float3::null);

				// Assignment operator
				CustomBodyState & operator = ( const CustomBodyState &state );

				// Get functions that calculate matrices that do not exist as variables
				::Oyster::Math::Float4x4	 GetRotation() const;
				::Oyster::Math::Float4x4	 GetOrientation() const;
				::Oyster::Math::Float4x4	 GetView() const;
				::Oyster::Math::Float4x4	 GetView( const ::Oyster::Math::Float3 &offset ) const;
			
				// Variables for state
				::Oyster::Math::Float mass, restitutionCoeff, staticFrictionCoeff, dynamicFrictionCoeff;
				::Oyster::Math::Float3 reach, centerPos, previousVelocity;
				::Oyster::Math::Quaternion quaternion;
			};	
		}
	} 
}


#include "PhysicsStructs-Impl.h"

#endif