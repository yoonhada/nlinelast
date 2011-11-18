#include "stdafx.h"

#include "SocketServer.h"


CSocketServer::CSocketServer()
{
	m_ListenSocket		= INVALID_SOCKET;

	m_hCompletionPort	= INVALID_HANDLE_VALUE;
	m_hAcceptThread		= INVALID_HANDLE_VALUE;
	m_hIOCPThread		= INVALID_HANDLE_VALUE;

	for( WORD i=0; i<4; ++i )
	{
		m_wIndexList[i][0] = TRUE;
		m_wIndexList[i][1] = i;
	}
	m_wIndex = 0;
}


CSocketServer::~CSocketServer()
{
	Stop();

	// IOCP �ݱ�
	if( m_hCompletionPort != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hCompletionPort );
	}

	// Accept ������ ����
	if( m_hAcceptThread != INVALID_HANDLE_VALUE )
	{
		WaitForSingleObject( m_hAcceptThread, INFINITE );
	}

	// IOCP ������ ����
	if( m_hIOCPThread != INVALID_HANDLE_VALUE )
	{
		WaitForSingleObject( m_hIOCPThread, INFINITE );
	}

	WSACleanup();
}


BOOL CSocketServer::Initialize()
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	// IOCP ����
	m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hCompletionPort == NULL )
	{
		cout << "CreateIoCompletionPort() failed" << endl;
		return FALSE;
	}

	// Worker ������ ����
	UINT dwThreadID;
	m_hIOCPThread = (HANDLE)_beginthreadex( NULL, 0, IOCPWorkerProc, this, 0, &dwThreadID );

	return TRUE;
}


BOOL CSocketServer::StartServer( WORD Port )
{
	m_ListenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( m_ListenSocket == INVALID_SOCKET )
	{
		cout << "WSASocket() error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	SOCKADDR_IN addr;
	memset( &addr, 0, sizeof( SOCKADDR_IN ) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( Port );

	INT r = bind( m_ListenSocket, (SOCKADDR*)&addr, sizeof( SOCKADDR_IN ) );
	if( r != 0 )
	{
		cout << "bind() error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	r = listen( m_ListenSocket, SOMAXCONN );
	if( r != 0 )
	{
		cout << "listen() error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	UINT dwThreadID;
	m_hAcceptThread = (HANDLE)_beginthreadex( NULL, 0, AcceptProc, this, 0, &dwThreadID );

	cout << "Started Server!" << endl;

	return TRUE;
}


VOID CSocketServer::Stop()
{
	if( m_ListenSocket != INVALID_SOCKET )
	{
		closesocket( m_ListenSocket );
	}

	if( m_hIOCPThread != INVALID_HANDLE_VALUE )
	{
		PostQueuedCompletionStatus( m_hIOCPThread, 0, NULL, NULL );
	}
}


VOID CSocketServer::UpdateFrame()
{
	
}


VOID CSocketServer::OnAccept( SOCKET hSocket )
{
	CNTClient* pClient = new CNTClient;
	pClient->Initialize();
	pClient->m_Socket = hSocket;
	
	// ���� �ν� ��ȣ 0 ~ 3
	WORD find = 0;
	for( INT i=0; i<4; ++i )
	{
		if( m_wIndexList[i][0] == TRUE )
		{
			find = i;
			m_wIndexList[i][0] = FALSE;

			break;
		}
	}
	pClient->m_Index = find;

	if( CreateIoCompletionPort( (HANDLE)hSocket, m_hCompletionPort, (ULONG_PTR)pClient, 0 ) == NULL )
	{
		cout << "CreateIoCompletionPort() Error : " << WSAGetLastError() << endl;
		SAFE_DELETE( pClient );
		closesocket( hSocket );

		return;
	}

	pClient->PostRecv();
}


VOID CSocketServer::CS_Logon( CNTClient* pClient, CPacket& pk )
{
	CHAR szName[128] = { 0, };
	CHAR szPass[128] = { 0, };

	pk.ReadString( szName, 128 );
	pk.ReadString( szPass, 128 );

	cout << szName << " " << szPass << endl;

	// ���� �߰�
	m_Map_LogonClients.insert( map<WORD, CNTClient*>::value_type( pClient->m_Index, pClient ) );
	cout << "User Count : " << m_Map_LogonClients.size() << endl;

	// ������ �������� �ʱ� ������ �����ش�.
	SC_INITDATA( pClient );

	// �ٸ� �����鿡�� ���� ������ ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}

		SC_NEWUSER( it->second, pClient );
	}

}


VOID CSocketServer::SC_INITDATA( CNTClient* pClient )
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_INITDATA;

	// ������ �������� �ʱ� ��ǥ�� �����ش�.
	static FLOAT x = 200.0f;
	static FLOAT z = 0.0f;

	// ���� ������ ������ ����  
	static bool host = TRUE;

	// �������� ������ ��ȣ
	static WORD user_no = 0;

	// �ڽ��� �� ������ ���� ��
	WORD userCount = m_Map_LogonClients.size() - 1;

	pk.Write( wMsgSize );
	pk.Write( wMsgID );
	pk.Write( host );
	pk.Write( pClient->m_Index );
	pk.Write( userCount );

	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( pClient->m_Index == it->second->m_Index )
		{
			continue;
		}

		pk.Write( it->second->m_Index );
	}

	// ��Ŷ ũ�� ���
	pk.CalcSize();

	pClient->Send( pk );

	x += 50.0f;
	if( x >= 400.0f )
	{
		x = 200.0f;
	}

	if( host )
	{
		host = FALSE;
	}

	++user_no;
}


VOID CSocketServer::SC_NEWUSER( CNTClient* pClient, CNTClient* pOtherClientInfo )
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgType = MSG_SC_NEWUSER;
	pk.Write( wMsgSize );
	pk.Write( wMsgType );
	pk.Write( pOtherClientInfo->m_Index );
	pk.CalcSize();
	
	pClient->Send( pk );
}


