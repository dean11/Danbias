/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "DBServer.h"
//#include "GameLogic\?"
#include "Utilities.h"

using namespace DanBias;

DBServer::DBServer()
	:	initiated(0)
	,	running(0)
	,	released(0)
{
}
DBServer::~DBServer()
{

}
DanBiasServerReturn DBServer::Create(const DanBias::DanBiasServerDesc& desc)
{
	this->initiated = true;
	return DanBiasServerReturn_Sucess;
}
DanBiasServerReturn DBServer::Run()
{
	
	if(this->running)
	{
		return DanBiasServerReturn_Error;
	}
	if(this->released)
	{
		return DanBiasServerReturn_Error;
	}
	if(!this->initiated)
	{
		return DanBiasServerReturn_Error;
	}
	this->running = true;
	while (this->running)
	{
		MessageBox(0, L"What to do here...", L"TYPELESS", 0);

		this->running = false;
	}
	return DanBiasServerReturn_Sucess;
}
DanBiasServerReturn DBServer::Release()
{
	this->released = true;
	return DanBiasServerReturn_Sucess;
}


