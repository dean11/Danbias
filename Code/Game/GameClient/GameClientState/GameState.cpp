﻿#include "GameState.h"
#include "DllInterfaces/GFXAPI.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include "Camera_FPSV2.h"
//#include <GameServerAPI.h>
#include "C_Light.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_DynamicObj.h"
#include "C_obj/C_StaticObj.h"
#include "Utilities.h"
#include "GamingUI.h"
#include "RespawnUI.h"
#include "StatsUI.h"
#include "IngameMenyUI.h"
#include "InGameOptionsUI.h"
#include <ObjectDefines.h>
#include "ColorDefines.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Math3D;
using namespace ::GameLogic;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::String;
using namespace ::Utility::Value;
using namespace ::Utility::StaticArray;

//#define _CAMERA_DEBUG
struct  GameState::MyData
{
	MyData(){}
	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	::Input::Mouse *mouseInput;
	::Input::Keyboard *keyboardInput;

	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *staticObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_DynamicObj>> *dynamicObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>> *lights;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *pickups;

	//C_Player player;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Player>> players;
	Camera_FPSV2 camera;

	FirstPersonWeapon* weapon;

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
	this->privData->keyboardInput = shared.keyboardDevice;
	this->privData->staticObjects = &shared.staticObjects;
	this->privData->dynamicObjects = &shared.dynamicObjects;
	this->privData->lights = &shared.lights;
	this->privData->pickups = &shared.pickups;
	this->privData->weapon = shared.weapon;

	Graphics::API::Option gfxOp = Graphics::API::GetOption();
	Float aspectRatio = gfxOp.resolution.x / gfxOp.resolution.y;
	this->privData->camera.SetPerspectiveProjection( Utility::Value::Radian(90.0f), aspectRatio, 0.1f, 200.0f );
	Graphics::API::SetProjection( this->privData->camera.GetProjectionMatrix() );

	// DEGUG KEYS
	this->key_Reload_Shaders = false;
	this->key_Wireframe_Toggle = false;
	this->renderWhireframe = false;
	// !DEGUG KEYS
	this->gameOver = false;

	shared.keyboardDevice->ReleaseTextTarget();
	//shared.mouseDevice->AddMouseEvent(this);

	auto light = this->privData->lights->begin();
	for( ; light != this->privData->lights->end(); ++light )
	{
		light->second->Render();
	}

	this->privData->mouseInput->SetSensitivity(1.7f);

	// create UI states
	this->gameUI = new GamingUI( &shared, &this->privData->camera );
	this->respawnUI = new RespawnUI( &shared );
	this->statsUI = new StatsUI( &shared );
	this->inGameMeny = new IngameMenyUI( &shared );
	this->inGameOptions = new InGameOptionsUI( &shared );
	((GamingUI*)this->gameUI)->Init();
	((RespawnUI*)this->respawnUI)->Init();
	((IngameMenyUI*)this->inGameMeny)->Init();
	((InGameOptionsUI*)this->inGameOptions)->Init();

	this->uiStack[0] = this->gameUI;
	this->uiStackTop = 0;
	this->gameUI->ActivateInput();

	// HACK hardcoded max nr of players
	((StatsUI*)statsUI)->Init(10);

	//tell server ready
	this->privData->nwClient->Send( Protocol_General_Status(Protocol_General_Status::States_ready) );

	return true;
}

