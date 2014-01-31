#include "../../Input/L_inputClass.h"

namespace Oyster
{
	namespace Event
	{
		class IEventButton
		{

		private:
			
			enum ButtonState
			{
				Button_Clicked,
				Button_Hover,
				Button_Hold,
				Button_Smashed,	
			};

			struct ButtonEvent
			{
				ButtonState state; 
				IEventButton &sender;

				template<class owner>
				owner          owner;
			};	

		public:
			virtual void checkCollision(InputClass *input) = 0;

			virtual void SetEventFunc(void (*EventFunc)( ButtonEvent e )) = 0;
			
			virtual unsigned int GetID() = 0;

			template<class owner>
			virtual owner& GetOwner() = 0; 
		
		};
	}
}