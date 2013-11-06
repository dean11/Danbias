/////////////////////////////////////////////////////////////////////
// by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_MATH_H
#define OYSTER_MATH_H

#include "Utilities.h"
#include "LinearMath.h"
#include <limits>

namespace Oyster { namespace Math
{
	typedef float Float;

	typedef ::LinearAlgebra::Vector2<Float> Float2;
	typedef ::LinearAlgebra::Vector3<Float> Float3;
	typedef ::LinearAlgebra::Vector4<Float> Float4;

	typedef ::LinearAlgebra::Matrix2x2<Float> Float2x2;
	typedef ::LinearAlgebra::Matrix3x3<Float> Float3x3;
	typedef ::LinearAlgebra::Matrix4x4<Float> Float4x4;

	typedef Float4x4 Matrix;
	typedef Float2 Vector2;
	typedef Float3 Vector3;
	typedef Float4 Vector4;



	Float2 & operator *= ( Float2 &left, const Float2 &right );

	inline Float2 operator * ( const Float2 &left, const Float2 &right )
	{ return Float2(left) *= right; }

	inline Float2 operator * ( const Float &left, const Float2 &right )
	{ return Float2(right) *= left; }

	Float3 & operator *= ( Float3 &left, const Float3 &right );

	inline Float3 operator * ( const Float3 &left, const Float3 &right )
	{ return Float3(left) *= right; }

	inline Float3 operator * ( const Float &left, const Float3 &right )
	{ return Float3(right) *= left; }

	Float4 & operator *= ( Float4 &left, const Float4 &right );

	inline Float4 operator * ( const Float4 &left, const Float4 &right )
	{ return Float4(left) *= right; }

	inline Float4 operator * ( const Float &left, const Float4 &right )
	{ return Float4(right) *= left; }

	inline Float2x2 operator * ( const Float &left, const Float2x2 &right )
	{ return Float2x2(right) *= left; }

	inline Float3x3 operator * ( const Float &left, const Float3x3 &right )
	{ return Float3x3(right) *= left; }

	inline Float4x4 operator * ( const Float &left, const Float4x4 &right )
	{ return Float4x4(right) *= left; }

	// Deprecated function! Use the static const member identity instead.
	inline void identityMatrix( Float2x2 &output )
	{ output = Float2x2::identity; }

	// Deprecated function! Use the static const member identity instead.
	inline void identityMatrix( Float3x3 &output )
	{ output = Float3x3::identity; }

	// Deprecated function! Use the static const member identity instead.
	inline void identityMatrix( Float4x4 &output )
	{ output = Float4x4::identity; }

	// If rigidBody is assumed to be by all definitions a rigid body matrix. Then this is a faster inverse method.
	inline void inverseRigidBodyMatrix( Float4x4 &output, const Float4x4 &rigidBody )
	{ ::LinearAlgebra::_3D::inverseRigidBody( output, rigidBody ); }

	inline void translationMatrix( Float4x4 &output, const Float3 &position )
	{ ::LinearAlgebra::_3D::translationMatrix( output, position ); }

	// counterclockwise rotation around X axis
	inline void rotationMatrix_AxisX( Float4x4 &output, const Float &radian )
	{ ::LinearAlgebra::_3D::rotationMatrix_AxisX( output, radian ); }

	// counterclockwise rotation around Y axis
	inline void rotationMatrix_AxisY( Float4x4 &output, const Float &radian )
	{ ::LinearAlgebra::_3D::rotationMatrix_AxisY( output, radian ); }

	// counterclockwise rotation around Z axis
	inline void rotationMatrix_AxisZ( Float4x4 &output, const Float &radian )
	{ ::LinearAlgebra::_3D::rotationMatrix_AxisZ( output, radian ); }
	
	// counterclockwise rotation around any given Float3 vector (normalizedAxis). Please make sure it is normalized.
	inline void rotationMatrix( Float4x4 &output, const Float &radian, const Float3 &normalizedAxis )
	{ ::LinearAlgebra::_3D::rotationMatrix( output, normalizedAxis, radian ); }

	/*
		returns a deltaAngularAxis which is a vectorProduct of the particleMovementVector and leverVector.
		angular: (1/I) * L, there I is known as the "moment of inertia", L as the "angular momentum vector".
		lever: Displacement vector relative to the center of mass.
		Recommended reading: http://en.wikipedia.org/wiki/Torque
	*/
	inline Float3 deltaAngularAxis( const Float3 &movement, const Float3 &lever )
	{ return ::LinearAlgebra::_3D::deltaAngularAxis( movement, lever ); }

	inline Float3 particleRotationMovement( const Float3 &deltaRadian, const Float3 &lever )
	{ return ::LinearAlgebra::_3D::particleRotationMovement( deltaRadian, lever ); }

	inline Float3 vectorProjection( const Float3 &vector, const Float3 &axis )
	{ return ::LinearAlgebra::_3D::vectorProjection( vector, axis ); }

	/*
		output: is set to a rigibody matrix that revolve/rotate around centerOfMass and then translates.
		sumDeltaAngularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
		sumTranslation: sum of all the translation vectors.
		centerOfMass: the point the particles is to revolve around, prior to translation. Default set to null vector aka origo.
		Recommended reading: http://en.wikipedia.org/wiki/Torque
	*/
	inline void rigidBodyMatrix( Float4x4 &output, const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, const Float3 &centerOfMass = Float3::null )
	{ ::LinearAlgebra::_3D::rigidBodyMatrix( output, sumDeltaAngularAxis, sumTranslation, centerOfMass ); }

