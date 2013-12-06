/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CUSTOM_NETWORK_PROTOCOL_H
#define NETWORK_CUSTOM_NETWORK_PROTOCOL_H

#include <string>

#ifdef CUSTOM_NET_PROTOCOL_EXPORT
	#define NET_PROTOCOL_EXPORT __declspec(dllexport)
#else
	#define NET_PROTOCOL_EXPORT __declspec(dllimport)
#endif

namespace Network
{
	extern "C"
	{
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
		class NET_PROTOCOL_EXPORT CustomNetProtocol
		{
		public:
			CustomNetProtocol();
			~CustomNetProtocol();

			NetAttributeContainer* operator[](int ID);

			NetAttributeContainer* BuildValue(NetAttributeContainer value, short attributeId);
			NetAttributeContainer* BuildValue(NetAttributeValue value, NetAttributeType type, short attributeId);
			NetAttributeContainer* BuildValue(const bool				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const char				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const unsigned char		& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const short				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const unsigned short		& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const int					& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const unsigned int		& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const __int64				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const unsigned __int64	& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const float				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const double				& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const std::string			& attribute, short attributeId);
			NetAttributeContainer* BuildValue(const char				* attribute, short attributeId);

			//void SetArrayValue(NetAttributeContainer attribute[], unsigned int attributeCount, int attributeId);

			NetAttributeContainer& GetSingleValue(int attributeID);
			//NetAttributeContainer* GetArrayValue(int attributeID);

		private:
			struct PrivateData;
			PrivateData* privateData;
		};

	}//End extern "C"
} //End namespace Network
#endif // !NETWORK_CUSTOM_NETWORK_PROTOCOL_H












