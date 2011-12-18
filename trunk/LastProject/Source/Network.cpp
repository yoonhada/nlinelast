#include "stdafx.h"

#include "Network.h"
#include "Packet.h"
#include "Charactor.h"
#include "Monster.h"
#include "MainScene.h"
#include "LobbyScene.h"
#include "Chase.h"
#include "Melee.h"
#include "Melee2.h"
#include "Spin.h"
#include "Dash.h"
#include "DashReady.h"
#include "TimeLifeItem.h"


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


VOID CNetwork::SC_LOGON( CPacket& a_pk )
{

}


//처음 접속시
VOID CNetwork::SC_INIT( CPacket& a_pk )
{
	BOOL host;
	WORD user_no;
	WORD userCount;
	WORD user_list;
	BOOL bSelected;

	a_pk.Read( &host );
	a_pk.Read( &user_no );
	a_pk.Read( &userCount );

	CObjectManage::GetInstance()->Set_Host( host );
	CObjectManage::GetInstance()->Set_ClientNumber( user_no );
	//CDebugConsole::GetInstance()->Messagef( L"HOST : %d Number : %d\n" , host, user_no );
	CCharactor * pCharactors = CObjectManage::GetInstance()->Get_Charactor();
	// 유저수 만큼 루프
	for( WORD i=0; i<userCount; ++i )
	{
		a_pk.Read( &user_list );

		if( pCharactors[user_list].Get_Active() == FALSE )
		{
			pCharactors[user_list].Set_Active( TRUE );
			pCharactors[user_list].Set_ClientNumber( user_list );
		}
	}

	for( INT i=0; i<4; ++i )
	{
		a_pk.Read( &bSelected );
		if( bSelected == TRUE )
		{
			// 선택되어 있는 캐릭터들 표시
		}
	}

	//CSceneManage::GetInstance()->OrderChangeScene( new CMainScene );
}


VOID CNetwork::SC_CHANGE_HOST( CPacket& a_pk )
{
	// 호스트로 설정한다.
	CObjectManage::GetInstance()->Set_Host( TRUE );
}


//추가 유저 접속시
VOID CNetwork::SC_ADD_USER( CPacket& a_pk )
{
	// 유저 접속 처리
	WORD wNumber;
	a_pk.Read( &wNumber );

	//CCharactor ** pCharactors = CObjectManage::GetInstance()->Get_Charactors();
	//if( pCharactors[wNumber]->Get_Active() == FALSE )
	//{
	//	pCharactors[wNumber]->Set_Active( TRUE );
	//	pCharactors[wNumber]->Set_ClientNumber( wNumber );
	//}

	if ( CObjectManage::GetInstance()->IsHost() )
	{
		CObjectManage::GetInstance()->GetLobbyScene()->EnableButton( LOBBY_START, FALSE );
	}
	//CDebugConsole::GetInstance()->Messagef( L"New User Number : %d\n" , wNumber );
}


