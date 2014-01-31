#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../../Input/L_inputClass.h"

#include "EventButtonCollection.h"
#include "EventButton.h"

#include <vector>

namespace Oyster
{
	namespace Event
	{
		class EventHandler
		{
		public:
			EventHandler();
			~EventHandler();

			static EventHandler& Instance();

			void Update(InputClass* inputObject);

			void AddCollection(EventButtonCollection& collection);
			EventButtonCollection& CreateCollection();

		private:
			std::vector<EventButtonCollection*> collections;

		};
	}
}

#endif