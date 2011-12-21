/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "MainScene.h"
#include "GameEvent.h"
#include "Monster.h"

using namespace std;

CGameEvent::CGameEvent( )
: m_iMaxCharaNum ( 4 )
{
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
	m_vMonsterPosition[2] = D3DXVECTOR3( -100.0f, 0.0f, 680.0f);

	m_vWallPosition[0] = D3DXVECTOR3( 183.4534f, 0.0f, -202.7f );
	m_vWallPosition[1] = D3DXVECTOR3( 241.4534f, 0.0f, -202.7f );
	m_vWallPosition[2] = D3DXVECTOR3( 298.4534f, 0.0f, -202.7f );

	m_vItemPosition[0] = D3DXVECTOR3(- 65.8f, 0.0f, 208.5f );
	m_vItemPosition[1] = D3DXVECTOR3( 291.1f, 0.0f, 529.0f );
	m_vItemPosition[2] = D3DXVECTOR3(-336.0f, 0.0f, 488.0f );
	m_vItemPosition[3] = D3DXVECTOR3( 176.4f, 0.0f, 792.9f );
	m_vItemPosition[4] = D3DXVECTOR3(-177.8f, 0.0f, 803.8f );

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
	m_nDieMonsterNumber = -1;

	m_nPrevEvent = NONE;
	m_nScene = NONE;
	m_nTutorial = -1;
	m_nMonsterIndex = -1;
	m_nPlayerIndex = -1;


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
	SetMonsterLifeCheck();

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

VOID CGameEvent::SetMonsterLifeCheck()
{
	//m_nDieMonsterNumber
	CMonster ** pMonster = CObjectManage::GetInstance()->Get_Monster();

	if ( m_nMonstersState & CGameEvent::PANDA && m_nDieMonsterNumber == 0 )
	{
		pMonster[ 0 ]->BreakNockdown();
		pMonster[ 0 ]->ChangeAnimation( CMonster::ANIM_STAND );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			pMonster[ 0 ]->GetFSM()->SetCurrentState( NULL );
		}
	}	
	if ( m_nMonstersState & CGameEvent::BEAR && m_nDieMonsterNumber == 1 )
	{
		pMonster[ 1 ]->BreakNockdown();
		pMonster[ 1 ]->ChangeAnimation( CMonster::ANIM_STAND );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			pMonster[ 1 ]->GetFSM()->SetCurrentState( NULL );
		}
	}	
	if ( m_nMonstersState & CGameEvent::CLOWN && m_nDieMonsterNumber == 2 )
	{
		pMonster[ 2 ]->BreakNockdown();
		pMonster[ 2 ]->ChangeAnimation( CMonster::ANIM_STAND );
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			pMonster[ 2 ]->GetFSM()->SetCurrentState( NULL );
		}
		AddEvent( CGameEvent::GAME_WIN_END, 2.0f );
	}
}