VOID CNetwork::SC_READY( CPacket& a_pk )
{
	WORD wUserNumber;
	WORD wSelectNumber;
	BOOL bSelect;

	a_pk.Read( &wUserNumber );
	a_pk.Read( &wSelectNumber );
	a_pk.Read( &bSelect );

	// 캐릭터 선택
	CObjectManage * pOM = CObjectManage::GetInstance();
	//pOM->Set_Char( wUserNumber, wSelect );
	if ( pOM->Get_ClientNumber() == wUserNumber )
	{
		if ( bSelect == TRUE )
		{
			// 케릭터 선택 
			pOM->GetLobbyScene()->m_nSelectState[wUserNumber] = wSelectNumber;
			pOM->GetLobbyScene()->CreateWeapon( pOM->GetLobbyScene()->m_nCharSelect );
			pOM->GetLobbyScene()->ChangeStateButton( LOBBY_READY, DOWN );
		}
		else
		{
			// 케릭터 선택 실패.. 회전 제거.
			pOM->GetLobbyScene()->DestoryWeapon( pOM->GetLobbyScene()->m_nSelectState[wUserNumber] );
			pOM->GetLobbyScene()->m_nSelectState[wUserNumber] = -1;
			pOM->GetLobbyScene()->ChangeStateButton( LOBBY_READY, UP );
			pOM->GetLobbyScene()->ChangeStateButton( LOBBY_SELECT_1 + wSelectNumber, DISABLE );
		}
	}
	else 
	{
		if ( bSelect == TRUE )
		{
			// 다른 케릭터 선택, 선택차단....
			pOM->GetLobbyScene()->m_nSelectState[wUserNumber] = wSelectNumber;
			pOM->GetLobbyScene()->EnableRotate( wSelectNumber );
		}
		else
		{
			// 다른 플레이어 선택해지,  차단 해지...
			pOM->GetLobbyScene()->m_nSelectState[wUserNumber] = -1;
			pOM->GetLobbyScene()->DisableRotate( wSelectNumber );
		}
	}

	//if ( pOM->IsHost() && 
	//	pOM->GetLobbyScene()->m_nSelectState[0] == LobbyScene::SELECT &&
	//	pOM->GetLobbyScene()->m_nSelectState[1] == LobbyScene::SELECT &&
	//	pOM->GetLobbyScene()->m_nSelectState[2] == LobbyScene::SELECT &&
	//	pOM->GetLobbyScene()->m_nSelectState[3] == LobbyScene::SELECT )
	//{
	//	// 스타트 버튼 활성화
	//}

}

VOID CNetwork::SC_ENABLE_START( CPacket& a_pk )
{
	BOOL bStart;
	a_pk.Read( &bStart );

	if ( bStart )
	{
		CObjectManage::GetInstance()->GetLobbyScene()->EnableButton( LOBBY_START, TRUE );
	}
	else
	{
		CObjectManage::GetInstance()->GetLobbyScene()->EnableButton( LOBBY_START, FALSE );
	}
}


VOID CNetwork::SC_GAME_START( CPacket& a_pk )
{
	CObjectManage::GetInstance()->GetLobbyScene()->UpdateCharArray();
	CObjectManage::GetInstance()->GetLobbyScene()->SetSceneState( IScene::SCENE_END );
}


VOID CNetwork::SC_LODING_COMPLETE( CPacket& a_pk )
{
	// 모든 유저의 로딩이 끝나면 Scene을 변경한다.

}


VOID CNetwork::SC_CLIENT_DISCONNECT( CPacket& a_pk )
{
	WORD wClientNumber;
	a_pk.Read( &wClientNumber );

	// 접속 유저 비활성화
	CObjectManage * pOM = CObjectManage::GetInstance();
	pOM->Get_CharTable()[ wClientNumber ] = -1;
	//
	//CCharactor& rChar = pOM->Get_Charactor()[ pOM->Get_CharTable( wClientNumber ) ];

	//CObjectManage::GetInstance()->Get_Charactors()[wClientNumber]->Set_Active( FALSE );
}


VOID CNetwork::SC_SERVER_CLOSE( CPacket& a_pk )
{
	// 서버가 종료된 처리를 한다.
}


VOID CNetwork::SC_EVENT_STATE( CPacket& a_pk )
{
	WORD wEventID;
	a_pk.Read( &wEventID );

	// 이벤트 활성화
	CGameEvent::GetInstance()->AddEvent( wEventID, 0.01f );
	CDebugConsole::GetInstance()->Message("SC_EVENT_STATE\n\n");
}


VOID CNetwork::SC_EVENT_COMBO_INFO( CPacket& a_pk )
{
	INT iEventKind[4];
	a_pk.Read( &iEventKind[0] );
	a_pk.Read( &iEventKind[1] );
	a_pk.Read( &iEventKind[2] );
	a_pk.Read( &iEventKind[3] );

	INT *nTable = CObjectManage::GetInstance()->Get_EventTable();
	nTable[0] = iEventKind[0];
	nTable[1] = iEventKind[1];
	nTable[2] = iEventKind[2];
	nTable[3] = iEventKind[3];

	CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO, 0.01f );
}


