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


VOID CPacket::Copy( CHAR* pData, INT nSize )
{
	memcpy( m_Buff, pData, nSize );
//	m_BuffPoint += nSize;
//	m_DataSize += nSize;
	m_DataSize = nSize;
	m_BuffPoint = 0;
}


BOOL CPacket::WriteString( CHAR* pData, WORD wSize )
{
	INT remained = PACKET_SIZE - m_BuffPoint;
	if( wSize > remained )
	{
		return FALSE;
	}

	Write( wSize );
	memcpy( m_Buff + m_BuffPoint, pData, wSize );

	m_BuffPoint += wSize;
	m_DataSize += wSize;

	if( m_BuffPoint > 2 )
	{
		memcpy( m_Buff, &m_DataSize, sizeof( WORD ) );
	}

	return TRUE;
}


BOOL CPacket::ReadString( CHAR* pOut, WORD wMaxSize )
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

	memcpy( pOut, m_Buff + m_BuffPoint, wSize );

	return TRUE;
}

VOID CPacket::CalcSize()
{
	WORD wSize = m_DataSize;
	memcpy( &m_Buff, &wSize, sizeof( WORD ) );
}