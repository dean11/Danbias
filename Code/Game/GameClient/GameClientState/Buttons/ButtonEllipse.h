//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_BUTTON_CIRCLE_H
#define DANBIAS_CLIENT_BUTTON_CIRCLE_H

#include "EventButtonGUI.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class ButtonEllipse : public EventButtonGUI<Owner>
		{
		public:
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, backColor, hoverColor, pressedColor, owner, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, backColor, hoverColor, pressedColor, func, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, backColor, hoverColor, pressedColor, func, owner, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, void* userData, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, backColor, hoverColor, pressedColor, func, owner, userData, pos, size, resize)
			{}
			virtual ~ButtonEllipse()
			{}

			//Circle vs point collision
			bool Collision(Oyster::Event::MouseInput& input)
			{
				float xMouse = input.x, yMouse = input.y;

				double normx = (xMouse - pos.x) / size.x;
				double normy = (yMouse - pos.y) / size.y;

				return (normx * normx + normy * normy) < 0.25;
			}

		protected:

		};
	}
}

#endif