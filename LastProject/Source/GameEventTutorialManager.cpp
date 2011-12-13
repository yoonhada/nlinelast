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
		break;
	case GameEventTutorial::TUTORIAL_FIRSTAID:
		break;
	case GameEventTutorial::TUTORIAL_SIGNALLIGHT:
		break;
	}

}

VOID GameEventTutorialManager::Create()
{
}

VOID GameEventTutorialManager::Update()
{
	if( m_pGameEventTutorial == NULL )
		return;

	m_pGameEventTutorial->Update();
}

VOID GameEventTutorialManager::Render()
{
	if( m_pGameEventTutorial == NULL )
		return;

	m_pGameEventTutorial->Render();
}