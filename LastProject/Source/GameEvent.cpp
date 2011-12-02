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
, m_pPosition( 0 )
{
	Clear();
}

CGameEvent::CGameEvent(INT nMaxChar)
: m_iMaxCharaNum ( nMaxChar )
, m_pPosition( 0 )
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
	
	SAFE_DELETE_ARRAY( m_pPosition );
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

	SAFE_DELETE_ARRAY( m_pPosition );
	m_pPosition = new D3DXVECTOR3[4];
	m_pPosition[0] = D3DXVECTOR3(-100.0f, 0.0f, 650.0f);
	m_pPosition[1] = D3DXVECTOR3(-120.0f, 0.0f, 650.0f);
	m_pPosition[2] = D3DXVECTOR3(-140.0f, 0.0f, 650.0f);
	m_pPosition[3] = D3DXVECTOR3(-160.0f, 0.0f, 650.0f);
}

VOID CGameEvent::Update()
{
	if (!m_bHost)
	{
		return;
	}

	m_pShotedPoint[0] += CTree::GetInstance()->GetCharAtkVector()->size();
	m_nHPMonstor = CTree::GetInstance()->GetMonsAtkVector()->size();
}

VOID CGameEvent::Render()
{
}

D3DXVECTOR3& CGameEvent::GetDefaultCharPosition( INT nClient )
{
	return m_pPosition[ nClient ];
}