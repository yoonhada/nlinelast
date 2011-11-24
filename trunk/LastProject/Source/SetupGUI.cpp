#include "Stdafx.h"
#include "SetupGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIScrollbar.h"

VOID SetupGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pGUIEffectSound	= new GUIScrollbar( m_pd3dDevice, m_pSprite );
	m_pGUIBackSound		= new GUIScrollbar( m_pd3dDevice, m_pSprite );
}

VOID SetupGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIEffectSound;
	delete m_pGUIBackSound;
}

VOID SetupGUI::CreateBackground()
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

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\SetupScene\\Background.png" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID SetupGUI::CreateScrollbar()
{
	GUIBase::IMAGEPARAM imgThumbParam;
	GUIBase::IMAGEPARAM imgBackParam;

	imgThumbParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgThumbParam.fX			= m_fX + 100.0f;
	imgThumbParam.fY			= m_fY + 100.0f;
	imgThumbParam.fWidth		= 50.0f;
	imgThumbParam.fHeight		= 50.0f;

	m_pGUIBase->AddFileName( 0, imgThumbParam, L"Img\\SetupScene\\Brick.png" );

	imgBackParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgBackParam.fX			= m_fX + 100.0f;
	imgBackParam.fY			= m_fY + 100.0f;
	imgBackParam.fWidth		= 300.0f;
	imgBackParam.fHeight	= 50.0f;
	
	m_pGUIBase->AddFileName( 0, imgBackParam, L"Img\\SetupScene\\Brick.png" );

	m_pGUIEffectSound->Create( imgThumbParam, imgBackParam );
	m_pGUIEffectSound->SetStyle( GUIScrollbar::SCL_HROZ );

	imgThumbParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgThumbParam.fX			= m_fX + 100.0f;
	imgThumbParam.fY			= m_fY + 300.0f;
	imgThumbParam.fWidth		= 50.0f;
	imgThumbParam.fHeight		= 50.0f;

	imgBackParam.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgBackParam.fX			= m_fX + 100.0f;
	imgBackParam.fY			= m_fY + 300.0f;
	imgBackParam.fWidth		= 300.0f;
	imgBackParam.fHeight	= 50.0f;

	m_pGUIBackSound->Create( imgThumbParam, imgBackParam );
	m_pGUIBackSound->SetStyle( GUIScrollbar::SCL_HROZ );
}

VOID SetupGUI::Create()
{
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	m_fWidth	= 512.0f;
	m_fHeight	= 512.0f;
	m_fX		= static_cast<FLOAT>( Vp.Width ) * 0.5f - m_fWidth * 0.5f;
	m_fY		= static_cast<FLOAT>( Vp.Height ) * 0.5f - m_fHeight * 0.5f;

	m_pMouse->Initialize( m_hWnd );

	CreateBackground();
	CreateScrollbar();
}

VOID SetupGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBackground->Update();
	m_pGUIEffectSound->Update();
	m_pGUIBackSound->Update();

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

VOID SetupGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIEffectSound->Render();
	m_pGUIBackSound->Render();
}

VOID SetupGUI::OnDown( INT x, INT y )
{
	m_pGUIEffectSound->OnDown( x, y );
	m_pGUIBackSound->OnDown( x, y );
}

VOID SetupGUI::OnMove( INT x, INT y )
{
	m_pGUIEffectSound->OnMove( x, y );
	m_pGUIBackSound->OnMove( x, y );
}

VOID SetupGUI::OnUp( INT x, INT y )
{
	m_pGUIEffectSound->OnUp( x, y );
	m_pGUIBackSound->OnUp( x, y );
}

VOID SetupGUI::Command( DWORD& _dOut )
{
}

BOOL SetupGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
