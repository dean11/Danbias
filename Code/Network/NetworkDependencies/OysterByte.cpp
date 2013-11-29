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

OysterByte::~OysterByte()
{
	delete[] byteArray;
}

void OysterByte::Clear(unsigned int cap)
{
	delete[] byteArray;
	byteArray = new unsigned char[cap];
	size = 0;
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
	int oldSize = this->size;
	this->size += size;

	if(this->size >= capacity)
	{
		IncreaseCapacity(oldSize);
	}
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

/////////////
// Private //
/////////////

void OysterByte::IncreaseCapacity(unsigned int oldSize)
{
	capacity = size * 2;
	unsigned char* temp = new unsigned char[capacity];
	
	for(int i = 0; i < (int)oldSize; i++)
	{
		temp[i] = byteArray[i];
	}

	delete[] byteArray;
	byteArray = temp;
}