#include "stdafx.h"

#include "NTClient.h"


CNTClient::CNTClient()
{
	m_Socket = INVALID_SOCKET;
	m_Index	= 0;

	InitializeCriticalSection( &m_cs );
}


CNTClient::~CNTClient()
{
	DeleteCriticalSection( &m_cs );
}


VOID CNTClient::Initialize()
{
	memset( (CHAR*)&m_OV_IN, 0, sizeof( OVERLAPPED_PLUS ) );
	memset( (CHAR*)&m_OV_OUT, 0, sizeof( OVERLAPPED_PLUS ) );

	m_OV_IN.m_pClient = this;
	m_OV_OUT.m_pClient = this;

	m_OV_IN.m_Mode = RECV_POSTED;
	m_OV_OUT.m_Mode = SEND_POSTED;
}


VOID CNTClient::Close()
{
	if( m_Socket != INVALID_SOCKET )
	{
		closesocket( m_Socket );
	}
}


BOOL CNTClient::SendRest()
{
	EnterCriticalSection( &m_cs );

	// 버퍼에 남은 데이터를 전송한다.
	if( m_SendBuffer.GetStoredSize() )
	{
		DWORD dwSent;
		WSABUF wsabuf;
		wsabuf.buf = m_SendBuffer.m_Buffer;
		wsabuf.len = m_SendBuffer.GetStoredSize();

		INT r = WSASend( m_Socket, &wsabuf, 1, &dwSent, 0, (OVERLAPPED*)&m_OV_OUT, NULL );
		if( ( r == SOCKET_ERROR ) && ( WSAGetLastError() != ERROR_IO_PENDING ) )
		{
			cout << "WSASend() Error : " << WSAGetLastError() << endl;

			LeaveCriticalSection( &m_cs );
			return FALSE;
		}
	}

	LeaveCriticalSection( &m_cs );

	return TRUE;
}


BOOL CNTClient::PostRecv()
{
	WSABUF wsabuf;
	DWORD dwReceived, dwFlags = 0;

	if( m_Socket == INVALID_SOCKET )
	{
		return FALSE;
	}

	wsabuf.buf = m_RecvBuffer.GetWritePointer();
	wsabuf.len = m_RecvBuffer.GetEmptySize();

	INT r = WSARecv( m_Socket, &wsabuf, 1, &dwReceived, &dwFlags, (OVERLAPPED*)&m_OV_IN, NULL );
	if( ( r == SOCKET_ERROR ) && ( WSAGetLastError() != ERROR_IO_PENDING ) )
	{
		cout << "WSARecv() Error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	return TRUE;
}


BOOL CNTClient::Send( CPacket& pk )
{
	EnterCriticalSection( &m_cs );

	BOOL bSendNow = TRUE;
	if( m_SendBuffer.GetStoredSize() )
	{
		bSendNow = FALSE;
	}

	DWORD dwPacketSize = pk.GetLength();
	m_SendBuffer.Write( pk.GetBuff(), dwPacketSize );

	if( bSendNow )
	{
		WSABUF wsabuf;
		wsabuf.buf = m_SendBuffer.m_Buffer;
		wsabuf.len = m_SendBuffer.GetStoredSize();

		INT r = WSASend( m_Socket, &wsabuf, 1, &dwPacketSize, 0, (OVERLAPPED*)&m_OV_OUT, NULL );
		if( ( r == SOCKET_ERROR ) && ( WSAGetLastError() != ERROR_IO_PENDING ) )
		{
			cout << "WSASend() Error : " << WSAGetLastError() << endl;

			LeaveCriticalSection( &m_cs );
			return FALSE;
		}
	}

	LeaveCriticalSection( &m_cs );

	return TRUE;
}


BOOL CNTClient::SendComplete( DWORD dwBytes )
{
	EnterCriticalSection( &m_cs );

	m_SendBuffer.ReadCommit( dwBytes );

	// 버퍼에 남은 데이터가 있으면 송신한다.
	if( m_SendBuffer.GetStoredSize() )
	{
		if( SendRest() != TRUE )
		{
			LeaveCriticalSection( &m_cs );
			return FALSE;
		}
	}

	LeaveCriticalSection( &m_cs );

	return TRUE;
}