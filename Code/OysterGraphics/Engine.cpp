#include "Engine.h"

bool CreateDepthStencil(bool MSAA_Quality);
bool CreateRenderTarget();
void SetViewPort();

class oysterPrivates
{
public:
	bool instance,swapChained; 
	ID3D11DepthStencilView* depth;
	ID3D11RenderTargetView *rtv;
	ID3D11UnorderedAccessView *uav;
	ID3D11ShaderResourceView* depthTexture;
	DXGI_SAMPLE_DESC sampleDesc;
	D3D11_VIEWPORT viewPort;
	oysterPrivates():instance(false),swapChained(false),depth(NULL),rtv(NULL), depthTexture(NULL){};
	int sizeX, sizeY;
	class State
	{
	public:
		int NrOfSamples;
		int SampleSpread;
		int NrOfPointlights;
	}States;

}instance;

Oyster::Engine::Engine()
{
}

Oyster::Engine::~Engine()
{
}

//Init
bool Oyster::Engine::Init::IsInstanced()
{
	return instance.instance;
}

bool Oyster::Engine::Init::Instance(bool SingleThreaded,bool Reference,bool ForceDX11)
{
	if(!instance.instance)
		if(Oyster::Core::Init(SingleThreaded,Reference,ForceDX11))
			instance.instance=true;

	return instance.instance;
}

bool Oyster::Engine::Init::HasSwapChain()
{
	return instance.swapChained;
}

bool Oyster::Engine::Init::CreateSwapChain(HWND Window,int NrofBuffers,bool MSAA_Quality,bool Fullscreen)
{
	RECT rc;
	GetClientRect( Window, &rc );
	instance.sizeX = rc.right - rc.left;
	instance.sizeY = rc.bottom - rc.top;

	if(!instance.swapChained)
	{
		if(Oyster::Core::CreateSwapChain(Window,NrofBuffers,MSAA_Quality,Fullscreen))
			instance.swapChained=true;
	}

	return instance.swapChained;
}

bool Oyster::Engine::Init::FullInit(const Setup& setup)
{
	if(!Oyster::Engine::Init::Instance(setup.SingleThreaded,setup.Reference,setup.ForceDX11))
		return false;
	if(!Oyster::Engine::Init::CreateSwapChain(setup.window,setup.NrOfBuffers,setup.MSAA_Quality,setup.Fullscreen))
		return false;
	/// \todo Add Init Shaders to full Init Process
	if(setup.GenerateDepthStencil)
		if(!CreateDepthStencil(setup.MSAA_Quality))
			return false;
	if(!CreateRenderTarget())
		return false;
	if(!Oyster::Render::Textbox::Init())
		return false;
	SetViewPort();
	if(setup.BindDefault)
		Oyster::Engine::PrepareForRendering::BindRenderTargets(&instance.rtv,1,instance.depth);

	instance.States.NrOfSamples = 14;
	instance.States.SampleSpread = 4;
	instance.States.NrOfPointlights = 1024;
	
	Oyster::Resources::Buffers::Init();
	Oyster::Resources::ShaderEffects::Init();
	Oyster::Resources::PipeLineResourses::Init(instance.sizeX, instance.sizeY);
	return true;

}

Oyster::Buffer* Oyster::Engine::Init::Buffers::CreateBuffer(const Oyster::Buffer::BUFFER_INIT_DESC desc)
{
	Oyster::Buffer *buffy = new Oyster::Buffer();
	buffy->Init(desc);
	return buffy;
}


//PrepareForRendering
void Oyster::Engine::PrepareForRendering::BindRenderTargets(ID3D11RenderTargetView** RenderTargets,int NrOfTargets,ID3D11DepthStencilView* depth)
{
	Oyster::Core::DeviceContext->OMSetRenderTargets(NrOfTargets,RenderTargets,depth);
}

void Oyster::Engine::PrepareForRendering::BindRenderTargets(ID3D11RenderTargetView** RenderTargets,int NrOfTargets)
{
	Oyster::Core::DeviceContext->OMSetRenderTargets(NrOfTargets,RenderTargets,instance.depth);
}

void Oyster::Engine::PrepareForRendering::BindBackBufferAsUAV()
{
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews(0,1,&instance.uav,0);
}

void Oyster::Engine::PrepareForRendering::BindUAV(ID3D11UnorderedAccessView** uav, int NrOfUavs)
{
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews(0,NrOfUavs,uav,0);
}

