#include <Protocols.h>
#include "InGameOptionsUI.h"
#include "DllInterfaces\GFXAPI.h"
#include "Utilities.h"

using namespace ::Oyster;
using namespace ::Oyster::Math;
using namespace ::Oyster::Event;
using namespace ::Oyster::Network;
using namespace ::DanBias::Client;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Utility::StaticArray;
using namespace ::Input;
using namespace ::Input::Enum;


const wchar_t* resolution[] = 
{
	{ L"640x480" }, { L"800x600" }, { L"1024x600" }, { L"1024x768" }, 
	{ L"1152x864" }, { L"1280x720" }, { L"1280x768" }, { L"1280x800" }, 
	{ L"1280x960" }, { L"1280x1024" }, { L"1360x768" }, { L"1366x768" }, 
	{ L"1400x1050" }, { L"1440x900" }, { L"1600x900" }, { L"1600x1200" }, 
	{ L"1680x1050" }, { L"1920x1080" }, { L"1920x1200" }, { L"2048x1152" },
	{ L"2560x1440" }, { L"2560x1600" }
};

void OnButtonInteract_InGame_Exit( Oyster::Event::ButtonEvent<InGameOptionsUI*>& e );
void OnButtonInteract_InGame_Resume( Oyster::Event::ButtonEvent<InGameOptionsUI*>& e );

::std::wstring & Float2ToStr( const Float2 &v, ::std::wstring &targetString )
{
	std::wstring &str = targetString;
	wchar_t buff[10];
	_itow_s((int)v.x, buff, 10); 
	str.append(buff);
	str.append(L"x");
	_itow_s((int)v.y, buff, 10); 
	str.append(buff);

	return str;
}

Float2 & WStrToFloat2( const std::wstring &str, Float2 &targetVector )
{
	Float2 &val = targetVector;
	static std::vector<std::wstring> s;
	Utility::String::Split(s, str, L"x");
	val.x = (Float)_wtof(s[0].c_str());
	val.y = (Float)_wtof(s[1].c_str());
	s.clear();
	return val;
}

InGameOptionsUI::InGameOptionsUI() :
	GameStateUI( nullptr )
{ /* Should never be called! */ }

InGameOptionsUI::InGameOptionsUI( SharedStateContent* shared ) :
	GameStateUI( shared )
{
	this->nextState			= GameStateUI::UIState_same;
	this->render			= false;

	Graphics::API::Option op		= Graphics::API::GetOption();
	this->options.toogleFullScreen	= op.fullscreen;
	this->options.resolution		= op.resolution;
	::std::wstring c; Float2ToStr( this->options.resolution, c );

	for( unsigned int i = 0; i < NumElementsOf(resolution); ++i )
	{
		if( resolution[i] == c )
		{
			this->options.currentRes = i;
			break;
		}
	}
}

InGameOptionsUI::~InGameOptionsUI() {}

void InGameOptionsUI::CreateMouseSlider()
{
	Float4 TextCol = Float4(0.1f,0.1f,0.1f,1.0f);
	Float4 BackCol = Float4(1.0f);
	Float4 HoverCol = Float4(1.2f);
	Float4 PressCol = Float4(1.5f);

	this->mouseSensitivity.pos = Float3(0.5f, 0.354f, 0.6f);
	this->mouseSensitivity.button =  new ButtonRectangle<InGameOptionsUI*>( L"noedge-slider-btn.png", L"", TextCol, BackCol, 
																		HoverCol, PressCol, OnButtonInteract, this, 
																		Float3(this->mouseSensitivity.pos.xy.x, this->mouseSensitivity.pos.xy.y, 0.4f), 
																		Float2(0.016f, 0.029f));
	this->guiElements.AddButton( mouseSensitivity.button );

	mouseSensitivity.button->SetUserData((void*)(int)ButtonType_MouseSensitivity);

	this->mouseSensitivity.mouseSlider = Oyster::Graphics::API::CreateTexture(L"noedge-slider-vertical.png");
	if(!mouseSensitivity.mouseSlider)
	{
		printf("Failed to load texture noedge-slider-vertical.png");
	}

}

