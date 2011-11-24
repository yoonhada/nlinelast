#include "stdafx.h"
#include "LoginGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"
#include "GUIEdit.h"

VOID LoginGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pd3dDevice, m_pSprite );
	m_pGUIEdit			= new GUIEdit( m_pd3dDevice, m_pSprite );
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

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= fX;
	imgParam.fY			= fY;
	imgParam.fWidth		= fWidth;
	imgParam.fHeight	= fHeight;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoginScene\\Background.png" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID LoginGUI::CreateButton()
{
	GUIBase::IMAGEPARAM imgNormal, imgHot, imgDown, imgDisable;

	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	FLOAT fX		= m_fX + 250.0f;
	FLOAT fY		= m_fY + 350.0f;
	FLOAT fWidth	= 128.0f;
	FLOAT fHeight	= 64.0f;

	imgNormal.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgNormal.fX			= fX;
	imgNormal.fY			= fY;
	imgNormal.fWidth		= fWidth;
	imgNormal.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgNormal, L"Img\\LoginScene\\Go_Normal.png" );

	imgHot.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgHot.fX			= fX;
	imgHot.fY			= fY;
	imgHot.fWidth		= fWidth;
	imgHot.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgHot, L"Img\\LoginScene\\Go_Normal.png" );

	imgDown.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgDown.fX			= fX;
	imgDown.fY			= fY;
	imgDown.fWidth		= fWidth;
	imgDown.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgDown, L"Img\\LoginScene\\Go_Down.png" );

	imgDisable.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgDisable.fX			= fX;
	imgDisable.fY			= fY;
	imgDisable.fWidth		= fWidth;
	imgDisable.fHeight		= fHeight;

	m_pGUIBase->AddFileName( 0, imgDisable, L"Img\\LoginScene\\Go_Down.png" );

	m_pGUIBtnManager->Create( GUIBTN_LOGIN_GO, 0, imgNormal, imgHot, imgDown, imgDisable );
	
}

VOID LoginGUI::CreateEdit()
{
	GUIBase::IMAGEPARAM imgParam;

	imgParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgParam.fX			= m_fX + 250.0f;
	imgParam.fY			= m_fY + 200.0f;
	imgParam.fWidth		= 128.0f;
	imgParam.fHeight	= 64.0f;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoginScene\\Name_Edit.png" );

	m_pGUIEdit->Create( imgParam );
}

VOID LoginGUI::Create()
{
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

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
}

BOOL LoginGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
