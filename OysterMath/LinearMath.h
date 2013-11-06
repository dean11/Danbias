/////////////////////////////////////////////////////////////////////
// Collection of Linear Math Stuff
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef LINEARMATH_H
#define LINEARMATH_H

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

namespace LinearAlgebra
{
	// x2
	template<typename ElementType>
	Matrix2x2<ElementType> operator * ( const Matrix2x2<ElementType> &left, const Matrix2x2<ElementType> &right )
	{ return Matrix2x2<ElementType>( (left.m11 * right.m11) + (left.m21 * right.m12), (left.m11 * right.m21) + (left.m21 * right.m22), (left.m12 * right.m11) + (left.m22 * right.m12), (left.m12 * right.m21) + (left.m22 * right.m22) ); }

	template<typename ElementType>
	Vector2<ElementType> operator * ( const Matrix2x2<ElementType> &matrix, const Vector2<ElementType> &vector )
	{ return Vector2<ElementType>( (matrix.m11 * vector.x) + (matrix.m21 * vector.y), (matrix.m12 * vector.x) + (matrix.m22 * vector.y) ); }

	template<typename ElementType>
	Vector2<ElementType> operator * ( const Vector2<ElementType> &vector, const Matrix2x2<ElementType> &left )
	{ return Vector2<ElementType>( (vector.x * matrix.m11) + (vector.y * matrix.m12), (vector.x * matrix.m21) + (vector.y * matrix.m22) ); }

	// x3
	template<typename ElementType>
	Matrix3x3<ElementType> operator * ( const Matrix3x3<ElementType> &left, const Matrix3x3<ElementType> &right )
	{
		Matrix3x3<ElementType> product, leftT = left.getTranspose();
		for( int i = 0; i < 3; ++i ) for( int j = 0; j < 3; ++j )
			product.m[i][j] = leftT.v[i].dot(right.v[j]);
		return product;
	}

	template<typename ElementType>
	Vector3<ElementType> operator * ( const Matrix3x3<ElementType> &matrix, const Vector3<ElementType> &vector )
	{ return Vector3<ElementType>( (matrix.m11 * vector.x) + (matrix.m21 * vector.y) + (matrix.m31 * vector.z), (matrix.m12 * vector.x) + (matrix.m22 * vector.y) + (matrix.m32 * vector.z), (matrix.m13 * vector.x) + (matrix.m23 * vector.y) + (matrix.m33 * vector.z) ); }

	template<typename ElementType>
	Vector3<ElementType> operator * ( const Vector3<ElementType> &vector, const Matrix3x3<ElementType> &left )
	{ return Vector3<ElementType>( (vector.x * matrix.m11) + (vector.y * matrix.m12) + (vector.z * matrix.m13), (vector.x * matrix.m21) + (vector.y * matrix.m22) + (vector.z * matrix.m23), (vector.x * matrix.m31) + (vector.y * matrix.m32) + (vector.z * matrix.m33) ); }

	// x4
	template<typename ElementType>
	Matrix4x4<ElementType> operator * ( const Matrix4x4<ElementType> &left, const Matrix4x4<ElementType> &right )
	{
		Matrix4x4<ElementType> product, rightT = right.getTranspose();
		for( int i = 0; i < 4; ++i )
		{
			product.m[i][0] = left.v[i].dot(rightT.v[0]);
			product.m[i][1] = left.v[i].dot(rightT.v[1]);
			product.m[i][2] = left.v[i].dot(rightT.v[2]);
			product.m[i][3] = left.v[i].dot(rightT.v[3]);
		}
		return product;
	}

	template<typename ElementType>
	Vector4<ElementType> operator * ( const Matrix4x4<ElementType> &matrix, const Vector4<ElementType> &vector )
	{ return Vector4<ElementType>( (matrix.m11 * vector.x) + (matrix.m21 * vector.y) + (matrix.m31 * vector.z) + (matrix.m41 * vector.w), (matrix.m12 * vector.x) + (matrix.m22 * vector.y) + (matrix.m32 * vector.z) + (matrix.m42 * vector.w), (matrix.m13 * vector.x) + (matrix.m23 * vector.y) + (matrix.m33 * vector.z) + (matrix.m43 * vector.w), (matrix.m14 * vector.x) + (matrix.m24 * vector.y) + (matrix.m34 * vector.z) + (matrix.m44 * vector.w) ); }

