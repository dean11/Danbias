#include "Camera.h"

Camera::Camera()
{
	this->m_position = Oyster::Math::Float3(0, 50, 0);
	this->mRight = Oyster::Math::Float3(1, 0, 0);
	this->mUp = Oyster::Math::Float3(0, 1, 0);
	this->mLook = Oyster::Math::Float3(0, 0, 1);
}

Camera::~Camera()
{
}

void Camera::SetPosition(const Oyster::Math::Float3& v)
{
	this->m_position = v;
}

Oyster::Math::Float3 Camera::GetPosition()const
{
	return this->m_position;
}

Oyster::Math::Float3 Camera::GetRight()const
{
	return this->mRight;
}

Oyster::Math::Float3 Camera::GetUp()const
{
	return this->mUp;
}

Oyster::Math::Float3 Camera::GetLook()const
{
	return this->mLook;
}

float Camera::GetNearZ()const
{
	return this->mNearZ;
}

float Camera::GetFarZ()const
{
	return this->mFarZ;
}

float Camera::GetAspect()const
{
	return this->mAspect;
}

Oyster::Math::Float3 Camera::CrossMatrix(const Oyster::Math::Float3& vector, const Oyster::Math::Float4x4& matrix)
{
	Oyster::Math::Float3 vec;
	vec.x = matrix.m11*vector.x + matrix.m12*vector.y + matrix.m13*vector.z;
	vec.y = matrix.m21*vector.x + matrix.m22*vector.y + matrix.m23*vector.z;
	vec.z = matrix.m31*vector.x + matrix.m32*vector.y + matrix.m33*vector.z;
	return vec;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	this->mFovY = fovY;
	this->mAspect = aspect;
	this->mNearZ = zn;
	this->mFarZ = zf;

	float yScale = tan((Oyster::Math::pi*0.5f) - (mFovY*0.5f));
	float xScale = yScale/this->mAspect;

	mProj = Oyster::Math::Float4x4(xScale, 0, 0, 0, 
					   0, yScale, 0, 0,
					   0, 0, zf/(zf-zn), 1,
					   0, 0, -zn*zf/(zf-zn), 0);
	mProj.Transpose();
}

void Camera::LookAt(Oyster::Math::Float3 pos, Oyster::Math::Float3 target, Oyster::Math::Float3 worldUp)
{
	Oyster::Math::Float3 L;

	L = target - pos;
	L.Normalize();
	
	Oyster::Math::Float3 R;
	R = worldUp.Cross(L);
	R.Normalize();

	Oyster::Math::Float3 U;
	U = L.Cross(R);

	this->m_position = pos;
	this->mLook = L;
	this->mRight = R;
	this->mUp = U;
}

Oyster::Math::Float4x4 Camera::View()const
{
	return this->mView;
}

Oyster::Math::Float4x4 Camera::Proj()const
{
	return this->mProj;
}

Oyster::Math::Float4x4 Camera::ViewsProj()const
{
	Oyster::Math::Float4x4 M;
	M = mView * mProj;
	return M;
}

void Camera::Walk(float dist)
{
	this->m_position += dist*this->mLook;
}

void Camera::Strafe(float dist)
{
	this->m_position += dist*this->mRight;
}

void Camera::Pitch(float angle)
{
	float radians = angle * 0.0174532925f;
	
	Oyster::Math::Float4x4 R;
							  
	//D3DXMatrixRotationAxis(&R, &-mRight, radians);
	Oyster::Math3D::RotationMatrix(radians,-mRight,R);
	this->mUp = CrossMatrix(this->mUp, R);
	this->mLook = CrossMatrix(this->mLook, R);
}

void Camera::Yaw(float angle)
{
	float radians = angle * 0.0174532925f;

	Oyster::Math::Float4x4 R;
							  
	Oyster::Math::Float3 up(0,1,0);
	//D3DXMatrixRotationAxis(&R, &-up, radians);
	Oyster::Math3D::RotationMatrix(radians,-up,R);

	this->mRight = CrossMatrix(this->mRight, R);
	this->mUp = CrossMatrix(mUp, R);
	this->mLook = CrossMatrix(this->mLook, R);
}

void Camera::UpdateViewMatrix()
{
	mLook.Normalize();
	mUp = mLook.Cross(mRight);
	mUp.Normalize();
	mRight = mUp.Cross(mLook);
	
	float x = -m_position.Dot(mRight);
	float y = -m_position.Dot(mUp);
	float z = -m_position.Dot(mLook);

	//this->mView(0, 0) = this->mRight.x;
	mView.m11 = mRight.x;
	//this->mView(1, 0) = this->mRight.y;
	mView.m21 = mRight.y;
	//this->mView(2, 0) = this->mRight.z;
	mView.m31 = mRight.z;
	//this->mView(3, 0) = x;
	mView.m41 = x;

	//this->mView(0, 1) = this->mUp.x;
	mView.m12 = mUp.x;
	//this->mView(1, 1) = this->mUp.y;
	mView.m22 = mUp.y;
	//this->mView(2, 1) = this->mUp.z;
	mView.m32 = mUp.z;
	//this->mView(3, 1) = y;
	mView.m42 = y;

	//this->mView(0, 2) = this->mLook.x;
	mView.m13 = mLook.x;
	//this->mView(1, 2) = this->mLook.y;
	mView.m23 = mLook.y;
	//this->mView(2, 2) = this->mLook.z;
	mView.m33 = mLook.z;
	//this->mView(3, 2) = z;
	mView.m43 = z;

	//this->mView(0, 3) = 0.0f;
	mView.m14 = 0.0f;
	//this->mView(1, 3) = 0.0f;
	mView.m24 = 0.0f;
	
	//this->mView(2, 3) = 0.0f;
	mView.m34 = 0.0f;
	//this->mView(3, 3) = 1.0f;
	mView.m44 = 1.0f;

	mView.Transpose();
}