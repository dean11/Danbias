//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "PhysicsAPI.h"
#include "GameLogicStates.h"


namespace GameLogic
{
	class Object
	{
	public:		
		Object();
		Object(void* collisionFunc, OBJECT_TYPE type);
		~Object(void);

		OBJECT_TYPE GetType();

		Oyster::Physics::ICustomBody* GetRigidBody();

	private:
		OBJECT_TYPE type;
	protected:
		Oyster::Physics::ICustomBody *rigidBody;
	};

}

#endif 