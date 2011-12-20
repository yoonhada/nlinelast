/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once

class CGameEvent : public CSingleton<CGameEvent>
{
	friend class CSingleton<CGameEvent>;
private:
	typedef struct _EVENT
	{
		INT nKind;
		FLOAT fTime;

		_EVENT(INT n, FLOAT f) : nKind(n), fTime(f) {}
	} EVENT;

	// Host
	BOOL m_bHost;
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

	// Const 
	D3DXVECTOR3 m_vNonePosition;
	D3DXVECTOR3 m_vCharactorPosition[2][4];
	D3DXVECTOR3 m_vMonsterPosition[3];
	D3DXVECTOR3 m_vWallPosition[3];
	D3DXVECTOR3 m_vItemPosition[5];

	// Char
	INT m_iMaxCharaNum;
	INT * m_pAttackPoint;
	INT * m_pShotedPoint;

	// Monster
	INT m_nMonstersState;
	INT m_nDieMonsterNumber;

	// Event
	INT m_nScene;
	INT m_nPrevEvent;
	INT m_nTutorial;
	INT m_nMonsterIndex;
	INT m_nPlayerIndex;
	std::list<EVENT*> m_listEvent;
	std::list <EVENT*>::iterator Iter;

	// CEvent
	WORD m_wSlotNumber;
	BOOL m_bResult;

	CGameEvent();
	virtual ~CGameEvent();

public:
	enum { ENUM_FAMILY_0	= 0, ENUM_FAMILY_1, ENUM_FAMILY_2, ENUM_FAMILY_3, ENUM_FAMILY_END }; 
	enum { ENUM_MONSTER_0	= 0, ENUM_MONSTER_1, ENUM_MONSTER_2, ENUM_MONSTER_END };
	enum { ENUM_DOOR_0		= 0, ENUM_DOOR_1, ENUM_DOOR_2, ENUM_DOOR_END };
	enum { ENUM_FAK_0		= 0, ENUM_FAK_1, ENUM_FAK_2, ENUM_FAK_3, ENUM_FAK_4, ENUM_FAK_END };

	enum MonstersName {
		EMPTY, 
		PANDA		= 0x0001, 
		BEAR		= 0x0002, 
		CLOWN		= 0x0004, 
		DOOR_LEFT	= 0x0010, 
		DOOR_CENTER	= 0x0020, 
		DOOR_RIGHT	= 0x0040, 
		ITEM_FAK1	= 0x0100, 
		ITEM_FAK2	= 0x0200, 
		ITEM_FAK3	= 0x0400, 
		ITEM_FAK4	= 0x0800, 
		ITEM_FAK5	= 0x1000, 
	};
	enum EventKind {
		NONE = 0, 
		GAME_HEALING, 
		GAME_WIN_END, GAME_LOSE_END, GAME_END, 
		EVENT_FAK, EVENT_FAK_END, 
		MONSTER_BREAK_NOCKDOWN, DOOR_BREAK_NOCKDOWN, 	
		EVENT_COMBO, EVENT_COMBO_SUCCESS, EVENT_COMBO_FAIL, EVENT_COMBO_END, 

		SCENE_TUTORIAL = 1000, 
		MAP_WALK, MAP_WALK_END,
		TUTORIAL_ATACK, TUTORIAL_ATACK_END, 
		TUTORIAL_COMBO, TUTORIAL_COMBO_SUCCESS, TUTORIAL_COMBO_FAIL, TUTORIAL_COMBO_END, 
		SCENE_TUTORIAL_END, 
		
		SCENE_BEAR = 2000, 
		SCENE_BEAR_END, 
		
		SCENE_CLOWN = 3000, 
		SCENE_CLOWN_END, 

		NETWORK_ENUM = 9000, 
		EVENT_COMBO_SLOTSTATE, EVENT_COMBO_RESULT, 
	};

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, INT nClient );
	HRESULT Release();	///< 오브젝트들 제거
	INT Update();		///< Update

	VOID AddEvent( INT, FLOAT );
	VOID ClearEvent();
	VOID ClearCombo();
	VOID IndexInit();

	// Get
	INT GetScene()					{ return m_nScene; }
	INT GetPrevEvent()				{ return m_nPrevEvent; }
	INT GetTutorial()				{ return m_nTutorial; }
	INT GetMonsterIndex()			{ return m_nMonsterIndex; }
	INT GetPlayerIndex()			{ return m_nPlayerIndex; }
	INT GetMonsterState()			{ return m_nMonstersState; }

	D3DXVECTOR3& GetNonePosition( )							{ return m_vNonePosition; }
	D3DXVECTOR3& GetCharPosition( INT nType, INT nIndex )	{ return m_vCharactorPosition[nType][nIndex]; }
	D3DXVECTOR3& GetMonsPosition( INT nIndex )				{ return m_vMonsterPosition[nIndex]; }
	D3DXVECTOR3& GetWallPosition( INT nIndex )				{ return m_vWallPosition[nIndex]; }
	D3DXVECTOR3& GetItemPosition( INT nIndex )				{ return m_vItemPosition[nIndex]; }

	// Set
	VOID SetScene(INT a_nScene )				{ m_nScene = a_nScene; }
	VOID SetTutorial( INT a_nTutorial )			{ m_nTutorial = a_nTutorial; }
	VOID SetMonstersState( INT a_nMonstersState){ m_nMonstersState = a_nMonstersState; }
	VOID Set_PlayerIndex( INT a_nPlayerIndex )	{ m_nPlayerIndex = a_nPlayerIndex; }
	VOID Set_MonsterIndex( INT a_nMonsterIndex ){ m_nMonsterIndex = a_nMonsterIndex; }
	VOID SetAttackPoint( INT nChar, INT nCount ){ m_pAttackPoint[nChar] += nCount; }
	VOID SetShotedPoint( INT nChar, INT nCount ){ m_pShotedPoint[nChar] -= nCount; }
	VOID SetSlotNumber( WORD wSlotNumber )		{ m_wSlotNumber = wSlotNumber; }
	VOID SetResult( BOOL bResult )				{ m_bResult = bResult; }
	VOID SetDie( INT nMonsterNumber )			{ m_nDieMonsterNumber = nMonsterNumber; }
	VOID SetMonsterLifeCheck();

	INT GetAttackPoint( INT nChar )				{ return m_pAttackPoint[nChar]; }
	INT GetShotedPoint( INT nChar )				{ return m_pShotedPoint[nChar]; }
	WORD GetSlotNumber()						{ return m_wSlotNumber; }
	BOOL GetResult()							{ return m_bResult; }
};
