#include "C_ClientLogic.h"

using namespace DanBias::Client;
using namespace Oyster;

C_ClientLogic::UpdateRotation::UpdateRotation(C_ClientLogic* clientLogic) : UpdateMethod(clientLogic)
{

}
C_ClientLogic::UpdateRotation::~UpdateRotation()
{

}

void C_ClientLogic::UpdateRotation::Update( ::Oyster::Math::Float deltaTime )
{
	Math3D::Quaternion rotationRelative = Math3D::Rotation( this->clientLogic->rotationSpeed*deltaTime, this->clientLogic->roataionVector );
	Math3D::Quaternion rotationCurrent =clientLogic->getRotation();
	clientLogic->setRot( rotationCurrent * rotationRelative);
	clientLogic->updateWorld();
}

C_ClientLogic::UpdateLight::UpdateLight(C_ClientLogic* clientLogic) : UpdateMethod(clientLogic)
{

}
C_ClientLogic::UpdateLight::~UpdateLight()
{

}

void C_ClientLogic::UpdateLight::Update( ::Oyster::Math::Float deltaTime )
{
	if(clientLogic->LightState)
	{
		clientLogic->interpolated += deltaTime;
	}
	else
	{
		clientLogic->interpolated -= deltaTime;
	}
	
	if(clientLogic->interpolated > clientLogic->interpolationSpeed || clientLogic->interpolated < 0)
	{
		clientLogic->LightState = !clientLogic->LightState;
	}
	float val = clientLogic->interpolated / clientLogic->interpolationSpeed;
	clientLogic->currentLight.Bright =  (clientLogic->origLight.Bright * val) + (clientLogic->origLight.Bright * 0.5f * (1.0f - val));
	clientLogic->currentLight.Radius =  (clientLogic->origLight.Radius * val) + (clientLogic->origLight.Radius * 0.5f * (1.0f - val));
}

C_ClientLogic::C_ClientLogic()
{

}

void C_ClientLogic::Init(ModelInitData desc,UpdateMethod* updateMethod, Oyster::Graphics::Definitions::Pointlight origLight)
{
	C_Object::Init(desc);
	updater = updateMethod;
	this->origLight = origLight;
	currentLight = origLight;
	LightState = true;
	interpolated = 0;
	interpolationSpeed = ((float)rand() / (RAND_MAX + 1) * (10 - 5) + 5);
	Graphics::API::AddLight(&currentLight);
}
void C_ClientLogic::Init(ModelInitData desc,UpdateMethod* updateMethod, float speed, Oyster::Math::Float3 axis)
{
	C_Object::Init(desc);
	rotationSpeed = speed;
	roataionVector = axis.GetNormalized();
	updater = updateMethod;
}
C_ClientLogic::~C_ClientLogic()
{
	delete updater;
	Oyster::Graphics::API::RemoveLight(&currentLight);
}

void C_ClientLogic::Update( ::Oyster::Math::Float deltaTime )
{
	updater->Update(deltaTime);
}

C_ClientLogic::UpdateMethod::UpdateMethod( C_ClientLogic* clientLogic)
{
	this->clientLogic = clientLogic;
}

C_ClientLogic::UpdateMethod::~UpdateMethod()
{

}