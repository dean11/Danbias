/////////////////////////////////////////////////////////////////////
// Linear Math Matrixes (Column weighted)
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef LINEARALGEBRA_MATRIX_H
#define LINEARALGEBRA_MATRIX_H

#include "Vector.h"
#include "Utilities.h"

namespace LinearAlgebra
{
	template<typename ScalarType>
	struct Matrix2x2
	{
	public:
		union
		{
			ScalarType m[2][2];
			struct{ Vector2<ScalarType> v[2]; };
			struct{ ScalarType m11, m21, m12, m22; };
			ScalarType element[4];
			char byte[sizeof(ScalarType[4])];
		};

		static const Matrix2x2<ScalarType> identity, null;

		Matrix2x2( );
		Matrix2x2( const ScalarType &m11, const ScalarType &m12,
			       const ScalarType &m21, const ScalarType &m22 );
		explicit Matrix2x2( const Vector2<ScalarType> vec[2] );
		Matrix2x2( const Vector2<ScalarType> &vec1, const Vector2<ScalarType> &vec2 );
		explicit Matrix2x2( const ScalarType element[4] );
		Matrix2x2( const Matrix2x2<ScalarType> &matrix );

		operator ScalarType* ( );
		operator const ScalarType* ( ) const;

		Matrix2x2<ScalarType> & operator  = ( const Vector2<ScalarType> vec[2] );
		Matrix2x2<ScalarType> & operator  = ( const Matrix2x2<ScalarType> &matrix );
		Matrix2x2<ScalarType> & operator += ( const Matrix2x2<ScalarType> &matrix );
		Matrix2x2<ScalarType> & operator -= ( const Matrix2x2<ScalarType> &matrix );
		Matrix2x2<ScalarType> & operator *= ( const ScalarType &scalar );
		Matrix2x2<ScalarType> & operator /= ( const ScalarType &scalar );
		Matrix2x2<ScalarType>	 operator  + ( const Matrix2x2<ScalarType> &matrix ) const;
		Matrix2x2<ScalarType>	 operator  - ( const Matrix2x2<ScalarType> &matrix ) const;
		Matrix2x2<ScalarType>	 operator  * ( const ScalarType &scalar ) const;
		Matrix2x2<ScalarType>	 operator  / ( const ScalarType &scalar ) const;
		Matrix2x2<ScalarType>	 operator  - ( ) const; // unary negation

		ScalarType GetDeterminant( ) const;
		Matrix2x2<ScalarType>   GetAdjoint( ) const;
		Matrix2x2<ScalarType>   GetTranspose( ) const;
		Matrix2x2<ScalarType> & Transpose( );
		Matrix2x2<ScalarType>   GetInverse( ) const;
		Matrix2x2<ScalarType>   GetInverse( ScalarType &determinant ) const;
		Matrix2x2<ScalarType> & Invert( );
		Matrix2x2<ScalarType> & Invert( ScalarType &determinant );
		Vector2<ScalarType>	 GetRowVector( unsigned int rowID ) const;
		Vector2<ScalarType>	 GetColumnVector( unsigned int colID ) const;
	};

	template<typename ScalarType>
	struct Matrix3x3
	{
	public:
		union
		{
			ScalarType m[3][3];
			struct{ Vector3<ScalarType> v[3]; };
			struct{ ScalarType m11, m21, m31, m12, m22, m32, m13, m23, m33; };
			ScalarType element[9];
			char byte[sizeof(ScalarType[9])];
		};

		static const Matrix3x3<ScalarType> identity, null;

		Matrix3x3( );
		Matrix3x3( const ScalarType &m11, const ScalarType &m12, const ScalarType &m13,
			       const ScalarType &m21, const ScalarType &m22, const ScalarType &m23,
				   const ScalarType &m31, const ScalarType &m32, const ScalarType &m33 );
		explicit Matrix3x3( const Vector3<ScalarType> vec[3] );
		Matrix3x3( const Vector3<ScalarType> &vec1, const Vector3<ScalarType> &vec2, const Vector3<ScalarType> &vec3 );
		explicit Matrix3x3( const ScalarType element[9] );
		Matrix3x3( const Matrix3x3<ScalarType> &matrix );

