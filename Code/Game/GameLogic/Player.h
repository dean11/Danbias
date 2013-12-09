//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H

namespace GameLogic
{
	class Player
	{

	public:
		Player(void);
		~Player(void);

		/********************************************************
		* Update the position of the rigid body
		* This will be done with physics later
		********************************************************/
		void Update();
		void Move();
		void Shoot();
	
	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif