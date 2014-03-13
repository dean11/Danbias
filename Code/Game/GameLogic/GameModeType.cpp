#include "GameModeType.h"

using namespace GameLogic;


GameModeType::GameModeType()
{
}


GameModeType::~GameModeType(void)
{

}
void GameModeType::initGameMode( EndConditions end )
{
	this->endConditions = end;
}
void GameModeType::Update( float deltaTime)
{
	this->endConditions.timer += deltaTime;
}
bool GameModeType::TimeExit()
{
	if( this->endConditions.timer > this->endConditions.endTimeSec )
	{
		return true;
	}
	return false;
}
int GameModeType::EndConditionMet( Utility::DynamicMemory::DynamicArray<Player*>& players )
{
	int winnerID = -1; 
	int highetsScore = 0;
	int highetsScoreID = 0;
	for(int i = 0; i < (int)players.Size(); i++)
	{
		if(players[i])
		{
			if(!players[highetsScoreID] || (players[i]->GetKills() > players[highetsScoreID]->GetKills() ) )
			{
				highetsScore = i;
			}
			if(players[i]->GetKills() > this->endConditions.killCount )
			{
				winnerID = i;
			}
		}
	}

	// if timer ends, the person with the highest score is the winner
	if( this->endConditions.timer > this->endConditions.endTimeSec )
	{
		return highetsScoreID;
	}
	return winnerID;
}
float GameModeType::GetGameTime() const
{
	return this->endConditions.endTimeSec - this->endConditions.timer;
}