VOID CNetwork::SC_EVENT_COMBO_SLOT_STATE( CPacket& a_pk )
{
	WORD wSlotNumber;
	BOOL bResult;

	a_pk.Read( &wSlotNumber );
	a_pk.Read( &bResult );

	CGameEvent::GetInstance()->AddEvent( CGameEvent::SC_EVENT_COMBO_SLOT_STATE, 0.01f );
	CGameEvent::GetInstance()->SetSlotNumber( wSlotNumber );
	CGameEvent::GetInstance()->SetResult( bResult );
}


VOID CNetwork::SC_EVENT_COMBO_RESULT( CPacket& a_pk )
{
	BOOL bResult;

	a_pk.Read( &bResult );
}


VOID CNetwork::SC_CHAT( CPacket& a_pk )
{
	WCHAR szText[256] = { 0, };
	a_pk.ReadString( szText, 256 );

	// 채팅창에 출력
	CObjectManage::GetInstance()->GetLobbyScene()->SetText( szText );
}


VOID CNetwork::SC_PLAYER_MOVEMENT( CPacket& a_pk )
{
	FLOAT x, z, angle;
	WORD number;

	a_pk.Read( &number );
	a_pk.Read( &x );
	a_pk.Read( &z );
	a_pk.Read( &angle );

	m_vMove = D3DXVECTOR3( x, 0.0f, z );

	CObjectManage* pOM = CObjectManage::GetInstance();
	CCharactor& pChar = pOM->Get_Charactor()[ pOM->Get_CharTable( number ) ];

	if ( pOM->Get_ClientNumber() != number )
	{
		pChar.UpdateByValue( m_vMove, angle );
	}
/*
	for( INT Loop = 0; Loop < 4; ++Loop )
	{
		if( pCharactors[Loop]->Get_ClientNumber() == number  )
		{
			pCharactors[Loop]->UpdateByValue( m_vMove, angle );
			//CDebugConsole::GetInstance()->Messagef( L"Move Number : %d / Recv Pos : %f %f %f \n", number, x, z, angle );
			break;
		}
	}
*/	

	// 이동 데이터 처리
}


VOID CNetwork::SC_MONSTER_MOVEMENT( CPacket& a_pk )
{
	WORD wMonsterNumber;
	WORD wPathCount;
	WORD wX, wY, wRemainedNode;
	PathNode* pStartPath = NULL;
	PathNode* pPrev = NULL;
	PathNode* ptemp = NULL;

	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &wPathCount );

	for( WORD i=0; i<wPathCount; ++i )
	{
		a_pk.Read( &wX );
		a_pk.Read( &wY );
		a_pk.Read( &wRemainedNode );

		ptemp = Astar::GetInstance()->createPath( wX, wY );
		ptemp->remainedNode = wRemainedNode;

		if( i == 0 )
		{
			pStartPath = ptemp;
			pPrev = ptemp;
		}
		else
		{
			pPrev->next = ptemp;
			pPrev = ptemp;
		}
	}

	// 몬스터에게 적용한다.
	CMonster** pMonster = CObjectManage::GetInstance()->Get_Monster();
	pMonster[wMonsterNumber]->Set_Path( pStartPath );
	pMonster[wMonsterNumber]->GetFSM()->ChangeState( Chase::GetInstance() );
}


VOID CNetwork::SC_UTOM_ATTACK( CPacket& a_pk )
{
	WORD wClientNumber = 0;
	WORD wMonsterNumber = 0;
	FLOAT fDirX, fDirY, fDirZ;
	WORD wTotalParts = 0;
	WORD wDestroyPart = 0;
	WORD wDestroyCount = 0;

	a_pk.Read( &wClientNumber );
	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &fDirX);
	a_pk.Read( &fDirY);
	a_pk.Read( &fDirZ);
	a_pk.Read( &wTotalParts );

	for( WORD i=0; i<wTotalParts; ++i )
	{
		INT k = 0;
		WORD wList[1000];

		a_pk.Read( &wDestroyPart );
		a_pk.Read( &wDestroyCount );

		for( WORD j=0; j<wDestroyCount; ++j )
		{
			a_pk.Read( &wList[k] );
			CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount List : %d\n", wList[k] );
			++k;
		}

		CDebugConsole::GetInstance()->Messagef( L"Rcv Part:wDestroyCount : %d : %d\n", wDestroyPart, wDestroyCount );
		CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->Get_MonsterPart()[wDestroyPart].RecvBreakList( wDestroyCount, wList, D3DXVECTOR3( fDirX, fDirY, fDirZ ) );
		CGameEvent::GetInstance()->Set_PlayerIndex( CObjectManage::GetInstance()->Get_CharTable( static_cast<int>( wClientNumber ) ) );
		CGameEvent::GetInstance()->Set_MonsterIndex( static_cast<int>( wMonsterNumber ) );
	}
}


