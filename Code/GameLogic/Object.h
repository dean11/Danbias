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
		
		Oyster::Graphics::Model::Model* Render();

		OBJECT_TYPE GetType();

	private:
		OBJECT_TYPE type;

	protected:
		//either a model pointer or an ID to an arraypos filled with models that are to be rendered
		//rigidBody

		Oyster::Physics::ICustomBody* rigidBody;
		Oyster::Graphics::Model::Model* model;
	};

}

#endif 