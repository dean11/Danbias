#include <sstream>

#include "RenderEngine.h"
#include "IntoTheVoid.h"
#include "Player.h"
#include "GraphicStructs.h"

#define PRETTY_GRAPHICS 1
#define NrOfLights 512

Oyster::Buffer* RenderEngine::camera = NULL;
Oyster::Buffer* RenderEngine::perObject = NULL;
//Oyster::Buffer* CSBuff = NULL;
Oyster::Buffer* Vertex = NULL;
Oyster::Buffer* RenderEngine::View = NULL;
Oyster::Shader::ShaderEffect BasicEffect;
Oyster::Shader::ShaderEffect DefferedEffect;
Oyster::Shader::ShaderEffect LazerEffect;
ID3D11ShaderResourceView* damageOverlay, *GuiOverlay, *scoreboardBackground;

ID3D11ShaderResourceView* SrvNulls[16];
ID3D11RenderTargetView* RtvNulls[16];
ID3D11UnorderedAccessView* uavNULL[16];

ID3D11ShaderResourceView*     ComputeInternalResources[4];
ID3D11UnorderedAccessView*    ComputeOutput[4];

ID3D11UnorderedAccessView*    ComputeStorageOut;
ID3D11ShaderResourceView*     ComputeStorageIn;

Oyster::Resources::BufferDefinitions::PointLightDescription pls[1024];

std::string HP;
std::string Shield;
std::string fps;
std::string mps;
Oyster::Math::Float3 speedPos = Oyster::Math::Float3(410,538,0);
Oyster::Math::Float3 HPPos = Oyster::Math::Float3(33,538,0);
Oyster::Math::Float3 ShieldPos = Oyster::Math::Float3(690,538,0);
Oyster::Math::Float3 offset = Oyster::Math::Float3::null;
float fpsUpdateTimer = 0.0f;
float damageOverlayTime = 0.0f;
Oyster::Math::Float3 Bullet[2];
Oyster::Math::Float4x4 V;
Oyster::Math::Float4x4 P;
std::vector<Network::EffectData> effects;


void Init_ComputeExternalResources()
{

	//create pointlights data
	for(int i = 0; i < 1023; ++i )
	{
		pls[i].color = Float3::null;
		while( pls[i].color == Float3::null )
			pls[i].color =  Oyster::Math::Vector3((double)rand() / (RAND_MAX + 1) * (256 - 0)+ 0,(double)rand() / (RAND_MAX + 1) * (256 - 0)+ 0,(double)rand() / (RAND_MAX + 1) * (256 - 0)+ 0);

		pls[i].pos.center = Oyster::Math::Vector3(
			(double)rand() / (RAND_MAX + 1) * (10000 - -10000)+ -10000,
			(double)rand() / (RAND_MAX + 1) * (10000 - -10000)+ -10000,
			(double)rand() / (RAND_MAX + 1) * (10000 - -10000)+ -10000);
		pls[i].pos.radius = 15000;
		pls[i].intensty  = pls[i].pos.radius;
		pls[i].intensty += 12.566370614359172953850573533118f * pls[i].pos.radius * pls[i].pos.radius;
		pls[i].intensty /= 256.0f * 256.0f;
	}
	//upload Pointlights to graphic memory
	Oyster::Engine::Pipeline::Deffered_Lightning::InputPointLights(pls, NrOfLights);

}

void Init_ComputeInternalResources()
{
	/*D3D11_TEXTURE2D_DESC Tdesc;
	Tdesc.Width = Oyster::Window::Size.left;
	Tdesc.Height = Oyster::Window::Size.bottom;
	Tdesc.MipLevels = Tdesc.ArraySize = 1;
	Tdesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Tdesc.SampleDesc.Count = 1;
	Tdesc.SampleDesc.Quality=0;
	Tdesc.Usage = D3D11_USAGE_DEFAULT;
	Tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	Tdesc.CPUAccessFlags = 0;
	Tdesc.MiscFlags = 0;

	ID3D11Texture2D *pTexture;
	HRESULT hr;
	for(int i = 0; i < 4; ++i )
	{
		hr = Oyster::Core::Device->CreateTexture2D( &Tdesc, NULL, &pTexture );
		hr = Oyster::Core::Device->CreateShaderResourceView( pTexture, 0, &(ComputeInternalResources[i]) );
		hr = Oyster::Core::Device->CreateUnorderedAccessView( pTexture, 0, &(ComputeOutput[i]) );
		pTexture->Release();
	}

	hr = Oyster::Core::Device->CreateTexture2D( &Tdesc, NULL, &pTexture );
	hr = Oyster::Core::Device->CreateShaderResourceView( pTexture, 0, &ComputeStorageIn );
	hr = Oyster::Core::Device->CreateUnorderedAccessView( pTexture, 0, &ComputeStorageOut );
	pTexture->Release();*/
}


