#ifndef DANBIASSERVER_GAME_SEESION_MANAGER_H
#define DANBIASSERVER_GAME_SEESION_MANAGER_H

namespace DanBias
{
	class GameSession;
}

class GameSessionManager
{
public:
	static void AddSession(DanBias::GameSession* session);
	static void CloseSession(DanBias::GameSession* session);
};

#endif // !DANBIASSERVER_GAME_SEESION_MANAGER_H
