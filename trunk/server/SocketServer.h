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
	VOID OnClientClose( CNTClient* pClient );

	VOID CS_Logon( CNTClient* pClient, CPacket& pk );
	VOID CS_SELECT_CHARACTER( CNTClient* pClient, CPacket& a_pk );
	VOID CS_READY( CNTClient* pClient, CPacket& a_pk );
	VOID CS_GAME_START( CNTClient* pClient, CPacket& a_pk );
	VOID CS_Chat( CNTClient* pClient, CPacket& pk );
	VOID CS_Player_Move( CNTClient* pClient, CPacket& pk );
	VOID CS_Monster_Move( CNTClient* pClient, CPacket& pk );
	VOID CS_UTOM_Attack( CNTClient* pClient, CPacket& pk );
	VOID CS_MTOU_Attack( CNTClient* pClient, CPacket& pk );
	VOID CS_Attack_Animation( CNTClient* pClient, CPacket& pk );

	VOID SC_INITDATA( CNTClient* pClient );
	VOID SC_Disconnect( CNTClient* pClient );

	VOID SendToClient( CNTClient* a_pClient, CPacket& a_pk );

	VOID ProcessPacket( CNTClient* pClient, CPacket& pk );


	static UINT WINAPI AcceptProc( VOID* p );
	static UINT WINAPI IOCPWorkerProc( VOID* p );
};


#endif