void GameState::InitiatePlayer( int id, const std::string &modelName, const float position[3], const float rotation[4], const float scale[3], bool isMyPlayer)
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

		// set color tint
		ColorDefines colors;
		p->SetTint(colors.getTintColor(id));
		p->SetGlowTint(colors.getGlowColor(id));
		
		Graphics::Definitions::Pointlight pl;
		pl.Pos = p->getPos();
		pl.Bright = 0.3f;
		pl.Radius = 20;
		pl.Color = p->GetGlowTint();
		UniquePointer<C_Light> newLight(new C_Light(pl, p->GetId()));
		p->SetLight(p->GetId());
		(newLight)->Render();
		(*this->privData->lights)[p->GetId()] = newLight;

		(this->privData->players)[id] = p;
		
		if( isMyPlayer )
		{
			this->privData->myId = id;
			this->privData->camera.SetPosition( p->getPos() );
			this->privData->camera.SetHeadOffset( Float3(0.0f, 0.45f * p->getScale().y, 0.0f) );
			Float3 offset = Float3( 0.0f );
			// DEBUG position of camera so we can see the player model
		#ifdef _CAMERA_DEBUG
			offset.y = p->getScale().y * 5.0f;
			offset.z = p->getScale().z * -5.0f;
			// !DEBUG
			this->privData->camera.SetHeadOffset( offset );
			this->privData->camera.UpdateOrientation();
		#endif
			((StatsUI*)this->statsUI)->addPLayer( id, colors.getColorName(id), 0, 0); 
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
	GameStateUI::UIState UIstate = this->UIstackUpdate( deltaTime );

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
	case DanBias::Client::GameStateUI::UIState_previous:
		this->UIstackPeek()->ChangeState( DanBias::Client::GameStateUI::UIState_same );
		this->UIstackPop();
		break;
	case DanBias::Client::GameStateUI::UIState_gaming:
		break;
	case DanBias::Client::GameStateUI::UIState_main_menu:
		{
			this->privData->nextState = ClientState_Main;

			// disconnect 

			// inactivate MenyInput
			this->privData->mouseInput->RemoveMouseEvent( (Input::Mouse::MouseEvent*)this->UIstackPeek() );
			this->privData->keyboardInput->RemoveKeyboardEvent( (Input::Keyboard::KeyboardEvent*)this->UIstackPeek() );
			this->UIstackPeek()->ChangeState( DanBias::Client::GameStateUI::UIState_same );

		}
		break;
	case GameStateUI::UIState_ingame_meny:
		{
			this->UIstackPeek()->ChangeState( DanBias::Client::GameStateUI::UIState_same );
			this->UIstackPush( this->inGameMeny );
		}
		break;
	case GameStateUI::UIState_ingame_options:
		{
			this->UIstackPeek()->ChangeState( DanBias::Client::GameStateUI::UIState_same );
			this->UIstackPush( this->inGameOptions );
		}
		break;
	case GameStateUI::UIState_resume_game:
		{
			this->UIstackPeek()->ChangeState( DanBias::Client::GameStateUI::UIState_same );
			this->UIstackPop();
		}
		break;
	
	default:
		break;
	} 
	// DEBUG keybindings
	ReadKeyInput();

	//Rotate pickups
	auto staticObject = this->privData->pickups->begin();
	for( ; staticObject != this->privData->pickups->end(); ++staticObject )
	{
		Quaternion rot = staticObject->second->getRotation();
		rot *= Quaternion(Float3(0, 1, 0), 1000 * deltaTime).Normalize(); 
		rot.Normalize();
		staticObject->second->setRot(rot);
		staticObject->second->updateWorld();
	}
	//!Rotate pickups

	return this->privData->nextState;
}

bool GameState::Render()
{
	Oyster::Graphics::API::SetView( this->privData->camera.GetViewMatrix() );

	Oyster::Graphics::API::NewFrame();
	
	this->privData->weapon->Update( this->privData->camera.GetViewMatrix(), this->privData->camera.GetLook() );
	this->privData->weapon->Render();

	// for debugging to be replaced with render weapon
	auto playerObject = this->privData->players.begin();
	for( ; playerObject != this->privData->players.end(); ++playerObject )
	{
		if(playerObject->second)
		{
			#ifndef _CAMERA_DEBUG
			if( this->privData->myId != playerObject->second->GetId() )
			#endif
			{
				playerObject->second->Render();
			}
		}
	}

	auto staticObject = this->privData->staticObjects->begin();
	for( ; staticObject != this->privData->staticObjects->end(); ++staticObject )
	{
		if(staticObject->second)
		{
			staticObject->second->Render();
		}
	}

	auto dynamicObject = this->privData->dynamicObjects->begin();
	for( ; dynamicObject != this->privData->dynamicObjects->end(); ++dynamicObject )
	{
		if( dynamicObject->second )
		{
			dynamicObject->second->Render();

		}
	}

	auto pickup = this->privData->pickups->begin();
	for( ; pickup != this->privData->pickups->end(); ++pickup )
	{
		if( pickup->second )
		{
			pickup->second->Render();

		}
	}

#ifdef _DEBUG //RB DEBUG render wire frame 
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
#endif //!RB DEBUG 

	// render gui elements
	Oyster::Graphics::API::StartGuiRender();
	this->UIstackRenderGUI();


	if(renderStats)
	{	
		if(statsUI->HaveGUIRender())
			statsUI->RenderGUI();
	}

	// render gui text
	Oyster::Graphics::API::StartTextRender();
	this->UIstackRenderText();

	if(renderStats)
	{	
		if(statsUI->HaveTextRender())
			statsUI->RenderText();
	}

	if( this->gameOver )
	{
		Oyster::Graphics::API::RenderText( L"GAME OVER", Float3(0.2f,0.1f,0.1f), Float2(0.6f,0.1f), 0.1f);
		Oyster::Graphics::API::RenderText( L"press 'ESC' to continue", Float3(0.2f,0.8f,0.1f), Float2(0.8f,0.1f), 0.04f);
	}
	Oyster::Graphics::API::EndFrame();
	return true;
}