VOID CNetwork::SC_MTOU_ATTACK( CPacket& a_pk )
{
	WORD wClientNumber;
	FLOAT fDirX, fDirY, fDirZ;
	WORD wDestroyCount;
	WORD wList[1000];

	a_pk.Read( &wClientNumber );
	a_pk.Read( &fDirX);
	a_pk.Read( &fDirY);
	a_pk.Read( &fDirZ);
	a_pk.Read( &wDestroyCount );


	for( WORD i=0; i<wDestroyCount; ++i )
	{
		a_pk.Read( &wList[i] );
		CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount List : %d\n", wList[i] );
	}

	CObjectManage * pOM = CObjectManage::GetInstance();
	CCharactor& rChar = pOM->Get_Charactor()[ pOM->Get_CharTable( wClientNumber ) ];

	rChar.RecvBreakList( wDestroyCount, wList, D3DXVECTOR3( fDirX, fDirY, fDirZ ) );
	//CDebugConsole::GetInstance()->Messagef( L"Rcv Part:wDestroyCount : %d : %d\n", cDestroyPart, wDestroyCount );

	//CObjectManage::GetInstance()->Get_CharactorList()[wClientNumber]->RecvBreakList( wDestroyCount, wList, D3DXVECTOR3( fDirX, fDirY, fDirZ ) );
}


VOID CNetwork::SC_EVENT_ATTACK( CPacket& a_pk )
{
	WORD wClientNumber = 0;
	WORD wMonsterNumber = 0;
	FLOAT fDirX, fDirY, fDirZ;
	WORD wTotalParts = 0;
	WORD wDestroyPart = 0;
	WORD wDestroyCount = 0;

	a_pk.Read( &wClientNumber );
	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &fDirX);
	a_pk.Read( &fDirY);
	a_pk.Read( &fDirZ);
	a_pk.Read( &wDestroyCount );

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyCount : %d \n", wDestroyCount );
#endif

	WORD wList[1000];
	for( WORD i=0; i<wDestroyCount; ++i )
	{
		a_pk.Read( &wList[i] );
#ifdef _DEBUG
		CDebugConsole::GetInstance()->Messagef( L"Rcv wDestroyList : %d \n", wList[i] );
#endif
	}

	CObjectManage::GetInstance()->Get_Wall()[wMonsterNumber].RecvBreakList( wDestroyCount, wList, D3DXVECTOR3( fDirX, fDirY, fDirZ ) );
	CGameEvent::GetInstance()->Set_PlayerIndex( CObjectManage::GetInstance()->Get_CharTable( static_cast<int>( wClientNumber ) ) );
	CGameEvent::GetInstance()->Set_MonsterIndex( static_cast<int>( wMonsterNumber ) );
}


VOID CNetwork::SC_Player_Attack_Animation( CPacket& a_pk )
{
	WORD wClientNumber;
	WORD wAnimationNumber;

	a_pk.Read( &wClientNumber );
	a_pk.Read( &wAnimationNumber );

	CObjectManage * pOM = CObjectManage::GetInstance();
	CCharactor& rChar = pOM->Get_Charactor()[ pOM->Get_CharTable( wClientNumber ) ];

	rChar.Set_WeaponAnimationState( wAnimationNumber );
}


VOID CNetwork::SC_Monster_Attack_Animation( CPacket& a_pk )
{
	WORD wMonsterNumber;
	WORD wAnimationNumber;

	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &wAnimationNumber );

	if( wAnimationNumber == CMonster::ANIM_MELEE_ATTACK )
	{
		CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->GetFSM()->ChangeState( Melee::GetInstance() );
	}
	else if( wAnimationNumber == CMonster::ANIM_MELEE_ATTACK2 )
	{
		CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->GetFSM()->ChangeState( Melee2::GetInstance() );
	}
	else if( wAnimationNumber == CMonster::ANIM_DASH )
	{
		CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->GetFSM()->ChangeState( DashReady::GetInstance() );
	}
}