	template<typename ElementType> // works for column weighted matrixes
	Vector4<ElementType> operator * ( const Vector4<ElementType> &vector, const Matrix4x4<ElementType> &matrix )
	{ return Vector4<ElementType>( (vector.x * matrix.m11) + (vector.y * matrix.m12) + (vector.z * matrix.m13) + (vector.w * matrix.m14), (vector.x * matrix.m21) + (vector.y * matrix.m22) + (vector.z * matrix.m23) + (vector.w * matrix.m24), (vector.x * matrix.m31) + (vector.y * matrix.m32) + (vector.z * matrix.m33) + (vector.w * matrix.m34), (vector.x * matrix.m41) + (vector.y * matrix.m42) + (vector.z * matrix.m43) + (vector.w * matrix.m44) ); }

	namespace _2D
	{
		template<typename ElementType>
		inline void translationMatrix( Matrix3x3<ElementType> &output, const Vector2<ElementType> &position )
//		{ output = Matrix3x3<ElementType>( 1, 0, position.x, 0, 1, position.y, 0, 0, 1 ); }
		{ output = Matrix3x3<ElementType>( 1, 0, 0, 0, 1, 0, position.x, position.y, 1 ); }

		template<typename ElementType>
		void rotationMatrix( Matrix2x2<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix2x2<ElementType>( c, -s, s, c );
			output = Matrix2x2<ElementType>( c, s, -s, c );
		}

		template<typename ElementType>
		void rotationMatrix( Matrix3x3<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix3x3<ElementType>( c, -s, 0, s, c, 0, 0, 0, 1 );
			output = Matrix3x3<ElementType>( c, s, 0, -s, c, 0, 0, 0, 1 );
		}

