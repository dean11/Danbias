#include "C_Light.h"
using namespace DanBias::Client;
C_Light::C_Light( Oyster::Graphics::Definitions::Pointlight pointLightDesc, int id )
{
	this->pointLightDesc = pointLightDesc;
	this->id = id;
}
C_Light::~C_Light()
{

}
Oyster::Graphics::Definitions::Pointlight C_Light::getLightDesc() const
{
	return this->pointLightDesc;
}
void C_Light::setLightDesc( Oyster::Graphics::Definitions::Pointlight pointLightDesc )
{
	this->pointLightDesc = pointLightDesc;
}
Oyster::Math::Float3 C_Light::getPos() const
{
	return this->pointLightDesc.Pos;
}
void C_Light::setPos( Oyster::Math::Float3 newPos)
{
	this->pointLightDesc.Pos = newPos;
}	

int C_Light::GetId() const
{
	return this->id;
}
void C_Light::Render()
{
	// will be changed to new API 
	Oyster::Graphics::API::AddLight(&pointLightDesc);
}

void C_Light::Release()
{
	// will be changed to new API 
	Oyster::Graphics::API::RemoveLight(&pointLightDesc);
}
