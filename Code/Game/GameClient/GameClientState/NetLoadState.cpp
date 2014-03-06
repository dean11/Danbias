#include "NetLoadState.h"
#include "NetworkClient.h"
#include "OysterMath.h"
#include "Protocols.h"
#include "LevelLoader.h"
#include "Utilities.h"
#include "C_obj\C_StaticObj.h"
#include "C_obj\C_DynamicObj.h"
#include "C_Light.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Math;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::String;

struct NetLoadState::MyData
{
	MyData() {}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	Graphics::API::Texture background;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *staticObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_DynamicObj>> *dynamicObjects;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Light>> *lights;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_StaticObj>> *pickups;
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_Player>> *weapons;

	FirstPersonWeapon* weapon;

	bool loading;
};

inline Quaternion ArrayToQuaternion( const float source[4] )
{
	return Quaternion( Float3(source[0], source[1], source[2]), source[3] );
}

NetLoadState::NetLoadState(void) {}

NetLoadState::~NetLoadState(void)
{
	if( this->privData )
		this->Release();
}

bool NetLoadState::Init( SharedStateContent &shared )
{
	this->privData = new MyData();

	this->privData->nextState		= GameClientState::ClientState_Same;
	this->privData->nwClient		= shared.network;
	this->privData->background		= Graphics::API::CreateTexture( L"grass_md.png" );
	this->privData->dynamicObjects	= &shared.dynamicObjects;
	this->privData->staticObjects	= &shared.staticObjects;
	this->privData->lights			= &shared.lights;
	this->privData->pickups			= &shared.pickups;
	this->privData->weapons			= &shared.weapons;

	shared.weapon = new FirstPersonWeapon;
	this->privData->weapon			= shared.weapon;

	this->privData->loading = false;

	// we may assume that nwClient is properly connected to the server
	// signals querry to server for loading instructions
	this->privData->nwClient->Send( Protocol_QuerryGameType() );

	return true;
}

GameClientState::ClientState NetLoadState::Update( float deltaTime )
{
	return this->privData->nextState;
}

bool NetLoadState::Render()
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float3(0.5f, 0.5f, 1.0f), Float2(1.0f) );

	Graphics::API::EndFrame();

	return true;
}

bool NetLoadState::Release()
{
	if( this->privData )
	{
		this->privData = NULL;
	}
	return true;
}

void NetLoadState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

const GameClientState::NetEvent & NetLoadState::DataRecieved( const GameClientState::NetEvent &message )
{
	// fetching the id data.
	short ID = message.args.data.protocol[0].value.netShort;
	
	if( ID == protocol_Lobby_CreateGame )
	{
		if( !this->privData->loading )
		{
			this->LoadGame( Protocol_LobbyCreateGame(message.args.data.protocol).mapName );
			this->ChangeState( ClientState_Game );
			this->privData->loading = false;
		}
		return GameClientState::event_processed;
	}
	else
	{ // HACK: Debug trap
		const char *breakPoint = "Being greedy.";
		return message;
	}
}

void NetLoadState::LoadGame( const ::std::string &fileName )
{
	this->privData->loading = true;

	LevelLoader loader( "..\\Content\\Worlds\\" );
	auto objects = loader.LoadLevel( fileName );
	auto object = objects.begin();
	ObjectTypeHeader *oth;

	int objectID = 100; // first 100 is reserved for players. This is how the server does it.

	for( ; object != objects.end(); ++object )
	{
		++objectID;
		oth = (ObjectTypeHeader*)(*object._Ptr);
		switch( oth->typeID )
		{
		case ObjectType::ObjectType_Static:
		case ObjectType::ObjectType_Dynamic:
			{
				LoadObject(oth,objectID);
			}
			break;
		case ObjectType::ObjectType_Light:
			{
				BasicLight *light = (BasicLight*)oth;
				Graphics::Definitions::Pointlight pointLight; 

				pointLight.Color	= light->color;
				pointLight.Pos		= light->position;
				pointLight.Bright	= light->intensity;
				pointLight.Radius	= light->radius; 

				C_Light *newLight = new C_Light( pointLight, objectID );

				(*this->privData->lights)[objectID] = newLight;
			}
			break;
		default: break;
		}
	}

	//Load ten weapons
	ModelInitData desc;
	desc.id			= 0;
	StringToWstring("wpn_massdriver_low.dan", desc.modelPath );
	desc.position	= Float3(-14, 196, 0); 
	desc.rotation	= Quaternion(Float3(0, 0, 0), 1);
	desc.scale		= Float3(1, 1, 1);
	desc.visible	= false;
	desc.tint = Float3(1.0f);
	desc.gtint = Float3(1.0f);

	for(int i = 0; i < 10; i++)
	{
		(*this->privData->weapons)[i] = new C_Player();

		(*this->privData->weapons)[i]->Init(desc);
		//(*this->privData->weapons)[i]->playAnimation(L"idle", true); //This animation is currently broken and will make you enter an endless loop.
	}


	this->privData->weapon->Init();

	Graphics::API::EndLoadingModels();

	this->privData->nextState = ClientState::ClientState_Game;
}

