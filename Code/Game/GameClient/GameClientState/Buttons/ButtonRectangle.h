//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_BUTTON_RECTANGLE_H
#define DANBIAS_CLIENT_BUTTON_RECTANGLE_H

#include "EventButtonGUI.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class ButtonRectangle : public EventButtonGUI<Owner>
		{
		public:
			ButtonRectangle(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None)
				: EventButtonGUI(textureName, buttonText
				, textColor, backColor, hoverColor, pressedColor
				, owner, pos, size, resize)
			{}
			ButtonRectangle(std::wstring textureName, std::wstring buttonText,
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None)
				: EventButtonGUI(textureName, buttonText
				, textColor, backColor, hoverColor, pressedColor
				, func, pos, size, resize)
			{}
			ButtonRectangle(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None)
				: EventButtonGUI(textureName, buttonText
				, textColor, backColor, hoverColor, pressedColor
				, func, owner, pos, size, resize)
			{}
			ButtonRectangle(std::wstring textureName, std::wstring buttonText, 
				Oyster::Math::Float4 textColor, Oyster::Math::Float4 backColor, Oyster::Math::Float4 hoverColor, Oyster::Math::Float4 pressedColor,
				EventFunc func, Owner owner, void* userData, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_None)
				: EventButtonGUI(textureName, buttonText
				, textColor, backColor, hoverColor, pressedColor
				, func, owner, userData, pos, size, resize)
			{}
			virtual ~ButtonRectangle()
			{}

			//Rectangle vs point collision
			bool Collision(Oyster::Event::MouseInput& input)
			{
				float xMouse = input.x, yMouse = input.y;

				float widthTemp = pos.x - size.x * 0.5f;
				float widthTemp2 = pos.x + size.x * 0.5f;
				float heightTemp = pos.y - size.y * 0.5f;
				float heightTemp2 = pos.y + size.y * 0.5f;

				if(xMouse >= widthTemp && xMouse <= widthTemp2 && 
					yMouse >= heightTemp && yMouse <= heightTemp2)
				{
					return true;
				}

				return false;
			}

		protected:

		};
	}
}

#endif