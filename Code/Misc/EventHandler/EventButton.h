///////////////////////
// Sam Svensson 2013 //
///////////////////////

#ifndef MISC_EVENT_BUTTON_H
#define MISC_EVENT_BUTTON_H

#include "IEventButton.h"

namespace Oyster
{
	namespace Event
	{
		template <typename Owner>
		struct ButtonEvent
		{
			ButtonState state;
			IEventButton* sender;
			Owner owner;
			void* userData;
		};

		template <typename Owner>
		class EventButton : public IEventButton
		{
		protected:
			//typedef for callback function pointer
			typedef void (*EventFunc)(Oyster::Event::ButtonEvent<Owner>& e);

			struct PrivData
			{
				PrivData() : ID(currID++){}

				static unsigned int currID;
				const unsigned int ID;

				ButtonState previousState;
				Owner owner;
				EventFunc eventCallback;
				void* userData;
				bool enabled;
			};
			
			
			PrivData privData;

		private:
			//Implement this in the inherited classes for collision against that shape.
			virtual bool Collision(InputClass *input) = 0;

		public:
			EventButton();
			EventButton(Owner owner);
			EventButton(EventFunc func);
			EventButton(EventFunc func, Owner owner);
			EventButton(EventFunc func, Owner owner, void* userData);
			~EventButton();

			void Update(InputClass *input);

			//Send event to callback function
			void SendEvent(ButtonState state);
			
			//Set
			void SetUserData(void* data);
			void SetEventFunc(EventFunc func);
			void SetOwner(Owner owner);
			
			//Get
			bool Enabled();
			unsigned int GetID();
			//EventFunc GetFunctionPointer();
			Owner GetOwner();

			bool operator ==(const EventButton<Owner>& obj);

		};
		
		template <typename Owner>
		unsigned int EventButton<Owner>::PrivData::currID = 0;

		template <typename Owner>
		EventButton<Owner>::EventButton()
		{
			this->privData.eventCallback = NULL;
			this->privData.userData = NULL;
			this->privData.previousState = ButtonState_None;
			this->privData.enabled = true;
		}

		template <typename Owner>
		EventButton<Owner>::EventButton(Owner owner)
		{
			this->privData.owner = owner;
			this->privData.eventCallback = NULL;
			this->privData.userData = NULL;
			this->privData.previousState = ButtonState_None;
			this->privData.enabled = true;
		}

		template <typename Owner>
		EventButton<Owner>::EventButton(EventFunc func)
		{
			this->privData.eventCallback = func;
			this->privData.userData = NULL;
			this->privData.previousState = ButtonState_None;
			this->privData.enabled = true;
		}

		template <typename Owner>
		EventButton<Owner>::EventButton(EventFunc func, Owner owner)
		{
			this->privData.owner = owner;
			this->privData.eventCallback = func;
			this->privData.userData = NULL;
			this->privData.previousState = ButtonState_None;
			this->privData.enabled = true;
		}
		
		template <typename Owner>
		EventButton<Owner>::EventButton(EventFunc func, Owner owner, void* userData)
		{
			this->privData.owner = owner;
			this->privData.eventCallback = func;
			this->privData.userData = userData;
			this->privData.previousState = ButtonState_None;
			this->privData.enabled = true;
		}

		template <typename Owner>
		EventButton<Owner>::~EventButton()
		{}
		
		//Checks for collision and 
		template <typename Owner>
		void EventButton<Owner>::Update(InputClass *input)
		{
			if(this->privData.enabled)
			{
				ButtonState currentState = ButtonState_None;

				if(Collision(input))
				{
					if(input->IsMousePressed())
					{
						//Change state when the mouse button is pressed
						switch(this->privData.previousState)
						{
						case ButtonState_None:
							currentState = ButtonState_Hover;
							break;

						case ButtonState_Hover:
						case ButtonState_Released:
							currentState = ButtonState_Pressed;
							break;

						case ButtonState_Pressed:
						case ButtonState_Down:
							currentState = ButtonState_Down;
							break;
						default:
							break;
						}
					}
					else
					{
						//Change state when the mouse button is NOT pressed
						switch(this->privData.previousState)
						{
						case ButtonState_None:
						case ButtonState_Hover:
						case ButtonState_Released:
							currentState = ButtonState_Hover;
							break;

						case ButtonState_Pressed:
						case ButtonState_Down:
							currentState = ButtonState_Released;
							break;
						default:
							break;
						}
					}
				}

				//Only call the callback function when the state has changed.
				if(this->privData.previousState != currentState)
					SendEvent(currentState);

				this->privData.previousState = currentState;
			}
		}

		template <typename Owner>
		void EventButton<Owner>::SendEvent(ButtonState state)
		{
			if(privData.eventCallback != NULL)
			{
				Oyster::Event::ButtonEvent<Owner> event;
				event.state = state;
				event.sender = this;
				event.owner = privData.owner;
				event.userData = privData.userData;
				privData.eventCallback(event);
			}
		}
		
		template <typename Owner>
		void EventButton<Owner>::SetUserData(void* data)
		{
			this->privData.userData = data;
		}

		template <typename Owner>
		void EventButton<Owner>::SetEventFunc(EventFunc func)
		{
			this->privData.EventFunc = EventFunc;
		}

		template <typename Owner>
		void EventButton<Owner>::SetOwner(Owner owner)
		{
			this->privData.owner = owner;
		}
		
		template <typename Owner>
		bool EventButton<Owner>::Enabled()
		{
			return this->privData.enabled;
		}

		template <typename Owner>
		unsigned int EventButton<Owner>::GetID()
		{
			return this->privData.ID;
		}

		/* Something is wrong, can't return EventFunc
		template <typename Owner>
		EventFunc EventButton<Owner>::GetFunctionPointer()
		{
			return this->privData.eventCallback;
		}*/

		template <typename Owner>
		Owner EventButton<Owner>::GetOwner()
		{
			return this->privData.owner;
		}
		
		template <typename Owner>
		bool EventButton<Owner>::operator ==(const EventButton<Owner>& obj)
		{
			return (this->privData.ID == obj.privData.ID);
		}
	}
}


#endif