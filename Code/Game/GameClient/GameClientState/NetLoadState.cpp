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
	::std::map<int, ::Utility::DynamicMemory::UniquePointer<::DanBias::Client::C_ClientLogic>> *clientObjects;

	FirstPersonWeapon* weapon;
	C_AudioHandler* soundManager;
	
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
	this->privData->clientObjects = &shared.clientObjects;
	

	shared.weapon = new FirstPersonWeapon;
	this->privData->weapon			= shared.weapon;

	this->privData->loading = false;

	// we may assume that nwClient is properly connected to the server
	// signals querry to server for loading instructions
	this->privData->nwClient->Send( Protocol_QuerryGameType() );

	// SOUND
	this->privData->soundManager = shared.soundManager;

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
	}
	return message;
}

void NetLoadState::LoadGame( const ::std::string &fileName )
{
	this->privData->loading = true;

	// Load sounds, this should probably depend on the map later
	LoadSound();

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
	desc.modelPath	= L"wpn_massdriver_low.dan";
	desc.position	= Float3( -14.0f, 196.0f, 0.0f ); 
	desc.rotation	= Quaternion::identity;
	desc.scale		= Float3(1.0f);
	desc.visible	= false;
	desc.tint		= Float3(1.0f);
	desc.gtint		= Float3(1.0f);

	for( int i = 0; i < 10; ++i )
	{
		C_Player *w = new C_Player();
		w->Init( desc );
		//w->playAnimation( L"idle", true );
		(*this->privData->weapons)[i] = w;
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
			desc.gtint = Float3(1.0f);
			Graphics::Definitions::Pointlight pointLight; 

			pointLight.Color	= Float3(1.0f, 0.3f, 0.0f);
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
			desc.gtint = Float3(1.0f);
			desc.tint = Float3(1.0f);
			C_ClientLogic *logic = new C_ClientLogic();
			logic->Init(desc, new C_ClientLogic::UpdateRotation(logic), -Math::pi, Math::Vector3(0.0f,1.0f,0.0f));
			(*this->privData->clientObjects)[ID] = logic;
			return;
		}
	case ObjectSpecialType::ObjectSpecialType_StandardBox:
		{
			desc.tint = Float3(1.0f);
			desc.gtint = Float3(1.0f);
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
			desc.gtint *= 2;
			Graphics::Definitions::Pointlight pointLight; 

			pointLight.Color	= desc.gtint;
			pointLight.Pos		= desc.position;
			pointLight.Bright	= 0.5f;
			pointLight.Radius	= 5.0f; 

			C_Light *newLight = new C_Light( pointLight, ID );

			(*this->privData->lights)[ID] = newLight;
			break;
		}
	case ObjectSpecialType::ObjectSpecialType_LightSource:
		{
			C_ClientLogic *logic = new C_ClientLogic();
			Graphics::Definitions::Pointlight pointLight; 
			desc.tint = Float3(1.0f);
			desc.gtint = Float3(1.0f);
			pointLight.Color	= Float3(0.0f, 1.0f, 1.0f);
			pointLight.Pos		= desc.position + desc.position.GetNormalized()*5;
			pointLight.Bright	= ((float)rand() / (RAND_MAX + 1) * (2 - 0) + 0.5f);
			pointLight.Radius	= ((float)rand() / (RAND_MAX + 1) * (30 - 8) + 5);
			logic->Init(desc, new C_ClientLogic::UpdateLight(logic), pointLight);
			(*this->privData->clientObjects)[ID] = logic;
			return;
		}
	case ObjectSpecialType::ObjectSpecialType_NonSolidRotationQuick:
		{
			desc.tint = Float3(1.0f);
			desc.gtint = Float3(1.0f);
			C_ClientLogic *logic = new C_ClientLogic();
			logic->Init(desc, new C_ClientLogic::UpdateRotation(logic), Math::pi/2, Math::Vector3(1.0f,0.0f,0.0f));
			(*this->privData->clientObjects)[ID] = logic;
			return;
		}
	case ObjectSpecialType::ObjectSpecialType_NonSolidRotationSlow:
		{
			desc.tint = Float3(1.0f);
			desc.gtint = Float3(1.0f);
			C_ClientLogic *logic = new C_ClientLogic();
			logic->Init(desc, new C_ClientLogic::UpdateRotation(logic), Math::pi/64, Math::Vector3(1.0f,0.0f,0.0f));
			(*this->privData->clientObjects)[ID] = logic;
			return;
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
			if (oh->specialTypeID == ObjectSpecialType_JumpPad)
			{
				//// sound settings first
				//this->privData->soundManager->getSound(jumppad)->setMode(Sound::Loop_normal);
				//// play sound
				//this->privData->soundManager->getSound(jumppad)->Play_Sound(true);
				//// channel settings
				//this->privData->soundManager->getSound(jumppad)->setChannel3DAttributes(object->getPos(), Float3(1,0,0));
				//this->privData->soundManager->getSound(jumppad)->setMinMaxDistance(1, 10000);
				// unpause to play. 
			}
			if(oh->specialTypeID == ObjectSpecialType_PickupHealth || oh->specialTypeID == ObjectSpecialType_PickupEnergy)
			{
				(*this->privData->pickups)[ID] = (C_StaticObj*)object;
				// ambient sound
				//this->privData->soundManager->getSound(SoundID_Pickup_Music)->setMode(Sound::Loop_normal);
				//Sound::AudioAPI::Audio_PlaySound(this->privData->soundManager->getSound(SoundID_Pickup_Music), this->privData->soundManager->getChannel(ChannelID_pickUpSound), true);
				//this->privData->soundManager->getChannel(ChannelID_pickUpSound)->setChannel3DAttributes(object->getPos(), Float3(1,0,0));
				//this->privData->soundManager->getChannel(ChannelID_pickUpSound)->setMinMaxDistance(1, 100000);
				//this->privData->soundManager->getChannel(ChannelID_pickUpSound)->addChannelToGroup(this->privData->soundManager->getChannelGroup(ChannelGroup_GameMusic));
				//this->privData->soundManager->getChannel(ChannelID_pickUpSound)->setVolym( 0.5);
				//this->privData->soundManager->getChannel(ChannelID_pickUpSound)->setSoundVolume();
				//this->privData->soundManager->getChannelGroup(ChannelGroup_GameMusic)->setVolym(0.9);
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
void NetLoadState::LoadSound()
{
	this->privData->soundManager->getChannel(ChannelID_Menu_Soundtrack)->SetPauseChannel(true);

	// Game music
	this->privData->soundManager->addMusic(SoundDesc("Energy01.mp3",		SoundID_Game_SoundTrack)); // !
	this->privData->soundManager->addMusic(SoundDesc("industrial.mp3",		SoundID_Game_GameOver)); 

	// event sounds, using collision channels
	this->privData->soundManager->addSFX_3D(SoundDesc("scifi2.mp3",			SoundID_PickUpHealth));
	this->privData->soundManager->addSFX_3D(SoundDesc("JumpPad01.mp3",		SoundID_Jumppad));
	this->privData->soundManager->addSFX_3D(SoundDesc("Collision02.mp3",	SoundID_CrystalCollision));
	this->privData->soundManager->addSFX_3D(SoundDesc("explosion3.mp3",		SoundID_CrateExplosion));

	//this->privData->soundManager->addSFX_3D(SoundDesc("energy_pickup.wav",	SoundID_BoxVsBoxCollision));
	//this->privData->soundManager->addSFX_3D(SoundDesc("energy_pickup.wav",	SoundID_BoxVsPlayerCollision));
	this->privData->soundManager->addSFX_3D(SoundDesc("teleport.mp3",			SoundID_Portal));

	// player sounds, using player channels
	this->privData->soundManager->addSFX_3D(SoundDesc("Running01.mp3",			SoundID_Player_Walk));
	this->privData->soundManager->addSFX_3D(SoundDesc("Jump01.mp3",				SoundID_Player_Jump));
	this->privData->soundManager->addSFX_3D(SoundDesc("Energy01.mp3",			SoundID_Player_WeaponPull));
	this->privData->soundManager->addSFX_3D(SoundDesc("Energy02.mp3",			SoundID_Player_WeaponPush));
	this->privData->soundManager->addSFX_3D(SoundDesc("weaponShot.mp3",			SoundID_Player_WeaponShoot));
	this->privData->soundManager->addSFX_3D(SoundDesc("Skada1.wav",				SoundID_Player_DMGtaken));
	//this->privData->soundManager->addSFX_3D(SoundDesc("jaguar.wav",			SoundID_Player_Heal));
	this->privData->soundManager->addSFX_3D(SoundDesc("spawn.wav",				SoundID_Player_Respawn)); 
	this->privData->soundManager->addSFX_3D(SoundDesc("gameOver.mp3",			SoundID_Player_Die));
	//this->privData->soundManager->addSFX_3D(SoundDesc("jaguar.wav",			SoundID_Player_Join));

	// ambient sounds
	//this->privData->soundManager->addSFX_3D(SoundDesc("level_background.mp3", SoundID_Pickup_Music, ChannelID_pickUpSound ));
	//this->privData->soundManager->addSFX_3D(SoundDesc("1978-039 Kansas - Dust In The Wind.mp3", ambient, ChannelID_ambient));
	
	// Background music 
	//this->privData->soundManager->getSound(SoundID_Game_SoundTrack)->setMode(Sound::Loop_normal);
	//Sound::AudioAPI::Audio_PlaySound(this->privData->soundManager->getSound(SoundID_Game_SoundTrack), this->privData->soundManager->getChannel(ChannelID_Game_Soundtrack), true);
	
	this->privData->soundManager->getSound(SoundID_Player_Walk)->setMode(Sound::Loop_normal);
}

