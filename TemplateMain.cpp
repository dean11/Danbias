//--------------------------------------------------------------------------------------
// File: TemplateMain.cpp
//
// BTH-D3D-Template
//
// Copyright (c) Stefan Petersson 2011. All rights reserved.
//--------------------------------------------------------------------------------------
#include "Engine\Engine.h"
using namespace Oyster;
using namespace Oyster::Render;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
bool				InitDX11(HINSTANCE hInstance,int nCmdShow);
HRESULT				Render(float deltaTime);
HRESULT				Update(float deltaTime);

Oyster::Buffer		*cb;
Oyster::Buffer		*camBuffer1;
Oyster::Buffer		*camBuffer2;
bool cam1Buffered	= true;
Model  m[2];
std::vector<Oyster::Render::Model> models = std::vector<Oyster::Render::Model>(10);
Oyster::Render::Camera cam = Oyster::Render::Camera();

struct PerObjectBuffer
{
	Oyster::Math::Float4x4 matrix;
};


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if(!InitDX11(hInstance,nCmdShow))
		return 0;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			//render
			Update(dt);
			::Render(dt);

			prevTimeStamp = currTimeStamp;
		}
	}

	return (int) msg.wParam;
}

bool InitDX11(HINSTANCE hInstance,int nCmdShow)
{
	Engine::Init::Setup setup=Engine::Init::Setup();
	setup.Window.appname = "OysterDemo";
	setup.Window.classname = "OysterDemo";
	setup.Window.hinstance = hInstance;
	setup.Window.nCmdShow = nCmdShow;
	setup.Window.wProc = &WndProc;
	setup.Window.InitWindow = true;
	setup.Common.Fullscreen = false;
	if(!Oyster::Engine::Init::FullInit(setup))
		return 0;

	Oyster::Math::Float4x4 matrix = Oyster::Math::Float4x4();
	cam.LookAt(Oyster::Math::Float3(0,0,-70),Oyster::Math::Float3(0,0,1),Oyster::Math::Float3(0,1,0));
	//cam.SetLens(PI/4,1024.0f / 768.0f,1,200);
	cam.SetLens(PI/4,1920.0f / 1080.0f,1,200);
	cam.UpdateViewMatrix();
	matrix=cam.ViewProj();

	Oyster::Shader::ShaderEffect se;
	se.Shaders.Vertex = Oyster::Shader::Get::GetVertex("VSObj");
	se.Shaders.Pixel = Oyster::Shader::Get::GetPixel("PSObj");
	se.IAStage.Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ID3D11InputLayout* layout;

	D3D11_INPUT_ELEMENT_DESC ElemDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	Oyster::Shader::CreateInputLayout(ElemDesc,3,Oyster::Shader::Get::GetVertex("VSObj"),layout);
	se.IAStage.Layout = layout;

	Oyster::Buffer::BUFFER_INIT_DESC desc;
	
	desc.ElementSize=sizeof(Oyster::Math::Float4x4);
	desc.NumElements = 1;
	desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
	desc.Type = Oyster::Buffer::CONSTANT_BUFFER_VS;
	desc.InitData = matrix;

	camBuffer1 = Oyster::Engine::Init::Buffers::CreateBuffer(desc);
	se.CBuffers.Vertex.push_back(camBuffer1);

	Oyster::Shader::SetShaderEffect(se);

	//create Buffers
	Oyster::FileLoaders::ObjReader *reader = Oyster::FileLoaders::ObjReader::LoadFile("bth.OBJ");
	Oyster::FileLoaders::ObjReader::Vertex** vertex = new Oyster::FileLoaders::ObjReader::Vertex*[1];
	int vcount;
	std::map<std::string,ID3D11ShaderResourceView *> textures;
	reader->GetVertexData(vertex,vcount,textures);

	desc.ElementSize=sizeof(Oyster::FileLoaders::ObjReader::Vertex);
	desc.NumElements = vcount;
	desc.InitData = *vertex;
	desc.Type = Oyster::Buffer::VERTEX_BUFFER;
	desc.Usage = Oyster::Buffer::BUFFER_DEFAULT;

	Oyster::Buffer *VB = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	ID3D11ShaderResourceView *srv = textures["Diffuse"];

	Oyster::Math::Float4x4 *world = new Oyster::Math::Float4x4();
	Oyster::Math::identityMatrix(*world);

	world->m43 = 20; // edited by Dan 04-19 : Still not good :/
	world->m11 = 0.5f;
	world->m22 = 0.5f;
	world->m33 = 0.5f;

	desc.ElementSize=64;
	desc.NumElements=1;
	desc.Type = Oyster::Buffer::CONSTANT_BUFFER_VS;
	desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
	desc.InitData = NULL;

	cb = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	m[0] = *(new  Oyster::Render::Model());

	m[0].World = world;
	m[0].info->VertexCount = vcount;
	m[0].info->Material.push_back(srv);
	m[0].info->Indexed=false;
	m[0].Visible=true;
	m[0].info->Vertices=*VB;

	models.push_back(m[0]);

	world = new Oyster::Math::Float4x4(*world);

	m[1] = * (new  Oyster::Render::Model(m[0]));
	world->m43 = 00;
	m[1].World=world;

	//models.push_back(m[1]);

	return true;
}

HRESULT Update(float deltaTime)
{
	//Early Exit
	if(Oyster::Input::Controller::isKeyReleased(VK_ESCAPE))
		PostQuitMessage(0);
	if(Oyster::Input::Controller::isKeyDown(87))
		cam.Walk(10*deltaTime);
	if(Oyster::Input::Controller::isKeyDown(0x53))
		cam.Walk(-10*deltaTime);
	if(Oyster::Input::Controller::isKeyDown(0x41))
		cam.Strafe(-10*deltaTime);
	if(Oyster::Input::Controller::isKeyDown(0x44))
		cam.Strafe(10*deltaTime);
	if(Oyster::Input::Controller::isKeyDown(0x10))
		cam.Fly(-10*deltaTime);
	if(Oyster::Input::Controller::isKeyDown(0x20))
		cam.Fly(10*deltaTime);

	Matrix rot;
	Oyster::Math::rotationMatrix_AxisY(rot,deltaTime*PI/4);
	*(m[1].World) =rot * *(m[1].World);

	Oyster::Math::rotationMatrix_AxisX(rot,deltaTime*PI/4);
	*(m[0].World) = rot * *(m[0].World);

	cam.UpdateViewMatrix();


	void* temp = camBuffer1->Map();
	if(temp)
	{
		memcpy(temp,&cam.ViewProj(),sizeof(Matrix));
		camBuffer1->Unmap();
	}
		


	return S_OK;
}

HRESULT Render(float deltaTime)
{
	Oyster::Engine::PrepareForRendering::ClearBackBuffer(Oyster::Math::Float4(0,0,1,1));

	Oyster::Engine::Render::Geometry(m,2,cb,1);

	Oyster::Engine::Render::PresentScene();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}