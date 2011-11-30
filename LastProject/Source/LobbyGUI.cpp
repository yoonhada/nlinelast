#include "stdafx.h"
#include "LobbyGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"

VOID LobbyGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pD3dDevice, m_pSprite );
}

VOID LobbyGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
}

VOID LobbyGUI::CreateBackground()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParam;

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= 0.0f;
	imgParam.fY			= 0.0f;
	imgParam.fWidth		= fWidth;
	imgParam.fHeight	= fHeight;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LobbyScene\\Background.png" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID LobbyGUI::CreateButton()
{
	// Start Button
	GUIBase::IMAGEPARAM imgStartNormal, imgStartHot, imgStartDown, imgStartDisable;

	FLOAT fX		= 50.0f;
	FLOAT fY		= m_fY + m_fHeight - 100.0f;
	FLOAT fWidth	= 256.0f;
	FLOAT fHeight	= 64.0f;

	m_pGUIBase->AddFileName( 0, imgStartNormal, L"Img\\LobbyScene\\Start_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgStartHot, L"Img\\LobbyScene\\Start_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgStartDown, L"Img\\LobbyScene\\Start_Down.png" );
	m_pGUIBase->AddFileName( 0, imgStartDisable, L"Img\\LobbyScene\\Start_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_LOBBY_START, 0, fX, fY, fWidth,fHeight, imgStartNormal, imgStartHot, imgStartDown, imgStartDisable );

	// End Button
	GUIBase::IMAGEPARAM imgEndNormal, imgEndHot, imgEndDown, imgEndDisable;
	
	fX		= m_fX + m_fWidth - 300.0f;
	fY		= m_fY + m_fHeight - 100.0f;
	fWidth	= 256.0f;
	fHeight	= 64.0f;

	m_pGUIBase->AddFileName( 0, imgEndNormal, L"Img\\LobbyScene\\Back_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgEndHot, L"Img\\LobbyScene\\Back_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgEndDown, L"Img\\LobbyScene\\Back_Down.png" );
	m_pGUIBase->AddFileName( 0, imgEndDisable, L"Img\\LobbyScene\\Back_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_LOBBY_BACK, 0, fX, fY, fWidth,fHeight, imgEndNormal, imgEndHot, imgEndDown, imgEndDisable );

	//	Character Select
	GUIBase::IMAGEPARAM imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable;

	fX			= 50.0f;
	fY			= 100.0f;
	fWidth		= 300.0f;
	fHeight		= 600.0f;

	m_pGUIBase->AddFileName( 0, imgSelectNormal, L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectHot, L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectDown, L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectDisable, L"Img\\LobbyScene\\Select_Normal.png" );

	m_pGUIBtnManager->Create( GUIBTN_LOBBY_SELECT_1, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 310.0f;
	m_pGUIBtnManager->Create( GUIBTN_LOBBY_SELECT_2, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 310.0f;
	m_pGUIBtnManager->Create( GUIBTN_LOBBY_SELECT_3, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 310.0f;
	m_pGUIBtnManager->Create( GUIBTN_LOBBY_SELECT_4, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	//	Ready
	GUIBase::IMAGEPARAM imgReadyNormal, imgReadyHot, imgReadyDown;

	fX			= 400.0f;
	fY			= 800.0f;
	fWidth		= 200.0f;
	fHeight		= 200.0f;
	
	m_pGUIBase->AddFileName( 0, imgReadyNormal, L"Img\\LobbyScene\\quote-Normal.png" );
	m_pGUIBase->AddFileName( 0, imgReadyHot, L"Img\\LobbyScene\\quote-Hot.png" );
	m_pGUIBase->AddFileName( 0, imgReadyDown, L"Img\\LobbyScene\\quote-Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_LOBBY_READY, 0, fX, fY, fWidth, fHeight, imgReadyNormal, imgReadyHot, imgReadyDown, imgReadyDown );
}

VOID LobbyGUI::Create()
{
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	m_fX		= 0.0f;
	m_fY		= 0.0f;
	m_fWidth	= static_cast<FLOAT>( Vp.Width );
	m_fHeight	= static_cast<FLOAT>( Vp.Height );

	m_pMouse->Initialize( m_hWnd );
	CreateBackground();
	CreateButton();
}

VOID LobbyGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBackground->Update();
	m_pGUIBtnManager->Update( pt.x, pt.y );

	static BOOL bFirst = FALSE;
	
	if( m_pMouse->LButton_Down() )
	{
		if( bFirst == FALSE )
		{
			OnDown( pt.x, pt.y );
			bFirst = TRUE;
		}
		//if( GrpBtn::g_bCapture == FALSE )
		//{
			//	버튼을 누르면 g_bCapture = TREU 가 된다
			//	영역체크에 쓴다
		//}
	}
	
	OnMove( pt.x, pt.y );
	
	if( m_pMouse->LButton_Up() )
	{
		OnUp( pt.x, pt.y );
		bFirst = FALSE;
	}
}

VOID LobbyGUI::Render()
{
	//m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
}

VOID LobbyGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID LobbyGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID LobbyGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID LobbyGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

BOOL LobbyGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