		operator ScalarType* ( );
		operator const ScalarType* ( ) const;

		Matrix3x3<ScalarType> & operator  = ( const Vector3<ScalarType> vec[3] );
		Matrix3x3<ScalarType> & operator  = ( const Matrix3x3<ScalarType> &matrix );
		Matrix3x3<ScalarType> & operator += ( const Matrix3x3<ScalarType> &matrix );
		Matrix3x3<ScalarType> & operator -= ( const Matrix3x3<ScalarType> &matrix );
		Matrix3x3<ScalarType> & operator *= ( const ScalarType &scalar );
		Matrix3x3<ScalarType> & operator /= ( const ScalarType &scalar );
		Matrix3x3<ScalarType>	 operator  + ( const Matrix3x3<ScalarType> &matrix ) const;
		Matrix3x3<ScalarType>	 operator  - ( const Matrix3x3<ScalarType> &matrix ) const;
		Matrix3x3<ScalarType>	 operator  * ( const ScalarType &scalar ) const;
		Matrix3x3<ScalarType>	 operator  / ( const ScalarType &scalar ) const;
		Matrix3x3<ScalarType>	 operator  - ( ) const; // unary negation

		ScalarType GetDeterminant( ) const;
		Matrix3x3<ScalarType>   GetAdjoint( ) const;
		Matrix3x3<ScalarType>   GetTranspose( ) const;
		Matrix3x3<ScalarType> & Transpose( );
		Matrix3x3<ScalarType>   GetInverse( ) const;
		Matrix3x3<ScalarType>   GetInverse( ScalarType &determinant ) const;
		Matrix3x3<ScalarType> & Invert( );
		Matrix3x3<ScalarType> & Invert( ScalarType &determinant );
		Vector3<ScalarType>	 GetRowVector( unsigned int rowID ) const;
		Vector3<ScalarType>	 GetColumnVector( unsigned int colID ) const;
	};

	template<typename ScalarType>
	struct Matrix4x4
	{
	public:
		union
		{
			ScalarType m[4][4];
			struct{ Vector4<ScalarType> v[4]; };
			struct{ ScalarType m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44; };
			ScalarType element[16];
			char byte[sizeof(ScalarType[16])];
		};

		static const Matrix4x4<ScalarType> identity, null;

		Matrix4x4( );
		Matrix4x4( const ScalarType &m11, const ScalarType &m12, const ScalarType &m13, const ScalarType &m14,
			       const ScalarType &m21, const ScalarType &m22, const ScalarType &m23, const ScalarType &m24,
				   const ScalarType &m31, const ScalarType &m32, const ScalarType &m33, const ScalarType &m34,
				   const ScalarType &m41, const ScalarType &m42, const ScalarType &m43, const ScalarType &m44 );
		explicit Matrix4x4( const Vector4<ScalarType> vec[4] );
		Matrix4x4( const Vector4<ScalarType> &vec1, const Vector4<ScalarType> &vec2, const Vector4<ScalarType> &vec3, const Vector4<ScalarType> &vec4 );
		explicit Matrix4x4( const ScalarType element[16] );
		Matrix4x4( const Matrix4x4<ScalarType> &matrix );

		operator ScalarType* ( );
		operator const ScalarType* ( ) const;

		Matrix4x4<ScalarType> & operator  = ( const Vector4<ScalarType> vec[4] );
		Matrix4x4<ScalarType> & operator  = ( const Matrix4x4<ScalarType> &matrix );
		Matrix4x4<ScalarType> & operator += ( const Matrix4x4<ScalarType> &matrix );
		Matrix4x4<ScalarType> & operator -= ( const Matrix4x4<ScalarType> &matrix );
		Matrix4x4<ScalarType> & operator *= ( const ScalarType &scalar );
		Matrix4x4<ScalarType> & operator /= ( const ScalarType &scalar );
		Matrix4x4<ScalarType>	 operator  + ( const Matrix4x4<ScalarType> &matrix ) const;
		Matrix4x4<ScalarType>	 operator  - ( const Matrix4x4<ScalarType> &matrix ) const;
		Matrix4x4<ScalarType>	 operator  * ( const ScalarType &scalar ) const;
		Matrix4x4<ScalarType>	 operator  / ( const ScalarType &scalar ) const;
		Matrix4x4<ScalarType>	 operator  - ( ) const; // unary negation