VOID CSocketServer::CS_Chat( CNTClient* pClient, CPacket& pk )
{
	CHAR szChat[128] = { 0, };

	pk.ReadString( szChat, 128 );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgType = MSG_SC_CHAT;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgType );
	sendPk.WriteString( szChat, strlen( szChat ) );

	SC_Chat( pClient, sendPk ); 
}


VOID CSocketServer::SC_Chat( CNTClient* pClient, CPacket& pk )
{
	// �ٸ� �����鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}

		it->second->Send( pk );
	}
}


VOID CSocketServer::CS_Move( CNTClient* pClient, CPacket& pk )
{
	WORD wClientNumber;
	FLOAT fX, fZ, fAngle;

	pk.Read( &wClientNumber );
	pk.Read( &fX );
	pk.Read( &fZ );
	pk.Read( &fAngle );

	//cout << wClientNumber << " : " << fX << " " << fZ << " " << fAngle << endl;

	// ���� ��Ŷ�� ���� �����Ѵ�.
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_MOVE;
	
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( fX );
	sendPk.Write( fZ );
	sendPk.Write( fAngle );
	sendPk.CalcSize();

	// �ٸ� �����鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( pClient->m_Index == it->second->m_Index )
		{
			continue;
		}
		
		SC_Move( it->second, sendPk );
		//cout << "send to : " << it->second->m_Index << endl;
	}
}


VOID CSocketServer::SC_Move( CNTClient* pClient, CPacket& pk )
{
	pClient->Send( pk );
}


VOID CSocketServer::CS_UTOM_Attack( CNTClient* pClient, CPacket& pk )
{
	WORD wClientNumber;
	FLOAT fDirX, fDirY, fDirZ;
	WORD wTotalParts = 0;
	WORD wDestroyPart = 0;
	WORD wDestroyCount = 0;
	WORD wList = 0;

	pk.Read( &wClientNumber );
	pk.Read( &fDirX );
	pk.Read( &fDirY );
	pk.Read( &fDirZ );
	pk.Read( &wTotalParts );
	
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_UTOM_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( fDirX );
	sendPk.Write( fDirY );
	sendPk.Write( fDirZ );
	sendPk.Write( wTotalParts );

	cout << "cTotalParts : " << wTotalParts << endl;

	static WORD wTemp;
	for( WORD i=0; i<wTotalParts; ++i )
	{
		pk.Read( &wDestroyPart );
		pk.Read( &wDestroyCount );
		sendPk.Write( wDestroyPart );
		sendPk.Write( wDestroyCount );

		cout << "cDestroyPart : " << wDestroyPart << "  wDestroyCount : " << wDestroyCount << endl;
		cout << "Destroy list" << endl;

		for( WORD j=0; j<wDestroyCount; ++j )
		{
			pk.Read( &wList );
			sendPk.Write( wList );
			cout << wList << " ";
			wTemp = wList;
		}
		cout << endl;
	}

	cout << endl;

	sendPk.CalcSize();

	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	SC_UTOM_Attack( pClient, sendPk );
}


