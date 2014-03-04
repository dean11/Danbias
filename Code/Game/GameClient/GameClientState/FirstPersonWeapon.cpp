#include "FirstPersonWeapon.h"

using namespace DanBias::Client;
using namespace Oyster::Math;

Coil::Coil()
{
	maxGlow = Float3(2, 0, 0);
	minGlow = Float3(0, 2, 2);
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
	this->model->GlowTint = minGlow;
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
	//Create world matrix
	//Oyster::Math3D::Float4x4 rotTrans = Oyster::Math3D::TranslationMatrix(-offsetPosition);
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(this->scale);
	//Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(angular.w, angular.xyz);
	Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(offsetPosition);

	//rot = rotTrans * rot * rotTrans.GetInverse();
	this->model->WorldMatrix = this->world * translation * scale;

	this->model->Tint = this->model->GlowTint;

	//Update glow color
	Float3 colorChange = ((maxGlow - minGlow));

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
	modelData.scale = Oyster::Math::Float3(0.7,0.7,0.7);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1);
	modelData.id = -1;

	//Initial rotation on the entire weapon
	initialRotation = Float4(0, 1, 0, pi);
	initialRotation.xyz.Normalize();

	C_Object::Init(modelData);

	modelData.modelPath = L"wpn_massdriver_high_base.dan";
	modelData.visible = true;
	modelData.scale = Oyster::Math::Float3(1,1,1);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1);
	modelData.id = -1;

	((C_Object*)base)->Init(modelData);
	base->Init();

	modelData.modelPath = L"wpn_massdriver_high_coil.dan";
	modelData.visible = true;
	modelData.scale = Oyster::Math::Float3(1,1,1);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0, 0, 0), 1).GetNormalized();
	modelData.id = -2;

	Float3 off = 1.0f;

	((C_Object*)coil[0])->Init(modelData);
	coil[0]->SetOffsetPosition(Oyster::Math::Float3(0, 0.460931031166, 0) * off);

	((C_Object*)coil[1])->Init(modelData);
	coil[1]->SetOffsetPosition(Oyster::Math::Float3(0.397489709703, 0.230752453198, 0.0) * off);

	((C_Object*)coil[2])->Init(modelData);
	coil[2]->SetOffsetPosition(Oyster::Math::Float3(0.397489709703, -0.229263727694, 0.0) * off);

	((C_Object*)coil[3])->Init(modelData);
	coil[3]->SetOffsetPosition(Oyster::Math::Float3(0.0, -0.459221058447, 0.0) * off);

	((C_Object*)coil[4])->Init(modelData);
	coil[4]->SetOffsetPosition(Oyster::Math::Float3(-0.397058802324, -0.232241178703, 0.0) * off);

	((C_Object*)coil[5])->Init(modelData);
	coil[5]->SetOffsetPosition(Oyster::Math::Float3(-0.397489709703, 0.230752453198, 0.0) * off);

	for(int i = 0; i < 6; i++)
	{
		coil[i]->Init();
		//coil[i]->SetInitialRotation(initialRotation);
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
	static int steps = 0;
	if(rotate)
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
	Oyster::Math3D::Float4x4 rot2 = Oyster::Math3D::RotationMatrix(angular.w, angular.xyz);
	Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(initialRotation.w, initialRotation.xyz);
	Oyster::Math3D::Float4x4 translation2 = Oyster::Math3D::TranslationMatrix(Float3(1.1, -1.5, -0.5));
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