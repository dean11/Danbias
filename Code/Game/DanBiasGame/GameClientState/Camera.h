#ifndef CAMERA__H
#define CAMERA__H

#include "OysterMath.h"

class Camera
{
private:

	Oyster::Math::Float3 m_position;
	Oyster::Math::Float3 mRight;
	Oyster::Math::Float3 mUp;
	Oyster::Math::Float3 mLook;



	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;

	Oyster::Math::Float4x4 mView;
	Oyster::Math::Float4x4 mProj;

public:
	Camera();
	virtual ~Camera();

	void SetPosition(const Oyster::Math::Float3& v);

	Oyster::Math::Float3 GetPosition()const;

	Oyster::Math::Float3 GetRight()const;
	Oyster::Math::Float3 GetUp()const;
	Oyster::Math::Float3 GetLook()const;

	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;

	Oyster::Math::Float3 CrossMatrix(const Oyster::Math::Float3& v, const Oyster::Math::Float4x4& m);

	void SetLens(float fovY, float aspect, float zn, float zf);

	void LookAt(Oyster::Math::Float3 pos, Oyster::Math::Float3 target, Oyster::Math::Float3 worldUp);

	void setLook(Oyster::Math::Float3 look) { mLook = look; }
	void setUp(Oyster::Math::Float3 up) { mUp = up; }
	void setRight(Oyster::Math::Float3 right) { mRight = right; }

	Oyster::Math::Float4x4 View()const;
	Oyster::Math::Float4x4 Proj()const;
	Oyster::Math::Float4x4 ViewsProj()const;

	void Walk(float dist);
	void Strafe(float dist);

	void Pitch(float angle);
	void Yaw(float angle);

	void UpdateViewMatrix();
};
#endif