void NetLoadState::LoadObject( ObjectTypeHeader* oth, int ID)
{
	ObjectHeader *oh = (ObjectHeader*)oth;

	ModelInitData desc;
	desc.id			= ID;
	StringToWstring( oh->ModelFile, desc.modelPath );
	desc.position	= oh->position; 
	desc.rotation	= ArrayToQuaternion( oh->rotation );
	desc.scale		= oh->scale;
	desc.visible	= true;

	int light = -1;

	switch (oh->specialTypeID)
	{
	case ObjectSpecialType::ObjectSpecialType_RedExplosiveBox:
		{
			desc.tint = Float3(1.0f);
			desc.gtint = Float3(1.0f, 0.0f, 0.0f);

			Graphics::Definitions::Pointlight pointLight; 

			pointLight.Color	= desc.gtint;
			pointLight.Pos		= desc.position;
			pointLight.Bright	= 2.0f;
			pointLight.Radius	= 5.0f; 

			C_Light *newLight = new C_Light( pointLight, ID );
			light = ID;
			(*this->privData->lights)[ID] = newLight;
			break;
		}
	case ObjectSpecialType::ObjectSpecialType_Portal:
		{
			desc.tint = Float3(0.0f, 0.0f, 1.0f);
			desc.gtint = Float3(1.0f, 1.0f, 1.0f);
			break;
		}
	case ObjectSpecialType::ObjectSpecialType_StandardBox:
		{
			desc.tint = Float3(1.0f);
			if(desc.modelPath == L"crate_colonists.dan")
			{
				desc.gtint = Float3(
					((float)rand() / (RAND_MAX + 1) * (1 - 0.5f) + 0),
					((float)rand() / (RAND_MAX + 1) * (1 - 0) + 0.5f),
					((float)rand() / (RAND_MAX + 1) * (1 - 0) + 0)
					).Normalize();
			}
			else
			{
				desc.gtint = Float3(
					((float)rand() / (RAND_MAX + 1) * (1 - 0.5f) + 0),
					((float)rand() / (RAND_MAX + 1) * (1 - 0) + 0),
					((float)rand() / (RAND_MAX + 1) * (1 - 0.) + 0.5f)
					).Normalize();
			}
			light = ID;

			Graphics::Definitions::Pointlight pointLight; 

			pointLight.Color	= desc.gtint;
			pointLight.Pos		= desc.position;
			pointLight.Bright	= 3;
			pointLight.Radius	= 2.0f; 

			C_Light *newLight = new C_Light( pointLight, ID );

			(*this->privData->lights)[ID] = newLight;
			break;
		}

	default:
		desc.tint = Float3(1.0f);
		desc.gtint = Float3(1.0f);
		break;
	}

	C_Object* object = nullptr;

	if(oth->typeID == ObjectType::ObjectType_Static)
	{
		object = new C_StaticObj(oh->specialTypeID);
	}
	else
	{
		if(oth->typeID == ObjectType::ObjectType_Dynamic)
		{
			object = new C_DynamicObj();
		}
	}
	if(object)
	{
		if(object->Init(desc))
		{
			object->SetLight(light);
			// RB DEBUG
			RBInitData RBData;
			if(oh->boundingVolume.geoType == CollisionGeometryType_Box)
			{
				RBData.position = ((Float3)oh->position + (Float3)oh->boundingVolume.box.position) * (Float3)oh->scale;
				RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
				RBData.scale =  (Float3)oh->scale * (Float3)oh->boundingVolume.box.size * 2;
				RBData.type = RB_Type_Cube;
				object->InitRB( RBData );
			}

			if(oh->boundingVolume.geoType == CollisionGeometryType_Sphere)
			{
				RBData.position = ((Float3)oh->position + (Float3)oh->boundingVolume.box.position) * (Float3)oh->scale;
				RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
				RBData.scale =  (Float3)oh->scale * oh->boundingVolume.sphere.radius * 2;
				RBData.type = RB_Type_Sphere;
				object->InitRB( RBData );
			}
			// !RB DEBUG 

			if(oh->specialTypeID == ObjectSpecialType_PickupHealth || oh->specialTypeID == ObjectSpecialType_PickupEnergy)
			{
				(*this->privData->pickups)[ID] = (C_StaticObj*)object;
			}
			else if(oth->typeID == ObjectType::ObjectType_Static)
			{
				(*this->privData->staticObjects)[ID] = (C_StaticObj*)object;
			}
			else
			{
				if(oth->typeID == ObjectType::ObjectType_Dynamic)
				{
					(*this->privData->dynamicObjects)[ID] = (C_DynamicObj*)object;
				}
			}
				
		}
		else
		{
			delete object;
		}
	}
}
