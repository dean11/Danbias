#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera_FPSV2.h"
#include <GameServerAPI.h>
#include "C_Light.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include "C_obj/C_StaticObj.h"
#include "Utilities.h"
#include "GamingUI.h"
#include "RespawnUI.h"
#include "StatsUI.h"
#include <ObjectDefines.h>

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Math3D;
using namespace ::GameLogic;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::String;
using namespace ::Utility::Value;

struct  GameState::MyData
{
	MyData(){}
	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	InputClass *input;

	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *staticObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_DynamicObj>> *dynamicObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>> *lights;

	//C_Player player;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Player>> players;
	Camera_FPSV2 camera;

	int myId;

} privData;

inline Quaternion ArrayToQuaternion( const float source[4] )
{
	return Quaternion( Float3(source[0], source[1], source[2]), source[3] );
}

GameState::GameState()
{
	this->privData = nullptr;
}

GameState::~GameState()
{
	if( this->privData )
		this->Release();
}

bool GameState::Init( SharedStateContent &shared )
{
	// we may assume that shared.network is properly connected
	// and there is content in shared.dynamicObjects and shared.staticObjects

	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = shared.network;
	this->privData->input = shared.input;
	this->privData->staticObjects = &shared.staticObjects;
	this->privData->dynamicObjects = &shared.dynamicObjects;
	this->privData->lights = &shared.lights;

	Graphics::API::Option gfxOp = Graphics::API::GetOption();
	Float aspectRatio = gfxOp.Resolution.x / gfxOp.Resolution.y;
	this->privData->camera.SetPerspectiveProjection( Utility::Value::Radian(90.0f), aspectRatio, 0.1f, 1000.0f );
	Graphics::API::SetProjection( this->privData->camera.GetProjectionMatrix() );
	gfxOp.AmbientValue = 0.5f;
	gfxOp.GlobalGlowTint = Math::Float3(1,1,1);
	gfxOp.GlobalTint = Math::Float3(1,1,1);
	Graphics::API::SetOptions(gfxOp);

	//tell server ready
	this->privData->nwClient->Send( Protocol_General_Status(Protocol_General_Status::States_ready) );
			
	// DEGUG KEYS
	this->key_Reload_Shaders = false;
	this->key_Wireframe_Toggle = false;
	this->renderWhireframe = false;
	// !DEGUG KEYS

	auto light = this->privData->lights->begin();
	for( ; light != this->privData->lights->end(); ++light )
	{
		light->second->Render();
	}

	// create UI states
	this->gameUI = new GamingUI(this->privData->input, this->privData->nwClient, &this->privData->camera);
	this->respawnUI = new RespawnUI(this->privData->nwClient, 20);
	this->statsUI = new StatsUI();
	this->currGameUI = gameUI; 
	((GamingUI*)gameUI)->Init();
	((RespawnUI*)respawnUI)->Init();
	((StatsUI*)statsUI)->Init();

	return true;
}

void GameState::InitiatePlayer( int id, const std::string &modelName, const float position[3], const float rotation[4], const float scale[3], bool isMyPlayer )
{
	ModelInitData modelData;
	modelData.visible	= true;
	modelData.position	= position;
	modelData.rotation	= ArrayToQuaternion( rotation );
	modelData.scale		= scale;
	modelData.id		= id;
	StringToWstring(modelName,modelData.modelPath);

	// RB DEBUG
	RBInitData RBData;
	RBData.position = position;
	RBData.rotation = ArrayToQuaternion( rotation );
	RBData.scale =  scale;
	RBData.type = RB_Type_Cube;
	// !RB DEBUG 
	C_Player *p = new C_Player();
	if( p->Init(modelData) )
	{
		// RB DEBUG
		p->InitRB( RBData );
		// !RB DEBUG 
		// start with runing animation
		p->playAnimation( L"idle", true );

		(this->privData->players)[id] = p;
		
		if( isMyPlayer )
		{
			this->privData->myId = id;
			this->privData->camera.SetPosition( p->getPos() );
			Float3 offset = Float3( 0.0f );
			// DEBUG position of camera so we can see the player model
			//offset.y = p->getScale().y * 5.0f;
			//offset.z = p->getScale().z * -5.0f;
			// !DEBUG
			this->privData->camera.SetHeadOffset( offset );
			this->privData->camera.UpdateOrientation();
		}
	}
	else
	{
		int i = 0; 
		// some error loading model 
	}
}

