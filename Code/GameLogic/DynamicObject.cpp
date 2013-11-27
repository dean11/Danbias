#include "DynamicObject.h"

using namespace GameLogic;
using namespace Oyster::Physics;
using namespace Utility::DynamicMemory;

DynamicObject::DynamicObject(void)
{
	rigidBody = API::Instance().CreateSimpleRigidBody();
	API::Instance().AddObject(rigidBody);
}


DynamicObject::~DynamicObject(void)
{
}
