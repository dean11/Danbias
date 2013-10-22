#include "GameEndedMenu.h"
#include "RenderEngine.h"
#include "Input\InputController.h"
#include "Window\Window.h"
#include "Lobby.h"

Oyster::Math::Float3 MainMenuPos, WinnerPos;




GameEndedMenu::GameEndedMenu(Player *players[], Score* scores, int winningTeam)
{
	m_stateStack = NULL;
	winner=winningTeam;
	for (int i=0; i< PLAYER_MAX_COUNT; i++)
	{
		this->scoreboard[i]=scores[i];
		playerList[i]=players[i];
	}
}

GameEndedMenu::~GameEndedMenu()
{
}

void GameEndedMenu::Enter(GameStateStack *stateStack)
{

	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);

	m_stateStack = stateStack;

	Oyster::Math::Float2 windowSize;
	windowSize.x = (float)Oyster::Window::Size.right;
	windowSize.y = (float)Oyster::Window::Size.top;
	
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_darkblue_250x64.png", NULL, NULL, &m_quitTextures[0], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_250x64.png", NULL, NULL, &m_quitTextures[1], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\lobby_bg_screen.png", NULL, NULL, &m_backgroundOverlay.texture, NULL);

	m_backgroundOverlay.size = windowSize;
	m_backgroundOverlay.position.x = 0.0f;
	m_backgroundOverlay.position.y = 0.0f;
	m_backgroundOverlay.position.z = 0.5f;



	m_quitOverlay.texture = m_quitTextures[0];
	m_quitOverlay.size = Oyster::Math::Float2(132.0f, 83.0f);
	m_quitOverlay.position.x = windowSize.x / 2.0f - m_quitOverlay.size.x / 2.0f;
	m_quitOverlay.position.y = windowSize.y - m_quitOverlay.size.y - 20.0f;
	m_quitOverlay.position.z = 0.0f;

	MainMenuPos = m_quitOverlay.position;
	MainMenuPos.x+=10;
	MainMenuPos.y+=5;

	WinnerPos.x = 120.0f;
	WinnerPos.y = 10.0f;
	WinnerPos.z = 0.0f;
}

void GameEndedMenu::Exit()
{
	m_stateStack = NULL;
}

void GameEndedMenu::Suspend()
{
}

void GameEndedMenu::Resume()
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);
}

void GameEndedMenu::Update(float timeDelta)
{

	Oyster::Math::Float2 cursorPosition = Oyster::Math::Float2((float)Oyster::Input::Controller::GetX(), (float)Oyster::Input::Controller::GetY());



	if (IntersectsOverlay(m_quitOverlay, cursorPosition))
		m_quitOverlay.texture = m_quitTextures[1];
	else
		m_quitOverlay.texture = m_quitTextures[0];

	if (Oyster::Input::Controller::isMouseBtnPressed(MK_LBUTTON))
	{
		if (IntersectsOverlay(m_quitOverlay, cursorPosition))
		{
			m_stateStack->SetState(new MainMenu());
		}
	}
}

void GameEndedMenu::Render() const
{
	RenderEngine::RenderScoreboard(this->scoreboard, PLAYER_MAX_COUNT, playerList);
	Oyster::Engine::PrepareForRendering::Begin2DTextRender();
	//Oyster::Engine::Render::Text("Into The Void!",Oyster::Math::Float2(20,50), TitlePos);
	Oyster::Engine::Render::Text(SSTR("Team "<<winner+1<<" won the game."),Oyster::Math::Float2(15,50), WinnerPos);
	Oyster::Engine::Render::Text("Quit",Oyster::Math::Float2(15,40),MainMenuPos);

	Oyster::Engine::PrepareForRendering::Begin2DRender();

	Oyster::Engine::Render::Sprite(m_backgroundOverlay.texture, m_backgroundOverlay.size, m_backgroundOverlay.position);
	Oyster::Engine::Render::Sprite(m_quitOverlay.texture, m_quitOverlay.size, m_quitOverlay.position);

}
