#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Utility::DynamicMemory;

DynamicObject::DynamicObject(std::wstring objFile)
	:Object(objFile)
{
	rigidBody->SetSubscription(CollisionManager::BoxCollision);
}


DynamicObject::~DynamicObject(void)
{
}

void DynamicObject::Update()
{
	//update object
}