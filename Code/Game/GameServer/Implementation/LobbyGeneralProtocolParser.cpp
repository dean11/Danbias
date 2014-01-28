#include "GameLobby.h"
#include "NetworkSession.h"

using namespace DanBias;
using namespace GameLogic;

void GameLobby::ParseGeneralProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::NetworkSession* c)
{
	switch (p[0].value.netShort)
	{
		case protocol_General_Status:
		{
			GeneralStatus(GameLogic::Protocol_General_Status(p), c);
		} break;
		case protocol_General_Text:
		{
			GeneralText(GameLogic::Protocol_General_Text(p), c);
		} break;
	}
}

//////////////////////////////////////////////////////

void GameLobby::GeneralStatus(GameLogic::Protocol_General_Status& p, DanBias::NetworkSession* c)
{
	switch (p.status)
	{
		case Protocol_General_Status::States_ready:
		{

		}
		case Protocol_General_Status::States_idle:
		{

		}
		case Protocol_General_Status::States_leave:
		{

		}
		case Protocol_General_Status::States_disconected:
		{
			Detach(c)->Disconnect();
		}
	}
}

void GameLobby::GeneralText(GameLogic::Protocol_General_Text& p, DanBias::NetworkSession* c)
{

}


