/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CUSTOM_NETWORK_PROTOCOL_H
#define NETWORK_CUSTOM_NETWORK_PROTOCOL_H

#include <string>
//#include <vld.h>

#ifdef CUSTOM_NET_PROTOCOL_EXPORT
	#define NET_PROTOCOL_EXPORT __declspec(dllexport)
#else
	#define NET_PROTOCOL_EXPORT __declspec(dllimport)
#endif

namespace Oyster
{
	namespace Network
	{
		extern "C"
		{
			//Please don't create a type with a number higher than 127 for now.
			//This may increase to 255 later on.
			enum NetAttributeType
			{
				NetAttributeType_Bool,
				NetAttributeType_Char,
				NetAttributeType_UnsignedChar,
				NetAttributeType_Short,
				NetAttributeType_UnsignedShort,
				NetAttributeType_Int,
				NetAttributeType_UnsignedInt,
				NetAttributeType_Int64,
				NetAttributeType_UnsignedInt64,
				NetAttributeType_Float,
				NetAttributeType_Double,
				NetAttributeType_CharArray,
				NetAttributeType_UNKNOWN,
			};
			union NetAttributeValue
			{
				bool					netBool;
				char					netChar;
				unsigned char			netUChar;
				short					netShort;
				unsigned short			netUShort;
				int						netInt;
				unsigned int			netUInt;
				__int64					netInt64;
				unsigned __int64		netUInt64;
				float					netFloat;
				double					netDouble;
				char*					netCharPtr;
			
				NetAttributeValue(){ memset(this, 0, sizeof(NetAttributeValue)); }
				NetAttributeValue(bool				v)	: netBool	(v) {}
				NetAttributeValue(char				v)	: netChar	(v) {}
				NetAttributeValue(unsigned char		v)	: netUChar	(v) {}
				NetAttributeValue(short				v)	: netShort	(v) {}
				NetAttributeValue(unsigned short	v)	: netUShort	(v) {}
				NetAttributeValue(int				v)	: netInt	(v) {}
				NetAttributeValue(unsigned int		v)	: netUInt	(v) {}
				NetAttributeValue(__int64			v)	: netInt64	(v) {}
				NetAttributeValue(unsigned __int64	v)	: netUInt64	(v) {}
				NetAttributeValue(float				v)	: netFloat	(v) {}
				NetAttributeValue(double			v)	: netDouble	(v) {}
				NetAttributeValue(char*				v)	: netCharPtr(v) {}
			};
			struct NetAttributeContainer
			{
				NetAttributeType type;
				NetAttributeValue value;
				NetAttributeContainer() { type = NetAttributeType_UNKNOWN; }
			};
			class CustomNetProtocol;
			struct CustomProtocolObject
			{
				virtual CustomNetProtocol* GetProtocol() = 0;
			};

			class NET_PROTOCOL_EXPORT CustomNetProtocol
			{
			public:
				CustomNetProtocol();
				~CustomNetProtocol();
				CustomNetProtocol(const CustomNetProtocol& o);
				const CustomNetProtocol& operator=(const CustomNetProtocol& o);

				NetAttributeContainer& operator[](int ID);

			private:
				struct PrivateData;
				PrivateData* privateData;

				friend class Translator;
			};

		}//End extern "C"
	} //End namespace Network
}
#endif // !NETWORK_CUSTOM_NETWORK_PROTOCOL_H












