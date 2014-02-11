//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_BUTTON_CIRCLE_H
#define MISC_EVENT_BUTTON_CIRCLE_H

#include "EventButton.h"
#include "../../Input/L_inputClass.h"

namespace Oyster
{
	namespace Event
	{
		template <typename Owner>
		class EventButtonCircle : public EventButton<Owner>
		{
		public:
			EventButtonCircle() 
				: EventButton(), xPos(0), yPos(0), radius(0)
			{}
			EventButtonCircle(Owner owner, float xPos, float yPos, float radius) 
				: EventButton(owner), xPos(xPos), yPos(yPos), radius(radius)
			{}
			EventButtonCircle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), float xPos, float yPos, float radius) 
				: EventButton(EventFunc), xPos(xPos), yPos(yPos), radius(radius)
			{}
			EventButtonCircle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), Owner owner, float xPos, float yPos, float radius) 
				: EventButton(EventFunc, owner), xPos(xPos), yPos(yPos), radius(radius)
			{}
			EventButtonCircle(void (*EventFunc)( Oyster::Event::ButtonEvent<Owner>& e), Owner owner, void* userData, float xPos, float yPos, float radius) 
				: EventButton(EventFunc, owner, userData), xPos(xPos), yPos(yPos), radius(radius)
			{}
			~EventButtonCircle()
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

		private:
			float xPos, yPos;
			float radius;

		};
	}
}

#endif