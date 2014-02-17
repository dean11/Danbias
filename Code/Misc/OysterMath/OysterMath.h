/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#ifndef OYSTER_MATH_H
#define OYSTER_MATH_H

#include "Utilities.h"
#include "LinearMath.h"
#include <limits>

namespace Oyster { namespace Math //! Oyster's native math library
{
	typedef float Float; //!< Oyster's native scalar is float

	typedef ::LinearAlgebra::Vector2<Float> Float2; //!< 2D Linear Vector for Oyster
	typedef ::LinearAlgebra::Vector3<Float> Float3; //!< 3D Linear Vector for Oyster
	typedef ::LinearAlgebra::Vector4<Float> Float4; //!< 4D Linear Vector for Oyster

	typedef ::LinearAlgebra::Matrix2x2<Float> Float2x2; //!< 2x2 Linear Matrix for Oyster
	typedef ::LinearAlgebra::Matrix3x3<Float> Float3x3; //!< 3x3 Linear Matrix for Oyster
	typedef ::LinearAlgebra::Matrix4x4<Float> Float4x4; //!< 4x4 Linear Matrix for Oyster

	typedef ::LinearAlgebra::Quaternion<Float> Quaternion; //!< Quaternion for Oyster

	typedef Float4x4 Matrix; // by popular demand
	typedef Float2 Vector2;  // by popular demand
	typedef Float3 Vector3;  // by popular demand
	typedef Float4 Vector4;  // by popular demand

	const Float pi = 3.1415926535897932384626433832795f;

 //! Function Highly recommended to check at start, just in case current version is using a feature that might be available.
 //! @todo TODO: create a template UniquePointer to use here
	bool IsSupported();

 //! Creates a solution matrix for 'out´= 'targetMem' * 'in'.
 //! Returns false if there is no explicit solution.
	bool SuperpositionMatrix( const Float2x2 &in, const Float2x2 &out, Float2x2 &targetMem );

 //! Creates a solution matrix for 'out´= 'targetMem' * 'in'.
 //! Returns false if there is no explicit solution.
	bool SuperpositionMatrix( const Float3x3 &in, const Float3x3 &out, Float3x3 &targetMem );

 //! Creates a solution matrix for 'out´= 'targetMem' * 'in'.
 //! Returns false if there is no explicit solution.
	bool SuperpositionMatrix( const Float4x4 &in, const Float4x4 &out, Float4x4 &targetMem );

	/********************************************************************
	 * Linear Interpolation
	 * @return start * (1-t) + end * t
	 ********************************************************************/
	using ::LinearAlgebra::Lerp;

	/********************************************************************
	 * Normalized Linear Interpolation
	 * @return nullvector if Lerp( start, end, t ) is nullvector.
	 ********************************************************************/
	using ::LinearAlgebra::Nlerp;

	/********************************************************************
	 * Spherical Linear Interpolation on Quaternions
	 ********************************************************************/
	using ::LinearAlgebra::Slerp;
} }

inline ::Oyster::Math::Float2 & operator *= ( ::Oyster::Math::Float2 &left, const ::Oyster::Math::Float2 &right )
{
	return left.PiecewiseMultiplicationAdd( right );
}

inline ::Oyster::Math::Float2 operator * ( const ::Oyster::Math::Float2 &left, const ::Oyster::Math::Float2 &right )
{
	return left.PiecewiseMultiplication( right );
}

inline ::Oyster::Math::Float3 & operator *= ( ::Oyster::Math::Float3 &left, const ::Oyster::Math::Float3 &right )
{
	return left.PiecewiseMultiplicationAdd( right );
}

inline ::Oyster::Math::Float3 operator * ( const ::Oyster::Math::Float3 &left, const ::Oyster::Math::Float3 &right )
{
	return left.PiecewiseMultiplication( right );
}

inline ::Oyster::Math::Float4 & operator *= ( ::Oyster::Math::Float4 &left, const ::Oyster::Math::Float4 &right )
{
	return left.PiecewiseMultiplicationAdd( right );
}

namespace Oyster { namespace Math2D //! Oyster's native math library specialized for 2D
{
	using namespace ::Oyster::Math; // deliberate inheritance from ::Oyster::Math namespace

	//! If there is an Y-axis on a 2D plane, then there is an explicit X-axis on and that is what is returned.
	//! Recommended too make sure that yAxis is normalized.
	Float2 X_AxisTo( const Float2 &yAxis );

	//! If there is an X-axis on a 2D plane, then there is an explicit Y-axis and that is what is returned.
	//! Recommended too make sure that yAxis is normalized.
	Float2 Y_AxisTo( const Float2 &xAxis );