		ScalarType GetDeterminant( ) const;
		Matrix4x4<ScalarType>   GetAdjoint( ) const;
		Matrix4x4<ScalarType>   GetTranspose( ) const;
		Matrix4x4<ScalarType> & Transpose( );
		Matrix4x4<ScalarType>   GetInverse( ) const;
		Matrix4x4<ScalarType>   GetInverse( ScalarType &determinant ) const;
		Matrix4x4<ScalarType> & Invert( );
		Matrix4x4<ScalarType> & Invert( ScalarType &determinant );
		Vector4<ScalarType>		 GetRowVector( unsigned int rowID ) const;
		const Vector4<ScalarType> & GetColumnVector( unsigned int colID ) const;
	};
}

template<typename ScalarType> LinearAlgebra::Matrix2x2<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix2x2<ScalarType> &right );
template<typename ScalarType> LinearAlgebra::Matrix3x3<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix3x3<ScalarType> &right );
template<typename ScalarType> LinearAlgebra::Matrix4x4<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix4x4<ScalarType> &right );

///////////////////////////////////////////////////////////////////////////////////
//	Body
///////////////////////////////////////////////////////////////////////////////////

namespace LinearAlgebra
{
// Matrix2x2<ScalarType> ///////////////////////////////////////

	template<typename ScalarType>
	const Matrix2x2<ScalarType> Matrix2x2<ScalarType>::identity = Matrix2x2<ScalarType>( 1, 0, 0, 1 );

