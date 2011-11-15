#include "stdafx.h"
#include "GUI.h"
#include "Mouse.h"
#include "GUIBtnManager.h"

VOID GUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pMouse->Initialize( m_hWnd );
	m_pGUIBtnManager	= new GUIBtnManager( m_pd3dDevice );
}

VOID GUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBtnManager;
}

VOID GUI::Create( LPBTNDATA _pBtnData )
{	
	m_pGUIBtnManager->Create(	_pBtnData->dID,
								_pBtnData->dStlye,
								_pBtnData->fX, _pBtnData->fY,
								_pBtnData->fWidth, _pBtnData->fHeight,
								_pBtnData->sNormal,
								_pBtnData->sHot, 
								_pBtnData->sDown, 
								_pBtnData->sDisable );
}

VOID GUI::Update()
{
	POINT pt	=	m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBtnManager->Update( pt.x, pt.y );
	
	//	Input Message
	static BOOL bFirst	=	FALSE;

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

VOID GUI::Command( DWORD& _dID )
{
	m_pGUIBtnManager->GetCommandID( _dID );
}

VOID GUI::OnDown( INT x, INT y )
{
	m_pGUIBtnManager->OnDown( x, y );
}

VOID GUI::OnMove( INT x, INT y )
{
	m_pGUIBtnManager->OnMove( x, y );
}

VOID GUI::OnUp( INT x, INT y )
{
	m_pGUIBtnManager->OnUp( x, y );
}

VOID GUI::Render()
{
	m_pGUIBtnManager->Render();
}