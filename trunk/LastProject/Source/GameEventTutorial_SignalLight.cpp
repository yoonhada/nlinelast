#include "stdafx.h"
#include "GameEventTutorial_SignalLight.h"
#include "GUIBackground.h"

VOID GameEventTutorial_SignalLight::Initialize()
{
	m_pGUIBase			= new GUIBase( m_pd3dDevice, m_pSprite );
	m_pGUIBackground	= new GUIBackground( m_pd3dDevice, m_pSprite );
}

VOID GameEventTutorial_SignalLight::Release()
{
	SAFE_DELETE( m_pGUIBase );
	SAFE_DELETE( m_pGUIBackground ); 
}

VOID GameEventTutorial_SignalLight::Create()
{
	FLOAT fX		= 873.0f;
	FLOAT fY		= 498.0f;
	FLOAT fWidth	= 407.0f;
	FLOAT fHeight	= 526.0f;

	GUIBase::IMAGEPARAM imgParam;
	m_pGUIBase->AddFileName( 0, imgParam, L"Img\\Event\\Tutorial\\TutorialSignalLight.png" );

	m_pGUIBackground->Create( fX, fY, fWidth, fHeight, imgParam );
}

VOID GameEventTutorial_SignalLight::Update()
{
	m_pGUIBackground->Update();
}

VOID GameEventTutorial_SignalLight::Render()
{
	m_pGUIBackground->Render();
}

DWORD GameEventTutorial_SignalLight::GetState()
{
	return m_dCurrentState;
}

VOID GameEventTutorial_SignalLight::SetState( DWORD _dState )
{
	m_dCurrentState = _dState;
}