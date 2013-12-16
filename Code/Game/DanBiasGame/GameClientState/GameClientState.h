#ifndef DANBIAS_CLIENT_GAMECLIENTSTATE_H
#define DANBIAS_CLIENT_GAMECLIENTSTATE_H

#define NOMINMAX
#include "L_inputClass.h"

namespace DanBias
{
namespace Client
{

class GameClientState
{
public:
	struct ProtocolStruct
	{

	};
	struct  ObjPos :public ProtocolStruct
	{
		float worldPos[16]; 
	};
	struct  PlayerPos :public ProtocolStruct
	{
		float playerPos[3]; 
	};
	struct  PlayerMove :public ProtocolStruct
	{
		float playerPos[3]; 
	};
	struct  PlayerName :public ProtocolStruct
	{
		char name[255]; 
	};
	enum ClientState
	{
		ClientState_Lobby,
		ClientState_Game,
		ClientState_Same,
	};

public:
	GameClientState(void);
	virtual ~GameClientState(void);
	virtual bool Init() = 0;
	virtual ClientState Update(float deltaTime, InputClass* KeyInput) = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
	virtual void Protocol(ProtocolStruct* protocolStruct) = 0;

};
};
};
#endif