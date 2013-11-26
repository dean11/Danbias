/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "OysterMutex.h"

#include <chrono>
#include <thread>
#include <future>



OysterMutex::OysterMutex()
{}

OysterMutex::OysterMutex(bool initialOwnership)
{
	if(initialOwnership)
	{
		this->mutex.lock();
		this->id = std::this_thread::get_id();
	}
}
OysterMutex::~OysterMutex()
{
}
void OysterMutex::LockMutex()
{
	if(std::this_thread::get_id() == this->id)	return;
	
	this->mutex.lock();
	
	this->id = std::this_thread::get_id();
}
void OysterMutex::LockMutex(unsigned int msec)
{
	if(std::this_thread::get_id() == this->id)	return;

	auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(msec);
  
	do 
	{
		if(this->mutex.try_lock())
		{
			this->mutex.lock();
			this->id = std::this_thread::get_id();
			return;
		}
	} while (std::chrono::high_resolution_clock::now() < end);

	this->mutex.lock();
}
void OysterMutex::UnlockMutex()
{
	//Let the owner unlock
	if(std::this_thread::get_id() != this->id)	return;
	
	this->mutex.unlock();
	this->id = std::thread::id();
	
}
bool OysterMutex::IsTaken()
{
	return !this->mutex.try_lock();
}
void OysterMutex::Reset()
{
	if(!this->mutex.try_lock())
		this->mutex.unlock();
}