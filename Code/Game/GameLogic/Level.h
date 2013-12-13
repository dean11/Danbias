//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef LEVEL_H
#define LEVEL_H

namespace GameLogic
{

	class Level
	{

	public:
		Level(void);
		~Level(void);

		void InitiateLevel(std::string levelPath);

	private:
		struct PrivateData;
		PrivateData *myData;

	};

}
#endif