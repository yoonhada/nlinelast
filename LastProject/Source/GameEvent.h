/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once

class CGameEvent : public IObject
{
public:
	// Host
	BOOL m_bHost;

	// Char
	INT m_iMaxCharaNum;
	INT * m_pAttackPoint;
	INT * m_pShotedPoint;

	// Monster
	INT m_nHPMonstor;
	
	// Event
	INT m_nGameEvent;

public:
	CGameEvent();
	CGameEvent( INT );
	virtual ~CGameEvent();

	VOID Clear();
	HRESULT Create();	///< 초기화
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render
};
