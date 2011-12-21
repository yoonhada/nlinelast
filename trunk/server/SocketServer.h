#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#include "stdafx.h"
#include "NTClient.h"


class CSocketServer
{
public:
	SOCKET	m_ListenSocket;

	HANDLE	m_hCompletionPort;
	HANDLE	m_hAcceptThread;
	HANDLE	m_hIOCPThread;

	CNTClient* m_pHostClient;
	BOOL	m_bExistHost;
	BOOL	m_bGameStart;
	INT		m_iClientCount;
	INT		m_iReadyCount;
	INT		m_iLodingCompleteCount;
	WORD	m_wUserNumber[4];
	BOOL	m_bSelected[4];
	map<WORD, CNTClient*> m_Map_LogonClients;


public:
	CSocketServer();
	virtual ~CSocketServer();

	BOOL Initialize();
	VOID Clear();
	BOOL StartServer( WORD a_wPort );
	VOID Stop();

	VOID UpdateFrame();

	VOID OnAccept( SOCKET a_hSocket );
	VOID OnServerClose();
	VOID OnClientClose( CNTClient* a_pClient );

	VOID CS_LOGON( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_READY( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_GAME_START( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_LODING_COMPLETE( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_CLIENT_DISCONNECT( CNTClient* a_pClient, CPacket& a_pk );

	VOID CS_EVENT_STATE( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_EVENT_COMBO_INFO( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_EVENT_COMBO_SLOT_STATE( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_EVENT_COMBO_RESULT( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_EVENT_HEAL( CNTClient* a_pClient, CPacket& a_pk );

	VOID CS_CHAT( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_PLAYER_MOVE( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_MONSTER_MOVE( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_UTOM_ATTACK( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_MTOU_ATTACK( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_EVENT_ATTACK( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_PLAYER_ATTACK_ANIMATION( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_MONSTER_ATTACK_ANIMATION( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_MONSTER_ATTACK_ANIMATION2( CNTClient* a_pClient, CPacket& a_pk );
	VOID CS_MONSTER_LockOn( CNTClient* a_pClient, CPacket& a_pk );

	VOID CS_GAME_RESULT( CNTClient* a_pClient, CPacket& a_pk );

	VOID SC_INIT( CNTClient* a_pClient );
	VOID SC_CLIENT_DISCONNECT( CNTClient* a_pClient );

	VOID SendToClient( CNTClient* a_pClient, CPacket& a_pk );

	VOID ProcessPacket( CNTClient* a_pClient, CPacket& a_pk );


	static UINT WINAPI AcceptProc( VOID* p );
	static UINT WINAPI IOCPWorkerProc( VOID* p );
};


#endif