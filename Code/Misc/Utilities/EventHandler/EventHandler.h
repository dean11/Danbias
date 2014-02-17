//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_HANDLER_H
#define MISC_EVENT_HANDLER_H

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

			void Clean();

			void Update(MouseInput& input);
			void RenderTexture();
			void RenderText();

			/*Add a collection to the EventHandler will only add collections not already present in the list.

			*/
			void AddCollection(EventButtonCollection* collection);

			void ReleaseCollection(EventButtonCollection* collection);

		private:
			//Can't copy this class.
			EventHandler(const EventHandler& obj);
			EventHandler& operator =(const EventHandler& obj);

		private:
			std::vector<EventButtonCollection*> collections;

			//EventButtonCollection is a friend so it can delete it self.
			friend class EventButtonCollection;
		};
	}
}

#endif