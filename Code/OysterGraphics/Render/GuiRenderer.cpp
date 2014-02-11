#include "GuiRenderer.h"
#include "Resources.h"
#include "../Definitions/GraphicalDefinition.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Rendering
			{
				void Gui::BeginRender()
				{
					Core::PipelineManager::SetRenderPass(Render::Resources::Gui::Pass);
				}

				void Gui::Render(ID3D11ShaderResourceView* tex,Math::Float2 pos, Math::Float2 size)
				{
					Core::deviceContext->PSSetShaderResources(0,1,&tex);

					pos *= 2;
					pos -= 1;
					pos.y *= -1;

					Definitions::GuiData gd;

					gd.Translation = Math::Matrix::identity;
					gd.Translation.m41 = pos.x;
					gd.Translation.m42 = pos.y;
					gd.Translation.m11 = size.x;
					gd.Translation.m22 = size.y;


					void* data = Render::Resources::Gui::Data.Map();
					memcpy(data,&gd,sizeof(Definitions::GuiData));
					Render::Resources::Gui::Data.Unmap();

					Core::deviceContext->Draw(1,0);
				}
			}
		}
	}
}