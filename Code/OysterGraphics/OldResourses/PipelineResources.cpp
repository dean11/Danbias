#include "PipelineResources.h"

using namespace Oyster::Resources;

ID3D11UnorderedAccessView* PipeLineResourses::TempUav = 0;
ID3D11ShaderResourceView* PipeLineResourses::TempSrv = 0;

ID3D11ShaderResourceView* PipeLineResourses::GeometryOut[5] = {0};
ID3D11RenderTargetView* PipeLineResourses::GeometryTarget[5] = {0};

ID3D11ShaderResourceView* PipeLineResourses::ComputeResources[4] = {0};
Oyster::Buffer* PipeLineResourses::Resources[2] = {0};

ID3D11ShaderResourceView* PipeLineResourses::LightOut[4] = {0};
ID3D11UnorderedAccessView* PipeLineResourses::LightTarget[4] = {0};

ID3D11RenderTargetView* PipeLineResourses::RtvNulls[16] = {0};
ID3D11ShaderResourceView* PipeLineResourses::SrvNulls[16] = {0};
ID3D11UnorderedAccessView* PipeLineResourses::uavNULL[16] = {0};

//Oyster::Collision3D::Frustrum* PipeLineResourses::SubFrustrums = 0;
int PipeLineResourses::FrustrumSize = 0;
LinearAlgebra::Vector3<unsigned int> PipeLineResourses::FrustrumDimensions = LinearAlgebra::Vector3<unsigned int>();

Oyster::Resources::BufferDefinitions::LightStructureBuffer PipeLineResourses::LightData = Oyster::Resources::BufferDefinitions::LightStructureBuffer();

void PipeLineResourses::Init(int sizeX, int sizeY)
{
	InitGeometry(sizeX, sizeY);
	
	InitSSAOData();
	InitSubFrustrums(sizeX, sizeY);
	InitPointLights();
	InitLightData();

	InitLighting(sizeX, sizeY);
}

void PipeLineResourses::InitGeometry(int sizeX, int sizeY)
{
	D3D11_TEXTURE2D_DESC Tdesc;
	Tdesc.Width = sizeX;
	Tdesc.Height = sizeY;
	Tdesc.MipLevels = Tdesc.ArraySize = 1;
	Tdesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Tdesc.SampleDesc.Count = 1;
	Tdesc.SampleDesc.Quality=0;
	Tdesc.Usage = D3D11_USAGE_DEFAULT;
	Tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Tdesc.CPUAccessFlags = 0;
	Tdesc.MiscFlags = 0;

	ID3D11Texture2D *pTexture;
	HRESULT hr;

	//Geometry stage resourses
	for( int i = 0; i < 5; ++i )
	{
		hr = Oyster::Core::Device->CreateTexture2D( &Tdesc, NULL, &pTexture );
		hr = Oyster::Core::Device->CreateShaderResourceView(pTexture,0,&GeometryOut[i]);
		hr = Oyster::Core::Device->CreateRenderTargetView(pTexture,0,&GeometryTarget[i]);
		pTexture->Release();
	}
}

void PipeLineResourses::InitSSAOData()
{
	//create Half Spheres and Random Data

	Oyster::Buffer::BUFFER_INIT_DESC desc;
	HRESULT hr;

	int NrOfSamples = Oyster::Engine::States::GetNrOfSSAOSamples();
	int SampleSpread = Oyster::Engine::States::GetSSAOSampleSpread();

	Oyster::Math::Vector3* kernel = new Oyster::Math::Vector3[ NrOfSamples ];
	Oyster::Math::Vector3* random = new Oyster::Math::Vector3[ SampleSpread ];

	for(int i = 0; i < NrOfSamples; ++i)
	{
		kernel[i] = Oyster::Math::Vector3::null;
		while( kernel[i] == Oyster::Math::Vector3::null )
		{
			kernel[i] = Oyster::Math::Vector3(
				(float)rand() / (RAND_MAX + 1) * (1 - -1) + -1,
				(float)rand() / (RAND_MAX + 1) * (1 - -1) + -1,
				(float)rand() / (RAND_MAX + 1) * (1 - 0) + 0);
		}
		kernel[i].Normalize();

		float scale = float(i) / float(NrOfSamples);
		scale = (0.1f*(1 - scale * scale) + 1.0f *( scale * scale));
		kernel[i] *= scale;

		if( i < SampleSpread)
		{
			random[i] = Oyster::Math::Vector3::null;
			while( random[i] == Oyster::Math::Vector3::null )
			{
				random[i] = Oyster::Math::Vector3(
					(float)rand() / (RAND_MAX + 1) * (1 - -1)+ -1,
					(float)rand() / (RAND_MAX + 1) * (1 - -1)+ -1,
					0.0f);
			}
			random[i].Normalize();
		}
	}


	D3D11_TEXTURE1D_DESC T1desc;
	T1desc.Width = NrOfSamples;
	T1desc.MipLevels = T1desc.ArraySize = 1;
	T1desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	T1desc.Usage = D3D11_USAGE_DEFAULT;
	T1desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	T1desc.CPUAccessFlags = 0;
	T1desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sphere;
	sphere.pSysMem = kernel;

	D3D11_SUBRESOURCE_DATA rnd;
	rnd.pSysMem = random;


	ID3D11Texture1D *pTexture1[2];

	hr = Oyster::Core::Device->CreateTexture1D( &T1desc, &sphere, &pTexture1[0] );
	hr = Oyster::Core::Device->CreateShaderResourceView( pTexture1[0], 0, &ComputeResources[3] );
	pTexture1[0]->Release();
	delete[] kernel;

	T1desc.Width = SampleSpread;
	hr = Oyster::Core::Device->CreateTexture1D( &T1desc, &rnd, &pTexture1[1] );
	hr = Oyster::Core::Device->CreateShaderResourceView( (pTexture1[1]), 0, &ComputeResources[2] );
	pTexture1[1]->Release();
	delete[] random;
}

