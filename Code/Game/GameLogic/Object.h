//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "PhysicsAPI.h"
#include "GameLogicStates.h"
#include "GameLogicDef.h"


namespace GameLogic
{
	class DANBIAS_GAMELOGIC_DLL Object
	{
	public:		
		Object();
		Object(void* collisionFunc, OBJECT_TYPE type);
		~Object(void);

		OBJECT_TYPE GetType();
		int GetID();

		Oyster::Physics::ICustomBody* GetRigidBody();

	private:
		OBJECT_TYPE type;
		int objectID;
	protected:
		Oyster::Physics::ICustomBody *rigidBody;
	};

}

#endif 