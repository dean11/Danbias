#include "StaticObject.h"

using namespace GameLogic;



StaticObject::StaticObject()
	:Object()
{
	
}

StaticObject::StaticObject(void* collisionFunc, OBJECT_TYPE type)
	:Object(collisionFunc,type)
{

}
StaticObject::StaticObject(OBJECT_TYPE type)
	:Object(NULL,type)
{
	
}


StaticObject::~StaticObject(void)
{

}
