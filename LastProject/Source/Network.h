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
/*
		MSG_START = 0,
		MSG_CS_LOGON,
		MSG_CS_CHAT,
		MSG_CS_MOVE,
		MSG_MOVE,
		MSG_UTOM_ATTACK,
		MSG_MTOU_ATTACK,
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
*/
		MSG_START = 0,

		MSG_LOGON,
		MSG_INITDATA,
		MSG_CHANGE_HOST,
		MSG_NEWUSER,
		MSG_READY,
		MSG_ENABLE_START,
		MSG_GAME_START,
		MSG_DISCONNECT,

		MSG_CHAT,
		MSG_PLAYER_MOVE,
		MSG_MONSTER_MOVE,

		MSG_UTOM_ATTACK,
		MSG_MTOU_ATTACK,

		MSG_ATTACK_ANIMATION,

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

	// Server -> Client
	VOID SC_LOGON( CPacket& a_pk );
	VOID SC_InitData( CPacket& a_pk );
	VOID SC_NEWUSER( CPacket& a_pk );
	VOID SC_CHANGE_HOST( CPacket& a_pk );
	VOID SC_READY( CPacket& a_pk );
	VOID SC_ENABLE_START( CPacket& a_pk );
	VOID SC_GAME_START( CPacket& a_pk );

	VOID SC_CHAT( CPacket& a_pk );
	VOID SC_PLAYER_MOVEMENT( CPacket& a_pk );
	VOID SC_MONSTER_MOVEMENT( CPacket& a_pk );
	VOID SC_UTOM_ATTACK( CPacket& a_pk );
	VOID SC_MTOU_ATTACK( CPacket& a_pk );
	VOID SC_DISCONNECT( CPacket& a_pk );

	VOID SC_Attack_Animation( CPacket& a_pk );

	// Client -> Server
	VOID CS_LOGON( LPWSTR a_szNickName );
	VOID CS_READY( WORD a_wSelect, BOOL a_bSelect );
	VOID CS_GAME_START();

	VOID CS_CHAT( LPWSTR a_szText );
	VOID CS_PLAYER_MOVEMENT( CONST FLOAT& a_fX, CONST FLOAT& a_fZ, CONST FLOAT& a_fAngle );
	VOID CS_MONSTER_MOVEMENT( WORD a_iMonsterNumber, PathNode* a_pPath );
	VOID CS_UTOM_ATTACK( D3DXVECTOR3 a_vDirection, WORD a_wTotalParts, WORD a_wDestroyPart[], WORD a_wDestroyCount[], std::vector<WORD>& a_pList );
	VOID CS_MTOU_ATTACK( D3DXVECTOR3 a_vDirection, WORD cDestroyCount, std::vector<WORD>& a_pList );

	VOID CS_Attack_Animation( WORD a_wAnimationNumber );
	

	VOID UpdateLobby();
	VOID UpdateGame();

	BOOL SendToServer( CPacket& a_pk );

	VOID InsertPacket( CPacket& a_pk );
	VOID ProcessPacket( CPacket& a_pk );

	friend static UINT WINAPI RecvThread( LPVOID a_p );
};


#endif