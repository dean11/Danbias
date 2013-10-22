#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

class ThreadPool
{
public:
	ThreadPool();
	virtual ~ThreadPool();

	void EnqueueTask(const std::function<void()> &task);

private:
	PTP_POOL m_pool;
	TP_CALLBACK_ENVIRON m_environment;
};

class Mutex
{
public:
	Mutex();
	virtual ~Mutex();

	void Lock();
	void Unlock();

private:
	HANDLE m_mutex;
};

#endif
