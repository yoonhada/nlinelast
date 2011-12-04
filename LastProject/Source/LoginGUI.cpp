#include "stdafx.h"
#include "LoginGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"
#include "GUIEdit.h"

VOID LoginGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pD3dDevice, m_pSprite );
	m_pGUIEdit			= new GUIEdit( m_pD3dDevice, m_pSprite );
}

VOID LoginGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
	delete m_pGUIEdit;
}

VOID LoginGUI::CreateBackground()
{
	//	Create Background
	FLOAT fWidth	= m_fWidth;
	FLOAT fHeight	= m_fHeight;
	FLOAT fX		= m_fX;
	FLOAT fY		= m_fY;
	
	GUIBase::IMAGEPARAM imgParam;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoginScene\\Background.png" );
	
	m_pGUIBackground->Create( fX, fY, fWidth, fHeight, imgParam );
}

VOID LoginGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgGoNormal, imgGoHot, imgGoDown, imgGoDisable;

	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fX		= m_fX + 150.0f;
	FLOAT fY		= m_fY + 350.0f;
	FLOAT fWidth	= 200.0f;
	FLOAT fHeight	= 100.0f;

	m_pGUIBase->AddFileName( 0, imgGoNormal, L"Img\\LoginScene\\Go_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgGoHot, L"Img\\LoginScene\\Go_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgGoDown, L"Img\\LoginScene\\Go_Down.png" );
	m_pGUIBase->AddFileName( 0, imgGoDisable, L"Img\\LoginScene\\Go_Down.png" );

	m_pGUIBtnManager->Create( LOGIN_GO, 0, fX, fY, fWidth, fHeight, imgGoNormal, imgGoHot, imgGoDown, imgGoDisable );

	GUIBase::IMAGEPARAM imgExitNormal, imgExitHot, imgExitDown, imgExitDisable;

	fX		= m_fX + m_fWidth - 60.0f;
	fY		= m_fY + 10.0f;
	fWidth	= 50.0f;
	fHeight = 50.0f;

	m_pGUIBase->AddFileName( 0, imgExitNormal, L"Img\\LoginScene\\Exit_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgExitHot, L"Img\\LoginScene\\Exit_Hot.png" );
	m_pGUIBase->AddFileName( 0, imgExitDown, L"Img\\LoginScene\\Exit_Down.png" );
	m_pGUIBase->AddFileName( 0, imgExitDisable, L"Img\\LoginScene\\Exit_Down.png" );

	m_pGUIBtnManager->Create( LOGIN_EXIT, 0, fX, fY, fWidth, fHeight, imgExitNormal, imgExitHot, imgExitDown, imgExitDisable );
}

VOID LoginGUI::CreateEdit()
{
	FLOAT fX		= m_fX + 50.0f;
	FLOAT fY		= m_fY + 100.0f;
	FLOAT fWidth	= 400.0f;
	FLOAT fHeight	= 150.0f;

	GUIBase::IMAGEPARAM imgParam;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoginScene\\Name_Edit.png" );

	m_pGUIEdit->Create( fX, fY, fWidth, fHeight, imgParam );
	m_pGUIEdit->SetFont( L"휴먼매직체", 30, 80 );
}

VOID LoginGUI::Create()
{
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	m_fWidth	= 512.0f;
	m_fHeight	= 512.0f;
	m_fX		= static_cast<FLOAT>( Vp.Width ) * 0.5f - m_fWidth * 0.5f;
	m_fY		= static_cast<FLOAT>( Vp.Height ) * 0.5f - m_fHeight * 0.5f;

	m_pMouse->Initialize( m_hWnd );

	CreateBackground();
	CreateButton();
	CreateEdit();
}

VOID LoginGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBackground->Update();
	m_pGUIBtnManager->Update( pt.x, pt.y );
	m_pGUIEdit->Update();

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

VOID LoginGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
	m_pGUIEdit->Render();
}

VOID LoginGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID LoginGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID LoginGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID LoginGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

VOID LoginGUI::GetEditTexture( LPWSTR _pEditTexture )
{
	m_pGUIEdit->TakeMessage( _pEditTexture );
}

