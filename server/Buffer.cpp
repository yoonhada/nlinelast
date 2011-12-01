#include "stdafx.h"

#include "Buffer.h"


CBuffer::CBuffer()
{
	m_End = 0;
}


CBuffer::~CBuffer()
{

}


BOOL CBuffer::Write( CHAR* pData, UINT nSize )
{
	UINT remained = GetEmptySize();
	if( remained < nSize )
	{
		cout << "Buffer Overflow" << endl;
		return FALSE;
	}

	memcpy( m_Buffer + m_End, pData, nSize );

	WriteCommit( nSize );

	return TRUE;
}


VOID CBuffer::WriteCommit( UINT nSize )
{
	m_End += nSize;
}


BOOL CBuffer::ReadCommit( UINT nSize )
{
	if( m_End < nSize )
	{
		cout << "Buffer Underflow" << endl;
		return FALSE;
	}

	// 데이터를 앞으로 당긴다.
	m_End -= nSize;
	memmove( m_Buffer, m_Buffer + nSize, m_End );
	

	return TRUE;
}