GameClientState::ClientState GameState::Update( float deltaTime )
{
	GameStateUI::UIState UIstate = this->gameUI->Update( deltaTime );
	switch (UIstate)
	{
	case DanBias::Client::GameStateUI::UIState_shut_down:
		{
			this->privData->nextState = ClientState_Quit;
			// disconnect 
		}
	
		break;
	case DanBias::Client::GameStateUI::UIState_same:
		break;
	case DanBias::Client::GameStateUI::UIState_gaming:
		break;
	case DanBias::Client::GameStateUI::UIState_main_menu:
		{
			this->privData->nextState = ClientState_Main;
			// disconnect 
		}
		
		break;
	
	default:
		break;
	} 
	// DEBUG keybindings
	ReadKeyInput();

	return this->privData->nextState;
}

bool GameState::Render()
{
	Oyster::Graphics::API::SetView( this->privData->camera.GetViewMatrix() );

	Oyster::Graphics::API::NewFrame();

	// for debugging to be replaced with render weapon
	auto playerObject = this->privData->players.begin();
	for( ; playerObject != this->privData->players.end(); ++playerObject )
	{
		if(playerObject->second)
		{
			if( this->privData->myId != playerObject->second->GetId() )
			{
				playerObject->second->Render();
			}
		}
	}

	auto staticObject = this->privData->staticObjects->begin();
	for( ; staticObject != this->privData->staticObjects->end(); ++staticObject )
	{
		staticObject->second->Render();
	}

	auto dynamicObject = this->privData->dynamicObjects->begin();
	for( ; dynamicObject != this->privData->dynamicObjects->end(); ++dynamicObject )
	{
		if( dynamicObject->second )
		{
			dynamicObject->second->Render();

		}
	}

#ifdef _DEBUG
	//RB DEBUG render wire frame 
		if(this->renderWhireframe)
		{
			Oyster::Graphics::API::StartRenderWireFrame();

			playerObject = this->privData->players.begin();
			for( ; playerObject != this->privData->players.end(); ++playerObject )
			{
				if(playerObject->second)
				{
					if( playerObject->second->getBRtype() == RB_Type_Cube)
					{
						Oyster::Graphics::API::RenderDebugCube( playerObject->second->getRBWorld());
					}
					if( playerObject->second->getBRtype() == RB_Type_Sphere)
					{
						Oyster::Graphics::API::RenderDebugSphere( playerObject->second->getRBWorld());
					}
				}
			}

			staticObject = this->privData->staticObjects->begin();
			for( ; staticObject != this->privData->staticObjects->end(); ++staticObject )
			{
				if( staticObject->second->getBRtype() == RB_Type_Cube)
				{
					Oyster::Graphics::API::RenderDebugCube( staticObject->second->getRBWorld());
				}
				if( staticObject->second->getBRtype() == RB_Type_Sphere)
				{
					Oyster::Graphics::API::RenderDebugSphere( staticObject->second->getRBWorld());
				}
			}

			dynamicObject = this->privData->dynamicObjects->begin();
			for( ; dynamicObject != this->privData->dynamicObjects->end(); ++dynamicObject )
			{
				if( dynamicObject->second )
				{
					if( dynamicObject->second->getBRtype() == RB_Type_Cube)
					{
						Oyster::Graphics::API::RenderDebugCube( dynamicObject->second->getRBWorld());
					}
					if( dynamicObject->second->getBRtype() == RB_Type_Sphere)
					{
						Oyster::Graphics::API::RenderDebugSphere( dynamicObject->second->getRBWorld());
					}
				}
			}
		}
		//!RB DEBUG 
#endif

	Oyster::Graphics::API::StartGuiRender();
	// render gui elemnts
	if(currGameUI->HaveGUIRender())
		currGameUI->RenderGUI();
	if(renderStats)
	{	
		if(statsUI->HaveGUIRender())
			statsUI->RenderGUI();
	}
	Oyster::Graphics::API::StartTextRender();
	if(currGameUI->HaveTextRender())
		currGameUI->RenderText();
	if(renderStats)
	{	
		if(statsUI->HaveTextRender())
			statsUI->RenderText();
	}

	Oyster::Graphics::API::EndFrame();
	return true;
}

bool GameState::Release()
{
	Graphics::API::Option o = Graphics::API::GetOption();
	if( privData )
	{
		auto playerObject = this->privData->players.begin();
		for( ; playerObject != this->privData->players.end(); ++playerObject )
		{
			playerObject->second = nullptr;
		}

		auto staticObject = this->privData->staticObjects->begin();
		for( ; staticObject != this->privData->staticObjects->end(); ++staticObject )
		{
			staticObject->second = nullptr;
		}

		auto dynamicObject = this->privData->dynamicObjects->begin();
		for( ; dynamicObject != this->privData->dynamicObjects->end(); ++dynamicObject )
		{
			dynamicObject->second = nullptr;
		}

		auto light = this->privData->lights->begin();
		for( ; light != this->privData->lights->end(); ++light )
		{
			light->second->Render();
		}

		this->privData->staticObjects->clear();
		this->privData->dynamicObjects->clear();
		this->privData->lights->clear();

		privData = NULL;
	}

	if(respawnUI) 
	{
		respawnUI->Release();
		delete respawnUI;
		respawnUI = NULL;
	}
	if(gameUI)
	{
		gameUI->Release();
		delete gameUI;
		gameUI = NULL;
	}
	if(statsUI)
	{
		statsUI->Release();
		delete statsUI;
		statsUI = NULL;
	}
	currGameUI = NULL;

	return true;
}

