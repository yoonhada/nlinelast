/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once
class CMainScene;

class CGameEvent : public IObject
{
private:
	enum EVENTKIND{
		NONE = -1, INIT, 
	};
	typedef struct _EVENT
	{
		INT nKind;
		FLOAT fTime;

		_EVENT(INT n, FLOAT f) : nKind(n), fTime(f) {}
	} EVENT;

	// Host
	BOOL m_bHost;
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

	VOID AddEvent( EVENTKIND, FLOAT );

	VOID EventInit();
public:
	CGameEvent();
	CGameEvent( INT, CMainScene *);
	virtual ~CGameEvent();

	VOID Clear();
	HRESULT Create();	///< 초기화
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render

	// Get
	D3DXVECTOR3& GetDefaultCharPosition( INT nClient );
};
