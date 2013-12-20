#include "Debug.h"
#include "..\OysterGraphics\Definitions\GraphicalDefinition.h"

#ifdef _DEBUG
// /Bin/Executable/Tester ->
// /Code/OysterGraphics/Shader/HLSL
const std::wstring PathFromExeToCso = L"..\\Content\\Shaders\\";
const std::wstring PathFromExeToHlsl = L"..\\..\\Code\\OysterGraphics\\Shader\\HLSL\\";
const std::wstring VertexTransformDebug = L"TransformDebugVertex";
const std::wstring VertexDebug = L"DebugVertex";
const std::wstring PixelRed = L"DebugPixel";
const std::wstring PixelTexture = L"Texture";
const std::wstring ComputeDebug = L"Debug";

typedef Oyster::Graphics::Core::PipelineManager::ShaderType ShaderType;
typedef Oyster::Graphics::Core::PipelineManager::Get GetShader;
typedef Oyster::Graphics::Core::PipelineManager Shader;
typedef Oyster::Graphics::Core::Buffer Buffer;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			//Shader::RenderPass Resources::Debug::obj;
			Shader::RenderPass Resources::Debug::debugCompute;// = Shader::ShaderEffect();;
			//Buffer Resources::Debug::ModelData = Buffer();
			//Buffer Resources::Debug::VPData = Buffer();

			//ID3D11ShaderResourceView* Resources::Debug::srv = NULL;
			//ID3D11RenderTargetView* Resources::Debug::rtv = NULL;

			Core::Init::State Resources::Debug::Init()
			{

#pragma region LoadShaders
				/** Load Compute Shaders for d3dcompile */
				Core::PipelineManager::Init(PathFromExeToHlsl + L"ComputeDebug\\" + L"DebugCompute.hlsl", ShaderType::Compute, ComputeDebug);

#pragma endregion
				return Core::Init::Success;
			}

			void Resources::Debug::Clean()
			{

			}
		}
	}
}

#endif