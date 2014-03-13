#include "FirstPersonWeapon.h"

using namespace DanBias::Client;
using namespace Oyster::Math;

Coil::Coil()
{
	maxGlow = Float3(1, 0, 0);
	minGlow = Float3(0, 1, 1);
	increaseGlow = false;
	percent = 0.01;
	totalPercent = 0.0f;
}

Coil::~Coil()
{
	this->Release();
}

bool Coil::Init()
{
	if(!this->model) return false;

	this->model->GlowTint = minGlow;
	this->model->IgnoreDepth = true;
	this->model->Tint = Float3(1.0f);
	return true;
}

void Coil::Rotate(float angle)
{
	angular.xyz = Float3(0, 0, 1);
	angular.w += angle;
}

void Coil::SetRotate(float angle)
{
	angular.xyz = Float3(0, 0, 1);
	angular.w = angle;
}

void Coil::Update()
{
	if(!this->model) return;

	//Create world matrix
	//Oyster::Math3D::Float4x4 rotTrans = Oyster::Math3D::TranslationMatrix(-offsetPosition);
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->scale);
	//Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(angular.w, angular.xyz);
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(offsetPosition);

	//rot = rotTrans * rot * rotTrans.GetInverse();
	this->model->WorldMatrix = this->world * translation * scale;

	//Update glow color
	Float3 colorChange = (maxGlow - minGlow);

	if(increaseGlow)
	{
		model->GlowTint += colorChange * 0.1;
		totalPercent += 0.1;
		if(totalPercent > 1.0f)
		{
			increaseGlow = false;
		}
	}

	if(totalPercent > 0.0f)
	{
		model->GlowTint -= colorChange * 0.005;

		totalPercent -= 0.005;
	}
}

void Coil::Shoot()
{
	this->percent = 0.1;
	this->increaseGlow = true;
}

void Coil::Render()
{
	C_Object::Render();
}

void Coil::SetInitialRotation(Oyster::Math::Quaternion rot)
{
	initialRotation = rot;
}

void Coil::SetOffsetPosition(Oyster::Math::Float3 offsetPos)
{
	this->offsetPosition = offsetPos;
}

void Coil::SetWorld(Matrix world)
{
	this->world = world;
}

FirstPersonWeapon::FirstPersonWeapon()
{
	coilID = 0;
	rotate = false;
	elapsed = 0.0f;
	maxRotation = 0.0f;
	maxRot = 0.0f;
	currentRotation = 0.0f;
	smallStepRotation = 0.0f;

	coilID = 0;
	base = new Coil;
	for(int i = 0; i < 6; i++)
	{
		coil[i] = new Coil;
	}
}

FirstPersonWeapon::~FirstPersonWeapon()
{
	for(int i = 0; i < 6; i++)
	{
		if(coil[i])
		{
			delete coil[i];
			coil[i] = NULL;
		}
	}

	if(base)
	{
		delete base;
		base = NULL;
	}

	this->Release();
}

bool FirstPersonWeapon::Init()
{
	ModelInitData modelData;
	modelData.modelPath = L"wpn_massdriver_high_cylinder.dan";
	modelData.visible = true;
	modelData.scale = Oyster::Math::Float3(1,1,1);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1);
	modelData.tint = Float3(1.0f);
	modelData.id = -1;

	//Initial rotation on the entire weapon
	initialRotation = Float4(0, 1, 0, pi);
	initialRotation.xyz.Normalize();

	C_Object::Init(modelData);
	this->model->IgnoreDepth = true;
	this->model->Tint = Float3(1.0f);

	modelData.modelPath = L"wpn_massdriver_high_base.dan";
	modelData.visible = true;
	modelData.scale = Oyster::Math::Float3(1,1,1);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1);
	modelData.id = -1;

	if(((C_Object*)base)->Init(modelData))
		base->Init();
	

	modelData.modelPath = L"wpn_massdriver_high_coil.dan";
	modelData.visible = true;
	modelData.scale = Oyster::Math::Float3(1,1,1);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1).GetNormalized();
	modelData.id = -2;

	Float3 off = 1.0f;

	((C_Object*)coil[0])->Init(modelData);
	coil[0]->SetOffsetPosition(Oyster::Math::Float3(0.0f, 0.460931031166f, 0.0f) * off);

	((C_Object*)coil[1])->Init(modelData);
	coil[1]->SetOffsetPosition(Oyster::Math::Float3(0.397489709703f, 0.230752453198, 0.0f) * off);

	((C_Object*)coil[2])->Init(modelData);
	coil[2]->SetOffsetPosition(Oyster::Math::Float3(0.397489709703f, -0.229263727694f, 0.0f) * off);

	((C_Object*)coil[3])->Init(modelData);
	coil[3]->SetOffsetPosition(Oyster::Math::Float3(0.0f, -0.459221058447f, 0.0f) * off);

	((C_Object*)coil[4])->Init(modelData);
	coil[4]->SetOffsetPosition(Oyster::Math::Float3(-0.397058802324f, -0.232241178703f, 0.0f) * off);

	((C_Object*)coil[5])->Init(modelData);
	coil[5]->SetOffsetPosition(Oyster::Math::Float3(-0.397489709703f, 0.230752453198f, 0.0f) * off);

	for(int i = 0; i < 6; i++)
	{
		coil[i]->Init();
	}

	maxRotation = (2*Oyster::Math::pi) / 6;
	maxRot = maxRotation;
	currentRotation = 0;
	smallStepRotation = 1.9;
	rotate = false;

	return true;
}

