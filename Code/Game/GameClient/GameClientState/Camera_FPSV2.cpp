#include "Camera_FPSV2.h"
#include "Utilities.h"

using namespace ::Oyster::Math3D;
using namespace ::Utility::Value;

Camera_FPSV2::Camera_FPSV2()
{ // this->head is default set to identity uniformprojection at origo
	this->pitchUp = 0.0f;
	this->headOffset =
	this->body.translation = Float3::null;
	this->body.rotation = Quaternion::identity;
}

Camera_FPSV2::~Camera_FPSV2() {}

Camera_FPSV2 & Camera_FPSV2::operator = ( const Camera_FPSV2 &camera )
{
	this->head = camera.head;
	this->pitchUp = camera.pitchUp;
	this->headOffset = camera.headOffset;
	this->body.translation = camera.body.translation;
	this->body.rotation = camera.body.rotation;
	return *this;
}

void Camera_FPSV2::SetHeadOffset( const Float3 &translation )
{
	this->head.Move( translation - this->headOffset );
	this->headOffset = translation;
}

void Camera_FPSV2::SetPosition( const Float3 &translation )
{
	this->head.Move( translation - this->body.translation );
	this->body.translation = translation;
}

void Camera_FPSV2::SetRotation( const Quaternion &rotation )
{
	if( !Within(rotation.GetNorm(), .99f, 1.01f) )
	{ // HACK: bug trap
		const char *breakPoint = "Caught an invalid rotation!";
	}

	this->body.rotation = rotation;
	this->head.SetRotation( rotation * Rotation(this->pitchUp, WorldAxisOf(rotation, Float3::standard_unit_x) ) );
}

void Camera_FPSV2::SetAngular( const Float3 &axis )
{
	this->SetRotation( Rotation(axis) );
}

void Camera_FPSV2::SetProjection( const Float4x4 &matrix )
{
	this->head.SetProjection( matrix );
}

void Camera_FPSV2::SetOrthographicProjection( Float width, Float height, Float nearClip, Float farClip )
{
	this->head.SetOrthographicProjection( width, height, nearClip, farClip );
}

void Camera_FPSV2::SetPerspectiveProjection( Float verticalFoV, Float aspectRatio, Float nearClip, Float farClip )
{
	this->head.SetPerspectiveProjection( verticalFoV, aspectRatio, nearClip, farClip );
}

void Camera_FPSV2::UpdateOrientation()
{
	Float4x4 orientation;
	OrientationMatrix( this->body.rotation, this->body.translation, orientation );

	this->head.SetPosition( (orientation * Float4(this->headOffset, 1.0f)).xyz );
}

void Camera_FPSV2::SnapUpToNormal( const Float3 &normal )
{
	this->body.rotation = Rotation( SnapAngularAxis(AngularAxis(this->body.rotation), WorldAxisOf(this->body.rotation, Float3::standard_unit_y), normal) );
	this->head.SetRotation( this->body.rotation * Rotation(this->pitchUp * Float3::standard_unit_x) );
}

void Camera_FPSV2::Move( const Float3 &deltaPosition )
{
	this->head.Move( deltaPosition );
	this->body.translation += deltaPosition;
}

void Camera_FPSV2::Rotate( const Quaternion &deltaRotation )
{
	this->head.Rotate( deltaRotation );
	this->body.rotation *= deltaRotation;
}

void Camera_FPSV2::Rotate( const Float3 &deltaAngularAxis )
{
	this->Rotate( Rotation(deltaAngularAxis) );
}

void Camera_FPSV2::MoveForward( Float distance )
{
	this->MoveBackward( -distance );
}

void Camera_FPSV2::MoveBackward( Float distance )
{
	this->Move( distance * WorldAxisOf(this->body.rotation, Float3::standard_unit_z) );
}

void Camera_FPSV2::StrafeRight( Float distance )
{
	this->Move( distance * WorldAxisOf(this->body.rotation, Float3::standard_unit_x) );
}

void Camera_FPSV2::StrafeLeft( Float distance )
{
	this->StrafeRight( -distance );
}

void Camera_FPSV2::PitchUp( Float radian )
{
	this->pitchUp = Clamp( this->pitchUp + radian, -0.48f * pi, 0.48f * pi );
//	this->head.SetRotation( this->body.rotation * Rotation(this->pitchUp, Float3::standard_unit_x) );
}

void Camera_FPSV2::PitchDown( Float radian )
{
	this->PitchUp( -radian );
}

void Camera_FPSV2::YawRight( Float radian )
{
	this->YawLeft( -radian );
}

void Camera_FPSV2::YawLeft( Float radian )
{
	Quaternion deltaRotation = Rotation( radian, WorldAxisOf(this->body.rotation, Float3::standard_unit_y) );
	this->Rotate( deltaRotation );
}

const Float3 & Camera_FPSV2::GetHeadOffset() const
{
	return this->headOffset;
}

const Float3 & Camera_FPSV2::GetPosition() const
{
	return this->body.translation;
}

Float4x4 & Camera_FPSV2::GetViewMatrix( Float4x4 &targetMem ) const
{
	return this->head.GetViewMatrix( targetMem );
}

const Float4x4 & Camera_FPSV2::GetProjectionMatrix() const
{
	return this->head.GetProjectionMatrix();
}

Float4x4 & Camera_FPSV2::GetViewsProjMatrix( Float4x4 &targetMem ) const
{
	return this->head.GetViewsProjMatrix( targetMem );
}

Float3 Camera_FPSV2::GetNormalOf( const Float3 &axis ) const
{
	return this->head.GetNormalOf( axis );
}

Float3 Camera_FPSV2::GetRight() const
{
	return WorldAxisOf( this->body.rotation, Float3::standard_unit_x );
}

Float3 Camera_FPSV2::GetUp() const
{
	return WorldAxisOf( this->body.rotation, Float3::standard_unit_y );
}

Float3 Camera_FPSV2::GetLook() const
{
	return this->head.GetNormalOf( -Float3::standard_unit_z );
}

Float3 Camera_FPSV2::GetForward() const
{
	return WorldAxisOf( this->body.rotation, -Float3::standard_unit_z );
}