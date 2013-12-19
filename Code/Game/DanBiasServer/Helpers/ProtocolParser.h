#ifndef DANBIASSERVER_PROTOCOLPARSER_H
#define DANBIASSERVER_PROTOCOLPARSER_H

#include <GameProtocols.h>
namespace DanBias
{
	class ProtocolParser
	{
	public:
		union ProtocolCollection
		{
			struct{ GameLogic::Protocol_LobbyCreateGame createGame; };
			struct{ GameLogic::Protocol_LobbyJoinGame joinGame;		};
			struct{ GameLogic::Protocol_LobbyStartGame startGame;	};
			struct{ GameLogic::Protocol_LobbyJoinLobby joinLobby;	};
			struct{ GameLogic::Protocol_LobbyLeaveLobby leaveLobby; };
		};
		struct ProtocolData
		{
			short type;
			ProtocolCollection p;
		};
	public:
		static ProtocolData ParseProtocol(Oyster::Network::CustomNetProtocol& p);
	};
}
#endif // !DANBIASSERVER_PROTOCOLPARSER_H
