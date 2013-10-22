#include "ThreadPool.h"

static VOID CALLBACK TaskCallback(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work)
{
	std::function<void()> *task = (std::function<void()> *)parameter;

	(*task)();

	delete task;
}

ThreadPool::ThreadPool()
{
	m_pool = CreateThreadpool(NULL);
	SetThreadpoolThreadMinimum(m_pool, 1);
	// FIXME: Don't hardcode this.
	SetThreadpoolThreadMaximum(m_pool, 4);

	InitializeThreadpoolEnvironment(&m_environment);
	SetThreadpoolCallbackPool(&m_environment, m_pool);
}

ThreadPool::~ThreadPool()
{
	DestroyThreadpoolEnvironment(&m_environment);
	CloseThreadpool(m_pool);
}

void ThreadPool::EnqueueTask(const std::function<void()> &task)
{
	std::function<void()> *data = new std::function<void()>(task);

	PTP_WORK work = CreateThreadpoolWork(TaskCallback, (void *)data, &m_environment);
	SubmitThreadpoolWork(work);
	CloseThreadpoolWork(work);
}

Mutex::Mutex()
{
	m_mutex = CreateMutex(NULL, false, NULL);
}

Mutex::~Mutex()
{
	CloseHandle(m_mutex);
}

void Mutex::Lock()
{
	WaitForSingleObject(m_mutex, INFINITE);
}

void Mutex::Unlock()
{
	ReleaseMutex(m_mutex);
}
