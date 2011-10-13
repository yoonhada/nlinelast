/**
@file	CQube.h
@date	2011/09/19
@author	
@brief	Manage 인터페이스
*/

#ifndef _IManage_H_
#define _IManage_H_

#include <map>

template <class T>
class IManage
{
	//friend class CSingleton< IManage<T> >;
public:
	IManage( void ) : m_nLastID(0)	{ }
	virtual ~IManage( void )		{ m_pItem.erase( m_pItem.begin(), m_pItem.end() ); }

	virtual BOOL Create() = 0;
	virtual VOID Release() = 0;

	INT Push(T pItem)		
	{ 
		INT nID = 10000 + (++m_nLastID);
		m_pItem[nID] = pItem;
		return nID;
	}

	T Pop(INT nID)			
	{
		return m_pItem[nID];
	}

	// 버전(X) ID번호(XXXX)

protected:
	INT m_nLastID;
	std::map<int, T> m_pItem;
};

#endif