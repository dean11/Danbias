#pragma once
namespace DanBias
{
namespace Client
{

class GameClientState
{
public:
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
	virtual ClientState Update() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
};
};
};
