#include "stdafx.h"

#include "Network.h"
#include "Packet.h"
#include "Charactor.h"
//#include "Frequency.h"


CNetwork::CNetwork()
{
	m_socket		= INVALID_SOCKET;
	m_bStop			= FALSE;
	m_hRecvThread	= INVALID_HANDLE_VALUE;

	InitializeCriticalSection( &m_cs );
}


CNetwork::~CNetwork()
{
	if( m_hRecvThread != INVALID_HANDLE_VALUE )
	{
		WaitForSingleObject( m_hRecvThread, INFINITE );
		m_hRecvThread = INVALID_HANDLE_VALUE;
	}

	DeleteCriticalSection( &m_cs );
}


BOOL CNetwork::CreateSocket()
{
	m_socket = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_socket == INVALID_SOCKET )
	{
		//cout << "socket error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	return TRUE;
}


VOID CNetwork::Close()
{
	if( m_socket != INVALID_SOCKET )
	{
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
	}
}
	

BOOL CNetwork::ConnectToServer( CHAR* szIP, WORD Port )
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( szIP );
	addr.sin_port = htons( Port );

	INT r = connect( m_socket, (SOCKADDR*)&addr, sizeof( SOCKADDR_IN ) );
	if( r != 0 )
	{
		//cout << "connect error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	UINT dwThreadID;
	m_hRecvThread = (HANDLE)_beginthreadex( NULL, 0, RecvThread, this, NULL, &dwThreadID );

	return TRUE;
}


VOID CNetwork::Update()
{
	// 이동 데이터 서버로 보내기
	// 초당 1번 이동 데이터를 보낸다.
	/*FLOAT x = 100.0f;
	FLOAT z = 200.0f;
	FLOAT angle = 300.0f;*/

	static FLOAT time = 0.0f;
	time += CFrequency::GetInstance()->getFrametime();

	if( time > NETWORK_RECV_TIME )
	{
		// 이동 데이터 서버로 보내기
		csMOVE( CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaPos().x,
			CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaPos().z,
			CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaAngle() );

		time = 0.0f;
	}

	// 서버로 부터 받은 패킷을 처리한다.
	EnterCriticalSection( &m_cs );

	if( m_VectorPackets.empty() )
	{
		LeaveCriticalSection( &m_cs );
		return;
	}

	std::vector<CPacket>::iterator it;
	for( it = m_VectorPackets.begin(); it != m_VectorPackets.end(); ++it )
	{
		ProcessPacket( *it );
	}

	
	m_VectorPackets.clear();
	//m_VectorPackets.erase( m_VectorPackets.begin(), m_VectorPackets.end() );

	LeaveCriticalSection( &m_cs );


}


BOOL CNetwork::SendToServer( CPacket& pk )
{
	INT r = send( m_socket, pk.GetBuff(), pk.GetLength(), 0 );
	if( r == SOCKET_ERROR )
	{
		//cout << "send error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	return TRUE;
}


VOID CNetwork::InsertPacket( CPacket& pk )
{
	EnterCriticalSection( &m_cs );

	m_VectorPackets.push_back( pk );

	LeaveCriticalSection( &m_cs );
}


VOID CNetwork::scLOGON( CPacket& pk )
{

}


VOID CNetwork::scCHAT( CPacket& pk )
{
	CHAR szText[128] = { 0, };
	pk.ReadString( szText, 128 );

	//cout << szText << endl;
}


VOID CNetwork::scMOVE( CPacket& pk )
{
	FLOAT x, z, angle;
	WORD number;

	pk.Read( &number );
	pk.Read( &x );
	pk.Read( &z );
	pk.Read( &angle );

	m_vMove = D3DXVECTOR3( x, 0.0f, z );

	for( INT Loop=0; Loop<3; ++Loop )
	{
		if( CObjectManage::GetInstance()->Get_Charactors()[Loop].Get_ClientNumber() == number  )
		{
			CObjectManage::GetInstance()->Get_Charactors()[Loop].UpdateByValue( m_vMove, angle );
			//CDebugConsole::GetInstance()->Messagef( L"Move Number : %d / Recv Pos : %f %f %f \n", number, x, z, angle );
			break;
		}
	}
	

	// 이동 데이터 처리
}

//추가 유저 접속시
VOID CNetwork::scNEWUSER( CPacket& pk )
{
	// 유저 접속 처리
	WORD wNumber;
	pk.Read( &wNumber );

	for( INT Loop=0; Loop<3; ++Loop )
	{
		if( CObjectManage::GetInstance()->Get_Charactors()[Loop].Get_Active() == FALSE )
		{
			CObjectManage::GetInstance()->Get_Charactors()[Loop].Set_Active( TRUE );
			CObjectManage::GetInstance()->Get_Charactors()[Loop].Set_ClientNumber( wNumber );
			break;
		}
	}

	//CDebugConsole::GetInstance()->Messagef( L"New User Number : %d\n" , wNumber );
}

//처음 접속시
VOID CNetwork::scInitData( CPacket& pk )
{
	bool host;
	WORD user_no;
	WORD userCount;
	WORD user_list;

	pk.Read( &host );
	pk.Read( &user_no );
	pk.Read( &userCount );

	CObjectManage::GetInstance()->Set_Host( host );
	CObjectManage::GetInstance()->Set_ClientNumber( user_no );
	//CDebugConsole::GetInstance()->Messagef( L"HOST : %d Number : %d\n" , host, user_no );

	// 유저수 만큼 루프
	for( WORD i=0; i<userCount; ++i )
	{
		pk.Read( &user_list );

		for( INT Loop=0; Loop<3; ++Loop )
		{
			if( CObjectManage::GetInstance()->Get_Charactors()[Loop].Get_Active() == FALSE )
			{
				CObjectManage::GetInstance()->Get_Charactors()[Loop].Set_Active( TRUE );
				CObjectManage::GetInstance()->Get_Charactors()[Loop].Set_ClientNumber( user_list );
				break;
			}
		}
	}
}

VOID CNetwork::csLOGON()
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_LOGON;
	pk.Write( wMsgSize );
	pk.Write( wMsgID );

	CHAR* szName = "HelloWorld";
	CHAR* szPass = "Beautiful";

	pk.WriteString( szName, strlen( szName ) );
	pk.WriteString( szPass, strlen( szPass ) );

	SendToServer( pk );

}


