#include "stdafx.h"

#include "Network.h"
#include "Packet.h"
#include "Charactor.h"
#include "Monster.h"
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
	

BOOL CNetwork::ConnectToServer( CHAR* a_szIP, WORD a_wPort )
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( a_szIP );
	addr.sin_port = htons( a_wPort );

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
	// �̵� ������ ������ ������
	// �ʴ� 1�� �̵� �����͸� ������.
	/*FLOAT x = 100.0f;
	FLOAT z = 200.0f;
	FLOAT angle = 300.0f;*/

	static FLOAT time = 0.0f;
	time += CFrequency::GetInstance()->getFrametime();

	if( time > NETWORK_RECV_TIME )
	{
		// �̵� ������ ������ ������
		csMOVE( CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaPos().x,
			CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaPos().z,
			CObjectManage::GetInstance()->Get_MyCharactor()->Get_CharaAngle() );

		time = 0.0f;
	}

	// ������ ���� ���� ��Ŷ�� ó���Ѵ�.
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


BOOL CNetwork::SendToServer( CPacket& a_pk )
{
	INT r = send( m_socket, a_pk.GetBuff(), a_pk.GetLength(), 0 );
	if( r == SOCKET_ERROR )
	{
		//cout << "send error : " << WSAGetLastError() << endl;
		return FALSE;
	}

	return TRUE;
}


VOID CNetwork::InsertPacket( CPacket& a_pk )
{
	EnterCriticalSection( &m_cs );

	m_VectorPackets.push_back( a_pk );

	LeaveCriticalSection( &m_cs );
}


VOID CNetwork::scLOGON( CPacket& a_pk )
{

}


VOID CNetwork::scCHAT( CPacket& a_pk )
{
	CHAR szText[128] = { 0, };
	a_pk.ReadString( szText, 128 );

	//cout << szText << endl;
}


VOID CNetwork::scMOVE( CPacket& a_pk )
{
	FLOAT x, z, angle;
	WORD number;

	a_pk.Read( &number );
	a_pk.Read( &x );
	a_pk.Read( &z );
	a_pk.Read( &angle );

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
	

	// �̵� ������ ó��
}

//�߰� ���� ���ӽ�
VOID CNetwork::scNEWUSER( CPacket& a_pk )
{
	// ���� ���� ó��
	WORD wNumber;
	a_pk.Read( &wNumber );

	if( CObjectManage::GetInstance()->Get_Charactors()[wNumber].Get_Active() == FALSE )
	{
		CObjectManage::GetInstance()->Get_Charactors()[wNumber].Set_Active( TRUE );
		CObjectManage::GetInstance()->Get_Charactors()[wNumber].Set_ClientNumber( wNumber );
	}

	//CDebugConsole::GetInstance()->Messagef( L"New User Number : %d\n" , wNumber );
}

//ó�� ���ӽ�
VOID CNetwork::scInitData( CPacket& a_pk )
{
	bool host;	// 1����Ʈ bool ���
	WORD user_no;
	WORD userCount;
	WORD user_list;

	a_pk.Read( &host );
	a_pk.Read( &user_no );
	a_pk.Read( &userCount );

	CObjectManage::GetInstance()->Set_Host( host );
	CObjectManage::GetInstance()->Set_ClientNumber( user_no );
	//CDebugConsole::GetInstance()->Messagef( L"HOST : %d Number : %d\n" , host, user_no );

	// ������ ��ŭ ����
	for( WORD i=0; i<userCount; ++i )
	{
		a_pk.Read( &user_list );

		if( CObjectManage::GetInstance()->Get_Charactors()[user_list].Get_Active() == FALSE )
		{
			CObjectManage::GetInstance()->Get_Charactors()[user_list].Set_Active( TRUE );
			CObjectManage::GetInstance()->Get_Charactors()[user_list].Set_ClientNumber( user_list );
		}
	}
}

VOID CNetwork::csLOGON()
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_LOGON;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );

	CHAR* szName = "HelloWorld";
	CHAR* szPass = "Beautiful";

	sendPk.WriteString( szName, strlen( szName ) );
	sendPk.WriteString( szPass, strlen( szPass ) );

	SendToServer( sendPk );

}


VOID CNetwork::csCHAT()
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_CHAT;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );

	CHAR* szText = "Who Are You?";

	sendPk.WriteString( szText, strlen( szText ) );

	SendToServer( sendPk );
}


