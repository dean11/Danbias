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
	MyData()
	{
		this->currentText = 0;
	}

	SharedStateContent *sharedData;

	GameClientState::ClientState nextState;
	Float3 mousePos;
	EventButtonCollection guiElements;

	TextField<LanMenuState*> *connectIP;
	TextField<LanMenuState*> *alias;
	Utility::DynamicMemory::DynamicArray<TextField<LanMenuState*>*> textsRefs;
	int currentText;

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

	this->privData->sharedData = &shared;
	this->privData->nextState = GameClientState::ClientState_Same;

	// create guiElements
	this->privData->connectIP = new TextField<LanMenuState*>( L"noedge-btn-ipfield.png", Float4(1.0f), Float4(1.0f), this, Float3(0.5f, 0.2f, 0.9f), Float2(0.5f, 0.05f), ResizeAspectRatio_Height );
	this->privData->connectIP->ReserveLines( 1 );
	this->privData->connectIP->AppendText( L"127.0.0.1:15151" );
	this->privData->connectIP->SetFontHeight( 0.035f );
	this->privData->connectIP->SetLineSpacing( 0.005f );
	this->privData->connectIP->SetBottomAligned();

	this->privData->alias = new TextField<LanMenuState*>( L"noedge-btn-ipfield.png", Float4(1.0f), Float4(1.0f), this, Float3(0.5f, 0.35f, 0.9f), Float2(0.5f, 0.05f), ResizeAspectRatio_Height );
	this->privData->alias->ReserveLines( 1 );
	this->privData->alias->AppendText( L"Player" );
	this->privData->alias->SetFontHeight( 0.035f );
	this->privData->alias->SetLineSpacing( 0.005f );
	this->privData->alias->SetBottomAligned();

	this->privData->textsRefs.Push(this->privData->alias);
	this->privData->guiElements.AddButton( this->privData->alias );

	this->privData->textsRefs.Push(this->privData->connectIP);
	this->privData->guiElements.AddButton( this->privData->connectIP );
	this->privData->sharedData->keyboardDevice->BindTextTarget( &(*this->privData->connectIP)[0] );

	Float4 bg = this->privData->connectIP->GetBackColor(); bg.w = 1.6f;
	this->privData->connectIP->SetBackColor(bg);

	ButtonRectangle<LanMenuState*> *guiElements;
	guiElements = new ButtonRectangle<LanMenuState*>( L"noedge-btn-join.png", L"", Float4(1.0f),Float4(1.0f),Float4(1.2f),Float4(1.5f), OnButtonInteract_Connect, this, Float3(0.5f, 0.5f, 0.5f), Float2(0.5f, 0.18f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	guiElements = new ButtonRectangle<LanMenuState*>( L"noedge-btn-back.png", L"", Float4(1.0f),Float4(1.0f),Float4(1.2f),Float4(1.5f), OnButtonInteract_Exit, this, Float3(0.5f, 0.8f, 0.5f), Float2(0.5f, 0.18f), ResizeAspectRatio_None );
	this->privData->guiElements.AddButton( guiElements );

	// bind guiElements collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );

	this->privData->connectPort = 15151;

	this->privData->sharedData->keyboardDevice->Activate();
	this->privData->sharedData->keyboardDevice->AddKeyboardEvent(this);

	if(!this->privData->sharedData->network->StartListeningForBroadcasting(this->privData->connectPort))
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
		this->privData->sharedData->mouseDevice->GetNormalizedPosition( pos );

		this->privData->mousePos.x = mouseState.x = pos.x;
		this->privData->mousePos.y = mouseState.y = pos.y;
		mouseState.mouseButtonPressed = this->privData->sharedData->mouseDevice->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}
	EventHandler::Instance().Update( mouseState );

	this->privData->sharedData->network->Update();

	return this->privData->nextState;
}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->sharedData->mouseCursor, this->privData->mousePos, Float2(0.15f, 0.24), Float4(1.0f) );
	Graphics::API::RenderGuiElement( this->privData->sharedData->background, Float3(0.5f, 0.5f, 1.0f), Float2(1.0f) );
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
		this->privData->sharedData->network->StopListeningForBroadcasting();
	}

	privData = NULL;
	return true;
}

void LanMenuState::ChangeState( ClientState next )
{
	switch( next )
	{
	case GameClientState::ClientState_NetLoad:
	{
		std::wstring spl;
		std::vector<std::wstring> s;
		Utility::String::Split(s, (*this->privData->connectIP)[0], L":");

		if(s.size() != 2)
			return;

		this->privData->connectPort = _wtoi(s[1].c_str());

		// attempt to connect to lobby
		if( !this->privData->sharedData->network->Connect(this->privData->connectPort, s[0] ) )
			return;

		std::string al;
		Utility::String::WStringToString((*this->privData->alias)[0], al);

		if(al.size() == 0 ) al = "Player";

		this->privData->sharedData->network->Send(GameLogic::Protocol_LobbyJoin(al));
	} break;

	default: break;
	}

	this->privData->sharedData->keyboardDevice->RemoveKeyboardEvent(this);
	this->privData->sharedData->keyboardDevice->ReleaseTextTarget();

	this->privData->nextState = next;
}
void LanMenuState::PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode )
{
	Sound::ISound* sound = this->privData->sharedData->soundManager->getSound(soundID);
	Sound::IChannel* channel = this->privData->sharedData->soundManager->getChannel(channelID);
	
	this->privData->sharedData->soundManager->PlaySoundOnChannel(sound, channel, playMode);
}

void OnButtonInteract_Connect( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button1, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button1, PlayMode_Restart);
		e.owner->ChangeState( GameClientState::ClientState_NetLoad );
		break;
	default: break;
	}
}

void OnButtonInteract_Exit( Oyster::Event::ButtonEvent<LanMenuState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button2, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_Main );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button2, PlayMode_Restart);
		break;
	default: break;
	}
}

const GameClientState::NetEvent& LanMenuState::DataRecieved( const NetEvent &message )
{
	if( message.args.type == NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend )
	{ // TODO: Reconnect
		const char *breakpoint = "temp trap";
		this->privData->sharedData->network->Disconnect();
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

void LanMenuState::OnKeyPress(Input::Enum::SAKI key, Input::Keyboard* sender) 
{
	if(key == Input::Enum::SAKI_Tab)
	{
		
		Float4 bg = this->privData->textsRefs[this->privData->currentText]->GetBackColor();
		bg.w = 1.0f;
		this->privData->textsRefs[this->privData->currentText]->SetBackColor(bg);


		this->privData->currentText = (this->privData->currentText + 1) % this->privData->textsRefs.Size();
		this->privData->sharedData->keyboardDevice->BindTextTarget(	&(*this->privData->textsRefs[this->privData->currentText])[0] );


		bg = this->privData->textsRefs[this->privData->currentText]->GetBackColor();
		bg.w = 1.6f;
		this->privData->textsRefs[this->privData->currentText]->SetBackColor(bg);
	}
	else if (key == Input::Enum::SAKI_Enter || key == Input::Enum::SAKI_NumpadEnter)
	{
		this->ChangeState( GameClientState::ClientState_NetLoad );
	}
}



