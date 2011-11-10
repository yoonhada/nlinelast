#include "Stdafx.h"
#include "SceneManage.h"

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

}

HRESULT CSceneManage::Create()
{
	/*m_pScene = CMainManage::GetInstance();
	m_pScene->Create( m_pD3dDevice );*/

	return S_OK;
}

HRESULT CSceneManage::Release()
{
	return S_OK;
}

BOOL CSceneManage::ChangeScene( CManage* a_pScene )
{
	
	/*m_pScene->DestoryInstance();

	m_pScene = a_pScene->GetInstance();

	m_pScene->Create( m_pD3dDevice );*/

	return TRUE;
}

VOID CSceneManage::Update()
{
	m_pScene->Update();
}

VOID CSceneManage::Render()
{
	m_pScene->Render();
}