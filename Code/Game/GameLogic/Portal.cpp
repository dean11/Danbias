#include "Portal.h"

using namespace GameLogic;
using namespace Oyster::Math;

Portal::Portal(void)
	:StaticObject()
{
	this->portalExit = Float3(0,0,0);
}


Portal::Portal(Oyster::Physics::ICustomBody *rigidBody, ObjectSpecialType type,int objectID ,Oyster::Math::Float3 portalExit)
	:StaticObject(rigidBody, Portal::PortalActivated, type, objectID)
{
	this->portalExit = portalExit;
}

Portal::~Portal(void)
{

}

