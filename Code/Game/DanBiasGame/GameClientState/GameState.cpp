﻿#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera.h"
#include <GameServerAPI.h>
#include "LevelLoader\ObjectDefines.h"

using namespace GameLogic;
using namespace DanBias::Client;
using namespace Oyster::Math;
struct  GameState::myData
{
	myData(){}
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
	timer = 0;
}


GameState::~GameState(void)
{
	delete this->camera;
	delete this->privData;
}
bool GameState::Init(Oyster::Network::NetworkClient* nwClient)
{
	// load models
	camera = new Camera;
	privData = new myData();
	privData->state = gameStateState_loading;
	privData->nwClient = nwClient;	
	privData->state = LoadGame();
	pitch = 0;

	//tELL SERver ready
	nwClient->Send(GameLogic::Protocol_General_Status(GameLogic::Protocol_General_Status::States_ready));

	return true;
}
GameState::gameStateState GameState::LoadGame() 
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Float3(615, 0 ,5);
	plight.Color = Float3(0.9f,0.7f,0.2f);
	plight.Radius = 100;
	plight.Bright = 0.5f;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Float3(10,800,5);
	plight.Color = Float3(0.9f,0.7f,0.3f);
	plight.Radius = 300;
	plight.Bright = 0.5f;
	Oyster::Graphics::API::AddLight(plight);

	// use level loader
	LoadModels("../Content/Worlds/ccc.bias");

	// hardcoded objects
	Float3 startPos = Float3(0,0,20.0f);
	InitCamera(startPos);
	return gameStateState_playing;
}
bool GameState::LoadModels()
{
	// open file
	// read file 
	// init models
	int nrOfBoxex = 5;
	int id = 100; 

	// add world model
	ModelInitData modelData;

	modelData.position = Oyster::Math::Float3(0,0,0);
	modelData.rotation = Oyster::Math::Quaternion::identity;
	modelData.scale =  Oyster::Math::Float3(2,2,2);

	modelData.modelPath = L"world_earth.dan";
	modelData.id = id++;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	
	// add box model
	modelData.position = Oyster::Math::Float3(0,0,0);
	modelData.rotation = Oyster::Math::Quaternion::identity;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = L"crate_colonists.dan";


	for(int i =0; i< nrOfBoxex; i ++)
	{
		modelData.position = Oyster::Math::Float3(4,320,0);
		modelData.id = id++;

		this->dynamicObjects.Push(new C_DynamicObj());
		this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);
	}

	// add crystal model 
	modelData.position = Oyster::Math::Float3(10, 601, 0);
	modelData.modelPath = L"crystalformation_b.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add house model 
	modelData.position = Oyster::Math::Float3(-50, 590, 0);
	//Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(Oyster::Math::Float3(0 ,Utility::Value::Radian(90.0f), 0));

	modelData.visible = true;
	modelData.modelPath = L"building_corporation.dan";
	modelData.id = id++;
	// load models
	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	// add jumppad
	modelData.position = Oyster::Math::Float3(4, 600.3f, 0);
	modelData.modelPath = L"jumppad_round.dan";
	modelData.id = id++;
	// load models
	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	// add player model
	modelData.position = Oyster::Math::Float3(0, 602, 0);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add player model 2
	modelData.position = Oyster::Math::Float3(50, 602, 0);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	

	// add sky sphere
	modelData.position = Oyster::Math::Float3(0,0,0);
	modelData.scale =  Oyster::Math::Float3(800,800,800);
	modelData.modelPath = L"skysphere.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);
	return true;
}
bool GameState::LoadModels(std::string mapFile)
{
	GameLogic::LevelLoader levelLoader;
	std::vector<Utility::DynamicMemory::SmartPointer<GameLogic::ObjectTypeHeader>> objects; 
	objects = levelLoader.LoadLevel(mapFile);

	int objCount = objects.size();
	int modelId = 100;
	ModelInitData modelData;
	for (int i = 0; i < objCount; i++)
	{
		GameLogic::ObjectTypeHeader* obj = objects.at(i);

		switch (obj->typeID)
		{
		case GameLogic::ObjectType::ObjectType_LevelMetaData:

			break;
		case GameLogic::ObjectType::ObjectType_Static:
			{
				GameLogic::ObjectHeader* staticObjData = ((GameLogic::ObjectHeader*)obj);
				if((ObjectSpecialType)staticObjData->specialTypeID == ObjectSpecialType_Sky)
				{
					int i = 0; 
				}
				modelData.modelPath.assign(staticObjData->ModelFile.begin(), staticObjData->ModelFile.end());
				modelData.visible = true;
				modelData.position = staticObjData->position;
				modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(staticObjData->rotation), staticObjData->rotation[3]);
				modelData.scale =  staticObjData->scale;
				modelData.id = modelId++;

				this->staticObjects.Push(new C_StaticObj());
				this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);
			}
		break;
		case GameLogic::ObjectType::ObjectType_Dynamic:
			{
				GameLogic::ObjectHeader* dynamicObjData = ((GameLogic::ObjectHeader*)obj);
				modelData.modelPath.assign(dynamicObjData->ModelFile.begin(), dynamicObjData->ModelFile.end());
				modelData.visible = true;
				modelData.position = dynamicObjData->position;
				modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(dynamicObjData->rotation), dynamicObjData->rotation[3]);
				modelData.scale = dynamicObjData->scale;
				modelData.id = modelId++;

				this->dynamicObjects.Push(new C_DynamicObj());
				this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);
			}
			break;
		case  GameLogic::ObjectType::ObjectType_Light:
			{
				GameLogic::BasicLight* lightData = ((GameLogic::BasicLight*)obj);

				if(lightData->lightType == GameLogic::LightType_PointLight)
				{
					Oyster::Graphics::Definitions::Pointlight plight;
					plight.Pos = ((GameLogic::BasicLight*)lightData)->position;
					plight.Color = lightData->color;
					plight.Radius = 100;
					plight.Bright = 0.9f;
					Oyster::Graphics::API::AddLight(plight);
				}
			}
			break;
		default:
			break;
		}
	}
	
	Oyster::Math::Quaternion first = Oyster::Math::Quaternion(Float3(0.3536,0.3536,-0.146), 0.8536);
	Oyster::Math::Quaternion second = Oyster::Math::Quaternion(Float3(0.3536,0.3536,-0.146), 0.8536);
	Oyster::Math::Quaternion result = first * second;
	Oyster::Math::Quaternion total = Oyster::Math::Quaternion(Float3(0.5,0.5,-0.5), 0.5);
	modelData.visible = true;
	modelData.position = Oyster::Math::Float3(20, 127,0);
	modelData.rotation = first;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = myId;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	modelData.visible = true;
	modelData.position = Oyster::Math::Float3(22, 127,0);
	modelData.rotation = second;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = myId;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	modelData.visible = true;
	modelData.position = Oyster::Math::Float3(24, 127,0);
	modelData.rotation = result;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = myId;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	modelData.visible = true;
	modelData.position = Oyster::Math::Float3(26, 127,0);
	modelData.rotation = total;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = myId;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	return true;
	
}
bool GameState::InitCamera(Float3 startPos)
{
	Float3 dir = Float3(0,0,1);
	Float3 up = Float3(0,1,0);
	Float3 pos = Float3(0, 0, 20);
	camera->LookAt(pos, dir, up);
	camera->SetLens(pi/4, 1024/768, 1, 1000);
	camera->UpdateViewMatrix();
	Oyster::Graphics::API::SetProjection(camera->Proj());
	
	return true;
}
void GameState::InitiatePlayer(int id, std::wstring modelName, Oyster::Math::Float4x4 world)
{
	myId = id;

	ModelInitData modelData;
	C_Object* obj;
	modelData.visible = true;
	//modelData.world = world;
	modelData.position = Oyster::Math::Float3(world[12], world[13], world[14]);
	modelData.rotation = Oyster::Math::Quaternion(Oyster::Math::Float3(0,0,0), 1);
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.modelPath = modelName;
	modelData.id = myId;
	
	obj =  new C_Player();
	this->dynamicObjects.Push(obj);
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);
	
	
	Oyster::Math::Float3 right = Oyster::Math::Float3(world[0], world[1], world[2]);
	Oyster::Math::Float3 up = Oyster::Math::Float3(world[4], world[5], world[6]);
	Oyster::Math::Float3 objForward = (Oyster::Math::Float3(world[8], world[9], world[10]));
	Oyster::Math::Float3 pos = Oyster::Math::Float3(world[12], world[13], world[14]);

	Oyster::Math::Float3 cameraLook = camera->GetLook();
	Oyster::Math::Float3 cameraUp = camera->GetUp();
				
			

	/*Oyster::Math::Float3 newUp = cameraUp.Dot(up);
	up *= newUp;
	up.Normalize();
	Oyster::Math::Float3 newLook = up.Cross(right);
	newLook.Normalize();*/

	camera->setRight(right);
	camera->setUp(up);
	camera->setLook(objForward);
				
	up *= 2;
	objForward *= 3;
	Oyster::Math::Float3 cameraPos = up + pos + objForward;
	camera->SetPosition(cameraPos);

	camera->UpdateViewMatrix();
	
}
GameClientState::ClientState GameState::Update(float deltaTime, InputClass* KeyInput)
{
	switch (privData->state)
	{
	case gameStateState_loading:	//Will this ever happen in this scope??
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
			timer += deltaTime;
			if(timer > 0.03)
			{
				readKeyInput(KeyInput);
				timer = 0;
			}
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
bool GameState::Render(float dt)
{
	Oyster::Graphics::API::SetView(camera->View());

	Oyster::Graphics::API::NewFrame();
	for (unsigned int i = 0; i < staticObjects.Size(); i++)
	{
		staticObjects[i]->Render();
	}
	for (unsigned int i = 0; i < dynamicObjects.Size(); i++)
	{
		dynamicObjects[i]->Render();
	}
	Oyster::Graphics::API::StartTextRender();
	std::wstring fps;
	float f = 1/dt;
	fps = std::to_wstring(f);
	//Oyster::Graphics::API::RenderText(fps,Oyster::Math::Float2(0.5f,0.1f),Oyster::Math::Float2(0.2f,0.05f));

	Oyster::Graphics::API::EndFrame();
	return true;
}
bool GameState::Release()
{
	/*for (unsigned int i = 0; i < privData->object.size(); i++)
	{
	privData->object[i]->Release();
	delete privData->object[i];
	privData->object[i] = NULL;
	}*/

	delete privData;  
	privData = NULL;
	return true;
}
void GameState::readKeyInput(InputClass* KeyInput)
{

	if(KeyInput->IsKeyPressed(DIK_W))
	{
		//if(!key_forward)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementForward());
			key_forward = true;
		}
	}
	else
		key_forward = false;

	if(KeyInput->IsKeyPressed(DIK_S))
	{
		//if(!key_backward)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementBackward());
			key_backward = true;
		}
	}
	else 
		key_backward = false;

	if(KeyInput->IsKeyPressed(DIK_A))
	{
		//if(!key_strafeLeft)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementLeft());
			key_strafeLeft = true;
		}
	}
	else 
		key_strafeLeft = false;

	if(KeyInput->IsKeyPressed(DIK_D))
	{
		//if(!key_strafeRight)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementRight());
			key_strafeRight = true;
		}
	} 
	else 
		key_strafeRight = false;


	//send delta mouse movement 
	if (KeyInput->IsMousePressed())
	{
		camera->Yaw(-KeyInput->GetYaw());
		camera->Pitch(KeyInput->GetPitch());
		pitch = KeyInput->GetPitch();
		camera->UpdateViewMatrix();		
		Oyster::Math::Float3 look = camera->GetLook();
		Oyster::Math::Float3 right = camera->GetRight();
		GameLogic::Protocol_PlayerLook playerLook(look, right);

		privData->nwClient->Send(playerLook);
	}

	// shoot
	if(KeyInput->IsKeyPressed(DIK_Z))
	{
		if(!key_Shoot)
		{
			GameLogic::Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = true;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = false;
			privData->nwClient->Send(playerShot);
			key_Shoot = true;
		}
	} 
	else 
		key_Shoot = false;

	if(KeyInput->IsKeyPressed(DIK_X))
	{
		if(!key_Shoot)
		{
			GameLogic::Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = true;
			playerShot.utilityPressed = false;
			privData->nwClient->Send(playerShot);
			key_Shoot = true;
		}
	} 
	else 
		key_Shoot = false;

	if(KeyInput->IsKeyPressed(DIK_C))
	{
		if(!key_Shoot)
		{
			GameLogic::Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = true;
			privData->nwClient->Send(playerShot);
			key_Shoot = true;
		}
	} 
	else 
		key_Shoot = false;

	// jump
	if(KeyInput->IsKeyPressed(DIK_SPACE))
	{
		if(!key_Jump)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerJump());
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
	//Oyster::Math::Float4x4 world, translate;

	//world = Oyster::Math::Float4x4::identity;
	//translate = Oyster::Math::Float4x4::identity;
	//translate = Oyster::Math3D::TranslationMatrix(Oyster::Math::Float3(pos->playerPos[0],pos->playerPos[1],pos->playerPos[2]));
	//world = world * translate;
	////privData->object[0]->setPos( world );
	//for (unsigned int i = 0; i < dynamicObjects.Size(); i++)
	//{
	//	dynamicObjects[i]->Render();
	//}
}

