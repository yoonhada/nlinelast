#include "stdafx.h"
#include "LobbyGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIBtnManager.h"
#include "GUIEdit.h"
#include "GUIListbox.h"

VOID LobbyGUI::Initialize()
{
	m_pStr = new TCHAR[ 1024 ];
	ZeroMemory( m_pStr, sizeof( TCHAR ) * 1024 );

	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIBtnManager	= new GUIBtnManager( m_pD3dDevice, m_pSprite );
	m_pGUIEdit			= new GUIEdit( m_pD3dDevice, m_pSprite );
	m_pGUIListbox		= new GUIListbox( m_pD3dDevice, m_pSprite );
}

VOID LobbyGUI::Release()
{
	delete[] m_pStr;

	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIBtnManager;
	delete m_pGUIEdit;
	delete m_pGUIListbox;
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
	FLOAT fY		= 940.0f;
	FLOAT fWidth	= 252.0f;
	FLOAT fHeight	= 70.0f;

	m_pGUIBase->AddFileName( 0, imgStartNormal,		L"Img\\LobbyScene\\Start_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgStartHot,		L"Img\\LobbyScene\\Start_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgStartDown,		L"Img\\LobbyScene\\Start_Down.png" );
	m_pGUIBase->AddFileName( 0, imgStartDisable,	L"Img\\LobbyScene\\Start_Disable.png" );

	m_pGUIBtnManager->Create( LOBBY_START, 0, fX, fY, fWidth,fHeight, imgStartNormal, imgStartHot, imgStartDown, imgStartDisable );
	m_pGUIBtnManager->Enable( LOBBY_START, FALSE );

	// Back Button
	GUIBase::IMAGEPARAM imgBackNormal, imgBackHot, imgBackDown, imgBackDisable;
	
	fX		= 1000.0f;
	fY		= 940.0f;
	fWidth	= 252.0f;
	fHeight	= 70.0f;

	m_pGUIBase->AddFileName( 0, imgBackNormal,	L"Img\\LobbyScene\\Back_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgBackHot,		L"Img\\LobbyScene\\Back_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgBackDown,	L"Img\\LobbyScene\\Back_Down.png" );
	m_pGUIBase->AddFileName( 0, imgBackDisable,	L"Img\\LobbyScene\\Back_Down.png" );

	m_pGUIBtnManager->Create( LOBBY_BACK, 0, fX, fY, fWidth,fHeight, imgBackNormal, imgBackHot, imgBackDown, imgBackDisable );

	//	Character Select
	GUIBase::IMAGEPARAM imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable;

	fX			= 20.0f;
	fY			= 190.0f;
	fWidth		= 286.0f;
	fHeight		= 339.0f;

	m_pGUIBase->AddFileName( 0, imgSelectNormal,	L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectHot,		L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectDown,		L"Img\\LobbyScene\\Select_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgSelectDisable,	L"Img\\LobbyScene\\Select_Normal.png" );

	m_pGUIBtnManager->Create( LOBBY_SELECT_1, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 320.0f;
	m_pGUIBtnManager->Create( LOBBY_SELECT_2, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 320.0f;
	m_pGUIBtnManager->Create( LOBBY_SELECT_3, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	fX += 320.0f;
	m_pGUIBtnManager->Create( LOBBY_SELECT_4, 1, fX, fY, fWidth,fHeight, imgSelectNormal, imgSelectHot, imgSelectDown, imgSelectDisable );

	//	Ready
	GUIBase::IMAGEPARAM imgReadyNormal, imgReadyHot, imgReadyDown, imgReadyDisable;

	fX			= 350.0f;
	fY			= 940.0f;
	fWidth		= 252.0f;
	fHeight		= 70.0f;
	
	m_pGUIBase->AddFileName( 0, imgReadyNormal,		L"Img\\LobbyScene\\Ready_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgReadyHot,		L"Img\\LobbyScene\\Ready_Normal.png" );
	m_pGUIBase->AddFileName( 0, imgReadyDown,		L"Img\\LobbyScene\\Ready_Down.png" );
	m_pGUIBase->AddFileName( 0, imgReadyDisable,	L"Img\\LobbyScene\\Ready_Down.png" );

	m_pGUIBtnManager->Create( LOBBY_READY, 1, fX, fY, fWidth, fHeight, imgReadyNormal, imgReadyHot, imgReadyDown, imgReadyDisable );
}

VOID LobbyGUI::CreateEdit()
{
	FLOAT fX		= 40.0f;
	FLOAT fY		= 860.0f;
	FLOAT fWidth	= 726.0f;
	FLOAT fHeight	= 53.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LobbyScene\\Edit.png" );

	m_pGUIEdit->Create( fX, fY, fWidth, fHeight, imgParam );
	m_pGUIEdit->SetStyle( GUIEdit::EDT_DYNAMIC );
	m_pGUIEdit->SetFont( L"궁서", 20, 20 );
}
VOID LobbyGUI::CreateListbox()
{
	FLOAT fX		= 50.0f;
	FLOAT fY		= 590.0f;
	FLOAT fWidth	= 722.0f;
	FLOAT fHeight	= 259.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LobbyScene\\Listbox_Back.png" );

	m_pGUIListbox->Create( fX, fY, fWidth, fHeight, imgParam );
	m_pGUIListbox->SetFont( L"궁서", 20, 20 );
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
	CreateEdit();
	CreateListbox();
}

VOID LobbyGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBackground->Update();
	m_pGUIBtnManager->Update( pt.x, pt.y );
	m_pGUIEdit->Update();
	m_pGUIListbox->Update();

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

	/*if( m_pGUIEdit->TakeMessage( m_pStr ) )
		m_pGUIListbox->AddItem( m_pStr );*/

}

VOID LobbyGUI::Render()
{
	//m_pGUIBackground->Render();
	m_pGUIBtnManager->Render();
	m_pGUIEdit->Render();
	m_pGUIListbox->Render();
}

VOID LobbyGUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
	m_pGUIListbox->OnDown( x, y );
}

VOID LobbyGUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
	m_pGUIListbox->OnMove( x, y );
}

VOID LobbyGUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
	m_pGUIListbox->OnUp( x, y );
}

VOID LobbyGUI::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

VOID LobbyGUI::EnableButton( DWORD _dID, BOOL _bEnable )
{
	m_pGUIBtnManager->Enable( _dID, _bEnable );
}

VOID LobbyGUI::SetText( LPWSTR _pText )
{
	m_pGUIListbox->AddItem( _pText );
}
VOID LobbyGUI::GetText( LPWSTR _pText )
{
	_tcscpy( _pText, m_pStr );
	ZeroMemory( m_pStr, sizeof( TCHAR ) * 1024 );
}

VOID LobbyGUI::ChangeScene( DWORD _dID, DWORD _dState )
{
	m_pGUIBtnManager->ChangeState( _dID, _dState );
}

BOOL LobbyGUI::TakeChattingMassage( LPWSTR _pText )
{
	if( m_pGUIEdit->TakeMessage( m_pStr ) )
	{
		m_pGUIListbox->AddItem( m_pStr );

		_tcscpy( _pText, m_pStr );
		return TRUE;
	}
	return FALSE;
}