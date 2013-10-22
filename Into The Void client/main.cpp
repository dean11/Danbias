#include "Time.h"
#include "UserInterface.h"
#include "RenderEngine.h"
#include "SoundSystem.h"

#include "GameStateStack.h"
#include "IntoTheVoid.h"
#include "MainMenu.h"

float elapsedRenderPeriod=0.0f;
float RenderPeriod = 1.0f/60.0f;

void appUpdate( GameStateStack *stack );

int WINAPI WinMain( HINSTANCE thisInstance, HINSTANCE previousInstance, LPSTR cmdLine, int showCommand )
{
	srand((unsigned int)(time(0)));
	if( Time::init() == Time::Failure ) return 0;
	if( RenderEngine::init( thisInstance, cmdLine, showCommand, &UserInterface::wndProc ) == RenderEngine::Failure ) return 0;
	if( UserInterface::init() == UserInterface::Failure ) return 0;
	if( SoundSystem::init() == SoundSystem::Failure ) return 0;
	
	Oyster::Render::Textbox::SetTexture("..\\Content\\Textbox.png");

	GameState *initialState = new MainMenu();
	GameStateStack *stack = new GameStateStack(initialState);

	bool done = false;

	while( !done && !stack->isEmpty() )
	{
		switch( UserInterface::MessageLoop() )
		{
		case UserInterface::Idle:
			appUpdate( stack );
			break;
		case UserInterface::ShutDown:
			done = true;
			break;
		case UserInterface::Updated:
		default:
			break;
		};
	}

	delete stack;

	SoundSystem::release();

	return 0;
}

void appUpdate( GameStateStack *stack )
{
	Time::update();
	float deltaTime = Time::getDeltaTime_Seconds();

	stack->Update(deltaTime);
	SoundSystem::update();

	elapsedRenderPeriod +=deltaTime;
	if(elapsedRenderPeriod>= RenderPeriod)
	{
		RenderEngine::PrepareRendering();
		stack->Render();
		RenderEngine::Present();
		elapsedRenderPeriod=0.0f;
	}
}