void Oyster::Engine::PrepareForRendering::ClearBackBuffer(Math::Float4 color)
{
	Oyster::Core::DeviceContext->ClearRenderTargetView(instance.rtv,(float*)color);
	Oyster::Core::DeviceContext->ClearDepthStencilView(instance.depth,1,1,0);
}

void Oyster::Engine::PrepareForRendering::BindBackBuffer(ID3D11DepthStencilView* depth)
{
	Oyster::Core::DeviceContext->OMSetRenderTargets(1,&instance.rtv,depth);
}

void Oyster::Engine::PrepareForRendering::BindBackBuffer()
{
	Oyster::Core::DeviceContext->OMSetRenderTargets(1,&instance.rtv,instance.depth);
}

void Oyster::Engine::PrepareForRendering::Begin2DRender()
{
	Oyster::Resources::Buffers::V2DSprites.Apply();
	Oyster::Resources::Buffers::CBufferGs.Apply();
	Core::ShaderManager::SetShaderEffect(Oyster::Resources::ShaderEffects::BasicSprite);
}

void Oyster::Engine::PrepareForRendering::Begin2DTextRender()
{
	Oyster::Render::Textbox::TextBuffer.Apply();
	Oyster::Resources::Buffers::CBufferGs.Apply();
	Oyster::Core::ShaderManager::SetShaderEffect(Oyster::Resources::ShaderEffects::Text2DEffect);
}


//Render
void Oyster::Engine::Render::PresentScene()
{
	Core::SwapChain->Present(0,0);
}

void Oyster::Engine::Render::Geometry(const Oyster::Render::Model* models,int count,Oyster::Buffer* cBufferEveryObject, int slot)
{
	if(cBufferEveryObject)
		cBufferEveryObject->Apply(slot);
	for(int i=0;i<count;++i)
	{
		if(models[i].Visible)
		{
			if(cBufferEveryObject)
			{
				void* data = cBufferEveryObject->Map();
				memcpy(data,&(models[i].World->GetTranspose()),64);
				cBufferEveryObject->Unmap();
			}
			Oyster::Core::DeviceContext->PSSetShaderResources(0,models[i].info->Material.size(),&(models[i].info->Material[0]));
			
			models[i].info->Vertices.Apply();
			if(models[i].info->Indexed)
			{
				models[i].info->Indecies.Apply();
				Oyster::Core::DeviceContext->DrawIndexed(models[i].info->VertexCount,0,0);
			}
			else
				Oyster::Core::DeviceContext->Draw(models[i].info->VertexCount,0);
		}
	}
}

void Oyster::Engine::Render::Text(std::string text, Oyster::Math::Float2 size, Oyster::Math::Float3 Pos)
{
	Pos.x -= instance.sizeX/2;
	Pos.x += size.x;

	Pos.y -= instance.sizeY/2;
	Pos.y += size.y;

	Matrix m;
	m = Math::Matrix::identity;
	float width = (1.0f/(instance.sizeX/2.0f));
	float height = (1.0f/(instance.sizeY/2.0f));
	m.m41=Pos.x * width;
	m.m42=-Pos.y * height;
	m.m43=Pos.z;
	m.m11=width*size.x;
	m.m22=height*size.y;
	void* dest = Resources::Buffers::CBufferGs.Map();
	memcpy(dest,&m.GetTranspose(),64);
	Resources::Buffers::CBufferGs.Unmap();
	Oyster::Render::Textbox::Update(text, size.x);
	Oyster::Engine::PrepareForRendering::Begin2DTextRender();
	Oyster::Core::DeviceContext->PSSetShaderResources(0,1,&(Oyster::Render::Textbox::Texture));
	//Should be able to be outside of the for loop. Keeping it here for now though.
	Oyster::Core::DeviceContext->Draw(Oyster::Render::Textbox::NumLetters, 0);
}

void Oyster::Engine::Render::ScreenQuad(ID3D11ShaderResourceView* srv, float ZPos)
{

	Oyster::Core::DeviceContext->PSSetShaderResources(0,1,&srv);

	Matrix m;
	m = Math::Matrix::identity;
	m.m43=ZPos;
	
	void* dest = Resources::Buffers::CBufferGs.Map();
	memcpy(dest,&m.GetTranspose(),64);
	Resources::Buffers::CBufferGs.Unmap();

	Oyster::Core::DeviceContext->Draw(1,0);
}

