/**
	@file	Singleton.h
	@date	2011/09/17
	@author	»≤¿±«œ
	@brief	ΩÃ≈¨≈Ê ≈¨∑°Ω∫
*/

#ifndef _Singleton_H_
#define _Singleton_H_

/**
	@class	CSingleton
	@date	2011/09/17
	@author	»≤¿±«œ
	@brief	ΩÃ≈¨≈Ê ≈¨∑°Ω∫
*/

template <class T>
class CSingleton
{
private:
	static T * _instance;

protected:
	CSingleton()			{};
	virtual ~CSingleton()	{};

public:
	static T * GetInstance()
	{
		if ( NULL == _instance )
		{
			_instance = new T;
		}

		return ( static_cast<T*>( _instance ) );
	}

	static void DestoryInstance()
	{
		if ( NULL != _instance )
		{
			delete _instance;
			_instance = NULL;
		}
	}
};

template <class T>
T * CSingleton<T>::_instance = NULL;

#endif