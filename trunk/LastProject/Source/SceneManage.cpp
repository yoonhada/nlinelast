#include "Stdafx.h"
#include "SceneManage.h"
#include "Scene.h"
#include "LoadingScene.h"
#include "LogoScene.h"
#include "StoryScene.h"
#include "MenuScene.h"
#include "LobbyScene.h"
#include "MainScene.h"
//#include "LoadScene.h"
#include "LogoScene.h"

#define _TEST

CSceneManage::CSceneManage()
{
	Clear();
}

CSceneManage::~CSceneManage()
{
	Release();
}

VOID CSceneManage::Clear()
{
	m_pScene		= NULL;
	m_pLoadingScene = NULL;
	//m_pPrevScene = NULL;
	//m_pNextScene = NULL;
	//m_pLoadScene = NULL;
	m_hThread		= NULL;
}

HRESULT CSceneManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	//	MainScene¿¡¼­ ´Ù½Ã ÄÒ´Ù
	CInput::GetInstance()->EnableInput( TRUE );

	m_pD3dDevice = a_pD3dDevice;
	
	//	Create the Sprite
	if( FAILED( D3DXCreateSprite( m_pD3dDevice, &m_pSprite ) ) )
	{
		MessageBox( NULL, L"WinClass::CreateSprite() failed", NULL, MB_OK );
		return E_FAIL;
	}

	m_hWnd = CWinBase::GetInstance()->Get_hWnd();

	CObjectManage::GetInstance()->Create( m_pD3dDevice );

	m_pScene = new LobbyScene;
	//m_pLoadScene = m_pScene;
#ifdef _TEST
	//m_pScene			= new CMainScene;
#else
	m_pScene			= new LogoScene;
#endif // _DEBUG	
	m_pLoadingScene		= new LoadingScene;

	m_pScene->Create( m_pD3dDevice, m_pSprite, m_hWnd );
	m_pLoadingScene->Create( m_pD3dDevice, m_pSprite, m_hWnd );

	return S_OK;
}

HRESULT CSceneManage::Release()
{
	//if( m_pScene == m_pLoadScene )
	//{
	//	SAFE_DELETE(m_pScene);
	//}
	//else
	//{
	//	SAFE_DELETE(m_pScene);
	//	SAFE_DELETE(m_pLoadScene);
	//}
	//SAFE_DELETE(m_pPrevScene);
	//SAFE_DELETE(m_pNextScene);

	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_pLoadingScene );

	SAFE_RELEASE( m_pSprite );

	CObjectManage::DestoryInstance();

	return S_OK;
}

UINT WINAPI CSceneManage::ThreadFunc(LPVOID lParam)
{
	CDebugConsole::GetInstance()->Messagef( L"**Thread Loading Start**\n" );
	//GetInstance()->m_pNextScene->Create( GetInstance()->m_pD3dDevice );
	//GetInstance()->m_pLoadingScene->Render();
	GetInstance()->m_pNextScene->Create( GetInstance()->m_pD3dDevice, GetInstance()->m_pSprite, GetInstance()->m_hWnd );
	CDebugConsole::GetInstance()->Messagef( L"**Thread Loading End**\n" );

	return 0;
}

//BOOL CSceneManage::OrderChangeScene( IScene* a_pScene )
//{
//	//ÇöÀç¾À ÀúÀå
//	//m_pPrevScene = m_pScene;
//	//º¯°æµÉ ¾À ÀúÀå
//	m_pNextScene = a_pScene;
//	//ÇöÀç ¾À ·Îµù¾ÀÀ¸·Î º¯°æ
//	m_pScene = m_pLoadScene;
//
//	//m_bThreadOn = TRUE;
//	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &m_uiThreadID);
//	if(m_hThread == 0)
//	{
//		MessageBox(NULL, L"Create SceneManage Thread Error",NULL, MB_OK);
//	}
//	//CloseHandle(m_hThread);
//
//	return TRUE;
//}

//VOID CSceneManage::ChangeScene()
//{
//	m_pScene = m_pNextScene;
//}
VOID CSceneManage::ChangeScene( INT _scnNext )
{
	SAFE_DELETE( m_pScene );

	switch( _scnNext )
	{
	case IScene::SCENE_LOGO:
		CDebugConsole::GetInstance()->Messagef( L"SCENE_LOGO\n" );
		m_pNextScene = new LogoScene;
		break;
	case IScene::SCENE_STORY:
		CDebugConsole::GetInstance()->Messagef( L"SCENE_STORY\n" );
		m_pNextScene = new StoryScene;
		break;
	case IScene::SCENE_MENU:
		CDebugConsole::GetInstance()->Messagef( L"SCENE_MENU\n" );
		m_pNextScene = new MenuScene;
		break;
	case IScene::SCENE_LOBBY:
		CDebugConsole::GetInstance()->Messagef( L"SCENE_LOBBY\n" );
		m_pNextScene = new LobbyScene;
		break;
	case IScene::SCENE_MAIN:
		CDebugConsole::GetInstance()->Messagef( L"SCENE_MAIN\n" );
		m_pNextScene = new CMainScene;
		break;
	}

	//m_pNextScene	= m_pScene;
	m_pScene		= m_pLoadingScene;

	//m_bThreadOn = TRUE;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &m_uiThreadID);
	if(m_hThread == 0)
	{
		MessageBox(NULL, L"Create SceneManage Thread Error",NULL, MB_OK);
	}
}
VOID CSceneManage::Update()
{
	if( WAIT_OBJECT_0 == WaitForSingleObject( m_hThread, 1 ) )
	{
		//ChangeScene();
		m_pScene = m_pNextScene;
	}

	if( m_pScene->GetSceneState() == IScene::SCENE_END )
	{
		ChangeScene( m_pScene->GetSceneNext() );
		return;
	}

	m_pScene->Update();
}

VOID CSceneManage::Render()
{
	//WaitForSingleObject( m_hThread, 1 );

	m_pScene->Render();
}