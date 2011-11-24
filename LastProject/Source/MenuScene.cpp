#include "Stdafx.h"
#include "MenuScene.h"
#include "MenuGUI.h"
#include "OptionScene.h"
#include "LoginScene.h"

VOID MenuScene::Initialize()
{
	m_scnNext			= SCENE_MAIN;
	m_scnState			= SCENE_RUNNING;

	m_bLoginScene		= FALSE;

	m_pMenuGUI			= NULL;
	m_pOptionScene		= NULL;
	m_pLoginScene		= NULL;
}

VOID MenuScene::Release()
{
	SAFE_DELETE( m_pMenuGUI );
	SAFE_DELETE( m_pOptionScene );
	SAFE_DELETE( m_pLoginScene );
}

HRESULT MenuScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pMenuGUI		= new MenuGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pOptionScene	= new OptionScene;
	m_pLoginScene	= new LoginScene;

	m_pMenuGUI->Create();
	m_pOptionScene->Create( _pd3dDevice, _pSprite, _hWnd );
	m_pLoginScene->Create( _pd3dDevice, _pSprite, _hWnd );

	return S_OK;
}
VOID MenuScene::Update()
{
	m_pMenuGUI->Update();
	m_pOptionScene->Update();

	if( m_bLoginScene )
		m_pLoginScene->Update();

	DWORD dID;
	m_pMenuGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_MENU_START:
		m_bLoginScene = !m_bLoginScene;
		break;
	case GUIBTN_MENU_END:
		m_scnState = IScene::SCENE_END;
		break;
	}
}

VOID MenuScene::Render()
{
	m_pMenuGUI->Render();
	m_pOptionScene->Render();

	if( m_bLoginScene )
		m_pLoginScene->Render();
}

INT MenuScene::GetSceneNext()
{
	return m_scnNext;
}
INT MenuScene::GetSceneState()
{
	return m_scnState;
}