RenderEngine::Result RenderEngine::init( HINSTANCE thisInstance, LPSTR cmdLine, int showCommand, WNDPROC wProc )
{
	Oyster::Engine::Init::Setup setup;
	setup.Window.appname = "Into The Void!";
	setup.Window.classname = "OysterDemo";
	setup.Window.hinstance = thisInstance;
	setup.Window.nCmdShow = showCommand;
	setup.Window.wProc = wProc;
	setup.Window.InitWindow = true;
	setup.Common.Fullscreen = false;
	if(!Oyster::Engine::Init::FullInit(setup))
		return Result::Failure;
	ID3D11InputLayout* layout;
	Oyster::Buffer::BUFFER_INIT_DESC desc;

	
	desc.ElementSize=sizeof(Oyster::Math::Float4x4);
	desc.NumElements = 1;
	desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
	desc.Type = Oyster::Buffer::CONSTANT_BUFFER_VS;
	desc.InitData = NULL;

	
	perObject = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	LazerEffect.Shaders.Vertex=Oyster::Shader::Get::GetVertex("LazerV");
	LazerEffect.Shaders.Geometry= Oyster::Shader::Get::GetGeometry("LazerG");
	LazerEffect.Shaders.Pixel =Oyster::Shader::Get::GetPixel("PS");
	LazerEffect.IAStage.Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	D3D11_INPUT_ELEMENT_DESC ElemDescLazer[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Oyster::Shader::CreateInputLayout(ElemDescLazer,1,Oyster::Shader::Get::GetVertex("LazerV"),layout);
	LazerEffect.IAStage.Layout = layout;

	LazerEffect.CBuffers.Geometry.push_back(&Oyster::Resources::Buffers::CBufferGs);
	desc.ElementSize=sizeof(Oyster::Math::Float3);
	desc.NumElements=2;
	desc.Type = Oyster::Buffer::VERTEX_BUFFER;


	Vertex = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	DefferedEffect.Shaders.Compute= Oyster::Shader::Get::GetCompute("Comp");

	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\overlay-red.png", NULL, NULL, &damageOverlay, NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\camera_lense_border_B_729x590.png", NULL, NULL, &GuiOverlay, NULL);
	D3DX11CreateShaderResourceViewFromFile(Oyster::Core::Device, "..\\Content\\GUI\\lobby_bg_screen.png", NULL, NULL, &scoreboardBackground, NULL);

	for(int i=0;i<16;++i)
	{
		RtvNulls[i]=NULL;
		SrvNulls[i]=NULL;
		uavNULL[i] = NULL;
	}

	Init_ComputeExternalResources();
	Init_ComputeInternalResources();

	return Result::Success;
}

RenderEngine::Result RenderEngine::update(const Oyster::Math::Float4x4 &ViewMatrix, const Oyster::Math::Float4x4 &Projection, float dt, NetworkConnection* conn)
{
	conn->fetchEffects(effects);
	P =  Oyster::Math::Float4x4(Projection);
	V = Oyster::Math::Float4x4(ViewMatrix);
	
	Float4 col = Float4(0,0,0,1);

	Oyster::Engine::Pipeline::Deffered_Lightning::NewFrame(col, ViewMatrix, Projection);

	fpsUpdateTimer -= dt;

	if (fpsUpdateTimer <= 0.0f)
	{
		dt=1/dt;
		fps = "P: X:";
		char c[10];
		int i = (int)ViewMatrix.v[3].x;
		_itoa_s(i,c,10);
		fps+=c;
		fps+=" Y:";
		i = (int)ViewMatrix.v[3].y;
		_itoa_s(i,c,10);
		fps+=c;
		fps+=" Z:";
		i = (int)ViewMatrix.v[3].z;
		_itoa_s(i,c,10);
		fps+=c;
		fpsUpdateTimer = 0.1f;
	}

	if (damageOverlayTime > 0.0f)
		damageOverlayTime -= dt;

	return Result::Success;
}

void RenderEngine::PrepareRendering()
{
	Oyster::Engine::PrepareForRendering::ClearBackBuffer(Oyster::Math::Float4(0, 0, 0, 1));
}

void RenderEngine::Present()
{
	Oyster::Engine::Render::PresentScene();
}

void RenderLazer()
{
	//	Oyster::Engine::PrepareForRendering::BindRenderTargets(&DefferedTargets[0], 1);
	//	//Oyster::Engine::PrepareForRendering::BindBackBuffer(NULL);
	//	Oyster::Shader::SetShaderEffect(LazerEffect);
	//	
	//	void* dst = Oyster::Resources::Buffers::CBufferGs.Map();
	//	memcpy(dst,&(P),64);
	//	Oyster::Resources::Buffers::CBufferGs.Unmap();
	//
	//	//render per lazer
	//	Vertex->Apply();
	//	for(size_t i=0;i<effects.size();++i)
	//	{
	//		dst = Vertex->Map();
	//		memcpy(dst,&effects[i].head,sizeof(Oyster::Math::Float3)*2);
	//		Vertex->Unmap();
	//		Oyster::Core::DeviceContext->Draw(2,0);
	//	}
	//
	//	
	//	Oyster::Engine::PrepareForRendering::BindRenderTargets(&DefferedTargets[1], 1);
	//	
	//	for(size_t i=0;i<effects.size();++i)
	//	{
	//		dst = Vertex->Map();
	//		memcpy(dst,&effects[i].head,sizeof(Oyster::Math::Float3)*2);
	//		Vertex->Unmap();
	//		Oyster::Core::DeviceContext->Draw(2,0);
}


