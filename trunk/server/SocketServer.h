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
	BOOL StartServer( WORD Port );
	VOID Stop();

	VOID UpdateFrame();

	VOID OnAccept( SOCKET hSocket );
	VOID OnServerClose();
	VOID OnClientClose( CNTClient* pClient );

	VOID CS_LOGON( CNTClient* pClient, CPacket& pk );
	VOID CS_READY( CNTClient* pClient, CPacket& a_pk );
	VOID CS_GAME_START( CNTClient* pClient, CPacket& a_pk );
	VOID CS_LODING_COMPLETE( CNTClient* pClient, CPacket& a_pk );
	VOID CS_CLIENT_DISCONNECT( CNTClient* pClient, CPacket& a_pk );

	VOID CS_EVENT_STATE( CNTClient* pClient, CPacket& a_pk );
	VOID CS_EVENT_COMBO( CNTClient* pClient, CPacket& a_pk );
	VOID CS_EVENT_COMBO_RESULT( CNTClient* pClient, CPacket& a_pk );

	VOID CS_CHAT( CNTClient* pClient, CPacket& pk );
	VOID CS_PLAYER_MOVE( CNTClient* pClient, CPacket& pk );
	VOID CS_MONSTER_MOVE( CNTClient* pClient, CPacket& pk );
	VOID CS_UTOM_ATTACK( CNTClient* pClient, CPacket& pk );
	VOID CS_MTOU_ATTACK( CNTClient* pClient, CPacket& pk );
	VOID CS_PLAYER_ATTACK_ANIMATION( CNTClient* pClient, CPacket& pk );
	VOID CS_MONSTER_ATTACK_ANIMATION( CNTClient* pClient, CPacket& pk );
	VOID CS_MONSTER_ATTACK_ANIMATION2( CNTClient* pClient, CPacket& pk );
	VOID CS_MONSTER_LockOn( CNTClient* pClient, CPacket& pk );

	VOID SC_INIT( CNTClient* pClient );
	VOID SC_CLIENT_DISCONNECT( CNTClient* pClient );

	VOID SendToClient( CNTClient* a_pClient, CPacket& a_pk );

	VOID ProcessPacket( CNTClient* pClient, CPacket& pk );


	static UINT WINAPI AcceptProc( VOID* p );
	static UINT WINAPI IOCPWorkerProc( VOID* p );
};


#endif