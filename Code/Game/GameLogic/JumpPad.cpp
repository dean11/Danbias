#include "JumpPad.h"
#include "PhysicsAPI.h"

using namespace GameLogic;
using namespace Oyster::Physics;

JumpPad::JumpPad(void)
{
}

JumpPad::JumpPad(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), OBJECT_TYPE type, Oyster::Math::Float3 pushForce)
	:StaticObject(rigidBody, collisionFuncAfter, type)
{
	this->pushForce = pushForce;
}



JumpPad::~JumpPad(void)
{
}
