#include "stdafx.h"
#include "LoadingScene.h"
#include "LoadingGUI.h"
#include "Monster.h"

VOID LoadingScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pLoadingGUI	= NULL;

	m_pClown		= NULL;
	m_pPanda		= NULL;
}

VOID LoadingScene::Release()
{
	SAFE_DELETE( m_pLoadingGUI );
	SAFE_DELETE( m_pClown );
	SAFE_DELETE( m_pPanda );
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