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
	m_pGUIBackground[0]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBackground[1]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBackground[2]	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBackground[3]	= new GUIBackground( m_pD3dDevice, m_pSprite );

	GUIBase::IMAGEPARAM imgParam[4];

	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );

	for (int Loop = 0; Loop < 4; ++Loop)
	{
		switch( m_nKindEvent[Loop] )
		{
		case 1:
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventF-1.png", 10000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventF-2.png", 10000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventF-3.png", 10000 );			
			break;
		case 2:
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventM-1.png", 10000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventM-2.png", 10000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventM-3.png", 10000 );			
			break;
		case 3:
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventS-1.png", 10000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventS-2.png", 10000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventS-3.png", 10000 );			
			break;
		case 4:
			m_pGUIBase->AddFileName( 0, imgParam[Loop], L"Img\\Event\\EventD-1.png", 10000 );
			m_pGUIBase->AddFileName( 1, imgParam[Loop], L"Img\\Event\\EventD-2.png", 10000 );
			m_pGUIBase->AddFileName( 2, imgParam[Loop], L"Img\\Event\\EventD-3.png", 10000 );			
			break;
		}
		
		m_pGUIBackground[Loop]->Create( fWidth * 0.5f - (400.0f - 100.0f * Loop ), fHeight * 0.33f - 100.0f, 100.0f, 100.0f, imgParam[Loop] );
	}	

	return S_OK;
}

HRESULT CGameEventCombo::Release()
{
	SAFE_DELETE ( m_pGUIBase );
	SAFE_DELETE ( m_pGUIBackground[0] );
	SAFE_DELETE ( m_pGUIBackground[1] );
	SAFE_DELETE ( m_pGUIBackground[2] );
	SAFE_DELETE ( m_pGUIBackground[3] );

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
	//m_pGUIBackground->Update();
}

VOID CGameEventCombo::Render()
{
	m_pGUIBackground[0]->Render();
	m_pGUIBackground[1]->Render();
	m_pGUIBackground[2]->Render();
	m_pGUIBackground[3]->Render();
}