bool InGameOptionsUI::Init()
{
	CreateMouseSlider();
	this->mousePos = Float3( 0.01f );

	// create buttons
	ButtonRectangle<InGameOptionsUI*> *button;
	Float4 TextCol	= Float4( 0.1f, 0.1f, 0.1f, 1.0f );
	Float4 BackCol	= Float4( 1.0f );
	Float4 HoverCol	= Float4( 1.2f );
	Float4 PressCol	= Float4( 1.5f );

	HoverCol.w = PressCol.w = 1.0f;
	
	button = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-fScreenOn.png", L"", TextCol, BackCol, HoverCol, PressCol, InGameOptionsUI::OnButtonInteract, this, Float3(0.58f, 0.25f, 0.49f), Float2(0.045f, 0.045f) );
	this->guiElements.AddButton( button );
	button->SetUserData( (int)ButtonType_FullScreen );

	fScreenBtnToggle = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-fScreenOff.png", L"", TextCol, BackCol, HoverCol, PressCol, InGameOptionsUI::OnButtonInteract, this, Float3(0.58f, 0.25f, 0.49f), Float2(0.045f, 0.045f) );
	this->guiElements.AddButton( fScreenBtnToggle );
	fScreenBtnToggle->SetUserData( (int)ButtonType_FullScreen );

	if(this->options.toogleFullScreen)
	{
		fScreenBtnToggle->SetEnabled(false);
	}
	else
	{
		button->SetEnabled(false);
		fScreenBtnToggle = button;
	}

	button = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-flipresLeft.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.38f, 0.48f, 0.49f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResLeft);

	button = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-flipresright.png", L"", TextCol, BackCol, HoverCol, PressCol, OnButtonInteract, this, Float3(0.62f, 0.48f, 0.49f), Float2(0.035f, 0.035f));
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_FlipResRight);

	button = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-apply.png", L"", TextCol, BackCol, HoverCol, PressCol, InGameOptionsUI::OnButtonInteract, this, Float3(0.5f, 0.6f, 0.49f), Float2(0.24f, 0.094f) );
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_Apply);

	button = new ButtonRectangle<InGameOptionsUI*>( L"noedge-btn-back.png", L"", TextCol, BackCol, HoverCol, PressCol, InGameOptionsUI::OnButtonInteract, this, Float3(0.5f, 0.7f, 0.49f), Float2(0.24f, 0.094f) );
	this->guiElements.AddButton( button );
	button->SetUserData((void*)(int)ButtonType_Back);

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->guiElements );

	printf("Init\n");

	return true;
}

GameStateUI::UIState InGameOptionsUI::Update( float deltaTime )
{
	MouseInput mouseState;
	{
		Input::Struct::SAIPointFloat2D p = this->shared->mouseDevice->GetNormalizedPosition();
		this->mousePos.x = mouseState.x = p.x;
		this->mousePos.y = mouseState.y = p.y;
		mouseState.mouseButtonPressed = this->shared->mouseDevice->IsBtnDown(SAMI_MouseLeftBtn);
	}
	EventHandler::Instance().Update( mouseState );

	return this->nextState;
}

bool InGameOptionsUI::HaveGUIRender() const
{
	return this->render; 
}

bool InGameOptionsUI::HaveTextRender() const
{
	return this->render; 
}

void InGameOptionsUI::RenderGUI() 
{
	//Render mouse Slider
	Graphics::API::RenderGuiElement( this->mouseSensitivity.mouseSlider, this->mouseSensitivity.pos, Float2(0.35f, 0.003f), Float4(1.0f) );

	if( this->shared->mouseDevice->IsBtnDown(Input::Enum::SAMI_MouseLeftBtn) )
		Graphics::API::RenderGuiElement( this->shared->mouseCursor, this->mousePos, Float2(0.15f), Float4(1.0f) );
	else
		Graphics::API::RenderGuiElement( this->shared->mouseCursor, this->mousePos, Float2(0.15f, 0.24f), Float4(1.0f) );
	
	Graphics::API::RenderGuiElement( this->shared->background, Float3(0.5f, 0.5f, 0.5f), Float2(0.38f, 0.67f), Float4(1,1,1,0.5) );
	this->guiElements.RenderTexture();
}

void InGameOptionsUI::RenderText() 
{
	Float3 sp = this->mouseSensitivity.pos;
	sp.x -= 0.18f; sp.y += 0.0011f; sp.z = 0.4f;
	Graphics::API::RenderText(L"Mouse sensitivity" , sp, Float2(0.4f, 0.1f), 0.028f);
	
	wchar_t val[25];
	swprintf_s(val, L"%f", this->shared->mouseDevice->GetSensitivity() * 10.0f);
	val[3] = L'\0';
	Float3 t = this->mouseSensitivity.button->GetPosition(); t.y = 0.29f; t.x -= 0.04f;
	Graphics::API::RenderText( val, t, Float2(0.13f, 0.13f), 0.028f);

	Graphics::API::RenderText( L"Full Screen", Float3(0.33f, 0.22f, 0.1f), Float2(0.2f, 0.1f), 0.028f );
	Graphics::API::RenderText(resolution[this->options.currentRes], Float3(0.41f, 0.45f, 0.1f), Float2(0.2f, 0.1f), 0.028f);
	this->guiElements.RenderText();
}

