﻿#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera_FPS.h"
#include <GameServerAPI.h>

#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include "C_obj/C_StaticObj.h"
#include "Utilities.h"

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

	bool key_forward;
	bool key_backward;
	bool key_strafeRight;
	bool key_strafeLeft;
	bool key_Shoot;
	bool key_Jump;

	bool key_Reload_Shaders;

	C_Player player;
	Camera_FPS camera;

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

	this->privData->key_forward = false;
	this->privData->key_backward = false;
	this->privData->key_strafeRight = false;
	this->privData->key_strafeLeft = false;

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = shared.network;
	this->privData->input = shared.input;
	this->privData->staticObjects = &shared.staticObjects;
	this->privData->dynamicObjects = &shared.dynamicObjects;

	Graphics::API::Option gfxOp = Graphics::API::GetOption();
	Float aspectRatio = gfxOp.Resolution.x / gfxOp.Resolution.y;
	this->privData->camera.SetPerspectiveProjection( Radian(90.0f), aspectRatio, 0.1f, 1000.0f );
	Graphics::API::SetProjection( this->privData->camera.GetProjectionMatrix() );

	//tell server ready
	this->privData->nwClient->Send( Protocol_General_Status(Protocol_General_Status::States_ready) );
			
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

	if( isMyPlayer )
	{
		if( this->privData->player.Init(modelData) )
		{
			this->privData->myId = id;
			this->privData->camera.SetPosition( this->privData->player.getPos() );
			Float3 offset = Float3( 0.0f );
			offset.y = this->privData->player.getScale().y * 0.9f;
			this->privData->camera.SetHeadOffset( offset );
			this->privData->camera.UpdateOrientation();
		}
	}
	else
	{
		C_DynamicObj *p = new C_DynamicObj();
		if( p->Init(modelData) )
		{
			(*this->privData->dynamicObjects)[id] = p;
		}
	}
}

GameClientState::ClientState GameState::Update( float deltaTime )
{
	this->ReadKeyInput();
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
			dynamicObject->second->Render();
	}

	Oyster::Graphics::API::EndFrame();
	return true;
}

bool GameState::Release()
{
	if( privData )
	{
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

		this->privData->staticObjects->clear();
		this->privData->dynamicObjects->clear();

		privData = NULL;
	}
	return true;
}

void GameState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

void GameState::ReadKeyInput()
{
	if( this->privData->input->IsKeyPressed(DIK_W) )
	{
		if(!this->privData->key_forward)
		{
			this->privData->nwClient->Send( Protocol_PlayerMovementForward() );
			this->privData->key_forward = true;
		}
	}
	else
		this->privData->key_forward = false;

	if( this->privData->input->IsKeyPressed(DIK_S) )
	{
		if( !this->privData->key_backward )
		{
			this->privData->nwClient->Send( Protocol_PlayerMovementBackward() );
			this->privData->key_backward = true;
		}
	}
	else 
		this->privData->key_backward = false;

	if( this->privData->input->IsKeyPressed(DIK_A) )
	{
		if( !this->privData->key_strafeLeft )
		{
			this->privData->nwClient->Send( Protocol_PlayerMovementLeft() );
			this->privData->key_strafeLeft = true;
		}
	}
	else 
		this->privData->key_strafeLeft = false;

	if( this->privData->input->IsKeyPressed(DIK_D) )
	{
		if( !this->privData->key_strafeRight )
		{
			this->privData->nwClient->Send( Protocol_PlayerMovementRight() );
			this->privData->key_strafeRight = true;
		}
	} 
	else 
		this->privData->key_strafeRight = false;

	if( this->privData->input->IsKeyPressed(DIK_R) )
	{
		if( !this->privData->key_Reload_Shaders )
		{
			//this->privData->nwClient->Send( Protocol_PlayerMovementRight() );
#ifdef _DEBUG
			Graphics::API::ReloadShaders();
#endif
			this->privData->key_Reload_Shaders = true;
		}
	} 
	else 
		this->privData->key_Reload_Shaders = false;


	//send delta mouse movement 
	{
		this->privData->camera.YawRight( this->privData->input->GetYaw() * 0.017f );
		this->privData->camera.PitchDown( this->privData->input->GetPitch() * 0.017f );
		this->privData->camera.UpdateOrientation();

		privData->nwClient->Send( Protocol_PlayerLook(this->privData->camera.GetLook(), this->privData->camera.GetRight()) );
	}

	// shoot
	if( this->privData->input->IsKeyPressed(DIK_Z) )
	{
		if( !this->privData->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = true;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = false;
			this->privData->nwClient->Send( playerShot );
			this->privData->key_Shoot = true;
		}
	} 
	else 
		this->privData->key_Shoot = false;
	if( this->privData->input->IsKeyPressed(DIK_X) )
	{
		if( !this->privData->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = true;
			playerShot.utilityPressed = false;
			this->privData->nwClient->Send( playerShot );
			this->privData->key_Shoot = true;
		}
	} 
	else 
		this->privData->key_Shoot = false;
	if( this->privData->input->IsKeyPressed(DIK_C) )
	{
		if( !this->privData->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = true;
			this->privData->nwClient->Send( playerShot );
			this->privData->key_Shoot = true;
		}
	} 
	else 
		this->privData->key_Shoot = false;

	// jump
	if( this->privData->input->IsKeyPressed(DIK_SPACE) )
	{
		if(!this->privData->key_Jump)
		{
			this->privData->nwClient->Send( Protocol_PlayerJump() );
			this->privData->key_Jump = true;
		}
	}
	else 
		this->privData->key_Jump = false;

	// TODO: implement sub-menu
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
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectScale:
			{
				Protocol_ObjectScale decoded(data);
				(*this->privData->dynamicObjects)[decoded.object_ID]->setScale( decoded.scale );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectRotation:
			{
				Protocol_ObjectRotation decoded(data);
				Quaternion rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );

				// if is this player. Remember to change camera
				if( this->privData->myId == decoded.object_ID )
					this->privData->camera.SetAngular( AngularAxis(rotation) );

				(*this->privData->dynamicObjects)[decoded.object_ID]->setRot( rotation );
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
					this->privData->camera.SetPosition( position );
					this->privData->camera.SetAngular( AngularAxis(rotation) );
				}

				C_DynamicObj *object = (*this->privData->dynamicObjects)[decoded.object_ID];
				if( object )
				{
					object->setPos( position );
					object->setRot( rotation );
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
					modelData.rotation = Quaternion( Float3(decoded.rotationQ), decoded.rotationQ[3] );
					modelData.scale = Float3( decoded.scale );
					modelData.visible = true;
					modelData.id = decoded.object_ID;

					::Utility::String::StringToWstring( decoded.name, modelData.modelPath );
				}
				object->Init(modelData);

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

	return message;
}
