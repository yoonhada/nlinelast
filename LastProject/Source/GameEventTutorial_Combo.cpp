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
	SAFE_DELETE( m_pGUIBase );
	SAFE_DELETE( m_pGUIBackground ); 
}

VOID GameEventTutorial_Combo::Create()
{
	m_dCurrentState	= TUTORIAL_RUNNING;

	FLOAT fX		= 901.0f;
	FLOAT fY		= 534.0f;
	FLOAT fWidth	= 379.0f;
	FLOAT fHeight	= 490.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\Event\\Tutorial\\TutorialCombo.png" );

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