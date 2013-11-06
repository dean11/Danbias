/////////////////////////////////////////////////////////////////////
// Linear Math Matrixes
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef LINEARALGEBRA_MATRIX_H
#define LINEARALGEBRA_MATRIX_H

#include "Vector.h"
#include "Utilities.h"

namespace LinearAlgebra
{
	template<typename ElementType>
	class Matrix2x2
	{
	public:
		union
		{
			ElementType m[2][2];
			struct{ Vector2<ElementType> v[2]; };
//			struct{ ElementType m11, m21, m12, m22; };
			struct{ ElementType m11, m12, m21, m22; };
			ElementType element[4];
			char byte[sizeof(ElementType[4])];
		};

		static const Matrix2x2<ElementType> identity, null;

		Matrix2x2( );
		Matrix2x2( const ElementType &m11, const ElementType &m12,
			       const ElementType &m21, const ElementType &m22 );
		Matrix2x2( const Vector2<ElementType> vec[2] );
		Matrix2x2( const Vector2<ElementType> &vec1, const Vector2<ElementType> &vec2 );
		Matrix2x2( const ElementType element[4] );
		Matrix2x2( const Matrix2x2<ElementType> &matrix );

		operator ElementType* ( );
		operator const ElementType* ( ) const;

		Matrix2x2<ElementType> & operator  = ( const Vector2<ElementType> vec[2] );
		Matrix2x2<ElementType> & operator  = ( const ElementType element[4] );
		Matrix2x2<ElementType> & operator  = ( const Matrix2x2<ElementType> &matrix );
		Matrix2x2<ElementType> & operator += ( const Matrix2x2<ElementType> &matrix );
		Matrix2x2<ElementType> & operator -= ( const Matrix2x2<ElementType> &matrix );
		Matrix2x2<ElementType> & operator *= ( const ElementType &scalar );
		Matrix2x2<ElementType> & operator /= ( const ElementType &scalar );
		Matrix2x2<ElementType>	 operator  + ( const Matrix2x2<ElementType> &matrix ) const;
		Matrix2x2<ElementType>	 operator  - ( const Matrix2x2<ElementType> &matrix ) const;
		Matrix2x2<ElementType>	 operator  * ( const ElementType &scalar ) const;
		Matrix2x2<ElementType>	 operator  / ( const ElementType &scalar ) const;
		Matrix2x2<ElementType>	 operator  - ( ) const; // unary negation

		ElementType getDeterminant( ) const;
		Matrix2x2<ElementType>   getAdjoint( ) const;
		Matrix2x2<ElementType>   getTranspose( ) const;
		Matrix2x2<ElementType> & transpose( );
		Matrix2x2<ElementType>   getInverse( ) const;
		Matrix2x2<ElementType>   getInverse( ElementType &determinant ) const;
		Matrix2x2<ElementType> & invert( );
		Matrix2x2<ElementType> & invert( ElementType &determinant );
		Vector2<ElementType>		 getRowVector( unsigned int rowID ) const;
		const Vector2<ElementType> & getColumnVector( unsigned int colID ) const;
	};

	template<typename ElementType>
	class Matrix3x3
	{
	public:
		union
		{
			ElementType m[3][3];
			struct{ Vector3<ElementType> v[3]; };
//			struct{ ElementType m11, m21, m31, m12, m22, m32, m13, m23, m33; };
			struct{ ElementType m11, m12, m13, m21, m22, m23, m31, m32, m33; };
			ElementType element[9];
			char byte[sizeof(ElementType[9])];
		};

		static const Matrix3x3<ElementType> identity, null;

		Matrix3x3( );
		Matrix3x3( const ElementType &m11, const ElementType &m12, const ElementType &m13,
			       const ElementType &m21, const ElementType &m22, const ElementType &m23,
				   const ElementType &m31, const ElementType &m32, const ElementType &m33 );
		Matrix3x3( const Vector3<ElementType> vec[3] );
		Matrix3x3( const Vector3<ElementType> &vec1, const Vector3<ElementType> &vec2, const Vector3<ElementType> &vec3 );
		Matrix3x3( const ElementType element[9] );
		Matrix3x3( const Matrix3x3<ElementType> &matrix );