void RenderEngine::RenderGame( const Player* const *p, int nr, int localPlayerID, const ::Oyster::Render::Model* const staticObject, unsigned int numStaticObjects )
{
	int i =(int)(p[localPlayerID]->Speed * 10);
	char c[100];
	_itoa_s( i, c, 10 );
	mps = c;

	i = p[localPlayerID]->HP;
	_itoa_s( i, c, 10 );
	HP = c;

	i = p[localPlayerID]->Shield;
	_itoa_s( i, c, 10 );
	Shield = c;
	offset.x = Shield.size() * 10;

	Oyster::Engine::Pipeline::Deffered_Lightning::BeginRenderGeometry();
	
	//render to g-Buffers
	for(int i = 0; i < nr; i++)
	{
		Oyster::Engine::Pipeline::Deffered_Lightning::RenderGeometry( (p[i]->renderAvatar), 1 );
	}

	Oyster::Engine::Pipeline::Deffered_Lightning::RenderGeometry( staticObject, numStaticObjects );

	RenderLazer();

	Oyster::Engine::Pipeline::Deffered_Lightning::EndRenderGeometry();

	Oyster::Engine::Pipeline::Deffered_Lightning::RenderLightning();

	//render Gui
	Oyster::Engine::PrepareForRendering::BindBackBuffer(NULL);
	Oyster::Engine::PrepareForRendering::Begin2DRender();

	//Oyster::Engine::Render::ScreenQuad( GeometryResources[3], 0.5f );
	//Oyster::Engine::Render::ScreenQuad( ComputeExternalResources[0], 0.5f );
	//Oyster::Engine::Render::ScreenQuad( ComputeInternalResources[0], 0.5f );

	Oyster::Engine::Render::ScreenQuad(GuiOverlay,0.11f);

	if (damageOverlayTime > 0.0f)
	{
		Oyster::Engine::Render::ScreenQuad(damageOverlay, 0.1f);
	}


	Oyster::Engine::PrepareForRendering::Begin2DTextRender();
	Oyster::Engine::Render::Text(fps,Oyster::Math::Float2(10,10),Oyster::Math::Float3::null);
	
	Oyster::Engine::Render::Text(mps,Oyster::Math::Float2(10,10),speedPos);

	Oyster::Engine::Render::Text(HP,Oyster::Math::Float2(10,10),HPPos);

	Oyster::Engine::Render::Text(Shield,Oyster::Math::Float2(10,10),ShieldPos+offset);
	
	
	

	//Testing to make sure it renders

	//leave for debug
	//Oyster::Engine::PrepareForRendering::Begin2DRender();
	//Oyster::Engine::Render::ScreenQuad(DefferedResourses[4],0.99f);
	//Oyster::Engine::Render::Sprite(p[0]->renderAvatar.Material[1],Oyster::Math::Float2(100,100),Oyster::Math::Float3(100,100,0));

}

void RenderEngine::RenderScoreboard(const Score scores[], int count, const Player * const * players)
{
	Float2 textSize = Float2(10, 10);
	Float3 textPosition = Float3(100, 100, 0);
	std::stringstream stream;
	std::string name;
	int teamPos;
	//TEMP score variable
	Score totScores[2];
	for(int i = 0; i < count; i++)
	{

		teamPos=i%2;
		textPosition.y = 170+teamPos*90+10*i;
		name = players[scores[i].id]->getName();

		if (!name.length())
			continue;

		stream.str("");
		stream << name;
		stream << ": K/D/TK: ";
		stream << scores[i].kills << "/";
		stream << scores[i].deaths << "/";
		stream << scores[i].teamkills;
		totScores[teamPos]+=scores[i];
		Oyster::Engine::Render::Text(stream.str(), textSize, textPosition);
	}
	//Oyster::Engine::Render::Text("Team 1", textSize, textPosition);
	//textPosition.y+=90;
	//Oyster::Engine::Render::Text("Team 2", textSize, textPosition);
	for (int i=0; i<2; i++)
	{
		textPosition.y=155+i*90;
		stream.str("");
		stream << "Team ";
		stream << i+1;
		stream << ": K/D/TK: ";
		stream << totScores[i].kills << "/";
		stream << totScores[i].deaths << "/";
		stream << totScores[i].teamkills;

		Oyster::Engine::Render::Text(stream.str(), textSize, textPosition);
	}

	Oyster::Engine::PrepareForRendering::Begin2DRender();
	Oyster::Engine::Render::Sprite(scoreboardBackground, Float2(650.0f, 240.0f), textPosition - Float3(20.0f, 120.0f, 0.0f));
}

void RenderEngine::Damaged()
{
	damageOverlayTime = 550.5f;
}
