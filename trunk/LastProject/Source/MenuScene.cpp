#include "Stdafx.h"
#include "MenuScene.h"
#include "MenuGUI.h"
#include "OptionScene.h"
#include "LoginScene.h"

VOID MenuScene::Initialize()
{
	m_scnNext			= SCENE_LOBBY;
	m_scnState			= SCENE_RUNNING;

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

	//FILE* pFile;
	//pFile = _wfopen( L"Data/IP.txt", L"r" );

	//if( NULL == pFile )
	//{
	//	MessageBox(GHWND, L"IP File Load Error", NULL, MB_OK);
	//	return E_FAIL;
	//}

	//fseek( pFile, 0L, SEEK_SET );

	//CHAR szTemp[255];
	//fscanf( pFile, "%s", szTemp );

	//fclose(pFile);

	WSADATA wsadata;
	WSAStartup( MAKEWORD( 2, 2), &wsadata );
	CNetwork::GetInstance()->CreateSocket();
	//CNetwork::GetInstance()->ConnectToServer( szTemp, 20202 );
	//CNetwork::GetInstance()->CS_LOGON( L"NickName" );


	return S_OK;
}
VOID MenuScene::Update()
{
	m_pMenuGUI->Update();
	m_pOptionScene->Update();
	m_pLoginScene->Update();

	DWORD dID;
	m_pMenuGUI->Command( dID );
	switch( dID )
	{
	case MENU_START:
		m_pLoginScene->SetActivate( TRUE );
		break;
	case MENU_END:
		m_scnState = IScene::SCENE_END;
		break;
	}

	if( m_pLoginScene->GetSceneState() == IScene::SCENE_END )
		m_scnState = IScene::SCENE_END;
}

VOID MenuScene::Render()
{
	m_pMenuGUI->Render();
	m_pOptionScene->Render();
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