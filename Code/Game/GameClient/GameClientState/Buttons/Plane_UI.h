#ifndef DANBIAS_CLIENT_PLANE_UI_H
#define DANBIAS_CLIENT_PLANE_UI_H

#include "DllInterfaces/GFXAPI.h"

namespace DanBias
{
	namespace Client
	{
		class Plane_UI 
		{
		public:
			Plane_UI( ) 
			{
				pos = Oyster::Math::Float3::null;
				size = Oyster::Math::Float2::null;
				tintColor =  Oyster::Math::Float4(1);
				texture = NULL;
			}
			Plane_UI( std::wstring textureName, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, Oyster::Math::Float4 tintColor =  Oyster::Math::Float4(1)) 
				: pos(pos), size(size), tintColor(tintColor)
			{
				CreateTexture(textureName);
			}
			virtual ~Plane_UI()
			{
				Oyster::Graphics::API::DeleteTexture(texture);
				texture = NULL;
			}
			void CreateTexture(std::wstring textureName) 
			{
				//Create texture
				texture = Oyster::Graphics::API::CreateTexture(textureName);
			}

			virtual void RenderTexture() const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tintColor);
			}
		private:
			Oyster::Math::Float3 pos;
			Oyster::Math::Float2 size;

			Oyster::Graphics::API::Texture texture;
			Oyster::Math::Float4 tintColor;
		};
	}
}

#endif