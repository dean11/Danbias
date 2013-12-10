#ifndef DANBIAS_CLIENT_LOBBYSTATE_H
#define DANBIAS_CLIENT_LOBBYSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
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
	bool LoadModels(std::wstring file);
	bool InitCamera(Oyster::Math::Float3 startPos);
	ClientState Update(float deltaTime, InputClass* KeyInput);
	// create session lobby
	// join session lobby
	// set name
	// set rules
	// set map
	// ready 
	// chat
	// kick

	bool Render();
	bool Release();
};};};
#endif // ! DANBIAS_CLIENT_GAMECLIENTSTATE_H
