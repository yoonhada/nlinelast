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
		MSG_CS_UTOM_ATTACK_ANIMATION,
		MSG_CS_MTOU_ATTACK_ANIMATION,
		///
		///
		MSG_SC_LOGON = 100,
		MSG_SC_INITDATA,
		MSG_SC_NEWUSER,
		MSG_SC_CHAT,
		MSG_SC_MOVE,
		MSG_SC_UTOM_ATTACK,
		MSG_SC_MTOU_ATTACK,
		MSG_SC_UTOM_ATTACK_ANIMATION,
		MSG_SC_MTOU_ATTACK_ANIMATION,
		MSG_SC_DISCONNECT,
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

	CRITICAL_SECTION m_cs;

	std::vector<CPacket> m_VectorPackets;

	D3DXVECTOR3 m_vMove;

private:
	CNetwork();
	~CNetwork();

public:

	BOOL CreateSocket();
	VOID Close();

	BOOL ConnectToServer( CHAR* a_szIP, WORD a_wPort );

	VOID SC_LOGON( CPacket& a_pk );
	VOID SC_InitData( CPacket& a_pk );
	VOID SC_CHAT( CPacket& a_pk );
	VOID SC_MOVEMENT( CPacket& a_pk );
	VOID SC_NEWUSER( CPacket& a_pk );
	VOID SC_UTOM_ATTACK( CPacket& a_pk );
	VOID SC_MTOU_ATTACK( CPacket& a_pk );
	VOID SC_DISCONNECT( CPacket& a_pk );

	VOID SC_UTOM_Attack_Animation( CPacket& a_pk );

	VOID CS_LOGON();
	VOID CS_CHAT();
	VOID CS_MOVEMENT( CONST FLOAT& a_fX, CONST FLOAT& a_fZ, CONST FLOAT& a_fAngle );
	VOID CS_UTOM_ATTACK( D3DXVECTOR3 a_vDirection, WORD a_wTotalParts, WORD a_wDestroyPart[], WORD a_wDestroyCount[], std::vector<WORD>& a_pList );
	VOID CS_MTOU_ATTACK( CHAR cDestroyPart, WORD cDestroyCount, std::vector<WORD>& a_pList, D3DXVECTOR3 a_vDirection );

	VOID CS_UTOM_Attack_Animation( WORD a_wAnimationNumber );
	

	VOID Update();

	BOOL SendToServer( CPacket& a_pk );

	VOID InsertPacket( CPacket& a_pk );
	VOID ProcessPacket( CPacket& a_pk );

	friend static UINT WINAPI RecvThread( LPVOID a_p );
};


#endif