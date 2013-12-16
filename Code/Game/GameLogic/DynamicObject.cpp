#include "DynamicObject.h"
#include "CollisionManager.h"

using namespace GameLogic;


DynamicObject::DynamicObject()
	:Object()
{
	
}

DynamicObject::DynamicObject(void* collisionFunc, OBJECT_TYPE type)
	:Object(collisionFunc, type)
{

}


DynamicObject::~DynamicObject(void)
{

}