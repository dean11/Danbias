///////////////////////
// Sam Svensson 2013 //
///////////////////////

#include "../../Input/L_inputClass.h"
#include <vector>

namespace Oyster
{
	namespace Event
	{
		struct IButton
		{
			enum ButtonState
			{
				Button_Clicked,
				Button_Hover,
				Button_Hold,
				Button_Smashed,	
			};
		};
		template <class T>
		class EventButton :public IButton
		{
		private:
			
			struct ButtonEvent
			{
				ButtonState state; 
				EventButton &sender;
				T          owner;
			};	

			struct PrivData
			{
				static unsigned int currID;
				unsigned int ID;

				T owner;
				void (*EventFunc)( ButtonEvent e );
			};

			PrivData privData;

		public:
			EventButton();
			EventButton(T owner);
			EventButton(void (*EventFunc)( ButtonEvent e));
			EventButton(void (*EventFunc)( ButtonEvent e), T owner);
			
			~EventButton();
			
			void checkCollision(InputClass *input);

			void SetEventFunc(void (*EventFunc)( ButtonEvent e )); //?
			
			unsigned int GetID();
			T& GetOwner(); 
		
		};


		template <class T>
		EventButton<T>::EventButton()
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = NULL;
		}

		template <class T>
		EventButton<T>::EventButton(T owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = NULL;
		}

		template <class T>
		EventButton<T>::EventButton(void (*EventFunc)( ButtonEvent e))
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = EventFunc;
		}

		template <class T>
		EventButton<T>::EventButton(void (*EventFunc)( ButtonEvent e), T owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = EventFunc;
		}

		template <class T>
		EventButton<T>~EventButton()
		{

		}
		
		template <class T>
		void EventButton<T>::checkCollision(InputClass *input)
		{
			//??????????????? TODO: everything
		}

		template <class T>
		void EventButton<T>::SetEventFunc(void (*EventFunc)( ButtonEvent e ))
		{
			this->privData.EventFunc = EventFunc;
		}
		
		template <class T>
		unsigned int EventButton<T>::GetID()
		{
			return this->privData.ID;
		}

		template <class T>
		T& EventButton<T>::GetOwner()
		{
			return this->privData.owner;
		}
	}
}