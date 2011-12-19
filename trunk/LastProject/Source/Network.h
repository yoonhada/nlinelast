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

		MSG_LOGON,
		MSG_INITDATA,
		MSG_CHANGE_HOST,
		MSG_ADD_USER,
		MSG_READY,
		MSG_START_BUTTON_STATE,
		MSG_GAME_START,
		MSG_LODING_COMPLETE,
		MSG_CHANGE_SCENE,
		MSG_CLIENT_DISCONNECT,
		MSG_SERVER_CLOSE,

		MSG_EVENT_STATE,
		MSG_EVENT_COMBO_INFO,
		MSG_EVENT_COMBO_SLOT_STATE,
		MSG_EVENT_COMBO_RESULT,

		MSG_CHAT,
		MSG_PLAYER_MOVE,
		MSG_MONSTER_MOVE,

		MSG_UTOM_ATTACK,
		MSG_MTOU_ATTACK,
		MSG_EVENT_ATTACK,

		MSG_PLAYER_ATTACK_ANIMATION,
		MSG_MONSTER_ATTACK_ANIMATION,
		MSG_MONSTER_ATTACK_ANIMATION2,
		MSG_MONSTER_LOCKON,

		MSG_GAME_RESULT,

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
	virtual ~CNetwork();


public:
	BOOL CreateSocket();
	VOID Close();

	BOOL ConnectToServer( CHAR* a_szIP, WORD a_wPort );

	// Server -> Client
	VOID SC_LOGON( CPacket& a_pk );
	VOID SC_INIT( CPacket& a_pk );
	VOID SC_CHANGE_HOST( CPacket& a_pk );
	VOID SC_ADD_USER( CPacket& a_pk );
	VOID SC_READY( CPacket& a_pk );
	VOID SC_ENABLE_START( CPacket& a_pk );
	VOID SC_GAME_START( CPacket& a_pk );
	VOID SC_LODING_COMPLETE( CPacket& a_pk );
	VOID SC_CLIENT_DISCONNECT( CPacket& a_pk );
	VOID SC_SERVER_CLOSE( CPacket& a_pk );

	VOID SC_EVENT_STATE( CPacket& a_pk );
	VOID SC_EVENT_COMBO_INFO( CPacket& a_pk );
	VOID SC_EVENT_COMBO_SLOT_STATE( CPacket& a_pk );
	VOID SC_EVENT_COMBO_RESULT( CPacket& a_pk );

	VOID SC_CHAT( CPacket& a_pk );
	VOID SC_PLAYER_MOVEMENT( CPacket& a_pk );
	VOID SC_MONSTER_MOVEMENT( CPacket& a_pk );
	VOID SC_UTOM_ATTACK( CPacket& a_pk );
	VOID SC_MTOU_ATTACK( CPacket& a_pk );
	VOID SC_EVENT_ATTACK( CPacket& a_pk );

	VOID SC_Player_Attack_Animation( CPacket& a_pk );
	VOID SC_Monster_Attack_Animation( CPacket& a_pk );
	VOID SC_Monster_Attack_Animation2( CPacket& a_pk );
	VOID SC_Monster_LockOn( CPacket& a_pk );

	VOID SC_GAME_RESULT( CPacket& a_pk );

	// Client -> Server
	VOID CS_LOGON( LPWSTR a_szNickName );
	VOID CS_READY( WORD a_wSelect, BOOL a_bSelect );
	VOID CS_GAME_START();
	VOID CS_LODING_COMPLETE();
	VOID CS_CLIENT_DISCONNECT();

	VOID CS_EVENT_STATE( WORD a_wEventID );
	VOID CS_EVENT_COMBO( INT* a_iEventKind );
	VOID CS_EVENT_COMBO_SLOT_STATE( WORD a_wSlotNumber, BOOL a_bState );
	VOID CS_EVENT_COMBO_RESULT( BOOL a_bResult );

	VOID CS_CHAT( LPWSTR a_szText );
	VOID CS_PLAYER_MOVEMENT( CONST FLOAT& a_fX, CONST FLOAT& a_fZ, CONST FLOAT& a_fAngle );
	VOID CS_MONSTER_MOVEMENT( WORD a_iMonsterNumber, PathNode* a_pPath );
	VOID CS_UTOM_ATTACK( WORD a_wMonsterNumber, D3DXVECTOR3 a_vDirection, WORD a_wTotalParts, WORD a_wDestroyPart[], WORD a_wDestroyCount[], std::vector<WORD>& a_pList );
	VOID CS_MTOU_ATTACK( D3DXVECTOR3 a_vDirection, WORD cDestroyCount, std::vector<WORD>& a_pList );
	VOID CS_EVENT_ATTACK( WORD a_wMonsterNumber, D3DXVECTOR3 a_vDirection, WORD a_wDestroyCount, std::vector<WORD>& a_pList );

	VOID CS_Player_Attack_Animation( WORD a_wAnimationNumber );
	VOID CS_Monster_Attack_Animation( WORD a_wMonsterNumber, WORD a_wAnimationNumber );
	VOID CS_Monster_Attack_Animation2( WORD a_wMonsterNumber, WORD a_wAnimationNumber, FLOAT a_fDegree, D3DXVECTOR3 a_vPos, D3DXVECTOR3 a_vNextPos, FLOAT a_fDistance );
	VOID CS_Monster_LockOn( WORD a_wMonsterNumber, FLOAT a_fAngle );

	VOID CS_GAME_RESULT();

	BOOL SendToServer( CPacket& a_pk );

	VOID InsertPacket( CPacket& a_pk );
	VOID ProcessPacket( CPacket& a_pk );

	VOID UpdateLobby();
	VOID UpdateGame();

	friend static UINT WINAPI RecvThread( LPVOID a_p );
};


#endif