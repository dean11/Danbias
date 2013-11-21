#include "Resources.h"
#include "..\OysterGraphics\Definitions\GraphicalDefinition.h"

const std::wstring PathFromExeToHlsl = L"..\\OysterGraphics\\Shader\\HLSL\\";
const std::wstring VertexTransformDebug = L"TransformDebugVertex";
const std::wstring VertexDebug = L"DebugVertex";
const std::wstring PixelRed = L"DebugPixel";

typedef Oyster::Graphics::Core::ShaderManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::ShaderManager::Get GetShader;
typedef Oyster::Graphics::Core::ShaderManager Shader;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			Shader::ShaderEffect Resources::obj;
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
				rdesc.FrontCounterClockwise = true;
				rdesc.DepthBias = 0;
				rdesc.DepthBiasClamp = 0;
				rdesc.DepthClipEnable = false;
				rdesc.SlopeScaledDepthBias = 0;
				rdesc.ScissorEnable = true;
				rdesc.MultisampleEnable = false;
				rdesc.AntialiasedLineEnable = false;

				ID3D11RasterizerState* rs = NULL;
				Oyster::Graphics::Core::device->CreateRasterizerState(&rdesc,&rs);
#pragma endregion

#pragma region Setup Views
				/** @todo Create Views */  
#pragma endregion

#pragma region Create Shader Effects
				/** @todo Create ShaderEffects */
				obj.Shaders.Pixel = GetShader::Pixel(PixelRed);
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

#pragma endregion

				return Core::Init::Sucsess;
			}
		}
	}
}