void Oyster::Engine::Render::Sprite(ID3D11ShaderResourceView* srv, Oyster::Math::Float2 size, Oyster::Math::Float3 Pos)
{

	Oyster::Core::DeviceContext->PSSetShaderResources(0,1,&srv);

	Pos.x -= instance.sizeX/2;
	Pos.x += size.x/2;

	Pos.y -= instance.sizeY/2;
	Pos.y += size.y/2;

	Matrix m;
	m = Math::Matrix::identity;
	float width = (1.0f/(instance.sizeX/2.0f));
	float height = (1.0f/(instance.sizeY/2.0f));
	m.m41=Pos.x * width;
	m.m42=-Pos.y * height;
	m.m43=Pos.z;
	m.m11=width*size.x/2;
	m.m22=height*size.y/2;
	
	void* dest = Resources::Buffers::CBufferGs.Map();
	memcpy(dest,&m.GetTranspose(),64);
	Resources::Buffers::CBufferGs.Unmap();

	Oyster::Core::DeviceContext->Draw(1,0);
}

//Misc
bool CreateDepthStencil(bool MSAA_Quality)
{
	
	D3D11_TEXTURE2D_DESC desc;
	desc.MipLevels=1;
	desc.ArraySize=1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags=0;
	desc.MiscFlags=0;
	desc.Height = instance.sizeY;
	desc.Width = instance.sizeX;


	//Check and Set multiSampling
	if(MSAA_Quality)
	{
		if(FAILED(Oyster::Core::Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&desc.SampleDesc.Quality)))
		{
			//Log<< "Failed to check multisample quality levels (MSAAQuality).\n";
			return false;
		}
		desc.SampleDesc.Count=4;
		--desc.SampleDesc.Quality;
		//Log << "Supported multisample quality levels (MSAAQuality): " << desc.SampleDesc.Quality+1 << "x\n";
	}
	else
	{
		desc.SampleDesc.Count=1;
		desc.SampleDesc.Quality=0;
	}

	ID3D11Texture2D* depthstencil;

	if(FAILED(Oyster::Core::Device->CreateTexture2D(&desc,0,&depthstencil)))
		return false;
	if(FAILED(Oyster::Core::Device->CreateDepthStencilView(depthstencil,0,&instance.depth)))
	{
		depthstencil->Release();
		return false;
	}
	depthstencil->Release();


		
	return true;
}

bool CreateRenderTarget()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
	ZeroMemory( &descView, sizeof(descView) );
	descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	descView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descView.Texture2D.MipSlice=0;

	ID3D11Texture2D* backBuffer;
	if(FAILED(Oyster::Core::SwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer))))
		return false;
	if(FAILED(Oyster::Core::Device->CreateRenderTargetView(backBuffer,0,&instance.rtv)))
	{
		backBuffer->Release();
		return false;
	}
	if(FAILED(Oyster::Core::Device->CreateUnorderedAccessView(backBuffer,0,&instance.uav)))
	{
		backBuffer->Release();
		return false;
	}

	backBuffer->Release();

	return true;
}

void SetViewPort()
{
	instance.viewPort.TopLeftX = 0.0f;
	instance.viewPort.TopLeftY = 0.0f;
	instance.viewPort.Width = (float)instance.sizeX;
	instance.viewPort.Height = (float)instance.sizeY;
	instance.viewPort.MinDepth = 0.0f;
	instance.viewPort.MaxDepth = 1.0f;

	Oyster::Core::DeviceContext->RSSetViewports(1,&instance.viewPort);
}

