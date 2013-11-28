#include "Octree.h"

using namespace Oyster;
using namespace	Physics;

Octree::Octree(unsigned int bufferSize, unsigned char numLayers, Math::Float3 worldSize)
{
	this->worldNode.dataPtr = NULL;

	this->worldNode.container.maxVertex = worldSize*0.5f;
	this->worldNode.container.minVertex = -worldSize*0.5f;
}

Octree::~Octree()
{

}

void Octree::AddObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef)
{
	Data data;
	//Data* tempPtr = this->worldNode.dataPtr;

	data.container = customBodyRef->GetBoundingSphere();
	data.queueRef = -1;
	data.next = NULL;
	data.prev = NULL;
	data.customBodyRef = customBodyRef;
	this->mapReferences.insert(std::pair <ICustomBody*, unsigned int> (customBodyRef, this->leafData.size()));
	this->leafData.push_back(data);

	/*if(tempPtr != NULL)
	{
		tempPtr->prev->next = &this->leafData[this->leafData.size() - 1];
		this->leafData[this->leafData.size() - 1].prev = tempPtr->prev;
		tempPtr->prev = &this->leafData[this->leafData.size() - 1];
		this->leafData[this->leafData.size() - 1].next = tempPtr;
	}
	else
	{
		this->worldNode.dataPtr = &this->leafData[this->leafData.size() - 1];
		this->worldNode.dataPtr->next = this->worldNode.dataPtr;
		this->worldNode.dataPtr->prev = this->worldNode.dataPtr;
	}*/
}

void Octree::MoveToUpdateQueue(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef)
{
	/*this->leafData[this->mapReferences[customBodyRef]].queueRef = this->updateQueue.size();
	this->updateQueue.push_back(&this->leafData[this->mapReferences[customBodyRef]]);*/
}


void Octree::DestroyObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef)
{
	std::map<ICustomBody*, unsigned int>::iterator it = this->mapReferences.find(customBodyRef);

	this->mapReferences.erase(it);

	this->leafData.erase(this->leafData.begin() + this->leafData[this->mapReferences[customBodyRef]].queueRef);
}

std::vector<ICustomBody*> Octree::Sample(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef)
{
	std::vector<ICustomBody*> list;

	for(unsigned int i = 0; i<this->leafData.size(); i++)
	{
		if(this->leafData[i].customBodyRef != customBodyRef) if(this->leafData[i].container.Intersects(customBodyRef->GetBoundingSphere()))
		{
			list.push_back(this->leafData[i].customBodyRef);
		}
	}

	return list;
}
