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
	D3DXVECTOR3 * m_pCharactorPosition;
	D3DXVECTOR3 * m_pMonsterPosition;

	// Char
	INT m_iMaxCharaNum;
	INT * m_pAttackPoint;
	INT * m_pShotedPoint;

	// Monster
	INT m_nMonsterState;
	
	// Event
	INT m_nMonsterIndex;
	INT m_nPlayerIndex;
	std::list<EVENT*> m_listEvent;
	std::list <EVENT*>::iterator Iter;

	CGameEvent();
	virtual ~CGameEvent();

public:
	enum EventKind {
		NONE = -1, INIT, 
		EVENTCAMERA, 
		EVENTCOMBO, EVENTDESTORYCOMBO, 
		EVENTFAK,
		TUTORIALATK, TUTORIALATKEND, 
		TUTORIALCOMBO, TUTORIALCOMBOEND, 
	};

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, INT nClient );
	HRESULT Release();	///< 오브젝트들 제거
	INT Update();		///< Update

	VOID AddEvent( INT, FLOAT );

	// Get
	INT GetMonsterIndex()						{ return m_nMonsterIndex; }
	INT GetPlayerIndex()						{ return m_nPlayerIndex; }
	INT GetMonsterState()						{ return m_nMonsterState; }
	D3DXVECTOR3& GetCharPosition(INT nIndex)	{ return m_pCharactorPosition[nIndex]; }
	D3DXVECTOR3& GetMonsPosition(INT nIndex)	{ return m_pMonsterPosition[nIndex]; }

	// Set
	VOID Set_PlayerIndex( INT a_nPlayerIndex )	{ m_nPlayerIndex = a_nPlayerIndex; }
	VOID Set_MonsterIndex( INT a_nMonsterIndex ){ m_nMonsterIndex = a_nMonsterIndex; }
};
