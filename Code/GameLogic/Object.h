#ifndef OBJECT_H
#define OBJECT_H

#include "Model/Model.h"
#include "Render/Rendering/Render.h"
#include "Utilities.h"

#include "PhysicsAPI.h"

namespace GameLogic
{

	class Object
	{

	public:
		Object(void);
		virtual ~Object(void);
	
		
		void Render();

	private:

	protected:
		//either a model pointer or an ID to an arraypos filled with models that are to be rendered
		//rigidBody

		Utility::DynamicMemory::UniquePointer<Oyster::Physics::ICustomBody> rigidBody;
		Utility::DynamicMemory::UniquePointer<Oyster::Graphics::Render::Model> model;
	};

}

#endif 