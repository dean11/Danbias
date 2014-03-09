#ifndef CAMERA_BASIC_V2_H
#define CAMERA_BASIC_V2_H

#include "OysterMath.h"

class Camera_BasicV2
{
public:
	Camera_BasicV2();
	Camera_BasicV2( const ::Oyster::Math::Float3 &position, const ::Oyster::Math::Quaternion &rotation, const ::Oyster::Math::Float4x4 &projection );
	virtual ~Camera_BasicV2();

	Camera_BasicV2 & operator = ( const Camera_BasicV2 &camera );

	void SetPosition( const ::Oyster::Math::Float3 &translation );
	void SetRotation( const ::Oyster::Math::Quaternion &rotation );
	void SetAngular( const ::Oyster::Math::Float3 &axis );
	void SetProjection( const ::Oyster::Math::Float4x4 &matrix );
	void SetOrthographicProjection( ::Oyster::Math::Float width, ::Oyster::Math::Float height, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );
	void SetPerspectiveProjection( ::Oyster::Math::Float verticalFoV, ::Oyster::Math::Float aspectRatio, ::Oyster::Math::Float nearClip, ::Oyster::Math::Float farClip );

	void Move( const ::Oyster::Math::Float3 &deltaPosition );
	void Rotate( const ::Oyster::Math::Quaternion &deltaRotation );
	void Rotate( const ::Oyster::Math::Float3 &deltaAngularAxis );

	const ::Oyster::Math::Float3	 & GetPosition() const;
		  ::Oyster::Math::Float3	 & GetAngularAxis( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
		  ::Oyster::Math::Float3	   GetNormalOf( const ::Oyster::Math::Float3 &axis ) const;
	const ::Oyster::Math::Quaternion & GetRotation() const;
		  ::Oyster::Math::Float3x3   & GetRotationMatrix( ::Oyster::Math::Float3x3 &targetMem ) const;
		  ::Oyster::Math::Float4x4   & GetRotationMatrix( ::Oyster::Math::Float4x4 &targetMem ) const;
		  ::Oyster::Math::Float4x4   & GetViewMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
	const ::Oyster::Math::Float4x4   & GetProjectionMatrix() const;
		  ::Oyster::Math::Float4x4   & GetViewsProjMatrix( Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

	private:
			::Oyster::Math::Float3 translation;
			::Oyster::Math::Quaternion rotation;
			::Oyster::Math::Float4x4 projection;
};

#endif