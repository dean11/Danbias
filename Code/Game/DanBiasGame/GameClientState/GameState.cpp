#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera_FPS.h"
#include <GameServerAPI.h>

using namespace DanBias::Client;
using namespace Oyster::Math3D;

struct  GameState::myData
{
	myData(){}
	int modelCount;
	Oyster::Network::NetworkClient* nwClient;
	gameStateState state;
} privData;

GameState::GameState(void)
{
	key_forward = false;
	key_backward = false;
	key_strafeRight = false;
	key_strafeLeft = false;
}

GameState::~GameState(void)
{
	delete this->privData;
}
bool GameState::Init(Oyster::Network::NetworkClient* nwClient)
{
	// load models
	privData = new myData();
	privData->state = gameStateState_loading;
	privData->nwClient = nwClient;	
	privData->state = LoadGame();

	//tELL SERver ready
	nwClient->Send(GameLogic::Protocol_General_Status(GameLogic::Protocol_General_Status::States_ready));

	return true;
}
GameState::gameStateState GameState::LoadGame() 
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Float3(315.0f, 0.0f ,5.0f);
	plight.Color = Float3(0.9f,0.7f,0.2f);
	plight.Radius = 100.0f;
	plight.Bright = 0.9f;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Float3(10.0f,350.0f,5.0f);
	plight.Color = Float3(0.9f,0.7f,0.3f);
	plight.Radius = 200.0f;
	plight.Bright = 0.7f;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Float3(350.0f,350.0f,5.0f);
	plight.Color = Float3(0.9f,0.7f,0.3f);
	plight.Radius = 200.0f;
	plight.Bright = 0.7f;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Float3(10.0f,350.0f,350.0f);
	plight.Color = Float3(0.9f,0.7f,0.3f);
	plight.Radius = 200.0f;
	plight.Bright = 0.7f;
	Oyster::Graphics::API::AddLight(plight);
	plight.Pos = Float3(10.0f,-15.0f,5.0f);
	plight.Color = Float3(0.0f,0.0f,1.0f);
	plight.Radius = 50.0f;
	plight.Bright = 2.0f;

	Oyster::Graphics::API::AddLight(plight);
	LoadModels();
	InitCamera(Float3(0.0f,0.0f,20.0f));
	// hardcoded objects
	LoadModels();
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

	modelData.position = Float3(0,0,0);
	modelData.rotation = Quaternion::identity;
	modelData.scale =  Float3(2,2,2);

	modelData.modelPath = L"world_earth.dan";
	modelData.id = id++;

	this->staticObjects.Push(new C_StaticObj());
	this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);

	/*
	// add box model
	modelData.position = Float3(0,0,0);
	modelData.rotation = Quaternion::identity;
	modelData.scale =  Float3(1,1,1);
	modelData.modelPath = L"crate_colonists.dan";


	for(int i =0; i< nrOfBoxex; i ++)
	{
		modelData.position = Float3(4,320,0);
		modelData.id = id++;

		this->dynamicObjects.Push(new C_DynamicObj());
		this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);
	}

	
	// add crystal model 
	modelData.position = Float3(10, 301, 0);
	modelData.modelPath = L"crystalformation_b.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add house model 
	modelData.position = Float3(-50, 290, 0);
	//Oyster::Math3D::Float4x4 rot = Oyster::Math3D::RotationMatrix(Float3(0 ,Utility::Value::Radian(90.0f), 0));

	modelData.visible = true;
	modelData.modelPath = L"building_corporation.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);


	// add player model
	modelData.position = Float3(0, 320, 0);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add player model 2
	modelData.position = Float3(50, 320, 0);
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add jumppad
	modelData.position = Float3(4, 300.3, 0);
	modelData.modelPath = L"jumppad_round.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	// add sky sphere
	modelData.position = Float3(0,0,0);
	modelData.scale =  Float3(800,800,800);
	modelData.modelPath = L"skysphere.dan";
	modelData.id = id++;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);*/
	return true;
}
bool GameState::LoadModels(std::string mapFile)
{
	GameLogic::LevelLoader levelLoader;
	std::vector<Utility::DynamicMemory::SmartPointer<GameLogic::ObjectTypeHeader>> objects; 
	objects = levelLoader.LoadLevel(mapFile);

	int objCount = objects.size();
	int modelId = 0;
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

				modelData.modelPath.assign(staticObjData->ModelFile.begin(), staticObjData->ModelFile.end());
				modelData.visible = true;
				//modelData.position = ;
				//modelData.rotation = Quaternion(Float3(2,2,-2), 1);
				//modelData.scale =  Float3(2,2,2);
				modelData.id = modelId++;

				this->staticObjects.Push(new C_StaticObj());
				this->staticObjects[this->staticObjects.Size() -1 ]->Init(modelData);
			}
		break;
		case GameLogic::ObjectType::ObjectType_Dynamic:
			{
				GameLogic::ObjectHeader* dynamicObjData = ((GameLogic::ObjectHeader*)obj);
				//modelData.position = ;
				//modelData.rotation = Quaternion(Float3(2,2,-2), 1);
				//modelData.scale =  Float3(2,2,2);
				modelData.modelPath.assign(dynamicObjData->ModelFile.begin(), dynamicObjData->ModelFile.end());
				modelData.visible = true;
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
					plight.Pos = ((GameLogic::PointLight*)lightData)->position;
					plight.Color = lightData->diffuseColor;
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
	myId += modelId++;
	// add player model
	//modelData.position = ;
	//modelData.rotation = Quaternion(Float3(2,2,-2), 1);
	//modelData.scale =  Float3(2,2,2);


	modelData.visible = true;
	modelData.modelPath = L"char_still_sizeref.dan";
	modelData.id = myId;
	// load models
	this->dynamicObjects.Push(new C_DynamicObj());
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	/*C_Player* obj =  new C_Player();
	privData->object.push_back(obj);
	privData->object[privData->object.size() -1 ]->Init(modelData);
	*/
	return true;
	
}
bool GameState::InitCamera(Float3 startPos)
{
	camera.SetHeadOffset( Float3(0.0f, 1.0f, 1.0f) );
	camera.SetPerspectiveProjection( pi / 4.0f, 1024.0f/768.0f, 1.0f, 1000.0f );
	camera.UpdateOrientation();
	Oyster::Graphics::API::SetProjection(camera.GetProjectionMatrix());
	
	return true;
}
void GameState::InitiatePlayer(int id, std::wstring modelName, Float4x4 world)
{
	myId = id;

	ModelInitData modelData;
	C_Object* obj;
	modelData.visible = true;
	//modelData.world = world;
	modelData.position = Float3(world[12], world[13], world[14]);
	modelData.rotation = Quaternion(Float3(0,0,0), 1);
	modelData.scale =  Float3(1,1,1);
	modelData.modelPath = modelName;
	modelData.id = myId;
	
	obj = new C_Player();
	this->dynamicObjects.Push(obj);
	this->dynamicObjects[this->dynamicObjects.Size() -1 ]->Init(modelData);

	Float3 pos = Float3(world[12], world[13], world[14]);

	camera.SetPosition( pos );
	camera.UpdateOrientation();
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
			readKeyInput(KeyInput);
			camera.UpdateOrientation();
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
	Oyster::Graphics::API::SetView( camera.GetViewMatrix() );

	Oyster::Graphics::API::NewFrame();
	for (unsigned int i = 0; i < staticObjects.Size(); i++)
	{
		staticObjects[i]->Render();
	}
	for (unsigned int i = 0; i < dynamicObjects.Size(); i++)
	{
		dynamicObjects[i]->Render();
	}

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
		if(!key_forward)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementForward());
			key_forward = true;
		}
	}
	else
		key_forward = false;

	if(KeyInput->IsKeyPressed(DIK_S))
	{
		if(!key_backward)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementBackward());
			key_backward = true;
		}
	}
	else 
		key_backward = false;

	if(KeyInput->IsKeyPressed(DIK_A))
	{
		if(!key_strafeLeft)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementLeft());
			key_strafeLeft = true;
		}
	}
	else 
		key_strafeLeft = false;

	if(KeyInput->IsKeyPressed(DIK_D))
	{
		if(!key_strafeRight)
		{
			privData->nwClient->Send(GameLogic::Protocol_PlayerMovementRight());
			key_strafeRight = true;
		}
	} 
	else 
		key_strafeRight = false;


	//send delta mouse movement 
	//if (KeyInput->IsMousePressed())
	{
		camera.YawRight( -KeyInput->GetYaw() );
		camera.PitchUp( KeyInput->GetPitch() );
		camera.UpdateOrientation();

		GameLogic::Protocol_PlayerLook playerLookDir;
		Float4 look = camera.GetLook();

		privData->nwClient->Send( playerLookDir );
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

using namespace ::Oyster::Network;
using namespace ::Utility::DynamicMemory;

// returns -1 if none found
int FindObject( const DynamicArray<SmartPointer<C_Object>> &collection, int id )
{
	int num = collection.Size();
	for( int i = 0; i < num; ++i ) if( id == collection[i]->GetId() )
		return i;
	return -1;
}

void GameState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	CustomNetProtocol data = e.args.data.protocol;
	short ID = data[0].value.netShort; // fetching the id data.
	
	if( ProtocolIsGameplay(ID) )
	{
		switch(ID)
		{
		case protocol_Gameplay_ObjectPickup:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectDamage:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectHealthStatus:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectPosition:
			{
				GameLogic::Protocol_ObjectPosition decoded(data);

				// if is this player. Remember to change camera
				if( this->myId == decoded.object_ID )
					camera.SetPosition( decoded.position );

				int i = FindObject( this->dynamicObjects, decoded.object_ID );
				if( i > -1 )
					this->dynamicObjects[i]->setPos( decoded.position );
			}
			break;
		case protocol_Gameplay_ObjectScale:
			{
				GameLogic::Protocol_ObjectScale decoded(data);
				int i = FindObject( this->dynamicObjects, decoded.object_ID );
				if( i > -1 )
					this->dynamicObjects[i]->setScale( decoded.scale );
			}
			break;
		case protocol_Gameplay_ObjectRotation:
			{
				GameLogic::Protocol_ObjectRotation decoded(data);
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );

				// if is this player. Remember to change camera
				if( this->myId == decoded.object_ID )
					camera.SetAngular( QuaternionToAngularAxis(rotation).xyz );

				int i = FindObject( this->dynamicObjects, decoded.object_ID );
				if( i > -1 )
					this->dynamicObjects[i]->setRot( rotation );
			}
			break;
		case protocol_Gameplay_ObjectPositionRotation:
			{
				GameLogic::Protocol_ObjectPositionRotation decoded(data);
				Float3 position = decoded.position;
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );

				// if is this player. Remember to change camera
				if( this->myId == decoded.object_ID )
				{
					camera.SetPosition( position );
					camera.SetAngular( QuaternionToAngularAxis(rotation).xyz );
				}

				int i = FindObject( this->dynamicObjects, decoded.object_ID );
				if( i > -1 )
				{
					this->dynamicObjects[i]->setPos( position );
					this->dynamicObjects[i]->setRot( rotation );
				}
			}
			break;
		case protocol_Gameplay_ObjectEnabled:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectDisabled:
			{
				GameLogic::Protocol_ObjectDisable decoded(data);

				int i = FindObject( this->dynamicObjects, decoded.objectID );
				if( i > -1 )
				{
					this->dynamicObjects[i].Release();
					this->dynamicObjects.Pop(i);
				}
			}
			break;
		case protocol_Gameplay_ObjectCreate:
			{
				GameLogic::Protocol_ObjectCreate decoded(data);
				C_DynamicObj* object = new C_DynamicObj();

				ModelInitData modelData;
				{
					modelData.position = Float3( decoded.position );
					modelData.rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );
					modelData.scale = Float3( decoded.scale );
					modelData.visible = true;
					modelData.id = decoded.object_ID;

					::Utility::String::StringToWstring( decoded.name, modelData.modelPath );
				}
				object->Init(modelData);

				dynamicObjects.Push(object);

			}		
			break;
		case protocol_Gameplay_ObjectCreatePlayer:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectJoinTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectLeaveTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponCooldown:	break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponEnergy:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectRespawn:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectDie:				break; /** @todo TODO: implement */
		default: break;
		}
	}
	else if( ProtocolIsGeneral(ID) )
	{
		switch( ID )
		{
			case protocol_General_Status:				break; /** @todo TODO: implement */
			case protocol_General_Text:					break; /** @todo TODO: implement */
		default: break;
		}
	}
}
