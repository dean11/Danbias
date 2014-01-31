#ifndef MISC_IEVENT_BUTTON
#define MISC_IEVENT_BUTTON
#include "../../Input/L_inputClass.h"

namespace Oyster
{
	namespace Event
	{
		class IEventButton
		{
			
		public:
			
			enum ButtonState
			{
				Button_Clicked,
				Button_Hover,
				Button_Hold,
				Button_Smashed,	
			};

			virtual void CheckCollision(InputClass *input) = 0;

			struct ButtonEvent;
			virtual void SetEventFunc(void (*EventFunc)( ButtonEvent e )) = 0;
			
			virtual unsigned int GetID() = 0;
		
		};
	}
}

#endif