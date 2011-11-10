#ifndef _CNETWORK_H_
#define _CNETWORK_H_

//#include "stdafx.h"
class CPacket;

class CNetwork : public CSingleton<CNetwork>
{
	friend class CSingleton<CNetwork>;
public:
	enum MESSAGE_ID
	{
		MSG_START = 0,
		MSG_CS_LOGON,
		MSG_CS_CHAT,
		MSG_CS_MOVE,
		MSG_CS_UTOM_ATTACK,
		MSG_CS_MTOU_ATTACK,
		///
		///
		MSG_SC_LOGON = 100,
		MSG_SC_INITDATA,
		MSG_SC_NEWUSER,
		MSG_SC_CHAT,
		MSG_SC_MOVE,
		MSG_SC_UTOM_ATTACK,
		MSG_SC_MTOU_ATTACK,
		MSG_END,

	};

	enum MONSTER_PART
	{
		PART_HEAD = 0,
		PART_BODY,
		PART_TAIL,
		PART_LEFT_ARM,
		PART_LEFT_FOOT,
		PART_RIGHT_ARM,
		PART_RIGHT_FOOT,
	};

	SOCKET	m_socket;
	BOOL	m_bStop;
	HANDLE	m_hRecvThread;
	WORD	m_wNumber;

	CRITICAL_SECTION m_cs;

	std::vector<CPacket> m_VectorPackets;

	D3DXVECTOR3 m_vMove;

private:
	CNetwork();
	~CNetwork();

public:

	BOOL CreateSocket();
	VOID Close();

	BOOL ConnectToServer( CHAR* szIP, WORD Port );

	VOID scLOGON( CPacket& pk );
	VOID scInitData( CPacket& pk );
	VOID scCHAT( CPacket& pk );
	VOID scMOVE( CPacket& pk );
	VOID scNEWUSER( CPacket& pk );
	VOID SC_UTOM_ATTACK( CPacket& pk );

	VOID csLOGON();
	VOID csCHAT();
	VOID csMOVE( const FLOAT& x, const FLOAT& z, const FLOAT& angle );
	VOID CS_UTOM_ATTACK( CHAR cDestroyPart, WORD cDestroyCount, std::vector<WORD>& pList );

	VOID Update();

	BOOL SendToServer( CPacket& pk );

	VOID InsertPacket( CPacket& pk );
	VOID ProcessPacket( CPacket& pk );

	friend static UINT WINAPI RecvThread( LPVOID p );
};


#endif