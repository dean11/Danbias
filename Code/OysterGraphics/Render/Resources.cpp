#pragma once

#include "Resources.h"
#include "../FileLoader/GeneralLoader.h"
#include "..\Definitions\GraphicalDefinition.h"

typedef Oyster::Graphics::Core::PipelineManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::PipelineManager::Get GetShader;
typedef Oyster::Graphics::Core::PipelineManager Shader;
typedef Oyster::Graphics::Core::Buffer Buffer;

const std::wstring PathToHLSL = L"..\\..\\Code\\OysterGraphics\\Shader\\Passes\\";
const std::wstring PathToCSO = L"..\\Content\\Shaders\\";

const int KernelSize = 10;
const int SampleSpread = 16;


const int MAX_LETTER_COUNT=60;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
				#pragma region Declare Static
				ID3D11RenderTargetView* Resources::GBufferRTV[Resources::GBufferSize] = {0};
				ID3D11ShaderResourceView* Resources::GBufferSRV[Resources::GBufferSize] = {0};

				ID3D11UnorderedAccessView* Resources::LBufferUAV[Resources::LBufferSize] = {0};
				ID3D11ShaderResourceView* Resources::LBufferSRV[Resources::LBufferSize] = {0};
				
				ID3D11UnorderedAccessView* Resources::Blur::BufferUAV = {0};
				ID3D11ShaderResourceView* Resources::Blur::BufferSRV = {0};

				Shader::RenderPass Resources::Gather::AnimatedPass;
				Shader::RenderPass Resources::Gather::InstancedPass;
				Shader::RenderPass Resources::Light::Pass;
				Shader::RenderPass Resources::Post::Pass;
				Shader::RenderPass Resources::Gui::Pass;
				Shader::RenderPass Resources::Gui::Text::Pass;
				Shader::RenderPass Resources::Blur::VertPass; //Set this pass second when doing a "fullscreen" blur
				Shader::RenderPass Resources::Blur::HorPass;  //Set this pass first when doing a "fullscreen" blur

				Buffer Resources::Gather::ModelData = Buffer();
				Buffer Resources::Gather::AnimationData = Buffer();
				Buffer Resources::Gather::InstancedData = Buffer();
				Buffer Resources::Light::LightConstantsData = Buffer();
				Buffer Resources::Gui::Data = Buffer();
				Buffer Resources::Color = Buffer();
				Buffer Resources::Gui::Text::Vertex = Buffer();
				Buffer Resources::Post::Data = Buffer();
				Buffer Resources::Blur::Data = Buffer();

				Buffer Resources::Light::PointLightsData = Buffer();
				ID3D11ShaderResourceView* Resources::Light::PointLightView = NULL;

				ID3D11ShaderResourceView* Resources::Light::SSAOKernel = NULL;
				ID3D11ShaderResourceView* Resources::Light::SSAORandom = NULL;

				ID3D11RasterizerState* Resources::RenderStates::rs = NULL;
				ID3D11SamplerState** Resources::RenderStates::ss = new ID3D11SamplerState*[1];
				ID3D11DepthStencilState* Resources::RenderStates::dsState = NULL;
				ID3D11BlendState* Resources::RenderStates::bs = NULL;

				ID3D11ShaderResourceView* Resources::Gui::Text::Font = NULL;
				ID3D11DepthStencilView* Resources::Gui::depth = NULL;

				ID3D11ShaderResourceView* Resources::Light::Up = NULL;
				ID3D11ShaderResourceView* Resources::Light::Down = NULL;
				ID3D11ShaderResourceView* Resources::Light::Left = NULL;
				ID3D11ShaderResourceView* Resources::Light::Right = NULL;
				ID3D11ShaderResourceView* Resources::Light::Front = NULL;
				ID3D11ShaderResourceView* Resources::Light::Back = NULL;

				ID3D11DepthStencilView* Resources::Gather::NoDepthView = NULL;
				ID3D11ShaderResourceView* Resources::Light::NoDepth = NULL;

				std::map<Model::ModelInfo*, Resources::ModelDataWrapper*> Resources::RenderData = std::map<Model::ModelInfo*, Resources::ModelDataWrapper*>();
				std::map<Model::ModelInfo*, Resources::ModelDataWrapper*> Resources::NoDepthData = std::map<Model::ModelInfo*, Resources::ModelDataWrapper*>();