		operator ElementType* ( );
		operator const ElementType* ( ) const;

		Matrix3x3<ElementType> & operator  = ( const Vector3<ElementType> vec[3] );
		Matrix3x3<ElementType> & operator  = ( const ElementType element[9] );
		Matrix3x3<ElementType> & operator  = ( const Matrix3x3<ElementType> &matrix );
		Matrix3x3<ElementType> & operator += ( const Matrix3x3<ElementType> &matrix );
		Matrix3x3<ElementType> & operator -= ( const Matrix3x3<ElementType> &matrix );
		Matrix3x3<ElementType> & operator *= ( const ElementType &scalar );
		Matrix3x3<ElementType> & operator /= ( const ElementType &scalar );
		Matrix3x3<ElementType>	 operator  + ( const Matrix3x3<ElementType> &matrix ) const;
		Matrix3x3<ElementType>	 operator  - ( const Matrix3x3<ElementType> &matrix ) const;
		Matrix3x3<ElementType>	 operator  * ( const ElementType &scalar ) const;
		Matrix3x3<ElementType>	 operator  / ( const ElementType &scalar ) const;
		Matrix3x3<ElementType>	 operator  - ( ) const; // unary negation

		ElementType getDeterminant( ) const;
		Matrix3x3<ElementType>   getAdjoint( ) const;
		Matrix3x3<ElementType>   getTranspose( ) const;
		Matrix3x3<ElementType> & transpose( );
		Matrix3x3<ElementType>   getInverse( ) const;
		Matrix3x3<ElementType>   getInverse( ElementType &determinant ) const;
		Matrix3x3<ElementType> & invert( );
		Matrix3x3<ElementType> & invert( ElementType &determinant );
		Vector3<ElementType>		 getRowVector( unsigned int rowID ) const;
		const Vector3<ElementType> & getColumnVector( unsigned int colID ) const;
	};

	template<typename ElementType>
	class Matrix4x4
	{
	public:
		union
		{
			ElementType m[4][4];
			struct{ Vector4<ElementType> v[4]; };
//			struct{ ElementType m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44; };
			struct{ ElementType m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44; };
			ElementType element[16];
			char byte[sizeof(ElementType[16])];
		};

		static const Matrix4x4<ElementType> identity, null;

		Matrix4x4( );
		Matrix4x4( const ElementType &m11, const ElementType &m12, const ElementType &m13, const ElementType &m14,
			       const ElementType &m21, const ElementType &m22, const ElementType &m23, const ElementType &m24,
				   const ElementType &m31, const ElementType &m32, const ElementType &m33, const ElementType &m34,
				   const ElementType &m41, const ElementType &m42, const ElementType &m43, const ElementType &m44 );
		Matrix4x4( const Vector4<ElementType> vec[4] );
		Matrix4x4( const Vector4<ElementType> &vec1, const Vector4<ElementType> &vec2, const Vector4<ElementType> &vec3, const Vector4<ElementType> &vec4 );
		Matrix4x4( const ElementType element[16] );
		Matrix4x4( const Matrix4x4<ElementType> &matrix );

		operator ElementType* ( );
		operator const ElementType* ( ) const;

		Matrix4x4<ElementType> & operator  = ( const Vector4<ElementType> vec[4] );
		Matrix4x4<ElementType> & operator  = ( const ElementType element[16] );
		Matrix4x4<ElementType> & operator  = ( const Matrix4x4<ElementType> &matrix );
		Matrix4x4<ElementType> & operator += ( const Matrix4x4<ElementType> &matrix );
		Matrix4x4<ElementType> & operator -= ( const Matrix4x4<ElementType> &matrix );
		Matrix4x4<ElementType> & operator *= ( const ElementType &scalar );
		Matrix4x4<ElementType> & operator /= ( const ElementType &scalar );
		Matrix4x4<ElementType>	 operator  + ( const Matrix4x4<ElementType> &matrix ) const;
		Matrix4x4<ElementType>	 operator  - ( const Matrix4x4<ElementType> &matrix ) const;
		Matrix4x4<ElementType>	 operator  * ( const ElementType &scalar ) const;
		Matrix4x4<ElementType>	 operator  / ( const ElementType &scalar ) const;
		Matrix4x4<ElementType>	 operator  - ( ) const; // unary negation

