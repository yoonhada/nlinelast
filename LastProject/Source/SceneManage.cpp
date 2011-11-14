#include "Stdafx.h"
#include "SceneManage.h"

#include "Scene.h"
#include "MainScene.h"
#include "LoadScene.h"
#include "LobbyScene.h"

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
	m_pScene = NULL;
	m_pPrevScene = NULL;
	m_pNextScene = NULL;
	m_pLoadScene = NULL;
	m_hThread = NULL;
}

HRESULT CSceneManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	CObjectManage::GetInstance()->Create( m_pD3dDevice );

	m_pScene = new CLobbyScene;
	m_pLoadScene = m_pScene;
	m_pScene->Create( m_pD3dDevice );

	return S_OK;
}

HRESULT CSceneManage::Release()
{
	if( m_pScene == m_pLoadScene )
	{
		SAFE_DELETE(m_pScene);
	}
	else
	{
		SAFE_DELETE(m_pScene);
		SAFE_DELETE(m_pLoadScene);
	}
	SAFE_DELETE(m_pPrevScene);
	//SAFE_DELETE(m_pNextScene);

	CObjectManage::DestoryInstance();

	return S_OK;
}

UINT WINAPI CSceneManage::ThreadFunc(LPVOID lParam)
{
	CDebugConsole::GetInstance()->Messagef( L"**Thread Loading Start**" );
	GetInstance()->m_pNextScene->Create( GetInstance()->m_pD3dDevice );
	CDebugConsole::GetInstance()->Messagef( L"**Thread Loading End**" );

	return 0;
}

BOOL CSceneManage::OrderChangeScene( IScene* a_pScene )
{
	//ÇöÀç¾À ÀúÀå
	//m_pPrevScene = m_pScene;
	//º¯°æµÉ ¾À ÀúÀå
	m_pNextScene = a_pScene;
	//ÇöÀç ¾À ·Îµù¾ÀÀ¸·Î º¯°æ
	m_pScene = m_pLoadScene;

	//m_bThreadOn = TRUE;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &m_uiThreadID);
	if(m_hThread == 0)
	{
		MessageBox(NULL, L"Create Thread Error",NULL, MB_OK);
	}
	//CloseHandle(m_hThread);

	return TRUE;
}

VOID CSceneManage::ChangeScene()
{
	m_pScene = m_pNextScene;
}

VOID CSceneManage::Update()
{
	if( WAIT_OBJECT_0 == WaitForSingleObject( m_hThread, 1 ) )
	{
		ChangeScene();
	}

	m_pScene->Update();
}

VOID CSceneManage::Render()
{
	m_pScene->Render();
}