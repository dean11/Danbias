/////////////////////////////////////////////////////////////////////
// by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_MATH_H
#define OYSTER_MATH_H

#include "Utilities.h"
#include "LinearMath.h"
#include <limits>

namespace Oyster { namespace Math /// Oyster's native math library
{
	typedef float Float; /// Oyster's native scalar is float

	typedef ::LinearAlgebra::Vector2<Float> Float2; /// 2D Linear Vector for Oyster
	typedef ::LinearAlgebra::Vector3<Float> Float3; /// 3D Linear Vector for Oyster
	typedef ::LinearAlgebra::Vector4<Float> Float4; /// 4D Linear Vector for Oyster

	typedef ::LinearAlgebra::Matrix2x2<Float> Float2x2; /// 2x2 Linear Matrix for Oyster
	typedef ::LinearAlgebra::Matrix3x3<Float> Float3x3; /// 3x3 Linear Matrix for Oyster
	typedef ::LinearAlgebra::Matrix4x4<Float> Float4x4; /// 4x4 Linear Matrix for Oyster

	typedef Float4x4 Matrix; // by popular demand
	typedef Float2 Vector2;  // by popular demand
	typedef Float3 Vector3;  // by popular demand
	typedef Float4 Vector4;  // by popular demand

	/// Function Highly recommended to check at start, just in case current version is using a feature that might be available.
	/// @todo TODO: create a template UniquePointer to use here
	inline bool IsSupported()
	{ return true; }

	/// Creates a solution matrix for 'out�= 'targetMem' * 'in'.
	/// Returns false if there is no explicit solution.
	inline bool SuperpositionMatrix( const Float2x2 &in, const Float2x2 &out, Float2x2 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }

	/// Creates a solution matrix for 'out�= 'targetMem' * 'in'.
	/// Returns false if there is no explicit solution.
	inline bool SuperpositionMatrix( const Float3x3 &in, const Float3x3 &out, Float3x3 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }

	/// Creates a solution matrix for 'out�= 'targetMem' * 'in'.
	/// Returns false if there is no explicit solution.
	inline bool SuperpositionMatrix( const Float4x4 &in, const Float4x4 &out, Float4x4 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }
} }

inline ::Oyster::Math::Float2 & operator *= ( ::Oyster::Math::Float2 &left, const ::Oyster::Math::Float2 &right )
{
	left.x *= right.x;
	left.y *= right.y;
	return left;
}

inline ::Oyster::Math::Float2 operator * ( const ::Oyster::Math::Float2 &left, const ::Oyster::Math::Float2 &right )
{ return ::Oyster::Math::Float2(left) *= right; }

inline ::Oyster::Math::Float2 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float2 &right )
{ return ::Oyster::Math::Float2(right) *= left; }

inline ::Oyster::Math::Float3 & operator *= ( ::Oyster::Math::Float3 &left, const ::Oyster::Math::Float3 &right )
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;
	return left;
}

inline ::Oyster::Math::Float3 operator * ( const ::Oyster::Math::Float3 &left, const ::Oyster::Math::Float3 &right )
{ return ::Oyster::Math::Float3(left) *= right; }

inline ::Oyster::Math::Float3 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float3 &right )
{ return ::Oyster::Math::Float3(right) *= left; }

inline ::Oyster::Math::Float4 & operator *= ( ::Oyster::Math::Float4 &left, const ::Oyster::Math::Float4 &right )
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;
	left.w *= right.w;
	return left;
}

inline ::Oyster::Math::Float4 operator * ( const ::Oyster::Math::Float4 &left, const ::Oyster::Math::Float4 &right )
{ return ::Oyster::Math::Float4(left) *= right; }

inline ::Oyster::Math::Float4 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float4 &right )
{ return ::Oyster::Math::Float4(right) *= left; }

inline ::Oyster::Math::Float2x2 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float2x2 &right )
{ return ::Oyster::Math::Float2x2(right) *= left; }

