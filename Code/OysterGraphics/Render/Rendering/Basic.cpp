#include "Render.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{
				Core::ShaderManager::ShaderEffect Basic::Resources::se = Core::ShaderManager::ShaderEffect();
				
				void Basic::Resources::Init()
				{
					se.Shaders.Vertex = Core::ShaderManager::Get::Vertex(L"DebugCamera");
					se.Shaders.Pixel = Core::ShaderManager::Get::Pixel(L"Debug");
					
				}

				void Basic::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4 Projection)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(0,0,0,1));
				}
				void Basic::RenderScene(Model* models, int count)
				{
				}
				void Basic::EndFrame()
				{
				}
			}
		}
	}
}