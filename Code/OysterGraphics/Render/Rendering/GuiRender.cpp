#include "GuiRender.h"
#include "../Resources/Deffered.h"
#include "../../Definitions/GraphicalDefinition.h"

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


					void* data = Render::Resources::Deffered::GuiData.Map();
					memcpy(data,&gd,sizeof(gd));
					Render::Resources::Deffered::GuiData.Unmap();

					Core::deviceContext->Draw(1,0);
				}
			}
		}
	}
}