#include "Player.h"
#include "OysterMath.h"


using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Utility::DynamicMemory;




Player::Player(void)
	:Object()
{
	life = 100;
	
	Oyster::Physics::ICustomBody* temp = rigidBody = API::Instance().CreateSimpleRigidBody().Release();
	
	rigidBody->SetCenter(Oyster::Math::Float3(50,0,0));
	rigidBody->SetMass_KeepMomentum(30);
	rigidBody->SetSize(Oyster::Math::Float3(2,2,2));
	rigidBody->SetSubscription(true);
	rigidBody->SetMomentOfInertiaTensor_KeepMomentum(Oyster::Math::Float4x4( Oyster::Physics::MomentOfInertia::CreateCuboidMatrix(30, 2, 2, 2)));
	
	//API::Instance().AddObject(temp);
}


Player::~Player(void)
{
	delete this->rigidBody;
}
void Player::Update(keyInput keyPressed)
{
	if(keyPressed != keyInput_none)
	{
		Move();

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
	}

}

void Player::Move()
{
	//API::Instance().Update();
	/*Oyster::Math::Float3 pos = this->rigidBody->GetCenter();
	pos.x += 0.1;
	rigidBody->SetCenter(pos);*/
	//API::Instance().SetCenter(rigidBody, pos);
}
void Player::Shoot()
{

}
