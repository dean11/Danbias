#include "OysterByte.h"

using namespace Oyster::Network;



OysterByte::OysterByte()
{
	size = 0;
	capacity = 1400;
#if test == TRUE
	byteArray = new unsigned char[capacity];
#endif
}

OysterByte::OysterByte(int cap)
{
	size = 0;
	capacity = cap;
#if test == TRUE
	byteArray = new unsigned char[capacity];
#endif
}

OysterByte::OysterByte(const OysterByte& obj)
{
#if test == TRUE
	this->byteArray = new unsigned char[obj.capacity];
#endif
	for(int i = 0; i < (int)obj.size; i++)
	{
		this->byteArray[i] = obj.byteArray[i];
	}
	this->size = obj.size;
	this->capacity = obj.capacity;
}

OysterByte::~OysterByte()
{
#if test == TRUE
	delete[] byteArray;
#endif
}

void OysterByte::Clear()
{
	size = 0;
}

void OysterByte::Resize(unsigned int cap)
{
	if(capacity < cap)
	{
#if test == TRUE
		delete[] byteArray;
		byteArray = new unsigned char[cap];
#endif
		capacity = cap;
	}
}

unsigned int OysterByte::GetSize()
{
	return size;
}

unsigned char* OysterByte::GetByteArray()
{
	return byteArray;
}

void OysterByte::AddSize(unsigned int size)
{
	unsigned int newCapacity = this->size + size;

	if(newCapacity >= capacity)
	{
		IncreaseCapacity(newCapacity);
	}

	this->size += size;
}

void OysterByte::SetBytes(unsigned char* bytes)
{
#if test == TRUE
	delete[] byteArray;
	byteArray = bytes;
#endif
}

void OysterByte::SetSize(unsigned int size)
{
	this->size = size;
}

void OysterByte::AppendPartOfArray(OysterByte& source, unsigned int startIndex, unsigned int endIndex)
{
	if(startIndex < 0 && startIndex >= endIndex)
		return;
	if(endIndex > source.size)
		return;

	unsigned int totalSize = endIndex - startIndex;
	totalSize += size;

	//Make sure the new data can fit in the array.
	if(totalSize > capacity)
	{
		IncreaseCapacity(totalSize);
	}

	//Copy over new data.
	for(unsigned int i = size; i < totalSize; i++)
	{
		byteArray[i] = source.byteArray[startIndex++];
	}

	//Set the new size
	size = totalSize;
}

OysterByte& OysterByte::operator =(const OysterByte& obj)
{
#if test == TRUE
	delete[] this->byteArray;
	this->byteArray = new unsigned char[obj.capacity];
#endif

	for(int i = 0; i < (int)obj.size; i++)
	{
		this->byteArray[i] = obj.byteArray[i];
	}
	this->size = obj.size;
	this->capacity = obj.capacity;

	return *this;
}

OysterByte::operator char*()
{
	return (char*)byteArray;
}

OysterByte::operator const char*()
{
	return (const char*)byteArray;
}

OysterByte::operator unsigned char*()
{
	return byteArray;
}

OysterByte& OysterByte::operator +=(const OysterByte& obj)
{
	int newSize = this->size + obj.size;
	
	if(newSize >= (int)capacity)
	{
		IncreaseCapacity(newSize);
	}

	for(int i = size, j = 0; i < newSize; i++, j++)
	{
		this->byteArray[i] = obj.byteArray[j];
	}
	
	this->size = newSize;

	return *this;
}

/////////////
// Private //
/////////////

void OysterByte::IncreaseCapacity(unsigned int newCapacity)
{
	capacity = newCapacity * 2;
#if test == TRUE
	unsigned char* temp = new unsigned char[capacity];
	
	for(int i = 0; i < (int)this->size; i++)
	{
		temp[i] = byteArray[i];
	}
	
	delete[] byteArray;
	byteArray = temp;
#endif
}