//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef LEVEL_H
#define LEVEL_H

#include "StaticObject.h"
#include "DynamicObject.h"
#include "GameMode.h"

namespace GameLogic
{

	class Level
	{

	public:
		Level(void);
		~Level(void);

	private:
		StaticObject** staticObjects;
		int nrOfStaticObjects;

		DynamicObject** dynamicObjects;
		int nrOfDynamicObjects;

		GameMode* gameMode;



	};

}
#endif