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
	typedef struct _EVENT
	{
		FLOAT fTime;
		INT nKind;
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
	std::list<EVENT*> m_ListEvent;
	EVENT * m_pCurrentEvent;

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
