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

	// Char
	INT m_iMaxCharaNum;
	INT * m_pAttackPoint;
	INT * m_pShotedPoint;

	// Monster
	INT m_nMonstersState;
	
	// Event
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
		
		GAME_WIN_END, GAME_LOSE_END, 
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
		SC_EVENT_COMBO_SLOT_STATE, SC_EVENT_COMBO_RESULT
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
	INT GetPrevEvent()									{ return m_nPrevEvent; }
	INT GetTutorial()									{ return m_nTutorial; }
	INT GetMonsterIndex()								{ return m_nMonsterIndex; }
	INT GetPlayerIndex()								{ return m_nPlayerIndex; }
	INT GetMonsterState()								{ return m_nMonstersState; }

	D3DXVECTOR3& GetNonePosition( )							{ return m_vNonePosition; }
	D3DXVECTOR3& GetCharPosition( INT nType, INT nIndex )	{ return m_vCharactorPosition[nType][nIndex]; }
	D3DXVECTOR3& GetMonsPosition( INT nIndex )				{ return m_vMonsterPosition[nIndex]; }
	D3DXVECTOR3& GetWallPosition( INT nIndex )				{ return m_vWallPosition[nIndex]; }

	// Set
	VOID SetTutorial( INT a_nTutorial )			{ m_nTutorial = a_nTutorial; }
	VOID SetMonstersState( INT a_nMonstersState){ m_nMonstersState = a_nMonstersState; }
	VOID Set_PlayerIndex( INT a_nPlayerIndex )	{ m_nPlayerIndex = a_nPlayerIndex; }
	VOID Set_MonsterIndex( INT a_nMonsterIndex ){ m_nMonsterIndex = a_nMonsterIndex; }
	VOID SetAttackPoint( INT nChar, INT nCount );
	VOID SetShotedPoint( INT nChar, INT nCount );
	VOID SetSlotNumber( WORD wSlotNumber )		{ m_wSlotNumber = wSlotNumber; }
	VOID SetResult( BOOL bResult )				{ m_bResult = bResult; }

	INT GetAttackPoint( INT nChar )				{ return m_pAttackPoint[nChar]; }
	INT GetShotedPoint( INT nChar )				{ return m_pShotedPoint[nChar]; }
	WORD GetSlotNumber()						{ return m_wSlotNumber; }
	BOOL GetResult()							{ return m_bResult; }

};
