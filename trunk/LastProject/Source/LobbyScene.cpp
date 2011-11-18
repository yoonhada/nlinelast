#include "Stdafx.h"
#include "LobbyScene.h"

#include "MainScene.h"

CLobbyScene::CLobbyScene()
{
	Clear();
}
CLobbyScene::~CLobbyScene()
{
	Release();
}

VOID CLobbyScene::Clear()
{

}
HRESULT CLobbyScene::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	// ��Ʈ��ũ

	WSAStartup( MAKEWORD( 2, 2), &m_wsadata );
	CNetwork::GetInstance();
	CNetwork::GetInstance()->CreateSocket();


	// ���Ͽ��� IP �޾ƿ���
	FILE* pFile;

	pFile = _wfopen( L"Data/IP.txt", L"r" );

	if( NULL == pFile )
	{
		MessageBox(GHWND, L"IP File Load Error", NULL, MB_OK);
		return E_FAIL;
	}

	fseek( pFile, 0L, SEEK_SET );

	CHAR szTemp[255];
	fscanf( pFile, "%s", szTemp );

	fclose(pFile);

	CNetwork::GetInstance()->ConnectToServer( szTemp, 20202 );
	CNetwork::GetInstance()->CS_LOGON();
	//CSceneManage::GetInstance()->OrderChangeScene( new CMainScene );

	// ������ �Ǿ��ٸ� �α��� �ϰ� ���� ������ ����
	//if( CNetwork::GetInstance()->ConnectToServer( szTemp, 20202 ) )
	//{
	//	CNetwork::GetInstance()->CS_LOGON();
	//	CSceneManage::GetInstance()->OrderChangeScene( new CMainScene );
	//}
	//else
	//{
	//	MessageBox( NULL, L"Connect Failed", NULL, MB_OK );
	//}
	
	return S_OK;
}
HRESULT CLobbyScene::Release()
{
	return S_OK;
}
VOID CLobbyScene::Update()
{
	CNetwork::GetInstance()->Update();
}

VOID CLobbyScene::Render()
{

}