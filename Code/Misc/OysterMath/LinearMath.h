/////////////////////////////////////////////////////////////////////
// Collection of Linear Math Stuff
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef LINEARMATH_H
#define LINEARMATH_H

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

// x2

template<typename ScalarType>
inline ::LinearAlgebra::Matrix2x2<ScalarType> operator * ( const ::LinearAlgebra::Matrix2x2<ScalarType> &left, const ::LinearAlgebra::Matrix2x2<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix2x2<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21),
												   (left.m11 * right.m12) + (left.m12 * right.m22),
												   (left.m21 * right.m11) + (left.m22 * right.m21),
												   (left.m21 * right.m12) + (left.m22 * right.m22) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector2<ScalarType> operator * ( const ::LinearAlgebra::Matrix2x2<ScalarType> &matrix, const ::LinearAlgebra::Vector2<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector2<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector2<ScalarType> operator * ( const ::LinearAlgebra::Vector2<ScalarType> &vector, const ::LinearAlgebra::Matrix2x2<ScalarType> &left )
{
	return ::LinearAlgebra::Vector2<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) );
}

// x3

template<typename ScalarType>
inline ::LinearAlgebra::Matrix3x3<ScalarType> operator * ( const ::LinearAlgebra::Matrix3x3<ScalarType> &left, const ::LinearAlgebra::Matrix3x3<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix3x3<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21) + (left.m13 * right.m31), (left.m11 * right.m12) + (left.m12 * right.m22) + (left.m13 * right.m32), (left.m11 * right.m13) + (left.m12 * right.m23) + (left.m13 * right.m33),
												   (left.m21 * right.m11) + (left.m22 * right.m21) + (left.m23 * right.m31), (left.m21 * right.m12) + (left.m22 * right.m22) + (left.m23 * right.m32), (left.m21 * right.m13) + (left.m22 * right.m23) + (left.m23 * right.m33),
												   (left.m31 * right.m11) + (left.m32 * right.m21) + (left.m33 * right.m31), (left.m31 * right.m12) + (left.m32 * right.m22) + (left.m33 * right.m32), (left.m31 * right.m13) + (left.m32 * right.m23) + (left.m33 * right.m33) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector3<ScalarType> operator * ( const ::LinearAlgebra::Matrix3x3<ScalarType> &matrix, const ::LinearAlgebra::Vector3<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector3<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y) + (matrix.m13 * vector.z),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) + (matrix.m23 * vector.z),
												 (matrix.m31 * vector.x) + (matrix.m32 * vector.y) + (matrix.m33 * vector.z) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector3<ScalarType> operator * ( const ::LinearAlgebra::Vector3<ScalarType> &vector, const ::LinearAlgebra::Matrix3x3<ScalarType> &left )
{
	return ::LinearAlgebra::Vector3<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21) + (vector.z * matrix.m31),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) + (vector.z * matrix.m32),
												 (vector.x * matrix.m13) + (vector.y * matrix.m23) + (vector.z * matrix.m33) );
}

// x4

template<typename ScalarType>
inline ::LinearAlgebra::Matrix4x4<ScalarType> operator * ( const ::LinearAlgebra::Matrix4x4<ScalarType> &left, const ::LinearAlgebra::Matrix4x4<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix4x4<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21) + (left.m13 * right.m31) + (left.m14 * right.m41), (left.m11 * right.m12) + (left.m12 * right.m22) + (left.m13 * right.m32) + (left.m14 * right.m42), (left.m11 * right.m13) + (left.m12 * right.m23) + (left.m13 * right.m33) + (left.m14 * right.m43), (left.m11 * right.m14) + (left.m12 * right.m24) + (left.m13 * right.m34) + (left.m14 * right.m44),
												   (left.m21 * right.m11) + (left.m22 * right.m21) + (left.m23 * right.m31) + (left.m24 * right.m41), (left.m21 * right.m12) + (left.m22 * right.m22) + (left.m23 * right.m32) + (left.m24 * right.m42), (left.m21 * right.m13) + (left.m22 * right.m23) + (left.m23 * right.m33) + (left.m24 * right.m43), (left.m21 * right.m14) + (left.m22 * right.m24) + (left.m23 * right.m34) + (left.m24 * right.m44),
												   (left.m31 * right.m11) + (left.m32 * right.m21) + (left.m33 * right.m31) + (left.m34 * right.m41), (left.m31 * right.m12) + (left.m32 * right.m22) + (left.m33 * right.m32) + (left.m34 * right.m42), (left.m31 * right.m13) + (left.m32 * right.m23) + (left.m33 * right.m33) + (left.m34 * right.m43), (left.m31 * right.m14) + (left.m32 * right.m24) + (left.m33 * right.m34) + (left.m34 * right.m44),
												   (left.m41 * right.m11) + (left.m42 * right.m21) + (left.m43 * right.m31) + (left.m44 * right.m41), (left.m41 * right.m12) + (left.m42 * right.m22) + (left.m43 * right.m32) + (left.m44 * right.m42), (left.m41 * right.m13) + (left.m42 * right.m23) + (left.m43 * right.m33) + (left.m44 * right.m43), (left.m41 * right.m14) + (left.m42 * right.m24) + (left.m43 * right.m34) + (left.m44 * right.m44) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector4<ScalarType> operator * ( const ::LinearAlgebra::Matrix4x4<ScalarType> &matrix, const ::LinearAlgebra::Vector4<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector4<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y) + (matrix.m13 * vector.z) + (matrix.m14 * vector.w),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) + (matrix.m23 * vector.z) + (matrix.m24 * vector.w),
												 (matrix.m31 * vector.x) + (matrix.m32 * vector.y) + (matrix.m33 * vector.z) + (matrix.m34 * vector.w),
												 (matrix.m41 * vector.x) + (matrix.m42 * vector.y) + (matrix.m43 * vector.z) + (matrix.m44 * vector.w) );
}

template<typename ScalarType>
inline ::LinearAlgebra::Vector4<ScalarType> operator * ( const ::LinearAlgebra::Vector4<ScalarType> &vector, const ::LinearAlgebra::Matrix4x4<ScalarType> &matrix )
{
	return ::LinearAlgebra::Vector4<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21) + (vector.z * matrix.m31) + (vector.w * matrix.m41),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) + (vector.z * matrix.m32) + (vector.w * matrix.m42),
												 (vector.x * matrix.m13) + (vector.y * matrix.m23) + (vector.z * matrix.m33) + (vector.w * matrix.m43),
												 (vector.x * matrix.m14) + (vector.y * matrix.m24) + (vector.z * matrix.m34) + (vector.w * matrix.m44) );
}

