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

			void RenderTexture() const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tintColor);
			}
			void RenderTexture( Oyster::Math::Float3 pos) const
			{
				if(texture)
				{
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tintColor);
				}
			}
			void RenderTexture( Oyster::Math::Float3 pos, Oyster::Math::Float2 size) const
			{
				if(texture)
				{
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tintColor);
				}
			}
			virtual void RenderTexture(const Oyster::Math::Float3& position) const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, position, size, tintColor);
			}
			virtual void RenderTexture(const Oyster::Math::Float2& texSize) const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, pos, texSize, tintColor);
			}
			virtual void RenderTexture(const Oyster::Math::Float4& tint) const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tint);
			}
			virtual void RenderTexture(const Oyster::Math::Float3& pos, const Oyster::Math::Float2& size, const Oyster::Math::Float4& tint) const
			{
				if(texture)
					Oyster::Graphics::API::RenderGuiElement(texture, pos, size, tint);
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