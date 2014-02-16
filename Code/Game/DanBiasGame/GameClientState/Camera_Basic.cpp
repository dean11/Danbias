#include "Camera_Basic.h"

using namespace ::Oyster::Math3D;

Camera_Basic::Camera_Basic()
{
	this->translation = this->angularAxis = Float3::null;
	this->projection = Float4x4::identity;
	this->rotation = Quaternion::identity;
	rotationIsOutOfDate = false;
}

Camera_Basic::Camera_Basic( const Float3 &position, const Float3 &angularAxis, const Float4x4 &projection )
{
	this->translation = position;
	this->angularAxis = angularAxis;
	this->projection = projection;
	this->rotation = Quaternion::identity;
	rotationIsOutOfDate = true;
}

Camera_Basic::~Camera_Basic() {}

Camera_Basic & Camera_Basic::operator = ( const Camera_Basic &camera )
{
	this->translation = camera.translation;
	this->angularAxis = camera.angularAxis;
	this->projection = camera.projection;
	this->rotation = camera.rotation;
	rotationIsOutOfDate = camera.rotationIsOutOfDate;
	return *this;
}

void Camera_Basic::SetPosition( const Float3 &translation )
{
	this->translation = translation;
}

void Camera_Basic::SetAngular( const Float3 &axis )
{
	this->angularAxis = axis;
	this->rotationIsOutOfDate = true;
}

void Camera_Basic::SetProjection( const Float4x4 &matrix )
{
	this->projection = matrix;
}

void Camera_Basic::SetOrthographicProjection( Float width, Float height, Float nearClip, Float farClip )
{
	ProjectionMatrix_Orthographic( width, height, nearClip, farClip, this->projection );
}

void Camera_Basic::SetPerspectiveProjection( Float verticalFoV, Float aspectRatio, Float nearClip, Float farClip )
{
	ProjectionMatrix_Perspective( verticalFoV, aspectRatio, nearClip, farClip, this->projection );
}

void Camera_Basic::Move( const Float3 &deltaPosition )
{
	this->translation += deltaPosition;
}

void Camera_Basic::Rotate( const Float3 &deltaAngularAxis )
{
	this->angularAxis += deltaAngularAxis;
	this->rotationIsOutOfDate = true;
}

const Float3 & Camera_Basic::GetPosition() const
{
	return this->translation;
}

const Float3 & Camera_Basic::GetAngularAxis() const
{
	return this->angularAxis;
}

Float3 Camera_Basic::GetNormalOf( const Float3 &axis ) const
{
	return WorldAxisOf( this->GetRotation(), axis );
}

const Quaternion & Camera_Basic::GetRotation() const
{
	if( this->rotationIsOutOfDate )
	{
		// Maintain rotation resolution by keeping axis within [0, 2pi] (trigonometric methods gets faster too)
		Float4 integer;
		::std::modf( this->angularAxis * (0.5f / pi), integer.xyz );
		this->angularAxis -= ((2.0f * pi) * integer).xyz;

		this->rotation = Rotation( this->angularAxis );
		this->rotationIsOutOfDate = false;
	}

	return this->rotation;
}

Float3x3 & Camera_Basic::GetRotationMatrix( Float3x3 &targetMem ) const
{
	return RotationMatrix( this->rotation, targetMem );
}

Float4x4 & Camera_Basic::GetRotationMatrix( Float4x4 &targetMem ) const
{
	return RotationMatrix( this->rotation, targetMem );
}

Float4x4 & Camera_Basic::GetViewMatrix( Float4x4 &targetMem ) const
{
	return ViewMatrix( this->GetRotation(), this->translation, targetMem );
}

const Float4x4 & Camera_Basic::GetProjectionMatrix() const
{
	return this->projection;
}

Float4x4 & Camera_Basic::GetViewsProjMatrix( Float4x4 &targetMem ) const
{
	return TransformMatrix( this->projection, this->GetViewMatrix(), targetMem );
}