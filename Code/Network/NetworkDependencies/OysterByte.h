#ifndef NETWORK_DEPENDENCIES_OYSTER_BYTE_H
#define NETWORK_DEPENDENCIES_OYSTER_BYTE_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#define TRUE 1
#define FALSE 0
#define test (FALSE)

#include <string.h>

namespace Oyster
{
	namespace Network
	{
		class OysterByte
		{
		public:
			OysterByte();
			OysterByte(int cap);
			OysterByte(const OysterByte& obj);
			virtual ~OysterByte();

			//Resets size to 0
			void Clear();

			//Resizes the array with, it does not keep anything in it.
			void Resize(unsigned int cap);

			unsigned int GetSize();
			unsigned char* GetByteArray();

			void AddSize(unsigned int size);
			void SetBytes(unsigned char* bytes);

			//Only sets the private variable 'size'
			void SetSize(unsigned int size);

			//Copies over a part of the addFrom array and adds it to the end of this array. 
			void AppendPartOfArray(OysterByte& source, unsigned int startIndex, unsigned int endIndex);

			OysterByte& operator =(const OysterByte& obj);

			operator char*();
			operator const char*();
			operator unsigned char*();

			OysterByte& operator +=(const OysterByte& obj);

		private:
			//Expands the byteArray
			void IncreaseCapacity(unsigned int cap);


		private:
#if (test == TRUE)
			unsigned char* byteArray;
#else
			unsigned char byteArray[1400];
#endif
			unsigned int size;
			unsigned int capacity;

		};
	}
}

#endif