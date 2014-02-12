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

void C_Player::Init(ModelInitData modelInit)
{
	C_Object::Init(modelInit);
	Oyster::Graphics::API::PlayAnimation(model, L"movement");
	//Oyster::Graphics::API::Update(0.002f);
}
