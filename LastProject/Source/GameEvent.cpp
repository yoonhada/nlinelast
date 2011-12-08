/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "MainScene.h"
#include "GameEvent.h"

using namespace std;

CGameEvent::CGameEvent( )
: m_iMaxCharaNum ( 4 )
{
	Clear();
}

CGameEvent::~CGameEvent()
{
	Release();
}

HRESULT CGameEvent::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, INT a_nMaxChar )
{
	m_pD3dDevice = a_pD3dDevice;
	m_iMaxCharaNum  = a_nMaxChar;

	m_bHost = CObjectManage::GetInstance()->IsHost();
	m_pAttackPoint = new INT[4];
	m_pShotedPoint = new INT[4];

	m_pAttackPoint[0] = m_pAttackPoint[1] = m_pAttackPoint[2] = m_pAttackPoint[3] = 0;
	m_pShotedPoint[0] = m_pShotedPoint[1] = m_pShotedPoint[2] = m_pShotedPoint[3] = 0;
	
	AddEvent( INIT, 1.0f );
	AddEvent( EVENT_CAMERA, 0.1f );
	return S_OK;
}

HRESULT CGameEvent::Release()
{
	SAFE_DELETE ( m_pAttackPoint );
	SAFE_DELETE ( m_pShotedPoint );
	
	SAFE_DELETE_ARRAY( m_pCharactorPosition );
	SAFE_DELETE_ARRAY( m_pMonsterPosition );

	for ( Iter = m_listEvent.begin(); Iter != m_listEvent.end( ); Iter++ )
	{
		SAFE_DELETE( (*Iter) );
	}
	m_listEvent.erase( m_listEvent.begin(), m_listEvent.end() );


	return S_OK;
}

VOID CGameEvent::Clear()
{
	m_bHost = FALSE;
	m_iMaxCharaNum = 0;
	
	m_pAttackPoint = NULL;
	m_pShotedPoint = NULL;

	m_nMonsterState = 3;

	m_nMonsterIndex = -1;
	m_nPlayerIndex = -1;


	m_pCharactorPosition = new D3DXVECTOR3[4];
	m_pCharactorPosition[0] = D3DXVECTOR3( 180.0f, 0.0f, -330.0f);
	m_pCharactorPosition[1] = D3DXVECTOR3( 220.0f, 0.0f, -330.0f);
	m_pCharactorPosition[2] = D3DXVECTOR3( 260.0f, 0.0f, -330.0f);
	m_pCharactorPosition[3] = D3DXVECTOR3( 300.0f, 0.0f, -330.0f);

	m_pMonsterPosition = new D3DXVECTOR3[3];
	m_pMonsterPosition[0] = D3DXVECTOR3(-100.0f, 0.0f, 660.0f);
	m_pMonsterPosition[1] = D3DXVECTOR3( -70.0f, 0.0f, 660.0f);
	m_pMonsterPosition[2] = D3DXVECTOR3( -40.0f, 0.0f, 660.0f);
}

VOID CGameEvent::AddEvent( INT nKind, FLOAT fTime )
{
	BOOL bChk = FALSE;

	Iter = m_listEvent.begin();
	while ( Iter != m_listEvent.end( ) )
	{
		if ( (*Iter)->fTime > fTime )
		{
			m_listEvent.insert( Iter, new EVENT( nKind, fTime ) );
			bChk = TRUE;
		}		

		Iter++;
	}

	if ( !bChk )
	{
		m_listEvent.push_back( new EVENT( nKind, fTime ) );
	}
}

INT CGameEvent::Update()
{
	INT nEvent = NONE;

	if ( m_bHost && !m_listEvent.empty() )
	{
		Iter = m_listEvent.begin();
		while ( Iter != m_listEvent.end() )
		{
			( *Iter )->fTime -= CFrequency::GetInstance()->getFrametime();
			Iter++;
		}

		Iter = m_listEvent.begin();
		if ( ( *Iter )->fTime < 0.0f )
		{
			nEvent = ( *Iter )->nKind;

			SAFE_DELETE ( *Iter );
			Iter = m_listEvent.erase( Iter );
		}
	}

	return nEvent;
}