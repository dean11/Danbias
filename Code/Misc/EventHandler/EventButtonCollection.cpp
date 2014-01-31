#include "EventButtonCollection.h"

using namespace Oyster::Event;

EventButtonCollection::EventButtonCollection()
{

}

EventButtonCollection::~EventButtonCollection()
{
}

void EventButtonCollection::Update(InputClass* inputObject)
{
	for(int i = 0; i < buttons.size(); i++)
	{
		buttons.at(i)->Update(inputObject);
	}
}

EventButton* EventButtonCollection::AddButton(EventButton* button)
{

}