bool GameState::Release()
{
	Graphics::API::Option o = Graphics::API::GetOption();
	if( privData )
	{
		this->UIstackPeek()->DeactivateInput();

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
			light->second->Release();
		}

		auto pickup = this->privData->pickups->begin();
		for( ; pickup != this->privData->pickups->end(); ++pickup )
		{
			pickup->second = nullptr;
		}

		this->privData->staticObjects->clear();
		this->privData->dynamicObjects->clear();
		this->privData->lights->clear();
		this->privData->pickups->clear();

		if(this->privData->weapon)
		{
			delete this->privData->weapon;
			this->privData->weapon = nullptr;
		}

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
	if(inGameMeny)
	{
		inGameMeny->Release();
		delete inGameMeny;
		inGameMeny = NULL;
	}
	if(inGameOptions)
	{
		inGameOptions->Release();
		delete inGameOptions;
		inGameOptions = NULL;
	}
	if(statsUI)
	{
		statsUI->Release();
		delete statsUI;
		statsUI = NULL;
	}
	
	Graphics::API::SetView(Math::Float4x4::identity);
	Graphics::API::SetProjection(Math::Float4x4::null);
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
	if( this->privData->keyboardInput->IsKeyDown(::Input::Enum::SAKI_R) )
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
	if( this->privData->keyboardInput->IsKeyDown(::Input::Enum::SAKI_T) )
	{
		if( !this->key_Wireframe_Toggle )
		{
			this->renderWhireframe = !this->renderWhireframe;
			this->key_Wireframe_Toggle = true;
		}
	} 
	else 
	{
		this->key_Wireframe_Toggle = false;
	}
#endif // !DEGUG KEYS

	// show Stats
	if( this->privData->keyboardInput->IsKeyDown(::Input::Enum::SAKI_Tab) )
	{
		this->renderStats = true;
		this->privData->weapon->Shoot();
	} 
	else 
	{
		this->renderStats = false;
	}

	if( this->gameOver )
	{
		if( this->privData->keyboardInput->IsKeyDown(::Input::Enum::SAKI_Escape) )
		{
			this->privData->nextState = ClientState_Main; 
		}
		this->renderStats = true;
	}
}

void GameState::Gameplay_ObjectPickup( CustomNetProtocol data )
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

void GameState::Gameplay_ObjectDamage( CustomNetProtocol data )
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
			// set given players HP 
			((GamingUI*)this->gameUI)->SetHPtext( ::std::to_wstring((int)decoded.healthLost) );
		}
	}
}

void GameState::Gameplay_ObjectHealthStatus( CustomNetProtocol data )
{

}

