/////////////////////////////////////////////////////////////////////
// Linear Math Vectors
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef LINEARALGEBRA_VECTOR_H
#define LINEARALGEBRA_VECTOR_H

#include <math.h>

namespace LinearAlgebra
{
	template<typename ElementType>
	class Vector2
	{
	public:
		union
		{
			struct { ElementType x, y; };
			ElementType element[2];
			char byte[sizeof(ElementType[2])];
		};
		
		static const Vector2<ElementType> null;
		static const Vector2<ElementType> standardUnitX;
		static const Vector2<ElementType> standardUnitY;

		Vector2( );
		Vector2( const Vector2<ElementType> &vector );
		Vector2( const ElementType &element );
		Vector2( const ElementType element[2] );
		Vector2( const ElementType &x, const ElementType &y );
		~Vector2( );

		operator ElementType* ( );
		operator const ElementType* ( ) const;
		operator char* ( );
		operator const char* ( ) const;
		ElementType & operator [] ( int i );
		const ElementType & operator [] ( int i ) const;

		Vector2<ElementType> & operator  = ( const Vector2<ElementType> &vector );
		Vector2<ElementType> & operator  = ( const ElementType element[2] );
		Vector2<ElementType> & operator *= ( const ElementType &scalar );
		Vector2<ElementType> & operator /= ( const ElementType &scalar );
		Vector2<ElementType> & operator += ( const Vector2<ElementType> &vector );
		Vector2<ElementType> & operator -= ( const Vector2<ElementType> &vector );
		Vector2<ElementType>   operator  * ( const ElementType &scalar ) const;
		Vector2<ElementType>   operator  / ( const ElementType &scalar ) const;
		Vector2<ElementType>   operator  + ( const Vector2<ElementType> &vector ) const;
		Vector2<ElementType>   operator  - ( const Vector2<ElementType> &vector ) const;
		Vector2<ElementType>   operator  - ( ) const; // unary negation

		bool operator == ( const Vector2<ElementType> &vector ) const;
		bool operator != ( const Vector2<ElementType> &vector ) const;

		ElementType length( ) const;
		ElementType dot( const Vector2<ElementType> &vector ) const;

		Vector2<ElementType> & normalize( );
		Vector2<ElementType> getNormalized( ) const;
	};

	template<typename ElementType>
	class Vector3
	{
	public:
		union
		{
			struct { ElementType x, y, z; };
			struct { Vector2<ElementType> xy; };
			ElementType element[3];
			char byte[sizeof(ElementType[3])];
		};

		static const Vector3<ElementType> null;
		static const Vector3<ElementType> standardUnitX;
		static const Vector3<ElementType> standardUnitY;
		static const Vector3<ElementType> standardUnitZ;

		Vector3( );
		Vector3( const Vector3<ElementType> &vector );
		Vector3( const Vector2<ElementType> &vector, const ElementType &z );
		Vector3( const ElementType &element );
		Vector3( const ElementType element[3] );
		Vector3( const ElementType &x, const ElementType &y, const ElementType &z );
		~Vector3( );

		operator ElementType* ();
		operator const ElementType* () const;
		operator char* ( );
		operator const char* ( ) const;
		ElementType & operator [] ( int i );
		const ElementType & operator [] ( int i ) const;

		Vector3<ElementType> & operator  = ( const Vector3<ElementType> &vector );
		Vector3<ElementType> & operator  = ( const ElementType element[3] );
		Vector3<ElementType> & operator *= ( const ElementType &scalar );
		Vector3<ElementType> & operator /= ( const ElementType &scalar );
		Vector3<ElementType> & operator += ( const Vector3<ElementType> &vector );
		Vector3<ElementType> & operator -= ( const Vector3<ElementType> &vector );
		Vector3<ElementType>   operator  * ( const ElementType &scalar ) const;
		Vector3<ElementType>   operator  / ( const ElementType &scalar ) const;
		Vector3<ElementType>   operator  + ( const Vector3<ElementType> &vector ) const;
		Vector3<ElementType>   operator  - ( const Vector3<ElementType> &vector ) const;
		Vector3<ElementType>   operator  - ( ) const; // unary negation

		bool operator == ( const Vector3<ElementType> &vector ) const;
		bool operator != ( const Vector3<ElementType> &vector ) const;

