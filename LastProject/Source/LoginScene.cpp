#include "Stdafx.h"
#include "LoginScene.h"
#include "LoginGUI.h"

VOID LoginScene::Initialize()
{
	m_scnNext		= SCENE_MENU;
	m_scnState		= SCENE_RUNNING;

	m_pLoginGUI		= NULL;

	//	Popup창이라 생각하자
	m_bActivate		= FALSE;
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
	if( !m_bActivate )
		return;

	m_pLoginGUI->Update();

	TCHAR Str[ 1024 ];
	ZeroMemory( Str, sizeof( Str ) );

	DWORD dID;
	m_pLoginGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_LOGIN_GO:
		m_pLoginGUI->GetEditTexture( Str );
		MessageBox( NULL, Str, NULL, MB_OK );
		m_scnState = IScene::SCENE_END;
		break;
	case GUIBTN_LOGIN_EXIT:
		m_bActivate = FALSE;
		break;
	}
}

VOID LoginScene::Render()
{
	if( !m_bActivate )
		return;

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

VOID LoginScene::SetActivate( BOOL _bActivate )
{
	m_bActivate = _bActivate;
}