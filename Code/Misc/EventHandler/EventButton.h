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
		public:
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

			void SendEvent(IEventButton::ButtonState state);

			void SetEventFunc(void (*EventFunc)( ButtonEvent e )); //?
			
			unsigned int GetID();
		
		};
		
		template <typename owner>
		unsigned int EventButton<owner>::PrivData::currID = 0;

		template <typename owner>
		EventButton<owner>::EventButton()
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			//this->privData.owner = NULL;
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
			//this->privData.owner = NULL;
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
			SendEvent(Button_Smashed);
		}
		
		template <typename owner>
		void EventButton<owner>::SendEvent(IEventButton::ButtonState state)
		{
			ButtonEvent event;
			event.state = state;
			event.sender = this;
			event.owner = privData.owner;
			privData.EventFunc(event);
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
	}
}


#endif