/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	�����̺�Ʈ
*/

#pragma once
class CMainScene;
class CGameEventCombo;

class CGameEvent : public IObject
{
private:
	typedef struct _EVENT
	{
		INT nKind;
		FLOAT fTime;

		_EVENT(INT n, FLOAT f) : nKind(n), fTime(f) {}
	} EVENT;

	// Host
	BOOL m_bHost;
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
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
	BOOL m_bEventCombo;
	//INT m_nEvent;

	CGameEventCombo * m_pEventCombo;

	VOID EventInit();
	VOID EventCamera();
	VOID EventCombo();
	VOID EventDestoryCombo();
	VOID EventFirstAidKit();
public:
	enum EVENTKIND{
		NONE = -1, INIT, EVENTCAMERA, EVENTCOMBO, EVENTDESTORYCOMBO, EVENTFAK,
	};

	CGameEvent();
	CGameEvent( INT, CMainScene *);
	virtual ~CGameEvent();

	VOID Clear();
	HRESULT Create();	///< �ʱ�ȭ
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();	///< ������Ʈ�� ����
	VOID Update();		///< Update
	VOID Render();		///< Render

	VOID AddEvent( EVENTKIND, FLOAT );
};