#pragma endregion


				Core::Init::State Resources::InitShaders()
				{
#ifdef _DEBUG
					std::wstring path = PathToHLSL+L"Gather\\";
					std::wstring end = L".hlsl";
#else
					std::wstring path = PathToCSO;
					std::wstring end = L".cso";
#endif
					//Load Shaders
					Core::PipelineManager::Init(path + L"GatherAnimPixel" + end, ShaderType::Pixel, L"AGather");
					Core::PipelineManager::Init(path + L"GatherAnimVertex" + end, ShaderType::Vertex, L"AGather");

					
					Core::PipelineManager::Init(path + L"GatherInstPixel" + end, ShaderType::Pixel, L"IGather");
					Core::PipelineManager::Init(path + L"GatherInstVertex" + end, ShaderType::Vertex, L"IGather");
#ifdef _DEBUG
					path = PathToHLSL+L"Light\\";
#endif
					Core::PipelineManager::Init(path + L"LightPass" + end, ShaderType::Compute, L"LightPass"); 
#ifdef _DEBUG
					path = PathToHLSL+L"Post\\";
#endif
					Core::PipelineManager::Init(path + L"PostPass" + end, ShaderType::Compute, L"PostPass");
#ifdef _DEBUG
					path = PathToHLSL+L"Blur\\";
#endif
					Core::PipelineManager::Init(path + L"BlurHor" + end, ShaderType::Compute, L"BlurHor");
					Core::PipelineManager::Init(path + L"BlurVert" + end, ShaderType::Compute, L"BlurVert");
#ifdef _DEBUG	
					path = PathToHLSL+L"2D\\";
#endif
					Core::PipelineManager::Init(path + L"2DVertex" + end,ShaderType::Vertex, L"2D");
					Core::PipelineManager::Init(path + L"2DGeometry" + end,ShaderType::Geometry, L"2D");
					Core::PipelineManager::Init(path + L"2DPixel" + end,ShaderType::Pixel, L"2D");
#ifdef _DEBUG
					path = PathToHLSL+L"2D\\Text\\";
#endif
					Core::PipelineManager::Init(path + L"2DTextVertex" + end,ShaderType::Vertex, L"2DText");
					Core::PipelineManager::Init(path + L"2DTextGeometry" + end,ShaderType::Geometry, L"2DText");

					return Core::Init::State::Success;
				}

				Core::Init::State Resources::InitBuffers()
				{
					//Create Buffers
					Buffer::BUFFER_INIT_DESC desc;
					desc.ElementSize = sizeof(Definitions::PerModel);
					desc.NumElements = 1;
					desc.InitData = NULL;
					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_VS;
					desc.Usage = Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;

					Gather::ModelData.Init(desc);

					desc.NumElements = 1;
					desc.ElementSize = sizeof(Definitions::AnimationData);
					Gather::AnimationData.Init(desc);

					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_PS;
					desc.ElementSize = sizeof(Definitions::TintData);
					Color.Init(desc);

					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_GS;
					desc.NumElements = 1;
					desc.ElementSize = sizeof(Definitions::GuiData);
					Gui::Data.Init(desc);

					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_CS;
					desc.ElementSize = sizeof(Definitions::PostData);
					Post::Data.Init(desc);

					desc.ElementSize = sizeof(Definitions::LightConstants);
					Light::LightConstantsData.Init(desc);

					desc.ElementSize = sizeof(Definitions::BlurrData);
					Blur::Data.Init(desc);

					desc.ElementSize = sizeof(Definitions::Pointlight);
					desc.NumElements = MaxLightSize;
					desc.Type = Buffer::STRUCTURED_BUFFER;
					Light::PointLightsData.Init(desc);

					
					Buffer* b = &Light::PointLightsData;

					Core::Init::CreateLinkedShaderResourceFromStructuredBuffer(&b,&Light::PointLightView,NULL);

					desc.Type = Buffer::BUFFER_TYPE::VERTEX_BUFFER;
					desc.ElementSize = sizeof(Definitions::Text2D);
					desc.NumElements = MAX_LETTER_COUNT;
					Gui::Text::Vertex.Init(desc);


					return Core::Init::Success;
				}

				Core::Init::State Resources::InitRenderStates()
				{
					////Create States
					D3D11_RASTERIZER_DESC rdesc;
					rdesc.CullMode = D3D11_CULL_BACK;
					rdesc.FillMode = D3D11_FILL_SOLID;
					rdesc.FrontCounterClockwise = true;
					rdesc.DepthBias = 0;
					rdesc.DepthBiasClamp = 0;
					rdesc.DepthClipEnable = true;
					rdesc.SlopeScaledDepthBias = 0;
					rdesc.ScissorEnable = false;
					rdesc.MultisampleEnable = false;
					rdesc.AntialiasedLineEnable = false;

					
					Oyster::Graphics::Core::device->CreateRasterizerState(&rdesc,&RenderStates::rs);

					D3D11_SAMPLER_DESC sdesc;
					sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
					sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
					sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
					sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
					sdesc.MipLODBias = 0;
					sdesc.MaxAnisotropy =4;
					sdesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
					*sdesc.BorderColor = *Oyster::Math::Float4(1,1,1,1).element;
					sdesc.MinLOD = 0;
					sdesc.MaxLOD = D3D11_FLOAT32_MAX;

					
					Oyster::Graphics::Core::device->CreateSamplerState(&sdesc,RenderStates::ss);

					D3D11_DEPTH_STENCIL_DESC ddesc;
					ddesc.DepthEnable = true;
					ddesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
					ddesc.DepthFunc = D3D11_COMPARISON_LESS;

					ddesc.StencilEnable = true;
					ddesc.StencilReadMask = 0xFF;
					ddesc.StencilWriteMask = 0xFF;

					ddesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					ddesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
					ddesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					ddesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

					ddesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					ddesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
					ddesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					ddesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

					
					Core::device->CreateDepthStencilState(&ddesc,&RenderStates::dsState);

					D3D11_BLEND_DESC bdesc;
					bdesc.AlphaToCoverageEnable = true;
					bdesc.IndependentBlendEnable = false;
					bdesc.RenderTarget[0].BlendEnable = true;

					bdesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
					bdesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
					bdesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

					bdesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
					bdesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
					bdesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;

					bdesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

					Core::device->CreateBlendState(&bdesc,&RenderStates::bs);
					return Core::Init::Success;
				}

				Core::Init::State Resources::ReInitViews(Math::Float2 size)
				{
					//Release Views
					for(int i = 0; i < GBufferSize; ++i)
					{
						SAFE_RELEASE(GBufferRTV[i]);
						SAFE_RELEASE(GBufferSRV[i]);
						Core::UsedMem -= Core::resolution.x * Core::resolution.y * 16;
					}

					//Release Views
					for(int i = 0; i < LBufferSize; ++i)
					{
						SAFE_RELEASE(LBufferUAV[i]);
						SAFE_RELEASE(LBufferSRV[i]);
						Core::UsedMem -= Core::resolution.x * Core::resolution.y * 16;
					}

					SAFE_RELEASE(Gui::depth);
					Core::UsedMem -= Core::resolution.x * Core::resolution.y * 4;

					SAFE_RELEASE(Gather::NoDepthView);
					SAFE_RELEASE(Light::NoDepth);
					Core::UsedMem -= Core::resolution.x * Core::resolution.y * 4;

					SAFE_RELEASE(Blur::BufferSRV);
					SAFE_RELEASE(Blur::BufferUAV);
					Core::UsedMem -= Core::resolution.x * Core::resolution.y * 16;


					Core::resolution = size;

					//Create Views
					for(int i = 0; i< GBufferSize; ++i)
					{
						Core::Init::CreateLinkedShaderResourceFromTexture(&GBufferRTV[i],&GBufferSRV[i],NULL);
					}

					for(int i = 0; i < LBufferSize; ++i)
					{
						Core::Init::CreateLinkedShaderResourceFromTexture(NULL,&LBufferSRV[i],&LBufferUAV[i]);
					}

					//Blur
					Core::Init::CreateLinkedShaderResourceFromTexture(NULL,&Blur::BufferSRV,&Blur::BufferUAV);

					//GUI
					//create Depth Buffer
					D3D11_TEXTURE2D_DESC dTDesc;
					dTDesc.MipLevels=1;
					dTDesc.ArraySize=1;
					dTDesc.Format = DXGI_FORMAT_R32_TYPELESS;
					dTDesc.Usage = D3D11_USAGE_DEFAULT;
					dTDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
					dTDesc.CPUAccessFlags=0;
					dTDesc.MiscFlags=0;
					dTDesc.Height = (UINT)Core::resolution.y;
					dTDesc.Width = (UINT)Core::resolution.x;
					dTDesc.SampleDesc.Count=1;
					dTDesc.SampleDesc.Quality=0;

					D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
					dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					dsvDesc.Flags = 0;
					dsvDesc.Texture2D.MipSlice = 0;

					ID3D11Texture2D* depthstencil;
					Core::device->CreateTexture2D(&dTDesc,0,&depthstencil);
					Core::UsedMem += dTDesc.Height * dTDesc.Width * 4;
					Core::device->CreateDepthStencilView(depthstencil,&dsvDesc,&Gui::depth);
					depthstencil->Release();

					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;
					srvDesc.Texture2D.MostDetailedMip = 0;

					Core::device->CreateTexture2D(&dTDesc,0,&depthstencil);
					Core::UsedMem += dTDesc.Height * dTDesc.Width * 4;
					Core::device->CreateDepthStencilView(depthstencil,&dsvDesc,&Gather::NoDepthView);
					Core::device->CreateShaderResourceView(depthstencil,&srvDesc,&Light::NoDepth);
					depthstencil->Release();

					//Reset Passes--------------------------------------------------------------------------

					//Geometry
					for(int i = 0; i<GBufferSize;++i)
					{
						Gather::AnimatedPass.RTV[i] = GBufferRTV[i];
					}
					Gather::AnimatedPass.depth = Core::depthStencil;
					//Instanced Geometry
					for(int i = 0; i<GBufferSize;++i)
					{
						Gather::InstancedPass.RTV[i]  = GBufferRTV[i];
					}
					Gather::InstancedPass.depth = Core::depthStencil;
					//Light Pass
					for(int i = 0; i<LBufferSize;++i)
					{
						Light::Pass.UAV.Compute[i] = LBufferUAV[i];
					}
					for(int i = 0; i<GBufferSize;++i)
					{
						Light::Pass.SRV.Compute[i] = GBufferSRV[i];
					}
					Light::Pass.SRV.Compute[3] = Core::depthStencilUAV;
					Light::Pass.SRV.Compute[13] = Light::NoDepth;
					//Post Pass
					for(int i = 0; i<LBufferSize;++i)
					{
						Post::Pass.SRV.Compute[i] = LBufferSRV[i];
					}
					Post::Pass.UAV.Compute[0] = Core::backBufferUAV;
					//GUI Pass
					Gui::Pass.RTV[0] = GBufferRTV[2];
					Gui::Pass.depth = Gui::depth;
					//Blur Pass 
					Blur::HorPass.SRV.Compute[0] = LBufferSRV[2]; 
					Blur::HorPass.UAV.Compute[0] = Blur::BufferUAV;
					Blur::VertPass.SRV.Compute[0] = Blur::BufferSRV;
					Blur::VertPass.UAV.Compute[0] = LBufferUAV[2];
					//Text Pass
					Gui::Text::Pass.RTV[0] = GBufferRTV[2];
					Gui::Text::Pass.depth = Gui::depth;
					
					return Core::Init::State::Success;
				}

				Core::Init::State Resources::InitViews()
				{
					//Create Views
					for(int i = 0; i< GBufferSize; ++i)
					{
						Core::Init::CreateLinkedShaderResourceFromTexture(&GBufferRTV[i],&GBufferSRV[i],NULL);
					}

					for(int i = 0; i < LBufferSize; ++i)
					{
						Core::Init::CreateLinkedShaderResourceFromTexture(NULL,&LBufferSRV[i],&LBufferUAV[i]);
					}

					//Blur
					Core::Init::CreateLinkedShaderResourceFromTexture(NULL,&Blur::BufferSRV,&Blur::BufferUAV);

					srand((unsigned int)time(0));

					//SSAO
					Math::Vector3 kernel[KernelSize];
					Math::Vector3 random[SampleSpread];
					for(int i = 0;i < KernelSize; ++i)
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

						float scale = float(i) / float(KernelSize);

						scale = (0.1f*(1 - scale * scale) + 1.0f *( scale * scale));
						kernel[i] *= scale;

						
					}

					for( int i = 0; i < SampleSpread; ++i)
					{
						random[i] = Oyster::Math::Vector3::null;
						while( random[i] == Oyster::Math::Vector3::null )
						{
							random[i] = Oyster::Math::Vector3(
								(float)rand() / (RAND_MAX + 1) * (1 - -1)+ -1,
								(float)rand() / (RAND_MAX + 1) * (1 - -1)+ -1,
								//1.0f,
								0.0f);
						}
						random[i].Normalize();
					}

					D3D11_TEXTURE1D_DESC T1desc;
					T1desc.Width = KernelSize;
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
					rnd.SysMemPitch = (UINT)(sqrt(SampleSpread) * sizeof(Oyster::Math::Vector3));

					ID3D11Texture1D *pTexture1;

					 Core::device->CreateTexture1D( &T1desc, &sphere, &pTexture1 );
					Core::UsedMem += T1desc.Width * 16;
					 Core::device->CreateShaderResourceView( pTexture1, 0, &Light::SSAOKernel );
					pTexture1->Release();

					D3D11_TEXTURE2D_DESC T2desc;
					T2desc.Width = KernelSize;
					T2desc.MipLevels = T2desc.ArraySize = 1;
					T2desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
					T2desc.Usage = D3D11_USAGE_DEFAULT;
					T2desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					T2desc.CPUAccessFlags = 0;
					T2desc.MiscFlags = 0;
					T2desc.Height = (UINT)sqrt(SampleSpread);
					T2desc.Width = (UINT)(SampleSpread/sqrt(SampleSpread));
					T2desc.SampleDesc.Quality = 0;
					T2desc.SampleDesc.Count = 1;

					ID3D11Texture2D *pTexture2;

					Core::device->CreateTexture2D( &T2desc, &rnd, &pTexture2 );
					Core::UsedMem += T2desc.Height * T2desc.Width * 16;
					Core::device->CreateShaderResourceView( (pTexture2), 0, &Light::SSAORandom );
					pTexture2->Release();

					//create Depth Buffer
					D3D11_TEXTURE2D_DESC dTDesc;
					dTDesc.MipLevels=1;
					dTDesc.ArraySize=1;
					dTDesc.Format = DXGI_FORMAT_R32_TYPELESS;
					dTDesc.Usage = D3D11_USAGE_DEFAULT;
					dTDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
					dTDesc.CPUAccessFlags=0;
					dTDesc.MiscFlags=0;
					dTDesc.Height = (UINT)Core::resolution.y;
					dTDesc.Width = (UINT)Core::resolution.x;
					dTDesc.SampleDesc.Count=1;
					dTDesc.SampleDesc.Quality=0;

					D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
					dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					dsvDesc.Flags = 0;
					dsvDesc.Texture2D.MipSlice = 0;

					ID3D11Texture2D* depthstencil;
					Core::device->CreateTexture2D(&dTDesc,0,&depthstencil);
					Core::UsedMem += dTDesc.Height * dTDesc.Width * 4;
					Core::device->CreateDepthStencilView(depthstencil,&dsvDesc,&Gui::depth);
					depthstencil->Release();

					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;
					srvDesc.Texture2D.MostDetailedMip = 0;

					Core::device->CreateTexture2D(&dTDesc,0,&depthstencil);
					Core::UsedMem += dTDesc.Height * dTDesc.Width * 4;
					Core::device->CreateDepthStencilView(depthstencil,&dsvDesc,&Gather::NoDepthView);
					Core::device->CreateShaderResourceView(depthstencil,&srvDesc,&Light::NoDepth);
					depthstencil->Release();

					Light::Up = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
					Light::Down = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
					Light::Left = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
					Light::Right = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
					Light::Front = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);
					Light::Back = (ID3D11ShaderResourceView*)Core::loader.LoadResource((Core::texturePath + L"stars_up.png").c_str(),Oyster::Graphics::Loading::LoadTexture, Oyster::Graphics::Loading::UnloadTexture);

					return Core::Init::Success;
				}

				Core::Init::State Resources::InitPasses()
				{
					#pragma region Animated Pass Setup
					Gather::AnimatedPass.Shaders.Pixel = GetShader::Pixel(L"AGather");
					Gather::AnimatedPass.Shaders.Vertex = GetShader::Vertex(L"AGather");

					D3D11_INPUT_ELEMENT_DESC AnimInDesc[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					};

					Shader::CreateInputLayout(AnimInDesc,5,GetShader::Vertex(L"AGather"),Gather::AnimatedPass.IAStage.Layout);
					Gather::AnimatedPass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					Gather::AnimatedPass.CBuffers.Vertex.push_back(Gather::AnimationData);
					Gather::AnimatedPass.CBuffers.Vertex.push_back(Gather::ModelData);
					Gather::AnimatedPass.CBuffers.Pixel.push_back(Color);
					Gather::AnimatedPass.RenderStates.Rasterizer = RenderStates::rs;
					Gather::AnimatedPass.RenderStates.SampleCount = 1;
					Gather::AnimatedPass.RenderStates.SampleState = RenderStates::ss;
					Gather::AnimatedPass.RenderStates.DepthStencil = RenderStates::dsState;
					for(int i = 0; i<GBufferSize;++i)
					{
						Gather::AnimatedPass.RTV.push_back(GBufferRTV[i]);
					}
					Gather::AnimatedPass.depth = Core::depthStencil;
					#pragma endregion

					#pragma region Instanced Pass Setup
					Gather::InstancedPass.Shaders.Pixel = GetShader::Pixel(L"IGather");
					Gather::InstancedPass.Shaders.Vertex = GetShader::Vertex(L"IGather");

					D3D11_INPUT_ELEMENT_DESC InstInDesc[15];

					InstInDesc[0].AlignedByteOffset = 0;
					InstInDesc[0].SemanticName = "POSITION";
					InstInDesc[0].SemanticIndex = 0;
					InstInDesc[0].InputSlot = 0;
					InstInDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					InstInDesc[0].InstanceDataStepRate = 0;
					InstInDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

					InstInDesc[1].AlignedByteOffset = 12;
					InstInDesc[1].SemanticName = "TEXCOORD";
					InstInDesc[1].SemanticIndex = 0;
					InstInDesc[1].InputSlot = 0;
					InstInDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					InstInDesc[1].InstanceDataStepRate = 0;
					InstInDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;

					InstInDesc[2].AlignedByteOffset = 20;
					InstInDesc[2].SemanticName = "NORMAL";
					InstInDesc[2].SemanticIndex = 0;
					InstInDesc[2].InputSlot = 0;
					InstInDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					InstInDesc[2].InstanceDataStepRate = 0;
					InstInDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;

					InstInDesc[3].AlignedByteOffset = 36;
					InstInDesc[3].SemanticName = "BONEINDEX";
					InstInDesc[3].SemanticIndex = 0;
					InstInDesc[3].InputSlot = 0;
					InstInDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					InstInDesc[3].InstanceDataStepRate = 0;
					InstInDesc[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[4].AlignedByteOffset = 48;
					InstInDesc[4].SemanticName = "BONEWEIGHT";
					InstInDesc[4].SemanticIndex = 0;
					InstInDesc[4].InputSlot = 0;
					InstInDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					InstInDesc[4].InstanceDataStepRate = 0;
					InstInDesc[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					/// Set WV
					InstInDesc[5].AlignedByteOffset = 0;
					InstInDesc[5].SemanticName = "WV";
					InstInDesc[5].SemanticIndex = 0;
					InstInDesc[5].InputSlot = 1;
					InstInDesc[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[5].InstanceDataStepRate = 1;
					InstInDesc[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[6].AlignedByteOffset = 16;
					InstInDesc[6].SemanticName = "WV";
					InstInDesc[6].SemanticIndex = 1;
					InstInDesc[6].InputSlot = 1;
					InstInDesc[6].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[6].InstanceDataStepRate = 1;
					InstInDesc[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[7].AlignedByteOffset = 32;
					InstInDesc[7].SemanticName = "WV";
					InstInDesc[7].SemanticIndex = 2;
					InstInDesc[7].InputSlot = 1;
					InstInDesc[7].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[7].InstanceDataStepRate = 1;
					InstInDesc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[8].AlignedByteOffset = 48;
					InstInDesc[8].SemanticName = "WV";
					InstInDesc[8].SemanticIndex = 3;
					InstInDesc[8].InputSlot = 1;
					InstInDesc[8].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[8].InstanceDataStepRate = 1;
					InstInDesc[8].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					//WVP

					InstInDesc[9].AlignedByteOffset = 64;
					InstInDesc[9].SemanticName = "WVP";
					InstInDesc[9].SemanticIndex = 0;
					InstInDesc[9].InputSlot = 1;
					InstInDesc[9].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[9].InstanceDataStepRate = 1;
					InstInDesc[9].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[10].AlignedByteOffset = 80;
					InstInDesc[10].SemanticName = "WVP";
					InstInDesc[10].SemanticIndex = 1;
					InstInDesc[10].InputSlot = 1;
					InstInDesc[10].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[10].InstanceDataStepRate = 1;
					InstInDesc[10].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[11].AlignedByteOffset = 96;
					InstInDesc[11].SemanticName = "WVP";
					InstInDesc[11].SemanticIndex = 2;
					InstInDesc[11].InputSlot = 1;
					InstInDesc[11].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[11].InstanceDataStepRate = 1;
					InstInDesc[11].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[12].AlignedByteOffset = 112;
					InstInDesc[12].SemanticName = "WVP";
					InstInDesc[12].SemanticIndex = 3;
					InstInDesc[12].InputSlot = 1;
					InstInDesc[12].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[12].InstanceDataStepRate = 1;
					InstInDesc[12].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

					InstInDesc[13].AlignedByteOffset = 128;
					InstInDesc[13].SemanticName = "TINT";
					InstInDesc[13].SemanticIndex = 0;
					InstInDesc[13].InputSlot = 1;
					InstInDesc[13].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[13].InstanceDataStepRate = 1;
					InstInDesc[13].Format = DXGI_FORMAT_R32G32B32_FLOAT;

					InstInDesc[14].AlignedByteOffset = 140;
					InstInDesc[14].SemanticName = "GTINT";
					InstInDesc[14].SemanticIndex = 0;
					InstInDesc[14].InputSlot = 1;
					InstInDesc[14].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					InstInDesc[14].InstanceDataStepRate = 1;
					InstInDesc[14].Format = DXGI_FORMAT_R32G32B32_FLOAT;

					Shader::CreateInputLayout(InstInDesc,15,GetShader::Vertex(L"IGather"),Gather::InstancedPass.IAStage.Layout);
					Gather::InstancedPass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					Gather::InstancedPass.RenderStates.Rasterizer = RenderStates::rs;
					Gather::InstancedPass.RenderStates.SampleCount = 1;
					Gather::InstancedPass.RenderStates.SampleState = RenderStates::ss;
					Gather::InstancedPass.RenderStates.DepthStencil = RenderStates::dsState;
					for(int i = 0; i<GBufferSize;++i)
					{
						Gather::InstancedPass.RTV.push_back(GBufferRTV[i]);
					}
					Gather::InstancedPass.depth = Core::depthStencil;
					#pragma endregion

					#pragma region Light Pass Setup
					Light::Pass.Shaders.Compute = GetShader::Compute(L"LightPass");
					for(int i = 0; i<LBufferSize;++i)
					{
						Light::Pass.UAV.Compute.push_back(LBufferUAV[i]);
					}
					for(int i = 0; i<GBufferSize;++i)
					{
						Light::Pass.SRV.Compute.push_back(GBufferSRV[i]);
					}
					Light::Pass.SRV.Compute.push_back(Core::depthStencilUAV);
					Light::Pass.CBuffers.Compute.push_back(Light::LightConstantsData);
					Light::Pass.SRV.Compute.push_back(Light::PointLightView);
					Light::Pass.SRV.Compute.push_back(Light::SSAOKernel);
					Light::Pass.SRV.Compute.push_back(Light::SSAORandom);
					Light::Pass.SRV.Compute.push_back(Light::Up);
					Light::Pass.SRV.Compute.push_back(Light::Down);
					Light::Pass.SRV.Compute.push_back(Light::Left);
					Light::Pass.SRV.Compute.push_back(Light::Right);
					Light::Pass.SRV.Compute.push_back(Light::Front);
					Light::Pass.SRV.Compute.push_back(Light::Back);
					Light::Pass.SRV.Compute.push_back(Light::NoDepth);
					#pragma endregion

					#pragma region Post Pass Setup
					Post::Pass.Shaders.Compute = GetShader::Compute(L"PostPass");
					for(int i = 0; i<LBufferSize;++i)
					{
						Post::Pass.SRV.Compute.push_back(LBufferSRV[i]);
					}
					Post::Pass.UAV.Compute.push_back(Core::backBufferUAV);
					Post::Pass.CBuffers.Compute.push_back(Post::Data);
					Post::Pass.RenderStates.SampleCount = 1;
					Post::Pass.RenderStates.SampleState = RenderStates::ss;
					#pragma endregion

					#pragma region GUI Pass Setup
					Gui::Pass.Shaders.Vertex = GetShader::Vertex(L"2D");
					Gui::Pass.Shaders.Pixel = GetShader::Pixel(L"2D");
					Gui::Pass.Shaders.Geometry = GetShader::Geometry(L"2D");

					Gui::Pass.RTV.push_back(GBufferRTV[2]);
					Gui::Pass.CBuffers.Geometry.push_back(Gui::Data);
					Gui::Pass.CBuffers.Pixel.push_back(Color);

					Gui::Pass.depth = Gui::depth;

					D3D11_INPUT_ELEMENT_DESC indesc2D[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};

					Shader::CreateInputLayout(indesc2D,1,GetShader::Vertex(L"2D"),Gui::Pass.IAStage.Layout);
					Gui::Pass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
					
					Gui::Pass.RenderStates.SampleCount = 1;
					Gui::Pass.RenderStates.SampleState = RenderStates::ss;
					Gui::Pass.RenderStates.BlendState = RenderStates::bs;
					Gui::Pass.RenderStates.DepthStencil = RenderStates::dsState;
					#pragma endregion

					#pragma region Blur Pass Setup
					Blur::HorPass.Shaders.Compute = GetShader::Compute(L"BlurHor");
					Blur::VertPass.Shaders.Compute = GetShader::Compute(L"BlurVert");

					//Taking the Ambient SRV from LBufferSRV and setting it as input texture
					Blur::HorPass.SRV.Compute.push_back(LBufferSRV[2]); 
					//Output texture is the Blur UAV buffer
					Blur::HorPass.UAV.Compute.push_back(Blur::BufferUAV);

					//Taking the Blur SRV and setting it as input texture now
					Blur::VertPass.SRV.Compute.push_back(Blur::BufferSRV);
					//And the Ambient UAV is now the output texture
					Blur::VertPass.UAV.Compute.push_back(LBufferUAV[2]);

					Blur::HorPass.CBuffers.Compute.push_back(Blur::Data);
					Blur::VertPass.CBuffers.Compute.push_back(Blur::Data);
					#pragma endregion

					#pragma region //2DText Pass Setup
					Gui::Text::Pass.Shaders.Vertex = GetShader::Vertex(L"2DText");
					Gui::Text::Pass.Shaders.Geometry = GetShader::Geometry(L"2DText");
					Gui::Text::Pass.Shaders.Pixel = GetShader::Pixel(L"2D");

					Gui::Text::Pass.IAStage.Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

					D3D11_INPUT_ELEMENT_DESC Text2Ddesc[] =
					{
						{"Position",0,		DXGI_FORMAT_R32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0},
						{"Offset",0,		DXGI_FORMAT_R32_SINT,	0,	4,	D3D11_INPUT_PER_VERTEX_DATA,	0},
						{"CharOffset",0,	DXGI_FORMAT_R32_FLOAT,	0,	8,	D3D11_INPUT_PER_VERTEX_DATA,	0},
					};

					Shader::CreateInputLayout(Text2Ddesc,3, GetShader::Vertex(L"2DText") ,Gui::Text::Pass.IAStage.Layout);
					Gui::Text::Pass.CBuffers.Geometry.push_back(Gui::Data);
					Gui::Text::Pass.CBuffers.Pixel.push_back(Color);
					Gui::Text::Pass.SRV.Pixel.push_back(Gui::Text::Font);
					Gui::Text::Pass.RTV.push_back(GBufferRTV[2]);

					Gui::Text::Pass.depth = Gui::depth;

					Gui::Text::Pass.RenderStates.SampleCount = 1;
					Gui::Text::Pass.RenderStates.SampleState = RenderStates::ss;
					Gui::Text::Pass.RenderStates.BlendState = RenderStates::bs;
					Gui::Text::Pass.RenderStates.DepthStencil = RenderStates::dsState;
					#pragma endregion

					return Core::Init::Success;
				}

				Core::Init::State Resources::Init()
				{
					InitShaders();
					InitBuffers();
					InitRenderStates();
					InitViews();
					InitPasses();

					return Core::Init::State::Success;
				}

				void Resources::Clean()
				{
					Gather::ModelData.~Buffer();
					Gather::AnimationData.~Buffer();
					Gather::InstancedData.~Buffer();
					Light::LightConstantsData.~Buffer();
					Light::PointLightsData.~Buffer();
					Gui::Data.~Buffer();
					Color.~Buffer();
					Gui::Text::Vertex.~Buffer();
					Post::Data.~Buffer();
					Blur::Data.~Buffer();
					SAFE_RELEASE(Light::PointLightView);
					SAFE_RELEASE(Light::SSAOKernel);
					SAFE_RELEASE(Light::SSAORandom);

					SAFE_RELEASE(Blur::BufferSRV);
					SAFE_RELEASE(Blur::BufferUAV);

					for(int i = 0; i< GBufferSize; ++i)
					{
						SAFE_RELEASE(GBufferRTV[i]);
						SAFE_RELEASE(GBufferSRV[i]);
					}

					for(int i = 0; i< LBufferSize; ++i)
					{
						SAFE_RELEASE(LBufferUAV[i]);
						SAFE_RELEASE(LBufferSRV[i]);
					}

					SAFE_RELEASE(Gather::AnimatedPass.IAStage.Layout);
					SAFE_RELEASE(Gather::InstancedPass.IAStage.Layout);

					SAFE_RELEASE(Gather::AnimatedPass.RenderStates.BlendState);

					SAFE_RELEASE(Gather::AnimatedPass.RenderStates.DepthStencil);

					SAFE_RELEASE(Gather::AnimatedPass.RenderStates.Rasterizer);

					for(int i = 0; i < Gather::AnimatedPass.RenderStates.SampleCount; ++i)
					{
						SAFE_RELEASE(Gather::AnimatedPass.RenderStates.SampleState[i]);
					}
				
					SAFE_DELETE_ARRAY(Gather::AnimatedPass.RenderStates.SampleState);

					SAFE_RELEASE(Gui::Pass.IAStage.Layout);

					SAFE_RELEASE(Gui::Text::Pass.RenderStates.BlendState);

					SAFE_RELEASE(Gui::Text::Pass.IAStage.Layout);

					SAFE_RELEASE(Gui::depth);

					SAFE_RELEASE(Gather::NoDepthView);
					SAFE_RELEASE(Light::NoDepth);
				}
		}
	}
}