void PipeLineResourses::InitSubFrustrums(int sizeX, int sizeY)
{
	FrustrumDimensions.x = (sizeX + 15U) / 16U;
	FrustrumDimensions.y = (sizeY + 15U) / 16U;
	FrustrumDimensions.z = 1;
	FrustrumSize = FrustrumDimensions.x * FrustrumDimensions.y * FrustrumDimensions.z;
	//if(SubFrustrums!=0)
		//delete[] SubFrustrums;
	//SubFrustrums = new Collision3D::Frustrum[ FrustrumSize ];

	Oyster::Buffer::BUFFER_INIT_DESC desc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//buffer description for SubFrustrums
	desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
	desc.Type = Oyster::Buffer::STRUCTURED_BUFFER;
	desc.ElementSize = sizeof( ::Oyster::Resources::BufferDefinitions::ScreenTileFrustrum);
	desc.NumElements = FrustrumSize;
	desc.InitData = NULL;

	//create matching srv
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = FrustrumSize;

	PipeLineResourses::Resources[0] = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	HRESULT hr = Oyster::Core::Device->CreateShaderResourceView( *PipeLineResourses::Resources[0], &srvDesc, &Oyster::Resources::PipeLineResourses::ComputeResources[0] );
}

void PipeLineResourses::InitPointLights()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	Oyster::Buffer::BUFFER_INIT_DESC desc;
	HRESULT hr;

	//buffer description for pointlight
	desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
	desc.Type = Oyster::Buffer::STRUCTURED_BUFFER;
	desc.ElementSize = sizeof(Oyster::Resources::BufferDefinitions::PointLightDescription);
	desc.NumElements = Oyster::Engine::States::GetMaxPointlights();
	desc.InitData = NULL;

	PipeLineResourses::Resources[1] = Oyster::Engine::Init::Buffers::CreateBuffer(desc);

	//create matching srv
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = Oyster::Engine::States::GetMaxPointlights();

	hr = Oyster::Core::Device->CreateShaderResourceView( *PipeLineResourses::Resources[1], &srvDesc, &Oyster::Resources::PipeLineResourses::ComputeResources[1] );
}

void PipeLineResourses::InitLightData()
{
	LightData.numDispatches = FrustrumDimensions;
}

void PipeLineResourses::InitLighting(int sizeX, int sizeY)
{
	D3D11_TEXTURE2D_DESC Tdesc;
	Tdesc.Width = sizeX;
	Tdesc.Height = sizeY;
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
		hr = Oyster::Core::Device->CreateShaderResourceView( pTexture, 0, &(LightOut[i]) );
		hr = Oyster::Core::Device->CreateUnorderedAccessView( pTexture, 0, &(LightTarget[i]) );
		pTexture->Release();
	}

	hr = Oyster::Core::Device->CreateTexture2D( &Tdesc, NULL, &pTexture );
	hr = Oyster::Core::Device->CreateShaderResourceView( pTexture, 0, &TempSrv );
	hr = Oyster::Core::Device->CreateUnorderedAccessView( pTexture, 0, &TempUav );
	pTexture->Release();
}