#include "ProtocolParser.h"
using namespace DanBias;


void ParseGeneralP(Oyster::Network::CustomNetProtocol& p, const short &f, ProtocolParser::ProtocolData& d)
{
	switch (f)
	{
		case protocol_General_Disconnect:
			
		break;
		case protocol_General_Ping:
			
		break;
		case protocol_General_Text:
			
		break;
	}
}
void ParseLobbyP(Oyster::Network::CustomNetProtocol& p, const short &f, ProtocolParser::ProtocolData& d)
{
	Oyster::Network::CustomNetProtocol* lp;
	d.type = f;
	switch (f)
	{
		case protocol_Lobby_CreateGame:
		{
			//lp = d.p.createGame.GetProtocol();
			//d.p.createGame (*lp)[0].value;
		}	
		break;
		case protocol_Lobby_JoinGame:
		{
			lp = d.p.createGame.GetProtocol();
		}
		break;
		case protocol_Lobby_StartGame:
		{
			lp = d.p.createGame.GetProtocol();
			d.p.startGame.gameId = (*lp)[1].value.netChar;
		}
		break;
		case protocol_Lobby_JoinLobby:
		{
			lp = d.p.createGame.GetProtocol();
		}
		break;
		case protocol_Lobby_LeaveLobby:
		{
			lp = d.p.createGame.GetProtocol();
		}
		break;
	}
}


ProtocolParser::ProtocolData ProtocolParser::ParseProtocol(Oyster::Network::CustomNetProtocol& p)
{
	ProtocolParser::ProtocolData d;
	memset(&d, 0, sizeof(ProtocolData));

	short f = p[0].value.netShort;

	switch (f)
	{
		case protocol_TypeId_General:
			ParseGeneralP(p, f, d);
		break;
		case protocol_TypeId_Lobby:
			ParseLobbyP(p, f, d);
		break;
	}

	return d;
}