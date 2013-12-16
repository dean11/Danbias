#ifndef PACKING_H
#define PACKING_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include <string>

/******************************
		Packing
******************************/
namespace Oyster
{
	namespace Network
	{
		namespace Packing
		{
			//bool (1-bit)
			void Pack(unsigned char buffer[], bool i);

			//char (8-bit)
			void Pack(unsigned char buffer[], char i);
			void Pack(unsigned char buffer[], unsigned char i);		// unsigned

			//short (16-bit)
			void Pack(unsigned char buffer[], short i);
			void Pack(unsigned char buffer[], unsigned short i);	// unsigned

			//int (32-bit)
			void Pack(unsigned char buffer[], int i);
			void Pack(unsigned char buffer[], unsigned int i);		// unsigned

			//__int64 (64-bit)
			void Pack(unsigned char buffer[], __int64 i);
			void Pack(unsigned char buffer[], unsigned __int64 i);	// unsigned

			//floating point (32, 64-bit)
			void Pack(unsigned char buffer[], float i);
			void Pack(unsigned char buffer[], double i);

			//string
			void Pack(unsigned char buffer[], char str[]);
			void Pack(unsigned char buffer[], std::string& str);

			unsigned __int64 Pack754(long double f, unsigned bits, unsigned expbits);

			/******************************
					Unpacking
			******************************/

			//bool (1-bit)
			bool Unpackb(unsigned char buffer[]);

			//char (8-bit)
			char Unpackc(unsigned char buffer[]);
			unsigned char UnpackC(unsigned char buffer[]);		// unsigned

			//short (16-bit)
			short Unpacks(unsigned char buffer[]);
			unsigned short UnpackS(unsigned char buffer[]);		// unsigned

			//int (32-bit)
			int Unpacki(unsigned char buffer[]);
			unsigned int UnpackI(unsigned char buffer[]);		// unsigned

			//__int64 (64-bit)
			__int64 Unpacki64(unsigned char buffer[]);
			unsigned __int64 UnpackI64(unsigned char buffer[]);	// unsigned

			//floating point (32, 64-bit)
			float Unpackf(unsigned char buffer[]);
			double Unpackd(unsigned char buffer[]);

			//string
			char* UnpackCStr(unsigned char buffer[]);
			std::string UnpackStr(unsigned char buffer[]);

			long double Unpack754(unsigned __int64 i, unsigned bits, unsigned expbits);
		}
	}
}


//int32_t pack(unsigned char* buffer, char* format, ...);

//void unpack(unsigned char* buffer, char* format, ...);


/***********************************************
*	This table is used for naming pack/unpack functions.
*	It's also used to identify types in the 'format' string in function pack()/unpack()
*
*   bits |signed   unsigned   float   string
*   -----+----------------------------------
*      1 |	 b
*      8 |   c        C         
*     16 |   s        S         f
*     32 |   i        I         d
*     64 |   q        Q         g
*      - |                              str
*
*   (16-bit unsigned length is automatically added in front of strings)
*
*/

#endif