#include "stdafx.h"
#include "MainGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIListbox.h"
#include "GUIEditManager.h"

VOID MainGUI::Initialize()
{
	m_pMouse				= new Mouse;
	m_pGUIBase				= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUICharacterState	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pChatWindow			= new GUIListbox( m_pD3dDevice, m_pSprite );
	m_pChatEdit				= new GUIEditManager( m_pD3dDevice, m_pSprite );
	
	m_pMiniMap				= new GUIBackground( m_pD3dDevice, m_pSprite );
	for( INT i=0 ; i<MMP_END ; i++ )
		m_pMmpObject[ i ] = new GUIBackground( m_pD3dDevice, m_pSprite );
}

VOID MainGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUICharacterState;
	delete m_pChatWindow;
	delete m_pChatEdit;
	delete m_pMiniMap;
	for( INT i=0 ; i<MMP_END ; i++ )
		delete m_pMmpObject[ i ];
}

VOID MainGUI::CreateMiniMap()
{
	FLOAT fX		= 1050.0f;
	FLOAT fY		= 100.0f;
	FLOAT fWidth	= 208.0f;
	FLOAT fHeight	= 340.0f;
	
	GUIBase::IMAGEPARAM imgParamMiniMap;
	m_pGUIBase->AddFileName( 0, imgParamMiniMap, L"Img\\MainScene\\MiniMap.png" );

	m_pMiniMap->Create( fX, fY, fWidth, fHeight, imgParamMiniMap );

	fX		= 0.0f;
	fY		= 0.0f;
	fWidth	= 20.0f;
	fHeight	= 20.0f;

	GUIBase::IMAGEPARAM imgParamMom;
	m_pGUIBase->AddFileName( 0, imgParamMom, L"Img\\Event\\EventM-2.png" );

	m_pMmpObject[ MMP_MOM ]->Create( fX, fY, fWidth, fHeight, imgParamMom );

	GUIBase::IMAGEPARAM imgParamDaddy;
	m_pGUIBase->AddFileName( 0, imgParamDaddy, L"Img\\Event\\EventF-2.png" );

	m_pMmpObject[ MMP_DADDY ]->Create( fX, fY, fWidth, fHeight, imgParamDaddy );

	GUIBase::IMAGEPARAM imgParamSon;
	m_pGUIBase->AddFileName( 0, imgParamSon, L"Img\\Event\\EventS-2.png" );

	m_pMmpObject[ MMP_SON ]->Create( fX, fY, fWidth, fHeight, imgParamSon );

	GUIBase::IMAGEPARAM imgParamDaughter;
	m_pGUIBase->AddFileName( 0, imgParamDaughter, L"Img\\Event\\EventD-2.png" );

	m_pMmpObject[ MMP_DAUGHTER ]->Create( fX, fY, fWidth, fHeight, imgParamDaughter );

	GUIBase::IMAGEPARAM imgParamFirstAidKit0;
	m_pGUIBase->AddFileName( 0, imgParamFirstAidKit0, L"Img\\MainScene\\FirstAidKit.png" );

	m_pMmpObject[ MMP_FIRSTAIDKIT_0 ]->Create( fX, fY, fWidth, fHeight, imgParamFirstAidKit0 );

	GUIBase::IMAGEPARAM imgParamFirstAidKit1;
	m_pGUIBase->AddFileName( 0, imgParamFirstAidKit1, L"Img\\MainScene\\FirstAidKit.png" );

	m_pMmpObject[ MMP_FIRSTAIDKIT_1 ]->Create( fX, fY, fWidth, fHeight, imgParamFirstAidKit1 );

	GUIBase::IMAGEPARAM imgParamFirstAidKit2;
	m_pGUIBase->AddFileName( 0, imgParamFirstAidKit2, L"Img\\MainScene\\FirstAidKit.png" );

	m_pMmpObject[ MMP_FIRSTAIDKIT_2 ]->Create( fX, fY, fWidth, fHeight, imgParamFirstAidKit2 );

	GUIBase::IMAGEPARAM imgParamFirstAidKit3;
	m_pGUIBase->AddFileName( 0, imgParamFirstAidKit3, L"Img\\MainScene\\FirstAidKit.png" );

	m_pMmpObject[ MMP_FIRSTAIDKIT_3 ]->Create( fX, fY, fWidth, fHeight, imgParamFirstAidKit3 );

	GUIBase::IMAGEPARAM imgParamFirstAidKit4;
	m_pGUIBase->AddFileName( 0, imgParamFirstAidKit4, L"Img\\MainScene\\FirstAidKit.png" );

	m_pMmpObject[ MMP_FIRSTAIDKIT_4 ]->Create( fX, fY, fWidth, fHeight, imgParamFirstAidKit4 );

	GUIBase::IMAGEPARAM imgParamCrown;
	m_pGUIBase->AddFileName( 0, imgParamCrown, L"Img\\MainScene\\Crown.png" );

	m_pMmpObject[ MMP_CROWN ]->Create( fX, fY, fWidth, fHeight, imgParamCrown );

	GUIBase::IMAGEPARAM imgParamPanda;
	m_pGUIBase->AddFileName( 0, imgParamPanda, L"Img\\MainScene\\Panda.png" );

	m_pMmpObject[ MMP_PANDA ]->Create( fX, fY, fWidth, fHeight, imgParamPanda );

	GUIBase::IMAGEPARAM imgParamBear;
	m_pGUIBase->AddFileName( 0, imgParamBear, L"Img\\MainScene\\Bear.png" );

	m_pMmpObject[ MMP_BEAR ]->Create( fX, fY, fWidth, fHeight, imgParamBear );

	//	Set MiniMap Info
	//m_fMapWidth		= 1676.6f;
	//m_fMapHeight	= 938.1f;
	m_fMapWidth		= 1138.0f;
	m_fMapHeight	= 1800.0f;
	
	m_fMmpX			= 1050.0f;
	m_fMmpY			= 100.0f;
	m_fMmpWidth		= 208.0f;
	m_fMmpHeight	= 340.0f;

	m_fMmpRatioX	= m_fMmpWidth / m_fMapWidth;
	m_fMmpRatioY	= m_fMmpHeight / m_fMapHeight;

}