void GameState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}
void GameState::ReadKeyInput()
{
	// DEGUG KEYS

	// Reload shaders
	if( this->privData->input->IsKeyPressed(DIK_R) )
	{
		if( !this->key_Reload_Shaders )
		{
#ifdef _DEBUG
			Oyster::Graphics::API::ReloadShaders();
#endif
			this->key_Reload_Shaders = true;
		}
	} 
	else 
		this->key_Reload_Shaders = false;

	// toggle wire frame render
	if( this->privData->input->IsKeyPressed(DIK_T) )
	{
		if( !this->key_Wireframe_Toggle )
		{
			this->renderWhireframe = !this->renderWhireframe;
			this->key_Wireframe_Toggle = true;
			// DEBUG set you as dead when render wireframe
			this->currGameUI = respawnUI;
			// !DEBUG
		}
	} 
	else 
	{
		this->key_Wireframe_Toggle = false;
		// DEBUG set you as dead when render wireframe
		this->currGameUI = gameUI;
		// !DEBUG
	}

	// toggle wire frame render
	if( this->privData->input->IsKeyPressed(DIK_TAB) )
	{
		if( !this->key_showStats )
		{
			this->renderStats = true;
			this->key_showStats = true;
		}
	} 
	else 
	{
		this->renderStats = false;
		this->key_showStats = false;
	}
}

