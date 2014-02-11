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

			virtual void Render() = 0;
			virtual void Update(InputClass *input) = 0;

			virtual void SendEvent(ButtonState state) = 0;
			
			virtual unsigned int GetID() = 0;
		
		};
	}
}

#endif