void FirstPersonWeapon::Shoot()
{
	if(!rotate)
	{
		timer.reset();
		elapsed = timer.getElapsedSeconds();
		rotate = true;

		coil[coilID]->Shoot();

		coilID--;
		if(coilID < 0)
			coilID = 5;
	}
}

void FirstPersonWeapon::Update(Oyster::Math::Matrix viewMatrix, Oyster::Math::Float3 look)
{
	if(!this->model) return;

	static int steps = 0;
	if(rotate && timer.getElapsedSeconds() > 0.2f)
	{
		if(currentRotation < maxRotation)
		{
			float dt = timer.getElapsedSeconds() - elapsed;
			float deltaRotation = 0;

			if(smallStepRotation * dt + currentRotation >= maxRotation)
			{
				currentRotation = maxRotation;
				/*for(int i = 0; i < 6; i++)
				{
					//coil[i]->SetRotate(-currentRotation);
				}*/
			}
			else
			{
				deltaRotation = smallStepRotation * dt;
				currentRotation += deltaRotation;
				/*for(int i = 0; i < 6; i++)
				{
					//coil[i]->SetRotate(-currentRotation);
				}*/
			}
		}
		else
		{
			steps++;
			rotate = false;
			maxRotation += maxRot;

			//Reset rotation after a full loop
			//To avoid any possible faults in rotation several loops.
			if(steps > 5)
			{
				maxRotation = maxRot;
				currentRotation = 0;
				steps = 0;
			}
		}
	}
	elapsed = timer.getElapsedSeconds();

	Matrix viewInverse = viewMatrix;
	viewInverse.Invert();
	
	angular.xyz = Float3(0, 0, 1);
	angular.w = -this->currentRotation;
	
	//Tried to use this instead of viewInverse directly
	//It did no difference.
	/*Oyster::Math3D::Float4x4 trans = Oyster::Math3D::TranslationMatrix(Float3(viewInverse.m14, viewInverse.m24, viewInverse.m34));
	Quaternion asd;
	asd.real = sqrt(1 + viewInverse.m11 + viewInverse.m22 + viewInverse.m33) / 2;
	asd.imaginary[0] = (viewInverse.m32 - viewInverse.m23)/(4 * asd.real);
	asd.imaginary[1] = (viewInverse.m13 - viewInverse.m31)/(4 * asd.real);
	asd.imaginary[2] = (viewInverse.m21 - viewInverse.m12)/(4 * asd.real);
	Oyster::Math3D::Float4x4 r = Oyster::Math3D::RotationMatrix(asd);
	//qw= sqrt(1 + m00 + m11 + m22) /2
	//qx = (m21 - m12)/( 4 *qw)
	//qy = (m02 - m20)/( 4 *qw)
	//qz = (m10 - m01)/( 4 *qw)
	*/

	Oyster::Math3D::Float4x4 rot2 = Oyster::Math3D::RotationMatrix(angular.w, angular.xyz);
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(initialRotation.w, initialRotation.xyz);
	Oyster::Math3D::Float4x4 translation2 = Oyster::Math3D::TranslationMatrix(Float3(1.1, -1.5, 0));
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(2 * -look.GetNormalized());
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->scale);
	//rot = rot * rot2;
	this->world = translation * viewInverse * translation2 * rot * rot2 * scale;

	base->SetWorld(translation * viewInverse * translation2 * rot * scale);

	base->Update();

	for(int i = 0; i < 6; i++)
		coil[i]->SetWorld(this->world);

	this->model->WorldMatrix = this->world;

	for(int i = 0; i < 6; i++)
	{
		coil[i]->Update();
	}
}

void FirstPersonWeapon::Render()
{
	base->Render();

	C_Object::Render();

	for(int i = 0; i < 6; i++)
		coil[i]->Render();
}

void FirstPersonWeapon::SetRotationSpeed(float speed)
{
	this->smallStepRotation = speed;
	//this->
}