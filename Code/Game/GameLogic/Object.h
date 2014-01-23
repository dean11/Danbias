//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "GameLogicStates.h"
#include <PhysicsAPI.h>

namespace GameLogic
{
	class Game;

	class Object
	{
	public:		
		Object();
		Object(void* collisionFunc, OBJECT_TYPE type);
		Object(Oyster::Physics::ICustomBody *rigidBody ,void* collisionFunc, OBJECT_TYPE type);
		~Object(void);

		OBJECT_TYPE GetType() const;
		int GetID() const;
		Oyster::Physics::ICustomBody* GetRigidBody();
		void ApplyLinearImpulse(Oyster::Math::Float4 force);

		void BeginFrame();
		void EndFrame();

	private:
		OBJECT_TYPE type;
		int objectID;

	protected:
		Oyster::Physics::ICustomBody *rigidBody;
		Oyster::Physics::ICustomBody::State setState;
		Oyster::Physics::ICustomBody::State getState;

		static const Game* gameInstance;

	};

}

#endif 