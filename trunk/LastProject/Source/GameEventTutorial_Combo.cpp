#include "stdafx.h"
#include "GameEventTutorial_Combo.h"
#include "GUIBackground.h"

VOID GameEventTutorial_Combo::Initialize()
{
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
}

VOID GameEventTutorial_Combo::Release()
{
	SAFE_DELETE( m_pGUIBackground ); 
}

VOID GameEventTutorial_Combo::Create()
{
	m_dCurrentState	= TUTORIAL_RUNNING;

	FLOAT fX		= 800.0f;
	FLOAT fY		= 400.0f;
	FLOAT fWidth	= 200.0f;
	FLOAT fHeight	= 200.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\MainScene\\TutorialCombo0.png" );
	m_pGUIBase->AddFileName( 1, imgParam, L"Img\\MainScene\\TutorialCombo1.png" );
	m_pGUIBase->AddFileName( 2, imgParam, L"Img\\MainScene\\TutorialCombo2.png" );

	m_pGUIBackground->Create( fX, fY, fWidth, fHeight, imgParam );
}

VOID GameEventTutorial_Combo::Update()
{
	m_pGUIBackground->Update();
}

VOID GameEventTutorial_Combo::Render()
{
	m_pGUIBackground->Render();
}

DWORD GameEventTutorial_Combo::GetState()
{
	return m_dCurrentState;
}

VOID GameEventTutorial_Combo::SetState( DWORD _dState )
{
	m_dCurrentState = _dState;
}