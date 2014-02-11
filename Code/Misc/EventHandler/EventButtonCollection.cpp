//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#include "EventButtonCollection.h"

#include "../../Input/L_inputClass.h"

using namespace Oyster::Event;

EventButtonCollection::EventButtonCollection() 
	: collectionState(EventCollectionState_Enabled)
{
}

EventButtonCollection::~EventButtonCollection()
{
	int size = buttons.size();
	for(int i = 0; i < size; i++)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
}

void EventButtonCollection::Update(InputClass* inputObject)
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < (int)buttons.size(); i++)
		{
			buttons[i]->Update(inputObject);
		}
	}
}

EventCollectionState EventButtonCollection::GetState() const
{
	return collectionState;
}

void EventButtonCollection::SetState(const EventCollectionState state)
{
	collectionState = state;
}

void EventButtonCollection::Clear()
{
	buttons.clear();
	collectionState = EventCollectionState_Enabled;
}