#ifndef _CNETWORK_H_
#define _CNETWORK_H_

//#include "stdafx.h"
class CPacket;

class CNetwork
{
public:
	SOCKET	m_socket;
	BOOL	m_bStop;
	HANDLE	m_hRecvThread;

	CRITICAL_SECTION m_cs;

	std::vector<CPacket> m_VectorPackets;

	D3DXVECTOR3 m_vMove;


public:
	CNetwork();
	~CNetwork();

	BOOL CreateSocket();
	VOID Close();

	BOOL ConnectToServer( CHAR* szIP, WORD Port );

	VOID scLOGON( CPacket& pk );
	VOID scCHAT( CPacket& pk );
	VOID scMOVE( CPacket& pk );
	VOID scNEWUSER( CPacket& pk );

	VOID csLOGON();
	VOID csCHAT();
	VOID csMOVE( const FLOAT& x, const FLOAT& z, const FLOAT& angle );

	VOID Update();

	BOOL SendToSever( CPacket& pk );

	VOID InsertPacket( CPacket& pk );
	VOID ProcessPacket( CPacket& pk );

	friend static UINT WINAPI RecvThread( LPVOID p );
};


#endif