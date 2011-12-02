#include "Stdafx.h"
#include "SetupScene.h"
#include "SetupGUI.h"

VOID SetupScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pSetupGUI		= NULL;
}

VOID SetupScene::Release()
{
	SAFE_DELETE( m_pSetupGUI );
}

HRESULT SetupScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pSetupGUI		= new SetupGUI( _pd3dDevice, _pSprite, _hWnd );

	m_pSetupGUI->Create();

	return S_OK;
}

VOID SetupScene::Update()
{
	m_pSetupGUI->Update();

	
	DWORD dID;
	m_pSetupGUI->Command( dID );
	switch( dID )
	{
	case OPTION_SETUP:
		break;
	case OPTION_HELP:
		break;
	}
}

VOID SetupScene::Render()
{
	m_pSetupGUI->Render();
}

INT SetupScene::GetSceneNext()
{
	return m_scnNext;
}

INT SetupScene::GetSceneState()
{
	return m_scnState;
}