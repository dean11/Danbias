#include "Camera_FPS.h"
#include "Utilities.h"

using namespace ::Oyster::Math3D;
using namespace ::Utility::Value;

Camera_FPS::Camera_FPS()
{ // this->head is default set to identity uniformprojection at origo
	this->pitchUp = 0.0f;
	this->headOffset =
	this->body.translation =
	this->body.angularAxis = Float3( 0.0f );
	this->body.direction = Float3x3::identity;
}

Camera_FPS::~Camera_FPS() {}

Camera_FPS & Camera_FPS::operator = ( const Camera_FPS &camera )
{
	this->head = camera.head;
	this->pitchUp = camera.pitchUp;
	this->headOffset = camera.headOffset;
	this->body.translation = camera.body.translation;
	this->body.angularAxis = camera.body.angularAxis;
	this->body.direction = camera.body.direction;
	return *this;
}

void Camera_FPS::SetHeadOffset( const Float3 &translation )
{
	this->head.Move( translation - this->headOffset );
	this->headOffset = translation;
}

void Camera_FPS::SetPosition( const Float3 &translation )
{
	this->head.Move( translation - this->body.translation );
	this->body.translation = translation;
}

void Camera_FPS::SetAngular( const Float3 &axis )
{
	this->head.SetAngular( axis );
	this->pitchUp = 0.0f;
	this->body.angularAxis = axis;
}

void Camera_FPS::SetProjection( const Float4x4 &matrix )
{
	this->head.SetProjection( matrix );
}

void Camera_FPS::SetOrthographicProjection( Float width, Float height, Float nearClip, Float farClip )
{
	this->head.SetOrthographicProjection( width, height, nearClip, farClip );
}

void Camera_FPS::SetPerspectiveProjection( Float verticalFoV, Float aspectRatio, Float nearClip, Float farClip )
{
	this->head.SetPerspectiveProjection( verticalFoV, aspectRatio, nearClip, farClip );
}

void Camera_FPS::UpdateOrientation()
{
	RotationMatrix( Rotation(this->body.angularAxis), this->body.direction );

	Float4x4 orientation = Float4x4( Float4(this->body.direction.v[0], 0.0f),
									 Float4(this->body.direction.v[1], 0.0f),
									 Float4(this->body.direction.v[2], 0.0f),
									 Float4(this->body.translation, 1.0f) );

	this->head.SetPosition( (orientation * Float4(this->headOffset, 1.0f)).xyz );
}

void Camera_FPS::SnapUpToNormal( const Float3 &normal )
{
	SnapAngularAxis( this->body.angularAxis, this->body.direction.v[1], normal, this->body.angularAxis );
	this->head.SetAngular( this->body.angularAxis + this->pitchUp * Float3::standard_unit_x );
}

void Camera_FPS::Move( const Float3 &deltaPosition )
{
	this->head.Move( deltaPosition );
	this->body.translation += deltaPosition;
}

void Camera_FPS::Rotate( const Float3 &deltaAngularAxis )
{
	this->head.Rotate( deltaAngularAxis );
	this->body.angularAxis += deltaAngularAxis;
}

void Camera_FPS::MoveForward( Float distance )
{
	this->MoveBackward( -distance );
}

void Camera_FPS::MoveBackward( Float distance )
{
	this->Move( distance * this->body.direction.v[2] );
}

void Camera_FPS::StrafeRight( Float distance )
{
	this->Move( distance * this->body.direction.v[0] );
}

void Camera_FPS::StrafeLeft( Float distance )
{
	this->StrafeRight( -distance );
}

void Camera_FPS::PitchUp( Float radian )
{
	this->pitchUp = Clamp( this->pitchUp + radian, -0.48f * pi, 0.48f * pi );
	this->head.SetAngular( this->body.angularAxis + this->pitchUp * this->body.direction.v[0] );
}

void Camera_FPS::PitchDown( Float radian )
{
	this->PitchUp( -radian );
}

void Camera_FPS::YawRight( Float radian )
{
	this->YawLeft( -radian );
}

void Camera_FPS::YawLeft( Float radian )
{
	this->body.angularAxis += radian * this->body.direction.v[1];
	this->head.SetAngular( this->body.angularAxis + this->pitchUp * this->body.direction.v[0] );
}

const Float3 & Camera_FPS::GetHeadOffset() const
{
	return this->headOffset;
}

const Float3 & Camera_FPS::GetPosition() const
{
	return this->body.translation;
}

Float4x4 & Camera_FPS::GetViewMatrix( Float4x4 &targetMem ) const
{
	return this->head.GetViewMatrix( targetMem );
}

const Float4x4 & Camera_FPS::GetProjectionMatrix() const
{
	return this->head.GetProjectionMatrix();
}

Float4x4 & Camera_FPS::GetViewsProjMatrix( Float4x4 &targetMem ) const
{
	return this->head.GetViewsProjMatrix( targetMem );
}

Float3 Camera_FPS::GetNormalOf( const Float3 &axis ) const
{
	return this->head.GetNormalOf( axis );
}

Float3 Camera_FPS::GetRight() const
{
	return this->body.direction.v[0];
}

Float3 Camera_FPS::GetUp() const
{
	return this->body.direction.v[1];
}

Float3 Camera_FPS::GetLook() const
{
	return this->head.GetNormalOf( -Float3::standard_unit_z );
}

Float3 Camera_FPS::GetForward() const
{
	return -this->body.direction.v[2];
}