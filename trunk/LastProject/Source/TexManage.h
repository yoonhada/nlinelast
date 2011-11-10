/**
@file	CQube.h
@date	2011/09/19
@author	
@brief	Manage �������̽�
*/

#ifndef _ITexManage_H_
#define _ITexManage_H_

#include <map>

template <class T>
class ITexManage
{
	//friend class CSingleton< IManage<T> >;
public:
	ITexManage( void ) : m_nLastID(0)	{ }
	virtual ~ITexManage( void )		{ m_pItem.erase( m_pItem.begin(), m_pItem.end() ); }

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

	// ����(X) ID��ȣ(XXXX)

protected:
	INT m_nLastID;
	std::map<int, T> m_pItem;
};

#endif