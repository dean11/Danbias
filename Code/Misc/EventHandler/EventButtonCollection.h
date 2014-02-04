#ifndef EVENT_BUTTON_COLLECTION_H
#define EVENT_BUTTON_COLLECTION_H

#include "../../Input/L_inputClass.h"

#include "IEventButton.h"
#include "EventButton.h"

#include <vector>

namespace Oyster
{
	namespace Event
	{
		enum EventCollectionState
		{
			EventCollectionState_Disabled,
			EventCollectionState_Enabled,

			EventCollectionState_Count,
			EventCollectionState_Unknown = -1,
		};

		class EventButtonCollection
		{
		public:
			EventButtonCollection();
			~EventButtonCollection();

			void Update(InputClass* inputObject);

			template <typename Owner>
			void AddButton(EventButton<Owner>& button)
			{
				EventButton<Owner>* b = new EventButton<Owner>();
				buttons.size(); 
				buttons.push_back((IEventButton*)b);
				//buttons.push_back((IEventButton*)&button);
			}

			template <typename Owner>
			EventButton<Owner>& CreateButton()
			{
				EventButton<Owner> temp;
				buttons.push_back(&temp);
				return temp;
			}

			EventCollectionState GetState() const;
			void SetState(const EventCollectionState state);

			//Clear all buttons and reset the state.
			void Clear();

		private:
			std::vector<IEventButton*> buttons;
			EventCollectionState collectionState;

		};
	}
}

#endif