void Blur(int target)
{
	//TODO: proper size calculation

	//bind input
	Oyster::Core::DeviceContext->CSSetShaderResources(0,1,&Oyster::Resources::PipeLineResourses::LightOut[target]);
	Oyster::Engine::PrepareForRendering::BindUAV(&Oyster::Resources::PipeLineResourses::TempUav,1);

	//dispatch blurr horizontal
	Oyster::Core::ShaderManager::Set::Compute(Oyster::Core::ShaderManager::Get::Compute(L"BlurHorizontal"));
	Oyster::Core::DeviceContext->Dispatch(7,instance.sizeX,1);

	//clean Pipeline
	Oyster::Core::DeviceContext->CSSetShaderResources(0,16,&Oyster::Resources::PipeLineResourses::SrvNulls[0]);
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews(0,8,&Oyster::Resources::PipeLineResourses::uavNULL[0],0);

	//bind input
	Oyster::Core::DeviceContext->CSSetShaderResources(0,1,&Oyster::Resources::PipeLineResourses::TempSrv);
	Oyster::Engine::PrepareForRendering::BindUAV(&Oyster::Resources::PipeLineResourses::LightTarget[target],1);
	
	//dispatch blurr vertical
	Oyster::Core::ShaderManager::Set::Compute(Oyster::Core::ShaderManager::Get::Compute(L"BlurVertical"));
	Oyster::Core::DeviceContext->Dispatch(instance.sizeY,5,1);

	//clean Pipeline
	Oyster::Core::DeviceContext->CSSetShaderResources(0,16,&Oyster::Resources::PipeLineResourses::SrvNulls[0]);
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews(0,8,&Oyster::Resources::PipeLineResourses::uavNULL[0],0);
}


//Pipeline Render
void Oyster::Engine::Pipeline::Deffered_Lightning::NewFrame(const Float4& col, const Matrix& View, const Matrix& Projection)
{
	//diffuse
	Oyster::Core::DeviceContext->ClearRenderTargetView( Oyster::Resources::PipeLineResourses::GeometryTarget[0], col);

	//Specular
	Oyster::Core::DeviceContext->ClearRenderTargetView( Oyster::Resources::PipeLineResourses::GeometryTarget[1], col);
	
	//Glow
	Oyster::Core::DeviceContext->ClearRenderTargetView( Oyster::Resources::PipeLineResourses::GeometryTarget[2], col);

	//Pos
	Oyster::Core::DeviceContext->ClearRenderTargetView( Oyster::Resources::PipeLineResourses::GeometryTarget[3], col);

	//Normal
	Oyster::Core::DeviceContext->ClearRenderTargetView( Oyster::Resources::PipeLineResourses::GeometryTarget[4], col);

	Oyster::Engine::PrepareForRendering::ClearBackBuffer(col);

	//Upload Camera to Resources

	Matrix P =  Oyster::Math::Float4x4(Projection);
	Matrix V = Oyster::Math::Float4x4(View);
	Matrix VP = V*P;

	Oyster::Resources::PipeLineResourses::LightData.projectionMatrix = P.GetTranspose();
	Oyster::Resources::PipeLineResourses::LightData.viewMatrix = V;

	//Oyster::Collision3D::Frustrum( VP ).Split(Oyster::Resources::PipeLineResourses::SubFrustrums, Oyster::Resources::PipeLineResourses::FrustrumDimensions.x, Oyster::Resources::PipeLineResourses::FrustrumDimensions.y, Oyster::Resources::PipeLineResourses::FrustrumDimensions.z );

	void* dest = Oyster::Resources::ShaderEffects::ModelEffect.CBuffers.Vertex[0]->Map();
	memcpy(dest,&VP.GetTranspose(),64);
	Oyster::Resources::ShaderEffects::ModelEffect.CBuffers.Vertex[0]->Unmap();

	dest= Oyster::Resources::ShaderEffects::ModelEffect.CBuffers.Vertex[1]->Map();
	memcpy(dest,&V.GetTranspose(),64);
	Oyster::Resources::ShaderEffects::ModelEffect.CBuffers.Vertex[1]->Unmap();

	dest = Oyster::Resources::PipeLineResourses::Resources[0]->Map();
	unsigned int bytes=0;
	for(int i=0;i<Oyster::Resources::PipeLineResourses::FrustrumSize;++i)
	{
		//Oyster::Resources::PipeLineResourses::SubFrustrums[i].WriteToByte( (unsigned char*)dest,bytes);
	}
	Oyster::Resources::PipeLineResourses::Resources[0]->Unmap();

	dest = Oyster::Resources::Buffers::CBufferPipelineCs.Map();
	memcpy(dest, &Oyster::Resources::PipeLineResourses::LightData, sizeof( Oyster::Resources::BufferDefinitions::LightStructureBuffer ) );
	Oyster::Resources::Buffers::CBufferPipelineCs.Unmap();

}

void Oyster::Engine::Pipeline::Deffered_Lightning::BeginRenderGeometry()
{
	Oyster::Engine::PrepareForRendering::BindRenderTargets( Oyster::Resources::PipeLineResourses::GeometryTarget, 5 );
	Oyster::Core::ShaderManager::SetShaderEffect( Oyster::Resources::ShaderEffects::ModelEffect );
}

