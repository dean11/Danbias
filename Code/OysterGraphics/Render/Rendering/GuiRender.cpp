#include "GuiRender.h"

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

				void Gui::Render(ID3D11Texture2D* tex,Math::Float2 pos, Math::Float2 size)
				{
					//Oyster::Core::DeviceContext->PSSetShaderResources(0,1,&srv);

					//Pos.x -= instance.sizeX/2;
					//Pos.x += size.x/2;

					//Pos.y -= instance.sizeY/2;
					//Pos.y += size.y/2;

					//Matrix m;
					//m = Math::Matrix::identity;
					//float width = (1.0f/(instance.sizeX/2.0f));
					//float height = (1.0f/(instance.sizeY/2.0f));
					//m.m41=Pos.x * width;
					//m.m42=-Pos.y * height;
					//m.m43=Pos.z;
					//m.m11=width*size.x/2;
					//m.m22=height*size.y/2;
	
					//void* dest = Resources::Buffers::CBufferGs.Map();
					//memcpy(dest,&m.GetTranspose(),64);
					//Resources::Buffers::CBufferGs.Unmap();

					//Oyster::Core::DeviceContext->Draw(1,0);
				}
			}
		}
	}
}