		ElementType length( ) const;
		ElementType dot( const Vector3<ElementType> &vector ) const;
		Vector3<ElementType> cross( const Vector3<ElementType> &vector ) const;

		Vector3<ElementType> & normalize( );
		Vector3<ElementType> getNormalized( ) const;
	};

	template<typename ElementType>
	class Vector4
	{
	public:
		union
		{
			struct { ElementType x, y, z, w; };
			struct { Vector2<ElementType> xy; };
			struct { Vector3<ElementType> xyz; };
			ElementType element[4];
			char byte[sizeof(ElementType[4])];
		};

		static const Vector4<ElementType> null;
		static const Vector4<ElementType> standardUnitX;
		static const Vector4<ElementType> standardUnitY;
		static const Vector4<ElementType> standardUnitZ;
		static const Vector4<ElementType> standardUnitW;

		Vector4( );
		Vector4( const Vector4<ElementType> &vector );
		Vector4( const Vector3<ElementType> &vector, const ElementType &w );
		Vector4( const Vector2<ElementType> &vector, const ElementType &z, const ElementType &w );
		Vector4( const ElementType &element );
		Vector4( const ElementType element[4] );
		Vector4( const ElementType &x, const ElementType &y, const ElementType &z, const ElementType &w );
		~Vector4( );

		operator ElementType* ();
		operator const ElementType* () const;
		operator char* ( );
		operator const char* ( ) const;
		ElementType & operator [] ( int i );
		const ElementType & operator [] ( int i ) const;

		Vector4<ElementType> & operator  = ( const Vector4<ElementType> &vector );
		Vector4<ElementType> & operator  = ( const ElementType element[4] );
		Vector4<ElementType> & operator *= ( const ElementType &scalar );
		Vector4<ElementType> & operator /= ( const ElementType &scalar );
		Vector4<ElementType> & operator += ( const Vector4<ElementType> &vector );
		Vector4<ElementType> & operator -= ( const Vector4<ElementType> &vector );
		Vector4<ElementType>   operator  * ( const ElementType &scalar ) const;
		Vector4<ElementType>   operator  / ( const ElementType &scalar ) const;
		Vector4<ElementType>   operator  + ( const Vector4<ElementType> &vector ) const;
		Vector4<ElementType>   operator  - ( const Vector4<ElementType> &vector ) const;
		Vector4<ElementType>   operator  - ( ) const; // unary negation
		
		bool operator == ( const Vector4<ElementType> &vector ) const;
		bool operator != ( const Vector4<ElementType> &vector ) const;

		ElementType length( ) const;
		ElementType dot( const Vector4<ElementType> &vector ) const;

		Vector4<ElementType> & normalize( );
		Vector4<ElementType> getNormalized( ) const;
	};

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////

// Vector2<ElementType> ///////////////////////////////////////
	template<typename ElementType> const Vector2<ElementType> Vector2<ElementType>::null = Vector2<ElementType>( );
	template<typename ElementType> const Vector2<ElementType> Vector2<ElementType>::standardUnitX = Vector2<ElementType>( 1, 0 );
	template<typename ElementType> const Vector2<ElementType> Vector2<ElementType>::standardUnitY = Vector2<ElementType>( 0, 1 );

	template<typename ElementType>
	Vector2<ElementType>::Vector2( ) : x(0), y(0) {}

	template<typename ElementType>
	Vector2<ElementType>::Vector2( const Vector2<ElementType> &vector ) : x(vector.x), y(vector.y)
	{ this->x = vector.x; this->y = vector.y; }

	template<typename ElementType>
	Vector2<ElementType>::Vector2( const ElementType &_element ) : x(_element), y(_element)
	{ this->x = this->y = _element; }

	template<typename ElementType>
	Vector2<ElementType>::Vector2( const ElementType _element[2] ) : x(_element[0]), y(_element[1]) {}

	template<typename ElementType>
	Vector2<ElementType>::Vector2( const ElementType &_x, const ElementType &_y ) : x(_x), y(_y) {}

	template<typename ElementType>
	Vector2<ElementType>::~Vector2( ) { /* Nothing that needs to be done */ }

	template<typename ElementType>
	inline Vector2<ElementType>::operator ElementType* ()
	{ return this->element; }
	