	//! Sets and returns targetMem to a translationMatrix with position as translation. 
	Float3x3 & TranslationMatrix( const Float2 &position, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as a counterclockwise rotationMatrix
	Float3x3 & RotationMatrix( const Float &radian, Float3x3 &targetMem = Float3x3() );

	//! If rotation is assumed to be by all definitions a rotation matrix. Then this is a much faster inverse method.
	Float2x2 & InverseRotationMatrix( const Float2x2 &rotation, Float2x2 &targetMem = Float2x2() );
	
	//! If rotation is assumed to be by all definitions a rotation matrix. Then this is a much faster inverse method.
	Float3x3 & InverseRotationMatrix( const Float3x3 &rotation, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as an orientation Matrix composed by the rotation matrix and translation vector
	Float3x3 & OrientationMatrix( const Float2x2 &rotation, const Float2 &translation, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as an orientation Matrix composed by the rotation matrix and translation vector
	Float3x3 & OrientationMatrix( const Float3x3 &rotation, const Float2 &translation, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as an orientation Matrix with position as translation and radian rotation
	Float3x3 & OrientationMatrix( const Float2 &position, const Float &radian, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as an orientation Matrix with position as translation and local y-axis directed at lookAt
	Float3x3 & OrientationMatrix( const Float2 &position, const Float2 &lookAt, Float3x3 &targetMem = Float3x3() );

	//! Sets and returns targetMem as an orientation Matrix that is rotated around localCenterOfRotation and then translated with position.
	//! TODO: not tested
	Float3x3 & OrientationMatrix( const Float2 &position, Float radian, const Float2 &localCenterOfRotation, Float3x3 &targetMem = Float3x3() );

	//! If orientationMatrix is assumed to be by all definitions a rigid orientation matrix aka rigid body matrix. Then this is a much faster inverse method.
	Float3x3 & InverseOrientationMatrix( const Float3x3 &orientationMatrix, Float3x3 &targetMem = Float3x3() );

	//! Returns targetmem after writing the rotation data from orientation, into it.
	Float3x3 & ExtractRotationMatrix( const Float3x3 &orientation, Float3x3 &targetMem = Float3x3() );
} }

namespace Oyster { namespace Math3D //! Oyster's native math library specialized for 3D
{
	using namespace ::Oyster::Math; // deliberate inheritance from ::Oyster::Math namespace

	//! Converts a rotationQuaternion to an angularAxis
	Float3 AngularAxis( const Quaternion &rotation );

	//! Extracts the angularAxis from rotationMatrix
	//Float4 AngularAxis( const Float3x3 &rotationMatrix );

	////! Extracts the angularAxis from rotationMatrix
	//Float4 AngularAxis( const Float4x4 &rotationMatrix );

	////! Extracts the angularAxis from orientationMatrix
	//Float4 ExtractAngularAxis( const Float4x4 &orientationMatrix );

	//! Sets and returns targetMem to a translationMatrix with position as translation. 
	Float4x4 & TranslationMatrix( const Float3 &position, Float4x4 &targetMem = Float4x4() );

	/** @todo TODO: add doc */
	Quaternion Rotation( Float radian, const Float3 &normalizedAxis );

	/** @todo TODO: add doc */
	Quaternion Rotation( Float radian, const Float3 &normalizedAxis );

	/** @todo TODO: add doc */
	Quaternion Rotation( const Float3 &angularAxis );

	/** @todo TODO: add doc */
	Quaternion Rotation( const Float4 &angularAxis );

	/** @todo TODO: add doc */
	Float3x3 & RotationMatrix( const Quaternion &rotationQuaternion, Float3x3 &targetMem );

	/** @todo TODO: add doc */
	Float4x4 & RotationMatrix( const Quaternion &rotationQuaternion, Float4x4 &targetMem = Float4x4() );

	/** @todo TODO: add doc */
	Float4x4 & OrientationMatrix( const Quaternion &rotationQuaternion, const Float3 &translation, Float4x4 &targetMem = Float4x4() );

	/** @todo TODO: add doc */
	Float4x4 & OrientationMatrix( const Quaternion &rotationQuaternion, const Float4 &translation, Float4x4 &targetMem = Float4x4() );

	/** @todo TODO: add doc */
	Float4x4 & ViewMatrix( const Quaternion &rotationQuaternion, const Float3 &translation, Float4x4 &targetMem = Float4x4() );
	
	/** @todo TODO: add doc */
	Float4x4 & ViewMatrix( const Quaternion &rotationQuaternion, const Float4 &translation, Float4x4 &targetMem = Float4x4() );

	//! Sets and returns targetMem as an counterclockwise rotation matrix around the global X-axis
	Float4x4 & RotationMatrix_AxisX( const Float &radian, Float4x4 &targetMem = Float4x4() );

	//! Sets and returns targetMem as an counterclockwise rotation matrix around the global Y-axis
	Float4x4 & RotationMatrix_AxisY( const Float &radian, Float4x4 &targetMem = Float4x4() );

	 //! Sets and returns targetMem as an counterclockwise rotation matrix around the global Z-axis
	Float4x4 & RotationMatrix_AxisZ( const Float &radian, Float4x4 &targetMem = Float4x4() );

	 //! Sets and returns targetMem as an counterclockwise rotation matrix around the angularAxis.
	Float4x4 & RotationMatrix( const Float3 &angularAxis, Float4x4 &targetMem = Float4x4() );

	 //! Sets and returns targetMem as an counterclockwise rotation matrix around the normalizedAxis.
	 //! Please make sure normalizedAxis is normalized.
	Float4x4 & RotationMatrix( const Float &radian, const Float3 &normalizedAxis, Float4x4 &targetMem = Float4x4() );

	 //! If rotation is assumed to be by all definitions a rotation matrix. Then this is a much faster inverse method.
	Float3x3 & InverseRotationMatrix( const Float3x3 &rotation, Float3x3 &targetMem = Float3x3() );

	 //! If rotation is assumed to be by all definitions a rotation matrix. Then this is a much faster inverse method.
	Float4x4 & InverseRotationMatrix( const Float4x4 &rotation, Float4x4 &targetMem = Float4x4() );
	
	 //! Sets and returns targetMem as an orientation Matrix composed by the rotation matrix and translation vector
	Float4x4 & OrientationMatrix( const Float3x3 &rotation, const Float3 &translation, Float4x4 &targetMem = Float4x4() );

	 //! Sets and returns targetMem as an orientation Matrix composed by the rotation matrix and translation vector
	Float4x4 & OrientationMatrix( const Float4x4 &rotation, const Float3 &translation, Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 * Sets and returns targetMem as an orientation Matrix
	 * @param normalizedAxis: The normalized vector parallell with the rotationAxis.
	 * @param deltaRadian: The rotation angle.
	 * @param sumTranslation: sum of all the translation vectors.
	 * @param targetMem: is set to a rigibody matrix that rotate counterclockwise and then translates.
	 * @return targetMem
	 *******************************************************************/
	Float4x4 & OrientationMatrix( const Float3 &normalizedAxis, const Float & deltaRadian, const Float3 &sumTranslation, Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 *	Sets and returns targetMem as an orientation Matrix
	 *	@param angularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
	 *	@param translation: sum of all the translation vectors.
	 *	@param targetMem: is set to a rigibody matrix that rotate counterclockwise and then translates.
	 *	@return targetMem
	 *******************************************************************/
	Float4x4 & OrientationMatrix( const Float3 &angularAxis, const Float3 &translation, Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 *	Sets and returns targetMem as a view Matrix
	 *	@param angularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
	 *	@param translation: sum of all the translation vectors.
	 *	@param targetMem: is set to a rigibody matrix that rotate counterclockwise and then translates.
	 *	@return targetMem
	 *******************************************************************/
	Float4x4 & ViewMatrix( const Float3 &angularAxis, const Float3 &translation, Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 *	Sets and returns targetMem as an orientation Matrix
	 *	@param sumDeltaAngularAxis: sum of all ( (1/I) * ( L x D ) )-vectorproducts. There I is known as "moment of inertia", L as "angular momentum vector" and D the "lever vector".
	 *	@param sumTranslation: sum of all the translation vectors.
	 *	@param centerOfMass: the point the particles is to revolve around, prior to translation. Default set to null vector aka origo.
	 *	@param targetMem: is set to a rigibody matrix that revolve/rotate counterclockwise around centerOfMass and then translates.
	 *	@return targetMem
	 *	@todo TODO: not tested 
	 *******************************************************************/
	Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, const Float3 &centerOfMass, Float4x4 &targetMem = Float4x4() );

	//! @todo TODO: Add documentation and not tested
	Float4x4 & OrientationMatrix_LookAtDirection( const Float3 &normalizedDirection, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem = Float4x4() );

	//! @todo TODO: Add documentation and not tested
	Float4x4 & OrientationMatrix_LookAtPos( const Float3 &worldLookAt, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem = Float4x4() );

	//! @todo TODO: Add documentation and not tested
	Float4x4 & ViewMatrix_LookAtDirection( const Float3 &normalizedDirection, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem = Float4x4() );

	//! @todo TODO: Add documentation and not tested
	Float4x4 & ViewMatrix_LookAtPos( const Float3 &worldLookAt, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem = Float4x4() );

	//! If orientationMatrix is assumed to be by all definitions a rigid orientation matrix aka rigid body matrix. Then this is a much faster inverse method.
	Float4x4 & InverseOrientationMatrix( const Float4x4 &orientationMatrix, Float4x4 &targetMem = Float4x4() );

	// O0 = T0 * R0
	// O1 = T1 * T0 * R1 * R0
	Float4x4 & UpdateOrientationMatrix( const Float3 &deltaPosition, const Float4x4 &deltaRotationMatrix, Float4x4 &orientationMatrix );

	//! Returns targetmem after writing the rotation data from orientation, into it.
	Float4x4 & ExtractRotationMatrix( const Float4x4 &orientation, Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 *	Creates an orthographic projection matrix designed for DirectX enviroment.
	 *	@param width; of the projection sample volume.
	 *	@param height; of the projection sample volume.
	 *	@param nearClip: Distance to the nearPlane.
	 *	@param farClip: Distance to the farPlane.
	 *	@param targetMem; is set to an orthographic projection matrix.
	 *	@return targetMem
	 *	@todo TODO: not tested
	 *******************************************************************/
	Float4x4 & ProjectionMatrix_Orthographic( const Float &width, const Float &height, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max(), Float4x4 &targetMem = Float4x4() );

	/*******************************************************************
	 *	Creates a perspective projection matrix designed for DirectX enviroment.
	 *	@param vertFoV; is the vertical field of vision in radians. (lookup FoV Hor+ )
	 *	@param aspect; is the screenratio width/height (example 16/9 or 16/10 )
	 *	@param nearClip: Distance to the nearPlane
	 *	@param farClip: Distance to the farPlane
	 *	@param targetMem; is set to a perspective transform matrix.
	 *	@return targetMem
	 *	@todo TODO: not tested
	 *******************************************************************/
	Float4x4 & ProjectionMatrix_Perspective( const Float &verticalFoV, const Float &aspectRatio, const Float &nearClip = ::std::numeric_limits<Float>::epsilon(), const Float &farClip = ::std::numeric_limits<Float>::max(), Float4x4 &targetMem = Float4x4() );

	//! returns the component vector of vector that is parallell with axis
	Float3 VectorProjection( const Float3 &vector, const Float3 &axis );

	//! returns the component vector of vector that is parallell with axis
	Float4 VectorProjection( const Float4 &vector, const Float4 &axis );

	//! returns the component vector of vector that is parallell with axis. Faster than VectorProjection.
	Float3 NormalProjection( const Float3 &vector, const Float3 &normalizedAxis );

	//! returns the component vector of vector that is parallell with axis. Faster than VectorProjection.
	Float4 NormalProjection( const Float4 &vector, const Float4 &normalizedAxis );

	//! Helper inline function that sets and then returns targetMem = projection * view
	inline Float4x4 & ViewProjectionMatrix( const Float4x4 &view, const Float4x4 &projection, Float4x4 &targetMem = Float4x4() )
	{ return targetMem = projection * view; }

	/** Helper inline function that sets and then returns targetMem = transformer * transformee */
	inline Float4x4 & TransformMatrix( const Float4x4 &transformer, const Float4x4 &transformee, Float4x4 &targetMem )
	{ return targetMem = transformer * transformee; }

	/** Helper inline function that sets and then returns transformer * transformee */
	inline Float4x4 TransformMatrix( const Float4x4 &transformer, const Float4x4 &transformee )
	{ return transformer * transformee; }

	//! Helper inline function that sets and then returns targetMem = transformer * transformee
	inline Float4 & TransformVector( const Float4x4 &transformer, const Float4 &transformee, Float4 &targetMem = Float4() )
	{ return targetMem = transformer * transformee; }

	using ::LinearAlgebra3D::SnapAxisYToNormal_UsingNlerp;
	using ::LinearAlgebra3D::InterpolateAxisYToNormal_UsingNlerp;
	using ::LinearAlgebra3D::InterpolateRotation_UsingNonRigidNlerp;
	using ::LinearAlgebra3D::InterpolateRotation_UsingRigidNlerp;
	using ::LinearAlgebra3D::InterpolateOrientation_UsingNonRigidNlerp;
	using ::LinearAlgebra3D::InterpolateOrientation_UsingRigidNlerp;
	using ::LinearAlgebra3D::InterpolateOrientation_UsingSlerp;
	using ::LinearAlgebra3D::SnapAngularAxis;
	using ::LinearAlgebra3D::WorldAxisOf;
	using ::LinearAlgebra3D::ScalingMatrix;
} }

#endif