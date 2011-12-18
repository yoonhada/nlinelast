/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	�����̺�Ʈ
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

	return S_OK;
}

HRESULT CGameEvent::Release()
{
	SAFE_DELETE ( m_pAttackPoint );
	SAFE_DELETE ( m_pShotedPoint );
	
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

	m_nMonstersState = NONE ;

	m_nPrevEvent = NONE;
	m_nTutorial = -1;
	m_nMonsterIndex = -1;
	m_nPlayerIndex = -1;

	m_vNonePosition = D3DXVECTOR3( -100.0f, 0.0f, -250.0f );

	m_vCharactorPosition[0][0] = D3DXVECTOR3( 200.0f, 0.0f,-505.0f );
	m_vCharactorPosition[0][1] = D3DXVECTOR3( 225.0f, 0.0f,-505.0f );
	m_vCharactorPosition[0][2] = D3DXVECTOR3( 250.0f, 0.0f,-505.0f );
	m_vCharactorPosition[0][3] = D3DXVECTOR3( 275.0f, 0.0f,-505.0f );
	m_vCharactorPosition[1][0] = D3DXVECTOR3( 190.0f, 0.0f, -90.0f );
	m_vCharactorPosition[1][1] = D3DXVECTOR3( 215.0f, 0.0f, -90.0f );
	m_vCharactorPosition[1][2] = D3DXVECTOR3( 240.0f, 0.0f, -90.0f );
	m_vCharactorPosition[1][3] = D3DXVECTOR3( 265.0f, 0.0f, -90.0f );

	//m_vMonsterPosition[0] = D3DXVECTOR3( 225.0f, 0.0f,-505.0f );
	//m_vMonsterPosition[1] = D3DXVECTOR3( 250.0f, 0.0f,-505.0f );

	m_vMonsterPosition[0] = D3DXVECTOR3( 120.0f, 0.0f, 180.0f);
	m_vMonsterPosition[1] = D3DXVECTOR3( 170.0f, 0.0f, 180.0f);
	m_vMonsterPosition[2] = D3DXVECTOR3( -40.0f, 0.0f, 660.0f);

	m_vWallPosition[0] = D3DXVECTOR3( 183.4534f, 0.0f, -202.7f );
	m_vWallPosition[1] = D3DXVECTOR3( 241.4534f, 0.0f, -202.7f );
	m_vWallPosition[2] = D3DXVECTOR3( 298.4534f, 0.0f, -202.7f );

}

VOID CGameEvent::ClearEvent( )
{
	for ( Iter = m_listEvent.begin(); Iter != m_listEvent.end( ); Iter++ )
	{
		SAFE_DELETE( (*Iter) );
	}

	m_listEvent.erase( m_listEvent.begin(), m_listEvent.end() );
}

VOID CGameEvent::ClearCombo()
{
	Iter = m_listEvent.begin();

	while ( Iter != m_listEvent.end() )
	{
		if ( EVENT_COMBO <= (*Iter)->nKind &&  (*Iter)->nKind <= EVENT_COMBO_END )
		{
			delete *Iter;
			*Iter = NULL;
			//SAFE_DELETE( (*Iter) );
			Iter = m_listEvent.erase( Iter );
		}
		else
		{
			Iter++;
		}
	}
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

VOID CGameEvent::IndexInit()
{
	m_nMonsterIndex = -1;
	m_nPlayerIndex = -1;
}

INT CGameEvent::Update()
{
	INT nEvent = NONE;
	if ( !m_listEvent.empty() )
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
			m_nPrevEvent = nEvent;

			SAFE_DELETE ( *Iter );
			Iter = m_listEvent.erase( Iter );
		}
	}

	return nEvent;
}

VOID CGameEvent::SetAttackPoint( INT nChar, INT nCount )
{

}

VOID CGameEvent::SetShotedPoint( INT nChar, INT nCount )
{

}
