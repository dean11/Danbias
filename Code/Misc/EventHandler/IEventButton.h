//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_IEVENT_BUTTON
#define MISC_IEVENT_BUTTON

class InputClass;

namespace Oyster
{
	namespace Event
	{
		enum ButtonState
		{
			ButtonState_None,
			ButtonState_Hover,
			ButtonState_Pressed,
			ButtonState_Down,
			ButtonState_Released,
		};

		class IEventButton
		{
		public:
			virtual ~IEventButton(){}

			virtual void Update(InputClass *input){}

			virtual void SendEvent(ButtonState state){}

			struct ButtonEvent;
			virtual void SetEventFunc(void (*EventFunc)( ButtonEvent e )){}
			
			virtual unsigned int GetID(){ return -1; }
		
		};
	}
}

#endif