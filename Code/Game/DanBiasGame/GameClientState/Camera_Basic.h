#ifndef CAMERA_BASIC_H
#define CAMERA_BASIC_H

#include "OysterMath.h"

class Camera_Basic
{
public:
	Camera_Basic();
	Camera_Basic( const ::Oyster::Math::Float3 &position, const ::Oyster::Math::Float3 &angularAxis, const ::Oyster::Math::Float4x4 &projection );
	virtual ~Camera_Basic();

	Camera_Basic & operator = ( const Camera_Basic &camera );

	void SetPosition( const ::Oyster::Math::Float3 &translation );
	void SetAngular( const ::Oyster::Math::Float3 &axis );
	void SetProjection( const ::Oyster::Math::Float4x4 &matrix );
	void SetOrthographicProjection( ::Oyster::Math::Float width, ::Oyster::Math::Float height, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );
	void SetPerspectiveProjection( ::Oyster::Math::Float verticalFoV, ::Oyster::Math::Float aspectRatio, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );

	void Move( const ::Oyster::Math::Float3 &deltaPosition );
	void Rotate( const ::Oyster::Math::Float3 &deltaAngularAxis );

	const ::Oyster::Math::Float3	 & GetPosition() const;
	const ::Oyster::Math::Float3	 & GetAngularAxis() const;
		  ::Oyster::Math::Float3	   GetNormalOf( const ::Oyster::Math::Float3 &axis ) const;
	const ::Oyster::Math::Quaternion & GetRotation() const;
		  ::Oyster::Math::Float3x3   & GetRotationMatrix( ::Oyster::Math::Float3x3 &targetMem ) const;
		  ::Oyster::Math::Float4x4   & GetRotationMatrix( ::Oyster::Math::Float4x4 &targetMem ) const;
		  ::Oyster::Math::Float4x4   & GetViewMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
	const ::Oyster::Math::Float4x4   & GetProjectionMatrix() const;
		  ::Oyster::Math::Float4x4   & GetViewsProjMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

	private:
			::Oyster::Math::Float3 translation;
	mutable ::Oyster::Math::Float3 angularAxis;
			::Oyster::Math::Float4x4 projection;
	mutable ::Oyster::Math::Quaternion rotation;
	mutable bool rotationIsOutOfDate;
};

#endif