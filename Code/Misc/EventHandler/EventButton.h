///////////////////////
// Sam Svensson 2013 //
///////////////////////

#ifndef MISC_EVENT_BUTTON_H
#define MISC_EVENT_BUTTON_H
#include "../../Input/L_inputClass.h"

namespace Oyster
{
	namespace Event
	{
		template <typename owner>
		class EventButton
		{
		private:
			struct ButtonEvent
			{
				IEventButton::ButtonState state; 
				EventButton<owner> &sender;
				owner          owner;
			};	

			struct PrivData
			{
				static unsigned int currID;
				unsigned int ID;

				owner owner;
				void (*EventFunc)( ButtonEvent e );
			};

			PrivData privData;

		public:
			EventButton();
			EventButton(owner owner);
			EventButton(void (*EventFunc)( ButtonEvent e));
			EventButton(void (*EventFunc)( ButtonEvent e), owner owner);
			
			~EventButton();
			
			void CheckCollision(InputClass *input);

			void SetEventFunc(void (*EventFunc)( ButtonEvent e )); //?
			
			unsigned int GetID();
			owner& GetOwner(); 
		
		};


		template <typename owner>
		EventButton<owner>::EventButton()
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = NULL;
		}

		template <typename owner>
		EventButton<owner>::EventButton(owner owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = NULL;
		}

		template <typename owner>
		EventButton<owner>::EventButton(void (*EventFunc)( ButtonEvent e))
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = EventFunc;
		}

		template <typename owner>
		EventButton<owner>::EventButton(void (*EventFunc)( ButtonEvent e), owner owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = EventFunc;
		}

		template <typename owner>
		EventButton<owner>::~EventButton()
		{

		}
		
		template <typename owner>
		void EventButton<owner>::CheckCollision(InputClass *input)
		{
			//??????????????? TODO: everything
		}

		template <typename owner>
		void EventButton<owner>::SetEventFunc(void (*EventFunc)( ButtonEvent e ))
		{
			this->privData.EventFunc = EventFunc;
		}
		
		template <typename owner>
		unsigned int EventButton<owner>::GetID()
		{
			return this->privData.ID;
		}

		template <typename owner>
		owner& EventButton<owner>::GetOwner()
		{
			return this->privData.owner;
		}
	}
}

#endif