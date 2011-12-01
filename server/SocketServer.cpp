#include "stdafx.h"

#include "SocketServer.h"


CSocketServer::CSocketServer()
{
	m_ListenSocket		= INVALID_SOCKET;

	m_hCompletionPort	= INVALID_HANDLE_VALUE;
	m_hAcceptThread		= INVALID_HANDLE_VALUE;
	m_hIOCPThread		= INVALID_HANDLE_VALUE;

	Clear();
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


VOID CSocketServer::Clear()
{
	m_bHostClient = FALSE;
	m_pHostClient = NULL;

	m_iClientCount = 0;
	m_iReadyCount = 0;

	for( WORD i=0; i<4; ++i )
	{
		m_wUserNumber[i] = TRUE;

		m_bSelected[i] = FALSE;
	}
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

	// ���� �ν� ��ȣ 0 ~ 3�� ���� ������ ��ȣ�� ã�´�.
	WORD find = 0;
	for( INT i=0; i<4; ++i )
	{
		if( m_wUserNumber[i] == TRUE )
		{
			find = i;
			m_wUserNumber[i] = FALSE;

			break;
		}
	}
	pClient->m_Index = find;

	// IOCP�� ����
	if( CreateIoCompletionPort( (HANDLE)hSocket, m_hCompletionPort, (ULONG_PTR)pClient, 0 ) == NULL )
	{
		cout << "CreateIoCompletionPort() Error : " << WSAGetLastError() << endl;
		SAFE_DELETE( pClient );
		closesocket( hSocket );

		return;
	}

	// ���� ��� ���·� �����.
	pClient->PostRecv();
}


VOID CSocketServer::OnClientClose( CNTClient* pClient )
{
	// ���� ������ ������ȣ�� �ٽ� Ȱ��ȭ ���·�
	m_wUserNumber[pClient->m_Index] = FALSE;

	// �ٸ� �����ڵ鿡�� �ش� ������ ���� ������ �˸���.
	SC_Disconnect( pClient );

	m_Map_LogonClients.erase( pClient->m_Index );

	// Ŭ���̾�Ʈ �� -1
	--m_iClientCount;
	if( m_iClientCount == 0 )
	{
		Clear();
	}
	else
	{
		// READY ���¿����� ReadyCount -1
		if( pClient->m_bReady == TRUE )
		{
			--m_iReadyCount;
		}
	}
	
	// ���� �����Ѱ� ȣ��Ʈ�̸� ȣ��Ʈ �缳��
	if( pClient->m_bHost == TRUE )
	{
		m_bHostClient = FALSE;

		INT iSize = m_Map_LogonClients.size();

		if( iSize > 0 )
		{
			CPacket sendPk;
			WORD wMsgSize = 0;
			WORD wMsgID = MSG_CHANGE_HOST;
			sendPk.Write( wMsgSize );
			sendPk.Write( wMsgID );
			sendPk.CalcSize();

			map<WORD, CNTClient*>::iterator it = m_Map_LogonClients.begin();
			it->second->Send( sendPk );

			m_bHostClient = TRUE;
		}
	}

#ifdef _DEBUG_
	cout << "Current User Count : " << m_iClientCount << endl;
#endif

	pClient->Close();
	SAFE_DELETE( pClient );
}


VOID CSocketServer::CS_Logon( CNTClient* pClient, CPacket& pk )
{
	WCHAR szName[256] = { 0, };

	pk.ReadString( szName, 256 );
	wcout << szName << endl;

	// ���� �߰�
	m_Map_LogonClients.insert( map<WORD, CNTClient*>::value_type( pClient->m_Index, pClient ) );
	++m_iClientCount;

#ifdef _DEBUG_
	cout << "User Count : " << m_iClientCount << endl;
#endif

	// ������ �������� �ʱ� ������ �����ش�.
	SC_INITDATA( pClient );

	// �ٸ� �����鿡�� ���� ��Ŷ�� �����.
	CPacket sendpk;
	WORD wMsgSize = 0;
	WORD wMsgType = MSG_NEWUSER;
	sendpk.Write( wMsgSize );
	sendpk.Write( wMsgType );
	sendpk.Write( pClient->m_Index );
	sendpk.CalcSize();

	SendToClient( pClient, sendpk );
}


VOID CSocketServer::SC_INITDATA( CNTClient* pClient )
{
	// ������ �������� �ʱ� ��ǥ�� �����ش�.
	static FLOAT x = -100.0f;
	static FLOAT z = 600.0f;

	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_INITDATA;

	pk.Write( wMsgSize );
	pk.Write( wMsgID );
	pk.Write( m_bHostClient );
	pk.Write( pClient->m_Index );
	pk.Write( m_iClientCount - 1 );

	// ���ӵǾ��ִ� Ŭ���̾�Ʈ���� ��ȣ
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( pClient->m_Index != it->second->m_Index )
		{
			pk.Write( it->second->m_Index );
		}
	}

	// ���� ���õǾ��ִ� ĳ����
	for( INT i=0; i<4; ++i )
	{
		pk.Write( m_bSelected[i] );
	}

	pk.CalcSize();

	pClient->Send( pk );

	// ȣ��Ʈ ���� ����
	if( m_bHostClient == FALSE )
	{
		m_pHostClient = pClient;
		m_bHostClient = TRUE;
		pClient->m_bHost = TRUE;
	}
	else
	{
		pClient->m_bHost = FALSE;
	}
}


