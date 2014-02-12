#ifndef JUMPPAD_H
#define JUMPPAD_H
#include "StaticObject.h"
namespace GameLogic
{
	class JumpPad : public StaticObject
	{
	public:
		JumpPad(void);

		JumpPad(Oyster::Physics::ICustomBody *rigidBody 
			,Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss)
			,OBJECT_TYPE type, Oyster::Math::Float3 pushForce);

		~JumpPad(void);

		static void JumpPadActivated(Oyster::Physics::ICustomBody *rigidBodyJumpPad, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

	private:
		Oyster::Math::Float3 pushForce;


	};
}
#endif
