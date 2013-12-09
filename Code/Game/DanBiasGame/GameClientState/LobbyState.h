#ifndef DANBIAS_CLIENT_GAMECLIENTSTATE_H
#define DANBIAS_CLIENT_GAMECLIENTSTATE_H

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
#endif // ! DANBIAS_CLIENT_GAMECLIENTSTATE_H
