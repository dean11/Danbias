#include "Player.h"
#include "OysterMath.h"
#include "CollisionManager.h"
#include "Weapon.h"

using namespace GameLogic;
using namespace Oyster::Physics;

struct Player::PrivateData
{
	PrivateData()
	{
		weapon = new Weapon();
	}
	~PrivateData()
	{
		if (weapon)
		{
			delete weapon;
		}
	}

	int life;
	Weapon *weapon;

}myData;

Player::Player()
{
	myData = new PrivateData();

}
Player::~Player(void)
{
	delete myData;
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
