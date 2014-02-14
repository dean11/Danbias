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
			,ObjectSpecialType type,int objectID ,Oyster::Math::Float3 portalExit);

		~Portal(void);

		static void PortalActivated(Oyster::Physics::ICustomBody *rigidBodyPortal, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);
		
	private:
		Oyster::Math::Float3 portalExit;
	};
}
#endif