void Oyster::Engine::Pipeline::Deffered_Lightning::RenderGeometry(const Oyster::Render::Model* models,int count)
{
	//TODO: Add sorting to minimiza state changes
	Render::Geometry( models, count, &Oyster::Resources::Buffers::CbufferVS, 2 );
}

void Oyster::Engine::Pipeline::Deffered_Lightning::EndRenderGeometry()
{
	//TODO: Actualy Render data from previous pass
	Oyster::Engine::PrepareForRendering::BindRenderTargets( Oyster::Resources::PipeLineResourses::RtvNulls, 8, NULL );
	Oyster::Core::DeviceContext->PSSetShaderResources(0, 16, Oyster::Resources::PipeLineResourses::SrvNulls);
}

void Oyster::Engine::Pipeline::Deffered_Lightning::InputPointLights(Oyster::Resources::BufferDefinitions::PointLightDescription *p, int nr)
{
	void* dest = Oyster::Resources::PipeLineResourses::Resources[1]->Map();
	memcpy(dest, p, sizeof(Oyster::Resources::BufferDefinitions::PointLightDescription) * nr );
	Oyster::Resources::PipeLineResourses::Resources[1]->Unmap();
}

void Oyster::Engine::Pipeline::Deffered_Lightning::RenderLightning()
{
	Oyster::Engine::PrepareForRendering::BindUAV( Oyster::Resources::PipeLineResourses::LightTarget, 4 );
	Oyster::Core::DeviceContext->CSSetShaderResources(0,5, Oyster::Resources::PipeLineResourses::GeometryOut);
	Oyster::Resources::Buffers::CBufferPipelineCs.Apply();
	Oyster::Core::DeviceContext->CSSetShaderResources( 6, 4, Oyster::Resources::PipeLineResourses::ComputeResources );
	Oyster::Core::ShaderManager::Set::Compute( Oyster::Core::ShaderManager::Get::Compute(L"Pass0") );

	Oyster::Core::DeviceContext->Dispatch( 49, 36, 1 );

	//clean Pipeline
	Oyster::Core::DeviceContext->CSSetShaderResources( 0, 16, Oyster::Resources::PipeLineResourses::SrvNulls );
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews( 0, 8, Oyster::Resources::PipeLineResourses::uavNULL, 0 );

	//Blurr
	//Blur( 2 );
	//Blur( 3 );

	//clean Pipeline
	Oyster::Core::DeviceContext->CSSetShaderResources( 0, 16, Oyster::Resources::PipeLineResourses::SrvNulls );
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews( 0, 8, Oyster::Resources::PipeLineResourses::uavNULL, 0 );

	//prepare and render final pass
	Oyster::Engine::PrepareForRendering::BindBackBufferAsUAV();
	Oyster::Core::ShaderManager::Set::Compute( Oyster::Core::ShaderManager::Get::Compute(L"Pass1") );
	Oyster::Core::DeviceContext->CSSetShaderResources( 0, 4, Oyster::Resources::PipeLineResourses::LightOut );

	Oyster::Core::DeviceContext->Dispatch( 49, 36, 1 );

	//clean Pipeline
	Oyster::Core::DeviceContext->CSSetShaderResources( 0, 16, Oyster::Resources::PipeLineResourses::SrvNulls );
	Oyster::Core::DeviceContext->CSSetUnorderedAccessViews( 0, 8, Oyster::Resources::PipeLineResourses::uavNULL, 0 );
}


//States
int Oyster::Engine::States::GetNrOfSSAOSamples()
{
	return instance.States.NrOfSamples;
}

void Oyster::Engine::States::SetNrOfSSAOSamples(int nr)
{
	instance.States.NrOfSamples = nr;
}

int Oyster::Engine::States::GetSSAOSampleSpread()
{
	return instance.States.SampleSpread;
}

void Oyster::Engine::States::SetSSAOSampleSpread(int spread)
{
	instance.States.SampleSpread = spread;
}

int Oyster::Engine::States::GetMaxPointlights()
{
	return instance.States.NrOfPointlights;
}

void Oyster::Engine::States::SetMaxPointlights(int nr)
{
	instance.States.NrOfPointlights = nr;
}