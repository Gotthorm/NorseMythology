// SINGLETON.H

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <assert.h>

template<typename T>
class Singleton
{
public:
	static bool Create();
	static void Destroy();

	static T* GetInstance() { assert( m_Instance ); return m_Instance; }

private:
	Singleton( Singleton const& ) = delete;
	Singleton& operator=( Singleton const& ) = delete;

protected:
	Singleton() { m_Instance = static_cast<T*>( this ); };
	virtual ~Singleton() = default;

	static T* m_Instance;
};

template<typename T>
typename T* Singleton<T>::m_Instance = nullptr;

template<typename T>
bool Singleton<T>::Create()
{
	if( m_Instance == 0 )
	{
		Singleton<T>::m_Instance = new T();

		return true;
	}

	return false;
}

template<typename T>
void Singleton<T>::Destroy()
{
	delete Singleton<T>::m_Instance;

	Singleton<T>::m_Instance = nullptr;
}

#endif // _SINGLETON_H_

