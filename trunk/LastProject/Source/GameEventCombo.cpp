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
	m_pGUIBackground= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIForground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[0]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[1]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[2]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIButton[3]	= new GUIBackground( m_pD3dDevice, m_pSprite );


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
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventF-1.png", 1000000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 1000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 1000 );			
			break;																   
		case 2:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventM-1.png", 1000000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 1000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );			
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 1000 );			
			break;																   
		case 3:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventS-1.png", 1000000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 1000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );			
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 1000 );			
			break;																   
		case 4:																	   
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\Event.png", 3000 );
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventD-1.png", 1000000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 1000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );			
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 1000 );			
			break;
		}
		
		m_pGUIButton[Loop]->Create( fWidth * 0.5f - (400.0f - 100.0f * Loop ), fHeight * 0.33f - 100.0f, 100.0f, 100.0f, imgParam[Loop] );
	}	

	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround3.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround2.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround1.png", 1000 );
	m_pGUIBase->AddFileName( 0, imgParam[4], L"Img\\Event\\BackGround.png", 1000000 );
	m_pGUIBackground->Create( fWidth * 0.5f - 400.0f, fHeight * 0.33f - 100.0f, 100.0f, 100.0f, imgParam[4] );

	m_pGUIBase->AddFileName( 0, imgParam[5], L"Img\\Event\\Event.png", 1000 );
	m_pGUIBase->AddFileName( 1, imgParam[5], L"Img\\Event\\EmptyS.png", 1000 );
	m_pGUIBase->AddFileName( 2, imgParam[5], L"Img\\Event\\EmptyF.png", 1000 );
	m_pGUIForground->Create( fWidth * 0.5f - 400.0f, fHeight * 0.33f - 100.0f, 100.0f, 100.0f, imgParam[5] );

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
}

VOID CGameEventCombo::Update()
{
	INT nPlayer = CGameEvent::GetInstance()->GetPlayerIndex();
	
	if ( nPlayer >= 0 )
		CheckKindEvent( nPlayer );	
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
	BOOL bRet = FALSE;

	for ( INT Loop = 0; Loop < 4; ++Loop )
	{
		if ( !( ( 0x0001 << Loop ) & m_nKindIndex ) )
		{
			m_pGUIButton[Loop]->NextAnimation();
			m_nKindIndex = m_nKindIndex | ( 0x0001 << Loop );
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}