	template<typename ScalarType>
	const Matrix2x2<ScalarType> Matrix2x2<ScalarType>::null = Matrix2x2<ScalarType>( 0, 0, 0, 0 );

	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( ) : m11(), m21(), m12(), m22() {}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( const ScalarType &_m11, const ScalarType &_m12, const ScalarType &_m21, const ScalarType &_m22 )
	{
		this->m11 = _m11; this->m12 = _m12;
		this->m21 = _m21; this->m22 = _m22;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( const Vector2<ScalarType> vec[2] )
	{ this->v[0] = vec[0]; this->v[1] = vec[1]; }

	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( const Vector2<ScalarType> &vec1, const Vector2<ScalarType> &vec2 )
	{ this->v[0] = vec1; this->v[1] = vec2; }

	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( const ScalarType _element[4] )
	{
		this->m11 = _element[0]; this->m12 = _element[1];
		this->m21 = _element[2]; this->m22 = _element[3];
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType>::Matrix2x2( const Matrix2x2<ScalarType> &matrix )
	{ this->v[0] = matrix.v[0]; this->v[1] = matrix.v[1]; }

	template<typename ScalarType>
	inline Matrix2x2<ScalarType>::operator ScalarType* ( )
	{ return this->element; }
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType>::operator const ScalarType* ( ) const
	{ return this->element; }

	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator = ( const Vector2<ScalarType> vec[2] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator = ( const Matrix2x2<ScalarType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator += ( const Matrix2x2<ScalarType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator -= ( const Matrix2x2<ScalarType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator *= ( const ScalarType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		return *this;
	}

	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		return *this;
	}
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator + ( const Matrix2x2<ScalarType> &matrix ) const
	{ return Matrix2x2<ScalarType>(*this) += matrix; }
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator - ( const Matrix2x2<ScalarType> &matrix ) const
	{ return Matrix2x2<ScalarType>(*this) -= matrix; }
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Matrix2x2<ScalarType>(*this) *= scalar; }
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Matrix2x2<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator - ( ) const
	{ return Matrix2x2<ScalarType>(-this->v[0], -this->v[1]); }

	template<typename ScalarType>
	ScalarType Matrix2x2<ScalarType>::GetDeterminant( ) const
	{
		ScalarType determinant = (this->m11 * this->m22);
		return determinant -= (this->m12 * this->m21);
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> Matrix2x2<ScalarType>::GetAdjoint( ) const
	{ return Matrix2x2<ScalarType>( this->m22, -this->m21, -this->m12, this->m11 ); }

	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::GetTranspose( ) const
	{ return Matrix2x2<ScalarType>( this->element[0], this->element[1], this->element[2], this->element[3] ); }
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::Transpose( )
	{
		ScalarType swapSpace;
		::Utility::Element::Swap( this->m12, this->m21, swapSpace );
		return *this;
	}
	
	template<typename ScalarType>
	inline Matrix2x2<ScalarType> Matrix2x2<ScalarType>::GetInverse( ) const
	{ return this->GetAdjoint() /= this->GetDeterminant(); }
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> Matrix2x2<ScalarType>::GetInverse( ScalarType &determinant ) const
	{
		determinant = this->GetDeterminant();
		if( determinant != 0 )
			return this->GetAdjoint() / determinant;
		return Matrix2x2<ScalarType>::null;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::Invert( )
	{ 
		*this /= this->GetDeterminant();
		this->m12 *= -1; this->m21 *= -1;

		ScalarType swapSpace;
		Utility::Element::Swap( this->m12, this->m21, swapSpace );
		Utility::Element::Swap( this->m11, this->m22, swapSpace );
		return *this;
	}
	
	template<typename ScalarType>
	Matrix2x2<ScalarType> & Matrix2x2<ScalarType>::Invert( ScalarType &determinant )
	{
		determinant = this->GetDeterminant();

		if( determinant != 0 )
		{
			*this /= determinant;
			this->m12 *= -1; this->m21 *= -1;

			ScalarType swapSpace;
			Utility::Element::Swap( this->m12, this->m21, swapSpace );
			Utility::Element::Swap( this->m11, this->m22, swapSpace );
		}
		return *this;
	}

	template<typename ScalarType>
	inline Vector2<ScalarType> Matrix2x2<ScalarType>::GetRowVector( unsigned int rowID ) const
	{ return Vector2<ScalarType>( this->m[0][rowID], this->m[1][rowID] ); }
	
	template<typename ScalarType>
	inline Vector2<ScalarType> Matrix2x2<ScalarType>::GetColumnVector( unsigned int colID ) const
	{ return this->v[colID]; }

// Matrix3x3<ScalarType> ///////////////////////////////////////

	template<typename ScalarType>
	const Matrix3x3<ScalarType> Matrix3x3<ScalarType>::identity = Matrix3x3<ScalarType>( 1, 0, 0, 0, 1, 0, 0, 0, 1 );

	template<typename ScalarType>
	const Matrix3x3<ScalarType> Matrix3x3<ScalarType>::null = Matrix3x3<ScalarType>( 0, 0, 0, 0, 0, 0, 0, 0, 0 );

	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( ): m11(), m21(), m31(), m12(), m22(), m32(), m13(), m23(), m33() {}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( const ScalarType &_m11, const ScalarType &_m12, const ScalarType &_m13, const ScalarType &_m21, const ScalarType &_m22, const ScalarType &_m23, const ScalarType &_m31, const ScalarType &_m32, const ScalarType &_m33 )
	{
		this->m11 = _m11; this->m12 = _m12; this->m13 = _m13;
		this->m21 = _m21; this->m22 = _m22; this->m23 = _m23;
		this->m31 = _m31; this->m32 = _m32; this->m33 = _m33;
	}

	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( const Vector3<ScalarType> vec[3] )
	{ this->v[0] = vec[0]; this->v[1] = vec[1]; this->v[2] = vec[2]; }
	
	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( const Vector3<ScalarType> &vec1, const Vector3<ScalarType> &vec2, const Vector3<ScalarType> &vec3 )
	{ this->v[0] = vec1; this->v[1] = vec2; this->v[2] = vec3; }
	
	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( const ScalarType _element[9] )
	{
		this->m11 = _element[0]; this->m12 = _element[1]; this->m13 = _element[2];
		this->m21 = _element[3]; this->m22 = _element[4]; this->m23 = _element[5];
		this->m31 = _element[6]; this->m32 = _element[7]; this->m33 = _element[8];
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType>::Matrix3x3( const Matrix3x3<ScalarType> &matrix )
	{ this->v[0] = matrix.v[0]; this->v[1] = matrix.v[1]; this->v[2] = matrix.v[2]; }

	template<typename ScalarType>
	inline Matrix3x3<ScalarType>::operator ScalarType* ( )
	{ return this->element; }
	
	template<typename ScalarType>
	inline Matrix3x3<ScalarType>::operator const ScalarType* ( ) const
	{ return this->element; }

	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator = ( const Vector3<ScalarType> vec[3] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		this->v[2] = vec[2];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator = ( const Matrix3x3<ScalarType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		this->v[2] = matrix.v[2];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator += ( const Matrix3x3<ScalarType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		this->v[2] += matrix.v[2];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator -= ( const Matrix3x3<ScalarType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		this->v[2] -= matrix.v[2];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator *= ( const ScalarType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		this->v[2] *= scalar;
		return *this;
	}

	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		this->v[2] /= scalar;
		return *this;
	}
	
	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::operator + ( const Matrix3x3<ScalarType> &matrix ) const
	{ return Matrix3x3<ScalarType>(*this) += matrix; }
	
	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::operator - ( const Matrix3x3<ScalarType> &matrix ) const
	{ return Matrix3x3<ScalarType>(*this) -= matrix; }
		
	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Matrix3x3<ScalarType>(*this) *= scalar; }
	
	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Matrix3x3<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::operator - ( ) const
	{ return Matrix3x3<ScalarType>(-this->v[0], -this->v[1], -this->v[2]); }

	template<typename ScalarType>
	ScalarType Matrix3x3<ScalarType>::GetDeterminant( ) const
	{ 
		ScalarType determinant = (this->m11 * this->m22 * this->m33);
		determinant += (this->m12 * this->m23 * this->m31);
		determinant += (this->m13 * this->m21 * this->m32);
		determinant -= (this->m11 * this->m23 * this->m32);
		determinant -= (this->m12 * this->m21 * this->m33);
		return determinant -= (this->m13 * this->m22 * this->m31);
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> Matrix3x3<ScalarType>::GetAdjoint( ) const
	{
		return Matrix3x3<ScalarType>( (this->m22*this->m33 - this->m23*this->m32), (this->m13*this->m32 - this->m12*this->m33), (this->m12*this->m23 - this->m13*this->m22),
									   (this->m23*this->m31 - this->m21*this->m33), (this->m11*this->m33 - this->m13*this->m31), (this->m13*this->m21 - this->m11*this->m23),
									   (this->m21*this->m32 - this->m22*this->m31), (this->m12*this->m31 - this->m11*this->m32), (this->m11*this->m22 - this->m12*this->m21) );
	}

	template<typename ScalarType>
	inline Matrix3x3<ScalarType> Matrix3x3<ScalarType>::GetTranspose( ) const
	{
		return Matrix3x3<ScalarType>( this->m11, this->m21, this->m31,
									   this->m12, this->m22, this->m32,
									   this->m13, this->m23, this->m33 );
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::Transpose( )
	{
		ScalarType swapSpace;
		Utility::Element::Swap( this->m12, this->m21, swapSpace );
		Utility::Element::Swap( this->m13, this->m31, swapSpace );
		Utility::Element::Swap( this->m23, this->m32, swapSpace );
		return *this;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> Matrix3x3<ScalarType>::GetInverse( ) const
	{ return this->GetAdjoint() /= this->GetDeterminant(); }
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> Matrix3x3<ScalarType>::GetInverse( ScalarType &determinant ) const
	{
		determinant = this->GetDeterminant();
		if( determinant != 0 )
			return this->GetAdjoint() /= determinant;
		else return Matrix3x3<ScalarType>::null;
	}
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::Invert( )
	{ return *this = this->GetAdjoint() /= this->GetDeterminant(); }
	
	template<typename ScalarType>
	Matrix3x3<ScalarType> & Matrix3x3<ScalarType>::Invert( ScalarType &determinant )
	{
		determinant = this->GetDeterminant();
		if( determinant != 0 )
			return *this = this->GetAdjoint() /= determinant;
		return *this;
	}

	template<typename ScalarType>
	inline Vector3<ScalarType> Matrix3x3<ScalarType>::GetRowVector( unsigned int rowID ) const
	{ return Vector3<ScalarType>( this->m[0][rowID], this->m[1][rowID], this->m[2][rowID] ); }
	
	template<typename ScalarType>
	inline Vector3<ScalarType> Matrix3x3<ScalarType>::GetColumnVector( unsigned int colID ) const
	{ return this->v[colID]; }

// Matrix4x4<ScalarType> ///////////////////////////////////////

	template<typename ScalarType>
	const Matrix4x4<ScalarType> Matrix4x4<ScalarType>::identity = Matrix4x4<ScalarType>( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
	
	template<typename ScalarType>
	const Matrix4x4<ScalarType> Matrix4x4<ScalarType>::null = Matrix4x4<ScalarType>( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( )
		: m11(), m21(), m31(), m41(), m12(), m22(), m32(), m42(),
		  m13(), m23(), m33(), m43(), m14(), m24(), m34(), m44() {}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( const ScalarType &_m11, const ScalarType &_m12, const ScalarType &_m13, const ScalarType &_m14, const ScalarType &_m21, const ScalarType &_m22, const ScalarType &_m23, const ScalarType &_m24, const ScalarType &_m31, const ScalarType &_m32, const ScalarType &_m33, const ScalarType &_m34, const ScalarType &_m41, const ScalarType &_m42, const ScalarType &_m43, const ScalarType &_m44 )
	{
		this->m11 = _m11; this->m12 = _m12; this->m13 = _m13; this->m14 = _m14;
		this->m21 = _m21; this->m22 = _m22; this->m23 = _m23; this->m24 = _m24;
		this->m31 = _m31; this->m32 = _m32; this->m33 = _m33; this->m34 = _m34;
		this->m41 = _m41; this->m42 = _m42; this->m43 = _m43; this->m44 = _m44;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( const Vector4<ScalarType> vec[4] )
	{ this->v[0] = vec[0]; this->v[1] = vec[1]; this->v[2] = vec[2]; this->v[3] = vec[3]; }
	
	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( const Vector4<ScalarType> &vec1, const Vector4<ScalarType> &vec2, const Vector4<ScalarType> &vec3, const Vector4<ScalarType> &vec4 )
	{ this->v[0] = vec1; this->v[1] = vec2; this->v[2] = vec3; this->v[3] = vec4; }

	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( const ScalarType _element[16] )
	{
		this->m11 = _element[0];  this->m12 = _element[1];  this->m13 = _element[2];  this->m14 = _element[3];
		this->m21 = _element[4];  this->m22 = _element[5];  this->m23 = _element[6];  this->m24 = _element[7];
		this->m31 = _element[8];  this->m32 = _element[9];  this->m33 = _element[10]; this->m34 = _element[11];
		this->m41 = _element[12]; this->m42 = _element[13]; this->m43 = _element[14]; this->m44 = _element[15];
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType>::Matrix4x4( const Matrix4x4<ScalarType> &matrix )
	{ this->v[0] = matrix.v[0]; this->v[1] = matrix.v[1]; this->v[2] = matrix.v[2]; this->v[3] = matrix.v[3]; }

	template<typename ScalarType>
	inline Matrix4x4<ScalarType>::operator ScalarType* ( )
	{ return this->element; }
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType>::operator const ScalarType* ( ) const
	{ return this->element; }

	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator = ( const Vector4<ScalarType> vec[4] )
	{
		this->v[0] = vec[0];
		this->v[1] = vec[1];
		this->v[2] = vec[2];
		this->v[3] = vec[3];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator = ( const Matrix4x4<ScalarType> &matrix )
	{
		this->v[0] = matrix.v[0];
		this->v[1] = matrix.v[1];
		this->v[2] = matrix.v[2];
		this->v[3] = matrix.v[3];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator += ( const Matrix4x4<ScalarType> &matrix )
	{
		this->v[0] += matrix.v[0];
		this->v[1] += matrix.v[1];
		this->v[2] += matrix.v[2];
		this->v[3] += matrix.v[3];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator -= ( const Matrix4x4<ScalarType> &matrix )	
	{
		this->v[0] -= matrix.v[0];
		this->v[1] -= matrix.v[1];
		this->v[2] -= matrix.v[2];
		this->v[3] -= matrix.v[3];
		return *this;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator *= ( const ScalarType &scalar )	
	{
		this->v[0] *= scalar;
		this->v[1] *= scalar;
		this->v[2] *= scalar;
		this->v[3] *= scalar;
		return *this;
	}

	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::operator /= ( const ScalarType &scalar )
	{
		this->v[0] /= scalar;
		this->v[1] /= scalar;
		this->v[2] /= scalar;
		this->v[3] /= scalar;
		return *this;
	}
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator + ( const Matrix4x4<ScalarType> &matrix ) const
	{ return Matrix4x4<ScalarType>(*this) += matrix; }
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator - ( const Matrix4x4<ScalarType> &matrix ) const
	{ return Matrix4x4<ScalarType>(*this) -= matrix; }
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator * ( const ScalarType &scalar ) const
	{ return Matrix4x4<ScalarType>(*this) *= scalar; }
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator / ( const ScalarType &scalar ) const
	{ return Matrix4x4<ScalarType>(*this) /= scalar; }

	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator - ( ) const
	{ return Matrix4x4<ScalarType>(-this->v[0], -this->v[1], -this->v[2], -this->v[3]); }

	template<typename ScalarType>
	ScalarType Matrix4x4<ScalarType>::GetDeterminant( ) const
	{ 
		ScalarType determinant = this->m11 * Matrix3x3<ScalarType>(this->m22, this->m23, this->m24, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).GetDeterminant();
		determinant -= this->m12 * Matrix3x3<ScalarType>(this->m21, this->m23, this->m24, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).GetDeterminant();
		determinant += this->m13 * Matrix3x3<ScalarType>(this->m21, this->m22, this->m24, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).GetDeterminant();
		return determinant -= this->m14 * Matrix3x3<ScalarType>(this->m21, this->m22, this->m23, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).GetDeterminant();
	}

	template<typename ScalarType>
	Matrix4x4<ScalarType> Matrix4x4<ScalarType>::GetAdjoint( ) const
	{
		return Matrix4x4<ScalarType>(  Matrix3x3<ScalarType>(this->m22, this->m23, this->m24, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).GetDeterminant(), -Matrix3x3<ScalarType>(this->m12, this->m13, this->m14, this->m32, this->m33, this->m34, this->m42, this->m43, this->m44).GetDeterminant(),  Matrix3x3<ScalarType>(this->m12, this->m13, this->m14, this->m22, this->m23, this->m24, this->m42, this->m43, this->m44).GetDeterminant(), -Matrix3x3<ScalarType>(this->m12, this->m13, this->m14, this->m22, this->m23, this->m24, this->m32, this->m33, this->m34).GetDeterminant(),
									  -Matrix3x3<ScalarType>(this->m21, this->m23, this->m24, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).GetDeterminant(),  Matrix3x3<ScalarType>(this->m11, this->m13, this->m14, this->m31, this->m33, this->m34, this->m41, this->m43, this->m44).GetDeterminant(), -Matrix3x3<ScalarType>(this->m11, this->m13, this->m14, this->m21, this->m23, this->m24, this->m41, this->m43, this->m44).GetDeterminant(),  Matrix3x3<ScalarType>(this->m11, this->m13, this->m14, this->m21, this->m23, this->m24, this->m31, this->m33, this->m34).GetDeterminant(),
									   Matrix3x3<ScalarType>(this->m21, this->m22, this->m24, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).GetDeterminant(), -Matrix3x3<ScalarType>(this->m11, this->m12, this->m14, this->m31, this->m32, this->m34, this->m41, this->m42, this->m44).GetDeterminant(),  Matrix3x3<ScalarType>(this->m11, this->m12, this->m14, this->m21, this->m22, this->m24, this->m41, this->m42, this->m44).GetDeterminant(), -Matrix3x3<ScalarType>(this->m11, this->m12, this->m14, this->m21, this->m22, this->m24, this->m31, this->m32, this->m34).GetDeterminant(),
									  -Matrix3x3<ScalarType>(this->m21, this->m22, this->m23, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).GetDeterminant(),  Matrix3x3<ScalarType>(this->m11, this->m12, this->m13, this->m31, this->m32, this->m33, this->m41, this->m42, this->m43).GetDeterminant(), -Matrix3x3<ScalarType>(this->m11, this->m12, this->m13, this->m21, this->m22, this->m23, this->m41, this->m42, this->m43).GetDeterminant(),  Matrix3x3<ScalarType>(this->m11, this->m12, this->m13, this->m21, this->m22, this->m23, this->m31, this->m32, this->m33).GetDeterminant() );
	}
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::GetTranspose( ) const
	{
		return Matrix4x4<ScalarType>( this->m11, this->m21, this->m31, this->m41,
									  this->m12, this->m22, this->m32, this->m42,
									  this->m13, this->m23, this->m33, this->m43,
									  this->m14, this->m24, this->m34, this->m44 );
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::Transpose( )
	{
		ScalarType swapSpace;
		::Utility::Element::Swap( this->m12, this->m21, swapSpace );
		::Utility::Element::Swap( this->m13, this->m31, swapSpace );
		::Utility::Element::Swap( this->m14, this->m41, swapSpace );
		::Utility::Element::Swap( this->m23, this->m32, swapSpace );
		::Utility::Element::Swap( this->m24, this->m42, swapSpace );
		::Utility::Element::Swap( this->m34, this->m43, swapSpace );
		return *this;
	}
	
	template<typename ScalarType>
	inline Matrix4x4<ScalarType> Matrix4x4<ScalarType>::GetInverse( ) const
	{ return this->GetAdjoint() /= this->GetDeterminant() ; }
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> Matrix4x4<ScalarType>::GetInverse( ScalarType &determinant ) const
	{
		determinant = this->GetDeterminant();
		if( determinant != 0.0f )
			return this->GetAdjoint() /= determinant;
		return Matrix4x4<ScalarType>::null;
	}
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::Invert( )
	{ return *this = this->GetAdjoint() /= this->GetDeterminant(); }
	
	template<typename ScalarType>
	Matrix4x4<ScalarType> & Matrix4x4<ScalarType>::Invert( ScalarType &determinant )
	{
		determinant = this->GetDeterminant();
		if( determinant != 0.0f )
			return *this = this->GetAdjoint() /= determinant;
		return *this;
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> Matrix4x4<ScalarType>::GetRowVector( unsigned int rowID ) const
	{ return Vector4<ScalarType>( this->m[0][rowID], this->m[1][rowID], this->m[2][rowID], this->m[3][rowID] ); }
	
	template<typename ScalarType>
	inline const Vector4<ScalarType> & Matrix4x4<ScalarType>::GetColumnVector( unsigned int colID ) const
	{ return this->v[colID]; }
}

template<typename ScalarType>
inline LinearAlgebra::Matrix2x2<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix2x2<ScalarType> &right )
{
	return right * left;
}

template<typename ScalarType>
inline LinearAlgebra::Matrix3x3<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix3x3<ScalarType> &right )
{
	return right * left;
}

template<typename ScalarType>
inline LinearAlgebra::Matrix4x4<ScalarType> operator * ( const ScalarType &left, const LinearAlgebra::Matrix4x4<ScalarType> &right )
{
	return right * left;
}

#endif