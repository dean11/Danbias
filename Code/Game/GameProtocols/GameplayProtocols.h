#ifndef GAMEPROTOCOLS_GAMEPLAYPROTOCOLS_H
#define GAMEPROTOCOLS_GAMEPLAYPROTOCOLS_H

#include <vector>
#include <CustomNetProtocol.h>
#include "ProtocolIdentificationID.h"

namespace GameLogic
{
	struct Protocol_GameplayInitiateSession :public Oyster::Network::CustomProtocolObject
	{
		struct SessionInitData
		{
			int id;
			int type;
			//std::string text;
		};
		std::vector<SessionInitData> element;

		Protocol_GameplayInitiateSession()
		{
			this->protocol[protocol_INDEX_ID].value = protocol_Gameplay_Initiate;
			this->protocol[protocol_INDEX_ID].type = Oyster::Network::NetAttributeType_Short;
		}
		Protocol_GameplayInitiateSession(Oyster::Network::CustomNetProtocol& p)
		{
			int size = p[1].value.netInt;
			for (int i = 0; i < size; i++)
			{
				SessionInitData d = { p[i+2].value.netInt, p[i+3].value.netInt /*, p[i+4].value.netCharPtr */ };
				element.push_back(d);
			}
		}
		void Add(int id, int type, std::string text)
		{
			SessionInitData d = { id, type /*, text*/ };
			element.push_back(d);
		}
		Oyster::Network::CustomNetProtocol* GetProtocol() override
		{
			//Store the elements count
			this->protocol[1].value = element.size();
			this->protocol[1].type = Oyster::Network::NetAttributeType_Int;

			for (unsigned int i = 0; i < element.size(); i+=3)
			{
				this->protocol[i+2].value = element[i].id;
				this->protocol[i+3].value = element[i].type;
				//element[i].text.copy(this->protocol[i+4].value.netCharPtr, element[i].text.size()); 

				this->protocol[i+2].type = Oyster::Network::NetAttributeType_Int;
				this->protocol[i+3].type = Oyster::Network::NetAttributeType_Int;
				//this->protocol[i+4].type = Oyster::Network::NetAttributeType_CharArray;
			}
			return &protocol;		 
		}							 

	private:
		Oyster::Network::CustomNetProtocol protocol;
	};
}

#endif // !GAMEPROTOCOLS_GAMEPLAYPROTOCOLS_H
