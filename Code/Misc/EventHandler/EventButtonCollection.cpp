#include "EventButtonCollection.h"

using namespace Oyster::Event;

EventButtonCollection::EventButtonCollection() 
	: collectionState(EventCollectionState_Enabled)
{
}

EventButtonCollection::~EventButtonCollection()
{
}

void EventButtonCollection::Update(InputClass* inputObject)
{
	if(this->collectionState == EventCollectionState_Enabled)
	{
		for(int i = 0; i < buttons.size(); i++)
		{
			buttons.at(i)->CheckCollision(inputObject);
		}
	}
}

template <class Owner>
void EventButtonCollection::AddButton(EventButton<Owner>& button)
{
	buttons.push_back(button);
}

template <class Owner>
EventButton<Owner>& EventButtonCollection::CreateButton()
{
	EventButton temp;
	buttons.push_back(&temp);
	return temp;
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