bool InGameOptionsUI::Release()
{
	//Release input devices if have them
	this->DeactivateInput();

	EventHandler::Instance().ReleaseCollection( &this->guiElements );
	return true;
}

void InGameOptionsUI::ChangeState( UIState next )
{
	this->nextState = next;
}

void InGameOptionsUI::ActivateInput()
{
	this->guiElements.SetState(Oyster::Event::EventCollectionState_Enabled);
	this->active = true;
	this->render = true;
	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void InGameOptionsUI::DeactivateInput()
{
	this->guiElements.SetState(Oyster::Event::EventCollectionState_Disabled);
	this->active = false;
	this->render = false;
	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}

void InGameOptionsUI::OnMouseRelease( SAMI key, Mouse* sender )
{
	if(this->mouseSensitivity.isHeld)
	{
		this->mouseSensitivity.isHeld = false;
	}
}

void InGameOptionsUI::OnKeyRelease( SAKI key, Keyboard* sender )
{
	if( key == SAKI_Escape )
	{
		this->ChangeState( UIState_previous );
	}
}
void InGameOptionsUI::OnMouseMoveVelocity	( Input::Struct::SAIPointFloat2D coordinate, Input::Mouse* sender )
{
	if(this->mouseSensitivity.isHeld)
	{
		const float minThreshold = 0.0000001f;
		const float maxVal = 2.0f;

		Float3 temp = this->mouseSensitivity.button->GetPosition();
		temp.x = (sender->GetNormalizedPosition().x);
		
		temp.x = Max((0.5f - (0.35f/2.0f)), temp.x);
		temp.x = Min((0.5f + (0.35f/2.0f)), temp.x);
		this->mouseSensitivity.button->SetPosition(temp ); 

		float vel = this->shared->mouseDevice->GetSensitivity();

		float x = temp.x;
		float width = 0.35f;
		float halfWidth = 0.35f / 2.0f;
		float left = 0.5f - halfWidth;
		float localX = (temp.x - left);
		float value = localX * maxVal / width;
		float real = Max(value, minThreshold);
		
		this->shared->mouseDevice->SetSensitivity( real );
		this->shared->mouseSensitivity = real;
	}
}

void ClientResize(HWND hWnd, int nWidth, int nHeight); //Defined in OpstionsState.cpp

void InGameOptionsUI::OnButtonInteract( ButtonEvent<InGameOptionsUI*>& msg )
{
	ButtonType op = (ButtonType)(int)msg.userData;
	switch( op )
	{
		case DanBias::Client::InGameOptionsUI::ButtonType_MouseSensitivity:
		{
			if(msg.state == ButtonState_Pressed)
			{
				msg.owner->mouseSensitivity.isHeld = true;
			}
		}
		break;
		case InGameOptionsUI::ButtonType_FullScreen:
			if( msg.state == ButtonState_Released )
			{
				msg.owner->options.toogleFullScreen = !msg.owner->options.toogleFullScreen;
				msg.sender->SetEnabled( false );
				msg.owner->fScreenBtnToggle->SetEnabled( true );
				msg.owner->fScreenBtnToggle = (ButtonRectangle<InGameOptionsUI*>*)msg.sender;
			}
		break;
		case InGameOptionsUI::ButtonType_FlipResLeft:
			if( msg.state == ButtonState_Released )
			{
				static const int b = (int)NumElementsOf( resolution );
				int a = ( msg.owner->options.currentRes - 1 );
		
				msg.owner->options.currentRes = a % b;

				if( msg.owner->options.currentRes < 0 )
					msg.owner->options.currentRes = b - 1;
				
				WStrToFloat2( resolution[msg.owner->options.currentRes], msg.owner->options.resolution );
			}
		break;
		case InGameOptionsUI::ButtonType_FlipResRight:
			if( msg.state == ButtonState_Released )
			{
				msg.owner->options.currentRes = ( msg.owner->options.currentRes + 1) % NumElementsOf( resolution );
				WStrToFloat2( resolution[msg.owner->options.currentRes], msg.owner->options.resolution );
			}
		break;
		case InGameOptionsUI::ButtonType_Apply:
			if( msg.state == ButtonState_Released )
			{
				Graphics::API::Option op = Graphics::API::GetOption();

				if(!op.fullscreen)
					ClientResize(WindowShell::GetHWND(), (int)msg.owner->options.resolution.x, (int)msg.owner->options.resolution.y);

				op.fullscreen = msg.owner->options.toogleFullScreen;
				op.resolution = msg.owner->options.resolution;

				Graphics::API::SetOptions( op );
			}
		break;
		case InGameOptionsUI::ButtonType_Back:
			if( msg.state == ButtonState_Released)
			{
				msg.owner->nextState = UIState_previous;
			}
		break;
	}
}