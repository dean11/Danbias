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

	Float3x3 & OrientationMatrix( const Float2 &position, const Float &radian, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::OrientationMatrix( radian, position, targetMem ); }

	Float3x3 & OrientationMatrix( const Float2 &position, const Float2 &lookAt, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::OrientationMatrix( lookAt, position, targetMem ); }

	Float3x3 & OrientationMatrix( const Float2 &position, Float radian, const Float2 &localCenterOfRotation, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::OrientationMatrix( radian, position, localCenterOfRotation, targetMem ); }

	Float3x3 & InverseOrientationMatrix( const Float3x3 &orientationMatrix, Float3x3 &targetMem )
	{ return ::LinearAlgebra2D::InverseOrientationMatrix( orientationMatrix, targetMem ); }
} }

namespace Oyster { namespace Math3D
{
	Float4x4 & TranslationMatrix( const Float3 &position, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::TranslationMatrix( position, targetMem ); }

	Float4x4 & RotationMatrix_AxisX( const Float &radian, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisX( radian, targetMem ); }

	Float4x4 & RotationMatrix_AxisY( const Float &radian, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisY( radian, targetMem ); }

	Float4x4 & RotationMatrix_AxisZ( const Float &radian, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::RotationMatrix_AxisZ( radian, targetMem ); }

	Float4x4 & RotationMatrix( const Float &radian, const Float3 &normalizedAxis, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::RotationMatrix( normalizedAxis, radian, targetMem ); }

	Float4x4 & OrientationMatrix( const Float3 &normalizedAxis, const Float & deltaRadian, const Float3 &sumTranslation, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::OrientationMatrix( normalizedAxis, deltaRadian, sumTranslation, targetMem ); }

	Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::OrientationMatrix( sumDeltaAngularAxis, sumTranslation, targetMem ); }

	Float4x4 & OrientationMatrix( const Float3 &sumDeltaAngularAxis, const Float3 &sumTranslation, const Float3 &centerOfMass, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::OrientationMatrix( sumDeltaAngularAxis, sumTranslation, centerOfMass, targetMem ); }

	Float4x4 & InverseOrientationMatrix( const Float4x4 &orientationMatrix, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::InverseOrientationMatrix( orientationMatrix, targetMem ); }

	Float4x4 & ProjectionMatrix_Orthographic( const Float &width, const Float &height, const Float &nearClip, const Float &farClip, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::ProjectionMatrix_Orthographic( width, height, nearClip, farClip, targetMem ); }

	Float4x4 & ProjectionMatrix_Perspective( const Float &verticalFoV, const Float &aspectRatio, const Float &nearClip, const Float &farClip, Float4x4 &targetMem )
	{ return ::LinearAlgebra3D::ProjectionMatrix_Perspective( verticalFoV, aspectRatio, nearClip, farClip, targetMem ); }

	Float3 VectorProjection( const Float3 &vector, const Float3 &axis )
	{ return ::LinearAlgebra3D::VectorProjection( vector, axis ); }
} }