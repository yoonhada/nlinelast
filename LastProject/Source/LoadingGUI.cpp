#include "stdafx.h"
#include "LoadingGUI.h"
#include "GUIBackground.h"

VOID LoadingGUI::Initialize()
{
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
}

VOID LoadingGUI::Release()
{
	delete m_pGUIBase;
	delete m_pGUIBackground;
}

VOID LoadingGUI::CreateBackground()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParam;

	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_0.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_1.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_2.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_3.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_4.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_5.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_6.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_7.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_8.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_9.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_10.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_11.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_12.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_13.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_14.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_15.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_16.jpg", 50 );
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\LoadingScene\\Loading_17.jpg", 50 );


	m_pGUIBackground->Create( 0.0f, 0.0f, fWidth, fHeight, imgParam );
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
