#include "Octree.h"

using namespace Oyster;
using namespace	Physics;
using namespace ::Utility::DynamicMemory;

const unsigned int Octree::invalid_ref = ::Utility::Value::numeric_limits<unsigned int>::max();

Octree::Octree(unsigned int bufferSize, unsigned char numLayers, Math::Float3 worldSize)
{
	this->worldNode.dataPtr = NULL;

	this->worldNode.container.maxVertex = worldSize*0.5f;
	this->worldNode.container.minVertex = -worldSize*0.5f;
}

Octree::~Octree()
{

}

Octree& Octree::operator=(const Octree& orig)
{
	this->leafData = orig.leafData;
	this->updateQueue = orig.updateQueue;
	this->worldNode = orig.worldNode;
	this->mapReferences = orig.mapReferences;

	return *this;
}

void Octree::AddObject(UniquePointer< ICustomBody > customBodyRef)
{
	customBodyRef->SetScene( this );
	Data data;
	//Data* tempPtr = this->worldNode.dataPtr;

	data.container = customBodyRef->GetBoundingSphere();
	data.queueRef = -1;
	data.next = NULL;
	data.prev = NULL;
	data.customBodyRef = customBodyRef;
	this->mapReferences.insert(std::pair <ICustomBody*, unsigned int> (data.customBodyRef, this->leafData.size()));
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

void Octree::MoveToUpdateQueue(UniquePointer< ICustomBody > customBodyRef)
{
	/*this->leafData[this->mapReferences[customBodyRef]].queueRef = this->updateQueue.size();
	this->updateQueue.push_back(&this->leafData[this->mapReferences[customBodyRef]]);*/
}

void Octree::DestroyObject(UniquePointer< ICustomBody > customBodyRef)
{
	std::map<const ICustomBody*, unsigned int>::iterator it = this->mapReferences.find(customBodyRef);

	this->mapReferences.erase(it);

	this->leafData.erase(this->leafData.begin() + this->leafData[this->mapReferences[customBodyRef]].queueRef);
}

std::vector<ICustomBody*>& Octree::Sample(ICustomBody* customBodyRef, std::vector<ICustomBody*>& updateList)
{
	auto object = this->mapReferences.find(customBodyRef);

	if(object == this->mapReferences.end())
	{	
		return updateList;
	}	

	unsigned int tempRef = object->second;

	for(unsigned int i = 0; i<this->leafData.size(); i++)
	{
		if(tempRef != i) if(this->leafData[tempRef].container.Intersects(this->leafData[i].container))
		{
			updateList.push_back(this->leafData[i].customBodyRef);
		}
	}

	return updateList;
}

std::vector<ICustomBody*>& Octree::Sample(const Oyster::Collision3D::ICollideable& collideable, std::vector<ICustomBody*>& updateList)
{
	for(unsigned int i = 0; i<this->leafData.size(); i++)
	{
		if(this->leafData[i].container.Intersects(collideable))
		{
			updateList.push_back(this->leafData[i].customBodyRef);
		}
	}

	return updateList;
}

void Octree::Visit(ICustomBody* customBodyRef, VistorAction hitAction )
{
	auto object = this->mapReferences.find(customBodyRef);

	if(object == this->mapReferences.end())
	{	
		return;
	}

	unsigned int tempRef = object->second;

	for(unsigned int i = 0; i<this->leafData.size(); i++)
	{
		if(tempRef != i) if(this->leafData[tempRef].container.Intersects(this->leafData[i].container))
		{
			hitAction(*this, tempRef, i);
		}
	}
}

void Octree::Visit(const Oyster::Collision3D::ICollideable& collideable, VistorAction hitAction)
{
	for(unsigned int i = 0; i<this->leafData.size(); i++)
	{
		if(this->leafData[i].container.Intersects(collideable))
		{
			//hitAction(*this, tempRef, i); // @todo TODO: Add typedef to handle function calls with ICollideable
		}
	}
}

ICustomBody* Octree::GetCustomBody(const unsigned int tempRef)
{
	return this->leafData[tempRef].customBodyRef;
}

UniquePointer<ICustomBody> Octree::Extract( const ICustomBody* objRef )
{ // Dan Andersson
	auto iter = this->mapReferences.find( objRef );
	if( iter != this->mapReferences.end() )
	{
		return this->Extract( iter->second );
	}
	else
	{
		return NULL;
	}
}

UniquePointer<ICustomBody> Octree::Extract( unsigned int tempRef )
{
	if( tempRef != Octree::invalid_ref )
	{
		//! @todo TODO: implement stub
		return NULL;
	}
	else
	{
		return NULL;
	}
}

unsigned int Octree::GetTemporaryReferenceOf( const ICustomBody* objRef ) const
{ // Dan Andersson
	auto iter = this->mapReferences.find( objRef );
	if( iter != this->mapReferences.end() )
	{
		return iter->second;
	}
	else
	{
		return Octree::invalid_ref;
	}
}

void Octree::SetAsAltered( unsigned int tempRef )
{
	this->leafData[tempRef].container = this->leafData[tempRef].customBodyRef->GetBoundingSphere();
	//! @todo TODO: implement stub
}

void Octree::EvaluatePosition( unsigned int tempRef )
{
	//! @todo TODO: implement stub
}