#include "Resources.h"
#include "..\OysterGraphics\Definitions\GraphicalDefinition.h"

// /Bin/Executable/Tester ->
// /Code/OysterGraphics/Shader/HLSL
const std::wstring PathFromExeToCso = L"..\\Content\\Shaders\\";
const std::wstring PathFromExeToHlsl = L"..\\..\\Code\\OysterGraphics\\Shader\\HLSL\\";
const std::wstring VertexTransformDebug = L"TransformDebugVertex";
const std::wstring VertexDebug = L"DebugVertex";
const std::wstring PixelRed = L"DebugPixel";
const std::wstring PixelTexture = L"Texture";

typedef Oyster::Graphics::Core::ShaderManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::ShaderManager::Get GetShader;
typedef Oyster::Graphics::Core::ShaderManager Shader;
typedef Oyster::Graphics::Core::Buffer Buffer;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			Shader::ShaderEffect Resources::obj;// = Shader::ShaderEffect();;
			Buffer Resources::ModelData = Buffer();
			Buffer Resources::VPData = Buffer();

			Core::Init::State Resources::Init()
			{

#pragma region LoadShaders

#ifdef _DEBUG

				/** Load Vertex Shader for d3dcompile*/
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugCameraVertex.hlsl",ShaderType::Vertex, VertexTransformDebug);
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugVertex.hlsl",ShaderType::Vertex, VertexDebug);

				/** Load Pixel Shader for d3dcompile */
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" + L"DebugPixel.hlsl", ShaderType::Pixel, PixelRed);
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" + L"TextureDebug.hlsl", ShaderType::Pixel, PixelTexture);
				

#else
				/** Load Vertex Shader with Precompiled */
				Core::ShaderManager::Init(PathFromExeToCso + L"DebugCameraVertex.cso",ShaderType::Vertex, VertexTransformDebug);
				Core::ShaderManager::Init(PathFromExeToCso + L"DebugVertex.cso",ShaderType::Vertex, VertexDebug);

				/** Load Pixel Shader with Precompiled */
				Core::ShaderManager::Init(PathFromExeToCso + L"DebugPixel.cso",ShaderType::Pixel, PixelRed);
				Core::ShaderManager::Init(PathFromExeToCso + L"TextureDebug.cso",ShaderType::Pixel, PixelTexture);
#endif

#pragma endregion

#pragma region CreateBuffers
				Buffer::BUFFER_INIT_DESC desc;
				desc.ElementSize = sizeof(Oyster::Math::Matrix);
				desc.NumElements = 1;
				desc.InitData = NULL;
				desc.Type = Buffer::BUFFER_TYPE::CONSTANT_BUFFER_VS;
				desc.Usage = Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;

				ModelData.Init(desc);

				desc.NumElements = 2;
				VPData.Init(desc);

#pragma endregion

#pragma region Setup Render States
				/** @todo Create DX States */  

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
				/// @todo parata med fredrik om wraping
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

#pragma endregion

#pragma region Setup Views
				/** @todo Create Views */  
#pragma endregion

#pragma region Create Shader Effects
				/** @todo Create ShaderEffects */
				obj.Shaders.Pixel = GetShader::Pixel(PixelTexture);
				obj.Shaders.Vertex = GetShader::Vertex(VertexTransformDebug);

				D3D11_INPUT_ELEMENT_DESC indesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }

				};

				Shader::CreateInputLayout(indesc,3,GetShader::Vertex(VertexTransformDebug),obj.IAStage.Layout);
				obj.IAStage.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				obj.CBuffers.Vertex.push_back(&VPData);
				obj.RenderStates.Rasterizer = rs;
				obj.RenderStates.SampleCount = 1;
				obj.RenderStates.SampleState = ss;
				obj.RenderStates.DepthStencil = dsState;

				ModelData.Apply(1);
#pragma endregion
				return Core::Init::Sucsess;
			}

			void Resources::Clean()
			{
				Resources::ModelData.~Buffer();
				for(int i = 0; i < obj.CBuffers.Vertex.size(); ++i)
				{
					obj.CBuffers.Vertex[i]->~Buffer();
				}
				for(int i = 0; i < obj.CBuffers.Pixel.size(); ++i)
				{
					SAFE_DELETE(obj.CBuffers.Pixel[i]);
				}
				for(int i = 0; i < obj.CBuffers.Geometry.size(); ++i)
				{
					SAFE_DELETE(obj.CBuffers.Geometry[i]);
				}

				SAFE_RELEASE(obj.IAStage.Layout);

				SAFE_RELEASE(obj.RenderStates.BlendState);

				SAFE_RELEASE(obj.RenderStates.DepthStencil);

				SAFE_RELEASE(obj.RenderStates.Rasterizer);

				for(int i = 0; i < obj.RenderStates.SampleCount; ++i)
				{
					SAFE_RELEASE(obj.RenderStates.SampleState[i]);
				}
				
				SAFE_DELETE_ARRAY(obj.RenderStates.SampleState);
			}
		}
	}
}