		ElementType getDeterminant( ) const;
		Matrix4x4<ElementType>   getAdjoint( ) const;
		Matrix4x4<ElementType>   getTranspose( ) const;
		Matrix4x4<ElementType> & transpose( );
		Matrix4x4<ElementType>   getInverse( ) const;
		Matrix4x4<ElementType>   getInverse( ElementType &determinant ) const;
		Matrix4x4<ElementType> & invert( );
		Matrix4x4<ElementType> & invert( ElementType &determinant );
		Vector4<ElementType>		 getRowVector( unsigned int rowID ) const;
		const Vector4<ElementType> & getColumnVector( unsigned int colID ) const;
	};

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////

// Matrix2x2<ElementType> ///////////////////////////////////////
	template<typename ElementType>
	const Matrix2x2<ElementType> Matrix2x2<ElementType>::identity = Matrix2x2<ElementType>( 1, 0, 0, 1 );

	template<typename ElementType>
	const Matrix2x2<ElementType> Matrix2x2<ElementType>::null = Matrix2x2<ElementType>( 0, 0, 0, 0 );

	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( ) : m11(0), m21(0), m12(0), m22(0) {}
	
	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( const ElementType &_m11, const ElementType &_m12, const ElementType &_m21, const ElementType &_m22 )
		 : m11(_m11), m21(_m21), m12(_m12), m22(_m22) {}
	
	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( const Vector2<ElementType> vec[2] )
		: m11(vec[0].x), m21(vec[0].y), m12(vec[1].x), m22(vec[1].y) {}

	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( const Vector2<ElementType> &vec1, const Vector2<ElementType> &vec2 )
		: m11(vec1.x), m21(vec1.y), m12(vec2.x), m22(vec2.y) {}

	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( const ElementType _element[4] )
//		: m11(_element[0]), m21(_element[1]), m12(_element[2]), m22(_element[3]) {}
		: m11(_element[0]), m12(_element[1]), m21(_element[2]), m22(_element[3]) {}
	
	template<typename ElementType>
	Matrix2x2<ElementType>::Matrix2x2( const Matrix2x2<ElementType> &matrix )
		: m11(matrix.m11), m21(matrix.m12), m12(matrix.m21), m22(matrix.m22) {}

	template<typename ElementType>
	inline Matrix2x2<ElementType>::operator ElementType* ( )
	{ return this->element; }
	
