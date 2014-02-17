/////////////////////////////////////////////////////////////////////
// Created by Erik Persson 2014
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISION_3D_CONE_H
#define OYSTER_COLLISION_3D_CONE_H


#include "OysterMath.h"
#include "ICollideable.h"

namespace Oyster
{
	namespace Collision3D
	{
		class Cone : public ICollideable
		{
		public:
		
			union
			{
				struct{ ::Oyster::Math::Float3 center; ::Oyster::Math::Float4 quaternion; ::Oyster::Math::Float radius;  ::Oyster::Math::Float length; };
				char byte[sizeof(::Oyster::Math::Float3) + sizeof(::Oyster::Math::Float4) + sizeof(::Oyster::Math::Float) + sizeof(::Oyster::Math::Float)];
			};

			Cone();
			Cone( const ::Oyster::Math::Float &height, const Oyster::Math::Float3 &position, const Oyster::Math::Float4 &quaternion, const ::Oyster::Math::Float &radius );
			Cone( const ::Oyster::Math::Float &height, const Oyster::Math::Float4 &position, const Oyster::Math::Float4 &quaternion, const ::Oyster::Math::Float &radius );
			virtual ~Cone( );

			Cone & operator = ( const Cone &Cone );

			virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;

			bool Intersects( const ICollideable &target ) const{return false;};
			bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const{return false;};
			bool Contains( const ICollideable &target ) const{return false;};

			::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const{return 0;};
		};
	}

}




#endif