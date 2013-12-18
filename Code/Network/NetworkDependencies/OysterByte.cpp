#include "OysterByte.h"

using namespace Oyster::Network;

OysterByte::OysterByte()
{
	size = 0;
	capacity = 10;
	byteArray = new unsigned char[capacity];
}

OysterByte::OysterByte(int cap)
{
	size = 0;
	capacity = cap;
	byteArray = new unsigned char[capacity];
}

OysterByte::OysterByte(const OysterByte& obj)
{
	this->byteArray = new unsigned char[obj.capacity];

	for(int i = 0; i < (int)obj.size; i++)
	{
		this->byteArray[i] = obj.byteArray[i];
	}
	this->size = obj.size;
	this->capacity = obj.capacity;
}

OysterByte::~OysterByte()
{
	delete[] byteArray;
}

void OysterByte::Clear()
{
	size = 0;
}

void OysterByte::Resize(unsigned int cap)
{
	if(capacity < cap)
	{
		delete[] byteArray;
		byteArray = new unsigned char[cap];
		capacity = cap;
	}
}

int OysterByte::GetSize()
{
	return size;
}

unsigned char* OysterByte::GetByteArray()
{
	return byteArray;
}

void OysterByte::AddSize(unsigned int size)
{
	int newCapacity = this->size + size;

	if(newCapacity >= capacity)
	{
		IncreaseCapacity(newCapacity);
	}

	this->size += size;
}

void OysterByte::SetBytes(unsigned char* bytes)
{
	delete[] byteArray;
	byteArray = bytes;
}

void OysterByte::SetSize(unsigned int size)
{
	this->size = size;
}

OysterByte& OysterByte::operator =(const OysterByte& obj)
{
	delete[] this->byteArray;
	this->byteArray = new unsigned char[obj.capacity];

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
	unsigned char* temp = new unsigned char[capacity];
	
	for(int i = 0; i < (int)this->size; i++)
	{
		temp[i] = byteArray[i];
	}

	delete[] byteArray;
	byteArray = temp;
}