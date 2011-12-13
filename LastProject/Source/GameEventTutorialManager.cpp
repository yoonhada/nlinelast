#include "stdafx.h"
#include "GameEventTutorialManager.h"
#include "GameEventTutorial_Combo.h"
#include "GameEventTutorial_FirstAid.h"
#include "GameEventTutorial_SignalLight.h"

VOID GameEventTutorialManager::Initialize()
{
	m_pGameEventTutorial	= NULL;
}

VOID GameEventTutorialManager::Release()
{
	SAFE_DELETE( m_pGameEventTutorial );
}

VOID GameEventTutorialManager::ChangeEvent( DWORD _dNextEvent )
{
	SAFE_DELETE( m_pGameEventTutorial );

	switch( _dNextEvent )
	{
	case GameEventTutorial::TUTORIAL_COMBO:
		m_pGameEventTutorial = new GameEventTutorial_Combo( m_pd3dDevice, m_pSprite );
		break;
	case GameEventTutorial::TUTORIAL_FIRSTAID:
		m_pGameEventTutorial = new GameEventTutorial_FirstAid( m_pd3dDevice, m_pSprite );
		break;
	case GameEventTutorial::TUTORIAL_SIGNALLIGHT:
		m_pGameEventTutorial = new GameEventTutorial_SignalLight( m_pd3dDevice, m_pSprite );
		break;
	}

	if( m_pGameEventTutorial != NULL )
		m_pGameEventTutorial->Create();

}

VOID GameEventTutorialManager::EndEvent()
{
	SAFE_DELETE( m_pGameEventTutorial );
}

VOID GameEventTutorialManager::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite )
{
	m_pd3dDevice	= _pd3dDevice;
	m_pSprite		= _pSprite;
}

VOID GameEventTutorialManager::Update()
{
	if( m_pGameEventTutorial == NULL )
		return;

	if( m_pGameEventTutorial->GetState() == GameEventTutorial::TUTORIAL_END )
		this->EndEvent();
	
	m_pGameEventTutorial->Update();
}

VOID GameEventTutorialManager::Render()
{
	if( m_pGameEventTutorial == NULL )
		return;

	m_pGameEventTutorial->Render();
}