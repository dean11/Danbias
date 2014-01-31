#ifndef EVENT_BUTTON_COLLECTION_H
#define EVENT_BUTTON_COLLECTION_H

#include "../../Input/L_inputClass.h"

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

			void AddButton(EventButton& button);
			EventButton& CreateButton();

			EventCollectionState GetState() const;
			void SetState(const EventCollectionState state);

			//Clear all buttons and reset the state.
			void Clear();

		private:
			std::vector<EventButton*> buttons;
			EventCollectionState collectionState;

		};
	}
}

#endif