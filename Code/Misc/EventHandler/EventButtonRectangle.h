//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_BUTTON_RECTANGLE_H
#define MISC_EVENT_BUTTON_RECTANGLE_H

#include "EventButton.h"
#include "../../Input/L_inputClass.h"

namespace Oyster
{
	namespace Event
	{
		template <typename Owner>
		class EventButtonRectangle : public EventButton<Owner>
		{
		public:
			EventButtonRectangle() 
				: EventButton(), xPos(0), yPos(0), halfWidth(0), halfHeight(0)
			{}
			EventButtonRectangle(Owner owner, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(owner), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{}
			EventButtonRectangle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(EventFunc), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{}
			EventButtonRectangle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), Owner owner, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(EventFunc, owner), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{}
			EventButtonRectangle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), Owner owner, void* userData, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(EventFunc, owner, userData), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{}
			~EventButtonRectangle()
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

		private:
			float xPos, yPos;
			float halfWidth, halfHeight;

		};
	}
}

#endif