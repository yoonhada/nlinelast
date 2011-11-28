/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "GameEvent.h"

CGameEvent::CGameEvent( )
: m_iMaxCharaNum ( 4 )
{
	Clear();
}

CGameEvent::CGameEvent(INT nMaxChar)
: m_iMaxCharaNum ( nMaxChar )
{
	Clear();
}

CGameEvent::~CGameEvent()
{
	Release();
}

HRESULT CGameEvent::Create()
{
	m_pAttackPoint = new INT[4];
	m_pShotedPoint = new INT[4];

	m_pAttackPoint[0] = m_pAttackPoint[1] = m_pAttackPoint[2] = m_pAttackPoint[3] = 0;
	m_pShotedPoint[0] = m_pShotedPoint[1] = m_pShotedPoint[2] = m_pShotedPoint[3] = 0;
	
	return S_OK;
}

HRESULT CGameEvent::Release()
{
	SAFE_DELETE ( m_pAttackPoint );
	SAFE_DELETE ( m_pShotedPoint );

	return S_OK;
}

VOID CGameEvent::Clear()
{
	m_bHost = FALSE;
	m_iMaxCharaNum = 0;
	
	m_pAttackPoint = NULL;
	m_pShotedPoint = NULL;

	m_nHPMonstor = 0;
	m_nGameEvent = 0;
}

VOID CGameEvent::Update()
{
	if (!m_bHost)
	{
		return;
	}

	m_pShotedPoint[0] += CTree::GetInstance()->GetCharAtkVector()->size();
	m_nHPMonstor = CTree::GetInstance()->GetMonsAtkVector()->size();


	m_nGameEvent = FastRand2();
}

VOID CGameEvent::Render()
{
}