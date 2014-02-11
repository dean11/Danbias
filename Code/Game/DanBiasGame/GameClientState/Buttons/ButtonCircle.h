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
		class ButtonCircle : public EventButtonGUI<Owner>
		{
		public:
			ButtonCircle() 
				: EventButtonGUI(), radius(0)
			{}
			ButtonCircle(std::wstring textureName, Owner owner, float xPos, float yPos, float radius, float textureHalfWidth, float textureHalfHeight) 
				: EventButtonGUI(textureName, owner, xPos, yPos, textureHalfWidth, textureHalfHeight), radius(radius)
			{}
			ButtonCircle(std::wstring textureName, EventFunc func, float xPos, float yPos, float radius, float textureHalfWidth, float textureHalfHeight) 
				: EventButtonGUI(textureName, func, xPos, yPos, textureHalfWidth, textureHalfHeight), radius(radius)
			{}
			ButtonCircle(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float radius, float textureHalfWidth, float textureHalfHeight) 
				: EventButtonGUI(textureName, func, owner, xPos, yPos, textureHalfWidth, textureHalfHeight), radius(radius)
			{}
			ButtonCircle(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float radius, float textureHalfWidth, float textureHalfHeight) 
				: EventButtonGUI(textureName, func, owner, userData, xPos, yPos, textureHalfWidth, textureHalfHeight), radius(radius)
			{}
			~ButtonCircle()
			{}

			//Circle vs point collision
			bool Collision(InputClass* inputObject)
			{
				//Should come from the InputClass
				float xMouse = 2, yMouse = 2;

				float xDiff = xMouse - xPos;
				float yDiff = yMouse - yPos;

				float length = (xDiff * xDiff) + (yDiff * yDiff);

				if(length <= radius*radius)
				{
					return true;
				}

				return false;
			}

		protected:
			float radius;

		};
	}
}

#endif