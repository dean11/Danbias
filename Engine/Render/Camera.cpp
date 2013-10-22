#include "Camera.h"
//Hack: temp include, calc proj matrix properly later
#include "..\Core\CoreIncludes.h"

using namespace Oyster;
using namespace Render;
using namespace Oyster::Math;
Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

Math::Float3 Camera::GetPosition() {return pos;}
void Camera::SetPosition(Math::Float3 p){ pos=p;}

//axis
Math::Float3 Camera::GetRight() {return right;}
Math::Float3 Camera::GetUp() {return up;}
Math::Float3 Camera::GetLook() {return look;}

//frustrum
float Camera::GetNearZ(){return nearZ;}
float Camera::GetFarZ(){return farZ;}
float Camera::GetAspect(){return aspect;}
float Camera::GetFovY(){return fovY;}
float Camera::GetFovX(){return fovX;}

//set frustrum
void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	this->fovY=fovY;
	this->aspect=aspect;
	nearZ=zn;
	farZ=zf;
	D3DXMATRIX P;
	D3DXMatrixPerspectiveFovLH(&P,fovY,aspect,zn,zf);
	mProj = Math::Float4x4(P);
}

//normal LookAt
void Camera::LookAt(Math::Float3 pos, Math::Float3 target, Math::Float3 worldUp)
{
	look=target;
	up=worldUp;
	this->pos=pos;
	right = up.cross(look);
}

//Get Matrices
Matrix Camera::View(){return mView;}
Matrix Camera::Proj(){return mProj;}
Matrix Camera::ViewProj()
{
	return (mView * mProj).transpose(); // edited by Dan 04-19
}

//Move Camera
//FIX:  Multiply Add not working
//FIX:  Single Float Duplicate Creation
void Camera::Strafe(float d)
{
	Math::Float3 s= Math::Float3(d,d,d);
	pos= (Math::operator*(s,right)) + pos;
}
void Camera::Walk(float d)
{
	Math::Float3 s= Math::Float3(d,d,d);
	pos= (Math::operator*(s,look)) + pos;
}
void Camera::Fly(float d)
{
	Math::Float3 s= Math::Float3(d,d,d);
	pos= (Math::operator*(s,up)) + pos;
}

//Rotate Camera
//FIX: Float3 * float4x4
void Camera::Pitch(float angle)
{
	Matrix m;
	Math::rotationMatrix(m,angle,right);
	m = m.getAdjoint().getInverse();

	up = (Math::Float4(up,0) * m).xyz;
	look =  (Math::Float4(look,0) * m).xyz;
}
void Camera::Yaw(float angle)
{

	Matrix m;
	Math::rotationMatrix(m,angle,up);
	m = m.getAdjoint().getInverse();

	right = (Math::Float4(right,0) * m).xyz;
	look =  (Math::Float4(look,0) * m).xyz;
}
void Camera::Roll(float angle)
{
	Matrix m;
	Math::rotationMatrix(m, angle,look);
	m = m.getAdjoint().getInverse();

	up = (Math::Float4(up,0) * m).xyz;
	right =  (Math::Float4(right,0) * m).xyz;
}
void Camera::RotateY(float angle)
{
	Matrix m;

	Math::rotationMatrix_AxisY(m, angle);

	m = m.getAdjoint().getInverse();

	up = (Math::Float4(up,0) * m).xyz;
	look =  (Math::Float4(look,0) * m).xyz;
	right =  (Math::Float4(right,0) * m).xyz;
}

void Camera::UpdateViewMatrix()
{
	look.normalize();

	up = look.cross(right);
	up.normalize();

	right = up.cross(look);

	float x = -pos.dot(right);
	float y = -pos.dot(up);
	float z = -pos.dot(look);

	mView.m[0][0] = right.x;
	mView.m[1][0] = right.y;
	mView.m[2][0] = right.z;
	mView.m[3][0] = x;

	mView.m[0][1] = up.x;
	mView.m[1][1] = up.y;
	mView.m[2][1] = up.z;
	mView.m[3][1] = y;

	mView.m[0][2] = look.x;
	mView.m[1][2] = look.y;
	mView.m[2][2] = look.z;
	mView.m[3][2] = z;

	mView.m[0][3] = 0.0f;
	mView.m[1][3] = 0.0f;
	mView.m[2][3] = 0.0f;
	mView.m[3][3] = 1.0f;
}


