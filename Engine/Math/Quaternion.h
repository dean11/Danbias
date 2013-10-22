/////////////////////////////////////////////////////////////////////
// Linear Math Quaternions
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef LINEARALGEBRA_QUATERNION_H
#define LINEARALGEBRA_QUATERNION_H

#include "Vector.h"
#include <math.h>

namespace LinearAlgebra
{
	template<typename ElementType>
	class Quaternion
	{
	public:
		union
		{
			struct{ Vector3<ElementType> imaginary; ElementType real; };
			ElementType element[4];
			char byte[sizeof(ElementType[2])];
		};

		Quaternion( );
		Quaternion( const Quaternion &quaternion );
		Quaternion( const Vector3<ElementType> &imaginary, const ElementType &real );
		~Quaternion( );

		operator ElementType* ( );
		operator const ElementType* ( ) const;
		operator char* ( );
		operator const char* ( ) const;
		ElementType & operator [] ( int i );
		const ElementType & operator [] ( int i ) const;

		Quaternion<ElementType> & operator  = ( const Quaternion<ElementType> &quaternion );
		Quaternion<ElementType> & operator *= ( const ElementType &scalar );
		Quaternion<ElementType> & operator /= ( const ElementType &scalar );
		Quaternion<ElementType> & operator += ( const Quaternion<ElementType> &quaternion );
		Quaternion<ElementType> & operator -= ( const Quaternion<ElementType> &quaternion );
		Quaternion<ElementType>   operator  * ( const Quaternion<ElementType> &quaternion ) const;
		Quaternion<ElementType>   operator  * ( const Vector3<ElementType> &vector ) const;
		Quaternion<ElementType>   operator  * ( const ElementType &scalar ) const;
		Quaternion<ElementType>   operator  / ( const ElementType &scalar ) const;
		Quaternion<ElementType>   operator  + ( const Quaternion<ElementType> &quaternion ) const;
		Quaternion<ElementType>   operator  - ( const Quaternion<ElementType> &quaternion ) const;
		Quaternion<ElementType>   operator  - ( ) const;

		Quaternion<ElementType> getConjugate( ) const;
	};

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////
	
	template<typename ElementType>
	Quaternion<ElementType>::Quaternion( ) : imaginary(0,0,0), real(0) {}
	
	template<typename ElementType>
	Quaternion<ElementType>::Quaternion( const Quaternion &quaternion ) : imaginary(quaternion.imaginary), real(quaternion.real) {}
	
	template<typename ElementType>
	Quaternion<ElementType>::Quaternion( const Vector3<ElementType> &_imaginary, const ElementType &_real )  : imaginary(_imaginary), real(_real) {}
		
	template<typename ElementType>
	Quaternion<ElementType>::~Quaternion( ) { /* Nothing that needs to be done */ }
	
	template<typename ElementType>
	inline Quaternion<ElementType>::operator ElementType* ( )
	{ return this->element; }
	
	template<typename ElementType>
	inline Quaternion<ElementType>::operator const ElementType* ( ) const
	{ return this->element; }
	
	template<typename ElementType>
	inline Quaternion<ElementType>::operator char* ( )
	{ return this->byte; }
	
	template<typename ElementType>
	inline Quaternion<ElementType>::operator const char* ( ) const
	{ return this->byte; }
	
	template<typename ElementType>
	inline ElementType & Quaternion<ElementType>::operator [] ( int i )
	{ return this->element[i]; }
	
	template<typename ElementType>
	inline const ElementType & Quaternion<ElementType>::operator [] ( int i ) const
	{ return this->element[i]; }
	
	template<typename ElementType>
	Quaternion<ElementType> & Quaternion<ElementType>::operator = ( const Quaternion<ElementType> &quaternion )
	{
		this->imaginary = quaternion.imaginary;
		this->real = quaternion.real;
		return *this;
	}
	
	template<typename ElementType>
	Quaternion<ElementType> & Quaternion<ElementType>::operator *= ( const ElementType &scalar )
	{
		this->imaginary *= scalar;
		this->real *= scalar;
		return *this;
	}
	
	template<typename ElementType>
	Quaternion<ElementType> & Quaternion<ElementType>::operator /= ( const ElementType &scalar )
	{
		this->imaginary /= scalar;
		this->real /= scalar;
		return *this;
	}
	
	template<typename ElementType>
	Quaternion<ElementType> & Quaternion<ElementType>::operator += ( const Quaternion<ElementType> &quaternion )
	{
		this->imaginary += quaternion.imaginary;
		this->real += quaternion.real;
		return *this;
	}
	
	template<typename ElementType>
	Quaternion<ElementType> & Quaternion<ElementType>::operator -= ( const Quaternion<ElementType> &quaternion )
	{
		this->imaginary -= quaternion.imaginary;
		this->real -= quaternion.real;
		return *this;
	}
	
	template<typename ElementType>
	Quaternion<ElementType> Quaternion<ElementType>::operator * ( const Quaternion<ElementType> &quaternion ) const
	{
		Vector3<ElementType> im = this->imaginary.cross( quaternion.imaginary );
		im += (quaternion.imaginary * this->real);
		im += (this->imaginary * quaternion.real);

		ElementType re = this->real * quaternion.real;
		re -= this->imaginary.dot( quaternion.imaginary );

		return Quaternion<ElementType>( im, re );	
	}

	template<typename ElementType>
	Quaternion<ElementType> Quaternion<ElementType>::operator * ( const Vector3<ElementType> &vector ) const
	{
		Vector3<ElementType> im = this->imaginary.cross( vector );
		im += (vector * this->real);

		ElementType re = this->imaginary.dot( vector ) * -1;

		return Quaternion<ElementType>( im, re );	
	}
	
	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Quaternion<ElementType>(*this) *= scalar; }
	
	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::operator /  ( const ElementType &scalar ) const
	{ return Quaternion<ElementType>(*this) /= scalar; }
	
	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::operator + ( const Quaternion<ElementType> &quaternion ) const
	{ return Quaternion<ElementType>(*this) += quaternion; }
	
	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::operator - ( const Quaternion<ElementType> &quaternion ) const
	{ return Quaternion<ElementType>(*this) -= quaternion; }

	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::operator - ( ) const
	{ return Quaternion<ElementType>(-this->imaginary, -this->real); }

	template<typename ElementType>
	inline Quaternion<ElementType> Quaternion<ElementType>::getConjugate( ) const
	{ return Quaternion<ElementType>(this->imaginary * -1, this->real ); }
}

#endif