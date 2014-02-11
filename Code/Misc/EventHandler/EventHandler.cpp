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
	}
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
	collections.push_back(collection);
}