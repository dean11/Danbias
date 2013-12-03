#include <Windows.h>
#include "Include\DanBiasServer.h"

namespace DanBias
{
#pragma region Server Data
	class DanBiasServerPrivateData
	{
	public:
		DanBiasServerPrivateData()
		{

		}
		~DanBiasServerPrivateData()
		{

		}

	private:


	} data;
#pragma endregion


	DanBiasServerReturn DanBiasServer::Initiate(DanBiasServerDesc& desc)
	{
		return DanBiasServerReturn_Sucess;
	}
	DanBiasServerReturn DanBiasServer::Run()
	{
		return DanBiasServerReturn_Sucess;
	}
	void DanBiasServer::Release()
	{

	}

} //End namspace DanBias