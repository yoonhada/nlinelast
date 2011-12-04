/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once
class CMainScene;
class CGameEventCombo;

class CGameEvent : public IObject
{
private:
	enum EVENTKIND{
		NONE = -1, INIT, COMBO,
	};
	typedef struct _EVENT
	{
		INT nKind;
		FLOAT fTime;

		_EVENT(INT n, FLOAT f) : nKind(n), fTime(f) {}
	} EVENT;

	// Host
	BOOL m_bHost;
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	CMainScene * m_pScen;

	// Const 
	D3DXVECTOR3 * m_pPosition;

	// Char
	INT m_iMaxCharaNum;
	INT * m_pAttackPoint;
	INT * m_pShotedPoint;

	// Monster
	INT m_nHPMonstor;
	
	// Event
	std::list<EVENT*> m_listEvent;
	std::list <EVENT*>::iterator Iter;
	EVENT * m_pCurrentEvent;
	//INT m_nEvent;

	CGameEventCombo * m_pEventCombo;

	VOID AddEvent( EVENTKIND, FLOAT );

	VOID EventInit();
	VOID EventCombo();
public:
	CGameEvent();
	CGameEvent( INT, CMainScene *);
	virtual ~CGameEvent();

	VOID Clear();
	HRESULT Create();	///< 초기화
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render

	// Get
	D3DXVECTOR3& GetDefaultCharPosition( INT nClient );
};