VOID CNetwork::SC_Monster_Attack_Animation2( CPacket& a_pk )
{
	WORD wMonsterNumber;
	WORD wAnimationNumber;
	FLOAT fDegree;
	FLOAT fPosX;
	FLOAT fPosZ;
	FLOAT fNextPosX;
	FLOAT fNextPosZ;
	FLOAT fTargetDistance;

	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &wAnimationNumber );
	a_pk.Read( &fDegree );
	a_pk.Read( &fPosX );
	a_pk.Read( &fPosZ );
	a_pk.Read( &fNextPosX );
	a_pk.Read( &fNextPosZ );
	a_pk.Read( &fTargetDistance );

	D3DXVECTOR3 vPos		= D3DXVECTOR3( fPosX, 0.0f, fPosZ );
	D3DXVECTOR3 vNextPos	= D3DXVECTOR3( fNextPosX, 0.0f, fNextPosZ );

	CMonster** pMonster = CObjectManage::GetInstance()->Get_Monster();
	pMonster[wMonsterNumber]->Set_Angle( fDegree );
	pMonster[wMonsterNumber]->Set_Pos( vPos );
	pMonster[wMonsterNumber]->Set_TargetPos( vNextPos );
	pMonster[wMonsterNumber]->Set_TargetDistance( fTargetDistance );

	CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->GetFSM()->ChangeState( DashReady::GetInstance() );
}


VOID CNetwork::SC_Monster_LockOn( CPacket& a_pk )
{
	WORD wMonsterNumber;
	FLOAT fAngle;

	a_pk.Read( &wMonsterNumber );
	a_pk.Read( &fAngle );

	CObjectManage::GetInstance()->Get_Monster()[wMonsterNumber]->Set_Angle( D3DXToRadian( fAngle ) );
}


VOID CNetwork::CS_LOGON( LPWSTR a_szNickName )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_LOGON;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );

	sendPk.WriteString( a_szNickName, lstrlen( a_szNickName ) );

	SendToServer( sendPk );
#ifndef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"LOGON send \n" );
#endif
}


VOID CNetwork::CS_READY( WORD a_wSelect, BOOL a_bSelect )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_READY;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_wSelect );
	sendPk.Write( a_bSelect );
	sendPk.CalcSize();

	SendToServer( sendPk );
#ifndef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"READY send \n" );
#endif
}


VOID CNetwork::CS_GAME_START()
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_GAME_START;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.CalcSize();

	SendToServer( sendPk );
#ifndef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"GAME START send \n" );
#endif
}


VOID CNetwork::CS_LODING_COMPLETE()
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_LODING_COMPLETE;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.CalcSize();

	SendToServer( sendPk );
#ifndef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"LODING COMPLETE send \n" );
#endif
}


VOID CNetwork::CS_CLIENT_DISCONNECT()
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CLIENT_DISCONNECT;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::CS_EVENT_STATE( WORD a_wEventID )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_EVENT_STATE;
	
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_wEventID );
	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::CS_EVENT_COMBO( INT * a_iEventKind )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_EVENT_COMBO_INFO;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_iEventKind[0] );
	sendPk.Write( a_iEventKind[1] );
	sendPk.Write( a_iEventKind[2] );
	sendPk.Write( a_iEventKind[3] );
	sendPk.CalcSize();

	SendToServer( sendPk );	
}


VOID CNetwork::CS_EVENT_COMBO_SLOT_STATE( WORD a_wSlotNumber, BOOL a_bState )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_EVENT_COMBO_SLOT_STATE;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_wSlotNumber );
	sendPk.Write( a_bState );
	sendPk.CalcSize();

	SendToServer( sendPk );	
}


VOID CNetwork::CS_EVENT_COMBO_RESULT( BOOL a_bResult )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_EVENT_COMBO_RESULT;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_bResult );
	sendPk.CalcSize();

	SendToServer( sendPk );	
}


