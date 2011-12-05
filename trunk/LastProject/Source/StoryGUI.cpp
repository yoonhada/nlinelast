#include "stdafx.h"
#include "StoryGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"

VOID StoryGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pD3dDevice, m_pSprite );
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
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParam;

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= 0.0f;
	imgParam.fY			= 0.0f;
	imgParam.fWidth		= fWidth;
	imgParam.fHeight	= fHeight;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_1.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_2.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_3.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_4.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_5.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_6.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_7.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_8.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_9.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_10.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_11.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_12.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_13.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_14.jpg", 2000 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\StoryScene\\Story_15.jpg", 2000 );

	m_pGUIBackground->Create( imgParam );
}

VOID StoryGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgNormal, imgHot, imgDown, imgDisable;

	//	Background Next Button
	FLOAT fX		= 1150.0f;
	FLOAT fY		= 900.0f;
	FLOAT fWidth	= 121.0f;
	FLOAT fHeight	= 114.0f;

	m_pGUIBase->AddFileName( 0, imgNormal,	L"Img\\StoryScene\\Skip_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgHot,		L"Img\\StoryScene\\Skip_Hot.png" );
	m_pGUIBase->AddFileName( 0, imgDown,	L"Img\\StoryScene\\Skip_Down.png" );
	m_pGUIBase->AddFileName( 0, imgDisable, L"Img\\StoryScene\\Skip_Down.png" );

	m_pGUIBtnManager->Create( STORY_SKIP, 0, fX, fY, fWidth, fHeight, imgNormal, imgHot, imgDown, imgDisable );
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