VOID CNetwork::csCHAT()
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_CHAT;
	pk.Write( wMsgSize );
	pk.Write( wMsgID );

	CHAR* szText = "Who Are You?";

	pk.WriteString( szText, strlen( szText ) );

	SendToServer( pk );
}


VOID CNetwork::csMOVE( const FLOAT& x, const FLOAT& z, const FLOAT& angle )
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_MOVE;
	pk.Write( wMsgSize );
	pk.Write( wMsgID );
	pk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	pk.Write( x );
	pk.Write( z );
	pk.Write( angle );
	pk.CalcSize();

	SendToServer( pk );
}


VOID CNetwork::CS_UTOM_ATTACK( CHAR cDestroyPart, WORD wDestroyCount, std::vector<WORD>& pList )
{
	CPacket pk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_UTOM_ATTACK;

	pk.Write( wMsgSize );
	pk.Write( wMsgID );
	//pk.Write( m_wNumber );
	pk.Write( cDestroyPart );
	pk.Write( wDestroyCount );

	for( WORD i=0; i<wDestroyCount; ++i )
	{
		pk.Write( pList[i] );
		//CDebugConsole::GetInstance()->Messagef( L"Send cDestroy List : %d\n", pList[i] );
	}
	CDebugConsole::GetInstance()->Messagef( L"Send cDestroyCount : %d\n", wDestroyCount );

	pk.CalcSize();

	SendToServer( pk );
}


VOID CNetwork::SC_UTOM_ATTACK( CPacket& pk )
{
	WORD wClientNumber;
	CHAR cDestroyPart;
	WORD wDestroyCount;
	WORD wList[1000];

	//pk.Read( &wClientNumber );
	pk.Read( &cDestroyPart );
	pk.Read( &wDestroyCount );

	
	for( WORD i=0; i<wDestroyCount; ++i )
	{
		pk.Read( &wList[i] );
		CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount List : %d\n", wList[i] );
	}

	CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount : %d\n", wDestroyCount );

	CObjectManage::GetInstance()->Get_AlphaMon()->RecvBreakList( wDestroyCount, wList );

	// 디버깅용 출력
	//cout << wClientNumber << " : ";
	//for( WORD i=0; i<cDestroyCount; ++i )
	//{
	//	cout << wList[i];
	//}
	//cout << endl;
}


VOID CNetwork::SC_DISCONNECT( CPacket& pk )
{
	// 접속 유저 비활성화
}


VOID CNetwork::ProcessPacket( CPacket& pk )
{
	WORD wMsgSize, wMsgType;
	pk.Read( &wMsgSize );
	pk.Read( &wMsgType );

	switch( wMsgType )
	{
	// 로그온 처리
	case MSG_CS_LOGON:
		scLOGON( pk );
		break;

	// 초기 정보
	case MSG_SC_INITDATA:
		scInitData( pk );
		break;

	// 새로운 유저 추가 처리
	case MSG_SC_NEWUSER:
		scNEWUSER( pk );
		break;

	// 채팅 처리
	case MSG_SC_CHAT:
		scCHAT( pk );
		break;

	// 이동 처리
	case MSG_SC_MOVE:
		scMOVE( pk );
		break;

	// 공격 ( 유저 -> 몬스터 )
	case MSG_SC_UTOM_ATTACK:
		SC_UTOM_ATTACK( pk );
		break;

	// 유저 접속 종료
	case MSG_SC_DISCONNECT:
		SC_DISCONNECT( pk );
		break;
	}
}


UINT WINAPI RecvThread( LPVOID p )
{
	CNetwork* pNetwork = (CNetwork*)p;

	CHAR buff[BUF_SIZE];
//	INT remainedBufferSize = BUF_SIZE;
	INT buffSize = 0;

	while( pNetwork->m_bStop != TRUE )
	{
		INT nSize = recv( pNetwork->m_socket, buff + buffSize, BUF_SIZE - buffSize, 0 );
		if( nSize == SOCKET_ERROR )
		{
			//cout << "recv Error : " << WSAGetLastError() << endl;
			break;
		}

		// 접속 정료
		if( nSize == 0 )
		{
			//cout << "Disconnected" << endl;
			pNetwork->Close();
			break;
		}

		buffSize += nSize;

		// 수신된 데이터가 완성되었는지 확인한다.
		while( buffSize >= 2 )
		{
			WORD wMsgSize;
			memcpy( &wMsgSize, buff, sizeof( WORD ) );

			// 패킷 처리
			if( wMsgSize <= buffSize )
			{
				CPacket pk;
				pk.Copy( buff, wMsgSize );
				pNetwork->InsertPacket( pk );
				
				memmove( buff, buff + wMsgSize, wMsgSize );
				buffSize -= wMsgSize;
			}
		}
	}

	return 0;
}