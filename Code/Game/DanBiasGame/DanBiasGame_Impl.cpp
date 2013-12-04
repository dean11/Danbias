#include <Windows.h>
#include "Include\DanBiasGame.h"

namespace DanBias
{
#pragma region Game Data
	/**
	*	Private server data container
	*/
	class DanBiasGamePrivateData
	{
	public:
		bool initiated;
		bool running;
		bool released;

	public:
		DanBiasGamePrivateData()
			:running(0)
			, initiated(0)
		{}
		void Release()
		{

		}
	} data;
#pragma endregion


	DanBiasClientReturn DanBiasGame::Initiate(DanBiasGameDesc& desc)
	{

		data.initiated = true;
		return DanBiasClientReturn_Sucess;
	}
	DanBiasClientReturn DanBiasGame::Run()
	{
		if(data.running)
		{
			return DanBiasClientReturn_Error;
		}
		if(!data.released)
		{
			return DanBiasClientReturn_Error;
		}
		if(!data.initiated)
		{
			return DanBiasClientReturn_Error;
		}
		data.running = true;
		while (data.running)
		{

		}
		return DanBiasClientReturn_Sucess;
	}
	void DanBiasGame::Release()
	{
		data.released = true;
	}

} //End namspace DanBias