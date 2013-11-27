#include "Resources.h"
#include "..\OysterGraphics\Definitions\GraphicalDefinition.h"

// /Bin/Executable/Tester ->
// /Code/OysterGraphics/Shader/HLSL
const std::wstring PathFromExeToHlsl = L"..\\..\\..\\Code\\OysterGraphics\\Shader\\HLSL\\";
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
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugCameraVertex.hlsl",ShaderType::Vertex, VertexTransformDebug, false);
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugVertex.hlsl",ShaderType::Vertex, VertexDebug, false);

				/** Load Pixel Shader for d3dcompile */
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" + L"DebugPixel.hlsl", ShaderType::Pixel, PixelRed, false);
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" + L"TextureDebug.hlsl", ShaderType::Pixel, PixelTexture, false);
				

#else
				/** Load Vertex Shader with Precompiled */
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
				rdesc.CullMode = D3D11_CULL_NONE;
				rdesc.FillMode = D3D11_FILL_SOLID;
				rdesc.FrontCounterClockwise = false;
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
				sdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
				sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sdesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				sdesc.MipLODBias = 0;
				sdesc.MaxAnisotropy =4;
				sdesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				*sdesc.BorderColor = *Oyster::Math::Float4(0,0,0,1).element;
				sdesc.MinLOD = 0;
				sdesc.MaxLOD = D3D11_FLOAT32_MAX;

				ID3D11SamplerState** ss = new ID3D11SamplerState*[1];
				Oyster::Graphics::Core::device->CreateSamplerState(&sdesc,ss);

#pragma endregion

#pragma region Setup Views
				/** @todo Create Views */  
#pragma endregion

#pragma region Create Shader Effects
				/** @todo Create ShaderEffects */
				obj.Shaders.Pixel = GetShader::Pixel(PixelRed);
				obj.Shaders.Vertex = GetShader::Vertex(VertexDebug);

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

				ModelData.Apply(1);
#pragma endregion
				return Core::Init::Sucsess;
			}
		}
	}
}