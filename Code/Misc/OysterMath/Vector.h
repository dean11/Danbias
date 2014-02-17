/////////////////////////////////////////////////////////////////////
// Linear Math Vectors
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef LINEARALGEBRA_VECTOR_H
#define LINEARALGEBRA_VECTOR_H

#include <math.h>

namespace LinearAlgebra
{
	template<typename ScalarType>
	struct Vector2
	{
	public:
		union
		{
			struct { ScalarType x, y; };
			ScalarType element[2];
			char byte[sizeof(ScalarType[2])];
		};
		
		static const Vector2<ScalarType> null;
		static const Vector2<ScalarType> standard_unit_x;
		static const Vector2<ScalarType> standard_unit_y;

		Vector2( );
		Vector2( const Vector2<ScalarType> &vector );
		Vector2( const ScalarType &element );
		Vector2( const ScalarType element[2] );
		Vector2( const ScalarType &x, const ScalarType &y );

		operator ScalarType* ( );
		operator const ScalarType* ( ) const;
		operator char* ( );
		operator const char* ( ) const;
		ScalarType & operator [] ( int i );
		const ScalarType & operator [] ( int i ) const;

		Vector2<ScalarType> & operator  = ( const Vector2<ScalarType> &vector );
		Vector2<ScalarType> & operator  = ( const ScalarType element[2] );
		Vector2<ScalarType> & operator *= ( const ScalarType &scalar );
		Vector2<ScalarType> & operator /= ( const ScalarType &scalar );
		Vector2<ScalarType> & operator += ( const Vector2<ScalarType> &vector );
		Vector2<ScalarType> & operator -= ( const Vector2<ScalarType> &vector );
		Vector2<ScalarType>   operator  * ( const ScalarType &scalar ) const;
		Vector2<ScalarType>   operator  / ( const ScalarType &scalar ) const;
		Vector2<ScalarType>   operator  + ( const Vector2<ScalarType> &vector ) const;
		Vector2<ScalarType>   operator  - ( const Vector2<ScalarType> &vector ) const;
		Vector2<ScalarType>   operator  - ( ) const; // unary negation

		bool operator == ( const Vector2<ScalarType> &vector ) const;
		bool operator != ( const Vector2<ScalarType> &vector ) const;

		ScalarType GetLength( ) const;
		ScalarType GetMagnitude( ) const;
		ScalarType Dot( const Vector2<ScalarType> &vector ) const;

		//! @return (a.x * b.x, a.y * b.y)
		Vector2<ScalarType>   PiecewiseMultiplication( const Vector2<ScalarType> &vector ) const;

		//! @return a = (a.x * b.x, a.y * b.y)
		Vector2<ScalarType> & PiecewiseMultiplicationAdd( const Vector2<ScalarType> &vector );

		Vector2<ScalarType> & Normalize( );
		Vector2<ScalarType> GetNormalized( ) const;
	};

	template<typename ScalarType>
	struct Vector3
	{
	public:
		union
		{
			struct { ScalarType x, y, z; };
			struct { Vector2<ScalarType> xy; };
			ScalarType element[3];
			char byte[sizeof(ScalarType[3])];
		};

		static const Vector3<ScalarType> null;
		static const Vector3<ScalarType> standard_unit_x;
		static const Vector3<ScalarType> standard_unit_y;
		static const Vector3<ScalarType> standard_unit_z;

		Vector3( );
		Vector3( const Vector3<ScalarType> &vector );
		Vector3( const Vector2<ScalarType> &vector, const ScalarType &z );
		Vector3( const ScalarType &element );
		Vector3( const ScalarType element[3] );
		Vector3( const ScalarType &x, const ScalarType &y, const ScalarType &z );

		operator ScalarType* ();
		operator const ScalarType* () const;
		operator char* ( );
		operator const char* ( ) const;
		ScalarType & operator [] ( int i );
		const ScalarType & operator [] ( int i ) const;