inline ::Oyster::Math::Float3x3 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float3x3 &right )
{ return ::Oyster::Math::Float3x3(right) *= left; }

inline ::Oyster::Math::Float4x4 operator * ( const ::Oyster::Math::Float &left, const ::Oyster::Math::Float4x4 &right )
{ return ::Oyster::Math::Float4x4(right) *= left; }

namespace Oyster { namespace Math2D /// Oyster's native math library specialized for 2D
{
	using namespace ::Oyster::Math; // deliberate inheritance from ::Oyster::Math namespace

	/// If there is an Y-axis on a 2D plane, then there is an explicit X-axis on and that is what is returned.
	/// Recommended too make sure that yAxis is normalized.
	inline Float2 X_AxisTo( const Float2 &yAxis )
	{ return ::LinearAlgebra2D::X_AxisTo(yAxis); }

	/// If there is an X-axis on a 2D plane, then there is an explicit Y-axis and that is what is returned.
	/// Recommended too make sure that yAxis is normalized.
	inline Float2 Y_AxisTo( const Float2 &xAxis )
	{ return ::LinearAlgebra2D::Y_AxisTo(xAxis); }

	/// Sets and returns targetMem to a translationMatrix with position as translation. 
	inline Float3x3 & TranslationMatrix( const Float2 &position, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::TranslationMatrix( position, targetMem ); }

	/// Sets and returns targetMem as a counterclockwise rotationMatrix
	inline Float3x3 & RotationMatrix( const Float &radian, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::RotationMatrix( radian, targetMem ); }

	/// Sets and returns targetMem as an orientation Matrix with position as translation and radian rotation
	inline Float3x3 & OrientationMatrix( const Float2 &position, const Float &radian, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::OrientationMatrix( radian, position, targetMem ); }

	/// Sets and returns targetMem as an orientation Matrix with position as translation and local y-axis directed at lookAt
	inline Float3x3 & OrientationMatrix( const Float2 &position, const Float2 &lookAt, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::OrientationMatrix( lookAt, position, targetMem ); }

	/// Sets and returns targetMem as an orientation Matrix that is rotated around localCenterOfRotation and then translated with position.
	/// TODO: not tested
	inline Float3x3 & OrientationMatrix( const Float2 &position, Float radian, const Float2 &localCenterOfRotation, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::OrientationMatrix( radian, position, localCenterOfRotation, targetMem ); }

	/// If orientationMatrix is assumed to be by all definitions a rigid orientation matrix aka rigid body matrix. Then this is a much faster inverse method.
	inline Float3x3 & InverseOrientationMatrix( const Float3x3 &orientationMatrix, Float3x3 &targetMem = Float3x3() )
	{ return ::LinearAlgebra2D::InverseOrientationMatrix( orientationMatrix, targetMem ); }
} }

namespace Oyster { namespace Math3D /// Oyster's native math library specialized for 3D
{
	using namespace ::Oyster::Math; // deliberate inheritance from ::Oyster::Math namespace

	/// Sets and returns targetMem to a translationMatrix with position as translation. 
	inline Float4x4 & TranslationMatrix( const Float3 &position, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::TranslationMatrix( position, targetMem ); }

	/// Sets and returns targetMem as an counterclockwise rotation matrix around the global X-axis
	inline Float4x4 & RotationMatrix_AxisX( const Float &radian, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisX( radian, targetMem ); }

	/// Sets and returns targetMem as an counterclockwise rotation matrix around the global Y-axis
	inline Float4x4 & RotationMatrix_AxisY( const Float &radian, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisY( radian, targetMem ); }

	/// Sets and returns targetMem as an counterclockwise rotation matrix around the global Z-axis
	inline Float4x4 & RotationMatrix_AxisZ( const Float &radian, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisZ( radian, targetMem ); }

	/// Sets and returns targetMem as an counterclockwise rotation matrix around the normalizedAxis.
	/// Please make sure normalizedAxis is normalized.
	inline Float4x4 & RotationMatrix( const Float &radian, const Float3 &normalizedAxis, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::RotationMatrix( normalizedAxis, radian, targetMem ); }

