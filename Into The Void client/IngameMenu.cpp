#include "IngameMenu.h"
#include "RenderEngine.h"
#include "Input\InputController.h"
#include "Window\Window.h"

IngameMenu::IngameMenu()
{
	m_stateStack = NULL;
	m_backgroundOverlay.texture=NULL;
	m_resumeOverlay.texture=NULL;
	m_disconnectOverlay.texture=NULL;
}

IngameMenu::~IngameMenu()
{
	SAFE_RELEASE(m_textures[0]);
	SAFE_RELEASE(m_textures[1]);
	SAFE_RELEASE(m_textures[2]);
}

void IngameMenu::Enter(GameStateStack *stateStack)
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);

	m_stateStack = stateStack;

	Oyster::Math::Float2 windowSize;
	windowSize.x = (float)Oyster::Window::Size.right;
	windowSize.y = (float)Oyster::Window::Size.top;

	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\button_darkblue_250x64.png", NULL, NULL, &m_textures[0], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\button_250x64.png", NULL, NULL, &m_textures[1], NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\Gui\\lobby_bg_screen.png", NULL, NULL, &m_textures[2], NULL);

	m_backgroundOverlay.texture = m_textures[2];
	m_backgroundOverlay.size = Oyster::Math::Float2(350.0f, 200.0f);
	m_backgroundOverlay.position.x = windowSize.x / 2.0f - m_backgroundOverlay.size.x / 2.0f;
	m_backgroundOverlay.position.y = windowSize.y / 2.0f - m_backgroundOverlay.size.y / 2.0f;
	m_backgroundOverlay.position.z = 0.01f;

	m_resumeOverlay.texture = m_textures[0];
	m_resumeOverlay.size = Oyster::Math::Float2(256.0f, 64.0f);
	m_resumeOverlay.position.x = m_backgroundOverlay.position.x + (m_backgroundOverlay.size.x - m_resumeOverlay.size.x) / 2.0f;
	m_resumeOverlay.position.y = m_backgroundOverlay.position.y + 20.0f;
	m_resumeOverlay.position.z = 0.0f;

	m_disconnectOverlay.texture = m_textures[0];
	m_disconnectOverlay.size = Oyster::Math::Float2(256.0f, 64.0f);
	m_disconnectOverlay.position.x = m_backgroundOverlay.position.x + (m_backgroundOverlay.size.x - m_disconnectOverlay.size.x) / 2.0f;
	m_disconnectOverlay.position.y = m_resumeOverlay.position.y + m_resumeOverlay.size.y + 40.0f;
	m_disconnectOverlay.position.z = 0.0f;
}

void IngameMenu::Exit()
{
	m_stateStack = NULL;
}

void IngameMenu::Suspend()
{
}

void IngameMenu::Resume()
{
	Oyster::Input::Controller::OrigoTopLeft();
	Oyster::Input::Controller::RestrictMouse(false);
}

void IngameMenu::Update(float timeDelta)
{
	if (Oyster::Input::Controller::isKeyPressed(VK_ESCAPE))
	{
		m_stateStack->PopState();
		return;
	}

	Oyster::Math::Float2 cursorPosition = Oyster::Math::Float2((float)Oyster::Input::Controller::GetX(), (float)Oyster::Input::Controller::GetY());

	if (IntersectsOverlay(m_resumeOverlay, cursorPosition))
		m_resumeOverlay.texture = m_textures[1];
	else
		m_resumeOverlay.texture = m_textures[0];

	if (IntersectsOverlay(m_disconnectOverlay, cursorPosition))
		m_disconnectOverlay.texture = m_textures[1];
	else
		m_disconnectOverlay.texture = m_textures[0];

	if (Oyster::Input::Controller::isMouseBtnPressed(MK_LBUTTON))
	{
		if (IntersectsOverlay(m_resumeOverlay, cursorPosition))
		{
			m_stateStack->PopState();
			return;
		}
		else if (IntersectsOverlay(m_disconnectOverlay, cursorPosition))
		{
			m_stateStack->SetState(new MainMenu());
			return;
		}
	}
}

void IngameMenu::Render() const
{
	Oyster::Engine::Render::Text("Resume", Float2(15.0f, 30.0f), m_resumeOverlay.position + Float3(15.0f,5.0f,0));
	Oyster::Engine::Render::Text("Disconnect", Float2(10.0f, 30.0f), m_disconnectOverlay.position + Float3(15.0f,5.0f,0));

	Oyster::Engine::PrepareForRendering::Begin2DRender();

	Oyster::Engine::Render::Sprite(m_backgroundOverlay.texture, m_backgroundOverlay.size, m_backgroundOverlay.position);
	Oyster::Engine::Render::Sprite(m_resumeOverlay.texture, m_resumeOverlay.size, m_resumeOverlay.position);
	Oyster::Engine::Render::Sprite(m_disconnectOverlay.texture, m_disconnectOverlay.size, m_disconnectOverlay.position);
}

bool IngameMenu::IsTransparent() const
{
	return true;
}
