#include "LanMenuState.h"

#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"

#include "LobbyState.h"
#include "GameState.h"
#include "../Network/NetworkAPI/NetworkClient.h"

#include <Protocols.h>

#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"
#include "Buttons\TextField.h"

#include <GameServerAPI.h>
#include <string>

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Event;
using namespace ::Oyster::Math3D;
using namespace ::GameLogic;

struct  LanMenuState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	InputClass *input;
	Graphics::API::Texture background;
	EventButtonCollection guiElements;

	TextField<LanMenuState*> *connectIP;
	unsigned short connectPort;
} privData;

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e );
void OnButtonInteract_Exit( Oyster::Event::ButtonEvent<LanMenuState*>& e );

LanMenuState::LanMenuState() {}

LanMenuState::~LanMenuState()
{
	if( this->privData )
		this->Release();
}

bool LanMenuState::Init( SharedStateContent &shared )
{
	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = shared.network;
	this->privData->input = shared.input;

	this->privData->background = Graphics::API::CreateTexture( L"color_white.png" );

	// create guiElements
	this->privData->connectIP = new TextField<LanMenuState*>( L"color_white.png", Float4(1.0f), Float4(0.0f), this, Float3(0.5f, 0.3f, 0.5f), Float2(0.8f, 0.09f), ResizeAspectRatio_None );
	this->privData->connectIP->ReserveLines( 1 );
	this->privData->connectIP->AppendText( L"127.0.0.1" );
	//this->privData->connectIP->AppendText( L"194.47.150.206" ); // HACK: connecting to Dennis's server
	this->privData->connectIP->SetFontHeight( 0.08f );
	this->privData->connectIP->SetLineSpacing( 0.005f );
	this->privData->connectIP->SetTopAligned();
	
	this->privData->guiElements.AddButton( this->privData->connectIP );

	ButtonRectangle<LanMenuState*> *guiElements;
	guiElements = new ButtonRectangle<LanMenuState*>( L"color_white.png", L"Connect", Float4(1.0f),Float4(0.0f),Float4(0.0f),Float4(0.0f), OnButtonInteract_Connect, this, Float3(0.5f, 0.4f, 0.5f), Float2(0.3f, 0.05f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	guiElements = new ButtonRectangle<LanMenuState*>( L"color_white.png", L"Exit", Float4(1.0f),Float4(0.0f),Float4(0.0f),Float4(0.0f), OnButtonInteract_Exit, this, Float3(0.5f, 0.5f, 0.5f), Float2(0.3f, 0.05f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	// bind guiElements collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );

	this->privData->connectPort = 15151;

	if(!this->privData->nwClient->StartListeningForBroadcasting(this->privData->connectPort))
	{
		return false;
	}

	return true;
}

GameClientState::ClientState LanMenuState::Update( float deltaTime )
{
	MouseInput mouseState;
	{
		this->privData->input->GetMousePos( mouseState.x, mouseState.y );
		mouseState.mouseButtonPressed = this->privData->input->IsMousePressed();
	}

	EventHandler::Instance().Update( mouseState );

	this->privData->nwClient->Update();

	return this->privData->nextState;
}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float3(0.5f, 0.5f, 1.0f), Float2(1.0f) );
	this->privData->guiElements.RenderTexture();

	Graphics::API::StartTextRender();
	this->privData->guiElements.RenderText();

	Graphics::API::EndFrame();
	return true;
}

bool LanMenuState::Release()
{
	if(privData)
	{
		this->privData->nwClient->StopListeningForBroadcasting();
	}

	privData = NULL;
	return true;
}

void LanMenuState::ChangeState( ClientState next )
{
	switch( next )
	{
	case GameClientState::ClientState_Lobby:
		// attempt to connect to lobby
		if( !this->privData->nwClient->Connect(this->privData->connectPort, (*this->privData->connectIP)[0]) )
			return;
		break;
	default: break;
	}

	this->privData->nextState = next;
}

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Lobby );
		break;
	default: break;
	}
}

void OnButtonInteract_Exit( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Main );
		break;
	default: break;
	}
}

const GameClientState::NetEvent& LanMenuState::DataRecieved( const NetEvent &message )
{
	if( message.args.type == NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend )
	{ // TODO: Reconnect
		const char *breakpoint = "temp trap";
		this->privData->nwClient->Disconnect();
		this->ChangeState( GameClientState::ClientState_Main );
	}

	// fetching the id data.
	short ID = message.args.data.protocol[0].value.netShort;

	CustomNetProtocol data = message.args.data.protocol;

	switch(ID)
	{
	case protocol_Broadcast_Test:
		{
			Protocol_Broadcast_Test decoded(data);

			unsigned short port = decoded.port;
			std::string ip = decoded.ip;
			std::string name = decoded.name;
			printf("Broadcast message: %d: %s: %s\n", port, ip.c_str(), name.c_str());
		}
		break;

	default:
		break;
	}


	return message;
}