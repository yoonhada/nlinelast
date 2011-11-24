#include "stdafx.h"
#include "StoryGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"

VOID StoryGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pd3dDevice, m_pSprite );
}

VOID StoryGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
}

VOID StoryGUI::CreateBackground()
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

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_1.jpg" );
	m_pGUIBase->AddFileName( 1, imgParam, L"Img\\StoryScene\\Story_2.jpg" );
	m_pGUIBase->AddFileName( 2, imgParam, L"Img\\StoryScene\\Story_3.jpg" );
	m_pGUIBase->AddFileName( 3, imgParam, L"Img\\StoryScene\\Story_4.jpg" );
	m_pGUIBase->AddFileName( 4, imgParam, L"Img\\StoryScene\\Story_5.jpg" );
	m_pGUIBase->AddFileName( 5, imgParam, L"Img\\StoryScene\\Story_6.jpg" );
	m_pGUIBase->AddFileName( 6, imgParam, L"Img\\StoryScene\\Story_7.jpg" );
	m_pGUIBase->AddFileName( 7, imgParam, L"Img\\StoryScene\\Story_8.jpg" );
	m_pGUIBase->AddFileName( 8, imgParam, L"Img\\StoryScene\\Story_9.jpg" );
	m_pGUIBase->AddFileName( 9, imgParam, L"Img\\StoryScene\\Story_10.jpg" );
	m_pGUIBase->AddFileName( 10, imgParam, L"Img\\StoryScene\\Story_11.jpg" );
	m_pGUIBase->AddFileName( 11, imgParam, L"Img\\StoryScene\\Story_12.jpg" );
	m_pGUIBase->AddFileName( 12, imgParam, L"Img\\StoryScene\\Story_13.jpg" );
	m_pGUIBase->AddFileName( 13, imgParam, L"Img\\StoryScene\\Story_14.jpg" );
	m_pGUIBase->AddFileName( 14, imgParam, L"Img\\StoryScene\\Story_15.jpg" );

	m_pGUIBackground->Create( imgParam );
}

VOID StoryGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgNormal, imgHot, imgDown, imgDisable;

	//	Background Next Button
	FLOAT fX		= 700.0f;
	FLOAT fY		= 400.0f;
	FLOAT fWidth	= 50.0f;
	FLOAT fHeight	= 50.0f;

	imgNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgNormal.fX			= fX;
	imgNormal.fY			= fY;
	imgNormal.fWidth		= fWidth;
	imgNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgNormal, L"Img\\arrow-up-Normal.png" );

	imgHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHot.fX			= fX;
	imgHot.fY			= fY;
	imgHot.fWidth		= fWidth;
	imgHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHot, L"Img\\arrow-up-Hot.png" );

	imgDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgDown.fX			= fX;
	imgDown.fY			= fY;
	imgDown.fWidth		= fWidth;
	imgDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgDown, L"Img\\arrow-up-Down.png" );

	imgDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgDisable.fX			= fX;
	imgDisable.fY			= fY;
	imgDisable.fWidth		= fWidth;
	imgDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgDisable, L"Img\\arrow-up-Disable.png" );

	m_pGUIBtnManager->Create( GUIBTN_STORY_NEXT, 0, imgNormal, imgHot, imgDown, imgDisable );
}

VOID StoryGUI::Create()
{
	m_pMouse->Initialize( m_hWnd );
	CreateBackground();
	CreateButton();
}

VOID StoryGUI::Update()
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

VOID StoryGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
}

VOID StoryGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID StoryGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID StoryGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID StoryGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

BOOL StoryGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
