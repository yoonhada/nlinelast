/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "GameEvent.h"
#include "GameEventCombo.h"

#include "MainScene.h"
#include "Monster.h"
#include "Charactor.h"
#include "Weapon.h"
#include "Seek.h"
#include "Camera.h"
#include "TimeLifeItem.h"

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
	return S_OK;
}

HRESULT CGameEvent::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;
	m_bHost = CObjectManage::GetInstance()->IsHost();
	m_pAttackPoint = new INT[4];
	m_pShotedPoint = new INT[4];
	
	AddEvent( INIT, 1.0f );
	AddEvent( EVENTCAMERA, 0.1f );
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
	m_pPosition[0] = D3DXVECTOR3( 180.0f, 0.0f, -330.0f);
	m_pPosition[1] = D3DXVECTOR3( 220.0f, 0.0f, -330.0f);
	m_pPosition[2] = D3DXVECTOR3( 260.0f, 0.0f, -330.0f);
	m_pPosition[3] = D3DXVECTOR3( 300.0f, 0.0f, -330.0f);

	m_pEventCombo = NULL;
	m_bEventCombo = FALSE;
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

VOID CGameEvent::Update()
{
	INT nEvent = NONE;

	if (!m_listEvent.empty())
	{
		m_pCurrentEvent = m_listEvent.front();
	}			

	if ( m_bHost )
	{
		Iter = m_listEvent.begin();

		while ( Iter != m_listEvent.end() )
		{
			(*Iter)->fTime -= CFrequency::GetInstance()->getFrametime();
			Iter++;
		}

		if ( m_pCurrentEvent != NULL)
		{
			m_pCurrentEvent->fTime -= CFrequency::GetInstance()->getFrametime();
			if ( m_pCurrentEvent->fTime < 0.0f )
			{
				nEvent = m_pCurrentEvent->nKind;
				m_listEvent.pop_front();
				SAFE_DELETE( m_pCurrentEvent );
			}
		}


		// ComboEvent
		if ( m_pEventCombo )
		{
			BOOL bCheck = m_pEventCombo->CheckKindEvent( CObjectManage::GetInstance()->Get_LastAtkPlayer() );

			if ( bCheck == FALSE )
			{
				AddEvent( EVENTDESTORYCOMBO, 0.1f );
			}
			else
			{
				if ( m_pEventCombo->GetKindIndex() == 4 )
				{
					m_pEventCombo->ReSetKindIndex();
				}
			}
		}
	}
	else
	{
		// host network....  받기.
	}

	switch ( nEvent )
	{
	case INIT:
		CDebugConsole::GetInstance()->Message("Game Event : INIT \n");
		EventInit();
		break;
	case EVENTCAMERA:
		CDebugConsole::GetInstance()->Message("Game Event : EVENTCAMERA \n");
#ifndef _DEBUG
		EventCamera();
#endif // _DEBUG
		break;
	case EVENTCOMBO:
		if (m_bEventCombo == TRUE)
			break;
		CDebugConsole::GetInstance()->Message("Game Event : EVENTCOMBO \n");
		EventCombo();
		m_bEventCombo = TRUE;
		AddEvent( EVENTDESTORYCOMBO, 20.0f );
		break;
	case EVENTDESTORYCOMBO:
		CDebugConsole::GetInstance()->Message("Game Event : EVENTDESTORYCOMBO \n");
		EventDestoryCombo();
		m_bEventCombo = FALSE;
		break;
	case EVENTFAK:
		CDebugConsole::GetInstance()->Message("Game Event : EVENTFAK \n");
		EventFirstAidKit();
		break;
	default:
		break;
	}
}

VOID CGameEvent::Render()
{
	if( m_pEventCombo )
	{
		m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pEventCombo->Render();
		m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

VOID CGameEvent::EventInit()
{
	// 케릭터 위치 조정
	m_pScen->m_pCharactors[0].Set_Position( m_pPosition[0] );	
	m_pScen->m_pCharactors[1].Set_Position( m_pPosition[1] );	
	m_pScen->m_pCharactors[2].Set_Position( m_pPosition[2] );	
	m_pScen->m_pCharactors[3].Set_Position( m_pPosition[3] );	
	
	// 몬스터 위치 조정
	m_pScen->m_pMonster->Set_Pos( D3DXVECTOR3(-130.0f, 0.0f, 660.0f) );
	m_pScen->m_pMonster->Set_Angle( 0.0f );
	m_pScen->m_pMonster->ChangeAnimation( CMonster::ANIM_STAND );
	m_pScen->m_pMonster->EnableShadow( TRUE );

	// AI 초기화
	if( CObjectManage::GetInstance()->IsHost() == TRUE )
	{
		m_pScen->m_pMonster->GetFSM()->SetCurrentState( Seek::GetInstance() );
	}

	// Game Point
	m_pAttackPoint[0] = m_pAttackPoint[1] = m_pAttackPoint[2] = m_pAttackPoint[3] = 0;
	m_pShotedPoint[0] = m_pShotedPoint[1] = m_pShotedPoint[2] = m_pShotedPoint[3] = 0;
}


VOID CGameEvent::EventCamera()
{
	m_pScen->m_pCamera->SetEffect( 3 );
}

VOID CGameEvent::EventCombo()
{
	if ( !m_pEventCombo )
	{
		CObjectManage * pOM = CObjectManage::GetInstance();
		m_pEventCombo = new CGameEventCombo( m_pD3dDevice, CObjectManage::GetInstance()->GetSprite() );
		INT nClient = 0;
		// 접속 클라이언트 찾기.
		for ( int i = 0; i < 4; ++i )
		{
			if (pOM->Get_CharTable()[i] != -1 )
			{
				nClient++;
			}
		}

		INT nSelect;
		for ( int i = 0; i < 4; ++i )
		{
			nSelect = static_cast<int>( FastRand2() * nClient );
			m_pEventCombo->AddCombo( i, pOM->Get_CharTable()[nSelect] + 1 );
		}		

		m_pEventCombo->Create();
	}
}

VOID CGameEvent::EventDestoryCombo()
{
	SAFE_DELETE( m_pEventCombo );
}

VOID CGameEvent::EventFirstAidKit()
{
	m_pScen->m_pFirstAidKit->SetActive( TRUE );
}