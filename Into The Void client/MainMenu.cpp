#include "MainMenu.h"
#include "RenderEngine.h"
#include "Input\InputController.h"
#include "Window\Window.h"
#include "Lobby.h"

Oyster::Math::Float3 ConnectPos, QuitPos, TitlePos;

bool IntersectsOverlay(const Overlay &overlay, Oyster::Math::Float2 point)
{
	if (point.x >= overlay.position.x && point.x <= (overlay.position.x + overlay.size.x) &&
		point.y >= overlay.position.y && point.y <= (overlay.position.y + overlay.size.y))
	{
		return true;
	}

	return false;
}


MainMenu::MainMenu()
{
	m_stateStack = NULL;
	m_connection = NULL;
}

MainMenu::~MainMenu()
{
}

void MainMenu::Enter(GameStateStack *stateStack)
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);

	m_stateStack = stateStack;
	m_connection = NULL;

	Oyster::Math::Float2 windowSize;
	windowSize.x = (float)Oyster::Window::Size.right;
	windowSize.y = (float)Oyster::Window::Size.top;
	
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_darkblue_250x64.png", NULL, NULL, &m_connectTextures[0], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_250x64.png", NULL, NULL, &m_connectTextures[1], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_darkblue_250x64.png", NULL, NULL, &m_quitTextures[0], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\button_250x64.png", NULL, NULL, &m_quitTextures[1], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\lobby_bg_screen.png", NULL, NULL, &m_backgroundOverlay.texture, NULL);

	m_backgroundOverlay.size = windowSize;
	m_backgroundOverlay.position.x = 0.0f;
	m_backgroundOverlay.position.y = 0.0f;
	m_backgroundOverlay.position.z = 0.5f;

	m_connectOverlay.texture = m_connectTextures[0];
	m_connectOverlay.size = Oyster::Math::Float2(229.0f, 80.0f);
	m_connectOverlay.position.x = windowSize.x / 2.0f - m_connectOverlay.size.x / 2.0f;
	m_connectOverlay.position.y = 200.0f;
	m_connectOverlay.position.z = 0.0f;

	ConnectPos = m_connectOverlay.position;
	ConnectPos.x+=10;

	m_quitOverlay.texture = m_quitTextures[0];
	m_quitOverlay.size = Oyster::Math::Float2(132.0f, 83.0f);
	m_quitOverlay.position.x = windowSize.x / 2.0f - m_quitOverlay.size.x / 2.0f;
	m_quitOverlay.position.y = windowSize.y - m_quitOverlay.size.y - 20.0f;
	m_quitOverlay.position.z = 0.0f;

	QuitPos = m_quitOverlay.position;
	QuitPos.x+=10;
	QuitPos.y+=5;

	TitlePos.x = 120.0f;
	TitlePos.y = 10.0f;
	TitlePos.z = 0.0f;
}

void MainMenu::Exit()
{
	m_stateStack = NULL;
	m_connection = NULL;
}

void MainMenu::Suspend()
{
}

void MainMenu::Resume()
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);
}

void MainMenu::Update(float timeDelta)
{
	if (m_connection)
	{
		if (m_connection->getConnectionStatus() != OFFLINE)
		{
			m_stateStack->SetState(new Lobby(m_connection));
			return;
		}
		else
		{
			m_connectTimer += timeDelta;

			// If we haven't established a connection after 5 seconds, stop trying.
			if (m_connectTimer >= 5.0f)
			{
				m_connection->disconnect();
				delete m_connection;
				m_connection = NULL;
			}
		}
	}

	Oyster::Math::Float2 cursorPosition = Oyster::Math::Float2((float)Oyster::Input::Controller::GetX(), (float)Oyster::Input::Controller::GetY());

	if (IntersectsOverlay(m_connectOverlay, cursorPosition))
		m_connectOverlay.texture = m_connectTextures[1];
	else
		m_connectOverlay.texture = m_connectTextures[0];

	if (IntersectsOverlay(m_quitOverlay, cursorPosition))
		m_quitOverlay.texture = m_quitTextures[1];
	else
		m_quitOverlay.texture = m_quitTextures[0];

	if (Oyster::Input::Controller::isMouseBtnPressed(MK_LBUTTON))
	{
		if (!m_connection && IntersectsOverlay(m_connectOverlay, cursorPosition))
		{
			m_connection = new NetworkConnection();
			m_connectTimer = 0.0f;

			if (m_connection->connectWithFileData("../Connectiondata.txt")== NetworkConnection::Failure)
			{
				delete m_connection;
				m_connection = NULL;
			}
		}
		else if (IntersectsOverlay(m_quitOverlay, cursorPosition))
		{
			m_stateStack->PopState();
		}
	}
}

void MainMenu::Render() const
{
	Oyster::Engine::PrepareForRendering::Begin2DTextRender();
	Oyster::Engine::Render::Text("Into The Void!",Oyster::Math::Float2(20,50), TitlePos);
	Oyster::Engine::Render::Text("Quit",Oyster::Math::Float2(15,40),QuitPos);

	if (m_connection)
		Oyster::Engine::Render::Text("Connecting",Oyster::Math::Float2(10,50),ConnectPos);
	else
		Oyster::Engine::Render::Text("Connect",Oyster::Math::Float2(15,50),ConnectPos);

	Oyster::Engine::PrepareForRendering::Begin2DRender();

	Oyster::Engine::Render::Sprite(m_backgroundOverlay.texture, m_backgroundOverlay.size, m_backgroundOverlay.position);
	Oyster::Engine::Render::Sprite(m_connectOverlay.texture, m_connectOverlay.size, m_connectOverlay.position);
	Oyster::Engine::Render::Sprite(m_quitOverlay.texture, m_quitOverlay.size, m_quitOverlay.position);

}
