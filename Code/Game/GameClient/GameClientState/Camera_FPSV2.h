#ifndef CAMERA_FPSV2_H
#define CAMERA_FPSV2_H

#include "OysterMath.h"
#include "Camera_BasicV2.h"

class Camera_FPSV2
{
public:
	Camera_FPSV2();
	virtual ~Camera_FPSV2();

	Camera_FPSV2 & operator = ( const Camera_FPSV2 &camera );

	void SetHeadOffset( const ::Oyster::Math::Float3 &translation );
	void SetPosition( const ::Oyster::Math::Float3 &translation );
	void SetRotation( const ::Oyster::Math::Quaternion &rotation );
	void SetAngular( const ::Oyster::Math::Float3 &axis );
	void SetProjection( const ::Oyster::Math::Float4x4 &matrix );
	void SetOrthographicProjection( ::Oyster::Math::Float width, ::Oyster::Math::Float height, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );
	void SetPerspectiveProjection( ::Oyster::Math::Float verticalFoV, ::Oyster::Math::Float aspectRatio, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );

	void UpdateOrientation();

	void SnapUpToNormal( const ::Oyster::Math::Float3 &normal );

	void Move( const ::Oyster::Math::Float3 &deltaPosition );
	void Rotate( const ::Oyster::Math::Quaternion &deltaRotation );
	void Rotate( const ::Oyster::Math::Float3 &deltaAngularAxis );

	void MoveForward( ::Oyster::Math::Float distance );
	void MoveBackward( ::Oyster::Math::Float distance );
	void StrafeRight( ::Oyster::Math::Float distance );
	void StrafeLeft( ::Oyster::Math::Float distance );

	void PitchUp( ::Oyster::Math::Float radian );
	void PitchDown( ::Oyster::Math::Float radian );
	void YawRight( ::Oyster::Math::Float radian );
	void YawLeft( ::Oyster::Math::Float radian );

	const ::Oyster::Math::Float3	 & GetHeadOffset() const;
	const ::Oyster::Math::Float3	 & GetPosition() const;
		  ::Oyster::Math::Float4x4   & GetViewMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
	const ::Oyster::Math::Float4x4   & GetProjectionMatrix() const;
		  ::Oyster::Math::Float4x4   & GetViewsProjMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
		  ::Oyster::Math::Float3	   GetNormalOf( const ::Oyster::Math::Float3 &axis ) const;
		  ::Oyster::Math::Float3	   GetRight() const;
		  ::Oyster::Math::Float3	   GetUp() const;
		  ::Oyster::Math::Float3	   GetLook() const;
		  ::Oyster::Math::Float3	   GetForward() const;

private:
	Camera_BasicV2 head;
	::Oyster::Math::Float pitchUp;
	::Oyster::Math::Float3 headOffset;
	struct
	{
		::Oyster::Math::Float3 translation;
		::Oyster::Math::Quaternion rotation;
	} body;
};

#endif