		template<typename ElementType>
		void rigidBodyMatrix( Matrix3x3<ElementType> &output, const ElementType &radian, const Vector2<ElementType> &position )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix3x3<ElementType>( c, -s, position.x, s, c, position.y, 0, 0, 1 );
			output = Matrix3x3<ElementType>( c, s, 0, -s, c, 0, position.x, position.y, 1 );
		}
	}

	namespace _3D
	{
		template<typename ElementType>
		inline void translationMatrix( Matrix4x4<ElementType> &output, const Vector3<ElementType> &position )
//		{ output = Matrix4x4<ElementType>( 1, 0, 0, position.x, 0, 1, 0, position.y, 0, 0, 1, position.z,  0, 0, 0, 1 ); }
		{ output = Matrix4x4<ElementType>( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position.x, position.y, position.z, 1 ); }

		template<typename ElementType>
		void inverseRigidBody( Matrix4x4<ElementType> &output, const Matrix4x4<ElementType> &rigidBody )
		{
			output = Matrix4x4<ElementType>( rigidBody.m11, rigidBody.m21, rigidBody.m31, 0,
											 rigidBody.m12, rigidBody.m22, rigidBody.m32, 0,
											 rigidBody.m13, rigidBody.m23, rigidBody.m33, 0,
											 -rigidBody.v[3].xyz.dot(rigidBody.v[0].xyz),
											 -rigidBody.v[3].xyz.dot(rigidBody.v[1].xyz),
											 -rigidBody.v[3].xyz.dot(rigidBody.v[2].xyz), 1 );
		}

		template<typename ElementType>
		void rotationMatrix_AxisX( Matrix3x3<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix3x3<ElementType>( 1, 0, 0, 0, c, -s, 0, s, c );
			output = Matrix3x3<ElementType>( 1, 0, 0, 0, c, s, 0, -s, c );
		}

		template<typename ElementType>
		void rotationMatrix_AxisX( Matrix4x4<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix4x4<ElementType>( 1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1 );
			output = Matrix4x4<ElementType>( 1, 0, 0, 0, 0, c, s, 0, 0, -s, c, 0, 0, 0, 0, 1 );
		}

		template<typename ElementType>
		void rotationMatrix_AxisY( Matrix3x3<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix3x3<ElementType>( c, 0, s, 0, 1, 0, -s, 0, c );
			output = Matrix3x3<ElementType>( c, 0, -s, 0, 1, 0, s, 0, c );
		}

		template<typename ElementType>
		void rotationMatrix_AxisY( Matrix4x4<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix4x4<ElementType>( c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1 );
			output = Matrix4x4<ElementType>( c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, 0, 0, 0, 1 );
		}

		template<typename ElementType>
		inline void rotationMatrix_AxisZ( Matrix3x3<ElementType> &output, const ElementType &radian )
		{ ::LinearAlgebra::_2D::rotationMatrix( output, radian ); }

		template<typename ElementType>
		void rotationMatrix_AxisZ( Matrix4x4<ElementType> &output, const ElementType &radian )
		{
			ElementType s = std::sin( radian ),
						c = std::cos( radian );
//			output = Matrix4x4<ElementType>( c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
			output = Matrix4x4<ElementType>( c, s, 0, 0, -s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
		}

		template<typename ElementType>
		void rotationMatrix( Matrix4x4<ElementType> &output, const Vector3<ElementType> &normalizedAxis, const ElementType &radian )
		{ // TODO : Optimize
			ElementType r = radian * 0.5f,
						s = std::sin( r ),
						c = std::cos( r );
			Quaternion<ElementType> q( normalizedAxis * s, c ),
									qConj = q.getConjugate();

			output.v[0] = Vector4<ElementType>( (q*Vector3<ElementType>(1,0,0)*qConj).imaginary, 0 );
			output.v[1] = Vector4<ElementType>( (q*Vector3<ElementType>(0,1,0)*qConj).imaginary, 0 );
			output.v[2] = Vector4<ElementType>( (q*Vector3<ElementType>(0,0,1)*qConj).imaginary, 0 );
			output.v[3] = Vector4<ElementType>( 0, 0, 0, 1 );
		}

		/*
			returns a deltaAngularAxis which is a vectorProduct of the movementVector and leverVector.
			angular: (1/I) * L, there I is known as the "moment of inertia", L as the "angular momentum vector".
			lever: Displacement vector relative to the rotation pivot.
			Recommended reading: http://en.wikipedia.org/wiki/Torque
		*/
		template<typename ElementType>
		inline Vector3<ElementType> deltaAngularAxis( const Vector3<ElementType> &movement, const Vector3<ElementType> &lever )
		{ return movement.cross( lever ); }

		template<typename ElementType>
		inline Vector3<ElementType> particleRotationMovement( const Vector3<ElementType> &deltaRadian, const Vector3<ElementType> &lever )
		{ return lever.cross(deltaRadian) /= lever.dot(lever); }

		template<typename ElementType>
		inline Vector3<ElementType> vectorProjection( const Vector3<ElementType> &vector, const Vector3<ElementType> &axis )
		{ return axis * ( vector.dot(axis) / axis.dot(axis) ); }

		/*
			output; is set to a rigibody matrix that revolve/rotate around centerOfMass and then translates.
			sumDeltaAngularAxis: Sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
			sumTranslation: Sum of all the translation vectors.
			centerOfMass: The point the particles is to revolve around, prior to translation. Default set to null vector aka origo.
			Recommended reading: http://en.wikipedia.org/wiki/Torque
		*/
		template<typename ElementType>
		void rigidBodyMatrix( Matrix4x4<ElementType> &output, const Vector3<ElementType> &sumDeltaAngularAxis, const Vector3<ElementType> &sumTranslation, const Vector3<ElementType> &centerOfMass = Vector3<ElementType>::null )
		{
			ElementType deltaRadian = sumDeltaAngularAxis.length();
			if( deltaRadian != 0 )
			{
				Vector3<ElementType> axis = sumDeltaAngularAxis / deltaRadian;
				rotationMatrix( output, axis, deltaRadian );

				output.v[3].xyz = centerOfMass;
				output.v[3].x -= centerOfMass.dot( output.v[0].xyz );
				output.v[3].y -= centerOfMass.dot( output.v[1].xyz );
				output.v[3].z -= centerOfMass.dot( output.v[2].xyz );
			}
			else output = Matrix4x4<ElementType>::identity;

			output.v[3].xyz += sumTranslation;
		}

		/*
			output; is set to an orthographic projection matrix.
			width; of the projection sample volume.
			height; of the projection sample volume.
			nearClip: Distance to the nearClippingPlane.
			farClip: Distance to the farClippingPlane
		*/
		template<typename ElementType>
		void projectionMatrix_Orthographic( Matrix4x4<ElementType> &output, const ElementType &width, const ElementType &height, const ElementType &nearClip, const ElementType &farClip )
		{
			ElementType c = 1;
			c /= nearClip - farClip;
			output = Matrix4x4<ElementType>( 2/width, 0, 0, 0,
											0, 2/height, 0, 0,
											0, 0, -c, 0, 0,
											0, nearClip*c, 1 );
		}

		/*
			output; is set to a perspective transform matrix.
			vertFoV; is the vertical field of vision in radians. (se FoV Hor+ )
			aspect; is the screenratio width/height (example 16/9 or 16/10 )
			nearClip: Distance to the nearClippingPlane
			farClip: Distance to the farClippingPlane
		*/
		template<typename ElementType>
		void projectionMatrix_Perspective( Matrix4x4<ElementType> &output, const ElementType &vertFoV, const ElementType &aspect, const ElementType &nearClip, const ElementType &farClip )
		{
			ElementType fov = 1 / ::std::tan( vertFoV * 0.5f ),
						dDepth = farClip;
			dDepth /= farClip - nearClip;
			output = Matrix4x4<ElementType>( fov / aspect, 0, 0, 0, 0, fov, 0, 0, 0, 0, dDepth, 1, 0, 0, -(dDepth * nearClip), 0 );
		}
	}
}

#endif