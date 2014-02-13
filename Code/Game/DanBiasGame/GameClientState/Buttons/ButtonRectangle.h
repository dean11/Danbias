//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_BUTTON_RECTANGLE_H
#define DANBIAS_CLIENT_BUTTON_RECTANGLE_H

#include "EventButtonGUI.h"
#include <iostream>
//Only for testing because we don't have any other input
#include "../WindowManager/WindowShell.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class ButtonRectangle : public EventButtonGUI<Owner>
		{
		public:
			ButtonRectangle()
				: EventButtonGUI(), width(0), height(0)
			{}
			ButtonRectangle(std::wstring textureName, Owner owner, float xPos, float yPos, float width, float height, bool resizeToScreenAspectRatio = true)
				: EventButtonGUI(textureName, owner, xPos, yPos, width, height, resizeToScreenAspectRatio)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, float xPos, float yPos, float width, float height, bool resizeToScreenAspectRatio = true)
				: EventButtonGUI(textureName, func, xPos, yPos, width, height, resizeToScreenAspectRatio)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float width, float height, bool resizeToScreenAspectRatio = true)
				: EventButtonGUI(textureName, func, owner, xPos, yPos, width, height, resizeToScreenAspectRatio)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float width, float height, bool resizeToScreenAspectRatio = true)
				: EventButtonGUI(textureName, func, owner, userData, xPos, yPos, width, height, resizeToScreenAspectRatio)
			{}
			virtual ~ButtonRectangle()
			{}

			//Circle vs point collision
			bool Collision(Oyster::Event::MouseInput& input)
			{
				//Should come from the InputClass
				float xMouse = input.x, yMouse = input.y;

				float widthTemp = xPos - width * 0.5f;
				float widthTemp2 = xPos + width * 0.5f;
				float heightTemp = yPos - height * 0.5f;
				float heightTemp2 = yPos + height * 0.5f;
				//std::cout << p.x << ' ' << p.y << ' ' << widthTemp << ' ' << heightTemp << std::endl;

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