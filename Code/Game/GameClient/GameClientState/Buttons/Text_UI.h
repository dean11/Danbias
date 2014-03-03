#ifndef DANBIAS_CLIENT_TEXT_UI_H
#define DANBIAS_CLIENT_TEXT_UI_H

#include "DllInterfaces/GFXAPI.h"

namespace DanBias
{
	namespace Client
	{
		class Text_UI 
		{
		public:
			Text_UI( ) 
			{
				pos = Oyster::Math::Float3::null;
				size = Oyster::Math::Float2::null;
				text =  L"";
				textColor = Oyster::Math::Float4(1);
			}
			Text_UI(std::wstring text,  Oyster::Math::Float3 pos, Oyster::Math::Float2 size, Oyster::Math::Float4 textColor =  Oyster::Math::Float4(1)) 
				: pos(pos), size(size), text(text), textColor(textColor)
			{
			}
			void setText(std::wstring text)
			{
				this->text = text;
			}
			void RenderText() const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, size.y * 0.5f, textColor);
				}
			}
			void RenderText(std::wstring text) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, size.y * 0.5f, textColor);
				}
			}
			void RenderText( Oyster::Math::Float3 pos) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, size.y * 0.5f, textColor);
				}
			}
			void RenderText( Oyster::Math::Float3 pos, Oyster::Math::Float2 size) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, size.y * 0.5f, textColor);
				}
			}
		private:
			Oyster::Math::Float3 pos;
			Oyster::Math::Float2 size;

			std::wstring text;
			Oyster::Math::Float4 textColor;
		};
	}
}

#endif