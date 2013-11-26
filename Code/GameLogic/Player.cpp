#include "Player.h"
#include "OysterMath.h"


using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Utility::DynamicMemory;




Player::Player(void)
	:Object()
{
	life = 100;

	rigidBody = API::Instance().CreateSimpleRigidBody();
	API::Instance().AddObject(rigidBody);
}


Player::~Player(void)
{
	
}
void Player::Update()
{

}

void Player::Move()
{

}
void Player::Shoot()
{

}
