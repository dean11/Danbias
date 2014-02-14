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
	Clean();
}

void EventHandler::Clean()
{
	collections.clear();
}

void EventHandler::Update(MouseInput& input)
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		collections.at(i)->Update(input);
	}
}

void EventHandler::RenderTexture()
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		collections.at(i)->RenderTexture();
	}
}

void EventHandler::RenderText()
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		collections.at(i)->RenderText();
	}
}

void EventHandler::AddCollection(EventButtonCollection* collection)
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		//Do not add the collection if it's already in the list.
		if(collections.at(i) == collection)
			return;
	}
	collections.push_back(collection);
}

void EventHandler::ReleaseCollection(EventButtonCollection* collection)
{
	for(int i = 0; i < (int)collections.size(); i++)
	{
		if(collections.at(i) == collection)
		{
			collections.erase(collections.begin() + i);
			break;
		}
	}
}