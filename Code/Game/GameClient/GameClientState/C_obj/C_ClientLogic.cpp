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

C_ClientLogic::C_ClientLogic()
{

}

void C_ClientLogic::Init(ModelInitData desc,UpdateMethod* updateMethod, Oyster::Graphics::Definitions::Pointlight origLight)
{

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