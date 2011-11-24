#include "stdafx.h"
#include "LoadingScene.h"
#include "LoadingGUI.h"

VOID LoadingScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pLoadingGUI	= NULL;
}

VOID LoadingScene::Release()
{
	if( m_pLoadingGUI != NULL )
		delete m_pLoadingGUI;
}

HRESULT LoadingScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pLoadingGUI		= new LoadingGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pLoadingGUI->Create();

	return S_OK;
}

VOID LoadingScene::Update()
{
	m_pLoadingGUI->Update();
}

VOID LoadingScene::Render()
{
	m_pLoadingGUI->Render();
}

INT LoadingScene::GetSceneNext()
{
	return m_scnNext;
}

INT LoadingScene::GetSceneState()
{
	return m_scnState;
}