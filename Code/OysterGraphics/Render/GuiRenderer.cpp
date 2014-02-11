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
			const float TEXT_SIZE=2.5;

			void Gui::Begin2DRender()
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

			void Gui::Begin2DTextRender()
			{
				Resources::Gui::Text::Vertex.Apply();
				Core::PipelineManager::SetRenderPass(Resources::Gui::Text::Pass);
			}

			void Gui::RenderText(std::wstring text, Math::Float2 pos, Math::Float2 size)
			{
				//Pos.x -= instance.sizeX/2;
				//Pos.x += size.x;
				//Pos.y -= instance.sizeY/2;
				//Pos.y += size.y;
				//Matrix m;
				//m = Math::Matrix::identity;
				//float width = (1.0f/(instance.sizeX/2.0f));
				//float height = (1.0f/(instance.sizeY/2.0f));
				//m.m41=Pos.x * width;
				//m.m42=-Pos.y * height;
				//m.m43=Pos.z;
				//m.m11=width*size.x;
				//m.m22=height*size.y;
				//void* dest = Resources::Buffers::CBufferGs.Map();
				//memcpy(dest,&m.GetTranspose(),64);
				//Resources::Buffers::CBufferGs.Unmap();

				//Oyster::Render::Textbox::Update(text, size.x);
				//Oyster::Engine::PrepareForRendering::Begin2DTextRender();
				//Oyster::Core::DeviceContext->PSSetShaderResources(0,1,&(Oyster::Render::Textbox::Texture));
				////Should be able to be outside of the for loop. Keeping it here for now though.
				//Oyster::Core::DeviceContext->Draw(Oyster::Render::Textbox::NumLetters, 0);

				pos *= 2;
				pos -= 1;
				pos.y *= -1;

				Definitions::GuiData gd;

				gd.Translation = Math::Matrix::identity;
				gd.Translation.m41 = (pos.x - (size.x/2 * text.length()));
				gd.Translation.m42 = pos.y;
				gd.Translation.m11 = size.x;
				gd.Translation.m22 = size.y;


				void* data = Render::Resources::Gui::Data.Map();
				memcpy(data,&gd,sizeof(Definitions::GuiData));
				Render::Resources::Gui::Data.Unmap();
				Definitions::Text2D tmpInst;

				void* dest = Resources::Gui::Text::Vertex.Map();
				Definitions::Text2D* dataView = reinterpret_cast<Definitions::Text2D*>(dest);
				//tmpInst.charOffset=_pos;
				for (unsigned int i=0; i<text.length(); i++)
				{
					tmpInst.coff=(1.0f/TEXT_NR_LETTERS);
					tmpInst.offset=text[i]-32;
					tmpInst.pos=i*(size.x*2);
					//float tst=getCharID(_str[i]);
					//tmpInst.offset=tst;
					//tmpInst.charOffset.x=_pos.x-i*TEXT_SIZE;
					//tmpInst.data=tst;
					dataView[i]=tmpInst;
				}
				//TextInstances[_id].NumLetters=instances;
				Resources::Gui::Text::Vertex.Unmap();

					
				Core::deviceContext->Draw(text.length(), 0);
			}
		}
	}
}