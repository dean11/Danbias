#include "Portal.h"

using namespace GameLogic;
using namespace Oyster::Math;

Portal::Portal(void)
	:StaticObject()
{
	this->portalExit = Float3(0,0,0);
}

<<<<<<< HEAD
Portal::Portal(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, Oyster::Math::Float3 portalExit)
	:StaticObject(rigidBody, collisionFuncAfter, type)
=======
Portal::Portal(Oyster::Physics::ICustomBody *rigidBody, void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type,int objectID ,Oyster::Math::Float3 portalExit)
	:StaticObject(rigidBody, EventOnCollision, type, objectID)
>>>>>>> GL - fixed constructors to ease creation of objects, also fixed weapon crash on destruction
{
	this->portalExit = portalExit;
}

Portal::~Portal(void)
{

}

