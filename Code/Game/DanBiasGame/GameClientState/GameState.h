#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
namespace DanBias
{
namespace Client
{
class GameState : public GameClientState
{
	enum gameStateState
	{
		gameStateState_loading,
		gameStateState_playing,
		gameStateState_end,
	};
private:
	
	struct myData;
	myData* privData;
public:
	GameState(void);
	~GameState(void);
	bool Init(Oyster::Network::NetworkClient* nwClient);
	GameClientState::ClientState Update(float deltaTime, InputClass* KeyInput) override;
	bool LoadModels(std::wstring mapFile) ;
	bool InitCamera(Oyster::Math::Float3 startPos) ;
	gameStateState LoadGame();

	bool Render()override;
	bool Release()override;

	void Protocol(ProtocolStruct* pos)override;
	void Protocol(PlayerPos* pos);
	void Protocol(ObjPos* pos);
	void Protocol(KeyInput* pos);
	void PlayerPosProtocol(PlayerPos* pos);
	void ObjectPosProtocol(ObjPos* pos);
	//void Protocol(LightPos pos);
};
};
};
#endif