	template<typename ElementType>
	inline Matrix2x2<ElementType>::operator const ElementType* ( ) const
	{ return this->element; }

	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator = ( const Vector2<ElementType> vec[2] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator = ( const ElementType element[4] )
	{
		for( int i = 0; i < 4; ++i )
			this->element[i] = element[i];
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator = ( const Matrix2x2<ElementType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator += ( const Matrix2x2<ElementType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator -= ( const Matrix2x2<ElementType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator *= ( const ElementType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::operator /= ( const ElementType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		return *this;
	}
	
	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::operator + ( const Matrix2x2<ElementType> &matrix ) const
	{ return Matrix2x2<ElementType>(*this) += matrix; }
	
	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::operator - ( const Matrix2x2<ElementType> &matrix ) const
	{ return Matrix2x2<ElementType>(*this) -= matrix; }
	
	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Matrix2x2<ElementType>(*this) *= scalar; }
	
	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Matrix2x2<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::operator - ( ) const
	{ return Matrix2x2<ElementType>(-this->v[0], -this->v[1]); }

	template<typename ElementType>
	ElementType Matrix2x2<ElementType>::getDeterminant( ) const
	{
		ElementType determinant = (this->m11 * this->m22);
		return determinant -= (this->m12 * this->m21);
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> Matrix2x2<ElementType>::getAdjoint( ) const
	{ return Matrix2x2<ElementType>( this->m22, -this->m21, -this->m12, this->m11 ); }

	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::getTranspose( ) const
	{ return Matrix2x2<ElementType>( this->element[0], this->element[1], this->element[2], this->element[3] ); }
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::transpose( )
	{
		ElementType swapSpace;
		Utility::Element::swap( this->m12, this->m21, swapSpace );
		return *this;
	}
	
	template<typename ElementType>
	inline Matrix2x2<ElementType> Matrix2x2<ElementType>::getInverse( ) const
	{ return this->getAdjoint() /= this->getDeterminant(); }
	
	template<typename ElementType>
	Matrix2x2<ElementType> Matrix2x2<ElementType>::getInverse( ElementType &determinant ) const
	{
		determinant = this->getDeterminant();
		if( determinant != 0 )
			return this->getAdjoint() / determinant;
		return Matrix2x2<ElementType>();
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::invert( )
	{ 
		*this /= this->getDeterminant();
		this->m12 *= -1; this->m21 *= -1;

		ElementType swapSpace;
		Utility::Element::swap( this->m12, this->m21, swapSpace );
		Utility::Element::swap( this->m11, this->m22, swapSpace );
		return *this;
	}
	
	template<typename ElementType>
	Matrix2x2<ElementType> & Matrix2x2<ElementType>::invert( ElementType &determinant )
	{
		determinant = this->getDeterminant();

		if( determinant != 0 )
		{
			*this /= determinant;
			this->m12 *= -1; this->m21 *= -1;

			ElementType swapSpace;
			Utility::Element::swap( this->m12, this->m21, swapSpace );
			Utility::Element::swap( this->m11, this->m22, swapSpace );
		}
		return *this;
	}

	template<typename ElementType>
	inline Vector2<ElementType> Matrix2x2<ElementType>::getRowVector( unsigned int rowID ) const
	{ return Vector2<ElementType>( this->m[0][rowID], this->m[1][rowID] ); }
	
	template<typename ElementType>
	inline const Vector2<ElementType> & Matrix2x2<ElementType>::getColumnVector( unsigned int colID ) const
	{ return this->v[colID]; } 

// Matrix3x3<ElementType> ///////////////////////////////////////
	template<typename ElementType>
	const Matrix3x3<ElementType> Matrix3x3<ElementType>::identity = Matrix3x3<ElementType>( 1, 0, 0, 0, 1, 0, 0, 0, 1 );

	template<typename ElementType>
	const Matrix3x3<ElementType> Matrix3x3<ElementType>::null = Matrix3x3<ElementType>( 0, 0, 0, 0, 0, 0, 0, 0, 0 );

	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( ) : m11(0), m21(0), m31(0), m12(0), m22(0), m32(0), m13(0), m23(0), m33(0) {}
	
	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( const ElementType &_m11, const ElementType &_m12, const ElementType &_m13, const ElementType &_m21, const ElementType &_m22, const ElementType &_m23, const ElementType &_m31, const ElementType &_m32, const ElementType &_m33 )
		 : m11(_m11), m21(_m21), m31(_m31), m12(_m12), m22(_m22), m32(_m32), m13(_m13), m23(_m23), m33(_m33) {}

	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( const Vector3<ElementType> vec[3] )
		: m11(vec[0].x), m21(vec[0].y), m31(vec[0].z), m12(vec[1].x), m22(vec[1].y), m32(vec[1].z), m13(vec[2].x), m23(vec[2].y), m33(vec[2].z) {}
	
	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( const Vector3<ElementType> &vec1, const Vector3<ElementType> &vec2, const Vector3<ElementType> &vec3 )
		: m11(vec1.x), m21(vec1.y), m31(vec1.z), m12(vec2.x), m22(vec2.y), m32(vec2.z), m13(vec3.x), m23(vec3.y), m33(vec3.z) {}
	
	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( const ElementType _element[9] )
//		: m11(_element[0]), m21(_element[1]), m31(_element[2]), m12(_element[3]), m22(_element[4]), m32(_element[5]), m13(_element[6]), m23(_element[7]), m33(_element[8]) {}
		: m11(_element[0]), m12(_element[1]), m13(_element[2]), m21(_element[3]), m22(_element[4]), m23(_element[5]), m31(_element[6]), m32(_element[7]), m33(_element[8]) {}
	
	template<typename ElementType>
	Matrix3x3<ElementType>::Matrix3x3( const Matrix3x3<ElementType> &matrix )
		: m11(matrix.m11), m21(matrix.m21), m31(matrix.m31), m12(matrix.m12), m22(matrix.m22), m32(matrix.m32), m13(matrix.m13), m23(matrix.m23), m33(matrix.m33) {}

	template<typename ElementType>
	inline Matrix3x3<ElementType>::operator ElementType* ( )
	{ return this->element; }
	
	template<typename ElementType>
	inline Matrix3x3<ElementType>::operator const ElementType* ( ) const
	{ return this->element; }

	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator = ( const Vector3<ElementType> vec[3] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		this->v[2] = vec[2];
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator = ( const ElementType element[9] )
	{
		for( int i = 0; i < 9; ++i )
			this->element[i] = element[i];
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator = ( const Matrix3x3<ElementType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		this->v[2] = matrix.v[2];
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator += ( const Matrix3x3<ElementType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		this->v[2] += matrix.v[2];
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator -= ( const Matrix3x3<ElementType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		this->v[2] -= matrix.v[2];
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator *= ( const ElementType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		this->v[2] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::operator /= ( const ElementType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		this->v[2] /= scalar;
		return *this;
	}
	
	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::operator + ( const Matrix3x3<ElementType> &matrix ) const
	{ return Matrix3x3<ElementType>(*this) += matrix; }
	
	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::operator - ( const Matrix3x3<ElementType> &matrix ) const
	{ return Matrix3x3<ElementType>(*this) -= matrix; }
		
	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Matrix3x3<ElementType>(*this) *= scalar; }
	
	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Matrix3x3<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::operator - ( ) const
	{ return Matrix3x3<ElementType>(-this->v[0], -this->v[1], -this->v[2]); }

	template<typename ElementType>
	ElementType Matrix3x3<ElementType>::getDeterminant( ) const
	{ 
		ElementType determinant = (this->m11 * this->m22 * this->m33);
		determinant += (this->m12 * this->m23 * this->m31);
		determinant += (this->m13 * this->m21 * this->m32);
		determinant -= (this->m11 * this->m23 * this->m32);
		determinant -= (this->m12 * this->m21 * this->m33);
		return determinant -= (this->m13 * this->m22 * this->m31);
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> Matrix3x3<ElementType>::getAdjoint( ) const
	{
		return Matrix3x3<ElementType>( (this->m22*this->m33 - this->m23*this->m32), (this->m13*this->m32 - this->m12*this->m33), (this->m12*this->m23 - this->m13*this->m22),
									   (this->m23*this->m31 - this->m21*this->m33), (this->m11*this->m33 - this->m13*this->m31), (this->m13*this->m21 - this->m11*this->m23),
									   (this->m21*this->m32 - this->m22*this->m31), (this->m12*this->m31 - this->m11*this->m32), (this->m11*this->m22 - this->m12*this->m21) );
	}

	template<typename ElementType>
	inline Matrix3x3<ElementType> Matrix3x3<ElementType>::getTranspose( ) const
	{
		return Matrix3x3<ElementType>( this->m11, this->m21, this->m31,
									   this->m12, this->m22, this->m32,
									   this->m13, this->m23, this->m33 );
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::transpose( )
	{
		ElementType swapSpace;
		Utility::Element::swap( this->m12, this->m21, swapSpace );
		Utility::Element::swap( this->m13, this->m31, swapSpace );
		Utility::Element::swap( this->m23, this->m32, swapSpace );
		return *this;
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> Matrix3x3<ElementType>::getInverse( ) const
	{ return this->getAdjoint() /= this->getDeterminant(); }
	
	template<typename ElementType>
	Matrix3x3<ElementType> Matrix3x3<ElementType>::getInverse( ElementType &determinant ) const
	{
		determinant = this->getDeterminant();
		if( determinant != 0 )
			return this->getAdjoint() /= determinant;
		else return Matrix3x3<ElementType>();
	}
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::invert( )
	{ return *this = this->getAdjoint() /= this->getDeterminant(); }
	
	template<typename ElementType>
	Matrix3x3<ElementType> & Matrix3x3<ElementType>::invert( ElementType &determinant )
	{
		determinant = this->getDeterminant();
		if( determinant != 0 )
			return *this = this->getAdjoint() /= determinant;
		return *this;
	}

	template<typename ElementType>
	inline Vector3<ElementType> Matrix3x3<ElementType>::getRowVector( unsigned int rowID ) const
	{ return Vector3<ElementType>( this->m[0][rowID], this->m[1][rowID], this->m[2][rowID] ); }
	
	template<typename ElementType>
	inline const Vector3<ElementType> & Matrix3x3<ElementType>::getColumnVector( unsigned int colID ) const
	{ return this->v[colID]; }

// Matrix4x4<ElementType> ///////////////////////////////////////
	template<typename ElementType>
	const Matrix4x4<ElementType> Matrix4x4<ElementType>::identity = Matrix4x4<ElementType>( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );

	template<typename ElementType>
	const Matrix4x4<ElementType> Matrix4x4<ElementType>::null = Matrix4x4<ElementType>( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( )
		: m11(0), m21(0), m31(0), m41(0), m12(0), m22(0), m32(0), m42(0), m13(0), m23(0), m33(0), m43(0), m14(0), m24(0), m34(0), m44(0) {}
	
	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( const ElementType &_m11, const ElementType &_m12, const ElementType &_m13, const ElementType &_m14, const ElementType &_m21, const ElementType &_m22, const ElementType &_m23, const ElementType &_m24, const ElementType &_m31, const ElementType &_m32, const ElementType &_m33, const ElementType &_m34, const ElementType &_m41, const ElementType &_m42, const ElementType &_m43, const ElementType &_m44 )
		: m11(_m11), m21(_m21), m31(_m31), m41(_m41), m12(_m12), m22(_m22), m32(_m32), m42(_m42), m13(_m13), m23(_m23), m33(_m33), m43(_m43), m14(_m14), m24(_m24), m34(_m34), m44(_m44) {}
	
	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( const Vector4<ElementType> vec[4] )
		: m11(vec[0].x), m21(vec[0].y), m31(vec[0].z), m41(vec[0].w), m12(vec[1].x), m22(vec[1].y), m32(vec[1].z), m42(vec[1].w), m13(vec[2].x), m23(vec[2].y), m33(vec[2].z), m43(vec[2].w), m14(vec[3].x), m24(vec[3].y), m34(vec[3].z), m44(vec[3].w) {}
	
	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( const Vector4<ElementType> &vec1, const Vector4<ElementType> &vec2, const Vector4<ElementType> &vec3, const Vector4<ElementType> &vec4 )
		: m11(vec1.x), m21(vec1.y), m31(vec1.z), m41(vec1.w), m12(vec2.x), m22(vec2.y), m32(vec2.z), m42(vec2.w), m13(vec3.x), m23(vec3.y), m33(vec3.z), m43(vec3.w), m14(vec4.x), m24(vec4.y), m34(vec4.z), m44(vec4.w) {}

	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( const ElementType _element[16] )
//		: m11(_element[0]), m21(_element[1]), m31(_element[2]), m41(_element[3]), m12(_element[4]), m22(_element[5]), m32(_element[6]), m42(_element[7]), m13(_element[8]), m23(_element[9]), m33(_element[10]), m43(_element[11]), m14(_element[12]), m24(_element[13]), m34(_element[14]), m44(_element[15]) {}
		: m11(_element[0]), m12(_element[1]), m13(_element[2]), m14(_element[3]), m21(_element[4]), m22(_element[5]), m23(_element[6]), m24(_element[7]), m31(_element[8]), m32(_element[9]), m33(_element[10]), m34(_element[11]), m41(_element[12]), m42(_element[13]), m43(_element[14]), m44(_element[15]) {}
	
	template<typename ElementType>
	Matrix4x4<ElementType>::Matrix4x4( const Matrix4x4<ElementType> &matrix )
		: m11(matrix.m11), m21(matrix.m21), m31(matrix.m31), m41(matrix.m41), m12(matrix.m12), m22(matrix.m22), m32(matrix.m32), m42(matrix.m42), m13(matrix.m13), m23(matrix.m23), m33(matrix.m33), m43(matrix.m43), m14(matrix.m14), m24(matrix.m24), m34(matrix.m34), m44(matrix.m44) {}

	template<typename ElementType>
	inline Matrix4x4<ElementType>::operator ElementType* ( )
	{ return this->element; }
	
	template<typename ElementType>
	inline Matrix4x4<ElementType>::operator const ElementType* ( ) const
	{ return this->element; }

	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator = ( const Vector4<ElementType> vec[4] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		this->v[2] = vec[2];
		this->v[3] = vec[3];
		return *this;
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator = ( const ElementType element[16] )
	{
		for( int i = 0; i < 16; ++i )
			this->element[i] = element[i];
		return *this;
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator = ( const Matrix4x4<ElementType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		this->v[2] = matrix.v[2];
		this->v[3] = matrix.v[3];
		return *this;
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator += ( const Matrix4x4<ElementType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		this->v[2] += matrix.v[2];
		this->v[3] += matrix.v[3];
		return *this;
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator -= ( const Matrix4x4<ElementType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		this->v[2] -= matrix.v[2];
		this->v[3] -= matrix.v[3];
		return *this;
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator *= ( const ElementType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		this->v[2] *= scalar;
		this->v[3] *= scalar;
		return *this;
	}

	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::operator /= ( const ElementType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		this->v[2] /= scalar;
		this->v[3] /= scalar;
		return *this;
	}
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::operator + ( const Matrix4x4<ElementType> &matrix ) const
	{ return Matrix4x4<ElementType>(*this) += matrix; }
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::operator - ( const Matrix4x4<ElementType> &matrix ) const
	{ return Matrix4x4<ElementType>(*this) -= matrix; }
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::operator * ( const ElementType &scalar ) const
	{ return Matrix4x4<ElementType>(*this) *= scalar; }
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::operator / ( const ElementType &scalar ) const
	{ return Matrix4x4<ElementType>(*this) /= scalar; }

	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::operator - ( ) const
	{ return Matrix4x4<ElementType>(-this->v[0], -this->v[1], -this->v[2], -this->v[3]); }

	template<typename ElementType>
	ElementType Matrix4x4<ElementType>::getDeterminant( ) const
	{ 
		ElementType determinant = this->m11 * Matrix3x3<ElementType>(this->m22, this->m23, this->m24, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).getDeterminant();
		determinant -= this->m12 * Matrix3x3<ElementType>(this->m21, this->m23, this->m24, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).getDeterminant();
		determinant += this->m13 * Matrix3x3<ElementType>(this->m21, this->m22, this->m24, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).getDeterminant();
		return determinant -= this->m14 * Matrix3x3<ElementType>(this->m21, this->m22, this->m23, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).getDeterminant();
	}

	template<typename ElementType>
	Matrix4x4<ElementType> Matrix4x4<ElementType>::getAdjoint( ) const
	{
		return Matrix4x4<ElementType>(  Matrix3x3<ElementType>(this->m22, this->m23, this->m24, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).getDeterminant(), -Matrix3x3<ElementType>(this->m12, this->m13, this->m14, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).getDeterminant(),  Matrix3x3<ElementType>(this->m12, this->m13, this->m14, this->m22, this->m23, this->m24, this->m42, this->m43, this->m44).getDeterminant(), -Matrix3x3<ElementType>(this->m12, this->m13, this->m14, this->m22, this->m23, this->m24, this->m32, this->m33, this->m34).getDeterminant(),
									   -Matrix3x3<ElementType>(this->m21, this->m23, this->m24, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).getDeterminant(),  Matrix3x3<ElementType>(this->m11, this->m13, this->m14, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).getDeterminant(), -Matrix3x3<ElementType>(this->m11, this->m13, this->m14, this->m21, this->m23, this->m24, this->m41, this->m43, this->m44).getDeterminant(),  Matrix3x3<ElementType>(this->m11, this->m13, this->m14, this->m21, this->m23, this->m24, this->m31, this->m33, this->m34).getDeterminant(),
									    Matrix3x3<ElementType>(this->m21, this->m22, this->m24, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).getDeterminant(), -Matrix3x3<ElementType>(this->m11, this->m12, this->m14, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).getDeterminant(),  Matrix3x3<ElementType>(this->m11, this->m12, this->m14, this->m21, this->m22, this->m24, this->m41, this->m42, this->m44).getDeterminant(), -Matrix3x3<ElementType>(this->m11, this->m12, this->m14, this->m21, this->m22, this->m24, this->m31, this->m32, this->m34).getDeterminant(),
									   -Matrix3x3<ElementType>(this->m21, this->m22, this->m23, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).getDeterminant(),  Matrix3x3<ElementType>(this->m11, this->m12, this->m13, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).getDeterminant(), -Matrix3x3<ElementType>(this->m11, this->m12, this->m13, this->m21, this->m22, this->m23, this->m41, this->m42, this->m43).getDeterminant(),  Matrix3x3<ElementType>(this->m11, this->m12, this->m13, this->m21, this->m22, this->m23, this->m31, this->m32, this->m33).getDeterminant() );
	}
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::getTranspose( ) const
	{
		return Matrix4x4<ElementType>( this->m11, this->m21, this->m31, this->m41,
									   this->m12, this->m22, this->m32, this->m42,
									   this->m13, this->m23, this->m33, this->m43,
									   this->m14, this->m24, this->m34, this->m44 );
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::transpose( )
	{
		ElementType swapSpace;
		::Utility::Element::swap( this->m12, this->m21, swapSpace );
		::Utility::Element::swap( this->m13, this->m31, swapSpace );
		::Utility::Element::swap( this->m14, this->m41, swapSpace );
		::Utility::Element::swap( this->m23, this->m32, swapSpace );
		::Utility::Element::swap( this->m24, this->m42, swapSpace );
		::Utility::Element::swap( this->m34, this->m43, swapSpace );
		return *this;
	}
	
	template<typename ElementType>
	inline Matrix4x4<ElementType> Matrix4x4<ElementType>::getInverse( ) const
	{ return this->getAdjoint() /= this->getDeterminant() ; }
	
	template<typename ElementType>
	Matrix4x4<ElementType> Matrix4x4<ElementType>::getInverse( ElementType &determinant ) const
	{
		determinant = this->getDeterminant();
		if( determinant != 0.0f )
			return this->getAdjoint() /= determinant;
		return Matrix4x4<ElementType>();
	}
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::invert( )
	{ return *this = this->getAdjoint() /= this->getDeterminant(); }
	
	template<typename ElementType>
	Matrix4x4<ElementType> & Matrix4x4<ElementType>::invert( ElementType &determinant )
	{
		determinant = this->getDeterminant();
		if( determinant != 0.0f )
			return *this = this->getAdjoint() /= determinant;
		return *this;
	}

	template<typename ElementType>
	inline Vector4<ElementType> Matrix4x4<ElementType>::getRowVector( unsigned int rowID ) const
	{ return Vector4<ElementType>( this->m[0][rowID], this->m[1][rowID], this->m[2][rowID], this->m[3][rowID] ); }
	
	template<typename ElementType>
	inline const Vector4<ElementType> & Matrix4x4<ElementType>::getColumnVector( unsigned int colID ) const
	{ return this->v[colID]; }
}

#endif