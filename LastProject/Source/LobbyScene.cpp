#include "stdafx.h"
#include "LobbyScene.h"
#include "LobbyGUI.h"
#include "OptionScene.h"

VOID LobbyScene::Initialize()
{
	m_scnNext		= SCENE_MAIN;
	m_scnState		= SCENE_RUNNING;

	m_pLobbyGUI		= NULL;
	m_pOptionScene	= NULL;
}

VOID LobbyScene::Release()
{
	SAFE_DELETE( m_pLobbyGUI );
	SAFE_DELETE( m_pOptionScene );
}

HRESULT LobbyScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pLobbyGUI		= new LobbyGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pOptionScene	= new OptionScene;
	
	m_pLobbyGUI->Create();
	m_pOptionScene->Create( _pd3dDevice, _pSprite, _hWnd );

	return S_OK;
}

VOID LobbyScene::Update()
{
	m_pLobbyGUI->Update();
	m_pOptionScene->Update();


	DWORD dID;
	m_pLobbyGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_LOBBY_START:
		m_scnState = IScene::SCENE_END;
		break;
	}
	
}

VOID LobbyScene::Render()
{
	m_pLobbyGUI->Render();
	m_pOptionScene->Render();
}

INT LobbyScene::GetSceneNext()
{
	return m_scnNext;
}

INT LobbyScene::GetSceneState()
{
	return m_scnState;
}