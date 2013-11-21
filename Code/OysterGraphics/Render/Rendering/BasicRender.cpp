#include "Render.h"
#include "../Resources/Resources.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{

				void Basic::NewFrame(Oyster::Math::Float4x4 View, Oyster::Math::Float4 Projection)
				{
					Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(0,0,0,1));
					Core::ShaderManager::SetShaderEffect(Graphics::Render::Resources::obj);
					Preparations::Basic::BindBackBufferRTV();
				}
				void Basic::RenderScene(Model* models, int count)
				{
					for(int i = 0; i < count; ++i)
					{
						if(models[i].Visible)
						{
							void* data  = Resources::ModelData.Map();
							memcpy(data,&(models[i].World),64);
							Resources::ModelData.Unmap();

							//Set Materials :: NONE

							models[i].info->Vertices.Apply();
							if(models[i].info->Indexed)
							{
								models[i].info->Indecies.Apply();
								Oyster::Graphics::Core::deviceContext->DrawIndexed(models[i].info->VertexCount,0,0);
							}
							else
							{
								Oyster::Graphics::Core::deviceContext->Draw(models[i].info->VertexCount,0);
							}
						}
					}				
				}
				void Basic::EndFrame()
				{
					Core::swapChain->Present(0,0);
				}
			}
		}
	}
}