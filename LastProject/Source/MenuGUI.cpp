#include "Stdafx.h"
#include "MenuGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"

VOID MenuGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pd3dDevice, m_pSprite );
}

VOID MenuGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
}

VOID MenuGUI::CreateBackground()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParam;

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= 0.0f;
	imgParam.fY			= 0.0f;
	imgParam.fWidth		= fWidth;
	imgParam.fHeight	= fHeight;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\MenuScene\\Background.jpg" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID MenuGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgStartNormal, imgStartHot, imgStartDown, imgStartDisable;

	//	Background Next Button
	FLOAT fX		= 400.0f;
	FLOAT fY		= 400.0f;
	FLOAT fWidth	= 256.0f;
	FLOAT fHeight	= 64.0f;

	imgStartNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgStartNormal.fX			= fX;
	imgStartNormal.fY			= fY;
	imgStartNormal.fWidth		= fWidth;
	imgStartNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgStartNormal, L"Img\\MenuScene\\Start_Normal.png" );

	imgStartHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgStartHot.fX			= fX;
	imgStartHot.fY			= fY;
	imgStartHot.fWidth		= fWidth;
	imgStartHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgStartHot, L"Img\\MenuScene\\Start_Normal.png" );

	imgStartDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgStartDown.fX			= fX;
	imgStartDown.fY			= fY;
	imgStartDown.fWidth		= fWidth;
	imgStartDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgStartDown, L"Img\\MenuScene\\Start_Down.png" );

	imgStartDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgStartDisable.fX			= fX;
	imgStartDisable.fY			= fY;
	imgStartDisable.fWidth		= fWidth;
	imgStartDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgStartDisable, L"Img\\MenuScene\\Start_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_MENU_START, 0, imgStartNormal, imgStartHot, imgStartDown, imgStartDisable );

	GUIBase::IMAGEPARAM imgEndNormal, imgEndHot, imgEndDown, imgEndDisable;
	//	Background Next Button
	fX		= 400.0f;
	fY		= 500.0f;
	fWidth	= 256.0f;
	fHeight	= 64.0f;

	imgEndNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgEndNormal.fX			= fX;
	imgEndNormal.fY			= fY;
	imgEndNormal.fWidth		= fWidth;
	imgEndNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgEndNormal, L"Img\\MenuScene\\End_Normal.png" );

	imgEndHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgEndHot.fX			= fX;
	imgEndHot.fY			= fY;
	imgEndHot.fWidth		= fWidth;
	imgEndHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgEndHot, L"Img\\MenuScene\\End_Normal.png" );

	imgEndDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgEndDown.fX			= fX;
	imgEndDown.fY			= fY;
	imgEndDown.fWidth		= fWidth;
	imgEndDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgEndDown, L"Img\\MenuScene\\End_Down.png" );

	imgEndDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgEndDisable.fX			= fX;
	imgEndDisable.fY			= fY;
	imgEndDisable.fWidth		= fWidth;
	imgEndDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgEndDisable, L"Img\\MenuScene\\End_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_MENU_END, 0, imgEndNormal, imgEndHot, imgEndDown, imgEndDisable );
}

VOID MenuGUI::Create()
{
	m_pMouse->Initialize( m_hWnd );

	CreateBackground();
	CreateButton();
}

VOID MenuGUI::Update()
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

VOID MenuGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
}

VOID MenuGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID MenuGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID MenuGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID MenuGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

BOOL MenuGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
