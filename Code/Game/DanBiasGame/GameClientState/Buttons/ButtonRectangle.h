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
			ButtonRectangle()
				: EventButtonGUI(), halfWidth(0), halfHeight(0)
			{}
			ButtonRectangle(std::wstring textureName, Owner owner, float xPos, float yPos, float halfWidth, float halfHeight)
				: EventButtonGUI(textureName, owner, xPos, yPos, halfWidth, halfHeight)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, float xPos, float yPos, float halfWidth, float halfHeight)
				: EventButtonGUI(textureName, func, xPos, yPos, halfWidth, halfHeight)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float halfWidth, float halfHeight)
				: EventButtonGUI(textureName, func, owner, xPos, yPos, halfWidth, halfHeight)
			{}
			ButtonRectangle(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float halfWidth, float halfHeight)
				: EventButtonGUI(textureName, func, owner, userData, xPos, yPos, halfWidth, halfHeight)
			{}
			~ButtonRectangle()
			{}

			//Circle vs point collision
			bool Collision(InputClass* inputObject)
			{
				//Should come from the InputClass
				float xMouse = 1, yMouse = 0;

				if(xMouse >= xPos - halfWidth && xMouse <= xPos + halfWidth
					&& yMouse >= yPos - halfHeight && yMouse <= yPos + halfHeight)
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