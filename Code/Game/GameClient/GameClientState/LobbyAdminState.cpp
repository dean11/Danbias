#include "LobbyAdminState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include <Protocols.h>

#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Event;
using namespace ::Oyster::Math3D;

struct LobbyAdminState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	::Input::Mouse *mouseInput;
	Graphics::API::Texture background;
	EventButtonCollection guiElements;
	C_AudioHandler* soundManager;
} privData;

void OnButtonInteract_Ready( Oyster::Event::ButtonEvent<LobbyAdminState*>& e );

LobbyAdminState::LobbyAdminState(void) {}

LobbyAdminState::~LobbyAdminState(void)
{
	if( this->privData )
		this->Release();
}

bool LobbyAdminState::Init( SharedStateContent &shared )
{
	privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = shared.network;
	this->privData->mouseInput = shared.mouseDevice;

	this->privData->background = Graphics::API::CreateTexture( L"grass_md.png" );

	// create buttons
	ButtonRectangle<LobbyAdminState*> *button;
	
	button = new ButtonRectangle<LobbyAdminState*>( L"earth_md.png", L"Ready", Float4(1.0f),Float4(0.0f),Float4(0.0f),Float4(0.0f), OnButtonInteract_Ready, this, Float3(0.5f, 0.2f, 0.5f), Float2(0.3f, 0.1f), ResizeAspectRatio_Width );
	this->privData->guiElements.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->guiElements );

	// SOUND
	this->privData->soundManager = shared.soundManager;

	return true;
}

GameClientState::ClientState LobbyAdminState::Update( float deltaTime )
{
	MouseInput mouseState;
	{
		::Input::Struct::SAIPointInt2D pos;
		this->privData->mouseInput->GetPixelPosition( pos );

		mouseState.x = pos.x;
		mouseState.y = pos.y;
		mouseState.mouseButtonPressed = this->privData->mouseInput->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}
	EventHandler::Instance().Update( mouseState );

	return this->privData->nextState;
}
bool LobbyAdminState::Render( )
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
bool LobbyAdminState::Release()
{
	privData = NULL;
	return true;
}

void LobbyAdminState::ChangeState( ClientState next )
{
	if( next == GameClientState::ClientState_LobbyReady )
	{ // If all is ready start server

	}
	else
		this->privData->nextState = next;
}
void LobbyAdminState::PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode )
{
	Sound::ISound* sound = this->privData->soundManager->getSound(soundID);
	Sound::IChannel* channel = this->privData->soundManager->getChannel(channelID);

	this->privData->soundManager->PlaySoundOnChannel(sound, channel, playMode);
}
using namespace ::Oyster::Network;

const GameClientState::NetEvent & LobbyAdminState::DataRecieved( const GameClientState::NetEvent &message )
{
	// fetching the id data.
	short ID = message.args.data.protocol[0].value.netShort;
	
	// Block irrelevant messages.
	if( ProtocolIsLobby(ID) )
	{
		switch(ID)
		{
		case protocol_Lobby_CreateGame:		break; /** @todo TODO: implement */
		case protocol_Lobby_StartGame:		break; /** @todo TODO: implement */
		case protocol_Lobby_JoinGame:		break; /** @todo TODO: implement */
		case protocol_Lobby_Login:			break; /** @todo TODO: implement */
		case protocol_Lobby_Refresh:		break; /** @todo TODO: implement */
		case protocol_Lobby_ClientData:		break; /** @todo TODO: implement */
		case protocol_Lobby_GameData:		break; /** @todo TODO: implement */
		default:							break;
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

void OnButtonInteract_Ready( Oyster::Event::ButtonEvent<LobbyAdminState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Hover, ChannelID_Mouse_Hover_Button1, PlayMode_Restart);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_LobbyReady );
		// SOUND
		e.owner->PlaySound(SoundID_Mouse_Click, ChannelID_Mouse_Click_Button1, PlayMode_Restart);
		break;
	default: break;
	}
}