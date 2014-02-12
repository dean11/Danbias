//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#include "EventHandler.h"

using namespace Oyster::Event;

Oyster::Event::EventHandler EvtHandler;

EventHandler& EventHandler::Instance()
{
	return EvtHandler;
}

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
	int size = collections.size();
	for(int i = 0; i < size; i++)
	{
		delete collections[i];
		collections[i] = NULL;
	}
}

void EventHandler::Clean()
{
	int size = collections.size();
	for(int i = 0; i < size; i++)
	{
		delete collections[i];
		collections[i] = NULL;
	}
	collections.clear();
}

void EventHandler::Update(InputClass* inputObject)
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		collections.at(i)->Update(inputObject);
	}
}

void EventHandler::Render()
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		collections.at(i)->Render();
	}
}

void EventHandler::AddCollection(EventButtonCollection* collection)
{
	for(int i = 0; i < collections.size(); i++)
	{
		//Do not add the collection if it's already in the list.
		if(collections.at(i) == collection)
			return;
	}
	collections.push_back(collection);
}

void EventHandler::DeleteCollection(EventButtonCollection* collection)
{
	for(int i = 0; i < collections.size(); i++)
	{
		if(collections.at(i) == collection)
		{
			delete collection;
			collection = NULL;
			collections.erase(collections.begin() + i);
			break;
		}
	}
}