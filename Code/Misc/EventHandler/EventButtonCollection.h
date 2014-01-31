#ifndef EVENT_BUTTON_COLLECTION_H
#define EVENT_BUTTON_COLLECTION_H

#include "../../Input/L_inputClass.h"

#include "EventButton.h"

#include <vector>

namespace Oyster
{
	namespace Event
	{
		class EventButtonCollection
		{
		public:
			EventButtonCollection();
			~EventButtonCollection();

			void Update(InputClass* inputObject);

			EventButton* AddButton(EventButton* button);

		private:
			std::vector<EventButton*> buttons;

		};
	}
}

#endif