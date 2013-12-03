#include "Player.h"
#include "OysterMath.h"

using namespace GameLogic;
using namespace Oyster::Physics;

Player::Player(void)
	:Object()
{
	life = 100;
}
Player::~Player(void)
{
	delete this->rigidBody;
}

void Player::Update(keyInput keyPressed)
{
	if(keyPressed != keyInput_none)
	{
		Move(keyPressed);
	}
}

void Player::Move(keyInput keyPressed)
{
	if(keyPressed == keyInput_A)
	{
		Oyster::Math::Float3 pos = this->rigidBody->GetCenter();
		pos.x -= 0.1;
		rigidBody->SetCenter(pos);
	}
	if(keyPressed == keyInput_D)
	{
		Oyster::Math::Float3 pos = this->rigidBody->GetCenter();
		pos.x += 0.1;
		rigidBody->SetCenter(pos);
	}
	if(keyPressed == keyInput_S)
	{
		Oyster::Math::Float3 pos = this->rigidBody->GetCenter();
		pos.y -= 0.1;
		rigidBody->SetCenter(pos);
	}
	if(keyPressed == keyInput_W)
	{
		Oyster::Math::Float3 pos = this->rigidBody->GetCenter();
		pos.y += 0.1;
		rigidBody->SetCenter(pos);
	}
}
void Player::Shoot()
{

}
