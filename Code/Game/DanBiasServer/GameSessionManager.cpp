#include "GameSessionManager.h"

#include "ServerObjects\GameSession.h"
#include "DynamicArray.h"

struct GameSessionData
{
	Utility::DynamicMemory::DynamicArray<DanBias::GameSession*> sessions;
	int freeSpot;

	int Existst(DanBias::GameSession* s)
	{
		for (unsigned int i = 0; i < sessions.Size(); i++)
		{
			if(!sessions[i] && freeSpot == -1)		freeSpot = i;
			if(sessions[i] == s)					return i;
		}
		return -1;
	}
	int GetFree()
	{
		for (unsigned int i = 0; i < sessions.Size(); i++)
			if(!sessions[i]) 
				return i;
		return -1;
	}

} __gameSessionData;


void GameSessionManager::AddSession(DanBias::GameSession* session)
{
	if(__gameSessionData.Existst(session) == -1)
	{
		int k = __gameSessionData.freeSpot;

		if( k == -1) k = __gameSessionData.GetFree();

		if(k == -1) __gameSessionData.sessions.Push(session);
		else		__gameSessionData.sessions[k] = session;
	}

}
void GameSessionManager::CloseSession(DanBias::GameSession* session)
{
	int i = __gameSessionData.Existst(session);

	//Moron check...
	if(i == -1) return;

	//__gameSessionData.sessions[i]->Close();
}





