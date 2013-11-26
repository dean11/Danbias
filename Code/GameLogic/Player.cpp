#include "Player.h"
#include "OysterMath.h"


using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Utility::DynamicMemory;


//void ColisionEvent(unsigned int obj1Ref, unsigned int obj2Ref)
//{
//	const ICustomBody *body1 = &API::Instance().Peek( obj1Ref );
//	const ICustomBody *body2 = &API::Instance().Peek( obj2Ref );
//	if( body1 != &Error::nobody )
//	{
//		Object *obj1 = (Object*) const_cast<ICustomBody*>(body1);
//		
//		Object *obj2 = (Object*) const_cast<ICustomBody*>(body2);
//
//		//switch( obj1->type  )
//		//{
//		//case Player: 
//		//	switch (obj2->type)
//		//	{
//		//	case låda:
//		//		//action
//		//		//soud
//		//		//particle effect
//
//		//	}
//
//		//case låda :
//
//
//		//}
//	}
//
//}
void DestructionEvent(unsigned int obj1, ::Utility::DynamicMemory::UniquePointer<ICustomBody> obj2)
{

}

Player::Player(void)
	:Object()
{
	life = 10;

	rigidBody = API::Instance().CreateSimpleRigidBody();


	//ref = API::Instance().AddObject(rigidBody);
	////ref = API::Instance().AddObject(API::Instance().CreateSimpleRigidBody());
	//const ICustomBody* rB;

	////rB = &API::Instance().Peek(ref);
	////if( rB == &Error::nobody)
	////{
	////	//error
	////}

	//API::Instance().SetCenter(ref, Oyster::Math::Float3::null);
	//API::Instance().SetMass_KeepMomentum(ref, 20);
	//// get Tensor matrix (tröghetsmonent)
	////API::Instance().SetMomentOfInertiaTensor_KeepMomentum(ref, tensorMatrix )
	//
	////Oyster::Math::Float3 ve = rB->GetCenter();
	//API::Instance().SetDeltaTime(0.01f);

	////API::Instance().ApplyForceAt(ref, rB->GetCenter(), Oyster::Math::Float3::null);
	//
	////API::Instance().SetAction(ColisionEvent);

	////API::Instance().Update();



}


Player::~Player(void)
{
	
}
void Player::Update()
{
	//API::Instance().API::Update();
	//API::Instance().API::ApplyForceAt(ref, )
}

void Player::Move()
{

}
void Player::Shoot()
{

}
