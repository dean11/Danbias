#include <cassert>

#include "Lobby.h"
#include "RenderEngine.h"
#include "Input\InputController.h"
#include "Window\Window.h"
#include "IntoTheVoid.h"

Lobby::Lobby(NetworkConnection *connection)
{
	assert(connection->getConnectionStatus() != OFFLINE);

	m_stateStack = NULL;
	m_connection = connection;
	nrPlayers = m_connection->getLobbyData().playerCount;
}

Lobby::~Lobby()
{
}

void Lobby::Enter(GameStateStack *stateStack)
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);

	m_stateStack = stateStack;

	Oyster::Math::Float2 windowSize;
	windowSize.x = (float)Oyster::Window::Size.right;
	windowSize.y = (float)Oyster::Window::Size.top;

	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\button_darkblue_250x64.png", NULL, NULL, &m_queueTextures[0], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\button_250x64.png", NULL, NULL, &m_queueTextures[1], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\lobby_bg_screen.png", NULL, NULL, &m_queueTextures[2], NULL);

	m_queueOverlay.texture = m_queueTextures[0];
	m_queueOverlay.size = Oyster::Math::Float2(230.0f, 64.0f);
	m_queueOverlay.position.x = windowSize.x / 2.0f - m_queueOverlay.size.x / 2.0f;
	m_queueOverlay.position.y = 20.0f;
	m_queueOverlay.position.z = 0.0f;
}

void Lobby::Exit()
{
	m_stateStack = NULL;
}

void Lobby::Suspend()
{
}

void Lobby::Resume()
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);
}

void Lobby::Update(float timeDelta)
{
	if (m_connection->getConnectionStatus() == ONLINE_INGAME)
	{
		Oyster::Engine::PrepareForRendering::ClearBackBuffer(Oyster::Math::Float4(0, 0, 0, 1));
		Oyster::Engine::PrepareForRendering::Begin2DRender();
		Oyster::Engine::Render::ScreenQuad(m_queueTextures[2],0.5f);
		Oyster::Engine::Render::Text("Game found, loading...", Float2(10.0f, 15.0f), Float3(175.0f, 250.0f, 0));
		Oyster::Engine::Render::PresentScene();

		m_stateStack->SetState(new IntoTheVoid(m_connection));
		return;
	}
	
	Oyster::Math::Float2 cursorPosition = Oyster::Math::Float2((float)Oyster::Input::Controller::GetX(), (float)Oyster::Input::Controller::GetY());

	if (IntersectsOverlay(m_queueOverlay, cursorPosition))
		m_queueOverlay.texture = m_queueTextures[1];
	else
		m_queueOverlay.texture = m_queueTextures[0];

	if (Oyster::Input::Controller::isMouseBtnPressed(MK_LBUTTON))
	{
		if (IntersectsOverlay(m_queueOverlay, cursorPosition) && m_connection->getConnectionStatus() == ONLINE_MAINMENU)
		{
			m_connection->startQueue();
		}
	}
}

void Lobby::Render() const
{
	LobbyInitData lobbyData;
			
	Oyster::Engine::PrepareForRendering::Begin2DRender();
	Oyster::Engine::Render::ScreenQuad(m_queueTextures[2],0.5f);

	Oyster::Engine::PrepareForRendering::Begin2DTextRender();

	switch(m_connection->getConnectionStatus())
	{
		case ONLINE_MAINMENU:
			Oyster::Engine::Render::Text("Queue", Float2(15.0f, 30.0f), m_queueOverlay.position + Float3(15.0f,5.0f,0));

			Oyster::Engine::PrepareForRendering::Begin2DRender();
			Oyster::Engine::Render::Sprite(m_queueOverlay.texture, m_queueOverlay.size, m_queueOverlay.position);
			break;
		case ONLINE_QUEUEING:
			Oyster::Engine::Render::Text("Queueing...", Float2(10.0f, 30.0f), m_queueOverlay.position + Float3(15.0f,5.0f,0));

			Oyster::Engine::PrepareForRendering::Begin2DRender();
			Oyster::Engine::Render::Sprite(m_queueOverlay.texture, m_queueOverlay.size, m_queueOverlay.position);
			break;
		case ONLINE_INLOBBY:
			lobbyData = m_connection->getLobbyData();
			Float2 textSize=Float2(10,10);
			Float3 textPos=Float3(300,40,0);
			Oyster::Engine::Render::Text("In lobby", textSize, textPos);

			textPos.y=60;
			textPos.x=180;
			Oyster::Engine::Render::Text(SSTR("Starting in "<<m_connection->lobbySecondsLeft()<<" seconds."), textSize, textPos) ;


			textPos.y=100;
			textPos.x=80;

			Oyster::Engine::Render::Text("Team 1", textSize, textPos);
			for (int i=0; i<lobbyData.playerCount; i+=2)
			{
				if (strlen(lobbyData.players[i].usrName) > 0)
				{
					textPos.y+=15;
					Oyster::Engine::Render::Text(	lobbyData.players[i].usrName,
													textSize, 
													textPos);
				}
			}
			textPos.y=100;
			textPos.x=450;
			Oyster::Engine::Render::Text("Team 2", textSize, textPos);
			for (int i=1; i<lobbyData.playerCount; i+=2)
			{
				if (strlen(lobbyData.players[i].usrName) > 0)
				{
					textPos.y+=15;
					Oyster::Engine::Render::Text(	lobbyData.players[i].usrName,
													textSize, 
													textPos);
				}
			}

			break;
	}
}
