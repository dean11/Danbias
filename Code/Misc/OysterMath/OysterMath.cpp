/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "OysterMath.h"

namespace Oyster { namespace Math
{
	bool IsSupported()
	{ return true; }

	bool SuperpositionMatrix( const Float2x2 &in, const Float2x2 &out, Float2x2 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }

	bool SuperpositionMatrix( const Float3x3 &in, const Float3x3 &out, Float3x3 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }

	bool SuperpositionMatrix( const Float4x4 &in, const Float4x4 &out, Float4x4 &targetMem )
	{ return ::LinearAlgebra::SuperpositionMatrix( in, out, targetMem ); }
} }

namespace Oyster { namespace Math2D
{
	Float2 X_AxisTo( const Float2 &yAxis )
	{ return ::LinearAlgebra2D::X_AxisTo(yAxis); }

	Float2 Y_AxisTo( const Float2 &xAxis )
	{ return ::LinearAlgebra2D::Y_AxisTo(xAxis); }

	Float3x3 & TranslationMatrix( const Float2 &position, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::TranslationMatrix( position, targetMem ); }

	Float3x3 & RotationMatrix( const Float &radian, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::RotationMatrix( radian, targetMem ); }
	
	Float2x2 & InverseRotationMatrix( const Float2x2 &rotation, Float2x2 &targetMem )
	{
		return targetMem = ::LinearAlgebra2D::InverseRotationMatrix( rotation );
	}

	Float3x3 & InverseRotationMatrix( const Float3x3 &rotation, Float3x3 &targetMem )
	{
		return targetMem = ::LinearAlgebra2D::InverseRotationMatrix( rotation );
	}

	Float3x3 & OrientationMatrix( const Float2x2 &rotation, const Float2 &translation, Float3x3 &targetMem )
	{
		return targetMem = ::LinearAlgebra2D::OrientationMatrix( rotation, translation );
	}

	Float3x3 & OrientationMatrix( const Float3x3 &rotation, const Float2 &translation, Float3x3 &targetMem )
	{
		return targetMem = ::LinearAlgebra2D::OrientationMatrix( rotation, translation );
	}

	Float3x3 & OrientationMatrix( const Float2 &position, const Float &radian, Float3x3 &targetMem )
	{
		return ::LinearAlgebra2D::OrientationMatrix( radian, position, targetMem );
	}

	Float3x3 & OrientationMatrix( const Float2 &position, const Float2 &lookAt, Float3x3 &targetMem )
	{
		return ::LinearAlgebra2D::OrientationMatrix( lookAt, position, targetMem );
	}

	Float3x3 & OrientationMatrix( const Float2 &position, Float radian, const Float2 &localCenterOfRotation, Float3x3 &targetMem )
	{
		return ::LinearAlgebra2D::OrientationMatrix( radian, position, localCenterOfRotation, targetMem );
	}

	Float3x3 & InverseOrientationMatrix( const Float3x3 &orientationMatrix, Float3x3 &targetMem )
	{
		return ::LinearAlgebra2D::InverseOrientationMatrix( orientationMatrix, targetMem );
	}

	Float3x3 & ExtractRotationMatrix( const Float3x3 &orientation, Float3x3 &targetMem )
	{
		return targetMem = ::LinearAlgebra2D::ExtractRotationMatrix( orientation );
	}
} }

namespace Oyster { namespace Math3D
{
	//! Converts a rotationQuaternion to an angularAxis
	Float3 AngularAxis( const Quaternion &rotation )
	{
		return ::LinearAlgebra3D::AngularAxis( rotation );
	}

	//Float4 AngularAxis( const Float3x3 &rotationMatrix )
	//{
	//	return ::LinearAlgebra3D::AngularAxis( rotationMatrix );
	//}

	//Float4 AngularAxis( const Float4x4 &rotationMatrix )
	//{
	//	return ::LinearAlgebra3D::AngularAxis( rotationMatrix );
	//}

	//Float4 ExtractAngularAxis( const Float4x4 &orientationMatrix )
	//{
	//	return ::LinearAlgebra3D::ExtractAngularAxis( orientationMatrix );
	//}

	Float4x4 & TranslationMatrix( const Float3 &position, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::TranslationMatrix( position, targetMem );
	}

	Quaternion Rotation( Float radian, const Float3 &normalizedAxis )
	{
		return ::LinearAlgebra3D::Rotation( radian, normalizedAxis );
	}

	Quaternion Rotation( Float radian, const Float4 &normalizedAxis )
	{
		return ::LinearAlgebra3D::Rotation( radian, normalizedAxis );
	}

	Quaternion Rotation( const Float3 &angularAxis )
	{
		return ::LinearAlgebra3D::Rotation( angularAxis );
	}

	Quaternion Rotation( const Float4 &angularAxis )
	{
		return ::LinearAlgebra3D::Rotation( angularAxis );
	}

	Float3x3 & RotationMatrix( const Quaternion &rotationQuaternion, Float3x3 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix( rotationQuaternion, targetMem );
	}

	Float4x4 & RotationMatrix( const Quaternion &rotationQuaternion, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix( rotationQuaternion, targetMem );
	}

	Float4x4 & OrientationMatrix( const Quaternion &rotationQuaternion, const Float3 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::OrientationMatrix( rotationQuaternion, translation, targetMem );
	}

	Float4x4 & OrientationMatrix( const Quaternion &rotationQuaternion, const Float4 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::OrientationMatrix( rotationQuaternion, translation, targetMem );
	}

