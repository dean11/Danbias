#include "C_Player.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

C_Player::C_Player(void)
	:C_DynamicObj()
{

}

C_Player::~C_Player(void)
{
	 
}

bool C_Player::Init(ModelInitData modelInit)
{
	return C_Object::Init(modelInit);
}

void C_Player::playAnimation(std::wstring animation, bool loop)
{
	if(model)
		Oyster::Graphics::API::PlayAnimation(model, animation, loop);
}

void C_Player::stopAnimation(std::wstring animation)
{
	if(model)
		Oyster::Graphics::API::StopAnimation(model, animation);
}

void C_Player::stopAllAnimations()
{
	Oyster::Graphics::API::StopAllAnimations(model);
}