/**
@class	CGameEventCombo
@date	2011/12/5
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "GUIBackground.h"
#include "GameEventCombo.h"

CGameEventCombo::CGameEventCombo( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite ) 
: m_pD3dDevice( _pd3dDevice ), m_pSprite( _pSprite )
{
	Clear();
}

CGameEventCombo::~CGameEventCombo()
{
	Release();
}

HRESULT CGameEventCombo::Create( )
{
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIForground		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUICounting		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[0]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[1]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[2]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[3]		= new GUIBackground( m_pD3dDevice, m_pSprite );


	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width ) * 0.5f;
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height ) * 0.33f;

	GUIBase::IMAGEPARAM imgParam[7];

	FLOAT fWidBac, fWidFor, fHeiBac, fHeifor;
	fWidBac = 0.5f * 567.0f;
	fHeiBac = 0.5f * 198.0f;
	fWidFor = 0.5f * 116.0f;
	fHeifor = 0.5f * 112.0f;
	
	for (int Loop = 0; Loop < 4; ++Loop)
	{
		m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventF-1.png", 1000 );
		m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
		m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
		
		m_pGUIBase->AddFileName( 3, imgParam[Loop], L"Img\\Event\\EventM-1.png", 1000 );
		m_pGUIBase->AddFileName( 4, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );			
		m_pGUIBase->AddFileName( 5, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );

		m_pGUIBase->AddFileName( 6, imgParam[Loop], L"Img\\Event\\EventS-1.png", 1000 );
		m_pGUIBase->AddFileName( 7, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );			
		m_pGUIBase->AddFileName( 8, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );

		m_pGUIBase->AddFileName( 9, imgParam[Loop], L"Img\\Event\\EventD-1.png", 1000 );
		m_pGUIBase->AddFileName(10, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );			
		m_pGUIBase->AddFileName(11, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );

		m_pGUIButton[Loop]->Create( 
			fWidth - fWidFor * ( 2 * ( 2 - Loop) ),
			fHeight - fHeifor, 
			2.0f * fWidFor, 2.0f * fHeifor, 
			imgParam[Loop] );
	}	

	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround.png", 100000 );
	m_pGUIBackground->Create( fWidth - fWidBac, fHeight - fHeiBac, fWidBac * 2.0f, fHeiBac * 2.0f, imgParam[4] );

	m_pGUIBase->AddFileName( 0, imgParam[5], L"Img\\Event\\Empty.png", 1000 );
	m_pGUIBase->AddFileName( 1, imgParam[5], L"Img\\Event\\ForgroundS-1.png", 500 );
	m_pGUIBase->AddFileName( 1, imgParam[5], L"Img\\Event\\ForgroundS-2.png", 500 );
	m_pGUIBase->AddFileName( 2, imgParam[5], L"Img\\Event\\ForgroundF.png", 1000 );
	m_pGUIForground->Create( fWidth - fWidBac, fHeight - fHeiBac, fWidBac * 2.0f, fHeiBac * 2.0f, imgParam[5] );

	m_pGUIBase->AddFileName( 0, imgParam[6], L"Img\\Event\\Empty.png", 1000 );
	m_pGUIBase->AddFileName( 1, imgParam[6], L"Img\\Event\\BackGround3.png", 1000 );
	m_pGUIBase->AddFileName( 2, imgParam[6], L"Img\\Event\\BackGround2.png", 1000 );
	m_pGUIBase->AddFileName( 3, imgParam[6], L"Img\\Event\\BackGround1.png", 1000 );
	m_pGUICounting->Create( fWidth - fWidBac, fHeight - fHeiBac, fWidBac * 2.0f, fHeiBac * 2.0f, imgParam[6] );
	m_nKindIndex = 0;
	return S_OK;
}

HRESULT CGameEventCombo::Release()
{
	SAFE_DELETE ( m_pGUIBase );
	SAFE_DELETE ( m_pGUICounting );
	SAFE_DELETE ( m_pGUIForground );
	SAFE_DELETE ( m_pGUIBackground );

	SAFE_DELETE ( m_pGUICounting );
	SAFE_DELETE ( m_pGUIButton[0] );
	SAFE_DELETE ( m_pGUIButton[1] );
	SAFE_DELETE ( m_pGUIButton[2] );
	SAFE_DELETE ( m_pGUIButton[3] );

	return S_OK;
}

VOID CGameEventCombo::Clear()
{
	m_nKindEvent[0] = 0;
	m_nKindEvent[1] = 3;
	m_nKindEvent[2] = 6;
	m_nKindEvent[3] = 9;

	m_nState = NONE;
	m_fCount = 0.0f;
	m_nKindIndex = 0;
	
	m_pGUIBase = m_pGUIBackground = m_pGUIForground = m_pGUICounting = NULL;
	m_pGUIButton[0] = m_pGUIButton[1] = m_pGUIButton[2] = m_pGUIButton[3] = NULL;
}

VOID CGameEventCombo::Initialize()
{
	m_nState = NONE;
	m_fCount = 0.0f;
	m_nKindIndex = 0;
	m_nKindEvent[0] = 0;
	m_nKindEvent[1] = 3;
	m_nKindEvent[2] = 6;
	m_nKindEvent[3] = 9;

	m_pGUIForground->SelectAnimation( 0 );
	m_pGUICounting->SelectAnimation( 0 );
	m_pGUIButton[0]->SelectAnimation( 0 );
	m_pGUIButton[1]->SelectAnimation( 0 );
	m_pGUIButton[2]->SelectAnimation( 0 );
	m_pGUIButton[3]->SelectAnimation( 0 );
}

VOID CGameEventCombo::TimeUpdate( )
{
	m_fCount += CFrequency::GetInstance()->getFrametime();

	if ( m_fCount >= m_fTimeComp )
	{
		m_nState = FAIL;
	}
	else if ( m_fCount + 1 >= m_fTimeComp )
	{
		m_pGUICounting->SelectAnimation( 3 );
	}
	else if ( m_fCount + 2 >= m_fTimeComp )
	{
		m_pGUICounting->SelectAnimation( 2 );
	}
	else if ( m_fCount + 3 >= m_fTimeComp )
	{
		m_pGUICounting->SelectAnimation( 1 );
	}
}

VOID CGameEventCombo::Update()
{	
	//enum { NONE, READY, RUN, COUNTING, FAIL, SUCCESS };
	if ( CObjectManage::GetInstance()->IsHost() )
	{
		if ( m_nState == READY )
		{
			m_nState = RUN;
			TimeUpdate();
		}
		else if ( m_nState == RUN )
		{
			INT nPlayer = CGameEvent::GetInstance()->GetPlayerIndex();

			if ( nPlayer >= 0 )
			{
				CheckKindEvent( nPlayer );	
			}
			TimeUpdate();
		}
	}
}

VOID CGameEventCombo::Render()
{
	if ( m_nState != NONE )
	{
		m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pGUIBackground->Render();
		m_pGUIButton[0]->Render();
		m_pGUIButton[1]->Render();
		m_pGUIButton[2]->Render();
		m_pGUIButton[3]->Render();
		m_pGUICounting->Render();
		m_pGUIForground->Render();
		m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

VOID CGameEventCombo::AddCombo(INT nIndex, INT nKind)	
{
	m_nKindEvent[nIndex] = nKind; 
	m_pGUIButton[nIndex]->SelectAnimation( nKind * 3 );
}

BOOL CGameEventCombo::CheckKindEvent( INT nKindEvent )
{
	INT bRet = -1;

	for ( INT Loop = 0; Loop < 4; ++Loop )
	{
		// 빈곳 찾기.
		if ( !( ( 0x0001 << Loop ) & m_nKindIndex ) )
		{
			if ( ( nKindEvent ) == m_nKindEvent[Loop] )
			{
				m_pGUIButton[Loop]->NextAnimation();
				m_nKindIndex += ( 0x0001 << Loop );
				bRet = TRUE;
			}
			else
			{
				m_pGUIButton[Loop]->NextAnimation();
				m_pGUIButton[Loop]->NextAnimation();
				bRet = FALSE;
			}

			CNetwork::GetInstance()->CS_EVENT_COMBO_SLOT_STATE(Loop, bRet);
			break;
		}
		else
		{
			bRet = -1;
		}
	}

	if ( bRet == TRUE )
	{
		if ( m_nKindIndex == 0x000F )
		{
			m_nState = SUCCESS;
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_SUCCESS, 0.01f );
		}
	}
	else if  ( bRet == FALSE )
	{
		m_nState = FAIL;
		CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_FAIL, 0.01f );
	}

	return bRet;
}

BOOL CGameEventCombo::CheckClientKindEvent( INT nKindEvent, BOOL bResult )
{
	INT bRet = -1;

	if ( bResult )
	{
		m_pGUIButton[nKindEvent]->SelectAnimation( nKindEvent * 3 + 1 );
		//m_pGUIButton[nKindEvent]->NextAnimation();
		bRet = TRUE;
	}
	else
	{
		m_pGUIButton[nKindEvent]->SelectAnimation( nKindEvent * 3 + 2 );
		//m_pGUIButton[nKindEvent]->NextAnimation();
		bRet = FALSE;
	}

	return bRet;
}

VOID CGameEventCombo::Success()
{
	m_nState = END;
	m_pGUIForground->SelectAnimation( 1 );
}

VOID CGameEventCombo::Fail()
{
	m_nState = END;
	m_pGUIForground->SelectAnimation( 2 );
}
