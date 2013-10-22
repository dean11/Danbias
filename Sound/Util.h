#ifndef UTIL_H
#define UTIL_H

#include <cstdint>

#define FOURCC(a, b, c, d) ((uint32_t)(((d) << 24) | ((c) << 16) | ((b) << 8) | (a)))

template<class T>
class RefCounted
{
public:
	RefCounted(T value)
	{
		m_value = value;
		m_refCount = 1;
	}
	~RefCounted()
	{
	}
	T GetValue()
	{
		return m_value;
	}
	void IncRefCount()
	{
		m_refCount++;
	}
	int DecRefCount()
	{
		assert(m_refCount > 0);

		return --m_refCount;
	}
private:
	T m_value;
	unsigned int m_refCount;
};

#endif
