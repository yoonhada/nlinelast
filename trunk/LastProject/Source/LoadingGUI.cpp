#include "stdafx.h"
#include "LoadingGUI.h"
#include "GUIBackground.h"

VOID LoadingGUI::Initialize()
{
	m_pGUIBase			= new GUIBase( m_pD3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pD3dDevice, m_pSprite );
}

VOID LoadingGUI::Release()
{
	delete m_pGUIBase;
	delete m_pGUIBackground;
}

VOID LoadingGUI::CreateBackground()
{
	//	Create Background
	GUIBase::IMAGEPARAM imgParam;

	D3DVIEWPORT9 Vp;
	m_pD3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_0.png", 200 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_1.png", 200 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_2.png", 200 );

	m_pGUIBackground->Create( fWidth - 300.0f, fHeight - 300.0f, 300.0f, 300.0f, imgParam );
}

VOID LoadingGUI::Create()
{
	CreateBackground();
}

VOID LoadingGUI::Update()
{
	m_pGUIBackground->Update();
}

VOID LoadingGUI::Render()
{
	m_pGUIBackground->Render();
}
