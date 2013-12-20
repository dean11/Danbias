#pragma once

#include "Deffered.h"
#include "..\..\Definitions\GraphicalDefinition.h"

typedef Oyster::Graphics::Core::PipelineManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::PipelineManager::Get GetShader;
typedef Oyster::Graphics::Core::PipelineManager Shader;
typedef Oyster::Graphics::Core::Buffer Buffer;

const std::wstring PathToHLSL = L"..\\..\\Code\\OysterGraphics\\Shader\\HLSL\\Deffered Shaders\\";
const std::wstring PathToCSO = L"..\\Content\\Shaders\\";

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Resources
			{

				ID3D11RenderTargetView* Deffered::GBufferRTV[2] = {0};
				ID3D11ShaderResourceView* Deffered::GBufferSRV[2] = {0};


				Shader::RenderPass Deffered::GeometryPass;
				Shader::RenderPass Deffered::LightPass;

				Buffer Deffered::ModelData = Buffer();
				Buffer Deffered::VPData = Buffer();
				Buffer Deffered::LightConstantsData = Buffer();

				Buffer Deffered::PointLightsData = Buffer();
				ID3D11ShaderResourceView* Deffered::PointLightView = NULL;

				Core::Init::State Deffered::Init()
				{
#ifdef _DEBUG
					std::wstring path = PathToHLSL;
					std::wstring end = L".hlsl";
#else
					std::wstring path = PathToCSO;
					std::wstring end = L".cso";
#endif
					//Load Shaders
					Core::PipelineManager::Init(path + L"PixelGatherData" + end, ShaderType::Pixel, L"Geometry");
					Core::PipelineManager::Init(path + L"VertexGatherData" + end, ShaderType::Vertex, L"Geometry");
					Core::PipelineManager::Init(path + L"LightPass" + end, ShaderType::Compute, L"LightPass"); 

					//Create Buffers
					Buffer::BUFFER_INIT_DESC desc;
					desc.ElementSize = sizeof(Oyster::Math::Matrix);
					desc.NumElements = 1;
					desc.InitData = NULL;
					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_VS;
					desc.Usage = Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;

					ModelData.Init(desc);

					desc.NumElements = 2;
					VPData.Init(desc);

					desc.ElementSize = sizeof(Definitions::LightConstants);
					desc.NumElements = 1;
					desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_CS;
					
					LightConstantsData.Init(desc);

					desc.ElementSize = sizeof(Definitions::Pointlight);
					desc.NumElements = MaxLightSize;
					desc.Type = Buffer::STRUCTURED_BUFFER;
					PointLightsData.Init(desc);

					//Create States
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

					ID3D11RasterizerState* rs = NULL;
					Oyster::Graphics::Core::device->CreateRasterizerState(&rdesc,&rs);

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

					ID3D11SamplerState** ss = new ID3D11SamplerState*[1];
					Oyster::Graphics::Core::device->CreateSamplerState(&sdesc,ss);

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

					ID3D11DepthStencilState* dsState;
					Core::device->CreateDepthStencilState(&ddesc,&dsState);

					//Create Views
					for(int i = 0; i< Resources::Deffered::GBufferSize; ++i)
					{
						Core::Init::CreateLinkedShaderResourceFromTexture(&GBufferRTV[i],&GBufferSRV[i],NULL);
					}

					Buffer* b = &PointLightsData;

					Core::Init::CreateLinkedShaderResourceFromStructuredBuffer(&b,&PointLightView,NULL);

					//Create ShaderEffects
					GeometryPass.Shaders.Pixel = GetShader::Pixel(L"Geometry");
					GeometryPass.Shaders.Vertex = GetShader::Vertex(L"Geometry");

					D3D11_INPUT_ELEMENT_DESC indesc[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }

					};

					Shader::CreateInputLayout(indesc,3,GetShader::Vertex(L"Geometry"),GeometryPass.IAStage.Layout);
					GeometryPass.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					GeometryPass.CBuffers.Vertex.push_back(VPData);
					GeometryPass.CBuffers.Vertex.push_back(ModelData);
					GeometryPass.RenderStates.Rasterizer = rs;
					GeometryPass.RenderStates.SampleCount = 1;
					GeometryPass.RenderStates.SampleState = ss;
					GeometryPass.RenderStates.DepthStencil = dsState;
					for(int i = 0; i<Deffered::GBufferSize;++i)
					{
						GeometryPass.RTV.push_back(GBufferRTV[i]);
					}
					GeometryPass.depth = Core::depthStencil;

					LightPass.Shaders.Compute = GetShader::Compute(L"LightPass");
					LightPass.UAV.Compute.push_back(Core::backBufferUAV);
					for(int i = 0; i<Deffered::GBufferSize;++i)
					{
						LightPass.SRV.Compute.push_back(GBufferSRV[i]);
					}
					LightPass.SRV.Compute.push_back(Core::depthStencilUAV);
					LightPass.CBuffers.Compute.push_back(LightConstantsData);
					LightPass.SRV.Compute.push_back(PointLightView);


					return Core::Init::State::Success;
				}

				void Deffered::Clean()
				{
					Resources::Deffered::ModelData.~Buffer();
					Resources::Deffered::VPData.~Buffer();
					Resources::Deffered::LightConstantsData.~Buffer();
					Resources::Deffered::PointLightsData.~Buffer();

					for(int i = 0; i< GBufferSize; ++i)
					{
						SAFE_RELEASE(GBufferRTV[i]);
						SAFE_RELEASE(GBufferSRV[i]);
					}


					for(int i = 0; i < (int)GeometryPass.CBuffers.Vertex.size(); ++i)
					{
						SAFE_RELEASE(GeometryPass.CBuffers.Vertex[i]);
					}
					for(int i = 0; i < (int)GeometryPass.CBuffers.Pixel.size(); ++i)
					{
						SAFE_RELEASE(GeometryPass.CBuffers.Pixel[i]);
					}
					for(int i = 0; i < (int)GeometryPass.CBuffers.Geometry.size(); ++i)
					{
						SAFE_RELEASE(GeometryPass.CBuffers.Geometry[i]);
					}
					for(int i = 0; i < (int)GeometryPass.CBuffers.Compute.size(); ++i)
					{
						SAFE_RELEASE(GeometryPass.CBuffers.Compute[i]);
					}

					SAFE_RELEASE(GeometryPass.IAStage.Layout);

					SAFE_RELEASE(GeometryPass.RenderStates.BlendState);

					SAFE_RELEASE(GeometryPass.RenderStates.DepthStencil);

					SAFE_RELEASE(GeometryPass.RenderStates.Rasterizer);

					for(int i = 0; i < GeometryPass.RenderStates.SampleCount; ++i)
					{
						SAFE_RELEASE(GeometryPass.RenderStates.SampleState[i]);
					}
				
					SAFE_DELETE_ARRAY(GeometryPass.RenderStates.SampleState);
				}
			}
		}
	}
}