		Vector3<ScalarType> & operator  = ( const Vector3<ScalarType> &vector );
		Vector3<ScalarType> & operator  = ( const ScalarType element[3] );
		Vector3<ScalarType> & operator *= ( const ScalarType &scalar );
		Vector3<ScalarType> & operator /= ( const ScalarType &scalar );
		Vector3<ScalarType> & operator += ( const Vector3<ScalarType> &vector );
		Vector3<ScalarType> & operator -= ( const Vector3<ScalarType> &vector );
		Vector3<ScalarType>   operator  * ( const ScalarType &scalar ) const;
		Vector3<ScalarType>   operator  / ( const ScalarType &scalar ) const;
		Vector3<ScalarType>   operator  + ( const Vector3<ScalarType> &vector ) const;
		Vector3<ScalarType>   operator  - ( const Vector3<ScalarType> &vector ) const;
		Vector3<ScalarType>   operator  - ( ) const; // unary negation

		bool operator == ( const Vector3<ScalarType> &vector ) const;
		bool operator != ( const Vector3<ScalarType> &vector ) const;

		ScalarType GetLength( ) const;
		ScalarType GetMagnitude( ) const;
		ScalarType Dot( const Vector3<ScalarType> &vector ) const;
		Vector3<ScalarType> Cross( const Vector3<ScalarType> &vector ) const;

		//! @return (a.x * b.x, a.y * b.y, a.z * b.z)
		Vector3<ScalarType> PiecewiseMultiplication( const Vector3<ScalarType> &vector ) const;

		//! @return a = (a.x * b.x, a.y * b.y, a.z * b.z)
		Vector3<ScalarType> & PiecewiseMultiplicationAdd( const Vector3<ScalarType> &vector );

		Vector3<ScalarType> & Normalize( );
		Vector3<ScalarType> GetNormalized( ) const;
	};

	template<typename ScalarType>
	struct Vector4
	{
	public:
		union
		{
			struct { ScalarType x, y, z, w; };
			struct { Vector2<ScalarType> xy; };
			struct { Vector3<ScalarType> xyz; };
			ScalarType element[4];
			char byte[sizeof(ScalarType[4])];
		};

		static const Vector4<ScalarType> null;
		static const Vector4<ScalarType> standard_unit_x;
		static const Vector4<ScalarType> standard_unit_y;
		static const Vector4<ScalarType> standard_unit_z;
		static const Vector4<ScalarType> standard_unit_w;

		Vector4( );
		Vector4( const Vector4<ScalarType> &vector );
		Vector4( const Vector3<ScalarType> &vector, const ScalarType &w );
		Vector4( const Vector2<ScalarType> &vector, const ScalarType &z, const ScalarType &w );
		Vector4( const ScalarType &element );
		Vector4( const ScalarType element[4] );
		Vector4( const ScalarType &x, const ScalarType &y, const ScalarType &z, const ScalarType &w );

		operator ScalarType* ();
		operator const ScalarType* () const;
		operator char* ( );
		operator const char* ( ) const;
		ScalarType & operator [] ( int i );
		const ScalarType & operator [] ( int i ) const;

		Vector4<ScalarType> & operator  = ( const Vector4<ScalarType> &vector );
		Vector4<ScalarType> & operator  = ( const ScalarType element[4] );
		Vector4<ScalarType> & operator *= ( const ScalarType &scalar );
		Vector4<ScalarType> & operator /= ( const ScalarType &scalar );
		Vector4<ScalarType> & operator += ( const Vector4<ScalarType> &vector );
		Vector4<ScalarType> & operator -= ( const Vector4<ScalarType> &vector );
		Vector4<ScalarType>   operator  * ( const ScalarType &scalar ) const;
		Vector4<ScalarType>   operator  / ( const ScalarType &scalar ) const;
		Vector4<ScalarType>   operator  + ( const Vector4<ScalarType> &vector ) const;
		Vector4<ScalarType>   operator  - ( const Vector4<ScalarType> &vector ) const;
		Vector4<ScalarType>   operator  - ( ) const; // unary negation
		
		bool operator == ( const Vector4<ScalarType> &vector ) const;
		bool operator != ( const Vector4<ScalarType> &vector ) const;

