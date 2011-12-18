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
	SAFE_DELETE( m_pGameEventTutorialItem[0] );
	SAFE_DELETE( m_pGameEventTutorialItem[1] );
	SAFE_DELETE( m_pGameEventTutorialItem[2] );
}

VOID GameEventTutorialManager::ChangeEvent( DWORD _dNextEvent )
{
	m_pGameEventTutorial = m_pGameEventTutorialItem[_dNextEvent];
}

VOID GameEventTutorialManager::EndEvent()
{
	//SAFE_DELETE( m_pGameEventTutorial );
	m_pGameEventTutorial = NULL;
}

VOID GameEventTutorialManager::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite )
{
	m_pd3dDevice	= _pd3dDevice;
	m_pSprite		= _pSprite;

	m_pGameEventTutorialItem[0] = new GameEventTutorial_Combo( m_pd3dDevice, m_pSprite );
	m_pGameEventTutorialItem[1] = new GameEventTutorial_FirstAid( m_pd3dDevice, m_pSprite );
	m_pGameEventTutorialItem[2] = new GameEventTutorial_SignalLight( m_pd3dDevice, m_pSprite );

	m_pGameEventTutorialItem[0]->Create();
	m_pGameEventTutorialItem[1]->Create();
	m_pGameEventTutorialItem[2]->Create();
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