VOID CSocketServer::SC_UTOM_Attack( CNTClient* pClient, CPacket& pk )
{

	cout << "Size : " << m_Map_LogonClients.size() << endl;
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}
		
		it->second->Send( pk );
		cout << "UTOM Attack send to : " << it->second->m_Index << endl;
	}
}


VOID CSocketServer::CS_MTOU_Attack( CNTClient* pClient, CPacket& pk )
{
	WORD wClientNumber;
	FLOAT fDirX, fDirY, fDirZ;
	CHAR cDestroyPart;
	WORD wDestroyCount;
	WORD wList;

	pk.Read( &wClientNumber );
	pk.Read( &fDirX );
	pk.Read( &fDirY );
	pk.Read( &fDirZ );
	pk.Read( &cDestroyPart );
	pk.Read( &wDestroyCount );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_MTOU_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( fDirX );
	sendPk.Write( fDirY );
	sendPk.Write( fDirZ );
	sendPk.Write( cDestroyPart );
	sendPk.Write( wDestroyCount );

	for( WORD i=0; i<wDestroyCount; ++i )
	{
		pk.Read( &wList );
		sendPk.Write( wList );
		//cout << "Destroy list : " << wList << endl;
	}

	//cout << "wDestroyCount : " << static_cast<INT>(cDestroyPart) << ":" << wDestroyCount << endl;

	sendPk.CalcSize();

	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	SC_MTOU_Attack( pClient, sendPk );
}


VOID CSocketServer::SC_MTOU_Attack( CNTClient* pClient, CPacket& pk )
{
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}

		it->second->Send( pk );
		//cout << "send to : " << it->second->m_Index << endl;
	}
}


VOID CSocketServer::CS_UTOM_Attack_Animation( CNTClient* pClient, CPacket& pk )
{
	WORD wClientNumber;
	WORD wAnimationNumber;

	pk.Read( &wClientNumber );
	pk.Read( &wAnimationNumber );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_UTOM_ATTACK_ANIMATION;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( wAnimationNumber );

	cout << "Attack Ani "<< wClientNumber << "/" << wAnimationNumber << endl;

	sendPk.CalcSize();

	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	SC_UTOM_Attack_Animation( pClient, sendPk );
}


VOID CSocketServer::SC_UTOM_Attack_Animation( CNTClient* pClient, CPacket& pk )
{
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}
		
		it->second->Send( pk );
		cout << "Attack Ani send to : " << it->second->m_Index << endl;
	}
}


VOID CSocketServer::ProcessPacket( CNTClient* pClient, CPacket& pk )
{
	WORD wMsgSize, wMsgType;
	pk.Read( &wMsgSize );
	pk.Read( &wMsgType );

	switch( wMsgType )
	{
	// �α׿�
	case MSG_CS_LOGON:
		CS_Logon( pClient, pk );
		break;

	// ä��
	case MSG_CS_CHAT:
		CS_Chat( pClient, pk );
		break;

	// �̵�
	case MSG_CS_MOVE:
		CS_Move( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_CS_UTOM_ATTACK:
		CS_UTOM_Attack( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_CS_UTOM_ATTACK_ANIMATION:
		CS_UTOM_Attack_Animation( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_CS_MTOU_ATTACK:
//		CS_MTOU_Attack( pClient, pk );
		break;
	}
}


VOID CSocketServer::SC_Disconnect( CNTClient* pClient )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_SC_DISCONNECT;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( pClient->m_Index );

	sendPk.CalcSize();

	cout << "Disconnect : " << pClient->m_Index << endl;

	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index == pClient->m_Index )
		{
			continue;
		}
		
		it->second->Send( sendPk );
	}
}


