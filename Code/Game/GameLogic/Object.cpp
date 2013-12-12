#include "Object.h"
#include "OysterMath.h"
#include "RefManager.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Physics;

Object::Object()
{	
	API::SimpleBodyDescription sbDesc;
	//sbDesc.centerPosition = 

	//poi
	ICustomBody* temp = rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();

	GameLogic::RefManager::getInstance()->AddMapping(*rigidBody, *this);

	this->type = OBJECT_TYPE_UNKNOWN;

}

Object::Object(void* collisionFunc, OBJECT_TYPE type)
{
	API::SimpleBodyDescription sbDesc;
	//sbDesc.centerPosition = 

	//poi
	ICustomBody* temp = rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	
	rigidBody->SetSubscription((Oyster::Physics::ICustomBody::EventAction_Collision)(collisionFunc));

	GameLogic::RefManager::getInstance()->AddMapping(*rigidBody, *this);

	this->type = type;
}


Object::~Object(void)
{


}

OBJECT_TYPE Object::GetType()
{
	return this->type;
}