	/// If orientationMatrix is assumed to be by all definitions a rigid orientation matrix aka rigid body matrix. Then this is a much faster inverse method.
	inline Float4x4 & InverseOrientationMatrix( const Float4x4 &orientationMatrix, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::InverseOrientationMatrix( orientationMatrix, targetMem ); }

	/**	Sets and returns targetMem as an orientation Matrix
	 *	@param targetMem: is set to a rigibody matrix that rotate counterclockwise and then translates.
	 *	@param sumDeltaAngularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
	 *	@param sumTranslation: sum of all the translation vectors.
	 *	@return targetMem
		@todo TODO: not tested
	*/
	inline Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::OrientationMatrix( sumDeltaAngularAxis, sumTranslation, targetMem ); }

	/**	Sets and returns targetMem as an orientation Matrix
	 *	@param targetMem: is set to a rigibody matrix that revolve/rotate counterclockwise around centerOfMass and then translates.
	 *	@param sumDeltaAngularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
	 *	@param sumTranslation: sum of all the translation vectors.
	 *	@param centerOfMass: the point the particles is to revolve around, prior to translation. Default set to null vector aka origo.
	 *	@return targetMem
		@todo TODO: not tested 
	*/
	inline Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, const Float3 &centerOfMass, Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::OrientationMatrix( sumDeltaAngularAxis, sumTranslation, centerOfMass, targetMem ); }

	/**	Creates an orthographic projection matrix designed for DirectX enviroment.
	 *	@param targetMem; is set to an orthographic projection matrix.
	 *	@param width; of the projection sample volume.
	 *	@param height; of the projection sample volume.
	 *	@param nearClip: Distance to the nearPlane.
	 *	@param farClip: Distance to the farPlane.
	 *	@return targetMem
	 	@todo TODO: not tested
	*/
	inline Float4x4 & ProjectionMatrix_Orthographic( const Float &width, const Float &height, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max(), Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::ProjectionMatrix_Orthographic( width, height, nearClip, farClip, targetMem ); }

	/**	Creates a perspective projection matrix designed for DirectX enviroment.
	 *	@param targetMem; is set to a perspective transform matrix.
	 *	@param vertFoV; is the vertical field of vision in radians. (lookup FoV Hor+ )
	 *	@param aspect; is the screenratio width/height (example 16/9 or 16/10 )
	 *	@param nearClip: Distance to the nearPlane
	 *	@param farClip: Distance to the farPlane
	 *	@return targetMem
		@todo TODO: not tested
	*/
	inline Float4x4 & ProjectionMatrix_Perspective( const Float &verticalFoV, const Float &aspectRatio, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max(), Float4x4 &targetMem = Float4x4() )
	{ return ::LinearAlgebra3D::ProjectionMatrix_Perspective( verticalFoV, aspectRatio, nearClip, farClip, targetMem ); }

	/// returns the component vector of vector that is parallell with axis
	inline Float3 VectorProjection( const Float3 &vector, const Float3 &axis )
	{ return ::LinearAlgebra3D::VectorProjection( vector, axis ); }

	/// Helper inline function that sets and then returns targetMem = projection * view
	inline Float4x4 & ViewProjectionMatrix( const Float4x4 &view, const Float4x4 &projection, Float4x4 &targetMem = Float4x4() )
	{ return targetMem = projection * view; }

	/// Helper inline function that sets and then returns targetMem = transformer * transformee
	inline Float4x4 & TransformMatrix( const Float4x4 &transformer, const Float4x4 &transformee, Float4x4 &targetMem = Float4x4() )
	{ return targetMem = transformer * transformee; }

	/// Helper inline function that sets and then returns targetMem = transformer * transformee
	inline Float4 & TransformVector( const Float4x4 &transformer, const Float4 &transformee, Float4 &targetMem = Float4() )
	{ return targetMem = transformer * transformee; }
} }

#endif