void GameState::Protocol( ObjPos* pos )
{
	Oyster::Math::Float4x4 world;
	for(int i = 0; i<16; i++)
	{
		world[i] = pos->worldPos[i];
	}

	for (unsigned int i = 0; i < dynamicObjects.Size(); i++)
	{
		if(dynamicObjects[i]->GetId() == pos->object_ID)
		{
			dynamicObjects[i]->setPos(Float3(world[0], world[1], world[2]));
			dynamicObjects[i]->setRot(Quaternion(Float3(world[3], world[4], world[5]), world[6]));
			//dynamicObjects[i]->setWorld(world);

			if(dynamicObjects[i]->GetId() == myId) // playerobj
			{


				Float3 pos = dynamicObjects[i]->getPos();
				Float3 up = dynamicObjects[i]->getWorld().v[1];
				Float3 objForward = dynamicObjects[i]->getWorld().v[2];
				up *= 3;
				objForward *= -4;
				Oyster::Math::Float3 cameraPos = pos + up + objForward;
				camera->SetPosition(cameraPos);	
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

	//modelData.world = world;
	modelData.visible = true;
	modelData.id = newObj->object_ID;
	//not sure if this is good parsing rom char* to wstring
	const char* path = newObj->path;
	modelData.modelPath = std::wstring(path, path + strlen(path));  
	// load models
	C_DynamicObj* player = new C_DynamicObj();
	player->Init(modelData);

	dynamicObjects.Push(player);

}

void DanBias::Client::GameState::Protocol( RemoveObj* obj )
{
	for (unsigned int i = 0; i < dynamicObjects.Size(); i++)
	{
		if(dynamicObjects[i]->GetId() == obj->object_ID)
		{
			//dynamicObjects[i]->Release();
			dynamicObjects[i].Release();
			//dynamicObjects.erase(privData->object.begin() + i );
		}
	}
	//privData->object[obj->object_ID]->Release( );
}

//void GameState::Protocol(LightPos pos);