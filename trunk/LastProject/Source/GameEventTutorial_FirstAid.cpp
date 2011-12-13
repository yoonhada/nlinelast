#include "stdafx.h"
#include "GameEventTutorial_FirstAid.h"
#include "GUIBackground.h"

VOID GameEventTutorial_FirstAid::Initialize()
{
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
}

VOID GameEventTutorial_FirstAid::Release()
{
	SAFE_DELETE( m_pGUIBase );
	SAFE_DELETE( m_pGUIBackground );
}

VOID GameEventTutorial_FirstAid::Create()
{
	m_dCurrentState	= TUTORIAL_RUNNING;

	FLOAT fX		= 800.0f;
	FLOAT fY		= 400.0f;
	FLOAT fWidth	= 200.0f;
	FLOAT fHeight	= 200.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\MainScene\\TutorialFirstAid.png" );

	m_pGUIBackground->Create( fX, fY, fWidth, fHeight, imgParam );
}

VOID GameEventTutorial_FirstAid::Update()
{
	m_pGUIBackground->Update();
}

VOID GameEventTutorial_FirstAid::Render()
{
	m_pGUIBackground->Render();
}

DWORD GameEventTutorial_FirstAid::GetState()
{
	return m_dCurrentState;
}

VOID GameEventTutorial_FirstAid::SetState( DWORD _dState )
{
	m_dCurrentState = _dState;
}