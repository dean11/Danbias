//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_BUTTON_COLLECTION_H
#define MISC_EVENT_BUTTON_COLLECTION_H

#include "../DynamicArray.h"

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

		/********************************
		This EventButtonCollection will handle the destruction of the buttons when they are added to the collection
		********************************/
		class EventButtonCollection
		{
		public:
			EventButtonCollection(EventCollectionState state = EventCollectionState_Enabled);
			~EventButtonCollection();

			void Update(MouseInput& input);
			void RenderTexture();
			void RenderText();

			/*Add a button to the collection when a button is added to the collection you are not allowed to delete it.
			*/
			template <typename Owner> void AddButton(EventButton<Owner>* button)
			{
				buttons.push_back(button);
			}

			EventCollectionState GetState() const;
			void SetState(const EventCollectionState state);

			//Clear all buttons and reset the state.
			void Clear();

		private:
			//Can't copy
			EventButtonCollection(const EventButtonCollection& obj);
			EventButtonCollection& operator =(const EventButtonCollection& obj);

		protected:
			std::vector<IEventButton*> buttons;
			EventCollectionState collectionState;

		};
	}
}

#endif