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
		,ObjectSpecialType type, int objectID,Oyster::Math::Float3 pushForce);


		~JumpPad(void);

		static void JumpPadActivated(Oyster::Physics::ICustomBody *rigidBodyJumpPad, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

	private:
		Oyster::Math::Float3 pushForce;


	};
}
#endif