	template<typename ElementType>
	inline Vector2<ElementType>::operator const ElementType* () const
	{ return this->element; }

	template<typename ElementType>
	inline Vector2<ElementType>::operator char* ( )
	{ return this->byte; }

	template<typename ElementType>
	inline Vector2<ElementType>::operator const char* ( ) const
	{ return this->byte; }

	template<typename ElementType>
	inline ElementType & Vector2<ElementType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ElementType>
	inline const ElementType & Vector2<ElementType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator = ( const Vector2<ElementType> &vector )
	{
		this->element[0] = vector.element[0];
		this->element[1] = vector.element[1];
		return *this;
	}

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator = ( const ElementType _element[2] )
	{
		this->element[0] = _element[0];
		this->element[1] = _element[1];
		return *this;
	}

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator *= ( const ElementType &scalar )
	{
		this->element[0] *= scalar;
		this->element[1] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator /= ( const ElementType &scalar )
	{
		this->element[0] /= scalar;
		this->element[1] /= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator += ( const Vector2<ElementType> &vector )
	{
		this->element[0] += vector.element[0];
		this->element[1] += vector.element[1];
		return *this;
	}

	template<typename ElementType>
	Vector2<ElementType> & Vector2<ElementType>::operator -= ( const Vector2<ElementType> &vector )
	{
		this->element[0] -= vector.element[0];
		this->element[1] -= vector.element[1];
		return *this;
	}

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Vector2<ElementType>(*this) *= scalar; }

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Vector2<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::operator + ( const Vector2<ElementType> &vector ) const
	{ return Vector2<ElementType>(*this) += vector; }

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::operator - ( const Vector2<ElementType> &vector ) const
	{ return Vector2<ElementType>(*this) -= vector; }

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::operator - ( ) const
	{ return Vector2<ElementType>(-this->x, -this->y); }

	template<typename ElementType>
	bool Vector2<ElementType>::operator == ( const Vector2<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		return true;
	}

	template<typename ElementType>
	bool Vector2<ElementType>::operator != ( const Vector2<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		return false;
	}

	template<typename ElementType>
	inline ElementType Vector2<ElementType>::length( ) const
	{ return (ElementType) ::sqrt( this->dot(*this) ); }

	template<typename ElementType>
	ElementType Vector2<ElementType>::dot( const Vector2<ElementType> &vector ) const
	{
		ElementType value = 0;
		value += this->element[0] * vector.element[0];
		value += this->element[1] * vector.element[1];
		return value;
	}

	template<typename ElementType>
	inline Vector2<ElementType> & Vector2<ElementType>::normalize( )
	{ return (*this) /= this->length(); }

	template<typename ElementType>
	inline Vector2<ElementType> Vector2<ElementType>::getNormalized( ) const
	{ return Vector2<ElementType>(*this).normalize(); }

// Vector3<ElementType> ///////////////////////////////////////
	template<typename ElementType> const Vector3<ElementType> Vector3<ElementType>::null = Vector3<ElementType>( );
	template<typename ElementType> const Vector3<ElementType> Vector3<ElementType>::standardUnitX = Vector3<ElementType>( 1, 0, 0 );
	template<typename ElementType> const Vector3<ElementType> Vector3<ElementType>::standardUnitY = Vector3<ElementType>( 0, 1, 0 );
	template<typename ElementType> const Vector3<ElementType> Vector3<ElementType>::standardUnitZ = Vector3<ElementType>( 0, 0, 1 );

	template<typename ElementType>
	Vector3<ElementType>::Vector3( ) : x(0), y(0), z(0) {}

	template<typename ElementType>
	Vector3<ElementType>::Vector3( const Vector3<ElementType> &vector ) : x(vector.x), y(vector.y), z(vector.z)
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; }

	template<typename ElementType>
	Vector3<ElementType>::Vector3( const Vector2<ElementType> &vector, const ElementType &_z ) : x(vector.x), y(vector.y), z(_z)
	{ this->x = vector.x; this->y = vector.y; }

	template<typename ElementType>
	Vector3<ElementType>::Vector3( const ElementType &_element ) : x(_element), y(_element), z(_element)
	{ this->x = this->y = this->z = _element; }