VOID CNetwork::csMOVE( const FLOAT& a_fX, const FLOAT& a_fZ, const FLOAT& a_fAngle )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_MOVE;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_fX );
	sendPk.Write( a_fZ );
	sendPk.Write( a_fAngle );
	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::CS_UTOM_ATTACK( CHAR a_cDestroyPart, WORD a_wDestroyCount, std::vector<WORD>& a_pList, D3DXVECTOR3 a_vDirection )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_UTOM_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_vDirection.x );
	sendPk.Write( a_vDirection.y );
	sendPk.Write( a_vDirection.z );
	sendPk.Write( a_cDestroyPart );
	sendPk.Write( a_wDestroyCount );

	for( WORD i=0; i<a_wDestroyCount; ++i )
	{
		sendPk.Write( a_pList[i] );
		//CDebugConsole::GetInstance()->Messagef( L"Send cDestroy List : %d\n", pList[i] );
	}
	CDebugConsole::GetInstance()->Messagef( L"Send Part:cDestroyCount : %d : %d\n", a_cDestroyPart, a_wDestroyCount );

	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::SC_UTOM_ATTACK( CPacket& a_pk )
{
	FLOAT fDirX, fDirY, fDirZ;
	WORD wClientNumber;
	CHAR cDestroyPart;
	WORD wDestroyCount;
	WORD wList[1000];

	a_pk.Read( &fDirX);
	a_pk.Read( &fDirY);
	a_pk.Read( &fDirZ);
	a_pk.Read( &wClientNumber );
	a_pk.Read( &cDestroyPart );
	a_pk.Read( &wDestroyCount );

	
	for( WORD i=0; i<wDestroyCount; ++i )
	{
		a_pk.Read( &wList[i] );
		//CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount List : %d\n", wList[i] );
	}

	CDebugConsole::GetInstance()->Messagef( L"Rcv Part:wDestroyCount : %d : %d\n", cDestroyPart, wDestroyCount );

	CObjectManage::GetInstance()->Get_Monster()->Get_MonsterPart()[cDestroyPart].RecvBreakList( wDestroyCount, wList, D3DXVECTOR3( fDirX, fDirY, fDirZ ) );

	// ������ ���
	//cout << wClientNumber << " : ";
	//for( WORD i=0; i<cDestroyCount; ++i )
	//{
	//	cout << wList[i];
	//}
	//cout << endl;
}

VOID CNetwork::CS_UTOM_Attack_Animation( WORD a_wAnimationNumber )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CS_UTOM_ATTACK_ANIMATION;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_wAnimationNumber );

	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::SC_UTOM_Attack_Animation( CPacket& a_pk )
{
	WORD wClientNumber;
	WORD wAnimationNumber;

	a_pk.Read( &wClientNumber );
	a_pk.Read( &wAnimationNumber );

	CObjectManage::GetInstance()->Get_Charactors()[wClientNumber].Set_WeaponAnimationState( wAnimationNumber );
}


VOID CNetwork::SC_DISCONNECT( CPacket& a_pk )
{
	WORD wClientNumber;
	a_pk.Read( &wClientNumber );

	// ���� ���� ��Ȱ��ȭ
	CObjectManage::GetInstance()->Get_Charactors()[wClientNumber].Set_Active( FALSE );
}


VOID CNetwork::ProcessPacket( CPacket& a_pk )
{
	WORD wMsgSize, wMsgType;
	a_pk.Read( &wMsgSize );
	a_pk.Read( &wMsgType );

	switch( wMsgType )
	{
	// �α׿� ó��
	case MSG_CS_LOGON:
		scLOGON( a_pk );
		break;

	// �ʱ� ����
	case MSG_SC_INITDATA:
		scInitData( a_pk );
		break;

	// ���ο� ���� �߰� ó��
	case MSG_SC_NEWUSER:
		scNEWUSER( a_pk );
		break;

	// ä�� ó��
	case MSG_SC_CHAT:
		scCHAT( a_pk );
		break;

	// �̵� ó��
	case MSG_SC_MOVE:
		scMOVE( a_pk );
		break;

	// ���� ( ���� -> ���� )
	case MSG_SC_UTOM_ATTACK:
		SC_UTOM_ATTACK( a_pk );
		break;

	// ���� ���� ����
	case MSG_SC_DISCONNECT:
		SC_DISCONNECT( a_pk );
		break;

	case MSG_SC_UTOM_ATTACK_ANIMATION:
		SC_UTOM_Attack_Animation( a_pk );
		break;
	}
}


UINT WINAPI RecvThread( LPVOID a_p )
{
	CNetwork* pNetwork = (CNetwork*)a_p;

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

		// ���� ����
		if( nSize == 0 )
		{
			//cout << "Disconnected" << endl;
			pNetwork->Close();
			break;
		}

		buffSize += nSize;

		// ���ŵ� �����Ͱ� �ϼ��Ǿ����� Ȯ���Ѵ�.
		while( buffSize >= 2 )
		{
			WORD wMsgSize;
			memcpy( &wMsgSize, buff, sizeof( WORD ) );

			// ��Ŷ ó��
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