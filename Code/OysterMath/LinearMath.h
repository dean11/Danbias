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
::LinearAlgebra::Matrix2x2<ScalarType> operator * ( const ::LinearAlgebra::Matrix2x2<ScalarType> &left, const ::LinearAlgebra::Matrix2x2<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix2x2<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21),
												   (left.m11 * right.m12) + (left.m12 * right.m22),
												   (left.m21 * right.m11) + (left.m22 * right.m21),
												   (left.m21 * right.m12) + (left.m22 * right.m22) );
}

template<typename ScalarType>
::LinearAlgebra::Vector2<ScalarType> operator * ( const ::LinearAlgebra::Matrix2x2<ScalarType> &matrix, const ::LinearAlgebra::Vector2<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector2<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) );
}

template<typename ScalarType>
::LinearAlgebra::Vector2<ScalarType> operator * ( const ::LinearAlgebra::Vector2<ScalarType> &vector, const ::LinearAlgebra::Matrix2x2<ScalarType> &left )
{
	return ::LinearAlgebra::Vector2<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) );
}

// x3

template<typename ScalarType>
::LinearAlgebra::Matrix3x3<ScalarType> operator * ( const ::LinearAlgebra::Matrix3x3<ScalarType> &left, const ::LinearAlgebra::Matrix3x3<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix3x3<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21) + (left.m13 * right.m31), (left.m11 * right.m12) + (left.m12 * right.m22) + (left.m13 * right.m32), (left.m11 * right.m13) + (left.m12 * right.m23) + (left.m13 * right.m33),
												   (left.m21 * right.m11) + (left.m22 * right.m21) + (left.m23 * right.m31), (left.m21 * right.m12) + (left.m22 * right.m22) + (left.m23 * right.m32), (left.m21 * right.m13) + (left.m22 * right.m23) + (left.m23 * right.m33),
												   (left.m31 * right.m11) + (left.m32 * right.m21) + (left.m33 * right.m31), (left.m31 * right.m12) + (left.m32 * right.m22) + (left.m33 * right.m32), (left.m31 * right.m13) + (left.m32 * right.m23) + (left.m33 * right.m33) );
}

template<typename ScalarType>
::LinearAlgebra::Vector3<ScalarType> operator * ( const ::LinearAlgebra::Matrix3x3<ScalarType> &matrix, const ::LinearAlgebra::Vector3<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector3<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y) + (matrix.m13 * vector.z),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) + (matrix.m23 * vector.z),
												 (matrix.m31 * vector.x) + (matrix.m32 * vector.y) + (matrix.m33 * vector.z) );
}

template<typename ScalarType>
::LinearAlgebra::Vector3<ScalarType> operator * ( const ::LinearAlgebra::Vector3<ScalarType> &vector, const ::LinearAlgebra::Matrix3x3<ScalarType> &left )
{
	return ::LinearAlgebra::Vector3<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21) + (vector.z * matrix.m31),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) + (vector.z * matrix.m32),
												 (vector.x * matrix.m13) + (vector.y * matrix.m23) + (vector.z * matrix.m33) );
}

// x4

template<typename ScalarType>
::LinearAlgebra::Matrix4x4<ScalarType> operator * ( const ::LinearAlgebra::Matrix4x4<ScalarType> &left, const ::LinearAlgebra::Matrix4x4<ScalarType> &right )
{
	return ::LinearAlgebra::Matrix4x4<ScalarType>( (left.m11 * right.m11) + (left.m12 * right.m21) + (left.m13 * right.m31) + (left.m14 * right.m41), (left.m11 * right.m12) + (left.m12 * right.m22) + (left.m13 * right.m32) + (left.m14 * right.m42), (left.m11 * right.m13) + (left.m12 * right.m23) + (left.m13 * right.m33) + (left.m14 * right.m43), (left.m11 * right.m14) + (left.m12 * right.m24) + (left.m13 * right.m34) + (left.m14 * right.m44),
												   (left.m21 * right.m11) + (left.m22 * right.m21) + (left.m23 * right.m31) + (left.m24 * right.m41), (left.m21 * right.m12) + (left.m22 * right.m22) + (left.m23 * right.m32) + (left.m24 * right.m42), (left.m21 * right.m13) + (left.m22 * right.m23) + (left.m23 * right.m33) + (left.m24 * right.m43), (left.m21 * right.m14) + (left.m22 * right.m24) + (left.m23 * right.m34) + (left.m24 * right.m44),
												   (left.m31 * right.m11) + (left.m32 * right.m21) + (left.m33 * right.m31) + (left.m34 * right.m41), (left.m31 * right.m12) + (left.m32 * right.m22) + (left.m33 * right.m32) + (left.m34 * right.m42), (left.m31 * right.m13) + (left.m32 * right.m23) + (left.m33 * right.m33) + (left.m34 * right.m43), (left.m31 * right.m14) + (left.m32 * right.m24) + (left.m33 * right.m34) + (left.m34 * right.m44),
												   (left.m41 * right.m11) + (left.m42 * right.m21) + (left.m43 * right.m31) + (left.m44 * right.m41), (left.m41 * right.m12) + (left.m42 * right.m22) + (left.m43 * right.m32) + (left.m44 * right.m42), (left.m41 * right.m13) + (left.m42 * right.m23) + (left.m43 * right.m33) + (left.m44 * right.m43), (left.m41 * right.m14) + (left.m42 * right.m24) + (left.m43 * right.m34) + (left.m44 * right.m44) );
}

