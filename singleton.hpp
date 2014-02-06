#ifndef __SINGLETON_HPP_20130626__
#define __SINGLETON_HPP_20130626__

#include <stdlib.h>

template <class T>
class Singleton
{
public:
	~Singleton() {
		DestroyInstance();
	}

	static T* Instance() {
		if (!m_pInstance) {
			m_pInstance = new T;
		}
		return m_pInstance;
	}

	void DestroyInstance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

protected:
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton& operator = (const Singleton& t) {}

private:
	static T* m_pInstance;
};

template <class T>
T* Singleton<T>::m_pInstance = NULL;

#endif