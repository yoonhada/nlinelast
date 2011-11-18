#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#include "stdafx.h"
#include "NTClient.h"


class CSocketServer
{
public:
	SOCKET m_ListenSocket;

	HANDLE	m_hCompletionPort;
	HANDLE	m_hAcceptThread;
	HANDLE	m_hIOCPThread;

	WORD	m_wIndexList[4][2];
	WORD	m_wIndex;
	map<WORD, CNTClient*> m_Map_LogonClients;


public:
	CSocketServer();
	virtual ~CSocketServer();

	BOOL Initialize();
	BOOL StartServer( WORD Port );
	VOID Stop();

	VOID UpdateFrame();

	VOID OnAccept( SOCKET hSocket );
	VOID CS_Logon( CNTClient* pClient, CPacket& pk );
	VOID SC_INITDATA( CNTClient* pClient );
	VOID SC_NEWUSER( CNTClient* pClient, CNTClient* pOtherClientInfo );
	VOID CS_Chat( CNTClient* pClient, CPacket& pk );
	VOID SC_Chat( CNTClient* pClient, CPacket& pk );
	VOID CS_Move( CNTClient* pClient, CPacket& pk );
	VOID SC_Move( CNTClient* pClient, CPacket& pk );
	VOID CS_UTOM_Attack( CNTClient* pClient, CPacket& pk );
	VOID SC_UTOM_Attack( CNTClient* pClient, CPacket& pk );
	VOID CS_MTOU_Attack( CNTClient* pClient, CPacket& pk );
	VOID SC_MTOU_Attack( CNTClient* pClient, CPacket& pk );
	VOID CS_UTOM_Attack_Animation( CNTClient* pClient, CPacket& pk );
	VOID SC_UTOM_Attack_Animation( CNTClient* pClient, CPacket& pk );
	VOID SC_Disconnect( CNTClient* pClient );
	VOID ProcessPacket( CNTClient* pClient, CPacket& pk );

	VOID OnClientClose( CNTClient* pClient );


	static UINT WINAPI AcceptProc( VOID* p );
	static UINT WINAPI IOCPWorkerProc( VOID* p );
};


#endif