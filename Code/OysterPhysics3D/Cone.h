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
		
			Cone();
			Cone( const ::Oyster::Math::Float3 &height, const Oyster::Math::Float3 &position, const ::Oyster::Math::Float &radius );
			Cone( const ::Oyster::Math::Float4 &height, const Oyster::Math::Float4 &position, const ::Oyster::Math::Float &radius );
			virtual ~Cone( );

			Cone & operator = ( const Cone &Cone );

			virtual ::Utility::DynamicMemory::UniquePointer<ICollideable> Clone( ) const;

			bool Intersects( const ICollideable &target ) const{return false;};
			bool Intersects( const ICollideable &target, ::Oyster::Math::Float4 &worldPointOfContact ) const{return false;};
			bool Contains( const ICollideable &target ) const{return false;};

			::Oyster::Math::Float TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const{return 0;};


			Oyster::Math::Float3 height;
			Oyster::Math::Float3 position;
			Oyster::Math::Float radius;
		};
	}

}




#endif