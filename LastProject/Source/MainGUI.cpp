#include "stdafx.h"
#include "MainGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIListbox.h"
#include "GUIEdit.h"

VOID MainGUI::Initialize()
{
	m_pMouse				= new Mouse;
	m_pGUIBase				= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUICharacterState	= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pGUIMiniMap			= new GUIBackground( m_pd3dDevice, m_pSprite );
	m_pChatWindow			= new GUIListbox( m_pd3dDevice, m_pSprite );
	m_pChatEdit				= new GUIEdit( m_pd3dDevice, m_pSprite );
}

VOID MainGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUICharacterState;
	delete m_pGUIMiniMap;
	delete m_pChatWindow;
	delete m_pChatEdit;
}

VOID MainGUI::Create()
{
	m_pMouse->Initialize( m_hWnd );
	
	FLOAT fX		= 0.0f;
	FLOAT fY		= 0.0f;
	FLOAT fWidth	= 300.0f;
	FLOAT fHeight	= 200.0f;

	GUIBase::IMAGEPARAM imgParamCharacterState;

	m_pGUIBase->AddFileName( 0, imgParamCharacterState, L"Img\\MainScene\\Character_State.png" );
	
	m_pGUICharacterState->Create( fX, fY, fWidth, fHeight, imgParamCharacterState );

	fX		= 600.0f;
	fY		= 0.0f;
	fWidth	= 200.0f;
	fHeight = 200.0f;

	GUIBase::IMAGEPARAM imgParamMiniMap;

	m_pGUIBase->AddFileName( 0, imgParamMiniMap, L"Img\\MainScene\\MiniMap.png" );
	
	m_pGUIMiniMap->Create( fX, fY, fWidth, fHeight, imgParamMiniMap );

	fX		= 50.0f;
	fY		= 800.0f;
	fWidth	= 800.0f;
	fHeight	= 50.0f;

	GUIBase::IMAGEPARAM imgParamEdit;
	m_pGUIBase->AddFileName( 0, imgParamEdit, L"Img\\MainScene\\Listbox.png" );
	m_pChatEdit->Create( fX, fY, fWidth, fHeight, imgParamEdit );

	fX		= 50.0f;
	fY		= 300.0f;
	fWidth	= 500.0f;
	fHeight	= 400.0f;
	GUIBase::IMAGEPARAM imgParamChat;
	m_pGUIBase->AddFileName( 0, imgParamChat, L"Img\\MainScene\\Listbox1.png" );
	m_pChatWindow->Create( fX, fY, fWidth, fHeight, imgParamChat );

}

VOID MainGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUICharacterState->Update();
	m_pGUIMiniMap->Update();
	m_pChatWindow->Update();
	m_pChatEdit->Update();

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

	TCHAR Str[ 1024 ];
	if( m_pChatEdit->TakeMessage( Str ) )
		m_pChatWindow->AddItem( Str );
}

VOID MainGUI::Render()
{
	m_pGUICharacterState->Render();
	m_pGUIMiniMap->Render();
	m_pChatWindow->Render();
	m_pChatEdit->Render();
}

VOID MainGUI::OnDown( INT x, INT y )
{
	m_pChatWindow->OnDown( x, y );
}

VOID MainGUI::OnMove( INT x, INT y )
{
	m_pChatWindow->OnMove( x, y );
}

VOID MainGUI::OnUp( INT x, INT y )
{
	m_pChatWindow->OnUp( x, y );
}

VOID MainGUI::Command( DWORD& _dOut )
{
}

BOOL MainGUI::NextBackgroundImage()
{
	return FALSE;
}
