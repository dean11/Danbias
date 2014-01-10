#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include <GameProtocols.h>
#include "NetworkClient.h"


using namespace DanBias::Client;

struct  GameState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	std::vector<C_Object*> object;
	int modelCount;
	Oyster::Network::NetworkClient* nwClient;
	gameStateState state;

}privData;

GameState::GameState(void)
{
}


GameState::~GameState(void)
{

}
bool GameState::Init(Oyster::Network::NetworkClient* nwClient)
{
	// load models
	privData = new myData();
	privData->state = gameStateState_loading;
	privData->nwClient = nwClient;	
	privData->state = LoadGame();
	return true;
}
GameState::gameStateState GameState::LoadGame() 
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(0,3,0);
	plight.Color = Oyster::Math::Float3(0,1,0);
	plight.Radius = 5;
	plight.Bright = 2;
	Oyster::Graphics::API::AddLight(plight);
	LoadModels(L"map");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));
	return gameStateState_playing;
}
bool GameState::LoadModels(std::wstring mapFile)
{
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.world = Oyster::Math3D::Float4x4::identity;
	modelData.visible = true;
	modelData.modelPath = L"..\\Content\\worldDummy";
	modelData.id = 0;
	// load models
	C_Object* obj =  new C_Player();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);

	Oyster::Math3D::Float4x4 translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(-2,2,2));
	modelData.world = modelData.world * translate;
	modelData.modelPath = L"..\\Content\\worldDummy";
	modelData.id ++;

	obj = new C_DynamicObj();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);


	return true;
}
bool GameState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}

GameClientState::ClientState GameState::Update(float deltaTime, InputClass* KeyInput)
{
	switch (privData->state)
	{
	case gameStateState_loading:
		{
			// load map
			// wait for all players
			LoadGame();
			GameLogic::Protocol_General_Status gameStatus;
			gameStatus.status = GameLogic::Protocol_General_Status::States_ready;
			privData->nwClient->Send(gameStatus);
			privData->state = gameStateState_playing;
		}
		break;
	case gameStateState_playing:
		// read server data
		// update objects
		{
			bool send = false;
			GameLogic::Protocol_PlayerMovement movePlayer;
			movePlayer.bForward = false;
			movePlayer.bBackward = false;
			movePlayer.bStrafeLeft = false;
			movePlayer.bStrafeRight = false;
			movePlayer.bTurnLeft = false;
			movePlayer.bTurnRight = false;

			if(KeyInput->IsKeyPressed(DIK_W))
			{
				movePlayer.bForward = true;
				send = true;
			}
			if(KeyInput->IsKeyPressed(DIK_S))
			{
				movePlayer.bBackward = true;
				send = true;
			}
			if(KeyInput->IsKeyPressed(DIK_A))
			{
				movePlayer.bStrafeLeft = true;
				send = true;
			}
			if(KeyInput->IsKeyPressed(DIK_D))
			{
				movePlayer.bStrafeRight = true;
				send = true;
			} 

			if (privData->nwClient->IsConnected() && send)
			{
				privData->nwClient->Send(movePlayer);
			}

			//send delta mouse movement 
			if (KeyInput->IsMousePressed())
			{
				GameLogic::Protocol_PlayerMouse deltaMouseMove;
				deltaMouseMove.dxMouse = KeyInput->GetYaw();
				deltaMouseMove.dyMouse = KeyInput->GetPitch();
				//privData->nwClient->Send(deltaMouseMove);
			}
			
			// send event data
			//  
			if(KeyInput->IsKeyPressed(DIK_L))
				privData->state = GameState::gameStateState_end;
		}
		break;
	case gameStateState_end:
		return ClientState_Lobby;
		break;
	default:
		break;
	}
	
	// send key input to server. 
	return ClientState_Same;
}
bool GameState::Render()
{
	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection(privData->proj);
	Oyster::Graphics::API::NewFrame();
	for (int i = 0; i < privData->object.size(); i++)
	{
		privData->object[i]->Render();
	}
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	for (int i = 0; i < privData->object.size(); i++)
	{
		privData->object[i]->Release();
		delete privData->object[i];
		privData->object[i] = NULL;
	}

	delete privData;  
	privData = NULL;
	return true;
}

void GameState::Protocol(ProtocolStruct* pos)
{
	
}

void GameState::Protocol( PlayerPos* pos )
{
	Oyster::Math::Float4x4 world, translate;

	world = Oyster::Math::Float4x4::identity;
	translate = Oyster::Math::Float4x4::identity;
	translate = Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(pos->playerPos[0],pos->playerPos[1],pos->playerPos[2]));
	world = world * translate;
	privData->object[0]->setPos( world );
}

void GameState::Protocol( ObjPos* pos )
{
	Oyster::Math::Float4x4 world;
	for(int i = 0; i<16; i++)
	{
		world[i] = pos->worldPos[i];
	}

	for (int i = 0; i < privData->object.size(); i++)
	{
		if(privData->object[i]->GetId() == pos->object_ID)
			privData->object[i]->setPos(world);
	}
}

void GameState::Protocol( NewObj* pos )
{

	Oyster::Math::Float4x4 world;
	for(int i = 0; i<16; i++)
	{
		world[i] = pos->worldPos[i];
	}
	ModelInitData modelData;

	modelData.world = world;
	modelData.visible = true;
	modelData.id = pos->object_ID;
	//not sure if this is good parsing rom char* to wstring
	const char* path = pos->path;
	modelData.modelPath = std::wstring(path, path + strlen(path));  
	// load models
	C_Object* player = new C_Player();
	player->Init(modelData);

	privData->object.push_back(player);

}

void DanBias::Client::GameState::Protocol( RemoveObj* obj )
{
	for (int i = 0; i < privData->object.size(); i++)
	{
		if(privData->object[i]->GetId() == obj->object_ID)
		{
			privData->object.at(i)->Release();
			privData->object.erase(privData->object.begin() + i );
		}
	}
	//privData->object[obj->object_ID]->Release( );
}

//void GameState::Protocol(LightPos pos);