	template<typename ElementType>
	Vector3<ElementType>::Vector3( const ElementType _element[3] ) : x(_element[0]), y(_element[1]), z(_element[2]) {}

	template<typename ElementType>
	Vector3<ElementType>::Vector3( const ElementType &_x, const ElementType &_y, const ElementType &_z ) : x(_x), y(_y), z(_z)
	{ this->x = _x; this->y = _y; this->z = _z; }

	template<typename ElementType>
	Vector3<ElementType>::~Vector3( ) {  /* Nothing that needs to be done */ }

	template<typename ElementType>
	inline Vector3<ElementType>::operator ElementType* ()
	{ return this->element; }
	
	template<typename ElementType>
	inline Vector3<ElementType>::operator const ElementType* () const
	{ return this->element; }

	template<typename ElementType>
	inline ElementType & Vector3<ElementType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ElementType>
	inline const ElementType & Vector3<ElementType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator = ( const Vector3<ElementType> &vector )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] = vector.element[i];
		return *this;
	}

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator = ( const ElementType element[3] )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] = element[i];
		return *this;
	}

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator *= ( const ElementType &scalar )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator /= ( const ElementType &scalar )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] /= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator += ( const Vector3<ElementType> &vector )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] += vector.element[i];
		return *this;
	}

	template<typename ElementType>
	Vector3<ElementType> & Vector3<ElementType>::operator -= ( const Vector3<ElementType> &vector )
	{
		for( int i = 0; i < 3; ++i )
			this->element[i] -= vector.element[i];
		return *this;
	}

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Vector3<ElementType>(*this) *= scalar; }

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Vector3<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::operator + ( const Vector3<ElementType> &vector ) const
	{ return Vector3<ElementType>(*this) += vector; }

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::operator - ( const Vector3<ElementType> &vector ) const
	{ return Vector3<ElementType>(*this) -= vector; }

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::operator - ( ) const
	{ return Vector3<ElementType>(-this->x, -this->y, -this->z); }

	template<typename ElementType>
	bool Vector3<ElementType>::operator == ( const Vector3<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		if( this->z != vector.z ) return false;	
		return true;
	}

	template<typename ElementType>
	bool Vector3<ElementType>::operator != ( const Vector3<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		if( this->z != vector.z ) return true;
		return false;
	}

	template<typename ElementType>
	inline ElementType Vector3<ElementType>::length( ) const
	{ return (ElementType) ::sqrt( this->dot(*this) ); }

	template<typename ElementType>
	ElementType Vector3<ElementType>::dot( const Vector3<ElementType> &vector ) const
	{
		ElementType value = 0;
		for( int i = 0; i < 3; ++i )
			value += this->element[i] * vector.element[i];
		return value;
	}

	template<typename ElementType>
	Vector3<ElementType> Vector3<ElementType>::cross( const Vector3<ElementType> &vector ) const
	{
		return Vector3<ElementType>( (this->y*vector.z) - (this->z*vector.y),
									 (this->z*vector.x) - (this->x*vector.z),
									 (this->x*vector.y) - (this->y*vector.x) );
	}

	template<typename ElementType>
	inline Vector3<ElementType> & Vector3<ElementType>::normalize( )
	{ return (*this) /= this->length(); }

	template<typename ElementType>
	inline Vector3<ElementType> Vector3<ElementType>::getNormalized( ) const
	{ return Vector3<ElementType>(*this).normalize(); }