VOID CSocketServer::CS_SELECT_CHARACTER( CNTClient* pClient, CPacket& a_pk )
{

}


VOID CSocketServer::CS_READY( CNTClient* pClient, CPacket& a_pk )
{
	WORD wUserNumber;
	WORD wSelect;
	BOOL bSelect;

	a_pk.Read( &wUserNumber );
	a_pk.Read( &wSelect );
	a_pk.Read( &bSelect );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_READY;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wUserNumber );
	sendPk.Write( wSelect );

	// READY ���� ��Ŷ�̸�
	if( bSelect == TRUE )
	{
		// ���� ������ ĳ�����̸�
		if( m_bSelected[wSelect] == FALSE )
		{
			++m_iReadyCount;
			m_bSelected[wSelect] = TRUE;
			bSelect = TRUE;
		}
		else
		{
			bSelect = FALSE;
		}
	}
	// READY ���� ��Ŷ�̸�
	else
	{
		--m_iReadyCount;
		m_bSelected[wSelect] = FALSE;
		bSelect = FALSE;
	}

	sendPk.Write( bSelect );
	sendPk.CalcSize();

	pClient->Send( sendPk );
	SendToClient( pClient, sendPk );

	cout << m_iClientCount << " " << m_iReadyCount << endl;
	
	// ��� READY ���¸� ���� START ��ư Ȱ��ȭ
	if( m_iClientCount == m_iReadyCount )
	{
		cout << "Enable ok" << endl;
		CPacket pk;

		WORD wMsgSize = 0;
		WORD wMsgID = MSG_ENABLE_START;
		BOOL bStart = TRUE;

		pk.Write( wMsgSize );
		pk.Write( wMsgID );
		pk.Write( bStart );
		pk.CalcSize();

		m_pHostClient->Send( pk );
	}
	else
	{
		cout << "Enable no" << endl;
		CPacket pk;

		WORD wMsgSize = 0;
		WORD wMsgID = MSG_ENABLE_START;
		BOOL bStart = FALSE;

		pk.Write( wMsgSize );
		pk.Write( wMsgID );
		pk.Write( bStart );
		pk.CalcSize();

		m_pHostClient->Send( pk );
	}
}


VOID CSocketServer::CS_GAME_START( CNTClient* pClient, CPacket& a_pk )
{
	a_pk.Rewind();

	SendToClient( pClient, a_pk );
}


VOID CSocketServer::CS_Chat( CNTClient* pClient, CPacket& pk )
{
/*
	WCHAR szChat[256] = { 0, };
	
	pk.ReadString( szChat, 256 );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgType = MSG_CHAT;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgType );
	sendPk.WriteString( szChat, lstrlen( szChat ) );
*/
	pk.Rewind();
	SendToClient( pClient, pk ); 
}


