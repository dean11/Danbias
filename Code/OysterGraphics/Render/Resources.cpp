#pragma once

#include "Resources.h"
#include "..\Definitions\GraphicalDefinition.h"

typedef Oyster::Graphics::Core::PipelineManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::PipelineManager::Get GetShader;
typedef Oyster::Graphics::Core::PipelineManager Shader;
typedef Oyster::Graphics::Core::Buffer Buffer;

const std::wstring PathToHLSL = L"..\\..\\Code\\OysterGraphics\\Shader\\Passes\\";
const std::wstring PathToCSO = L"..\\Content\\Shaders\\";

const int KernelSize = 10;
const int SampleSpread = 16;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{

			ID3D11RenderTargetView* Resources::GBufferRTV[Resources::GBufferSize] = {0};
				ID3D11ShaderResourceView* Resources::GBufferSRV[Resources::GBufferSize] = {0};

				ID3D11UnorderedAccessView* Resources::LBufferUAV[Resources::LBufferSize] = {0};
				ID3D11ShaderResourceView* Resources::LBufferSRV[Resources::LBufferSize] = {0};
				
				ID3D11UnorderedAccessView* Resources::Blur::BufferUAV = {0};
				ID3D11ShaderResourceView* Resources::Blur::BufferSRV = {0};

				Shader::RenderPass Resources::Gather::Pass;
				Shader::RenderPass Resources::Light::Pass;
				Shader::RenderPass Resources::PostPass;
				Shader::RenderPass Resources::Gui::Pass;
				Shader::RenderPass Resources::Blur::VertPass; //Set this pass second when doing a "fullscreen" blur
				Shader::RenderPass Resources::Blur::HorPass;  //Set this pass first when doing a "fullscreen" blur

				Buffer Resources::Gather::ModelData = Buffer();
				Buffer Resources::Gather::AnimationData = Buffer();
				Buffer Resources::Light::LightConstantsData = Buffer();
				Buffer Resources::Gui::Data = Buffer();

				Buffer Resources::Light::PointLightsData = Buffer();
				ID3D11ShaderResourceView* Resources::Light::PointLightView = NULL;

				ID3D11ShaderResourceView* Resources::Light::SSAOKernel = NULL;
				ID3D11ShaderResourceView* Resources::Light::SSAORandom = NULL;

				ID3D11RasterizerState* Resources::RenderStates::rs = NULL;
				ID3D11SamplerState** Resources::RenderStates::ss = new ID3D11SamplerState*[1];
				ID3D11DepthStencilState* Resources::RenderStates::dsState = NULL;


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
					Core::PipelineManager::Init(path + L"GatherPixel" + end, ShaderType::Pixel, L"Gather");
					Core::PipelineManager::Init(path + L"GatherVertex" + end, ShaderType::Vertex, L"Gather");
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

					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_GS;
					desc.NumElements = 1;
					desc.ElementSize = sizeof(Definitions::GuiData);

					desc.ElementSize = sizeof(Definitions::LightConstants);
					desc.NumElements = 1;
					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_CS;
					
					Light::LightConstantsData.Init(desc);

					desc.ElementSize = sizeof(Definitions::Pointlight);
					desc.NumElements = MaxLightSize;
					desc.Type = Buffer::STRUCTURED_BUFFER;
					Light::PointLightsData.Init(desc);
					return Core::Init::Success;
				}

				Core::Init::State Resources::InitRenderStates()
				{
					////Create States
					D3D11_RASTERIZER_DESC rdesc;
					rdesc.CullMode = D3D11_CULL_BACK;
					rdesc.FillMode = D3D11_FILL_SOLID;
					rdesc.FrontCounterClockwise = false;
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
					sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
					sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
					sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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
					return Core::Init::Success;
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

					Buffer* b = &Light::PointLightsData;

					Core::Init::CreateLinkedShaderResourceFromStructuredBuffer(&b,&Light::PointLightView,NULL);
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
								/*(float)rand() / (RAND_MAX + 1) * (1 - -1)+ -1,*/
								1.0f,
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
					Core::device->CreateShaderResourceView( (pTexture2), 0, &Light::SSAORandom );
					pTexture2->Release();
					return Core::Init::Success;
				}

				Core::Init::State Resources::InitPasses()
				{

					////---------------- Geometry Pass Setup ----------------------------
					Gather::Pass.Shaders.Pixel = GetShader::Pixel(L"Gather");
					Gather::Pass.Shaders.Vertex = GetShader::Vertex(L"Gather");

					D3D11_INPUT_ELEMENT_DESC indesc[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					};

					Shader::CreateInputLayout(indesc,7,GetShader::Vertex(L"Gather"),Gather::Pass.IAStage.Layout);
					Gather::Pass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					Gather::Pass.CBuffers.Vertex.push_back(Gather::AnimationData);
					Gather::Pass.CBuffers.Vertex.push_back(Gather::ModelData);
					Gather::Pass.RenderStates.Rasterizer = RenderStates::rs;
					Gather::Pass.RenderStates.SampleCount = 1;
					Gather::Pass.RenderStates.SampleState = RenderStates::ss;
					Gather::Pass.RenderStates.DepthStencil = RenderStates::dsState;
					for(int i = 0; i<GBufferSize;++i)
					{
						Gather::Pass.RTV.push_back(GBufferRTV[i]);
					}
					Gather::Pass.depth = Core::depthStencil;

					////---------------- Light Pass Setup ----------------------------
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

					////---------------- Post Pass Setup ----------------------------
					PostPass.Shaders.Compute = GetShader::Compute(L"PostPass");
					for(int i = 0; i<LBufferSize;++i)
					{
						PostPass.SRV.Compute.push_back(LBufferSRV[i]);
					}
					PostPass.UAV.Compute.push_back(Core::backBufferUAV);

					////---------------- GUI Pass Setup ----------------------------
					Gui::Pass.Shaders.Vertex = GetShader::Vertex(L"2D");
					Gui::Pass.Shaders.Pixel = GetShader::Pixel(L"2D");
					Gui::Pass.Shaders.Geometry = GetShader::Geometry(L"2D");
					Gui::Pass.RTV.push_back(GBufferRTV[2]);

					D3D11_INPUT_ELEMENT_DESC indesc2D[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};

					Shader::CreateInputLayout(indesc2D,1,GetShader::Vertex(L"2D"),Gui::Pass.IAStage.Layout);
					Gui::Pass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
					
					Gui::Pass.RenderStates.SampleCount = 1;
					Gui::Pass.RenderStates.SampleState = RenderStates::ss;

					////---------------- Blur Pass Setup ----------------------------
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
					Light::LightConstantsData.~Buffer();
					Light::PointLightsData.~Buffer();
					Gui::Data.~Buffer();
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

					SAFE_RELEASE(Gather::Pass.IAStage.Layout);

					SAFE_RELEASE(Gather::Pass.RenderStates.BlendState);

					SAFE_RELEASE(Gather::Pass.RenderStates.DepthStencil);

					SAFE_RELEASE(Gather::Pass.RenderStates.Rasterizer);

					for(int i = 0; i < Gather::Pass.RenderStates.SampleCount; ++i)
					{
						SAFE_RELEASE(Gather::Pass.RenderStates.SampleState[i]);
					}
				
					SAFE_DELETE_ARRAY(Gather::Pass.RenderStates.SampleState);

					SAFE_RELEASE(Gui::Pass.IAStage.Layout);
				}
		}
	}
}