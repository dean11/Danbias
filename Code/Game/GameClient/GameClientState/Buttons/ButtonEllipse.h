//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_BUTTON_CIRCLE_H
#define DANBIAS_CLIENT_BUTTON_CIRCLE_H

#include "EventButtonGUI.h"

//Only for testing because we don't have any other input
#include "../WindowManager/WindowShell.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class ButtonEllipse : public EventButtonGUI<Owner>
		{
		public:
			ButtonEllipse() 
				: EventButtonGUI(), radius(0)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, Oyster::Math::Float4 textColor, Owner owner, Oyster::Math::Float3 pos, 
							Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, owner, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, Oyster::Math::Float4 textColor, EventFunc func, Oyster::Math::Float3 pos, 
							Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, func, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, Oyster::Math::Float4 textColor, EventFunc func, Owner owner, Oyster::Math::Float3 pos,
							Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, func, owner, pos, size, resize)
			{}
			ButtonEllipse(std::wstring textureName, std::wstring buttonText, Oyster::Math::Float4 textColor, EventFunc func, Owner owner, void* userData, Oyster::Math::Float3 pos, 
							Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height) 
				: EventButtonGUI(textureName, buttonText, textColor, func, owner, userData, pos, size, resize)
			{}
			virtual ~ButtonEllipse()
			{}

			//Circle vs point collision
			bool Collision(Oyster::Event::MouseInput& input)
			{
				//Should come from the InputClass
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