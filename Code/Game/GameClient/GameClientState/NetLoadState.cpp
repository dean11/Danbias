#include "NetLoadState.h"
#include "NetworkClient.h"
#include "OysterMath.h"
#include "Protocols.h"
#include "LevelLoader\LevelLoader.h"
#include "Utilities.h"
#include "C_obj\C_StaticObj.h"
#include "C_obj\C_DynamicObj.h"

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
			{
				ObjectHeader *oh = (ObjectHeader*)oth;

				ModelInitData desc;
				desc.id			= objectID;
				StringToWstring( oh->ModelFile, desc.modelPath );
				desc.position	= oh->position; 
				desc.rotation	= ArrayToQuaternion( oh->rotation );
				desc.scale		= oh->scale;
				desc.visible	= true; 

<<<<<<< HEAD
=======
				// HACK: untill the world is right in lvl format
				if( oh->specialTypeID == ObjectSpecialType_World)
				{
					desc.position	= Float3(0,0,0); 
					desc.rotation	= Quaternion::identity;
					desc.scale		= Float3(300,300,300);
				}

>>>>>>> ed6825a40888474eb1b4a803085fbe4e073812f2
				C_StaticObj *staticObject = new C_StaticObj();
				if( staticObject->Init( desc ) )
				{

					// RB DEBUG
					RBInitData RBData;
					if(oh->boundingVolume.geoType == CollisionGeometryType_Box)
					{
						RBData.position = (Float3)oh->position + (Float3)oh->boundingVolume.box.position;
						RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
						RBData.scale =  (Float3)oh->scale * (Float3)oh->boundingVolume.box.size;
						staticObject->InitRB( RBData );
					}

					if(oh->boundingVolume.geoType == CollisionGeometryType_Sphere)
					{
						RBData.position = (Float3)oh->position + (Float3)oh->boundingVolume.sphere.position;
						RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
						RBData.scale =  (Float3)oh->scale * oh->boundingVolume.sphere.radius;
						staticObject->InitRB( RBData );
					}

					(*this->privData->staticObjects)[objectID] = staticObject;	
				}
				else
				{
					delete staticObject;
				}
			}
			break;
		case ObjectType::ObjectType_Dynamic:
			{
				ObjectHeader *oh = (ObjectHeader*)oth;

				ModelInitData desc;
				desc.id			= objectID;
				StringToWstring( oh->ModelFile, desc.modelPath );
				desc.position	= oh->position; 
				desc.rotation	= ArrayToQuaternion( oh->rotation );
				desc.scale		= oh->scale;
				desc.visible	= true; 
					
				C_DynamicObj *dynamicObject = new C_DynamicObj();
				if( dynamicObject->Init( desc ) )
				{
					// RB DEBUG
					RBInitData RBData;
					if(oh->boundingVolume.geoType == CollisionGeometryType_Box)
					{
						RBData.position = (Float3)oh->position + (Float3)oh->boundingVolume.box.position;
						RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
						RBData.scale =  (Float3)oh->scale * (Float3)oh->boundingVolume.box.size;
						dynamicObject->InitRB( RBData );
					}

					if(oh->boundingVolume.geoType == CollisionGeometryType_Sphere)
					{
						RBData.position = (Float3)oh->position + (Float3)oh->boundingVolume.sphere.position;
						RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
						RBData.scale =  (Float3)oh->scale * oh->boundingVolume.sphere.radius;
						dynamicObject->InitRB( RBData );
					}

					(*this->privData->dynamicObjects)[objectID] = dynamicObject;
				}
				else
				{
					delete dynamicObject;
				}
			}
			break;
		case ObjectType::ObjectType_Light:
			{
				 /* TODO: implement light into the leveformat */
			}
			break;
		default: break;
		}
	}

	this->privData->nextState = ClientState::ClientState_Game;
}
