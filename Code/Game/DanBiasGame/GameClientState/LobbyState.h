#pragma once
#include "GameClientState.h"
namespace DanBias
{
	namespace Client
	{
		struct myData;
class LobbyState : public GameClientState
{
private:
	myData* privData;
public:
	LobbyState(void);
	~LobbyState(void);
	bool Init();
	ClientState Update();
	bool Render();
	bool Release();
};};};
