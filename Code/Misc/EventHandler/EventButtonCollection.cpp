//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#include "EventButtonCollection.h"
#include "EventHandler.h"
#include "../../Input/L_inputClass.h"

using namespace Oyster::Event;

EventButtonCollection::EventButtonCollection() 
	: collectionState(EventCollectionState_Enabled)
{
}

EventButtonCollection::~EventButtonCollection()
{
	for(int i = 0; i < EventHandler::Instance().collections.size(); i++)
	{
		if(EventHandler::Instance().collections.at(i) == this)
		{
			EventHandler::Instance().collections.erase(EventHandler::Instance().collections.begin() + i);
		}
	}

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

void EventButtonCollection::Render()
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < (int)buttons.size(); i++)
		{
			buttons[i]->Render();
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