#include "stdafx.h"

#include "Buffer.h"


CBuffer::CBuffer()
{
	m_End = 0;
}


CBuffer::~CBuffer()
{

}


BOOL CBuffer::Write( CHAR* pData, WORD nSize )
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


VOID CBuffer::WriteCommit( WORD nSize )
{
	m_End += nSize;
}


BOOL CBuffer::ReadCommit( WORD nSize )
{
	if( m_End < nSize )
	{
		cout << "Buffer Underflow" << endl;
		return FALSE;
	}

	// 데이터를 앞으로 당긴다.
	memmove( m_Buffer, m_Buffer + nSize, nSize );
	m_End -= nSize;

	return TRUE;
}