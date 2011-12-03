/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "GameEvent.h"

#include "MainScene.h"
#include "Monster.h"
#include "Charactor.h"
#include "Weapon.h"
#include "Seek.h"

using namespace std;

CGameEvent::CGameEvent( )
: m_iMaxCharaNum ( 4 )
{
	Clear();
}

CGameEvent::CGameEvent(INT nMaxChar, CMainScene * pScen )
: m_iMaxCharaNum ( nMaxChar )
, m_pScen( pScen )
{
	Clear();
}

CGameEvent::~CGameEvent()
{
	Release();
}

HRESULT CGameEvent::Create()
{
	m_bHost = CObjectManage::GetInstance()->IsHost();
	m_pAttackPoint = new INT[4];
	m_pShotedPoint = new INT[4];
	
	AddEvent( INIT, 0.0f );
	m_pCurrentEvent = m_listEvent.front();
	return S_OK;
}

HRESULT CGameEvent::Release()
{
	SAFE_DELETE ( m_pAttackPoint );
	SAFE_DELETE ( m_pShotedPoint );
	
	SAFE_DELETE_ARRAY( m_pPosition );

	for ( Iter = m_listEvent.begin(); Iter != m_listEvent.end( ); Iter++ )
	{
		SAFE_DELETE( (*Iter) );
	}
	m_listEvent.erase( m_listEvent.begin(), m_listEvent.end() );


	return S_OK;
}

VOID CGameEvent::Clear()
{
	m_pCurrentEvent = NULL;

	m_bHost = FALSE;
	m_iMaxCharaNum = 0;
	
	m_pAttackPoint = NULL;
	m_pShotedPoint = NULL;

	m_nHPMonstor = 0;

	m_pPosition = new D3DXVECTOR3[4];
	m_pPosition[0] = D3DXVECTOR3(-100.0f, 0.0f, 650.0f);
	m_pPosition[1] = D3DXVECTOR3(-120.0f, 0.0f, 650.0f);
	m_pPosition[2] = D3DXVECTOR3(-140.0f, 0.0f, 650.0f);
	m_pPosition[3] = D3DXVECTOR3(-160.0f, 0.0f, 650.0f);
}

VOID CGameEvent::Update()
{
	INT nEvent = NONE;
	if ( m_bHost && m_pCurrentEvent != NULL)
	{
		m_pCurrentEvent->fTime -= CFrequency::GetInstance()->getFrametime();
		if ( m_pCurrentEvent->fTime < 0.0f )
		{
			nEvent = m_pCurrentEvent->nKind;
			m_listEvent.pop_front();
			SAFE_DELETE( m_pCurrentEvent );
			if (!m_listEvent.empty())
			{
				m_pCurrentEvent = m_listEvent.front();
			}			
		}
	}

	switch ( nEvent )
	{
	case INIT:
		EventInit();
		break;
	default:
		break;
	}
}

VOID CGameEvent::Render()
{
}

D3DXVECTOR3& CGameEvent::GetDefaultCharPosition( INT nClient )
{
	return m_pPosition[ nClient ];
}

VOID CGameEvent::AddEvent( EVENTKIND enumKind, FLOAT fTime )
{
	BOOL bChk = FALSE;

	for ( Iter = m_listEvent.begin(); Iter != m_listEvent.end( ); Iter++ )
	{
		if ( (*Iter)->fTime > fTime )
		{
			m_listEvent.insert( Iter, new EVENT( enumKind, fTime ) );
			bChk = TRUE;
		}		
	}

	if ( !bChk )
	{
		m_listEvent.push_back( new EVENT( enumKind, fTime ) );
	}
}



VOID CGameEvent::EventInit()
{
	// Game Setting
	m_pScen->m_pMonster->Set_Pos( D3DXVECTOR3(-250.0f, 0.0f, 650.0f) );
	m_pScen->m_pMonster->Set_Angle( 0.0f );
	m_pScen->m_pMonster->Set_iSelectedFrameNum( 0 );
	m_pScen->m_pMonster->EnableShadow( TRUE );
	if( CObjectManage::GetInstance()->IsHost() == TRUE )
	{
		m_pScen->m_pMonster->GetFSM()->SetCurrentState( Seek::GetInstance() );
	}

	// Game Point
	for ( int i = 0; i < 4; ++i )
	{
		m_pAttackPoint[i] = 0;
		m_pShotedPoint[i] = 0;
	}
}