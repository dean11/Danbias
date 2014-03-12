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
				fontSize = 0;
			}
			Text_UI( std::wstring text ) 
			{
				pos = Oyster::Math::Float3::null;
				size = Oyster::Math::Float2::null;
				this->text = text;
				textColor = Oyster::Math::Float4(1);
				fontSize = 0;
			}
			Text_UI(std::wstring text,  Oyster::Math::Float3 pos, Oyster::Math::Float2 size, Oyster::Math::Float fontSize = 0.05f, Oyster::Math::Float4 textColor =  Oyster::Math::Float4(1)) 
				: pos(pos), size(size), fontSize(fontSize), text(text), textColor(textColor)
			{
			}
			void setText(std::wstring text)
			{
				this->text = text;
			}
			std::wstring getText( )
			{
				return this->text;
			}
			void setFontSize(Oyster::Math::Float fontSize)
			{
				this->fontSize = fontSize;
			}
			void SetPos(Oyster::Math::Float3 pos) { this->pos = pos; }
			Oyster::Math::Float3& GetPos() { return this->pos; }
			void SetSize(Oyster::Math::Float2 size) { this->size = size; }
			Oyster::Math::Float2& GetSize() { return this->size; }
			void SetCol(Oyster::Math::Float4 textColor) { this->textColor = textColor; }
			Oyster::Math::Float4& GetCol() { return this->textColor; }
			void RenderText() const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, fontSize, textColor);
				}
			}
			void RenderText(Oyster::Math::Float3 _pos, Oyster::Math::Float2 _size, float _fontSize) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, _pos, _size, _fontSize, textColor);
				}
			}
			void RenderText(Oyster::Math::Float3 _pos, Oyster::Math::Float2 _size, float _fontSize, Oyster::Math::Float4 _textColor) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, _pos, _size, _fontSize, _textColor);
				}
			}
			void RenderText(std::wstring text) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, fontSize, textColor);
				}
			}
			void RenderText( Oyster::Math::Float3 pos) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, fontSize, textColor);
				}
			}
			void RenderText( Oyster::Math::Float3 pos, Oyster::Math::Float2 size) const
			{
				if(text.size() > 0)
				{
					Oyster::Graphics::API::RenderText(text, pos, size, fontSize, textColor);
				}
			}
		private:
			Oyster::Math::Float3 pos;
			Oyster::Math::Float2 size;
			Oyster::Math::Float fontSize;

			std::wstring text;
			Oyster::Math::Float4 textColor;
		};
	}
}

#endif