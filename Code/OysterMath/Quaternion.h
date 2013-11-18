/////////////////////////////////////////////////////////////////////
// Linear Math Quaternions
// � Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef LINEARALGEBRA_QUATERNION_H
#define LINEARALGEBRA_QUATERNION_H

#include "Vector.h"
#include <math.h>

namespace LinearAlgebra
{
	template<typename ScalarType>
	struct Quaternion
	{
	public:
		union
		{
			struct{ Vector3<ScalarType> imaginary; ScalarType real; };
			ScalarType element[4];
			char byte[sizeof(ScalarType[2])];
		};

		Quaternion( );
		Quaternion( const Quaternion &quaternion );
		Quaternion( const Vector3<ScalarType> &imaginary, const ScalarType &real );

		operator ScalarType* ( );
		operator const ScalarType* ( ) const;
		operator char* ( );
		operator const char* ( ) const;
		ScalarType & operator [] ( int i );
		const ScalarType & operator [] ( int i ) const;

		Quaternion<ScalarType> & operator  = ( const Quaternion<ScalarType> &quaternion );
		Quaternion<ScalarType> & operator *= ( const ScalarType &scalar );
		Quaternion<ScalarType> & operator /= ( const ScalarType &scalar );
		Quaternion<ScalarType> & operator += ( const Quaternion<ScalarType> &quaternion );
		Quaternion<ScalarType> & operator -= ( const Quaternion<ScalarType> &quaternion );
		Quaternion<ScalarType>   operator  * ( const Quaternion<ScalarType> &quaternion ) const;
		Quaternion<ScalarType>   operator  * ( const Vector3<ScalarType> &vector ) const;
		Quaternion<ScalarType>   operator  * ( const ScalarType &scalar ) const;
		Quaternion<ScalarType>   operator  / ( const ScalarType &scalar ) const;
		Quaternion<ScalarType>   operator  + ( const Quaternion<ScalarType> &quaternion ) const;
		Quaternion<ScalarType>   operator  - ( const Quaternion<ScalarType> &quaternion ) const;
		Quaternion<ScalarType>   operator  - ( ) const;

		Quaternion<ScalarType> GetConjugate( ) const;
	};

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////
	
	template<typename ScalarType>
	Quaternion<ScalarType>::Quaternion( ) : imaginary(), real() {}
	
	template<typename ScalarType>
	Quaternion<ScalarType>::Quaternion( const Quaternion &quaternion )
	{ this->imaginary = quaternion.imaginary; this->real = quaternion.real; }
	
	template<typename ScalarType>
	Quaternion<ScalarType>::Quaternion( const Vector3<ScalarType> &_imaginary, const ScalarType &_real )
	{ this->imaginary = _imaginary; this->real = _real; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType>::operator ScalarType* ( )
	{ return this->element; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType>::operator const ScalarType* ( ) const
	{ return this->element; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType>::operator char* ( )
	{ return this->byte; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType>::operator const char* ( ) const
	{ return this->byte; }
	
	template<typename ScalarType>
	inline ScalarType & Quaternion<ScalarType>::operator [] ( int i )
	{ return this->element[i]; }
	
	template<typename ScalarType>
	inline const ScalarType & Quaternion<ScalarType>::operator [] ( int i ) const
	{ return this->element[i]; }
	
	template<typename ScalarType>
	Quaternion<ScalarType> & Quaternion<ScalarType>::operator = ( const Quaternion<ScalarType> &quaternion )
	{
		this->imaginary = quaternion.imaginary;
		this->real = quaternion.real;
		return *this;
	}
	
	template<typename ScalarType>
	Quaternion<ScalarType> & Quaternion<ScalarType>::operator *= ( const ScalarType &scalar )
	{
		this->imaginary *= scalar;
		this->real *= scalar;
		return *this;
	}
	
	template<typename ScalarType>
	Quaternion<ScalarType> & Quaternion<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->imaginary /= scalar;
		this->real /= scalar;
		return *this;
	}
	
	template<typename ScalarType>
	Quaternion<ScalarType> & Quaternion<ScalarType>::operator += ( const Quaternion<ScalarType> &quaternion )
	{
		this->imaginary += quaternion.imaginary;
		this->real += quaternion.real;
		return *this;
	}
	
	template<typename ScalarType>
	Quaternion<ScalarType> & Quaternion<ScalarType>::operator -= ( const Quaternion<ScalarType> &quaternion )
	{
		this->imaginary -= quaternion.imaginary;
		this->real -= quaternion.real;
		return *this;
	}
	
	template<typename ScalarType>
	Quaternion<ScalarType> Quaternion<ScalarType>::operator * ( const Quaternion<ScalarType> &quaternion ) const
	{
		Vector3<ScalarType> im = this->imaginary.Cross( quaternion.imaginary );
		im += (quaternion.imaginary * this->real);
		im += (this->imaginary * quaternion.real);

		ScalarType re = this->real * quaternion.real;
		re -= this->imaginary.Dot( quaternion.imaginary );

		return Quaternion<ScalarType>( im, re );	
	}

	template<typename ScalarType>
	Quaternion<ScalarType> Quaternion<ScalarType>::operator * ( const Vector3<ScalarType> &vector ) const
	{
		Vector3<ScalarType> im = this->imaginary.Cross( vector );
		im += (vector * this->real);

		ScalarType re = this->imaginary.Dot( vector ) * -1;

		return Quaternion<ScalarType>( im, re );	
	}
	
	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Quaternion<ScalarType>(*this) *= scalar; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::operator /  ( const ScalarType &scalar ) const
	{ return Quaternion<ScalarType>(*this) /= scalar; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::operator + ( const Quaternion<ScalarType> &quaternion ) const
	{ return Quaternion<ScalarType>(*this) += quaternion; }
	
	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::operator - ( const Quaternion<ScalarType> &quaternion ) const
	{ return Quaternion<ScalarType>(*this) -= quaternion; }

	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::operator - ( ) const
	{ return Quaternion<ScalarType>(-this->imaginary, -this->real); }

	template<typename ScalarType>
	inline Quaternion<ScalarType> Quaternion<ScalarType>::GetConjugate( ) const
	{ return Quaternion<ScalarType>(-this->imaginary, this->real ); }
}

#endif