void GameState::Gameplay_ObjectPosition( CustomNetProtocol data )
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
void GameState::Gameplay_ObjectScale( CustomNetProtocol data )
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
		if(object->GetLight()!=-1)
		{
			std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>>::iterator light = privData->lights->find(object->GetLight());
			if(light != privData->lights->end())
			{
				light->second->setPos(object->getPos());
			}
		}
	}
}
void GameState::Gameplay_ObjectRotation( CustomNetProtocol data )
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
void GameState::Gameplay_ObjectPositionRotation( CustomNetProtocol data )
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
		if(object->GetLight()!=-1)
		{
			std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>>::iterator light = privData->lights->find(object->GetLight());
			if(light != privData->lights->end())
			{
				light->second->setPos(object->getPos());
			}
		}
		// RB DEBUG 
		object->setRBPos ( position );  
		object->setRBRot ( rotation );  
		object->updateRBWorld();
		// !RB DEBUG 
	}
}
void GameState::Gameplay_ObjectEnabled( CustomNetProtocol data )
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

			if(!object)
			{
				object = (*this->privData->pickups)[decoded.objectID];
			}
		}
	}

	if( object )
	{
		object->SetVisible(true);
	}
}
void GameState::Gameplay_ObjectDisabled( CustomNetProtocol data )
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

			if(!object)
			{
				object = (*this->privData->pickups)[decoded.objectID];
			}
		}
	}

	if( object )
	{
		object->SetVisible(false);
	}
}
void GameState::Gameplay_ObjectCreate( CustomNetProtocol data )
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
void GameState::Gameplay_ObjectCreatePlayer( CustomNetProtocol data )
{
	Protocol_ObjectCreatePlayer decoded(data);
	this->InitiatePlayer( decoded.objectID, decoded.meshName, decoded.position, decoded.rotationQ, decoded.scale, decoded.owner );				
}
void GameState::Gameplay_ObjectJoinTeam( CustomNetProtocol data )
{

}
void GameState::Gameplay_ObjectLeaveTeam( CustomNetProtocol data )
{

}
void GameState::Gameplay_ObjectWeaponCooldown( CustomNetProtocol data )
{

}
void GameState::Gameplay_ObjectWeaponEnergy( CustomNetProtocol data )
{
	Protocol_ObjectWeaponEnergy decoded(data);
	if( this->privData->myId == decoded.objectID )
	{
		// show my energy 
		int energy = (int)decoded.energy;
		((GamingUI*)this->gameUI)->SetEnergyText(std::to_wstring(energy));
	}
}
void GameState::Gameplay_ObjectRespawn( CustomNetProtocol data )
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
			this->UIstackRemove( this->respawnUI );
		}

		object->setPos( decoded.position );
		object->updateWorld();
		// RB DEBUG 
		object->setRBPos ( decoded.position );  
		object->updateRBWorld();
		// !RB DEBUG 
	}
	
}
void GameState::Gameplay_ObjectDie( CustomNetProtocol data )
{
	Protocol_ObjectDie decoded(data);
	if( this->privData->myId == decoded.victimID )
	{
		GameStateUI * topStacked = this->UIstackPop();
		this->UIstackPush( this->respawnUI );
		
		if( topStacked != this->gameUI )
		{
			this->UIstackPush( topStacked );
		}

		((GamingUI*)this->gameUI)->StopGamingUI();
		// set countdown 
		((RespawnUI*)this->respawnUI)->SetCountdown( decoded.seconds );
		
	}
	// update score board
	((StatsUI*)this->statsUI)->updateDeatchScore( decoded.victimID, decoded.victimDeathCount ); 
	((StatsUI*)this->statsUI)->updateKillScore( decoded.killerID, decoded.killerKillCount ); 

	// print killer message
	ColorDefines colors;

	std::wstring message;
	if (decoded.victimID == decoded.killerID)
	{
		if (decoded.victimID == this->privData->myId)
		{
			message =  L"You committed suicide";
		}
		else
			message = colors.getColorName(decoded.killerID) + L" committed suicide";
	}
	else
	{
		if (decoded.victimID == this->privData->myId)
		{
			message = L"You were killed by " + colors.getColorName(decoded.killerID);
		}
		else if (decoded.killerID == this->privData->myId)
		{
			message = L"You killed " + colors.getColorName(decoded.victimID);
		}
		else
			message = colors.getColorName(decoded.killerID) + L" killed " + colors.getColorName(decoded.victimID);
	}

	((GamingUI*)this->gameUI)->SetKillMessage(message);
}
void GameState::Gameplay_PlayerScore( CustomNetProtocol data )
{
	Protocol_PlayerScore decoded(data);
	// update scoreboard 
	ColorDefines colors;
	((StatsUI*)this->statsUI)->addPLayer( decoded.playerID, colors.getColorName(decoded.playerID), decoded.killCount, decoded.deathCount );	
}
void GameState::Gameplay_ObjectDisconnectPlayer( CustomNetProtocol data )
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
			delete player;
			((StatsUI*)this->statsUI)->removePlayer( decoded.objectID);
		}
	}
}
void GameState::Gameplay_ObjectAction( CustomNetProtocol data )
{
	Protocol_ObjectAction decoded(data);

	C_Player *player; 
	player = (this->privData->players)[decoded.objectID];

	if( player )
	{
		if( this->privData->myId != decoded.objectID )
		{
			// my player animation
			switch (decoded.animationID)
			{
			case  GameLogic::PlayerAction::PlayerAction_Walk:
				player->playAnimation(L"run_forwards", true);
				break;
			case GameLogic::PlayerAction::PlayerAction_Jump:
				player->playAnimation(L"movement", true);
				break;
			case GameLogic::PlayerAction::PlayerAction_Idle:
				player->stopAllAnimations();
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
		else
		{
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
				player->stopAllAnimations();
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
void GameState::Gameplay_ObjectCollision( CustomNetProtocol data )
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

void GameState::General_GameOver( CustomNetProtocol data )
{
	Protocol_General_GameOver decoded(data);
	
	// turn off gameInput
	this->privData->mouseInput->RemoveMouseEvent((Input::Mouse::MouseEvent*)(GamingUI*)this->gameUI);
	this->privData->keyboardInput->RemoveKeyboardEvent((Input::Keyboard::KeyboardEvent*)(GamingUI*)this->gameUI);
	gameOver = true;
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
				Gameplay_ObjectPickup( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDamage:			
			{
				Gameplay_ObjectDamage( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectHealthStatus:		
			{ 
				// don't know if needed
				Gameplay_ObjectHealthStatus( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectPosition:
			{
				Gameplay_ObjectPosition( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectScale:
			{
				Gameplay_ObjectScale( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectRotation:
			{
				Gameplay_ObjectRotation( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectPositionRotation:
			{
				Gameplay_ObjectPositionRotation( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectEnabled:	
			{
				Gameplay_ObjectEnabled( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDisabled:
			{
				Gameplay_ObjectDisabled( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCreate:
			{
				Gameplay_ObjectCreate( data );
			}		
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCreatePlayer:
			{
				Gameplay_ObjectCreatePlayer( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectJoinTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectLeaveTeam:			break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponCooldown:	break; /** @todo TODO: implement */
		case protocol_Gameplay_ObjectWeaponEnergy:		
			{
				Gameplay_ObjectWeaponEnergy( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectRespawn:	
			{
				Gameplay_ObjectRespawn( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDie:
			{
				Gameplay_ObjectDie( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_PlayerScore:
			{
				Gameplay_PlayerScore( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectDisconnectPlayer:
			{
				Gameplay_ObjectDisconnectPlayer( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectAction:
			{
				Gameplay_ObjectAction( data );
			}
			return GameClientState::event_processed;
		case protocol_Gameplay_ObjectCollision:
			{
				Gameplay_ObjectCollision( data );
			}
			return GameClientState::event_processed;
		default: break;
		}
	}
	else if( ProtocolIsGeneral(ID) )
	{
		CustomNetProtocol data = message.args.data.protocol;
		switch( ID )
		{
			case protocol_General_Status:				break; /** @todo TODO: implement */
			case protocol_General_Text:					break; /** @todo TODO: implement */
			case protocol_General_GameOver: General_GameOver( data );
				break;
		default: break;
		}
	}

	return message;
}

void GameState::UIstackPush( GameStateUI *ui )
{
	if( !ui )
		return;

	GameStateUI *previous = this->UIstackPeek();

	++this->uiStackTop;
	if( this->uiStackTop >= (int)NumElementsOf(this->uiStack) )
	{
		--this->uiStackTop;
	}
	else
	{
		previous->DeactivateInput();
		this->uiStack[this->uiStackTop] = ui;
		this->UIstackPeek()->ActivateInput();
	}
}

GameStateUI * GameState::UIstackPop()
{
	GameStateUI *previous = this->UIstackPeek();
	this->UIstackPeek()->DeactivateInput();

	if( this->uiStackTop > 0 )
	{
		--this->uiStackTop;
	}
	else
	{
		this->UIstackClear();
	}

	this->UIstackPeek()->ActivateInput();

	return previous;
}

void GameState::UIstackRemove( GameStateUI *ui )
{
	if( ui == this->UIstackPeek() )
	{
		this->UIstackPop();
	}
	else
	{
		bool isFound = false;
		for( int i = 0; i < this->uiStackTop; ++i )
		{
			if( !isFound && ui == this->uiStack[i] )
			{
				isFound = true;
			}
			if( isFound )
			{
				this->uiStack[i] = this->uiStack[i+1];
			}
		}
		if( isFound )
			--this->uiStackTop;
	}
}

GameStateUI * GameState::UIstackPeek()
{
	return this->uiStack[this->uiStackTop];
}

void GameState::UIstackSet( GameStateUI *ui )
{
	if( ui )
	{
		this->UIstackPeek()->DeactivateInput();

		this->uiStack[0] = ui;
		this->uiStackTop = 0;

		this->UIstackPeek()->ActivateInput();
	}
	else this->UIstackClear();
}

void GameState::UIstackClear()
{
	this->UIstackPeek()->DeactivateInput();

	this->uiStack[0] = this->gameUI;
	this->uiStackTop = 0;

	this->UIstackPeek()->ActivateInput();
}

GameStateUI::UIState GameState::UIstackUpdate( float deltaTime )
{
	GameStateUI::UIState message;
	for( int i = 0; i <= this->uiStackTop; ++i )
	{
		message = uiStack[i]->Update( deltaTime );
	}
	return message;
}

void GameState::UIstackRenderGUI()
{
	for( int i = 0; i <= this->uiStackTop; ++i )
	{
		if( uiStack[i]->HaveGUIRender() )
			uiStack[i]->RenderGUI();
	}
}

void GameState::UIstackRenderText()
{
	for( int i = 0; i <= this->uiStackTop; ++i )
	{
		if( uiStack[i]->HaveTextRender() )
			uiStack[i]->RenderText();
	}
}

/* :HACK! */
// TODO: Fix camera movement on client
/*
void GameState::SetUp( DanBias::Client::C_Player* p)
{
	Float3 up;
	auto it = this->privData->staticObjects->begin();
	for (it; it != this->privData->staticObjects->end(); it++)
	{
		if(it->second->GetGameObjectType() == GameLogic::ObjectSpecialType_World)
		{
			up = - (p->getPos() - it->second->getPos());
			break;
		}
	}
	
	Quaternion newRotation;

	Float3 v1 = this->privData->camera.GetUp();
	Float3 v2(up.x, up.y, up.z);

	Quaternion q;
	Float3 a = v1.Cross(v2);
	
	if (v1.Dot(v2) < -0.999999) 
	{
		Float3 xCrossPre = Float3(1, 0 ,0).Cross(v1);
		if(xCrossPre.GetLength() < 0.000001)
			xCrossPre = Float3(0, 1 ,0).Cross(v1);
		xCrossPre.Normalize();
		
		//q.setRotation(xCrossPre, 3.1415);
	}
	else if (v1.Dot(v2) > 0.999999) 
	{
		   q = Quaternion(Float3(0.0f), 1);
	}
	else
	{
		q.imaginary.x = a.x;
		q.imaginary.y = a.y;
		q.imaginary.z = a.z;

		q.real = (1 + v1.Dot(v2));

		q.Normalize();
	}

	//Get Rotation from matrix
	//float trace = this->privData->camera..v[0].x + trans.v[1].y + trans.v[2].z;
	float trace = trans.v[0].x + trans.v[1].y + trans.v[2].z;

	float temp[4];

	if (trace > float(0.0)) 
	{
		float s = sqrt(trace + float(1.0));
		temp[3]=(s * float(0.5));
		s = float(0.5) / s;

		temp[0]=((trans.v[2].y - trans.v[1].z) * s);
		temp[1]=((trans.v[0].z - trans.v[2].x) * s);
		temp[2]=((trans.v[1].x - trans.v[0].y) * s);
	} 
	else 
	{
		int i = trans.v[0].x < trans.v[1].y ? 
			(trans.v[1].y < trans.v[2].z ? 2 : 1) :
			(trans.v[0].x < trans.v[2].z ? 2 : 0); 
		int j = (i + 1) % 3;  
		int k = (i + 2) % 3;

		float s = sqrt(trans.v[i][i] - trans.v[j][j] - trans.v[k][k] + float(1.0));
		temp[i] = s * float(0.5);
		s = float(0.5) / s;

		temp[3] = (trans.v[k][j] - trans.v[j][k]) * s;
		temp[j] = (trans.v[j][i] + trans.v[i][j]) * s;
		temp[k] = (trans.v[k][i] + trans.v[i][k]) * s;
	}
	Quaternion n = Quaternion(Float3(temp[0],temp[1],temp[2]),temp[3]);

	newRotation = q * n;
	this->privData->camera.SetRotation(newRotation);
}
void GameState::OnMouseMoveVelocity ( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )
{ 
	auto it = this->privData->players.begin();
	for (it; it != this->privData->players.end(); it++)
	{
		if(it->second->GetId() == this->privData->myId)
		{
			this->SetUp(it->second);
			return;
		}
	}
	
}
*/

