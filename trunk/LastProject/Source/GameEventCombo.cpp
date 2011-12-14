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
	m_pGUIButton[0]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[1]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[2]		= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[3]		= new GUIBackground( m_pD3dDevice, m_pSprite );


	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );

	GUIBase::IMAGEPARAM imgParam[6];

	for (int Loop = 0; Loop < 4; ++Loop)
	{
		switch( m_nKindEvent[Loop] )
		{
		case 1:
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventF-1.png", 100000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 100000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 100000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000000 );			
			break;																   
		case 2:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventM-1.png", 100000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 100000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 100000 );			
			break;																   
		case 3:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventS-1.png", 100000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 100000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 100000 );			
			break;																   
		case 4:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			//m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 1000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventD-1.png", 100000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );
			//m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 100000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );			
			//m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 100000 );			
			break;
		}
		
		m_pGUIButton[Loop]->Create( 
			fWidth * 0.5f - (400.0f - 100.0f * Loop ), 
			fHeight * 0.33f - 100.0f, 
			100.0f, 
			100.0f, 
			imgParam[Loop] );
	}	

	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround3.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround2.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround1.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround.png", 100000 );
	m_pGUIBackground->Create( fWidth * 0.5f - 407.0f, fHeight * 0.33f - 114.0f, 407.0f, 114.0f, imgParam[4] );

	m_pGUIBase->AddFileName( 0, imgParam[5], L"Img\\Event\\Empty.png", 1000 );
	m_pGUIBase->AddFileName( 1, imgParam[5], L"Img\\Event\\EmptyS.png", 1000 );
	m_pGUIBase->AddFileName( 2, imgParam[5], L"Img\\Event\\EmptyF.png", 1000 );
	m_pGUIForground->Create( fWidth * 0.5f - 407.0f, fHeight * 0.33f - 114.0f, 407.0f, 114.0f, imgParam[5] );

	m_nKindIndex = 0;
	return S_OK;
}

HRESULT CGameEventCombo::Release()
{
	SAFE_DELETE ( m_pGUIBase );
	SAFE_DELETE ( m_pGUIBackground );
	SAFE_DELETE ( m_pGUIForground );
	SAFE_DELETE ( m_pGUIButton[0] );
	SAFE_DELETE ( m_pGUIButton[1] );
	SAFE_DELETE ( m_pGUIButton[2] );
	SAFE_DELETE ( m_pGUIButton[3] );

	return S_OK;
}

VOID CGameEventCombo::Clear()
{
	m_nKindEvent[0] = 1;
	m_nKindEvent[1] = 2;
	m_nKindEvent[2] = 3;
	m_nKindEvent[3] = 4;

	m_bComplate = FALSE;
}

VOID CGameEventCombo::Update()
{
	INT nPlayer = CGameEvent::GetInstance()->GetPlayerIndex();
	
	
	if ( nPlayer >= 0 )
	{
		CheckKindEvent( nPlayer );	
	}
}

VOID CGameEventCombo::Render()
{
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pGUIBackground->Render();
	m_pGUIButton[0]->Render();
	m_pGUIButton[1]->Render();
	m_pGUIButton[2]->Render();
	m_pGUIButton[3]->Render();
	m_pGUIForground->Render();
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

BOOL CGameEventCombo::CheckKindEvent( INT nKindEvent )
{
	INT bRet = -1;

	for ( INT Loop = 0; Loop < 4; ++Loop )
	{
		// 빈곳 찾기.
		if ( !( ( 0x0001 << Loop ) & m_nKindIndex ) )
		{
			if ( ( nKindEvent + 1 ) == m_nKindEvent[Loop] )
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

			break;
		}
		else
		{
			bRet = -1;
		}
	}

	if ( CObjectManage::GetInstance()->IsHost() )
	{
		if ( bRet == TRUE )
		{
			if ( m_nKindIndex == 0x000F )
			{
				CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_SUCCESS, 0.01f );
			}
		}
		else if  ( bRet == FALSE )
		{
			CGameEvent::GetInstance()->AddEvent( CGameEvent::EVENT_COMBO_FAIL, 0.01f );
		}
	}

	return bRet;
}

VOID CGameEventCombo::Success()
{
	m_bComplate = TRUE;
	m_pGUIForground->NextAnimation();
}

VOID CGameEventCombo::Fail()
{
	if ( m_bComplate == FALSE )
	{
		m_pGUIForground->NextAnimation();
		m_pGUIForground->NextAnimation();
	}
}