const GameClientState::NetEvent & GameState::DataRecieved( const GameClientState::NetEvent &message )
{
	if( message.args.type == NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend )
	{ // TODO: Reconnect
		const char *breakpoint = "temp trap";
		this->privData->nwClient->Disconnect();
		this->ChangeState( GameClientState::ClientState_Main );
	}

	// fetching the id data.
	short ID = message.args.data.protocol[0].value.netShort;

	if( ProtocolIsGameplay(ID) )
	{
		CustomNetProtocol data = message.args.data.protocol;

		switch(ID)
		{
		case protocol_Gameplay_ObjectPickup:	
			{
				Protocol_ObjectPickup decoded(data);
				decoded.object_ID;
				C_Object *object; 
				object = (this->privData->players)[decoded.object_ID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.object_ID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.object_ID )
					{
						 // I picked up the pickUp!
					}

					if (decoded.pickup_ID == GameLogic::PickupType::PickupType_Health)
					{
						// object->PickupHealth();
					}
					else if (decoded.pickup_ID == GameLogic::PickupType::PickupType_SpeedBoost)
					{
						// object->PickupSpeed();
					}
				}
				decoded.pickup_ID;
				
				
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDamage:			
			{
				Protocol_ObjectDamage decoded(data);
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.objectID )
					{
						// show that you took dmg
						if(currGameUI == gameUI)
						{
							// set given players HP 
							((GamingUI*)currGameUI)->SetHPtext(std::to_wstring(decoded.healthLost));
						}
					}
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectHealthStatus:		
			{ 
				// don't know if needed
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectPosition:
			{
				Protocol_ObjectPosition decoded(data);

				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.objectID )
					{
						this->privData->camera.SetPosition( decoded.position );
					}

					object->setPos( decoded.position );
					// RB DEBUG 
					object->setRBPos ( decoded.position );  
					// !RB DEBUG 
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectScale:
			{
				Protocol_ObjectScale decoded(data);
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					object->setScale( decoded.scale );
					// RB DEBUG 
					object->setRBScale ( decoded.scale );  
					// !RB DEBUG 
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectRotation:
			{
				Protocol_ObjectRotation decoded(data);
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.objectID )
					{
						this->privData->camera.SetRotation( rotation );
					}

					object->setRot( rotation );
					// RB DEBUG 
					object->setRBRot( rotation );  
					// !RB DEBUG 
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectPositionRotation:
			{
				Protocol_ObjectPositionRotation decoded(data);
				Float3 position = decoded.position;
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.objectID )
					{
						this->privData->camera.SetPosition( position );
						this->privData->camera.SetRotation( rotation );
					}
					object->setPos( position );
					object->setRot( rotation );
					object->updateWorld();
					// RB DEBUG 
					object->setRBPos ( position );  
					object->setRBRot ( rotation );  
					object->updateRBWorld();
					// !RB DEBUG 
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectEnabled:	
			{
				Protocol_ObjectEnable decoded(data);
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];

					if(!object)
					{
						//If it is a static object
						object = (*this->privData->staticObjects)[decoded.objectID];
					}
				}

				if( object )
				{
					 object->SetVisible(true);
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDisabled:
			{
				Protocol_ObjectDisable decoded(data);
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
					
					if(!object)
					{
						//If it is a static object
						object = (*this->privData->staticObjects)[decoded.objectID];
					}
				}

				if( object )
				{
					object->SetVisible(false);
				}

				/*auto object = this->privData->dynamicObjects->find( decoded.objectID );
				if( object != this->privData->dynamicObjects->end() )
				{
				object->second = nullptr;
				this->privData->dynamicObjects->erase( object );
				}*/

			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCreate:
			{
				Protocol_ObjectCreate decoded(data);
				C_DynamicObj* object = new C_DynamicObj();

				ModelInitData modelData;
				{
					modelData.position = Float3( decoded.position );
					modelData.rotation = Quaternion( Float3(decoded.position), decoded.rotationQ[3] );
					modelData.scale = Float3( decoded.scale );
					modelData.visible = true;
					modelData.id = decoded.objectID;

					::Utility::String::StringToWstring( decoded.name, modelData.modelPath );
				}
				object->Init(modelData);
				// RB DEBUG
				// Is just using the model position since the rigid body data should never be sent to the client 
				RBInitData RBData;
				RBData.position = decoded.position;
				RBData.rotation = ArrayToQuaternion( decoded.position );
				RBData.scale =  Float3( decoded.scale );

				object->InitRB( RBData );
				// !RB DEBUG 

				(*this->privData->dynamicObjects)[decoded.objectID] = object;

			}		
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCreatePlayer:
			{
				Protocol_ObjectCreatePlayer decoded(data);
				this->InitiatePlayer( decoded.objectID, decoded.meshName, decoded.position, decoded.rotationQ, decoded.scale, decoded.owner );				
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectJoinTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectLeaveTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponCooldown:	break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponEnergy:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectRespawn:	
			{
				Protocol_ObjectRespawn decoded(data);
				
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}

				if( object )
				{
					if( this->privData->myId == decoded.objectID )
					{
						this->privData->camera.SetPosition( decoded.position );
					}
					object->setPos( decoded.position );
					object->updateWorld();
					// RB DEBUG 
					object->setRBPos ( decoded.position );  
					object->updateRBWorld();
					// !RB DEBUG 
				}
				this->currGameUI =  this->gameUI;
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDie:
			{
				Protocol_ObjectDie decoded(data);
				// if is this player. Remember to change camera
				int killerID = decoded.killerID;
				int victimID = decoded.objectID;
				if( this->privData->myId == decoded.objectID )
				{
					this->currGameUI =  this->respawnUI;
					// set countdown 
					((RespawnUI*)currGameUI)->SetCountdown( decoded.seconds );
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDisconnectPlayer:
			{
				//Remove the disconnected player
				Protocol_ObjectDisconnectPlayer decoded(data);
				C_Player *player; 
				player = (this->privData->players)[decoded.objectID];

				if( player )
				{
					if( this->privData->myId == decoded.objectID )
					{
						// dont delete my player
					}
					if( player )
					{
						player->SetVisible(false);
						(this->privData->players)[decoded.objectID].Release();
					}
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectAction:
			{
				Protocol_ObjectAction decoded(data);

				C_Player *player; 
				player = (this->privData->players)[decoded.objectID];
				
				if( player )
				{
					if( this->privData->myId == decoded.objectID )
					{
						// my player animation
					//}
					//else
					//{
						// HACK for now animate my char
						switch (decoded.animationID)
						{
						case  GameLogic::PlayerAction::PlayerAction_Walk:
							player->playAnimation(L"run_forwards", true);
							break;
						case GameLogic::PlayerAction::PlayerAction_Jump:
							player->playAnimation(L"movement", true);
							break;
						case GameLogic::PlayerAction::PlayerAction_Idle:
							player->playAnimation(L"idle", true);
							break;

						case GameLogic::WeaponAction::WeaponAction_PrimaryShoot:
							break;
						case GameLogic::WeaponAction::WeaponAction_SecondaryShoot:
							break;
						case GameLogic::WeaponAction::WeaponAction_Reload:
							break;


						default:
							break;
						}
					}
				}
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCollision:
			{
				Protocol_ObjectCollision decoded(data);
				C_Object *object; 
				object = (this->privData->players)[decoded.objectID];
				if( !object)
				{
					// if it is not a player 
					object = (*this->privData->dynamicObjects)[decoded.objectID];
				}
				if( object )
				{
					switch (decoded.collisionID)
					{
					case GameLogic::CollisionEvent::CollisionEvent_BasicCollision:
						break;
					default:
						break;
					}
				}
			}
			return GameClientState::event_processed;
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

	return message;
}
