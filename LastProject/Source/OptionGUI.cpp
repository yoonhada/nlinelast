#include "Stdafx.h"
#include "OptionGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"

VOID OptionGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pD3dDevice, m_pSprite );
}

VOID OptionGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
}

VOID OptionGUI::CreateBackground()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= 32.0f;
	
	GUIBase::IMAGEPARAM imgParam;

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= 0.0f;
	imgParam.fY			= 0.0f;
	imgParam.fWidth		= fWidth;
	imgParam.fHeight	= fHeight;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\OptionScene\\Background.png" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID OptionGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgSetupNormal, imgSetupHot, imgSetupDown, imgSetupDisable;

	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fX		= static_cast<FLOAT>( Vp.Width ) - 64.0f;
	FLOAT fY		= 0.0f;
	FLOAT fWidth	= 32.0f;
	FLOAT fHeight	= 32.0f;

	imgSetupNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgSetupNormal.fX			= fX;
	imgSetupNormal.fY			= fY;
	imgSetupNormal.fWidth		= fWidth;
	imgSetupNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgSetupNormal, L"Img\\OptionScene\\Setup_Normal.png" );

	imgSetupHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgSetupHot.fX			= fX;
	imgSetupHot.fY			= fY;
	imgSetupHot.fWidth		= fWidth;
	imgSetupHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgSetupHot, L"Img\\OptionScene\\Setup_Normal.png" );

	imgSetupDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgSetupDown.fX			= fX;
	imgSetupDown.fY			= fY;
	imgSetupDown.fWidth		= fWidth;
	imgSetupDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgSetupDown, L"Img\\OptionScene\\Setup_Down.png" );

	imgSetupDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgSetupDisable.fX			= fX;
	imgSetupDisable.fY			= fY;
	imgSetupDisable.fWidth		= fWidth;
	imgSetupDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgSetupDisable, L"Img\\OptionScene\\Setup_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_OPTION_SETUP, 0, imgSetupNormal, imgSetupHot, imgSetupDown, imgSetupDisable );

	GUIBase::IMAGEPARAM imgHelpNormal, imgHelpHot, imgHelpDown, imgHelpDisable;
	//	Background Next Button
	fX		= static_cast<FLOAT>( Vp.Width ) - 32.0f;
	fY		= 0.0f;
	fWidth	= 32.0f;
	fHeight	= 32.0f;

	imgHelpNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHelpNormal.fX			= fX;
	imgHelpNormal.fY			= fY;
	imgHelpNormal.fWidth		= fWidth;
	imgHelpNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHelpNormal, L"Img\\OptionScene\\Help_Normal.png" );

	imgHelpHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHelpHot.fX			= fX;
	imgHelpHot.fY			= fY;
	imgHelpHot.fWidth		= fWidth;
	imgHelpHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHelpHot, L"Img\\OptionScene\\Help_Normal.png" );

	imgHelpDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHelpDown.fX			= fX;
	imgHelpDown.fY			= fY;
	imgHelpDown.fWidth		= fWidth;
	imgHelpDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHelpDown, L"Img\\OptionScene\\Help_Down.png" );

	imgHelpDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHelpDisable.fX			= fX;
	imgHelpDisable.fY			= fY;
	imgHelpDisable.fWidth		= fWidth;
	imgHelpDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHelpDisable, L"Img\\OptionScene\\Help_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_OPTION_HELP, 0, imgHelpNormal, imgHelpHot, imgHelpDown, imgHelpDisable );
}

VOID OptionGUI::Create()
{
	m_pMouse->Initialize( m_hWnd );

	CreateBackground();
	CreateButton();
}

VOID OptionGUI::Update()
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

VOID OptionGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
}

VOID OptionGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID OptionGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID OptionGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID OptionGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

BOOL OptionGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
