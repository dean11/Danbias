#pragma once
#include "GameClientState.h"
namespace DanBias
{
	namespace Client
	{
		
class LobbyState : public GameClientState
{
private:
	struct myData;
	myData* privData;
public:
	LobbyState(void);
	~LobbyState(void);
	bool Init();
	ClientState Update(float deltaTime, InputClass* KeyInput);
	bool Render();
	bool Release();
};};};
