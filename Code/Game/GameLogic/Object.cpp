#include "Object.h"
#include "OysterMath.h"
#include "CollisionManager.h"
#include "GID.h"
#include "PhysicsAPI.h"
#include "Game.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Physics;

const Game *Object::gameInstance = (Game*)(&Game::Instance());

Object::Object()
{	

	this->rigidBody = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.0f, 0.0f, 0.0f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);

	this->type = ObjectSpecialType_Unknown;
	this->objectID = -1;
}

Object::Object(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
{
	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
	this->type = type;
	this->objectID = objectID;
	this->rigidBody->SetCustomTag(this);
}


Object::Object(Oyster::Physics::ICustomBody *rigidBody , Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID)
{
	this->rigidBody = rigidBody;
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
	this->type = type;
	this->objectID = objectID;
	this->rigidBody->SetCustomTag(this);
}

Object::~Object(void)
{

}

ObjectSpecialType Object::GetObjectType() const
{
	return this->type;
}
int Object::GetID() const
{
	return this->objectID;
}

Oyster::Physics::ICustomBody* Object::GetRigidBody()
{
	return this->rigidBody;
}


void Object::BeginFrame()
{
	
}
// update physic 
void Object::EndFrame()
{

}

void Object::setBeforeCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncBefore)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter))
{
	//this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_BeforeCollisionResponse)(collisionFuncBefore));
}
void Object::setAfterCollisonFunc(Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss))
{
	this->rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_AfterCollisionResponse)(collisionFuncAfter));
}

Oyster::Math::Float3 Object::GetPosition() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.centerPos;
}
Oyster::Math::Quaternion Object::GetRotation() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.quaternion;
}
Oyster::Math::Float3 Object::GetScale() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return Float3();
}
Oyster::Math::Float4x4 Object::GetOrientation() 
{
	Oyster::Physics::ICustomBody::State state; 
	state = this->rigidBody->GetState();
	return state.GetOrientation();
}