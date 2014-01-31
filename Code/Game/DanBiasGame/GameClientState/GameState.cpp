#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera.h"
#include <GameServerAPI.h>

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
	key_forward = false;
	key_backward = false;
	key_strafeRight = false;
	key_strafeLeft = false;
}


GameState::~GameState(void)
{

}
bool GameState::Init(Oyster::Network::NetworkClient* nwClient)
{
	// load models
	camera = new Camera;
	privData = new myData();
	privData->state = gameStateState_loading;
	privData->nwClient = nwClient;	
	privData->state = LoadGame();
	
	return true;
}
GameState::gameStateState GameState::LoadGame() 
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(0,15,5);
	plight.Color = Oyster::Math::Float3(0,1,0);
	plight.Radius = 50;
	plight.Bright = 2;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Oyster::Math::Float3(10,15,5);
	plight.Color = Oyster::Math::Float3(1,0,0);
	plight.Radius = 50;
	plight.Bright = 2;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Oyster::Math::Float3(10,-15,5);
	plight.Color = Oyster::Math::Float3(0,0,1);
	plight.Radius = 50;
	plight.Bright = 2;

	Oyster::Graphics::API::AddLight(plight);
	LoadModels(L"map");
	InitCamera(Oyster::Math::Float3(0,0,20.0f));
	return gameStateState_playing;
}
bool GameState::LoadModels(std::wstring mapFile)
{
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	// add world model
	ModelInitData modelData;
	Oyster::Math3D::Float4x4 translate;
	C_Object* obj;
	translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(0,0,0));
	Oyster::Math3D::Float4x4 scale = Oyster::Math3D::Float4x4::identity;
<<<<<<< HEAD
	scale.v[0].x = 0.2;
	scale.v[1].y = 0.2;
	scale.v[2].z = 0.2;
	scale.v[3].w = 1;
	modelData.world = translate  ;//modelData.world * translate 
	modelData.modelPath = L"world_earth.dan";
=======
	scale.v[0].x = 8;
	scale.v[1].y = 8;
	scale.v[2].z = 8;
	modelData.world = scale; //modelData.world * translate 
	modelData.modelPath = L"..\\Content\\Models\\ball.dan";
>>>>>>> cec022b3814c680d5be161e7cfe0596841736c77
	modelData.id = 0;

	obj = new C_Player();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);

	// add box model
	modelData.world = Oyster::Math3D::Float4x4::identity;
<<<<<<< HEAD
	translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(4,320,0));
=======
	translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(-5,15,0));
