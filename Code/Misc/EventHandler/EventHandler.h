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
			EventHandler();
			~EventHandler();

			EventHandler& Instance();

			void Update(InputClass* inputObject);

			EventButtonCollection* CreateCollection();
			EventButtonCollection* GetCollection(/*ID*/);


		private:
			std::vector<EventButtonCollection*> collections;

		};
	}
}

#endif