	/*
		output; is set to an orthographic projection matrix.
		width; of the projection sample volume.
		height; of the projection sample volume.
		near: Distance to the nearPlane.
		far: Distance to the farPlane
	*/
	inline void projectionMatrix_Orthographic( Float4x4 &output, const Float &width, const Float &height, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max() )
	{ ::LinearAlgebra::_3D::projectionMatrix_Orthographic( output, width, height, nearClip, farClip ); }

	/*
		output; is set to a perspective transform matrix.
		vertFoV; is the vertical field of vision in radians. (se FoV Hor+ )
		aspect; is the screenratio width/height (example 16/9 or 16/10 )
		near: Distance to the nearPlane
		far: Distance to the farPlane
	*/
	inline void projectionMatrix_Perspective( Float4x4 &output, const Float &verticalFoV, const Float &aspectRatio, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max() )
	{ ::LinearAlgebra::_3D::projectionMatrix_Perspective( output, verticalFoV, aspectRatio, nearClip, farClip ); }

	inline Float4x4 & viewProjectionMatrix( Float4x4 &output, const Float4x4 &view, const Float4x4 &projection )
	{ return output = (view * projection).getTranspose(); }

	inline Float4x4 & transformMatrix( Float4x4 &output, const Float4x4 &transformee, const Float4x4 &transformer )
	{ return output = transformee * transformer; }

	inline Float4x4 transformMatrix( const Float4x4 &transformee, const Float4x4 &transformer )
	{ return transformee * transformer; }

	inline Float4 & transformVector( Float4 &output, const Float4 &transformee, const Float4x4 &transformer )
	{ return output = transformer * transformee; }

	inline Float4 transformVector( const Float4 &transformee, const Float4x4 &transformer )
	{ return transformee * transformer; }
} }

namespace Utility { namespace Value
{ // Utility Value Specializations
	using namespace ::Oyster::Math;

	template< > inline Float2 abs<Float2>( const Float2 &value )
	{ return Float2( abs(value.x), abs(value.y) ); }

	template< > inline Float2 max<Float2>( const Float2 &valueA, const Float2 &valueB )
	{ return Float2( max(valueA.x, valueB.x), max(valueA.y, valueB.y) ); }

	template< > inline Float2 min<Float2>( const Float2 &valueA, const Float2 &valueB )
	{ return Float2( min(valueA.x, valueB.x), min(valueA.y, valueB.y) ); }

	template< > inline Float3 abs<Float3>( const Float3 &value )
	{ return Float3( abs(value.xy), abs(value.z) ); }

	template< > inline Float3 max<Float3>( const Float3 &valueA, const Float3 &valueB )
	{ return Float3( max(valueA.xy, valueB.xy), max(valueA.z, valueB.z) ); }

	template< > inline Float3 min<Float3>( const Float3 &valueA, const Float3 &valueB )
	{ return Float3( min(valueA.xy, valueB.xy), min(valueA.z, valueB.z) ); }

	template< > inline Float4 abs<Float4>( const Float4 &value )
	{ return Float4( abs(value.xyz), abs(value.w) ); }

	template< > inline Float4 max<Float4>( const Float4 &valueA, const Float4 &valueB )
	{ return Float4( max(valueA.xyz, valueB.xyz), max(valueA.w, valueB.w) ); }

	template< > inline Float4 min<Float4>( const Float4 &valueA, const Float4 &valueB )
	{ return Float4( min(valueA.xyz, valueB.xyz), min(valueA.w, valueB.w) ); }

	template< > inline Float2x2 abs<Float2x2>( const Float2x2 &value )
	{ return Float2x2( abs(value.v[0]), abs(value.v[1]) ); }

	template< > inline Float2x2 max<Float2x2>( const Float2x2 &valueA, const Float2x2 &valueB )
	{ return Float2x2( max(valueA.v[0], valueB.v[0]), max(valueA.v[1], valueB.v[1]) ); }

	template< > inline Float2x2 min<Float2x2>( const Float2x2 &valueA, const Float2x2 &valueB )
	{ return Float2x2( min(valueA.v[0], valueB.v[0]), min(valueA.v[1], valueB.v[1]) ); }

	template< > inline Float3x3 abs<Float3x3>( const Float3x3 &value )
	{ return Float3x3( abs(value.v[0]), abs(value.v[1]), abs(value[2]) ); }

	template< > inline Float3x3 max<Float3x3>( const Float3x3 &valueA, const Float3x3 &valueB )
	{ return Float3x3( max(valueA.v[0], valueB.v[0]), max(valueA.v[1], valueB.v[1]), max(valueA.v[2], valueB.v[2]) ); }

	template< > inline Float3x3 min<Float3x3>( const Float3x3 &valueA, const Float3x3 &valueB )
	{ return Float3x3( min(valueA.v[0], valueB.v[0]), min(valueA.v[1], valueB.v[1]), min(valueA.v[2], valueB.v[2]) ); }

	template< > inline Float4x4 abs<Float4x4>( const Float4x4 &value )
	{ return Float4x4( abs(value.v[0]), abs(value.v[1]), abs(value[2]), abs(value[3]) ); }

	template< > inline Float4x4 max<Float4x4>( const Float4x4 &valueA, const Float4x4 &valueB )
	{ return Float4x4( max(valueA.v[0], valueB.v[0]), max(valueA.v[1], valueB.v[1]), max(valueA.v[2], valueB.v[2]), max(valueA.v[3], valueB.v[3]) ); }

	template< > inline Float4x4 min<Float4x4>( const Float4x4 &valueA, const Float4x4 &valueB )
	{ return Float4x4( min(valueA.v[0], valueB.v[0]), min(valueA.v[1], valueB.v[1]), min(valueA.v[2], valueB.v[2]), min(valueA.v[3], valueB.v[3]) ); }
} }

#endif