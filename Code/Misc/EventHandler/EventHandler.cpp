#include "EventHandler.h"

using namespace Oyster::Event;

EventHandler EvtHandler;

EventHandler& EventHandler::Instance()
{
	return EvtHandler;
}

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::Update(InputClass* inputObject)
{
	for(int i = 0; i < collections.size(); i++)
	{
		collections.at(i)->Update(inputObject);
	}
}

void EventHandler::AddCollection(EventButtonCollection& collection)
{
	collections.push_back(&collection);
}

EventButtonCollection& EventHandler::CreateCollection()
{

}