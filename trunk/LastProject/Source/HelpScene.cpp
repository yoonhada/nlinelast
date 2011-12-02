#include "Stdafx.h"
#include "HelpScene.h"
#include "HelpGUI.h"

VOID HelpScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pHelpGUI		= NULL;
}

VOID HelpScene::Release()
{
	SAFE_DELETE( m_pHelpGUI );
}

HRESULT HelpScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pHelpGUI		= new HelpGUI( _pd3dDevice, _pSprite, _hWnd );

	m_pHelpGUI->Create();

	return S_OK;
}

VOID HelpScene::Update()
{
	m_pHelpGUI->Update();

	
	DWORD dID;
	m_pHelpGUI->Command( dID );
	switch( dID )
	{
	case OPTION_SETUP:
		break;
	case OPTION_HELP:
		break;
	}
}

VOID HelpScene::Render()
{
	m_pHelpGUI->Render();
}

INT HelpScene::GetSceneNext()
{
	return m_scnNext;
}

INT HelpScene::GetSceneState()
{
	return m_scnState;
}