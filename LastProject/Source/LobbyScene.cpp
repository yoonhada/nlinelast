#include "Stdafx.h"
#include "LobbyScene.h"

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

	// 네트워크
#ifndef _NETWORK
	WSAStartup( MAKEWORD( 2, 2), &m_wsadata );
	m_pNetwork = CNetwork::GetInstance();
	m_pNetwork->CreateSocket();
#endif

	// 파일에서 IP 받아오기
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

#ifndef _NETWORK
	m_pNetwork->ConnectToServer( szTemp, 20202 );
	m_pNetwork->csLOGON();
#endif
	
	return S_OK;
}
HRESULT CLobbyScene::Release()
{
	return S_OK;
}
VOID CLobbyScene::Update()
{
#ifndef _NETWORK
	m_pNetwork->Update();
#endif
}

VOID CLobbyScene::Render()
{

}