// Vector4<ElementType> ///////////////////////////////////////
	template<typename ElementType> const Vector4<ElementType> Vector4<ElementType>::null = Vector4<ElementType>( );
	template<typename ElementType> const Vector4<ElementType> Vector4<ElementType>::standardUnitX = Vector4<ElementType>( 1, 0, 0, 0 );
	template<typename ElementType> const Vector4<ElementType> Vector4<ElementType>::standardUnitY = Vector4<ElementType>( 0, 1, 0, 0 );
	template<typename ElementType> const Vector4<ElementType> Vector4<ElementType>::standardUnitZ = Vector4<ElementType>( 0, 0, 1, 0 );
	template<typename ElementType> const Vector4<ElementType> Vector4<ElementType>::standardUnitW = Vector4<ElementType>( 0, 0, 0, 1 );

	template<typename ElementType>
	Vector4<ElementType>::Vector4( ) : x(0), y(0), z(0), w(0) {}

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const Vector4<ElementType> &vector ) : x(vector.x), y(vector.y), z(vector.z), w(vector.z)
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; this->w = vector.w; }

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const Vector3<ElementType> &vector, const ElementType &_w ) : x(vector.x), y(vector.y), z(vector.z), w(_w)
	{ this->x = vector.x; this->y = vector.y; this->z = vector.z; }

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const Vector2<ElementType> &vector, const ElementType &_z, const ElementType &_w ) : x(vector.x), y(vector.y), z(_z), w(_w)
	{ this->x = vector.x; this->y = vector.y; this->z = _z; this->w = _w; }

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const ElementType &_element ) : x(_element), y(_element), z(_element), w(_element)
	{ this->x = this->y = this->z = this->w = _element; }

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const ElementType _element[4] ) : x(_element[0]), y(_element[1]), z(_element[2]), w(_element[3]) {}

	template<typename ElementType>
	Vector4<ElementType>::Vector4( const ElementType &_x, const ElementType &_y, const ElementType &_z, const ElementType &_w ) : x(_x), y(_y), z(_z), w(_w)
	{ this->x = _x; this->y = _y; this->z = _z; this->w = _w; }

	template<typename ElementType>
	Vector4<ElementType>::~Vector4( ) {  /* Nothing that needs to be done */ }

	template<typename ElementType>
	inline Vector4<ElementType>::operator ElementType* ()
	{ return this->element; }
	
	template<typename ElementType>
	inline Vector4<ElementType>::operator const ElementType* () const
	{ return this->element; }

	template<typename ElementType>
	inline ElementType & Vector4<ElementType>::operator [] ( int i )
	{ return this->element[i]; }

	template<typename ElementType>
	inline const ElementType & Vector4<ElementType>::operator [] ( int i ) const
	{ return this->element[i]; }

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator = ( const Vector4<ElementType> &vector )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] = vector.element[i];
		return *this;
	}

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator = ( const ElementType element[4] )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] = element[i];
		return *this;
	}

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator *= ( const ElementType &scalar )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator /= ( const ElementType &scalar )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] /= scalar;
		return *this;
	}

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator += ( const Vector4<ElementType> &vector )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] += vector.element[i];
		return *this;
	}

	template<typename ElementType>
	Vector4<ElementType> & Vector4<ElementType>::operator -= ( const Vector4<ElementType> &vector )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] -= vector.element[i];
		return *this;
	}

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Vector4<ElementType>(*this) *= scalar; }

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Vector4<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::operator + ( const Vector4<ElementType> &vector ) const
	{ return Vector4<ElementType>(*this) += vector; }

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::operator - ( const Vector4<ElementType> &vector ) const
	{ return Vector4<ElementType>(*this) -= vector; }

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::operator - ( ) const
	{ return Vector4<ElementType>(-this->x, -this->y, -this->z, -this->w); }

	template<typename ElementType>
	bool Vector4<ElementType>::operator == ( const Vector4<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return false;
		if( this->y != vector.y ) return false;
		if( this->z != vector.z ) return false;
		if( this->w != vector.w ) return false;	
		return true;
	}

	template<typename ElementType>
	bool Vector4<ElementType>::operator != ( const Vector4<ElementType> &vector ) const
	{
		if( this->x != vector.x ) return true;
		if( this->y != vector.y ) return true;
		if( this->z != vector.z ) return true;
		if( this->w != vector.w ) return true;
		return false;
	}

	template<typename ElementType>
	inline ElementType Vector4<ElementType>::length( ) const
	{ return (ElementType) ::sqrt( this->dot(*this) ); }

	template<typename ElementType>
	ElementType Vector4<ElementType>::dot( const Vector4<ElementType> &vector ) const
	{
		ElementType value = 0;
		for( int i = 0; i < 4; ++i )
			value += this->element[i] * vector.element[i];
		return value;
	}

	template<typename ElementType>
	inline Vector4<ElementType> & Vector4<ElementType>::normalize( )
	{ return (*this) /= this->length(); }

	template<typename ElementType>
	inline Vector4<ElementType> Vector4<ElementType>::getNormalized( ) const
	{ return Vector4<ElementType>(*this).normalize(); }
}

#endif