VOID CSocketServer::OnClientClose( CNTClient* pClient )
{
	// ���� ������ ������ȣ�� �ٽ� Ȱ��ȭ ���·�
	m_wIndexList[pClient->m_Index][0] = TRUE;

	// �ٸ� �����ڵ鿡�� �ش� ������ ���� ������ �˸���.
	SC_Disconnect( pClient );

	m_Map_LogonClients.erase( pClient->m_Index );
	cout << "Current User Count : " << m_Map_LogonClients.size() << endl;

	pClient->Close();
	SAFE_DELETE( pClient );
}


UINT WINAPI CSocketServer::AcceptProc( VOID* p )
{
	CSocketServer* pServer = (CSocketServer*)p;

	while( 1 )
	{
		SOCKET hSocket = accept( pServer->m_ListenSocket, NULL, NULL );

		if( hSocket == INVALID_SOCKET )
		{
			INT err = WSAGetLastError();
			if( err == WSAEINTR )
			{
				break;
			}

			cout << "accept() Error : << " << err << endl;
		}

		pServer->OnAccept( hSocket );
	}

	return 0;
}


UINT WINAPI CSocketServer::IOCPWorkerProc( VOID* p )
{
	CSocketServer* pServer = (CSocketServer*)p;
	DWORD dwTransferred = 0;
	CNTClient* pClient = NULL;
	LPOVERLAPPED pOverlapped = NULL;

	while( 1 )
	{
		BOOL bSuccess = GetQueuedCompletionStatus( pServer->m_hCompletionPort, &dwTransferred, (LPDWORD)&pClient, &pOverlapped, INFINITE );

		OVERLAPPED_PLUS* ov = (OVERLAPPED_PLUS*)pOverlapped;

		if( dwTransferred == 0 )
		{		
			// ���� ����
			if( pClient == NULL )
			{
				// �������� ��� Ŭ���̾�Ʈ�� �����Ѵ�.
				map<WORD, CNTClient*>::iterator it;
				for( it = pServer->m_Map_LogonClients.begin(); it != pServer->m_Map_LogonClients.end(); ++it )
				{
					pServer->OnClientClose( it->second );
				}
				return 0;
			}
			
			// Ŭ���̾�Ʈ ���� ����
			pServer->OnClientClose( pClient );
			continue;
		}

		if( ov->m_Mode == RECV_POSTED )
		{
			// ����
			pClient->m_RecvBuffer.WriteCommit( dwTransferred );

			// ���ŵ����Ͱ� �ż����� �ϼ��Ǿ����� Ȯ���ϰ� ó��
			UINT DataSize = pClient->m_RecvBuffer.GetStoredSize();
			while( DataSize >= HEADER_SIZE )
			{
				// ������� ũ�������� ���´�.
				WORD wMsgSize;
				memcpy( &wMsgSize, pClient->m_RecvBuffer.m_Buffer, sizeof( WORD ) );

				if( wMsgSize <= DataSize )
				{
					cout << "wMsgSize : " << wMsgSize << endl;

					// ��Ŷ���ſϷ�, ��Ŷ�� �����ؼ� ó���Լ��� �ѱ��.
					CPacket pk;
					pk.Copy( pClient->m_RecvBuffer.m_Buffer, wMsgSize );
					pClient->m_RecvBuffer.ReadCommit( wMsgSize );

					// �ϼ��� ��Ŷ�� ó���Ѵ�.
					pServer->ProcessPacket( pClient, pk );
				}

				DataSize = pClient->m_RecvBuffer.GetStoredSize();
			}

			// ������ ����Ѵ�.
			if( pClient->PostRecv() != TRUE )
			{
				pServer->OnClientClose( pClient );
			}
		}
		else
		{
			// �۽�
			if( pClient->SendComplete( dwTransferred ) != TRUE )
			{
				pServer->OnClientClose( pClient );
			}
		}
	}

	cout << "IOCPWorkerProc Terminated" << endl;

	return 0;
}