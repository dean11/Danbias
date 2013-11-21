#include "Resources.h"

const std::wstring PathFromExeToHlsl = L"";
const std::wstring VertexTransformDebug = L"TransformDebugVertex";
const std::wstring VertexDebug = L"DebugVertex";
const std::wstring PixelRed = L"DebugPixel";

typedef Oyster::Graphics::Core::ShaderManager::ShaderType Shader;

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			Core::Init::State Resources::Init()
			{

#pragma region LoadShaders

#ifdef _DEBUG

				/** Load Vertex Shader for d3dcompile*/
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugCameraVertex",Shader::Vertex, VertexTransformDebug, false);
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" +L"DebugVertex",Shader::Vertex, VertexDebug, false);

				/** Load Pixel Shader for d3dcompile */
				Core::ShaderManager::Init(PathFromExeToHlsl + L"SimpleDebug\\" + L"DebugPixel", Shader::Pixel, PixelRed, false);

#else
				/** Load Vertex Shader with Precompiled */
#endif

#pragma endregion

#pragma region CreateBuffers
				/** @todo Create Buffers */
#pragma endregion

#pragma region Setup Render States
				/** @todo Create DX States */  
#pragma endregion


#pragma region Create Shader Effects
				/** @todo Create ShaderEffects */
#pragma endregion
				return Core::Init::Sucsess;
			}
		}
	}
}