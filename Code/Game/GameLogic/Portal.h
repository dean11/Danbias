#ifndef PORTAL_H
#define PORTAL_H
#include "StaticObject.h"
namespace GameLogic
{
	class Portal : public StaticObject
	{
	public:
		Portal(void);

		Portal(Oyster::Physics::ICustomBody *rigidBody 
			,void (*EventOnCollision)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss)
			,ObjectSpecialType type,int objectID ,Oyster::Math::Float3 portalExit);

		~Portal(void);

		static void PortalActivated(Oyster::Physics::ICustomBody *rigidBodyPortal, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
		
	private:
		Oyster::Math::Float3 portalExit;
	};
}
#endif