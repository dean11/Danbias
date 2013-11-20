#include "..\Core\Core.h"

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			namespace Preparations
			{
				class Basic
				{
					void BindBackBufferRTV(bool UseDepthStencil = true);
					void BindBackBufferRTV(ID3D11DepthStencilView* depthStencil);

					void BindBackBufferUAV();

					void ClearBackBuffer(Oyster::Math::Float4 Color, bool ClearDepthStencil);
				};
			}
		}
	}
}