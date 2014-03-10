#include "Camera_BasicV2.h"

using namespace ::Oyster::Math3D;

inline Quaternion Transform( const Quaternion &transformer, const Quaternion &transformee )
{
	return transformer * transformee ;
}

inline Float3 Transform( const Quaternion &transformer, const Float3 &transformee, const Quaternion &transformerConjugate )
{
	return (transformer * transformee * transformerConjugate).imaginary;
}

inline Float3 Transform( const Quaternion &transformer, const Float3 &transformee )
{
	return Transform( transformer, transformee, transformer.GetConjugate() );
}

Camera_BasicV2::Camera_BasicV2()
{
	this->translation = Float3::null;
	this->rotation = Quaternion::identity;
	this->projection = Float4x4::identity;
}

Camera_BasicV2::Camera_BasicV2( const Float3 &position, const Quaternion &rotation, const Float4x4 &projection )
{
	this->translation = position;
	this->rotation = rotation;
	this->projection = projection;
}

Camera_BasicV2::~Camera_BasicV2() {}

Camera_BasicV2 & Camera_BasicV2::operator = ( const Camera_BasicV2 &camera )
{
	this->translation = camera.translation;
	this->rotation = camera.rotation;
	this->projection = camera.projection;
	return *this;
}

void Camera_BasicV2::SetPosition( const Float3 &translation )
{
	this->translation = translation;
}

void Camera_BasicV2::SetRotation( const Quaternion &rotation )
{
	this->rotation = rotation;
}

void Camera_BasicV2::SetAngular( const Float3 &axis )
{
	this->rotation = Rotation( axis );
}

void Camera_BasicV2::SetProjection( const Float4x4 &matrix )
{
	this->projection = matrix;
}

void Camera_BasicV2::SetOrthographicProjection( Float width, Float height, Float nearClip, Float farClip )
{
	ProjectionMatrix_Orthographic( width, height, nearClip, farClip, this->projection );
}

void Camera_BasicV2::SetPerspectiveProjection( Float verticalFoV, Float aspectRatio, Float nearClip, Float farClip )
{
	ProjectionMatrix_Perspective( verticalFoV, aspectRatio, nearClip, farClip, this->projection );
}

void Camera_BasicV2::Move( const Float3 &deltaPosition )
{
	this->translation += deltaPosition;
}

void Camera_BasicV2::Rotate( const Quaternion &deltaRotation )
{
	this->rotation *= deltaRotation;
}

void Camera_BasicV2::Rotate( const Float3 &deltaAngularAxis )
{
	this->rotation *= Rotation( deltaAngularAxis );
}

const Float3 & Camera_BasicV2::GetPosition() const
{
	return this->translation;
}

Float3 & Camera_BasicV2::GetAngularAxis( Float3 &targetMem ) const
{
	return targetMem = AngularAxis( this->rotation );
}

Float3 Camera_BasicV2::GetNormalOf( const Float3 &axis ) const
{
	return Transform( this->rotation, axis );
}

const Quaternion & Camera_BasicV2::GetRotation() const
{
	return this->rotation;
}

Float3x3 & Camera_BasicV2::GetRotationMatrix( Float3x3 &targetMem ) const
{
	return RotationMatrix( this->rotation, targetMem );
}

Float4x4 & Camera_BasicV2::GetRotationMatrix( Float4x4 &targetMem ) const
{
	return RotationMatrix( this->rotation, targetMem );
}

Float4x4 & Camera_BasicV2::GetViewMatrix( Float4x4 &targetMem ) const
{
	return ViewMatrix( this->rotation, this->translation, targetMem );
}

const Float4x4 & Camera_BasicV2::GetProjectionMatrix() const
{
	return this->projection;
}

Float4x4 & Camera_BasicV2::GetViewsProjMatrix( Float4x4 &targetMem ) const
{
	return TransformMatrix( this->projection, this->GetViewMatrix(), targetMem );
}