VOID CNetwork::CS_CHAT( LPWSTR a_szText )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_CHAT;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.WriteString( a_szText, lstrlen( a_szText ) * 2 );

	SendToServer( sendPk );
#ifndef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"CHAT send \n " );
#endif
}


VOID CNetwork::CS_PLAYER_MOVEMENT( CONST FLOAT& a_fX, CONST FLOAT& a_fZ, CONST FLOAT& a_fAngle )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_PLAYER_MOVE;
	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_fX );
	sendPk.Write( a_fZ );
	sendPk.Write( a_fAngle );
	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"****MOV \n " );
#endif
}


VOID CNetwork::CS_MONSTER_MOVEMENT( WORD a_iMonsterNumber, PathNode* a_pPath )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_MONSTER_MOVE;
	WORD wPathCount = a_pPath->remainedNode + 1;
	WORD wX, wY, wRemainedNode;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_iMonsterNumber );
	sendPk.Write( wPathCount );

	// Path를 읽어 패킷에 넣는다.
	while( a_pPath != NULL )
	{
		wX = a_pPath->x;
		wY = a_pPath->y;
		wRemainedNode = a_pPath->remainedNode;

		sendPk.Write( wX );
		sendPk.Write( wY );
		sendPk.Write( wRemainedNode );

		a_pPath = a_pPath->next;
	}

	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"MONSTER MOVEMENT send \n" );
#endif
}


VOID CNetwork::CS_UTOM_ATTACK( WORD a_wMonsterNumber, D3DXVECTOR3 a_vDirection, WORD a_wTotalParts, WORD a_wDestroyPart[], WORD a_wDestroyCount[], std::vector<WORD>& a_pList )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_UTOM_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_wMonsterNumber);
	sendPk.Write( a_vDirection.x );
	sendPk.Write( a_vDirection.y );
	sendPk.Write( a_vDirection.z );
	sendPk.Write( a_wTotalParts );

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Send TotalParts : %d \n", a_wTotalParts );
#endif

	INT k = 0;
	for( WORD i=0; i<a_wTotalParts; ++i )
	{
		sendPk.Write( a_wDestroyPart[i] );
		sendPk.Write( a_wDestroyCount[i] );

#ifdef _DEBUG
		CDebugConsole::GetInstance()->Messagef( L"Send DestroyPart : %d \n", a_wDestroyPart[i] );
		CDebugConsole::GetInstance()->Messagef( L"Send DestroyCount : %d \n", a_wDestroyCount[i] );
#endif

		for( WORD j=0; j<a_wDestroyCount[i]; ++j )
		{
			sendPk.Write( a_pList[k] );

#ifdef _DEBUG
			CDebugConsole::GetInstance()->Messagef( L"Send cDestroy List : %d\n", a_pList[k] );
#endif
			++k;
		}
	}

	sendPk.CalcSize();

	SendToServer( sendPk );

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Packet Length : %d \n", sendPk.GetLength() );
	CDebugConsole::GetInstance()->Messagef( L"Send Size : %d \n", wMsgSize );
	CDebugConsole::GetInstance()->Messagef( L"****C-ATK \n" );
#endif
}


VOID CNetwork::CS_MTOU_ATTACK( D3DXVECTOR3 a_vDirection, WORD a_wDestroyCount, std::vector<WORD>& a_pList )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_MTOU_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_vDirection.x );
	sendPk.Write( a_vDirection.y );
	sendPk.Write( a_vDirection.z );
	sendPk.Write( a_wDestroyCount );

#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"cDestroyCount : %d \n", a_wDestroyCount );
#endif

	for( WORD i=0; i<a_wDestroyCount; ++i )
	{
		sendPk.Write( a_pList[i] );

#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"Send cDestroy List : %d\n", a_pList[i] );
#endif
	}
	
	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"M-ATK \n" );
#endif
}