		ScalarType GetLength( ) const;
		ScalarType GetMagnitude( ) const;
		ScalarType Dot( const Vector4<ScalarType> &vector ) const;

		//! @return (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w)
		Vector4<ScalarType> PiecewiseMultiplication( const Vector4<ScalarType> &vector ) const;

		//! @return a = (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w)
		Vector4<ScalarType> & PiecewiseMultiplicationAdd( const Vector4<ScalarType> &vector );

		Vector4<ScalarType> & Normalize( );
		Vector4<ScalarType> GetNormalized( ) const;
	};
}

template<typename ScalarType> ::LinearAlgebra::Vector2<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector2<ScalarType> &right );
template<typename ScalarType> ::LinearAlgebra::Vector3<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector3<ScalarType> &right );
template<typename ScalarType> ::LinearAlgebra::Vector4<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector4<ScalarType> &right );

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////

namespace LinearAlgebra
{
// Vector2<ScalarType> ///////////////////////////////////////

	template<typename ScalarType> const Vector2<ScalarType> Vector2<ScalarType>::null = Vector2<ScalarType>( 0, 0 );
	template<typename ScalarType> const Vector2<ScalarType> Vector2<ScalarType>::standard_unit_x = Vector2<ScalarType>( 1, 0 );
	template<typename ScalarType> const Vector2<ScalarType> Vector2<ScalarType>::standard_unit_y = Vector2<ScalarType>( 0, 1 );

	template<typename ScalarType>
	inline Vector2<ScalarType>::Vector2( ) : x(), y() {}

