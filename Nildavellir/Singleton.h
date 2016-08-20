// SINGLETON.H

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<typename T>
class Singleton
{
public:
	static T* GetInstance();
	static void Destroy();

private:
	Singleton( Singleton const& ) {};
	Singleton& operator=( Singleton const& ) {};

protected:
	Singleton() { m_Instance = static_cast<T*>( this ); };
	~Singleton() {};

	static T* m_Instance;
};

template<typename T>
typename T* Singleton<T>::m_Instance = 0;

template<typename T>
T* Singleton<T>::GetInstance()
{
	if( m_Instance == 0 )
	{
		Singleton<T>::m_Instance = new T();
	}

	return m_Instance;
}

template<typename T>
void Singleton<T>::Destroy()
{
	delete Singleton<T>::m_Instance;
	Singleton<T>::m_Instance = 0;
}

#endif // _SINGLETON_H_

