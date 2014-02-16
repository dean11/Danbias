/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef NETWORK_API_SERVEREVENT_H
#define NETWORK_API_SERVEREVENT_H

namespace Oyster
{
	namespace Network
	{
		template <typename Sender, typename Args>
		struct NetEvent
		{
			Sender sender;
			Args args;
		};
	}
}

#endif // !SERVERDEPENDENCIES_SERVEREVENT_H
