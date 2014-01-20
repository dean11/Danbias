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
	API::SimpleBodyDescription sbDesc;
	//sbDesc.centerPosition = 

	//poi
	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();


	Oyster::Physics::API::Instance().AddObject(rigidBody);

	//rigidBody->gameObjectRef = this;

	this->objectID = GID();

	this->type = OBJECT_TYPE::OBJECT_TYPE_UNKNOWN;
}

Object::Object(void* collisionFunc, OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;
	//sbDesc.centerPosition = 

	//poi
	this->rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();

	Oyster::Physics::API::Instance().AddObject(rigidBody);
	
	rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_Collision)(collisionFunc));

	//rigidBody->gameObjectRef = this;

	this->objectID = GID();

	this->type = type;
}


Object::~Object(void)
{

}

OBJECT_TYPE Object::GetType() const
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
	this->rigidBody->SetState(this->setState);
}
void Object::EndFrame()
{
	this->rigidBody->GetState(this->getState);
	this->setState = this->getState;
}