#include "GuiRenderer.h"
#include "Resources.h"
#include "../Definitions/GraphicalDefinition.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{				
			const int TEXT_NR_LETTERS=95;
			const float TEXT_SPACING=2.0f;

			void Gui::Begin2DRender()
			{
				Core::PipelineManager::SetRenderPass(Render::Resources::Gui::Pass);
			}

			void Gui::Render(ID3D11ShaderResourceView* tex,Math::Float3 pos, Math::Float2 size, Math::Float4 color)
			{
				Core::deviceContext->PSSetShaderResources(0,1,&tex);

				pos.xy *= 2;
				pos.xy -= 1;
				pos.y *= -1;

				Definitions::GuiData gd;
				gd.Translation = Math::Matrix::identity;
				gd.Translation.m41 = pos.x;
				gd.Translation.m42 = pos.y;
				gd.Translation.m43 = pos.z;
				gd.Translation.m11 = size.x;
				gd.Translation.m22 = size.y;

				void* data = Render::Resources::Gui::Data.Map();
				memcpy(data,&gd,sizeof(Definitions::GuiData));
				Render::Resources::Gui::Data.Unmap();

				data = Render::Resources::Color.Map();
				memcpy(data,&color,sizeof(Math::Float4));
				Render::Resources::Color.Unmap();


				Core::deviceContext->Draw(1,0);
			}

			void Gui::Begin2DTextRender()
			{
				Resources::Gui::Text::Vertex.Apply();
				Core::PipelineManager::SetRenderPass(Resources::Gui::Text::Pass);
			}

			void Gui::RenderText(std::wstring text, Math::Float3 pos, Math::Float2 size, float FontSize, Math::Float4 color)
			{
				
				//size.x = size.x / (text.length() * TEXT_SPACING /2);
				

				pos.xy *= 2;
				pos.xy -= 1;
				pos.y *= -1;

				
				//pos.x =  pos.x - (size.x  * (text.length()-1) * TEXT_SPACING /2);
				pos.x = pos.x + size.x/ (text.length() * TEXT_SPACING/2);
				pos.y = pos.y - size.y/2;

				Definitions::GuiData gd;

				gd.Translation = Math::Matrix::identity;
				gd.Translation.m41 = pos.x;
				gd.Translation.m42 = pos.y;
				gd.Translation.m43 = pos.z;
				gd.Translation.m11 = FontSize * 0.8f;
				gd.Translation.m22 = FontSize;


				void* data = Render::Resources::Gui::Data.Map();
				memcpy(data,&gd,sizeof(Definitions::GuiData));
				Render::Resources::Gui::Data.Unmap();
				Definitions::Text2D tmpInst;

				data = Render::Resources::Color.Map();
				memcpy(data,&color,sizeof(Math::Float4));
				Render::Resources::Color.Unmap();

				void* dest = Resources::Gui::Text::Vertex.Map();
				Definitions::Text2D* dataView = reinterpret_cast<Definitions::Text2D*>(dest);

				for (unsigned int i=0; i<text.length(); i++)
				{
					tmpInst.coff=(1.0f/TEXT_NR_LETTERS);
					tmpInst.offset=text[i]-32;
					tmpInst.pos=i*(FontSize * 0.7f * TEXT_SPACING);
					if(tmpInst.pos > size.x*2)
					{
						text = text.substr(0,i-1);
						break;
					}
					dataView[i]=tmpInst;
				}

				Resources::Gui::Text::Vertex.Unmap();

					
				Core::deviceContext->Draw((UINT)text.length(), 0);
			}
		}
	}
}