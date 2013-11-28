//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "PhysicsAPI.h"
#include "DllInterfaces/GFXAPI.h"

#include "Model/Model.h"
#include "Utilities.h"


namespace GameLogic
{
	class Object
	{
	public:
		Object(void);
		virtual ~Object(void);

		enum OBJECT_TYPE
		{
			OBJECT_TYPE_PLAYER,
			OBJECT_TYPE_BOX,
		};
		
<<<<<<< HEAD
		void Render();
		void Update();
=======
		Oyster::Graphics::Model::Model* Render();
>>>>>>> 01515a4d2d309cfebf4e3dab3ad68787f93050e1

		OBJECT_TYPE GetType();

	private:
		OBJECT_TYPE type;

	protected:
		//either a model pointer or an ID to an arraypos filled with models that are to be rendered
		//rigidBody

<<<<<<< HEAD
		Utility::DynamicMemory::UniquePointer<Oyster::Physics::ICustomBody> rigidBody;
		Utility::DynamicMemory::UniquePointer<Oyster::Graphics::Model::Model> model;
=======
		Oyster::Physics::ICustomBody* rigidBody;
		Oyster::Graphics::Model::Model* model;
>>>>>>> 01515a4d2d309cfebf4e3dab3ad68787f93050e1
	};

}

#endif 