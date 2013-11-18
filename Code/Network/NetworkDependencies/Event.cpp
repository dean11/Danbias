#include "Event.h"
using namespace Event;


//----------------------------
// BulletCreated class definitions
BulletCreated::BulletCreated(int ownerID, Float3 position, Float3 direction)
	:
	GameEvent()
{
	data.owner=ownerID;
	data.head=direction;
}
void BulletCreated::LoadRawData(char* d)
{
	memcpy(&data, d, GetSize());
	/*int offset=0;
	memcpy(&data.position, data, sizeof(Float3));
	offset+=sizeof(Float3);

	memcpy(&data.head, d+offset, sizeof(Float3));
	offset+=sizeof(Float3);
	memcpy(&data.owner, d+offset, sizeof(int));*/
}
void BulletCreated::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}

//----------------------------
// BulletHit class definitions
BulletHit::BulletHit(int attacker, int hitPlayer)
	:
	GameEvent()
{
	data.hitTarget=hitPlayer;
	data.attackingTarget=attacker;
	//this->hpLeft=hl;
	//this->shieldLeft=sl;
}
void BulletHit::LoadRawData(char* d)
{
	memcpy(&data, d, GetSize());
}
void BulletHit::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}

ScoreUpdate::ScoreUpdate(Score* scores)
{
	for (int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		data.scoreboard[i]=scores[i];
	}
}
void ScoreUpdate::LoadRawData(char* d)
{
	memcpy(&data, d, GetSize());
}
void ScoreUpdate::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}

//----------------------------
// ShipSpawned class definitions
ShipSpawned::ShipSpawned(Float3 position, int id)
	:
	GameEvent()
{
	data.position=position;
	data.playerID=id;
}
void ShipSpawned::LoadRawData(char* d)
{
	memcpy(&data, d, this->GetSize());
	/*int offset=0;
	memcpy(&data.position, data, sizeof(Float3));
	offset+=sizeof(Float3);

	memcpy(&playerID, data+offset, sizeof(int));*/
}
void ShipSpawned::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}


//----------------------------
// GameEnded class definitions
GameEnded::GameEnded()
	:
	GameEvent()
{
}
GameEnded::GameEnded(int winner)
	:
	GameEvent()
{
	data.winningTeam=winner;
}
void GameEnded::LoadRawData(char* d)
{
	memcpy(&data, d, GetSize());
	/*int offset=0;
	memcpy(&eventPosition, data, sizeof(Float3));
	offset+=sizeof(Float3);

	memcpy(&winningTeam, data+offset, sizeof(int));
	offset+=sizeof(int);

	for (int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		memcpy(&data.scoreboard[i], data+offset, sizeof(Score));
		offset+=sizeof(Score);
	}*/
}
void GameEnded::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}
void GameEnded::setScore(int i, Score score)
{
	data.scoreboard[i]=score;
}
void GameEnded::setScore(int i, int k, int d, int tk)
{
	data.scoreboard[i].id=i;
	data.scoreboard[i].kills=k;
	data.scoreboard[i].deaths=d;
	data.scoreboard[i].teamkills=tk;
}
void GameEnded::sortScore()
{
	float sort[PLAYER_MAX_COUNT];
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		sort[i]=((float)(data.scoreboard[i].kills-data.scoreboard[i].teamkills))/(float)data.scoreboard[i].deaths;
	}
	Score tmp;
	int bestID=0;
	float bestScore;
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		bestScore=sort[i];
		bestID=i;
		for (int j=i; j<PLAYER_MAX_COUNT; j++)
		{
			if(bestScore<sort[j])
			{
				bestID=j;
				bestScore=sort[j];
			}
		}
		tmp=data.scoreboard[i];
		data.scoreboard[i]=data.scoreboard[bestID];
		data.scoreboard[bestID]=tmp;
	}
}

//----------------------------
// ShipDestroyed class definitions
ShipDestroyed::ShipDestroyed(int pid, int kid)
	:
	GameEvent()
{
	data.playerID=pid;
	data.killerID=kid;
}
void ShipDestroyed::setScore(int i, Score score)
{
	data.scoreboard[i]=score;
}
void ShipDestroyed::setScore(int i, int k, int d, int tk)
{
	data.scoreboard[i].id=i;
	data.scoreboard[i].kills=k;
	data.scoreboard[i].deaths=d;
	data.scoreboard[i].teamkills=tk;
}
void ShipDestroyed::sortScore()
{
	float sort[PLAYER_MAX_COUNT];
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		sort[i]=((float)(data.scoreboard[i].kills-data.scoreboard[i].teamkills))/data.scoreboard[i].deaths;
	}
	Score tmp;
	int bestID=0;
	float bestScore;
	for(int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		bestScore=sort[i];
		bestID=i;
		for (int j=i; j<PLAYER_MAX_COUNT; j++)
		{
			if(bestScore<sort[j])
			{
				bestID=j;
				bestScore=sort[j];
			}
		}
		tmp=data.scoreboard[i];
		data.scoreboard[i]=data.scoreboard[bestID];
		data.scoreboard[bestID]=tmp;
	}
}
void ShipDestroyed::LoadRawData(char* d)
{
	memcpy(&data, d, GetSize());
	/*int offset=0;
	memcpy(&eventPosition, data, sizeof(Float3));
	offset+=sizeof(Float3);

	memcpy(&playerID, data+offset, sizeof(int));
	offset+=sizeof(int);
	memcpy(&killerID, data+offset, sizeof(int));
	offset+=sizeof(int);
	for (int i=0; i<PLAYER_MAX_COUNT; i++)
	{
		memcpy(&data.scoreboard[i], data+offset, sizeof(Score));
		offset+=sizeof(Score);
	}*/
}
void ShipDestroyed::SaveRawData(char* d)
{
	memcpy(d, &data, GetSize());
}




Event::Type Event::getEventType(Event::GameEvent* evt)
{
	if (typeid(*evt)==typeid(Event::BulletCreated))
	{
		return eBulletCreated;
	}
	else if(typeid(*evt)==typeid(Event::BulletHit))
	{
		return eBulletHit;
	}
	else if(typeid(*evt)==typeid(Event::ShipSpawned))
	{
		return eShipSpawned;
	}
	else if(typeid(*evt)==typeid(Event::GameEnded))
	{
		return eGameEnded;
	}
	else if(typeid(*evt)==typeid(Event::ShipDestroyed))
	{
		return eShipDestroyed;
	}
	else if(typeid(*evt)==typeid(Event::ScoreUpdate))
	{
		return eScoreUpdate;
	}
	printf("UNSUPPORTED EVENT at getEventType, Event.cpp\n");
	return UNSUPPORTED_TYPE;
}