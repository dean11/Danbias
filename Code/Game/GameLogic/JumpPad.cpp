#include "JumpPad.h"
#include "PhysicsAPI.h"

using namespace GameLogic;
using namespace Oyster::Physics;

JumpPad::JumpPad(void)
{
}

JumpPad::JumpPad(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType  type,int objectID ,Oyster::Math::Float3 pushForce)
	:StaticObject(rigidBody, JumpPad::JumpPadActivated, type, objectID)
{
	this->pushForce = pushForce;
}



JumpPad::~JumpPad(void)
{
}
