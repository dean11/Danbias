#ifndef NETWORK_DEPENDENCIES_OYSTER_BYTE_H
#define NETWORK_DEPENDENCIES_OYSTER_BYTE_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

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
			virtual ~OysterByte();

			void Clear(unsigned int cap);

			int GetSize();
			unsigned char* GetByteArray();

			void AddSize(unsigned int size);
			void SetBytes(unsigned char* bytes);
			void SetSize(unsigned int size);		//Only sets the private variable 'size'

			operator char*();
			operator const char*();
			operator unsigned char*();
			
		private:
			void IncreaseCapacity(unsigned int cap);		//Expands the byteArray


		private:
			unsigned char* byteArray;
			unsigned int size;
			unsigned int capacity;

		};
	}
}

#endif