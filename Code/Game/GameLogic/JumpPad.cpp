#include "JumpPad.h"
#include "PhysicsAPI.h"

using namespace GameLogic;
using namespace Oyster::Physics;

JumpPad::JumpPad(void)
{
}


JumpPad::JumpPad(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, Oyster::Math::Float3 pushForce)
	:StaticObject(rigidBody, collisionFuncAfter, type)
{
	
}



JumpPad::~JumpPad(void)
{
}
