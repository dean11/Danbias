//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_EVENT_BUTTON_GUI_H
#define DANBIAS_CLIENT_EVENT_BUTTON_GUI_H

#include "EventHandler/EventButton.h"
#include "DllInterfaces/GFXAPI.h"
#include "../WindowManager/WindowShell.h"

namespace DanBias
{
	namespace Client
	{
		/*Dictates if the texture should be resized based on the screen aspect ratio.
		  
		*/
		enum ResizeAspectRatio
		{
			ResizeAspectRatio_None,
			ResizeAspectRatio_Width,
			ResizeAspectRatio_Height,

			ResizeAspectRatio_Count,
			ResizeAspectRatio_Unknown = -1
		};


		template <typename Owner>
		class EventButtonGUI : public Oyster::Event::EventButton<Owner>
		{
		public:
			EventButtonGUI(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None) 
				: EventButton(owner), pos(pos), size(size), texture(NULL), buttonText(buttonText), 
				textColor(textColor), backColor(backColor), hoverColor(hoverColor), pressedColor(pressedColor)
			{
				CreateTexture(textureName);
				this->resizedSize = size;
				if(resize != ResizeAspectRatio_None) ResizeWithAspectRatio(resize);
			}
			EventButtonGUI(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None) 
				: EventButton(func), pos(pos), size(size), texture(NULL), buttonText(buttonText), 
				textColor(textColor), backColor(backColor), hoverColor(hoverColor), pressedColor(pressedColor)
			{
				CreateTexture(textureName);
				this->resizedSize = size;
				if(resize != ResizeAspectRatio_None) ResizeWithAspectRatio(resize);
			}
			EventButtonGUI(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None) 
				: EventButton(func, owner), pos(pos), size(size), texture(NULL), buttonText(buttonText), 
				textColor(textColor), backColor(backColor), hoverColor(hoverColor), pressedColor(pressedColor)
			{
				CreateTexture(textureName);
				this->resizedSize = size;
				if(resize != ResizeAspectRatio_None) ResizeWithAspectRatio(resize);
			}
			EventButtonGUI(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, void* userData, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None) 
				: EventButton(func, owner, userData), pos(pos), size(size), texture(NULL), buttonText(buttonText), 
				textColor(textColor), backColor(backColor), hoverColor(hoverColor), pressedColor(pressedColor)
			{
				CreateTexture(textureName);
				this->resizedSize = size;
				if(resize != ResizeAspectRatio_None) ResizeWithAspectRatio(resize);
			}
			virtual ~EventButtonGUI()
			{
				Oyster::Graphics::API::DeleteTexture(texture);
				texture = NULL;
			}

			void CreateTexture(std::wstring textureName)
			{
				//Create texture
				texture = Oyster::Graphics::API::CreateTexture(textureName);
			}

			virtual void RenderTexture()
			{
				if(EventButton<Owner>::Enabled())
				{
					// let the using dev decide what is rendered
					//Oyster::Graphics::API::RenderGuiElement(texture, pos, size, Oyster::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f));

					//Render att xPos and yPos
					//With width and height

					if(EventButton<Owner>::GetState() == ButtonState_None)
					{
						Oyster::Graphics::API::RenderGuiElement(texture, pos, resizedSize, backColor);
					}
					else if(EventButton<Owner>::GetState() == ButtonState_Hover)
					{
						Oyster::Graphics::API::RenderGuiElement(texture, pos, resizedSize, hoverColor);
					}
					else
					{
						Oyster::Graphics::API::RenderGuiElement(texture, pos, resizedSize, pressedColor);
					}

				}
			}

			virtual void RenderText()
			{
				if(buttonText.size() > 0)
				{
					Oyster::Graphics::API::RenderText(buttonText, pos - Float3(size.x * 0.5f, size.y * 0.25f, +0.001f), size, size.y * 0.5f, textColor);
				}
			}

			virtual void SetBackColor( Oyster::Math::Float4 c ) { this->backColor = c; };

		private:
			void ResizeWithAspectRatio(ResizeAspectRatio resize)
			{
				RECT r;
				GetClientRect(WindowShell::GetHWND(), &r);

				if(resize == ResizeAspectRatio_Height)
					resizedSize.y = size.y * ((float)r.right/(float)r.bottom);
				else if(resize == ResizeAspectRatio_Width)
					resizedSize.x = size.x * ((float)r.bottom/(float)r.right);
			}

		protected:
			Oyster::Math::Float3 pos;
			Oyster::Math::Float2 size;

			//The new calculated resize, it's only used for the button not the text right now.
			Oyster::Math::Float2 resizedSize;

			Oyster::Graphics::API::Texture texture;
			
			std::wstring buttonText;
			Oyster::Math::Float4 textColor;
			
			Oyster::Math::Float4 backColor;
			Oyster::Math::Float4 hoverColor;
			Oyster::Math::Float4 pressedColor;
		};
	}
}

#endif