/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_OYSTER_MUTEX_H
#define MISC_OYSTER_MUTEX_H

#include <mutex>
#include <thread>
#include <atomic>

class OysterMutex
{
public:
	OysterMutex();
	OysterMutex(bool initialOwnership);
	virtual~OysterMutex();
	void LockMutex();
	void LockMutex(unsigned int timeSpan);
	void UnlockMutex();
	/** Returns true if mutex is taken */
	bool IsTaken();

private:
	std::mutex mutex;
	std::thread::id id;

	OysterMutex(const OysterMutex&);
};

#endif // !MISC_OYSTER_MUTEX_H