VOID CNetwork::CS_EVENT_ATTACK( WORD a_wMonsterNumber, D3DXVECTOR3 a_vDirection, WORD a_wDestroyCount, std::vector<WORD>& a_pList )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_EVENT_ATTACK;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_wMonsterNumber);
	sendPk.Write( a_vDirection.x );
	sendPk.Write( a_vDirection.y );
	sendPk.Write( a_vDirection.z );
	sendPk.Write( a_wDestroyCount );

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Send DestroyCount : %d \n", a_wDestroyCount );
#endif

	for( WORD i=0; i<a_wDestroyCount; ++i )
	{
		sendPk.Write( a_pList[i] );
#ifdef _DEBUG
		CDebugConsole::GetInstance()->Messagef( L"Send DestroyList : %d \n", a_pList[i] );
#endif
	}

	sendPk.CalcSize();

	SendToServer( sendPk );
}


VOID CNetwork::CS_Player_Attack_Animation( WORD a_wAnimationNumber )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_PLAYER_ATTACK_ANIMATION;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( CObjectManage::GetInstance()->Get_ClientNumber() );
	sendPk.Write( a_wAnimationNumber );

	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Player ATK ANI send \n" );
#endif
}


VOID CNetwork::CS_Monster_Attack_Animation( WORD a_wMonsterNumber, WORD a_wAnimationNumber )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_MONSTER_ATTACK_ANIMATION;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_wMonsterNumber );
	sendPk.Write( a_wAnimationNumber );

	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Monster ATK ANI send \n" );
#endif
}


VOID CNetwork::CS_Monster_Attack_Animation2( WORD a_wMonsterNumber, WORD a_wAnimationNumber, FLOAT a_fDegree, D3DXVECTOR3 a_vPos, D3DXVECTOR3 a_vNextPos, FLOAT a_fDistance )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_MONSTER_ATTACK_ANIMATION2;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_wMonsterNumber );
	sendPk.Write( a_wAnimationNumber );
	sendPk.Write( a_fDegree );
	sendPk.Write( a_vPos.x );
	sendPk.Write( a_vPos.z );
	sendPk.Write( a_vNextPos.x );
	sendPk.Write( a_vNextPos.z );
	sendPk.Write( a_fDistance );

	sendPk.CalcSize();

	SendToServer( sendPk );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Monster ATK ANI2 send \n" );
#endif
}


VOID CNetwork::CS_Monster_LockOn( WORD a_wMonsterNumber, FLOAT a_fAngle )
{
	CPacket sendPk;
	WORD wMsgSize = 0;
	WORD wMsgID = MSG_MONSTER_LOCKON;

	sendPk.Write( wMsgSize );
	sendPk.Write( wMsgID );
	sendPk.Write( a_wMonsterNumber );
	sendPk.Write( a_fAngle );

	sendPk.CalcSize();

	SendToServer( sendPk );

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Monster LockOn send \n ");
#endif
}


VOID CNetwork::InsertPacket( CPacket& a_pk )
{
	EnterCriticalSection( &m_cs );

	m_VectorPackets.push_back( a_pk );

	LeaveCriticalSection( &m_cs );
}