	template<typename ScalarType>
	inline Vector2<ScalarType>::Vector2( const Vector2<ScalarType> &vector )
	{ this->x = vector.x; this->y = vector.y; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::Vector2( const ScalarType &_element )
	{ this->x = this->y = _element; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::Vector2( const ScalarType _element[2] )
	{ this->x = _element[0]; this->y = _element[1]; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::Vector2( const ScalarType &_x, const ScalarType &_y )
	{ this->x = _x; this->y = _y; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::operator ScalarType* ()
	{ return this->element; }
	
	template<typename ScalarType>
	inline Vector2<ScalarType>::operator const ScalarType* () const
	{ return this->element; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::operator char* ( )
	{ return this->byte; }

	template<typename ScalarType>
	inline Vector2<ScalarType>::operator const char* ( ) const
	{ return this->byte; }

	template<typename ScalarType>
	inline ScalarType & Vector2<ScalarType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ScalarType>
	inline const ScalarType & Vector2<ScalarType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator = ( const Vector2<ScalarType> &vector )
	{
		this->element[0] = vector.element[0];
		this->element[1] = vector.element[1];
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator = ( const ScalarType _element[2] )
	{
		this->element[0] = _element[0];
		this->element[1] = _element[1];
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator *= ( const ScalarType &scalar )
	{
		this->element[0] *= scalar;
		this->element[1] *= scalar;
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->element[0] /= scalar;
		this->element[1] /= scalar;
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator += ( const Vector2<ScalarType> &vector )
	{
		this->element[0] += vector.element[0];
		this->element[1] += vector.element[1];
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::operator -= ( const Vector2<ScalarType> &vector )
	{
		this->element[0] -= vector.element[0];
		this->element[1] -= vector.element[1];
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Vector2<ScalarType>(*this) *= scalar; }

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Vector2<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::operator + ( const Vector2<ScalarType> &vector ) const
	{ return Vector2<ScalarType>(*this) += vector; }

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::operator - ( const Vector2<ScalarType> &vector ) const
	{ return Vector2<ScalarType>(*this) -= vector; }

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::operator - ( ) const
	{ return Vector2<ScalarType>(-this->x, -this->y); }

	template<typename ScalarType>
	inline bool Vector2<ScalarType>::operator == ( const Vector2<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		return true;
	}

	template<typename ScalarType>
	inline bool Vector2<ScalarType>::operator != ( const Vector2<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		return false;
	}

	template<typename ScalarType>
	inline ScalarType Vector2<ScalarType>::GetLength( ) const
	{ return this->GetMagnitude(); }

	template<typename ScalarType>
	inline ScalarType Vector2<ScalarType>::GetMagnitude( ) const
	{ return (ScalarType) ::sqrt( this->Dot(*this) ); }	

	template<typename ScalarType>
	inline ScalarType Vector2<ScalarType>::Dot( const Vector2<ScalarType> &vector ) const
	{
		ScalarType value = 0;
		value += this->element[0] * vector.element[0];
		value += this->element[1] * vector.element[1];
		return value;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::PiecewiseMultiplication( const Vector2<ScalarType> &vector ) const
	{
		return Vector2<ScalarType>( this->x * vector.x, this->y * vector.y );
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::PiecewiseMultiplicationAdd( const Vector2<ScalarType> &vector )
	{
		this->x *= vector.x;
		this->y *= vector.y;
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> & Vector2<ScalarType>::Normalize( )
	{ return (*this) /= this->GetLength(); }

	template<typename ScalarType>
	inline Vector2<ScalarType> Vector2<ScalarType>::GetNormalized( ) const
	{ return Vector2<ScalarType>(*this).Normalize(); }

// Vector3<ScalarType> ///////////////////////////////////////

	template<typename ScalarType> const Vector3<ScalarType> Vector3<ScalarType>::null = Vector3<ScalarType>( 0, 0, 0 );
	template<typename ScalarType> const Vector3<ScalarType> Vector3<ScalarType>::standard_unit_x = Vector3<ScalarType>( 1, 0, 0 );
	template<typename ScalarType> const Vector3<ScalarType> Vector3<ScalarType>::standard_unit_y = Vector3<ScalarType>( 0, 1, 0 );
	template<typename ScalarType> const Vector3<ScalarType> Vector3<ScalarType>::standard_unit_z = Vector3<ScalarType>( 0, 0, 1 );

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( ) : x(), y(), z() {}

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( const Vector3<ScalarType> &vector )
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; }

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( const Vector2<ScalarType> &vector, const ScalarType &_z )
	{ this->x = vector.x; this->y = vector.y; this->z = _z; }

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( const ScalarType &_element )
	{ this->x = this->y = this->z = _element; }

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( const ScalarType _element[3] )
	{ this->x = _element[0]; this->y = _element[1]; this->z = _element[2]; }

	template<typename ScalarType>
	inline Vector3<ScalarType>::Vector3( const ScalarType &_x, const ScalarType &_y, const ScalarType &_z )
	{ this->x = _x; this->y = _y; this->z = _z; }

	template<typename ScalarType>
	inline Vector3<ScalarType>::operator ScalarType* ()
	{ return this->element; }
	
	template<typename ScalarType>
	inline Vector3<ScalarType>::operator const ScalarType* () const
	{ return this->element; }

	template<typename ScalarType>
	inline ScalarType & Vector3<ScalarType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ScalarType>
	inline const ScalarType & Vector3<ScalarType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator = ( const Vector3<ScalarType> &vector )
	{
		this->element[0] = vector.element[0];
		this->element[1] = vector.element[1];
		this->element[2] = vector.element[2];
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator = ( const ScalarType element[3] )
	{
		this->element[0] = element[0];
		this->element[1] = element[1];
		this->element[2] = element[2];
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator *= ( const ScalarType &scalar )
	{
		this->element[0] *= scalar;
		this->element[1] *= scalar;
		this->element[2] *= scalar;			
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->element[0] /= scalar;
		this->element[1] /= scalar;
		this->element[2] /= scalar;	
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator += ( const Vector3<ScalarType> &vector )
	{
		this->element[0] += vector.element[0];
		this->element[1] += vector.element[1];
		this->element[2] += vector.element[2];
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::operator -= ( const Vector3<ScalarType> &vector )
	{
		this->element[0] -= vector.element[0];
		this->element[1] -= vector.element[1];
		this->element[2] -= vector.element[2];
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Vector3<ScalarType>(*this) *= scalar; }

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Vector3<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::operator + ( const Vector3<ScalarType> &vector ) const
	{ return Vector3<ScalarType>(*this) += vector; }

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::operator - ( const Vector3<ScalarType> &vector ) const
	{ return Vector3<ScalarType>(*this) -= vector; }

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::operator - ( ) const
	{ return Vector3<ScalarType>(-this->x, -this->y, -this->z); }

	template<typename ScalarType>
	inline bool Vector3<ScalarType>::operator == ( const Vector3<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		if( this->z != vector.z ) return false;	
		return true;
	}

	template<typename ScalarType>
	inline bool Vector3<ScalarType>::operator != ( const Vector3<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		if( this->z != vector.z ) return true;
		return false;
	}

	template<typename ScalarType>
	inline ScalarType Vector3<ScalarType>::GetLength( ) const
	{ return this->GetMagnitude(); }

	template<typename ScalarType>
	inline ScalarType Vector3<ScalarType>::GetMagnitude( ) const
	{ return (ScalarType) ::sqrt( this->Dot(*this) ); }

	template<typename ScalarType>
	inline ScalarType Vector3<ScalarType>::Dot( const Vector3<ScalarType> &vector ) const
	{
		ScalarType value = 0;
		value += this->element[0] * vector.element[0];
		value += this->element[1] * vector.element[1];
		value += this->element[2] * vector.element[2];
		return value;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::Cross( const Vector3<ScalarType> &vector ) const
	{
		return Vector3<ScalarType>( (this->y*vector.z) - (this->z*vector.y),
									 (this->z*vector.x) - (this->x*vector.z),
									 (this->x*vector.y) - (this->y*vector.x) );
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::PiecewiseMultiplication( const Vector3<ScalarType> &vector ) const
	{
		return Vector3<ScalarType>( this->x * vector.x, this->y * vector.y, this->z * vector.z );
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::PiecewiseMultiplicationAdd( const Vector3<ScalarType> &vector )
	{
		this->x *= vector.x;
		this->y *= vector.y;
		this->z *= vector.z;
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> & Vector3<ScalarType>::Normalize( )
	{ return (*this) /= this->GetLength(); }

	template<typename ScalarType>
	inline Vector3<ScalarType> Vector3<ScalarType>::GetNormalized( ) const
	{ return Vector3<ScalarType>(*this).Normalize(); }

// Vector4<ScalarType> ///////////////////////////////////////

	template<typename ScalarType> const Vector4<ScalarType> Vector4<ScalarType>::null = Vector4<ScalarType>( 0, 0, 0, 0 );
	template<typename ScalarType> const Vector4<ScalarType> Vector4<ScalarType>::standard_unit_x = Vector4<ScalarType>( 1, 0, 0, 0 );
	template<typename ScalarType> const Vector4<ScalarType> Vector4<ScalarType>::standard_unit_y = Vector4<ScalarType>( 0, 1, 0, 0 );
	template<typename ScalarType> const Vector4<ScalarType> Vector4<ScalarType>::standard_unit_z = Vector4<ScalarType>( 0, 0, 1, 0 );
	template<typename ScalarType> const Vector4<ScalarType> Vector4<ScalarType>::standard_unit_w = Vector4<ScalarType>( 0, 0, 0, 1 );

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( ) : x(), y(), z(), w() {}

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const Vector4<ScalarType> &vector )
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; this->w = vector.w; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const Vector3<ScalarType> &vector, const ScalarType &_w )
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; this->w = _w; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const Vector2<ScalarType> &vector, const ScalarType &_z, const ScalarType &_w )
	{ this->x = vector.x; this->y = vector.y; this->z = _z; this->w = _w; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const ScalarType &_element )
	{ this->x = this->y = this->z = this->w = _element; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const ScalarType _element[4] )
	{ this->x = _element[0]; this->y = _element[1]; this->z = _element[2]; this->w = _element[3]; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::Vector4( const ScalarType &_x, const ScalarType &_y, const ScalarType &_z, const ScalarType &_w )
	{ this->x = _x; this->y = _y; this->z = _z; this->w = _w; }

	template<typename ScalarType>
	inline Vector4<ScalarType>::operator ScalarType* ()
	{ return this->element; }
	
	template<typename ScalarType>
	inline Vector4<ScalarType>::operator const ScalarType* () const
	{ return this->element; }

	template<typename ScalarType>
	inline ScalarType & Vector4<ScalarType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ScalarType>
	inline const ScalarType & Vector4<ScalarType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator = ( const Vector4<ScalarType> &vector )
	{
		this->element[0] = vector.element[0];
		this->element[1] = vector.element[1];
		this->element[2] = vector.element[2];
		this->element[3] = vector.element[3];
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator = ( const ScalarType element[4] )
	{
		this->element[0] = element[0];
		this->element[1] = element[1];
		this->element[2] = element[2];
		this->element[3] = element[3];
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator *= ( const ScalarType &scalar )
	{
		this->element[0] *= scalar;
		this->element[1] *= scalar;
		this->element[2] *= scalar;
		this->element[3] *= scalar;
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->element[0] /= scalar;
		this->element[1] /= scalar;
		this->element[2] /= scalar;
		this->element[3] /= scalar;
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator += ( const Vector4<ScalarType> &vector )
	{
		this->element[0] += vector.element[0];
		this->element[1] += vector.element[1];
		this->element[2] += vector.element[2];
		this->element[3] += vector.element[3];
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::operator -= ( const Vector4<ScalarType> &vector )
	{
		this->element[0] -= vector.element[0];
		this->element[1] -= vector.element[1];
		this->element[2] -= vector.element[2];
		this->element[3] -= vector.element[3];
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Vector4<ScalarType>(*this) *= scalar; }

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Vector4<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator + ( const Vector4<ScalarType> &vector ) const
	{ return Vector4<ScalarType>(*this) += vector; }

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator - ( const Vector4<ScalarType> &vector ) const
	{ return Vector4<ScalarType>(*this) -= vector; }

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator - ( ) const
	{ return Vector4<ScalarType>(-this->x, -this->y, -this->z, -this->w); }

	template<typename ScalarType>
	inline bool Vector4<ScalarType>::operator == ( const Vector4<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		if( this->z != vector.z ) return false;
		if( this->w != vector.w ) return false;	
		return true;
	}

	template<typename ScalarType>
	inline bool Vector4<ScalarType>::operator != ( const Vector4<ScalarType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		if( this->z != vector.z ) return true;
		if( this->w != vector.w ) return true;
		return false;
	}

	template<typename ScalarType>
	inline ScalarType Vector4<ScalarType>::GetLength( ) const
	{ return this->GetMagnitude(); }

	template<typename ScalarType>
	inline ScalarType Vector4<ScalarType>::GetMagnitude( ) const
	{ return (ScalarType) ::sqrt( this->Dot(*this) ); }	

	template<typename ScalarType>
	inline ScalarType Vector4<ScalarType>::Dot( const Vector4<ScalarType> &vector ) const
	{
		ScalarType value = 0;
		value += this->element[0] * vector.element[0];
		value += this->element[1] * vector.element[1];
		value += this->element[2] * vector.element[2];
		value += this->element[3] * vector.element[3];
		return value;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::PiecewiseMultiplication( const Vector4<ScalarType> &vector ) const
	{
		return Vector4<ScalarType>( this->x * vector.x, this->y * vector.y, this->z * vector.z, this->w * vector.w );
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::PiecewiseMultiplicationAdd( const Vector4<ScalarType> &vector )
	{
		this->x *= vector.x;
		this->y *= vector.y;
		this->z *= vector.z;
		this->w *= vector.w;
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> & Vector4<ScalarType>::Normalize( )
	{ return (*this) /= this->GetLength(); }

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::GetNormalized( ) const
	{ return Vector4<ScalarType>(*this).Normalize(); }
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector2<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector2<ScalarType> &right )
{
	return right * left;
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector3<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector3<ScalarType> &right )
{
	return right * left;
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector4<ScalarType> operator * ( const ScalarType &left, const ::LinearAlgebra::Vector4<ScalarType> &right )
{
	return right * left;
}

#endif