template<typename ScalarType>
::LinearAlgebra::Vector4<ScalarType> operator * ( const ::LinearAlgebra::Matrix4x4<ScalarType> &matrix, const ::LinearAlgebra::Vector4<ScalarType> &vector )
{
	return ::LinearAlgebra::Vector4<ScalarType>( (matrix.m11 * vector.x) + (matrix.m12 * vector.y) + (matrix.m13 * vector.z) + (matrix.m14 * vector.w),
												 (matrix.m21 * vector.x) + (matrix.m22 * vector.y) + (matrix.m23 * vector.z) + (matrix.m24 * vector.w),
												 (matrix.m23 * vector.x) + (matrix.m32 * vector.y) + (matrix.m33 * vector.z) + (matrix.m34 * vector.w),
												 (matrix.m41 * vector.x) + (matrix.m42 * vector.y) + (matrix.m43 * vector.z) + (matrix.m44 * vector.w) );
}

template<typename ScalarType>
::LinearAlgebra::Vector4<ScalarType> operator * ( const ::LinearAlgebra::Vector4<ScalarType> &vector, const ::LinearAlgebra::Matrix4x4<ScalarType> &matrix )
{
	return ::LinearAlgebra::Vector4<ScalarType>( (vector.x * matrix.m11) + (vector.y * matrix.m21) + (vector.z * matrix.m31) + (vector.w * matrix.m41),
												 (vector.x * matrix.m12) + (vector.y * matrix.m22) + (vector.z * matrix.m32) + (vector.w * matrix.m42),
												 (vector.x * matrix.m13) + (vector.y * matrix.m23) + (vector.z * matrix.m33) + (vector.w * matrix.m43),
												 (vector.x * matrix.m14) + (vector.y * matrix.m24) + (vector.z * matrix.m34) + (vector.w * matrix.m44) );
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
}

namespace LinearAlgebra3D
{
	template<typename ScalarType>
	inline ::LinearAlgebra::Matrix4x4<ScalarType> & TranslationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &position, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( 1, 0, 0, position.x,
																   0, 1, 0, position.y,
																   0, 0, 1, position.z,
																   0, 0, 0, 1 );
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
	{ return ::LinearAlgebra2D::RotationMatrix( radian, targetMem ); }

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
	::LinearAlgebra::Matrix4x4<ScalarType> & RotationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis, const ScalarType &radian, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem )
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
	::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &normalizedAxis, const ScalarType &deltaRadian, const ::LinearAlgebra::Vector3<ScalarType> &sumTranslation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		RotationMatrix( normalizedAxis, deltaRadian, targetMem );
		targetMem.v[3].xyz = sumTranslation;
		return targetMem;
	}

	template<typename ScalarType>
	::LinearAlgebra::Matrix4x4<ScalarType> & OrientationMatrix( const ::LinearAlgebra::Vector3<ScalarType> &sumDeltaAngularAxis, const ::LinearAlgebra::Vector3<ScalarType> &sumTranslation, ::LinearAlgebra::Matrix4x4<ScalarType> &targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>() )
	{ /** @todo TODO: not tested */
		ScalarType radian = sumDeltaAngularAxis.Dot( sumDeltaAngularAxis );
		if( radian > 0 )
		{
			radian = ::std::sqrt( radian );
			return OrientationMatrix( sumDeltaAngularAxis / radian, radian, sumTranslation, targetMem );
		}
		else
		{
			targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>::identity;
			targetMem.v[3].xyz = sumTranslation;
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
																   0, 0, -c, 0, 0,
																   0, nearClip*c, 1 );
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
	{ /** @todo TODO: not tested */
		ScalarType fov = 1 / ::std::tan( vertFoV * 0.5f ),
					dDepth = farClip / (farClip - nearClip);
		return targetMem = ::LinearAlgebra::Matrix4x4<ScalarType>( fov / aspect, 0, 0, 0,
																   0, fov, 0, 0,
																   0, 0, dDepth, -(dDepth * nearClip),
																   0, 0, 1, 0 );
	}

	template<typename ScalarType>
	inline ::LinearAlgebra::Vector3<ScalarType> VectorProjection( const ::LinearAlgebra::Vector3<ScalarType> &vector, const ::LinearAlgebra::Vector3<ScalarType> &axis )
	{ return axis * ( vector.Dot(axis) / axis.Dot(axis) ); }
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