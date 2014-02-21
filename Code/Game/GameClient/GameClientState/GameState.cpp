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
	::Input::Mouse *mouseInput;
	::Input::Keyboard *keyboardInput_raw;
	::Input::ApplicationKeyboard *keyboardInput_app;

	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *staticObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_DynamicObj>> *dynamicObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>> *lights;

	C_Player player;
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
	this->privData->mouseInput = shared.mouseDevice;
	this->privData->keyboardInput_raw = shared.keyboardDevice_raw;
	this->privData->keyboardInput_app = shared.keyboardDevice_application;
	this->privData->staticObjects = &shared.staticObjects;
	this->privData->dynamicObjects = &shared.dynamicObjects;
	this->privData->lights = &shared.lights;

	this->privData->keyboardInput_app->Deactivate();
	this->privData->mouseInput->Enable();

	Graphics::API::Option gfxOp = Graphics::API::GetOption();
	Float aspectRatio = gfxOp.Resolution.x / gfxOp.Resolution.y;
	this->privData->camera.SetPerspectiveProjection( Utility::Value::Radian(90.0f), aspectRatio, 0.1f, 1000.0f );
	Graphics::API::SetProjection( this->privData->camera.GetProjectionMatrix() );
	gfxOp.AmbientValue = 0.5f;
	gfxOp.GlobalGlowTint = Math::Float3(2,1,1);
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
	this->gameUI = new GamingUI(this->privData->mouseInput, this->privData->keyboardInput_raw, this->privData->nwClient, &this->privData->camera);
	this->respawnUI = new RespawnUI(this->privData->nwClient, 20);
	this->currGameUI = gameUI; 
	((GamingUI*)gameUI)->Init();
	// TODO init respawn

	return true;
}

void GameState::InitiatePlayer( int id, const std::string &modelName, const float position[3], const float rotation[4], const float scale[3], bool isMyPlayer )
{
	ModelInitData modelData;
	modelData.visible	= true;
	modelData.position	= position;
	modelData.rotation	= ArrayToQuaternion( rotation );
	modelData.scale		= scale;
	StringToWstring( modelName, modelData.modelPath );
	modelData.id		= id;

	// RB DEBUG
	RBInitData RBData;
	RBData.position = position;
	RBData.rotation = ArrayToQuaternion( rotation );
	RBData.scale =  scale;
	RBData.type = RB_Type_Cube;
	// !RB DEBUG 
	if( isMyPlayer )
	{
		if( this->privData->player.Init(modelData) )
		{
			// RB DEBUG
			this->privData->player.InitRB( RBData );
			// !RB DEBUG 

			this->privData->myId = id;
			this->privData->camera.SetPosition( this->privData->player.getPos() );
			Float3 offset = Float3( 0.0f );
			// DEBUG position of camera so we can see the player model
			//offset.y = this->privData->player.getScale().y * 5.0f;
			//offset.z = this->privData->player.getScale().z * -5.0f;
			// !DEBUG
			this->privData->camera.SetHeadOffset( offset );
			this->privData->camera.UpdateOrientation();
		}
	}
	else
	{
		C_DynamicObj *p = new C_DynamicObj();
		if( p->Init(modelData) )
		{
			// RB DEBUG
			this->privData->player.InitRB( RBData );
			// !RB DEBUG 

			(*this->privData->dynamicObjects)[id] = p;
		}
	}
}

