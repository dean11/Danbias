#include "LobbyState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include <Protocols.h>

using namespace DanBias::Client;

struct  LobbyState::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	C_Object* object[2]; 
	int modelCount; 
	// UI object
	// game client* 
} privData;

LobbyState::LobbyState(void) {}

LobbyState::~LobbyState(void) {}

bool LobbyState::Init(Oyster::Network::NetworkClient* nwClient)
{
	privData = new myData();
	this->nwClient = nwClient;
	// load models
	LoadModels(L"UImodels.txt");
	InitCamera(Oyster::Math::Float3(0,0,5.4f));
	return true;
}

bool LobbyState::LoadModels(std::wstring file)
{
	Oyster::Graphics::Definitions::Pointlight plight;
	plight.Pos = Oyster::Math::Float3(-2,3,0);
	plight.Color = Oyster::Math::Float3(0,1,0);
	plight.Radius = 10;
	plight.Bright = 1;
	Oyster::Graphics::API::AddLight(plight);
	// open file
	// read file 
	// init models
	privData->modelCount = 2;

	ModelInitData modelData;

	modelData.position = Oyster::Math::Float3(0,0,0);
	modelData.rotation = Oyster::Math::Quaternion::identity;
	modelData.scale =  Oyster::Math::Float3(1,1,1);
	modelData.visible = true;
	modelData.modelPath = L"crate_colonists.dan";
	// load models
	privData->object[0] = new C_StaticObj();
	privData->object[0]->Init(modelData);

	modelData.position = Oyster::Math::Float3(2,2,2);

	privData->object[1] = new C_StaticObj();
	privData->object[1]->Init(modelData);
	return true;
}

bool LobbyState::InitCamera(Oyster::Math::Float3 startPos)
{
	privData->proj = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);
	//privData->proj = Oyster::Math3D::ProjectionMatrix_Orthographic(1024, 768, 1, 1000);
	Oyster::Graphics::API::SetProjection(privData->proj);

	privData->view = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),startPos);
	privData->view = Oyster::Math3D::InverseOrientationMatrix(privData->view);
	return true;
}
GameClientState::ClientState LobbyState::Update(float deltaTime, InputClass* KeyInput)
{
	// picking 
	// mouse events
	// different menus
	// play sounds
	// update animation
	// send data to server
	// check data from server

	if(GameServerAPI::ServerIsRunning() && GameServerAPI::ServerIsRunning())	//May be a problem if server is not shut down properly after lan session.
	{
		if( KeyInput->IsKeyPressed(DIK_G)) 
		{
			if(!DanBias::GameServerAPI::GameStart())
			{
				
			}
		}
	}
	  
	return ClientState_Same;
}
bool LobbyState::Render()
{

	Oyster::Graphics::API::SetView(privData->view);
	Oyster::Graphics::API::SetProjection( privData->proj);


	Oyster::Graphics::API::NewFrame();
	// render objects
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Render();
	}

	// render effects

	// render lights

	Oyster::Graphics::API::EndFrame();
	return true;
}
bool LobbyState::Release()
{
	for (int i = 0; i < privData->modelCount; i++)
	{
		privData->object[i]->Release();
		delete privData->object[i];
		privData->object[i] = NULL;
	}

	delete privData;  
	privData = NULL;
	return true;
}

using namespace ::Oyster::Network;

void LobbyState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	CustomNetProtocol data = e.args.data.protocol;
	short ID = data[0].value.netShort; // fetching the id data.
	
	// Block irrelevant messages.
	if( ProtocolIsLobby(ID) )
	{
		switch(ID)
		{
		case protocol_Lobby_Create:		break; /** @todo TODO: implement */
		case protocol_Lobby_Start:		break; /** @todo TODO: implement */
		case protocol_Lobby_Join:		break; /** @todo TODO: implement */
		case protocol_Lobby_Login:		break; /** @todo TODO: implement */
		case protocol_Lobby_Refresh:	break; /** @todo TODO: implement */
		case protocol_Lobby_ClientData:	break; /** @todo TODO: implement */
		case protocol_Lobby_GameData:	break; /** @todo TODO: implement */
		default:						break;
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