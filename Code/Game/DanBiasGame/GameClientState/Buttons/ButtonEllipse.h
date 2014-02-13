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
			ButtonEllipse(std::wstring textureName, Owner owner, float xPos, float yPos, float textureWidth, float textureHeight, bool resizeToScreenAspectRatio = true) 
				: EventButtonGUI(textureName, owner, xPos, yPos, textureWidth, textureHeight, resizeToScreenAspectRatio)
			{}
			ButtonEllipse(std::wstring textureName, EventFunc func, float xPos, float yPos, float textureWidth, float textureHeight, bool resizeToScreenAspectRatio = true) 
				: EventButtonGUI(textureName, func, xPos, yPos, textureWidth, textureHeight, resizeToScreenAspectRatio)
			{}
			ButtonEllipse(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float textureWidth, float textureHeight, bool resizeToScreenAspectRatio = true) 
				: EventButtonGUI(textureName, func, owner, xPos, yPos, textureWidth, textureHeight, resizeToScreenAspectRatio)
			{}
			ButtonEllipse(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float textureWidth, float textureHeight, bool resizeToScreenAspectRatio = true) 
				: EventButtonGUI(textureName, func, owner, userData, xPos, yPos, textureWidth, textureHeight, resizeToScreenAspectRatio)
			{}
			virtual ~ButtonEllipse()
			{}

			//Circle vs point collision
			bool Collision(Oyster::Event::MouseInput& input)
			{
				//Should come from the InputClass
				float xMouse = input.x, yMouse = input.y;

				double normx = (xMouse - xPos) / width;
				double normy = (yMouse - yPos) / height;

				return (normx * normx + normy * normy) < 0.25;
			}

		protected:

		};
	}
}

#endif