	Float4x4 & ViewMatrix( const Quaternion &rotationQuaternion, const Float3 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::ViewMatrix( rotationQuaternion, translation, targetMem );
	}

	Float4x4 & ViewMatrix( const Quaternion &rotationQuaternion, const Float4 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::ViewMatrix( rotationQuaternion, translation, targetMem );
	}

	Float4x4 & RotationMatrix_AxisX( const Float &radian, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix_AxisX( radian, targetMem );
	}

	Float4x4 & RotationMatrix_AxisY( const Float &radian, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix_AxisY( radian, targetMem );
	}

	Float4x4 & RotationMatrix_AxisZ( const Float &radian, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix_AxisZ( radian, targetMem );
	}

	Float4x4 & RotationMatrix( const Float3 &angularAxis, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::RotationMatrix( angularAxis );
	}

	Float4x4 & RotationMatrix( const Float &radian, const Float3 &normalizedAxis, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::RotationMatrix( normalizedAxis, radian, targetMem );
	}
	
	Float3x3 & InverseRotationMatrix( const Float3x3 &rotation, Float3x3 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::InverseRotationMatrix( rotation );
	}
	
	Float4x4 & InverseRotationMatrix( const Float4x4 &rotation, Float4x4 &targetMem )
	{
//		return targetMem = ::LinearAlgebra3D::InverseRotationMatrix( rotation );
		return targetMem = rotation.GetTranspose();
	}

	Float4x4 & OrientationMatrix( const Float3x3 &rotation, const Float3 &translation, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::OrientationMatrix( rotation, translation );
	}

	Float4x4 & OrientationMatrix( const Float4x4 &rotation, const Float3 &translation, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::OrientationMatrix( rotation, translation );
	}

	Float4x4 & OrientationMatrix( const Float3 &normalizedAxis, const Float & deltaRadian, const Float3 &sumTranslation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::OrientationMatrix( normalizedAxis, deltaRadian, sumTranslation, targetMem );
	}

	Float4x4 & OrientationMatrix( const Float3 &angularAxis, const Float3 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::OrientationMatrix( angularAxis, translation, targetMem );
	}

	Float4x4 & ViewMatrix( const Float3 &angularAxis, const Float3 &translation, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::ViewMatrix( angularAxis, translation, targetMem );
	}

	Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, const Float3 &centerOfMass, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::OrientationMatrix( sumDeltaAngularAxis, sumTranslation, centerOfMass, targetMem );
	}

	Float4x4 & OrientationMatrix_LookAtDirection( const Float3 &normalizedDirection, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::OrientationMatrix_LookAtDirection( normalizedDirection, normalizedUpVector, worldPos );
	}

	Float4x4 & OrientationMatrix_LookAtPos( const Float3 &worldLookAt, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::OrientationMatrix_LookAtPos( worldLookAt, normalizedUpVector, worldPos );
	}

	Float4x4 & ViewMatrix_LookAtDirection( const Float3 &normalizedDirection, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::InverseOrientationMatrix( ::LinearAlgebra3D::OrientationMatrix_LookAtDirection( normalizedDirection, normalizedUpVector, worldPos ), targetMem );																 
	}

	Float4x4 & ViewMatrix_LookAtPos( const Float3 &worldLookAt, const Float3 &normalizedUpVector, const Float3 &worldPos, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::InverseOrientationMatrix( ::LinearAlgebra3D::OrientationMatrix_LookAtPos( worldLookAt, normalizedUpVector, worldPos ), targetMem );	
	}

	Float4x4 & InverseOrientationMatrix( const Float4x4 &orientationMatrix, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::InverseOrientationMatrix( orientationMatrix, targetMem );
	}

	Float4x4 & UpdateOrientationMatrix( const Float3 &deltaPosition, const Float4x4 &deltaRotationMatrix, Float4x4 &orientationMatrix )
	{
		return ::LinearAlgebra3D::UpdateOrientationMatrix( deltaPosition, deltaRotationMatrix, orientationMatrix );
	}

	Float4x4 & ExtractRotationMatrix( const Float4x4 &orientation, Float4x4 &targetMem )
	{
		return targetMem = ::LinearAlgebra3D::ExtractRotationMatrix( orientation );
	}

	Float4x4 & ProjectionMatrix_Orthographic( const Float &width, const Float &height, const Float &nearClip, const Float &farClip, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::ProjectionMatrix_Orthographic( width, height, nearClip, farClip, targetMem );
	}

	Float4x4 & ProjectionMatrix_Perspective( const Float &verticalFoV, const Float &aspectRatio, const Float &nearClip, const Float &farClip, Float4x4 &targetMem )
	{
		return ::LinearAlgebra3D::ProjectionMatrix_Perspective( verticalFoV, aspectRatio, nearClip, farClip, targetMem );
	}

	Float3 VectorProjection( const Float3 &vector, const Float3 &axis )
	{
		return ::LinearAlgebra3D::VectorProjection( vector, axis );
	}

	Float4 VectorProjection( const Float4 &vector, const Float4 &axis )
	{
		return ::LinearAlgebra3D::VectorProjection( vector, axis );
	}

	Float3 NormalProjection( const Float3 &vector, const Float3 &normalizedAxis )
	{
		return ::LinearAlgebra3D::NormalProjection( vector, normalizedAxis );
	}

	Float4 NormalProjection( const Float4 &vector, const Float4 &normalizedAxis )
	{
		return ::LinearAlgebra3D::NormalProjection( vector, normalizedAxis );
	}
} }