VOID MainGUI::Create()
{
	m_pMouse->Initialize( m_hWnd );
	
	FLOAT fX		= 30.0f;
	FLOAT fY		= 30.0f;
	FLOAT fWidth	= 654.0f;
	FLOAT fHeight	= 224.0f;

	GUIBase::IMAGEPARAM imgParamCharacterState;

	m_pGUIBase->AddFileName( 0, imgParamCharacterState, L"Img\\MainScene\\Character_State.png" );
	
	m_pGUICharacterState->Create( fX, fY, fWidth, fHeight, imgParamCharacterState );

	fX		= 50.0f;
	fY		= 800.0f;
	fWidth	= 800.0f;
	fHeight	= 50.0f;

	GUIBase::IMAGEPARAM imgParamEdit;
	m_pGUIBase->AddFileName( 0, imgParamEdit, L"Img\\MainScene\\Edit.png" );
	
	GUIBase::IMAGEPARAM imgParamCaret;
	m_pGUIBase->AddFileName( 0, imgParamCaret, L"Img\\Caret0.png" );
	m_pGUIBase->AddFileName( 0, imgParamCaret, L"Img\\Listbox1.png" );

	m_pChatEdit->Create(	MAIN_CHAT,
							GUIEditManager::EDT_DYNAMIC,
							fX, fY,
							fWidth, 
							fHeight, 
							imgParamEdit,
							imgParamCaret );
	m_pChatEdit->SetFont( MAIN_CHAT, L"궁서", 20, 20 );
	
	fX		= 50.0f;
	fY		= 300.0f;
	fWidth	= 500.0f;
	fHeight	= 400.0f;
	GUIBase::IMAGEPARAM imgParamChat;
	m_pGUIBase->AddFileName( 0, imgParamChat, L"Img\\MainScene\\Edit.png" );
	m_pChatWindow->Create( fX, fY, fWidth, fHeight, imgParamChat );
	m_pChatWindow->SetFont( L"궁서", 20, 20, 0x00ffffff );

	CreateMiniMap();

}

VOID MainGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUICharacterState->Update();
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
	if( m_pChatEdit->TakeMessage( MAIN_CHAT, Str ) )
		m_pChatWindow->AddItem( Str );
}

VOID MainGUI::Render()
{
	m_pGUICharacterState->Render();
	m_pChatWindow->Render();
	m_pChatEdit->Render();
	
	//	MiniMap
	m_pMiniMap->Render();
	for( INT i=0 ; i<MMP_END ; i++ )
		m_pMmpObject[ i ]->Render();
}

VOID MainGUI::OnDown( INT x, INT y )
{
	m_pChatWindow->OnDown( x, y );
	m_pChatEdit->OnDown( x, y );
}

VOID MainGUI::OnMove( INT x, INT y )
{
	m_pChatWindow->OnMove( x, y );
	m_pChatEdit->OnMove( x, y );
}

VOID MainGUI::OnUp( INT x, INT y )
{
	m_pChatWindow->OnUp( x, y );
	m_pChatEdit->OnUp( x, y );
}

VOID MainGUI::Command( DWORD& _dOut )
{
}

BOOL MainGUI::NextBackgroundImage()
{
	return FALSE;
}

VOID MainGUI::SetMiniMapObjectVisible( INT _iIndex, BOOL _bVisible )
{
	DWORD dState = _bVisible ? GUIBackground::BGD_NORMAL : GUIBackground::BGD_HIDDEN;

	m_pMmpObject[ _iIndex ]->SetState( dState );
}

VOID MainGUI::SetMiniMapObjectPosition( INT _iIndex, LPD3DXVECTOR3 _pvecPosition )
{
	D3DXVECTOR3 vecPosition;
	vecPosition.x = m_fMmpX + ( m_fMmpRatioX * _pvecPosition->x + m_fMmpWidth * 0.5f);
	vecPosition.y = m_fMmpY + m_fMmpHeight - ( m_fMmpRatioY * _pvecPosition->z + m_fMmpHeight * 0.5f);
	vecPosition.z = 0.0f;

	m_pMmpObject[ _iIndex ]->SetPosition( &vecPosition );

	if( _iIndex == MMP_MOM )
		CDebugConsole::GetInstance()->Messagef( L"MOM : %f %f %f\n", vecPosition.x, vecPosition.y, vecPosition.z );
}