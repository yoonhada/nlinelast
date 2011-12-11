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
	D3DXVECTOR3 m_vCharactorPosition[2][4];
	D3DXVECTOR3 m_vMonsterPosition[3];
	D3DXVECTOR3 m_vWallPosition;

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

	CGameEvent();
	virtual ~CGameEvent();

public:
	enum MonstersName {
		EMPTY, 
		PANDA		= 0x0001, 
		BEAR		= 0x0002, 
		CLOWN		= 0x0004, 
		DOOR		= 0x0008, 
		ITEM_FAK	= 0x0010, 
	};
	enum EventKind {
		NONE = -1, INIT, 
		EVENT_MAP_CAMERA_WALK, EVENT_MAP_CAMERA_WALK_END,
		EVENT_COMBO, EVENT_COMBO_END, EVENT_COMBO_SUCCESS, EVENT_COMBO_FAIL, 
		EVENT_FAK,
		TUTORIAL_ATACK, TUTORIAL_ATACK_END, 
		TUTORIAL_COMBO, TUTORIAL_COMBO_END, 
		MONSTER_BREAK_NOCKDOWN, 
	};

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, INT nClient );
	HRESULT Release();	///< 오브젝트들 제거
	INT Update();		///< Update

	VOID AddEvent( INT, FLOAT );
	VOID IndexInit();
	
	// Get
	INT GetPrevEvent()									{ return m_nPrevEvent; }
	INT GetTutorial()									{ return m_nTutorial; }
	INT GetMonsterIndex()								{ return m_nMonsterIndex; }
	INT GetPlayerIndex()								{ return m_nPlayerIndex; }
	INT GetMonsterState()								{ return m_nMonstersState; }

	D3DXVECTOR3& GetCharPosition( INT nType, INT nIndex )	{ return m_vCharactorPosition[nType][nIndex]; }
	D3DXVECTOR3& GetMonsPosition( INT nIndex )				{ return m_vMonsterPosition[nIndex]; }
	D3DXVECTOR3& GetWallPosition( INT nIndex )				{ return m_vWallPosition; }

	// Set
	VOID SetTutorial( INT a_nTutorial )			{ m_nTutorial = a_nTutorial; }
	VOID SetMonstersState( INT a_nMonstersState){ m_nMonstersState = a_nMonstersState; }
	VOID Set_PlayerIndex( INT a_nPlayerIndex )	{ m_nPlayerIndex = a_nPlayerIndex; }
	VOID Set_MonsterIndex( INT a_nMonsterIndex ){ m_nMonsterIndex = a_nMonsterIndex; }
};
