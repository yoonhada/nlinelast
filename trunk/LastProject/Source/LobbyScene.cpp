#include "stdafx.h"
#include "LobbyScene.h"
#include "LobbyGUI.h"
#include "OptionScene.h"
#include "Charactor.h"

VOID LobbyScene::Initialize()
{
	m_scnNext		= SCENE_MAIN;
	m_scnState		= SCENE_RUNNING;

	m_pLobbyGUI		= NULL;
	m_pOptionScene	= NULL;

	m_aData			= NULL;
	m_fChrRotate	= 3.14f;
	m_fIncRotate	= 0.005f;

	m_pLight		= NULL;
	m_pMatrices		= NULL;

}

VOID LobbyScene::Release()
{
	SAFE_DELETE( m_pLobbyGUI );
	SAFE_DELETE( m_pOptionScene );

	for( INT i=0 ; i<4 ; i++ )
		SAFE_DELETE( m_aData[ i ].pCharacter );
	SAFE_DELETE_ARRAY( m_aData );

	SAFE_DELETE( m_pLight );
	//SAFE_DELETE( m_pMatrices );
	//CTree::DestoryInstance();
}

VOID LobbyScene::CreateData( LPDATA _pData, LPWSTR _pFileName, D3DXVECTOR3 _vecPosition )
{
	SAFE_DELETE( _pData->pCharacter );

	_pData->pCharacter = new CCharactor;

	_pData->pCharacter->Create( m_pD3dDevice );
	_pData->pCharacter->Load( _pFileName );
	_pData->pCharacter->Set_Position( _vecPosition );

	_pData->vecPosition = _vecPosition;

}
HRESULT LobbyScene::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite, HWND _hWnd )
{
	m_pD3dDevice	= _pd3dDevice;

	//	Create GUI
	m_pLobbyGUI		= new LobbyGUI( _pd3dDevice, _pSprite, _hWnd );
	m_pOptionScene	= new OptionScene;
	
	m_pLobbyGUI->Create();
	m_pOptionScene->Create( _pd3dDevice, _pSprite, _hWnd );

	//	Create Character
	m_aData		= new DATA[ 4 ];

	CreateData( &m_aData[ 0 ], L"Data\\CharData\\MOM.csav",		D3DXVECTOR3( 24.0f, 0.0f, 100.0f ) );
	CreateData( &m_aData[ 1 ], L"Data\\CharData\\APPA_0.csav",	D3DXVECTOR3( 8.0f, 0.0f, 100.0f ) );
	CreateData( &m_aData[ 2 ], L"Data\\CharData\\ADDLE_0.csav", D3DXVECTOR3( -8.0f, 0.0f, 100.0f ) );
	CreateData( &m_aData[ 3 ], L"Data\\CharData\\DDAL_0.csav",	D3DXVECTOR3( -24.0f, 0.0f, 100.0f ) );
	

	//CTree::GetInstance()->Create( 0, 0 );

	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( _pd3dDevice );

	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();	FILE* pFile;

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

	WSADATA wsadata;
	WSAStartup( MAKEWORD( 2, 2), &wsadata );
	CNetwork::GetInstance()->CreateSocket();
	CNetwork::GetInstance()->ConnectToServer( szTemp, 20202 );
	CNetwork::GetInstance()->CS_LOGON();

	return S_OK;
}

VOID LobbyScene::ActivateRotate( INT _iIndex )
{
	if( m_aData[ _iIndex ].bRotate )
		return;

	for( INT i=0 ; i<4 ; i++ )
		m_aData[ i ].bRotate = FALSE;

	m_aData[ _iIndex ].bRotate	= TRUE;
	
	m_fChrRotate = 3.14f;
	m_fIncRotate = 0.005f;
}

VOID LobbyScene::Update()
{
	m_pLobbyGUI->Update();
	m_pOptionScene->Update();

	for( INT i=0 ; i<4 ; i++ )
		m_aData[ i ].pCharacter->UpdateOtherPlayer( TRUE );
	

	DWORD dID;
	m_pLobbyGUI->Command( dID );
	switch( dID )
	{
	case GUIBTN_LOBBY_START:
		m_scnState = IScene::SCENE_END;
		break;
	case GUIBTN_LOBBY_BACK:
		m_scnNext	= IScene::SCENE_MENU;
		m_scnState	= IScene::SCENE_END;
		break;
	case GUIBTN_LOBBY_SELECT_1:
		ActivateRotate( 0 );
		break;
	case GUIBTN_LOBBY_SELECT_2:
		ActivateRotate( 1 );
		break;
	case GUIBTN_LOBBY_SELECT_3:
		ActivateRotate( 2 );
		break;
	case GUIBTN_LOBBY_SELECT_4:
		ActivateRotate( 3 );
		break;
	}
	
	if( m_fChrRotate > 314000.0f )
		m_fChrRotate = 3.14f;

	m_fChrRotate += m_fIncRotate;

	for( INT i=0 ; i<4 ; i++ )
	{
		if( m_aData[ i ].bRotate )
			m_aData[ i ].pCharacter->UpdateByValue( m_aData[ i ].vecPosition, m_fChrRotate );
		else
			m_aData[ i ].pCharacter->UpdateByValue( m_aData[ i ].vecPosition, 3.14f );
	}

}

VOID LobbyScene::Render()
{
	static FLOAT fX = 0.0f, fY = 0.0f, fZ = 180.0f;
	
	D3DXVECTOR3		vecEyePt( fX, fY, fZ );
	D3DXVECTOR3		vecLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3		vecUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16	matView;
	D3DXMatrixLookAtLH( &matView, &vecEyePt, &vecLookatPt, &vecUpVec );
	m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
	m_pMatrices->SetupProjection();
	m_pLight->EnableLight();

	for( INT i=0 ; i<4 ; i++ )
		m_aData[ i ].pCharacter->Render();

	m_pLight->DisableLight();
	m_pLobbyGUI->Render();
	m_pOptionScene->Render();

}

INT LobbyScene::GetSceneNext()
{
	return m_scnNext;
}

INT LobbyScene::GetSceneState()
{
	return m_scnState;
}