//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_EVENT_BUTTON_COLLECTION_H
#define MISC_EVENT_BUTTON_COLLECTION_H

#include "../../Input/L_inputClass.h"

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
		This EventButtonCollection will handle the destruction of the buttons when they are added to the collection.

		********************************/
		class EventButtonCollection
		{
		public:
			EventButtonCollection();
			~EventButtonCollection();

			void Update(InputClass* inputObject);
			void Render();

			template <typename Owner>
			void AddButton(EventButton<Owner>* button)
			{
				buttons.push_back(button);
			}

			EventCollectionState GetState() const;
			void SetState(const EventCollectionState state);

			//Clear all buttons and reset the state.
			void Clear();

		protected:
			std::vector<IEventButton*> buttons;
			EventCollectionState collectionState;

		};
	}
}

#endif