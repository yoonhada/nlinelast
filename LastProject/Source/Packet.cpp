#include "stdafx.h"

#include "Packet.h"


CPacket::CPacket()
{
	m_BuffPoint = 0;
	m_DataSize = 0;
}


CPacket::~CPacket()
{

}


VOID CPacket::Copy( CHAR* a_pData, INT a_nSize )
{
	memcpy( m_Buff, a_pData, a_nSize );
//	m_BuffPoint += nSize;
//	m_DataSize += nSize;
	m_DataSize = a_nSize;
	m_BuffPoint = 0;
}


BOOL CPacket::WriteString( LPCWSTR a_pData, WORD a_wSize )
{
	INT remained = PACKET_SIZE - m_BuffPoint;
	if( a_wSize > remained )
	{
		return FALSE;
	}

	Write( a_wSize );
	memcpy( m_Buff + m_BuffPoint, a_pData, a_wSize );

	m_BuffPoint += a_wSize;
	m_DataSize += a_wSize;

	if( m_BuffPoint > 2 )
	{
		memcpy( m_Buff, &m_DataSize, sizeof( WORD ) );
	}

	return TRUE;
}


BOOL CPacket::ReadString( LPCWSTR a_pOut, WORD a_wMaxSize )
{
	WORD wSize;
	if( Read( &wSize ) != TRUE )
	{
		return FALSE;
	}

	INT remainedData = m_DataSize - m_BuffPoint;
	if( wSize > remainedData )
	{
		return FALSE;
	}

	memcpy( a_pOut, m_Buff + m_BuffPoint, wSize );

	return TRUE;
}

VOID CPacket::CalcSize()
{
	WORD wSize = m_DataSize;
	memcpy( &m_Buff, &wSize, sizeof( WORD ) );
}