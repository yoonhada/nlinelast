#include "Stdafx.h"
#include "HelpGUI.h"
#include "Mouse.h"
#include "GUIBackground.h"
#include "GUIListbox.h"

VOID HelpGUI::Initialize()
{
	m_pMouse			= new Mouse;
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
	m_pGUIListbox		= new GUIListbox( m_pD3dDevice, m_pSprite );
}

VOID HelpGUI::Release()
{
	delete m_pMouse;
	delete m_pGUIBase;
	delete m_pGUIBackground;
	delete m_pGUIListbox;
}

VOID HelpGUI::CreateBackground()
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

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\HelpScene\\Background.png" );
	
	m_pGUIBackground->Create( imgParam );
}

VOID HelpGUI::CreateListbox()
{
	FLOAT fX		= m_fX + 100.0f;
	FLOAT fY		= m_fY + 100.0f;
	FLOAT fWidth	= 200.0f;
	FLOAT fHeight	= 400.0f;

	GUIBase::IMAGEPARAM imgBackground;

	m_pGUIBase->AddFileName( 0, imgBackground, L"Img\\HelpScene\\Help_Listbox.png" );

	m_pGUIListbox->Create( fX, fY, fWidth, fHeight, imgBackground );

	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
	m_pGUIListbox->AddItem( L"NULL값은 들어 갔나요?" );
}

VOID HelpGUI::Create()
{
	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	m_fWidth	= 512.0f;
	m_fHeight	= 512.0f;
	m_fX		= static_cast<FLOAT>( Vp.Width ) * 0.5f - m_fWidth * 0.5f;
	m_fY		= static_cast<FLOAT>( Vp.Height ) * 0.5f - m_fHeight * 0.5f;

	m_pMouse->Initialize( m_hWnd );

	CreateBackground();
	CreateListbox();
}

VOID HelpGUI::Update()
{
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBackground->Update();
	m_pGUIListbox->Update( pt.x, pt.y );

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

VOID HelpGUI::Render()
{
	m_pGUIBackground->Render();
	m_pGUIListbox->Render();
}

VOID HelpGUI::OnDown( INT x, INT y )
{
	m_pGUIListbox->OnDown( x, y );
}

VOID HelpGUI::OnMove( INT x, INT y )
{
	m_pGUIListbox->OnMove( x, y );
}

VOID HelpGUI::OnUp( INT x, INT y )
{
	m_pGUIListbox->OnUp( x, y );
}

VOID HelpGUI::Command( DWORD& _dOut )
{
}

BOOL HelpGUI::NextBackgroundImage()
{
	return m_pGUIBackground->NextAnimation();
}
