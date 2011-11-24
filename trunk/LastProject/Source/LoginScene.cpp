#include "Stdafx.h"
#include "LoginScene.h"
#include "LoginGUI.h"

VOID LoginScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pLoginGUI		= NULL;
}

VOID LoginScene::Release()
{
	SAFE_DELETE( m_pLoginGUI );
}

HRESULT LoginScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pLoginGUI		= new LoginGUI( _pd3dDevice, _pSprite, _hWnd );

	m_pLoginGUI->Create();

	return S_OK;
}

VOID LoginScene::Update()
{
	m_pLoginGUI->Update();

	
	DWORD dID;
	m_pLoginGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_LOGIN_GO:
		break;
	}
}

VOID LoginScene::Render()
{
	m_pLoginGUI->Render();
}

INT LoginScene::GetSceneNext()
{
	return m_scnNext;
}

INT LoginScene::GetSceneState()
{
	return m_scnState;
}