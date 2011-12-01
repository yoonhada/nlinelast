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

	// IOCP 닫기
	if( m_hCompletionPort != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hCompletionPort );
	}

	// Accept 쓰레드 종료
	if( m_hAcceptThread != INVALID_HANDLE_VALUE )
	{
		WaitForSingleObject( m_hAcceptThread, INFINITE );
	}

	// IOCP 쓰레드 종료
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

	// IOCP 생성
	m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hCompletionPort == NULL )
	{
		cout << "CreateIoCompletionPort() failed" << endl;
		return FALSE;
	}

	// Worker 쓰레드 생성
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

	// 유저 인식 번호 0 ~ 3중 가장 가능한 번호를 찾는다.
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

	// IOCP와 연결
	if( CreateIoCompletionPort( (HANDLE)hSocket, m_hCompletionPort, (ULONG_PTR)pClient, 0 ) == NULL )
	{
		cout << "CreateIoCompletionPort() Error : " << WSAGetLastError() << endl;
		SAFE_DELETE( pClient );
		closesocket( hSocket );

		return;
	}

	// 수신 대기 상태로 만든다.
	pClient->PostRecv();
}


VOID CSocketServer::OnClientClose( CNTClient* pClient )
{
	// 접속 해제한 유저번호가 다시 활성화 상태로
	m_wUserNumber[pClient->m_Index] = FALSE;

	// 다른 접속자들에게 해당 유저의 접속 해제를 알린다.
	SC_Disconnect( pClient );

	m_Map_LogonClients.erase( pClient->m_Index );

	// 클라이언트 수 -1
	--m_iClientCount;
	if( m_iClientCount == 0 )
	{
		Clear();
	}
	else
	{
		// READY 상태였으면 ReadyCount -1
		if( pClient->m_bReady == TRUE )
		{
			--m_iReadyCount;
		}
	}
	
	// 접속 종료한게 호스트이면 호스트 재설정
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

	// 유저 추가
	m_Map_LogonClients.insert( map<WORD, CNTClient*>::value_type( pClient->m_Index, pClient ) );
	++m_iClientCount;

#ifdef _DEBUG_
	cout << "User Count : " << m_iClientCount << endl;
#endif

	// 접속한 유저에게 초기 정보를 보내준다.
	SC_INITDATA( pClient );

	// 다른 유저들에게 보낼 패킷을 만든다.
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
	// 접속한 유저에게 초기 좌표를 보내준다.
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

	// 접속되어있는 클라이언트들의 번호
	map<WORD, CNTClient*>::iterator it;
	for( it = m_Map_LogonClients.begin(); it != m_Map_LogonClients.end(); ++it )
	{
		if( pClient->m_Index != it->second->m_Index )
		{
			pk.Write( it->second->m_Index );
		}
	}

	// 현재 선택되어있는 캐릭터
	for( INT i=0; i<4; ++i )
	{
		pk.Write( m_bSelected[i] );
	}

	pk.CalcSize();

	pClient->Send( pk );

	// 호스트 상태 설정
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

	// READY 선택 패킷이면
	if( bSelect == TRUE )
	{
		// 선택 가능한 캐릭터이면
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
	// READY 해제 패킷이면
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
	
	// 모두 READY 상태면 방장 START 버튼 활성화
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

	// 보낼 패킷을 새로 구성한다.
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
	// 클라이언트들에게 패킷을 보낸다.
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
	// 클라이언트들에게 패킷을 보낸다.
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
	// 클라이언트들에게 패킷을 보낸다.
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
	// 로그온
	case MSG_LOGON:
		CS_Logon( pClient, pk );
		break;

	// 캐릭터 선택
	case MSG_READY:
		CS_READY( pClient, pk );
		break;

	// 채팅
	case MSG_CHAT:
		CS_Chat( pClient, pk );
		break;

	// 이동
	case MSG_MOVE:
		CS_Move( pClient, pk );
		break;

	// 공격 : 유저 -> 몬스터
	case MSG_UTOM_ATTACK:
		CS_UTOM_Attack( pClient, pk );
		break;

	// 공격 : 유저 -> 몬스터
	case MSG_ATTACK_ANIMATION:
		CS_Attack_Animation( pClient, pk );
		break;

	// 공격 : 몬스터 -> 유저
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
			// 서버 종료
			if( pClient == NULL )
			{
				// 접속중인 모든 클라이언트를 종료한다.
				map<WORD, CNTClient*>::iterator it;
				for( it = pServer->m_Map_LogonClients.begin(); it != pServer->m_Map_LogonClients.end(); ++it )
				{
					pServer->OnClientClose( it->second );
				}
				return 0;
			}

			// 클라이언트 접속 종료
			pServer->OnClientClose( pClient );
			continue;
		}

		if( ov->m_Mode == RECV_POSTED )
		{
			// 수신
			pClient->m_RecvBuffer.WriteCommit( dwTransferred );

			// 수신데이터가 매세지로 완성되었는지 확인하고 처리
			UINT DataSize = pClient->m_RecvBuffer.GetStoredSize();
			while( DataSize >= 2/*HEADER_SIZE*/ )
			{
				// 헤더에서 크기정보를 얻어온다.
				WORD wMsgSize;
				memcpy( &wMsgSize, pClient->m_RecvBuffer.m_Buffer, sizeof( WORD ) );

				if( wMsgSize <= DataSize )
				{
					// 패킷수신완료, 패킷을 구성해서 처리함수로 넘긴다.
					CPacket pk;
					pk.Copy( pClient->m_RecvBuffer.m_Buffer, wMsgSize );
					pClient->m_RecvBuffer.ReadCommit( wMsgSize );
					
					// 완성된 패킷을 처리한다.
					pServer->ProcessPacket( pClient, pk );
				}

				DataSize = pClient->m_RecvBuffer.GetStoredSize();
			}

			// 수신을 계속한다.
			if( pClient->PostRecv() != TRUE )
			{
				pServer->OnClientClose( pClient );
			}
		}
		else
		{
			// 송신
			if( pClient->SendComplete( dwTransferred ) != TRUE )
			{
				pServer->OnClientClose( pClient );
			}
		}
	}

	cout << "IOCPWorkerProc Terminated" << endl;

	return 0;
}