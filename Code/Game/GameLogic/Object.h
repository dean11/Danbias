//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H


#include "GameLogicStates.h"
#include <PhysicsAPI.h>

namespace GameLogic
{
	class Object
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
		Oyster::Physics::ICustomBody::State state;

	};

}

#endif 