>>>>>>> cec022b3814c680d5be161e7cfe0596841736c77
	modelData.world = modelData.world * translate;
	modelData.modelPath = L"..\\Content\\Models\\box.dan";
	modelData.id = 1;

	obj = new C_Player();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);
	modelData.world = Oyster::Math3D::Float4x4::identity;

	// add player model
	modelData.world = Oyster::Math3D::Float4x4::identity;
	translate =  Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(0, 15, 0));

	modelData.world = modelData.world * translate;
	modelData.visible = true;
	modelData.modelPath = L"..\\Content\\Models\\char_white.dan";
	modelData.id = 2;
	// load models
	obj =  new C_Player();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);



	return true;
}
bool GameState::InitCamera(Oyster::Math::Float3 startPos)
{
	Oyster::Math::Float3 dir = Oyster::Math::Float3(0,0,-1);
	Oyster::Math::Float3 up =Oyster::Math::Float3(0,1,0);
	Oyster::Math::Float3 pos = Oyster::Math::Float3(0, 0, 20);

	camera->LookAt(pos, dir, up);
	camera->SetLens(3.14f/2, 1024/768, 1, 1000);

	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/4,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);
	camera->UpdateViewMatrix();
	privData->view = camera->View();
	privData->view = Oyster::Math3D::ViewMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
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
			readKeyInput(KeyInput);
			camera->UpdateViewMatrix();

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
	Oyster::Graphics::API::SetView(camera->View());
	//Oyster::Graphics::API::SetProjection(camera->Proj());
	//Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection(privData->proj);
	Oyster::Graphics::API::NewFrame();
	for (unsigned int i = 0; i < privData->object.size(); i++)
	{
		privData->object[i]->Render();
	}
	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	for (unsigned int i = 0; i < privData->object.size(); i++)
	{
		privData->object[i]->Release();
		delete privData->object[i];
		privData->object[i] = NULL;
	}

	delete privData;  
	privData = NULL;
	return true;
}
void GameState::readKeyInput(InputClass* KeyInput)
{

	bool send = false;
	GameLogic::Protocol_PlayerMovement movePlayer;
	movePlayer.bForward = false;
	movePlayer.bBackward = false;
	movePlayer.bLeft = false;
	movePlayer.bRight = false;

	if(KeyInput->IsKeyPressed(DIK_W))
	{

		if(!key_forward)
		{
			movePlayer.bForward = true;
			send = true;
			key_forward = true;
		}
	}
	else
		key_forward = false;

	if(KeyInput->IsKeyPressed(DIK_S))
	{
		if(!key_backward)
		{
			movePlayer.bBackward = true;
			send = true;
			key_backward = true;
		}
	}
	else 
		key_backward = false;

	if(KeyInput->IsKeyPressed(DIK_A))
	{
		if(!key_strafeLeft)
		{
			movePlayer.bLeft = true;
			send = true;
			key_strafeLeft = true;
		}
	}
	else 
		key_strafeLeft = false;

	if(KeyInput->IsKeyPressed(DIK_D))
	{
		if(!key_strafeRight)
		{
			movePlayer.bRight = true;
			send = true;
			key_strafeRight = true;
		}
	} 
	else 
		key_strafeRight = false;


	if (privData->nwClient->IsConnected() && send)
	{
		privData->nwClient->Send(movePlayer);
	}

	//send delta mouse movement 
	if (KeyInput->IsMousePressed())
	{
		camera->Yaw(KeyInput->GetYaw());
		camera->Pitch(KeyInput->GetPitch());
		camera->UpdateViewMatrix();
		GameLogic::Protocol_PlayerLook playerLookDir;
		Oyster::Math::Float3 look = camera->GetLook();
		playerLookDir.lookDirX = look.x;
		playerLookDir.lookDirY = look.y;
		playerLookDir.lookDirZ = look.z;
		privData->nwClient->Send(playerLookDir);
	}

	// shoot
	if(KeyInput->IsKeyPressed(DIK_Z))
	{
		if(!key_Shoot)
		{
			GameLogic::Protocol_PlayerShot playerShot;
			playerShot.hasShot = true;
			privData->nwClient->Send(playerShot);
			key_Shoot = true;
		}
	} 
	else 
		key_Shoot = false;

	// jump
	if(KeyInput->IsKeyPressed(DIK_X))
	{
		if(!key_Jump)
		{
			GameLogic::Protocol_PlayerJump playerJump;
			playerJump.hasJumped = true;
			privData->nwClient->Send(playerJump);
			key_Jump = true;
		}
	}
	else 
		key_Jump = false;

	// send event data
	//  
	if(KeyInput->IsKeyPressed(DIK_L))
		privData->state = GameState::gameStateState_end;
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

	for (unsigned int i = 0; i < privData->object.size(); i++)
	{
		if(privData->object[i]->GetId() == pos->object_ID)
		{
			privData->object[i]->setPos(world);
			//camera->setRight((Oyster::Math::Float3(world[0], world[1], world[2])));
			//camera->setUp((Oyster::Math::Float3(world[4], world[5], world[6])));
			//camera->setLook((Oyster::Math::Float3(world[8], world[9], world[10])));
			if(i == 2) // playerobj
			{
				camera->SetPosition(Oyster::Math::Float3(world[12], world[13], world[14]));
				camera->UpdateViewMatrix();
			}
		}
	}
}

void GameState::Protocol( NewObj* newObj )
{

	Oyster::Math::Float4x4 world;
	for(int i = 0; i<16; i++)
	{
		world[i] = newObj->worldPos[i];
	}
	ModelInitData modelData;

	modelData.world = world;
	modelData.visible = true;
	modelData.id = newObj->object_ID;
	//not sure if this is good parsing rom char* to wstring
	const char* path = newObj->path;
	modelData.modelPath = std::wstring(path, path + strlen(path));  
	// load models
	C_Object* player = new C_Player();
	player->Init(modelData);

	privData->object.push_back(player);

}

void DanBias::Client::GameState::Protocol( RemoveObj* obj )
{
	for (unsigned int i = 0; i < privData->object.size(); i++)
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