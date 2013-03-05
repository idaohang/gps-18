#pragma once

#include <iostream>

template <typename T>
class Singleton
{
public:
	static T        *getInstance()
	{
		if (_singleton == nullptr)
			allocSingleton();
		return (static_cast<T*>(_singleton));
	}

	static T        &get()
	{
		if (_singleton == nullptr)
			allocSingleton();
		return *(static_cast<T*>(_singleton));
	}

	static void     kill()
	{
		if (_singleton != nullptr)
		{
			delete _singleton;
			_singleton = nullptr;
		}
	}

	static void		setPointer(T &pointer)
	{
		kill();
		_singleton = &pointer;
	}

    protected:
	Singleton()
	{}

	virtual ~Singleton()
	{
		_singleton = nullptr;
	}

private:

	static void				allocSingleton()
	{
		if (_singleton == nullptr)
			_singleton = new T;
	}

	static T    			*_singleton;
};

template <typename T> T				*Singleton<T>::_singleton = nullptr;