namespace std
{
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> asin( const ::LinearAlgebra::Vector2<ScalarType> &vec )
	{
		return ::LinearAlgebra::Vector2<ScalarType>( asin(vec.x), asin(vec.y) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> asin( const ::LinearAlgebra::Vector3<ScalarType> &vec )
	{
		return ::LinearAlgebra::Vector3<ScalarType>( asin(vec.x), asin(vec.y), asin(vec.z) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> asin( const ::LinearAlgebra::Vector4<ScalarType> &vec )
	{
		return ::LinearAlgebra::Vector4<ScalarType>( asin(vec.x), asin(vec.y), asin(vec.z), asin(vec.w) );
	}

	/*******************************************************************
	 * @param numerator of the vector vec
	 * @return the denomiator of the vector vec.
	 *******************************************************************/
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> modf( const ::LinearAlgebra::Vector2<ScalarType> &vec, ::LinearAlgebra::Vector2<ScalarType> &numerator )
	{
		::LinearAlgebra::Vector2<ScalarType> denominator;
		denominator.x = (ScalarType)modf( vec.x, &numerator.x );
		denominator.y = (ScalarType)modf( vec.y, &numerator.y );
		return denominator;
	}

	/*******************************************************************
	 * @param integer part of the elements in vector vec
	 * @return the fract part of the elements in vector vec.
	 *******************************************************************/
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> modf( const ::LinearAlgebra::Vector3<ScalarType> &vec, ::LinearAlgebra::Vector3<ScalarType> &integer )
	{
		::LinearAlgebra::Vector3<ScalarType> fract;
		fract.x = (ScalarType)modf( vec.x, &integer.x );
		fract.y = (ScalarType)modf( vec.y, &integer.y );
		fract.z = (ScalarType)modf( vec.z, &integer.z );
		return fract;
	}

	/*******************************************************************
	 * @param numerator of the vector vec
	 * @return the denomiator of the vector vec.
	 *******************************************************************/
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> modf( const ::LinearAlgebra::Vector4<ScalarType> &vec, ::LinearAlgebra::Vector4<ScalarType> &numerator )
	{
		::LinearAlgebra::Vector4<ScalarType> denominator;
		denominator.x = (ScalarType)modf( vec.x, &numerator.x );
		denominator.y = (ScalarType)modf( vec.y, &numerator.y );
		denominator.z = (ScalarType)modf( vec.z, &numerator.z );
		denominator.w = (ScalarType)modf( vec.w, &numerator.w );
		return denominator;
	}
}

namespace LinearAlgebra
{
	// Creates a solution matrix for 'out´= 'targetMem' * 'in'.
	// Returns false if there is no explicit solution.
	template<typename ScalarType>
	bool SuperpositionMatrix( const Matrix2x2<ScalarType> &in, const Matrix2x2<ScalarType> &out, Matrix2x2<ScalarType> &targetMem )
	{
		ScalarType d = in.GetDeterminant();
		if( d == 0 ) return false;
		targetMem = out * (in.GetAdjoint() /= d);
		return true;
	}

	// Creates a solution matrix for 'out´= 'targetMem' * 'in'.
	// Returns false if there is no explicit solution.
	template<typename ScalarType>
	bool SuperpositionMatrix( const Matrix3x3<ScalarType> &in, const Matrix3x3<ScalarType> &out, Matrix3x3<ScalarType> &targetMem )
	{
		ScalarType d = in.GetDeterminant();
		if( d == 0 ) return false;
		targetMem = out * (in.GetAdjoint() /= d);
		return true;
	}

	// Creates a solution matrix for 'out´= 'targetMem' * 'in'.
	// Returns false if there is no explicit solution.
	template<typename ScalarType>
	bool SuperpositionMatrix( const Matrix4x4<ScalarType> &in, const Matrix4x4<ScalarType> &out,  Matrix4x4<ScalarType> &targetMem )
	{
		ScalarType d = in.GetDeterminant();
		if( d == 0 ) return false;
		targetMem = out * (in.GetAdjoint() /= d);
		return true;
	}

	/********************************************************************
	 * Linear Interpolation
	 * @return start * (1-t) + end * t
	 ********************************************************************/
	template<typename PointType, typename ScalarType>
	inline PointType Lerp( const PointType &start, const PointType &end,  const ScalarType &t )
	{
		return end * t + start * ( 1 - t );
	}

	/********************************************************************
	 * Normalized Linear Interpolation
	 * @return nullvector if Lerp( start, end, t ) is nullvector.
	 ********************************************************************/
	template<typename ScalarType>
	inline Vector2<ScalarType> Nlerp( const Vector2<ScalarType> &start, const Vector2<ScalarType> &end,  const ScalarType &t )
	{
		Vector2<ScalarType> output = Lerp( start, end, t );
		ScalarType magnitudeSquared = output.Dot( output );
		if( magnitudeSquared != 0 )
		{
			return output /= (ScalarType)::std::sqrt( magnitudeSquared );
		}		
		return output; // error: returning nullvector
	}

	/********************************************************************
	 * Normalized Linear Interpolation
	 * @return nullvector if Lerp( start, end, t ) is nullvector.
	 ********************************************************************/
	template<typename ScalarType>
	inline Vector3<ScalarType> Nlerp( const Vector3<ScalarType> &start, const Vector3<ScalarType> &end,  const ScalarType &t )
	{
		Vector3<ScalarType> output = Lerp( start, end, t );
		ScalarType magnitudeSquared = output.Dot( output );
		if( magnitudeSquared != 0 )
		{
			return output /= (ScalarType)::std::sqrt( magnitudeSquared );
		}		
		return output; // error: returning nullvector
	}

	/********************************************************************
	 * Normalized Linear Interpolation
	 * @return nullvector if Lerp( start, end, t ) is nullvector.
	 ********************************************************************/
	template<typename ScalarType>
	inline Vector4<ScalarType> Nlerp( const Vector4<ScalarType> &start, const Vector4<ScalarType> &end,  const ScalarType &t )
	{
		Vector4<ScalarType> output = Lerp( start, end, t );
		ScalarType magnitudeSquared = output.Dot( output );
		if( magnitudeSquared != 0 )
		{
			return output /= (ScalarType)::std::sqrt( magnitudeSquared );
		}		
		return output; // error: returning nullvector
	}

	/********************************************************************
	 * Spherical Linear Interpolation on Quaternions
	 ********************************************************************/
	template<typename ScalarType>
	inline Quaternion<ScalarType> Slerp( const Quaternion<ScalarType> &start, const Quaternion<ScalarType> &end,  const ScalarType &t )
	{
		ScalarType angle = (ScalarType)::std::acos( Vector4<ScalarType>(start.imaginary, start.real).Dot(Vector4<ScalarType>(end.imaginary, end.real)) );
		Quaternion<ScalarType> result = start * (ScalarType)::std::sin( angle * (1 - t) );
		result += end * (ScalarType)::std::sin( angle * t );
		result *= (ScalarType)1.0f / (ScalarType)::std::sin( angle );
		return result;
	}
}

namespace LinearAlgebra2D
{
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> X_AxisTo( const ::LinearAlgebra::Vector2<ScalarType> &yAxis )
	{ return ::LinearAlgebra::Vector2<ScalarType>( yAxis.y, -yAxis.x ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> Y_AxisTo( const ::LinearAlgebra::Vector2<ScalarType> &xAxis )
	{ return ::LinearAlgebra::Vector2<ScalarType>( -xAxis.y, xAxis.x ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & TranslationMatrix( const ::LinearAlgebra::Vector2<ScalarType> &position, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( 1, 0, position.x,
																   0, 1, position.y,
																   0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix2x2<ScalarType> & RotationMatrix( const ScalarType &radian, ::LinearAlgebra::Matrix2x2<ScalarType> &targetMem = ::LinearAlgebra::Matrix2x2<ScalarType>() )
	{
		ScalarType s = ::std::sin( radian ),
				   c = ::std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix2x2<ScalarType>( c, -s, s, c );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & RotationMatrix( const ScalarType &radian, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		ScalarType s = ::std::sin( radian ),
				   c = ::std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( c,-s, 0, s, c, 0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix2x2<ScalarType> & InverseRotationMatrix( const ::LinearAlgebra::Matrix2x2<ScalarType> &rotationMatrix )
	{
		return rotationMatrix.GetTranspose();
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & InverseRotationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &rotationMatrix )
	{
		return rotationMatrix.GetTranspose();
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> OrientationMatrix( const ::LinearAlgebra::Matrix2x2<ScalarType> &rotation, const ::LinearAlgebra::Vector2<ScalarType> &translation )
	{
		return ::LinearAlgebra::Matrix3x3<ScalarType>( ::LinearAlgebra::Vector3<ScalarType>(rotation.v[0], 0),
													   ::LinearAlgebra::Vector3<ScalarType>(rotation.v[1], 0),
													   ::LinearAlgebra::Vector3<ScalarType>(translation, 1) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> OrientationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &rotation, const ::LinearAlgebra::Vector2<ScalarType> &translation )
	{
		return ::LinearAlgebra::Matrix3x3<ScalarType>( rotation.v[0], rotation.v[1], ::LinearAlgebra::Vector3<ScalarType>(translation, 1) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & OrientationMatrix( const ScalarType &radian, const ::LinearAlgebra::Vector2<ScalarType> &position, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		ScalarType s = ::std::sin( radian ),
				   c = ::std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( c,-s, position.x,
																   s, c, position.y,
																   0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector2<ScalarType> &lookAt, const ::LinearAlgebra::Vector2<ScalarType> &position, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		::LinearAlgebra::Vector2<ScalarType> direction = ( lookAt - position ).GetNormalized();
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( ::LinearAlgebra::Vector3<ScalarType>( X_AxisTo(direction), 0.0f ),
																   ::LinearAlgebra::Vector3<ScalarType>( direction, 0.0f ),
																   ::LinearAlgebra::Vector3<ScalarType>( position, 1.0f ) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & OrientationMatrix( const ScalarType &radian, const ::LinearAlgebra::Vector2<ScalarType> &position, const ::LinearAlgebra::Vector2<ScalarType> &centerOfRotation, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{ // TODO: not tested
		RotationMatrix( radian, targetMem );
		targetMem.v[2].xy = position + centerOfRotation;
		targetMem.v[2].x -= ::LinearAlgebra::Vector2<ScalarType>( targetMem.v[0].x, targetMem.v[1].x ).Dot( centerOfRotation );
		targetMem.v[2].y -= ::LinearAlgebra::Vector2<ScalarType>( targetMem.v[0].y, targetMem.v[1].y ).Dot( centerOfRotation );
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & InverseOrientationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &orientationMatrix, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( orientationMatrix.m11, orientationMatrix.m21, -orientationMatrix.v[0].xy.Dot(orientationMatrix.v[2].xy),
																   orientationMatrix.m12, orientationMatrix.m22, -orientationMatrix.v[1].xy.Dot(orientationMatrix.v[2].xy),
																   0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> ExtractRotationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &orientationMatrix )
	{
		return ::LinearAlgebra::Matrix3x3<ScalarType>( orientationMatrix.v[0], orientationMatrix.v[1], ::LinearAlgebra::Vector3<ScalarType>::standard_unit_z );
	}
}

namespace LinearAlgebra3D
{
	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> WorldAxisOf( const ::LinearAlgebra::Quaternion<ScalarType> &rotation, const ::LinearAlgebra::Vector3<ScalarType> &localAxis )
	{
		return (rotation*localAxis*rotation.GetConjugate()).imaginary;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> AngularAxis( const ::LinearAlgebra::Quaternion<ScalarType> &rotation )
	{ // see http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
		if( rotation.real < ScalarType(1) )
		{
			ScalarType angle = ScalarType(2) * ScalarType( ::std::acos(rotation.real) ),
					   multiplier = angle / ScalarType( ::std::sqrt(ScalarType(1) - rotation.real * rotation.real) );
			return multiplier * rotation.imaginary;
		}
		else return ::LinearAlgebra::Vector3<ScalarType>::null;
	}

	// All Matrix to AngularAxis conversions here is incorrect
	//template<typename ScalarType>
	//inline ::LinearAlgebra::Vector4<ScalarType> AngularAxis( const ::LinearAlgebra::Matrix3x3<ScalarType> &rotationMatrix )
	//{
	//	return ::std::asin( ::LinearAlgebra::Vector4<ScalarType>(rotationMatrix.v[1].z, rotationMatrix.v[2].x, rotationMatrix.v[0].y, 1) );
	//}

	//template<typename ScalarType>
	//inline ::LinearAlgebra::Vector4<ScalarType> AngularAxis( const ::LinearAlgebra::Matrix4x4<ScalarType> &rotationMatrix )
	//{
	//	return ::std::asin( ::LinearAlgebra::Vector4<ScalarType>(rotationMatrix.v[1].z, rotationMatrix.v[2].x, rotationMatrix.v[0].y, 1) );
	//}

	//template<typename ScalarType>
	//inline ::LinearAlgebra::Vector4<ScalarType> ExtractAngularAxis( const ::LinearAlgebra::Matrix4x4<ScalarType> &orientationMatrix )
	//{
	//	return ::std::asin( ::LinearAlgebra::Vector4<ScalarType>(orientationMatrix.v[1].z, orientationMatrix.v[2].x, orientationMatrix.v[0].y, 0) );
	//}


	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> ScalingMatrix( const ::LinearAlgebra::Vector3<ScalarType> &s )
	{
		return ::LinearAlgebra::Matrix4x4<ScalarType>( s.x, 0, 0, 0,
													   0, s.y, 0, 0,
													   0, 0, s.z, 0,
													   0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> ScalingMatrix( const ::LinearAlgebra::Vector4<ScalarType> &s )
	{
		return ::LinearAlgebra::Matrix4x4<ScalarType>( s.x, 0, 0, 0,
													   0, s.y, 0, 0,
													   0, 0, s.z, 0,
													   0, 0, 0, s.w );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & TranslationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &position, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( 1, 0, 0, position.x,
																   0, 1, 0, position.y,
																   0, 0, 1, position.z,
																   0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Quaternion<ScalarType> Rotation( const ScalarType &radian, const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis )
	{
		ScalarType r = radian * 0.5f,
				   s = std::sin( r ),
				   c = std::cos( r );

		return ::LinearAlgebra::Quaternion<ScalarType>( normalizedAxis * s, c );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Quaternion<ScalarType> Rotation( const ScalarType &radian, const ::LinearAlgebra::Vector4<ScalarType> &normalizedAxis )
	{
		ScalarType r = radian * 0.5f,
				   s = std::sin( r ),
				   c = std::cos( r );

		return ::LinearAlgebra::Quaternion<ScalarType>( (normalizedAxis * s).xyz, c );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Quaternion<ScalarType> Rotation( const ::LinearAlgebra::Vector3<ScalarType> &angularAxis )
	{
		ScalarType radius = angularAxis.Dot( angularAxis );
		if( radius != 0 )
		{
			radius = (ScalarType)::std::sqrt( radius );
			return Rotation( radius, angularAxis / radius );
		}
		else
		{
			return ::LinearAlgebra::Quaternion<ScalarType>::identity;
		}
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Quaternion<ScalarType> Rotation( const ::LinearAlgebra::Vector4<ScalarType> &angularAxis )
	{
		ScalarType radius = angularAxis.Dot( angularAxis );
		if( radius != 0 )
		{
			radius = (ScalarType)::std::sqrt( radius );
			return Rotation( radius, angularAxis / radius );
		}
		else
		{
			return ::LinearAlgebra::Quaternion<ScalarType>::identity;
		}
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & RotationMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		::LinearAlgebra::Quaternion<ScalarType> conjugate = rotationQuaternion.GetConjugate();

		targetMem.v[0] = (rotationQuaternion * ::LinearAlgebra::Vector3<ScalarType>(1,0,0) * conjugate).imaginary;
		targetMem.v[1] = (rotationQuaternion * ::LinearAlgebra::Vector3<ScalarType>(0,1,0) * conjugate).imaginary;
		targetMem.v[2] = (rotationQuaternion * ::LinearAlgebra::Vector3<ScalarType>(0,0,1) * conjugate).imaginary;
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		::LinearAlgebra::Quaternion<ScalarType> conjugate = rotationQuaternion.GetConjugate();

		targetMem.v[0] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(1,0,0)*conjugate).imaginary, 0 );
		targetMem.v[1] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,1,0)*conjugate).imaginary, 0 );
		targetMem.v[2] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,0,1)*conjugate).imaginary, 0 );
		targetMem.v[3] = ::LinearAlgebra::Vector4<ScalarType>::standard_unit_w;
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, const ::LinearAlgebra::Vector3<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		::LinearAlgebra::Quaternion<ScalarType> conjugate = rotationQuaternion.GetConjugate();

		targetMem.v[0] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(1,0,0)*conjugate).imaginary, 0 );
		targetMem.v[1] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,1,0)*conjugate).imaginary, 0 );
		targetMem.v[2] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,0,1)*conjugate).imaginary, 0 );
		targetMem.v[3] = ::LinearAlgebra::Vector4<ScalarType>( translation, 1 );
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, const ::LinearAlgebra::Vector4<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		::LinearAlgebra::Quaternion<ScalarType> conjugate = rotationQuaternion.GetConjugate();

		targetMem.v[0] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(1,0,0)*conjugate).imaginary, 0 );
		targetMem.v[1] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,1,0)*conjugate).imaginary, 0 );
		targetMem.v[2] = ::LinearAlgebra::Vector4<ScalarType>( (rotationQuaternion*::LinearAlgebra::Vector3<ScalarType>(0,0,1)*conjugate).imaginary, 0 );
		targetMem.v[3] = translation;
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & ViewMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, const ::LinearAlgebra::Vector3<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		OrientationMatrix( rotationQuaternion, translation, targetMem );
		return InverseOrientationMatrix( targetMem, targetMem );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & ViewMatrix( const ::LinearAlgebra::Quaternion<ScalarType> &rotationQuaternion, const ::LinearAlgebra::Vector4<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		OrientationMatrix( rotationQuaternion, translation, targetMem );
		return InverseOrientationMatrix( targetMem, targetMem );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & RotationMatrix_AxisX( const ScalarType &radian, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		ScalarType s = std::sin( radian ),
				   c = std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( 1, 0, 0,
																   0, c,-s,
																   0, s, c );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix_AxisX( const ScalarType &radian, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		ScalarType s = std::sin( radian ),
				   c = std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( 1, 0, 0, 0,
																   0, c,-s, 0,
																   0, s, c, 0,
																   0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & RotationMatrix_AxisY( const ScalarType &radian, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		ScalarType s = std::sin( radian ),
				   c = std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>( c, 0, s,
																   0, 1, 0,
																  -s, 0, c );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix_AxisY( const ScalarType &radian, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		ScalarType s = std::sin( radian ),
				   c = std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( c, 0, s, 0,
																   0, 1, 0, 0,
																  -s, 0, c, 0,
																   0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & RotationMatrix_AxisZ( const ScalarType &radian, ::LinearAlgebra::Matrix3x3<ScalarType> &targetMem = ::LinearAlgebra::Matrix3x3<ScalarType>() )
	{
		return ::LinearAlgebra2D::RotationMatrix( radian, targetMem );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix_AxisZ( const ScalarType &radian, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		ScalarType s = std::sin( radian ),
				   c = std::cos( radian );
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( c,-s, 0, 0,
																   s, c, 0, 0,
																   0, 0, 1, 0,
																   0, 0, 0, 1 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> RotationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &angularAxis )
	{
		ScalarType radian = angularAxis.GetMagnitude();
		if( radian != 0 )
		{
			return RotationMatrix( angularAxis / radian, radian );
		}
		else
		{
			return ::LinearAlgebra::Matrix4x4<ScalarType>::identity;
		}
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis, const ScalarType &radian, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /// TODO: not verified
		ScalarType r = radian * 0.5f,
				   s = std::sin( r ),
				   c = std::cos( r );
		::LinearAlgebra::Quaternion<ScalarType> q( normalizedAxis * s, c ),
												qConj = q.GetConjugate();

		targetMem.v[0] = ::LinearAlgebra::Vector4<ScalarType>( (q*::LinearAlgebra::Vector3<ScalarType>(1,0,0)*qConj).imaginary, 0 );
		targetMem.v[1] = ::LinearAlgebra::Vector4<ScalarType>( (q*::LinearAlgebra::Vector3<ScalarType>(0,1,0)*qConj).imaginary, 0 );
		targetMem.v[2] = ::LinearAlgebra::Vector4<ScalarType>( (q*::LinearAlgebra::Vector3<ScalarType>(0,0,1)*qConj).imaginary, 0 );
		targetMem.v[3] = ::LinearAlgebra::Vector4<ScalarType>::standard_unit_w;
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> & InverseRotationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &rotationMatrix )
	{
		return rotationMatrix.GetTranspose();
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & InverseRotationMatrix( const ::LinearAlgebra::Matrix4x4<ScalarType> &rotationMatrix )
	{
		return rotationMatrix.GetTranspose();
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> OrientationMatrix( const ::LinearAlgebra::Matrix3x3<ScalarType> &rotation, const ::LinearAlgebra::Vector3<ScalarType> &translation )
	{
		return ::LinearAlgebra::Matrix4x4<ScalarType>( ::LinearAlgebra::Vector4<ScalarType>(rotation.v[0], 0),
													   ::LinearAlgebra::Vector4<ScalarType>(rotation.v[1], 0),
													   ::LinearAlgebra::Vector4<ScalarType>(rotation.v[2], 0),
													   ::LinearAlgebra::Vector4<ScalarType>(translation, 1) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> OrientationMatrix( const ::LinearAlgebra::Matrix4x4<ScalarType> &rotation, const ::LinearAlgebra::Vector3<ScalarType> &translation )
	{
		return ::LinearAlgebra::Matrix4x4<ScalarType>( rotation.v[0], rotation.v[1], rotation.v[2], ::LinearAlgebra::Vector4<ScalarType>(translation, 1) );
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis, const ScalarType &deltaRadian, const ::LinearAlgebra::Vector3<ScalarType> &sumTranslation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		RotationMatrix( normalizedAxis, deltaRadian, targetMem );
		targetMem.v[3].xyz = sumTranslation;
		return targetMem;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &angularAxis, const ::LinearAlgebra::Vector3<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		ScalarType radian = angularAxis.Dot( angularAxis );
		if( radian > 0 )
		{
			radian = ::std::sqrt( radian );
			return OrientationMatrix( angularAxis / radian, radian, translation, targetMem );
		}
		else
		{
			targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>::identity;
			targetMem.v[3].xyz = translation;
			return targetMem;
		}
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & ViewMatrix( const ::LinearAlgebra::Vector3<ScalarType> &angularAxis, const ::LinearAlgebra::Vector3<ScalarType> &translation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		ScalarType radian = angularAxis.Dot( angularAxis );
		if( radian > 0 )
		{
			radian = ::std::sqrt( radian );
			return InverseOrientationMatrix( OrientationMatrix(angularAxis / radian, radian, translation, targetMem) );
		}
		else
		{
			targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>::identity;
			targetMem.v[3].xyz = -translation;
			return targetMem;
		}
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &sumDeltaAngularAxis, const ::LinearAlgebra::Vector3<ScalarType> &sumTranslation, const ::LinearAlgebra::Vector3<ScalarType> &centerOfRotation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		ScalarType radian = sumDeltaAngularAxis.Dot( sumDeltaAngularAxis );
		if( radian > 0 )
		{
			radian = ::std::sqrt( radian );
			RotationMatrix( sumDeltaAngularAxis / radian, radian, targetMem );
		}
		else targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>::identity;

		targetMem.v[3].xyz = sumTranslation + centerOfRotation;
		targetMem.v[3].x -= ::LinearAlgebra::Vector3<ScalarType>( targetMem.v[0].x, targetMem.v[1].x, targetMem.v[2].x ).Dot( centerOfRotation );
		targetMem.v[3].y -= ::LinearAlgebra::Vector3<ScalarType>( targetMem.v[0].y, targetMem.v[1].y, targetMem.v[2].y ).Dot( centerOfRotation );
		targetMem.v[3].z -= ::LinearAlgebra::Vector3<ScalarType>( targetMem.v[0].z, targetMem.v[1].z, targetMem.v[2].z ).Dot( centerOfRotation );

		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> OrientationMatrix_LookAtDirection( const ::LinearAlgebra::Vector3<ScalarType> &normalizedDirection, const ::LinearAlgebra::Vector3<ScalarType> &normalizedUpVector, const ::LinearAlgebra::Vector3<ScalarType> &worldPos )
	{ // Righthanded system! Forward is considered to be along negative z axis. Up is considered along positive y axis.
		::LinearAlgebra::Vector3<ScalarType> right = normalizedDirection.Cross( normalizedUpVector ).GetNormalized();
		return ::LinearAlgebra::Matrix4x4<ScalarType>( ::LinearAlgebra::Vector4<ScalarType>( right, 0.0f ),
													   ::LinearAlgebra::Vector4<ScalarType>( right.Cross( normalizedDirection ), 0.0f ),
													   ::LinearAlgebra::Vector4<ScalarType>(-normalizedDirection, 0.0f ),
													   ::LinearAlgebra::Vector4<ScalarType>( worldPos, 1.0f ) );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> OrientationMatrix_LookAtPos( const ::LinearAlgebra::Vector3<ScalarType> &worldLookAt, const ::LinearAlgebra::Vector3<ScalarType> &normalizedUpVector, const ::LinearAlgebra::Vector3<ScalarType> &worldPos )
	{ // Righthanded system! Forward is considered to be along negative z axis. Up is considered along positive y axis.
		::LinearAlgebra::Vector3<ScalarType> direction = ( worldLookAt - worldPos ).GetNormalized();
		return OrientationMatrix_LookAtDirection( direction, normalizedUpVector, worldPos );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & InverseOrientationMatrix( const ::LinearAlgebra::Matrix4x4<ScalarType> &orientationMatrix, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( orientationMatrix.m11, orientationMatrix.m21, orientationMatrix.m31, -orientationMatrix.v[0].xyz.Dot(orientationMatrix.v[3].xyz),
																   orientationMatrix.m12, orientationMatrix.m22, orientationMatrix.m32, -orientationMatrix.v[1].xyz.Dot(orientationMatrix.v[3].xyz),
																   orientationMatrix.m13, orientationMatrix.m23, orientationMatrix.m33, -orientationMatrix.v[2].xyz.Dot(orientationMatrix.v[3].xyz),
																   0, 0, 0, 1 );
	}

	// O0 = T0 * R0
	// O1 = T1 * T0 * R1 * R0
	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & UpdateOrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &deltaPosition, const ::LinearAlgebra::Matrix4x4<ScalarType> &deltaRotationMatrix, ::LinearAlgebra::Matrix4x4<ScalarType> &orientationMatrix )
	{
		::LinearAlgebra::Vector3<ScalarType> position = deltaPosition + orientationMatrix.v[3].xyz;
		orientationMatrix.v[3].xyz = ::LinearAlgebra::Vector3<ScalarType>::null;

		orientationMatrix = deltaRotationMatrix * orientationMatrix;
		orientationMatrix.v[3].xyz = position;
		return orientationMatrix;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> ExtractRotationMatrix( const ::LinearAlgebra::Matrix4x4<ScalarType> &orientationMatrix )
	{
		return ::LinearAlgebra::Matrix4x4<ScalarType>( orientationMatrix.v[0], orientationMatrix.v[1], orientationMatrix.v[2], ::LinearAlgebra::Vector4<ScalarType>::standard_unit_w );
	}

	/*	Creates an orthographic projection matrix designed for DirectX enviroment.
		width; of the projection sample volume.
		height; of the projection sample volume.
		nearClip: Distance to the nearClippingPlane.
		farClip: Distance to the farClippingPlane
		targetMem; is set to an orthographic projection matrix and then returned.
	*/
	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & ProjectionMatrix_Orthographic( const ScalarType &width, const ScalarType &height, const ScalarType &nearClip, const ScalarType &farClip, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		ScalarType c = 1 / (nearClip - farClip);
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( 2/width, 0, 0, 0,
																   0, 2/height, 0, 0,
																   0, 0, -c, nearClip*c, 
																   0, 0, 0, 1 );
	}

	/*******************************************************************
	 * Creates a perspective projection matrix designed for DirectX enviroment.
	 * @param vertFoV; is the vertical field of vision in radians. (lookup FoV Hor+ )
	 * @param aspect; is the screenratio width/height (example 16/9 or 16/10 )
	 * @param nearClip: Distance to the nearClippingPlane
	 * @param farClip: Distance to the farClippingPlane
	 * @param targetMem; is set to a perspective transform matrix and then returned.
	 * @return targetMem
	 * @test Compare with transposed D3D counterpart
	 *******************************************************************/
	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & ProjectionMatrix_Perspective( const ScalarType &vertFoV, const ScalarType &aspect, const ScalarType &nearClip, const ScalarType &farClip, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ 
		ScalarType fov = 1 / ::std::tan( vertFoV * 0.5f ),
				   dDepth = farClip / (farClip - nearClip);
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( fov / aspect, 0, 0, 0, 
																   0, fov, 0, 0,
																   0, 0, dDepth, -(dDepth * nearClip),
																   0, 0, 1, 0 );
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & ProjectionMatrix_Perspective( const ScalarType &left, const ScalarType &right, const ScalarType &top, const ScalarType &bottom, const ScalarType &nearClip, const ScalarType &farClip, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		ScalarType fov = 1 / ::std::tan( vertFoV * 0.5f ),
				   dDepth = farClip / (farClip - nearClip);
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( 2*nearClip/(right - left), 0, -(right + left)/(right - left), 0,
																   0, 2*nearClip/(top - bottom), -(top + bottom)/(top - bottom), 0,
																   0, 0, dDepth, -(dDepth * nearClip),
																   0, 0, 1, 0 );
	}


	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> VectorProjection( const ::LinearAlgebra::Vector3<ScalarType> &vector, const ::LinearAlgebra::Vector3<ScalarType> &axis )
	{ return axis * ( vector.Dot(axis) / axis.Dot(axis) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> VectorProjection( const ::LinearAlgebra::Vector4<ScalarType> &vector, const ::LinearAlgebra::Vector4<ScalarType> &axis )
	{ return axis * ( vector.Dot(axis) / axis.Dot(axis) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> NormalProjection( const ::LinearAlgebra::Vector3<ScalarType> &vector, const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis )
	{ return normalizedAxis * ( vector.Dot(normalizedAxis) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> NormalProjection( const ::LinearAlgebra::Vector4<ScalarType> &vector, const ::LinearAlgebra::Vector4<ScalarType> &normalizedAxis )
	{ return normalizedAxis * ( vector.Dot(normalizedAxis) ); }

	template<typename ScalarType>
	::LinearAlgebra::Vector4<ScalarType> & SnapAngularAxis( const ::LinearAlgebra::Vector4<ScalarType> &startAngularAxis, const ::LinearAlgebra::Vector4<ScalarType> &localStartNormal, const ::LinearAlgebra::Vector4<ScalarType> &worldEndNormal, ::LinearAlgebra::Vector4<ScalarType> &targetMem = ::LinearAlgebra::Vector4<ScalarType>() )
	{
		::LinearAlgebra::Vector4<ScalarType> worldStartNormal( WorldAxisOf(Rotation(startAngularAxis.xyz), localStartNormal.xyz), (ScalarType)0 );
		targetMem = ::LinearAlgebra::Vector4<ScalarType>( worldStartNormal.xyz.Cross(worldEndNormal.xyz), (ScalarType)0);
		targetMem *= (ScalarType)::std::acos( ::Utility::Value::Clamp(worldStartNormal.Dot(worldEndNormal), (ScalarType)0, (ScalarType)1) );
		return targetMem += startAngularAxis;
	}

	template<typename ScalarType>
	::LinearAlgebra::Vector3<ScalarType> & SnapAngularAxis( const ::LinearAlgebra::Vector3<ScalarType> &startAngularAxis, const ::LinearAlgebra::Vector3<ScalarType> &localStartNormal, const ::LinearAlgebra::Vector3<ScalarType> &worldEndNormal, ::LinearAlgebra::Vector3<ScalarType> &targetMem = ::LinearAlgebra::Vector3<ScalarType>() )
	{ //! @todo TODO: This code is broken and do not work!
		::LinearAlgebra::Vector3<ScalarType> worldStartNormal( WorldAxisOf(Rotation(startAngularAxis), localStartNormal) );
		targetMem = worldStartNormal.Cross( worldEndNormal );
		targetMem *= (ScalarType)::std::acos( ::Utility::Value::Clamp(worldStartNormal.Dot(worldEndNormal), (ScalarType)0, (ScalarType)1) );
		return targetMem += startAngularAxis;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & SnapAxisYToNormal_UsingNlerp( ::LinearAlgebra::Matrix4x4<ScalarType> &rotation, const ::LinearAlgebra::Vector4<ScalarType> &normalizedAxis )
	{
		ScalarType projectedMagnitude = rotation.v[0].Dot( normalizedAxis );
		if( projectedMagnitude == 1 )
		{ // infinite possible solutions -> roadtrip!
			::LinearAlgebra::Vector4<ScalarType> interpolated = ::LinearAlgebra::Nlerp( rotation.v[1], normalizedAxis, (ScalarType)0.5f );
			
			// interpolated.Dot( interpolated ) == 0 should be impossible at this point
			projectedMagnitude = rotation.v[0].Dot( interpolated );
			rotation.v[0] -= projectedMagnitude * interpolated;
			rotation.v[0].Normalize();
			projectedMagnitude = rotation.v[0].Dot( normalizedAxis );
		}
		rotation.v[0] -= projectedMagnitude * normalizedAxis;
		rotation.v[0].Normalize();
		rotation.v[1] = normalizedAxis;
		rotation.v[2].xyz = rotation.v[0].xyz.Cross( rotation.v[1].xyz );
		return rotation;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateAxisYToNormal_UsingNlerp( ::LinearAlgebra::Matrix4x4<ScalarType> &rotation, const ::LinearAlgebra::Vector4<ScalarType> &normalizedAxis, ScalarType t )
	{
		::LinearAlgebra::Vector4<ScalarType> interpolated = ::LinearAlgebra::Nlerp( rotation.v[1], normalizedAxis, t );
		if( interpolated.Dot(interpolated) == 0 )
			return rotation; // return no change
		return SnapAxisYToNormal_UsingNlerp( rotation, interpolated );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateRotation_UsingNonRigidNlerp( const ::LinearAlgebra::Matrix4x4<ScalarType> &start, const ::LinearAlgebra::Matrix4x4<ScalarType> &end, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		targetMem.v[0] = ::LinearAlgebra::Nlerp( start.v[0], end.v[0], t );
		targetMem.v[1] = ::LinearAlgebra::Nlerp( start.v[1], end.v[1], t );
		targetMem.v[2] = ::LinearAlgebra::Nlerp( start.v[2], end.v[2], t );
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateOrientation_UsingNonRigidNlerp( const ::LinearAlgebra::Matrix4x4<ScalarType> &start, const ::LinearAlgebra::Matrix4x4<ScalarType> &end, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		InterpolateRotation_UsingNonRigidNlerp( start, end, t, targetMem );
		targetMem.v[3] = ::LinearAlgebra::Lerp( start.v[3], end.v[3], t );
		return targetMem;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateRotation_UsingRigidNlerp( const ::LinearAlgebra::Matrix4x4<ScalarType> &start, const ::LinearAlgebra::Matrix4x4<ScalarType> &end, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		// Nlerp y axis
		targetMem.v[1] = ::LinearAlgebra::Nlerp( start.v[1], end.v[1], t );
		
		// Lerp z axis and orthogonolize against y axis
		targetMem.v[2] = ::LinearAlgebra::Lerp( start.v[2], end.v[2], t );
		targetMem.v[2] -= targetMem.v[2].Dot(targetMem.v[1]) * targetMem.v[1];
		targetMem.v[2].Normalize();

		// Cross product x axis from y and z
		targetMem.v[0].xyz = targetMem.v[1].xyz.Cross(targetMem.v[2].xyz);
		return targetMem;
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateOrientation_UsingRigidNlerp( const ::LinearAlgebra::Matrix4x4<ScalarType> &start, const ::LinearAlgebra::Matrix4x4<ScalarType> &end, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		InterpolateRotation_UsingRigidNlerp( start, end, t, targetMem );
		targetMem.v[3] = ::LinearAlgebra::Lerp( start.v[3], end.v[3], t );
		return targetMem;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateOrientation_UsingNonRigidNlerp( const ::LinearAlgebra::Quaternion<ScalarType> &startR, const ::LinearAlgebra::Vector3<ScalarType> &startT, const ::LinearAlgebra::Quaternion<ScalarType> &endR, const ::LinearAlgebra::Vector3<ScalarType> &endT, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		return InterpolateOrientation_UsingNonRigidNlerp( OrientationMatrix(startR, startT), OrientationMatrix(endR, endT), t, targetMem );
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & InterpolateOrientation_UsingSlerp( const ::LinearAlgebra::Quaternion<ScalarType> &startR, const ::LinearAlgebra::Vector3<ScalarType> &startT, const ::LinearAlgebra::Quaternion<ScalarType> &endR, const ::LinearAlgebra::Vector3<ScalarType> &endT, ScalarType t, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
	{
		return OrientationMatrix( ::LinearAlgebra::Slerp(startR, endR, t), ::LinearAlgebra::Lerp(::LinearAlgebra::Vector4<ScalarType>(startT, (ScalarType)1.0f), ::LinearAlgebra::Vector4<ScalarType>(endT, (ScalarType)1.0f), t).xyz, targetMem );
	}	
}

#include "Utilities.h"

namespace Utility { namespace Value
{ // Utility Value Specializations

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> Abs( const ::LinearAlgebra::Vector2<ScalarType> &vector )
	{ return ::LinearAlgebra::Vector2<ScalarType>( Abs(vector.x), Abs(vector.y) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> Abs( const ::LinearAlgebra::Vector3<ScalarType> &vector )
	{ return ::LinearAlgebra::Vector3<ScalarType>( Abs(vector.xy), Abs(vector.z) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> Abs( const ::LinearAlgebra::Vector4<ScalarType> &vector )
	{ return ::LinearAlgebra::Vector4<ScalarType>( Abs(vector.xyz), Abs(vector.w) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix2x2<ScalarType> Abs( const ::LinearAlgebra::Matrix2x2<ScalarType> &matrix )
	{ return ::LinearAlgebra::Matrix2x2<ScalarType>( Abs(matrix.v[0]), Abs(matrix.v[1]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> Abs( const ::LinearAlgebra::Matrix3x3<ScalarType> &matrix )
	{ return ::LinearAlgebra::Matrix3x3<ScalarType>( Abs(matrix.v[0]), Abs(matrix.v[1]), Abs(matrix.v[2]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> Abs( const ::LinearAlgebra::Matrix4x4<ScalarType> &matrix )
	{ return ::LinearAlgebra::Matrix4x4<ScalarType>( Abs(matrix.v[0]), Abs(matrix.v[1]), Abs(matrix.v[2]), Abs(matrix.v[3]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> Max( const ::LinearAlgebra::Vector2<ScalarType> &vectorA, const ::LinearAlgebra::Vector2<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector2<ScalarType>( Max(vectorA.x, vectorB.x), Max(vectorA.y, vectorB.y) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> Max( const ::LinearAlgebra::Vector3<ScalarType> &vectorA, const ::LinearAlgebra::Vector3<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector3<ScalarType>( Max(vectorA.xy, vectorB.xy), Max(vectorA.z, vectorB.z) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> Max( const ::LinearAlgebra::Vector4<ScalarType> &vectorA, const ::LinearAlgebra::Vector4<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector4<ScalarType>( Max(vectorA.xyz, vectorB.xyz), Max(vectorA.w, vectorB.w) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix2x2<ScalarType> Max( const ::LinearAlgebra::Matrix2x2<ScalarType> &matrixA, const ::LinearAlgebra::Matrix2x2<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix2x2<ScalarType>( Max(matrixA.v[0], matrixB.v[0]), Max(matrixA.v[1], matrixB.v[1]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> Max( const ::LinearAlgebra::Matrix3x3<ScalarType> &matrixA, const ::LinearAlgebra::Matrix3x3<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix3x3<ScalarType>( Max(matrixA.v[0], matrixB.v[0]), Max(matrixA.v[1], matrixB.v[1]), Max(matrixA.v[2], matrixB.v[2]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> Max( const ::LinearAlgebra::Matrix4x4<ScalarType> &matrixA, const ::LinearAlgebra::Matrix4x4<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix4x4<ScalarType>( Max(matrixA.v[0], matrixB.v[0]), Max(matrixA.v[1], matrixB.v[1]), Max(matrixA.v[2], matrixB.v[2]), Max(matrixA.v[3], matrixB.v[3]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector2<ScalarType> Min( const ::LinearAlgebra::Vector2<ScalarType> &vectorA, const ::LinearAlgebra::Vector2<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector2<ScalarType>( Min(vectorA.x, vectorB.x), Min(vectorA.y, vectorB.y) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> Min( const ::LinearAlgebra::Vector3<ScalarType> &vectorA, const ::LinearAlgebra::Vector3<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector3<ScalarType>( Min(vectorA.xy, vectorB.xy), Min(vectorA.z, vectorB.z) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector4<ScalarType> Min( const ::LinearAlgebra::Vector4<ScalarType> &vectorA, const ::LinearAlgebra::Vector4<ScalarType> &vectorB )
	{ return ::LinearAlgebra::Vector4<ScalarType>( Min(vectorA.xyz, vectorB.xyz), Min(vectorA.w, vectorB.w) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix2x2<ScalarType> Min( const ::LinearAlgebra::Matrix2x2<ScalarType> &matrixA, const ::LinearAlgebra::Matrix2x2<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix2x2<ScalarType>( Min(matrixA.v[0], matrixB.v[0]), Min(matrixA.v[1], matrixB.v[1]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix3x3<ScalarType> Min( const ::LinearAlgebra::Matrix3x3<ScalarType> &matrixA, const ::LinearAlgebra::Matrix3x3<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix3x3<ScalarType>( Min(matrixA.v[0], matrixB.v[0]), Min(matrixA.v[1], matrixB.v[1]), Min(matrixA.v[2], matrixB.v[2]) ); }

	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> Min( const ::LinearAlgebra::Matrix4x4<ScalarType> &matrixA, const ::LinearAlgebra::Matrix4x4<ScalarType> &matrixB )
	{ return ::LinearAlgebra::Matrix4x4<ScalarType>( Min(matrixA.v[0], matrixB.v[0]), Min(matrixA.v[1], matrixB.v[1]), Min(matrixA.v[2], matrixB.v[2]), Min(matrixA.v[3], matrixB.v[3]) ); }
} }

#endif