GameClientState::ClientState GameState::Update( float deltaTime )
{
	GameStateUI::UIState UIstate = this->currGameUI->Update( deltaTime );
	switch (UIstate)
	{
	case DanBias::Client::GameStateUI::UIState_same:
		break;
	case DanBias::Client::GameStateUI::UIState_gaming:
		break;
	case DanBias::Client::GameStateUI::UIState_main_menu:
		//this->privData->nextState = 
		break;
	case DanBias::Client::GameStateUI::UIState_shut_down:
		this->privData->nextState = ClientState_Quit;
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
	this->privData->player.Render();

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

			Oyster::Math3D::Float4x4 translation = Oyster::Math3D::TranslationMatrix(Float3( 0,132, 20)); 
			Oyster::Math3D::Float4x4 scale = Oyster::Math3D::ScalingMatrix(Float3( 0.5f, 0.5f, 0.5f));
			Oyster::Math3D::Float4x4 world = translation  * scale;
			Oyster::Graphics::API::RenderDebugCube( world );
			Oyster::Graphics::API::RenderDebugCube(this->privData->player.getRBWorld()); 

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

	// render current UI state
	if(currGameUI->HaveGUIRender())
		currGameUI->RenderGUI();
	if(currGameUI->HaveTextRender())
		currGameUI->RenderText();
	
	Oyster::Graphics::API::EndFrame();
	return true;
}

bool GameState::Release()
{
	Graphics::API::Option o = Graphics::API::GetOption();
	if( privData )
	{
		this->privData->mouseInput->Disable();

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
	currGameUI = NULL;
	
	return true;
}

void GameState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

void GameState::ReadKeyInput()
{
#ifdef _DEBUG // DEGUG KEYS

	// Reload shaders
	if( this->privData->keyboardInput_raw->IsKeyDown(::Input::Enum::SAKI_R) )
	{
		if( !this->key_Reload_Shaders )
		{

			Graphics::API::ReloadShaders();

			this->key_Reload_Shaders = true;
		}
	} 
	else 
		this->key_Reload_Shaders = false;

	// toggle wire frame render
	if( this->privData->keyboardInput_raw->IsKeyDown(::Input::Enum::SAKI_T) )
	{
		if( !this->key_Wireframe_Toggle )
		{
			this->renderWhireframe = !this->renderWhireframe;
			this->key_Wireframe_Toggle = true;
		}
	} 
	else 
		this->key_Wireframe_Toggle = false;

#endif // !DEGUG KEYS
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
		case protocol_Gameplay_ObjectPickup:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectDamage:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectHealthStatus:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectPosition:
			{
				Protocol_ObjectPosition decoded(data);

				// if is this player. Remember to change camera
				if( this->privData->myId == decoded.object_ID )
					this->privData->camera.SetPosition( decoded.position );

				(*this->privData->dynamicObjects)[decoded.object_ID]->setPos( decoded.position );
				// RB DEBUG 
				(*this->privData->dynamicObjects)[decoded.object_ID]->setRBPos ( decoded.position );  
				// !RB DEBUG 
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectScale:
			{
				Protocol_ObjectScale decoded(data);
				(*this->privData->dynamicObjects)[decoded.object_ID]->setScale( decoded.scale );
				// RB DEBUG 
				(*this->privData->dynamicObjects)[decoded.object_ID]->setRBScale ( decoded.scale );  
				// !RB DEBUG 
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectRotation:
			{
				Protocol_ObjectRotation decoded(data);
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );

				// if is this player. Remember to change camera
				if( this->privData->myId == decoded.object_ID )
					this->privData->camera.SetRotation( rotation );

				(*this->privData->dynamicObjects)[decoded.object_ID]->setRot( rotation );
				// RB DEBUG 
				(*this->privData->dynamicObjects)[decoded.object_ID]->setRBRot ( rotation );  
				// !RB DEBUG 
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectPositionRotation:
			{
				Protocol_ObjectPositionRotation decoded(data);
				Float3 position = decoded.position;
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );

				// if is this player. Remember to change camera
				if( this->privData->myId == decoded.object_ID )
				{
					if( !Within(position.Dot(position), 2500.0f, 90000.0f) )
					{ // HACK: bug trap
						const char *breakPoint = "Something is wrong.";
						position = Float3( 0.0f, 160.0f, 0.0f );
					}

					this->privData->camera.SetPosition( position );
					this->privData->camera.SetRotation( rotation );
					this->privData->player.setPos( position );
					this->privData->player.setRot( rotation );
					this->privData->player.updateWorld();
					// RB DEBUG 
					this->privData->player.setRBPos ( position );  
					this->privData->player.setRBRot ( rotation );  
					this->privData->player.updateRBWorld();
					// !RB DEBUG 
				}
				else
				{
					C_DynamicObj *object = (*this->privData->dynamicObjects)[decoded.object_ID];

					if( object )
					{
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
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectEnabled:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectDisabled:
			{
				Protocol_ObjectDisable decoded(data);

				auto object = this->privData->dynamicObjects->find( decoded.objectID );
				if( object != this->privData->dynamicObjects->end() )
				{
					object->second = nullptr;
					this->privData->dynamicObjects->erase( object );
				}
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
					modelData.id = decoded.object_ID;

					::Utility::String::StringToWstring( decoded.name, modelData.modelPath );
				}
				object->Init(modelData);
				// RB DEBUG
				// Is just using the model position since the rigid body data should never be sent to the client 
				RBInitData RBData;
				RBData.position = decoded.position;
				RBData.rotation = ArrayToQuaternion( decoded.position );
				RBData.scale =  Float3( decoded.scale );

				this->privData->player.InitRB( RBData );
				// !RB DEBUG 

				(*this->privData->dynamicObjects)[decoded.object_ID] = object;

			}		
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCreatePlayer:
			{
				Protocol_ObjectCreatePlayer decoded(data);
				this->InitiatePlayer( decoded.object_ID, decoded.meshName, decoded.position, decoded.rotationQ, decoded.scale, decoded.owner );				
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectJoinTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectLeaveTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponCooldown:	break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponEnergy:		break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectRespawn:		
			this->currGameUI =  this->gameUI;
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDie:
			this->currGameUI =  this->respawnUI;
			// set countdown 
		case protocol_Gameplay_ObjectDisconnectPlayer:
			{
				//Removes 
				Protocol_ObjectDisconnectPlayer decoded(data);
				auto object = this->privData->dynamicObjects->find( decoded.objectID );
				if( object != this->privData->dynamicObjects->end() )
				{
					object->second = nullptr;
					this->privData->dynamicObjects->erase( object );
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
