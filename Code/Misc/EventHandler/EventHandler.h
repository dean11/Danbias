//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_HANDLER_H
#define MISC_EVENT_HANDLER_H

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
			void Render();

			void AddCollection(EventButtonCollection* collection);

		private:
			std::vector<EventButtonCollection*> collections;

		};
	}
}

#endif