VOID CSocketServer::CS_Move( CNTClient* pClient, CPacket& pk )
{
/*
	WORD wClientNumber;
	FLOAT fX, fZ, fAngle;

	pk.Read( &wClientNumber );
	pk.Read( &fX );
	pk.Read( &fZ );
	pk.Read( &fAngle );

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

	SendToClient( pClient, sendPk );
*/
	pk.Rewind();
	pClient->Send( pk );
}


VOID CSocketServer::CS_UTOM_Attack( CNTClient* pClient, CPacket& pk )
{
/*
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
	WORD wMsgID = MSG_UTOM_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( fDirX );
	sendPk.Write( fDirY );
	sendPk.Write( fDirZ );
	sendPk.Write( wTotalParts );

#ifdef _DEBUG_
	cout << "cTotalParts : " << wTotalParts << endl;
#endif

	static WORD wTemp;
	for( WORD i=0; i<wTotalParts; ++i )
	{
		pk.Read( &wDestroyPart );
		pk.Read( &wDestroyCount );
		sendPk.Write( wDestroyPart );
		sendPk.Write( wDestroyCount );

#ifdef _DEBUG_
		cout << "cDestroyPart : " << wDestroyPart << "  wDestroyCount : " << wDestroyCount << endl;
		cout << "Destroy list" << endl;
#endif

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
*/
	pk.Rewind();
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	SendToClient( pClient, pk );

#ifdef _DEBUG_
	cout << "UTOM Attack send" << endl;
#endif
}


VOID CSocketServer::CS_Attack_Animation( CNTClient* pClient, CPacket& pk )
{
/*
	WORD wClientNumber;
	WORD wAnimationNumber;

	pk.Read( &wClientNumber );
	pk.Read( &wAnimationNumber );

	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_UTOM_ATTACK_ANIMATION;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( wClientNumber );
	sendPk.Write( wAnimationNumber );

#ifdef _DEBUG_
	cout << "Attack Ani "<< wClientNumber << "/" << wAnimationNumber << endl;
#endif

	sendPk.CalcSize();
*/
	pk.Rewind();
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	SendToClient( pClient, pk );
	cout << "Attack Ani send" << endl;
}


VOID CSocketServer::CS_MTOU_Attack( CNTClient* pClient, CPacket& pk )
{
	pk.Rewind();
	
	SendToClient( pClient, pk );
	cout << "MTOU Attack send" << endl;
}


VOID CSocketServer::SendToClient( CNTClient* a_pClient, CPacket& a_pk )
{
	// Ŭ���̾�Ʈ�鿡�� ��Ŷ�� ������.
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( it->second->m_Index != a_pClient->m_Index )
		{
			it->second->Send( a_pk );
		}
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
	case MSG_LOGON:
		CS_Logon( pClient, pk );
		break;

	// ĳ���� ����
	case MSG_READY:
		CS_READY( pClient, pk );
		break;

	// ä��
	case MSG_CHAT:
		CS_Chat( pClient, pk );
		break;

	// �̵�
	case MSG_MOVE:
		CS_Move( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_UTOM_ATTACK:
		CS_UTOM_Attack( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_ATTACK_ANIMATION:
		CS_Attack_Animation( pClient, pk );
		break;

	// ���� : ���� -> ����
	case MSG_MTOU_ATTACK:
		CS_MTOU_Attack( pClient, pk );
		break;
	}
}


VOID CSocketServer::SC_Disconnect( CNTClient* pClient )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_DISCONNECT;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( pClient->m_Index );
	sendPk.CalcSize();

#ifdef _DEBUG_
	cout << "Disconnect : " << pClient->m_Index << endl;
#endif

	SendToClient( pClient, sendPk );
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
			while( DataSize >= 2/*HEADER_SIZE*/ )
			{
				// ������� ũ�������� ���´�.
				WORD wMsgSize;
				memcpy( &wMsgSize, pClient->m_RecvBuffer.m_Buffer, sizeof( WORD ) );

				if( wMsgSize <= DataSize )
				{
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