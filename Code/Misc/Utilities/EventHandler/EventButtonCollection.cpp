//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#include "EventButtonCollection.h"
#include "EventHandler.h"

using namespace Oyster::Event;

EventButtonCollection::EventButtonCollection(EventCollectionState state) 
	: collectionState(state)
{
}

EventButtonCollection::~EventButtonCollection()
{
	for(int i = 0; i < (int)EventHandler::Instance().collections.size(); i++)
	{
		if(EventHandler::Instance().collections.at(i) == this)
		{
			EventHandler::Instance().collections.erase(EventHandler::Instance().collections.begin() + i);
			break;
		}
	}

	int size = (int)buttons.size();
	for(int i = 0; i < size; i++)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
}

void EventButtonCollection::Update(MouseInput& input)
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < (int)buttons.size(); i++)
		{
			buttons[i]->Update(input);
		}
	}
}

void EventButtonCollection::RenderTexture()
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < (int)buttons.size(); i++)
		{
			buttons[i]->RenderTexture();
		}
	}
}

void EventButtonCollection::RenderText()
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < (int)buttons.size(); i++)
		{
			buttons[i]->RenderText();
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
	int size = (int)buttons.size();
	for(int i = 0; i < size; i++)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
	buttons.clear();

	collectionState = EventCollectionState_Enabled;
}