/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CUSTOM_NETWORK_PROTOCOL_H
#define NETWORK_CUSTOM_NETWORK_PROTOCOL_H

//needs to have dll-interface to be used by clients of class 'Oyster::Network::NetworkSession'
#pragma warning(disable : 4251)

#include <string>
#include "Utilities.h"
//#include <vld.h>
#include "NetworkAPI_Preprocessor.h"

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
				NetAttributeContainer() 
				{ type = NetAttributeType_UNKNOWN; }
				~NetAttributeContainer() 
				{ 
					if (this->type == NetAttributeType_CharArray)
					{
						delete this->value.netCharPtr;
						this->value.netCharPtr = 0;
					}
				}
				NetAttributeContainer(NetAttributeContainer& p) 
				{ 
					type = p.type; 
					if(type == NetAttributeType_CharArray && p.value.netCharPtr)
					{
						size_t len = 0;
						if((len = strlen(p.value.netCharPtr)) == 0) return;
						len++;
						value.netCharPtr = new char[len];
						memcpy(&value.netCharPtr[0], &p.value.netCharPtr[0], sizeof(p.value.netCharPtr[0]) * len);
					}
					else
					{
						value = p.value;
					}
				}
				const NetAttributeContainer& operator=(const NetAttributeContainer& p) 
				{ 
					if(this->type == NetAttributeType_CharArray)
					{
						delete this->value.netCharPtr;
						this->value.netCharPtr = 0;
					}

					type = p.type; 
					if(type == NetAttributeType_CharArray && p.value.netCharPtr)
					{
						size_t len = 0;
						if((len = strlen(p.value.netCharPtr)) == 0) return *this;
						len++;
						value.netCharPtr = new char[len];
						memcpy(&value.netCharPtr[0], &p.value.netCharPtr[0], sizeof(p.value.netCharPtr[0]) * len);
					}
					else
					{
						value = p.value;
					}
					return *this;
				}
			};
			class CustomNetProtocol;
			struct CustomProtocolObject
			{
				virtual CustomNetProtocol GetProtocol() = 0;
			};

			class NET_API_EXPORT CustomNetProtocol
			{
			public:
				CustomNetProtocol();
				~CustomNetProtocol();
				CustomNetProtocol( const CustomNetProtocol& o);
				CustomNetProtocol& operator=(const CustomNetProtocol& o);

				const NetAttributeContainer& operator[](int ID) const;
					  NetAttributeContainer& operator[](int ID);

				void Set(int id, Oyster::Network::NetAttributeValue val, Oyster::Network::NetAttributeType type);
				void Set(int ID, std::string s);
				const NetAttributeContainer& Get(int id);

			private:
				struct PrivateData;
				//Utility::DynamicMemory::SmartPointer<PrivateData> privateData;
				//Utility::Thread::ThreadSafeSmartPointer<PrivateData> privateData;
				PrivateData* privateData;

				friend class Translator;
			};

		}//End extern "C"
	} //End namespace Network
}
#endif // !NETWORK_CUSTOM_NETWORK_PROTOCOL_H












