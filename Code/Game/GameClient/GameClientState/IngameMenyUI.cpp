#include "IngameMenyUI.h"
#include <Protocols.h>
#include "Utilities.h"
// Debug


using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Input;
using namespace ::Input::Enum;
using namespace ::Oyster::Event;

IngameMenyUI::IngameMenyUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->mouseInput		= nullptr;
	this->mousePos			= Float3( 0.0f );
	this->audioHandler		= nullptr;
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState 		= GameStateUI::UIState_same;
}

IngameMenyUI::IngameMenyUI( SharedStateContent* shared ) :
	GameStateUI()
{
	this->mouseInput = shared->mouseDevice;
	this->mousePos = Float3( 0.0f );
	this->audioHandler = shared->soundManager;
	this->background		= nullptr;
	this->mouseCursor		= nullptr;
	this->nextState			= GameStateUI::UIState_same;
}

IngameMenyUI::~IngameMenyUI() { }
void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );
void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e );

bool IngameMenyUI::Init()
{
	
	// z value should be between 0.1 - 0.5 so that it will be above other states
	// add textures and text
	this->background	= new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f), Float4(0,0,0,0.5));
	this->mouseCursor	= new Plane_UI(L"cursor.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f));
	ButtonRectangle<IngameMenyUI*> *button;
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(0.6f, 0.6f, 0.6f, 1.0f);
	Float4 HoverCol = Float4(0.8f, 0.8f, 0.8f, 1.0f);
	Float4 PressCol = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-quit.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Exit, this, Float3(0.5f, 0.3f, 0.2f), Float2(0.5f, 0.18f));
	menyButtons.AddButton( button );
	button = new ButtonRectangle<IngameMenyUI*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract_InGame_Resume, this, Float3(0.5f, 0.6f, 0.2f), Float2(0.5f, 0.18f));
	menyButtons.AddButton( button );

	// HACK remove this later
	this->debugOutput 	= new Text_UI(L"", Float3(0.2f,0.8f,0.1f), Float2(0.3f,0.1f), 0.05f);

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &menyButtons );

	// setting input mode to all raw
	//this->mouseInput->AddMouseEvent(this);

	return true; 
}
void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(mouse_hoover);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_main_menu );
		// SOUND
		e.owner->PlaySound(mouse_click);
		break;
	default: break;
	}
}
void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<IngameMenyUI*>& e )
{
	switch( e.state )
	{
	case ButtonState_Hover:
		// SOUND
		e.owner->PlaySound(mouse_hoover);
		break;
	case ButtonState_Released:
		e.owner->ChangeState( GameStateUI::UIState_resumeGame );
		// SOUND
		e.owner->PlaySound(mouse_click);
		break;
	default: break;
	}
}

GameStateUI::UIState IngameMenyUI::Update( float deltaTime )
{
	MouseInput mouseState;
	{
		::Input::Struct::SAIPointFloat2D pos;
		this->mouseInput->GetNormalizedPosition( pos );

		this->mousePos.x = mouseState.x = pos.x;
		this->mousePos.y = mouseState.y = pos.y;
		mouseState.mouseButtonPressed = this->mouseInput->IsBtnDown( ::Input::Enum::SAMI_MouseLeftBtn );
	}

	this->debugOutput->setText(std::to_wstring(Oyster::Graphics::API::GetOption().bytesUsed));

	EventHandler::Instance().Update( mouseState );
	return this->nextState;
}

bool IngameMenyUI::HaveGUIRender() const
{
	return true; 
}

bool IngameMenyUI::HaveTextRender() const
{
	return true; 
}

void IngameMenyUI::RenderGUI() 
{
	if(this->mouseInput->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn))
		mouseCursor->RenderTexture( this->mousePos, Float2(0.15f));
	else
		mouseCursor->RenderTexture( this->mousePos, Float2(0.15f, 0.24) );

	this->background->RenderTexture();
	this->menyButtons.RenderTexture();
}

void IngameMenyUI::RenderText() 
{
	this->debugOutput->RenderText();
}

bool IngameMenyUI::Release()
{
	//Release as input event
	this->mouseInput->RemoveMouseEvent(this);

	// TODO: Release UI components here.
	if(this->background) 	delete this->background;
	if(this->mouseCursor) 	delete this->mouseCursor;

	if(this->debugOutput) 	delete this->debugOutput;

	EventHandler::Instance().ReleaseCollection( &this->menyButtons );
	this->mouseInput = 0;
	return true;
}

void IngameMenyUI::ReadKeyInput()
{

}
void IngameMenyUI::ChangeState( UIState next )
{
	this->nextState = next;
}
void IngameMenyUI::PlaySound( SoundID id )
{
	//this->audioHandler->getSound(id)->Play_Sound();
	Sound::AudioAPI::Audio_PlaySound(this->audioHandler->getSound(id), this->audioHandler->getChannel(id), true);
}