VOID CNetwork::ProcessPacket( CPacket& a_pk )
{
	WORD wMsgSize, wMsgType;
	a_pk.Read( &wMsgSize );
	a_pk.Read( &wMsgType );

	switch( wMsgType )
	{
	// 로그온
	case MSG_LOGON:
		SC_LOGON( a_pk );
		break;

	// 초기 정보
	case MSG_INITDATA:
		SC_INIT( a_pk );
		break;

	// 새로운 유저 추가
	case MSG_ADD_USER:
		SC_ADD_USER( a_pk );
		break;

	// 호스트 변경
	case MSG_CHANGE_HOST:
		SC_CHANGE_HOST( a_pk );
		break;

	// READY
	case MSG_READY:
		SC_READY( a_pk );
		break;

	// 게임 스타트 버튼 활성화
	case MSG_START_BUTTON_STATE:
		SC_ENABLE_START( a_pk );
		break;

	// 게임 시작
	case MSG_GAME_START:
		SC_GAME_START( a_pk );
		break;

	// 로딩 완료
	case MSG_LODING_COMPLETE:
		SC_LODING_COMPLETE( a_pk );
		break;

	// 유저 접속 종료
	case MSG_CLIENT_DISCONNECT:
		SC_CLIENT_DISCONNECT( a_pk );
		break;

	// 서버 종료
	case MSG_SERVER_CLOSE:
		SC_SERVER_CLOSE( a_pk );
		break;

	// 이벤트 발생
	case MSG_EVENT_STATE:
		SC_EVENT_STATE( a_pk );
		break;

	// 콤보 정보
	case MSG_EVENT_COMBO_INFO:
		SC_EVENT_COMBO_INFO( a_pk );
		break;

	case MSG_EVENT_COMBO_SLOT_STATE:
		SC_EVENT_COMBO_SLOT_STATE( a_pk );
		break;

	// 콤보 결과
	case MSG_EVENT_COMBO_RESULT:
		SC_EVENT_COMBO_RESULT( a_pk );
		break;

	// 채팅
	case MSG_CHAT:
		SC_CHAT( a_pk );
		break;

	// 유저 이동
	case MSG_PLAYER_MOVE:
		SC_PLAYER_MOVEMENT( a_pk );
		break;

	// 몬스터 이동
	case MSG_MONSTER_MOVE:
		SC_MONSTER_MOVEMENT( a_pk );
		break;

	// 공격 ( 유저 -> 몬스터 )
	case MSG_UTOM_ATTACK:
		SC_UTOM_ATTACK( a_pk );
		break;

	// 공격 : 몬스터 -> 유저
	case MSG_MTOU_ATTACK:
		SC_MTOU_ATTACK( a_pk );
		break;

	// 이벤트
	case MSG_EVENT_ATTACK:
		SC_EVENT_ATTACK( a_pk );
		break;

	// 유저 공격 애니메이션
	case MSG_PLAYER_ATTACK_ANIMATION:
		SC_Player_Attack_Animation( a_pk );
		break;

	// 몬스터 공격 애니메이션
	case MSG_MONSTER_ATTACK_ANIMATION:
		SC_Monster_Attack_Animation( a_pk );
		break;

	// 몬스터 공격 애니메이션2
	case MSG_MONSTER_ATTACK_ANIMATION2:
		SC_Monster_Attack_Animation2( a_pk );
		break;

	// 몬스터 각도
	case MSG_MONSTER_LOCKON:
		SC_Monster_LockOn( a_pk );
		break;
	}
}


VOID CNetwork::UpdateLobby()
{
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

	LeaveCriticalSection( &m_cs );
}


VOID CNetwork::UpdateGame()
{	
	static FLOAT time = 0.0f;
	time += CFrequency::GetInstance()->getFrametime();

	// 이동 데이터 서버로 보내기
	if( time > NETWORK_RECV_TIME )
	{
		time = 0.0f;

		CObjectManage * pOM = CObjectManage::GetInstance();
		CCharactor& rChar	= pOM->Get_Charactor()[ pOM->Get_CharTable( pOM->Get_ClientNumber() ) ];

		D3DXVECTOR3 vPos	= rChar.Get_CharaPos();
		FLOAT fAngle		= rChar.Get_CharaAngle();

		CS_PLAYER_MOVEMENT( vPos.x, vPos.z, fAngle );
	}

	// 서버로 부터 받은 패킷들을 처리한다.
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

	LeaveCriticalSection( &m_cs );
}


UINT WINAPI RecvThread( LPVOID a_p )
{
	CNetwork* pNetwork = (CNetwork*)a_p;

	CHAR buff[BUF_SIZE];
	INT buffSize = 0;

	while( pNetwork->m_bStop != TRUE )
	{
		INT nSize = recv( pNetwork->m_socket, buff + buffSize, BUF_SIZE - buffSize, 0 );
		if( nSize == SOCKET_ERROR )
		{
#ifdef _DEBUG
			CDebugConsole::GetInstance()->Messagef( "recv Error :  \n", WSAGetLastError() );
#endif
			break;
		}

		// 접속 정료
		if( nSize == 0 )
		{
#ifdef _DEBUG
			CDebugConsole::GetInstance()->Messagef( "Disconnected \n" );
#endif
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
				
				buffSize -= wMsgSize;
				memmove( buff, buff + wMsgSize, buffSize );
			}
		}
	}

	return 0;
}