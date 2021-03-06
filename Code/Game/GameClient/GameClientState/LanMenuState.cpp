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
	::Input::Mouse *mouseInput;
	::Input::Keyboard *keyboardInput;
	Float3 mousePos;
	Graphics::API::Texture background, mouseCursor;
	EventButtonCollection guiElements;

	TextField<LanMenuState*> *connectIP;
	unsigned short connectPort;

	std::string ip;

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
	this->privData->mouseInput = shared.mouseDevice;
	this->privData->keyboardInput = shared.keyboardDevice;

	this->privData->background = Graphics::API::CreateTexture( L"color_white.png" );
	this->privData->mouseCursor = Graphics::API::CreateTexture( L"cursor.png" );

	// create guiElements
	this->privData->connectIP = new TextField<LanMenuState*>( L"noedge-btn-ipfield.png", Float4(1.0f), Float4(1.0f), this, Float3(0.5f, 0.2f, 0.9f), Float2(0.5f, 0.05f), ResizeAspectRatio_Height );
	this->privData->connectIP->ReserveLines( 1 );
	this->privData->connectIP->AppendText( L"127.0.0.1" );
	//this->privData->connectIP->AppendText( L"194.47.150.206" ); // HACK: connecting to Dennis's server
	//this->privData->connectIP->AppendText( L"194.47.150.189" ); // HACK: connecting to Robins server
	this->privData->connectIP->SetFontHeight( 0.035f );
	this->privData->connectIP->SetLineSpacing( 0.005f );
	this->privData->connectIP->SetBottomAligned();
	
	this->privData->guiElements.AddButton( this->privData->connectIP );

	ButtonRectangle<LanMenuState*> *guiElements;
	guiElements = new ButtonRectangle<LanMenuState*>( L"noedge-btn-join.png", L"", Float4(1.0f),Float4(1.0f),Float4(1.2f),Float4(1.5f), OnButtonInteract_Connect, this, Float3(0.5f, 0.4f, 0.5f), Float2(0.5f, 0.18f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	guiElements = new ButtonRectangle<LanMenuState*>( L"noedge-btn-back.png", L"", Float4(1.0f),Float4(1.0f),Float4(1.2f),Float4(1.5f), OnButtonInteract_Exit, this, Float3(0.5f, 0.8f, 0.5f), Float2(0.5f, 0.18f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	// bind guiElements collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );

	this->privData->connectPort = 15151;

	this->privData->keyboardInput->BindTextTarget( &(*this->privData->connectIP)[0] );
	this->privData->keyboardInput->Activate();

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
		::Input::Struct::SAIPointFloat2D pos;
		this->privData->mouseInput->GetNormalizedPosition( pos );

		this->privData->mousePos.x = mouseState.x = pos.x;
		this->privData->mousePos.y = mouseState.y = pos.y;
		mouseState.mouseButtonPressed = this->privData->mouseInput->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}
	EventHandler::Instance().Update( mouseState );

	this->privData->nwClient->Update();

	return this->privData->nextState;
}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->mouseCursor, this->privData->mousePos, Float2(0.15f, 0.24), Float4(1.0f) );
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
	case GameClientState::ClientState_NetLoad:
		// attempt to connect to lobby
		if( !this->privData->nwClient->Connect(this->privData->connectPort, (*this->privData->connectIP)[0]) )
			return;
		break;
	default: break;
	}

	this->privData->keyboardInput->ReleaseTextTarget();

	this->privData->nextState = next;
}

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_NetLoad );
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

			//this->privData->connectPort = port;
			//this->privData->ip = ip;
		}
		break;

	default:
		break;
	}


	return message;
}