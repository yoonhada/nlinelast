#include "stdafx.h"
#include "LobbyScene.h"
#include "LobbyGUI.h"
#include "OptionScene.h"
#include "Charactor.h"
#include "Monster.h"
#include "CharCube.h"

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

	m_nCharSelect = -1;
	m_nSelectState[0] = m_nSelectState[1] = m_nSelectState[2] = m_nSelectState[3] = -1;
}

VOID LobbyScene::Release()
{
	SAFE_DELETE( m_pLobbyGUI );
	SAFE_DELETE( m_pOptionScene );

	for( INT i=0 ; i<4 ; i++ )
		SAFE_DELETE( m_aData[ i ].pCharacter );
	SAFE_DELETE_ARRAY( m_aData );

	SAFE_DELETE( m_pLight );
	//CTree::DestoryInstance();
}

VOID LobbyScene::CreateData( LPDATA _pData, LPWSTR _pFileName, D3DXVECTOR3 _vecPosition )
{
	SAFE_DELETE( _pData->pCharacter );

	_pData->pCharacter = new CCharactor;

	_pData->pCharacter->Create( m_pD3dDevice );
	_pData->pCharacter->Load( _pFileName );
	_pData->pCharacter->Set_Position( _vecPosition );
	_pData->pCharacter->EnableShadow( FALSE );
	_pData->vecPosition = _vecPosition;
}

VOID LobbyScene::CreateWeapon( INT _nKind )
{
	m_aData[ _nKind ].pCharacter->CreateWeapon( _nKind + 1 );
	m_aData[ _nKind ].pCharacter->SetWeaponAni();
}

VOID LobbyScene::DestoryWeapon( INT _nKind )
{
	m_aData[ _nKind ].pCharacter->DestoryWeapon( );
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

	CreateData( &m_aData[ 0 ], L"Data\\CharData\\APPA_1.csav",	D3DXVECTOR3( 31.0f, 5.0f, 120.0f ) );
	CreateData( &m_aData[ 1 ], L"Data\\CharData\\MOM_1.csav",	D3DXVECTOR3(  10.0f, 5.0f, 120.0f ) );
	CreateData( &m_aData[ 2 ], L"Data\\CharData\\ADDLE_1.csav", D3DXVECTOR3( -10.0f, 5.0f, 120.0f ) );
	CreateData( &m_aData[ 3 ], L"Data\\CharData\\DDAL_1.csav",	D3DXVECTOR3( -31.0f, 5.0f, 120.0f ) );
	
	//	Create Light
	m_pLight = new CLight;
	m_pLight->Create( _pd3dDevice );

	//	Create Matrices
	m_pMatrices = CMatrices::GetInstance();	
	
	return S_OK;
}

VOID LobbyScene::EnableRotate( INT _iIndex )
{
	if( m_aData[ _iIndex ].bRotate )
		return;

	for( INT i=0 ; i<4 ; i++ )
		m_aData[ i ].bRotate = FALSE;

	m_aData[ _iIndex ].bRotate	= TRUE;
	
	m_fChrRotate = 3.14f;
	m_fIncRotate = 0.005f;
}

VOID LobbyScene::DisableRotate( INT _iIndex )
{
	m_aData[ _iIndex ].bRotate	= FALSE;
}

VOID LobbyScene::Update()
{
	CNetwork::GetInstance()->UpdateLobby();
	m_pLobbyGUI->Update();
	m_pOptionScene->Update();

	for( INT i=0 ; i<4 ; i++ )
	{
		m_aData[ i ].pCharacter->UpdateOtherPlayer( TRUE );
		m_aData[ i ].pCharacter->Update();
	}

	CObjectManage * pOM = CObjectManage::GetInstance();
	DWORD dID;
	m_pLobbyGUI->Command( dID );
	switch( dID )
	{
	case LOBBY_START:
		CNetwork::GetInstance()->CS_GAME_START();
		break;
	case LOBBY_READY:
		for( INT i=0 ; i<4 ; i++ )
			m_aData[ i ].bRotate = FALSE;

		if ( m_nCharSelect < 0 )
		{
			pOM->GetLobbyScene()->ChangeStateButton( LOBBY_READY, UP );
			break;
		}
		// 최초선택
		if ( m_nSelectState[pOM->Get_ClientNumber()] == -1 )
		{
			CNetwork::GetInstance()->CS_READY( m_nCharSelect, TRUE );
		}
		else if ( m_nSelectState[pOM->Get_ClientNumber()] == m_nCharSelect )
		{
			CNetwork::GetInstance()->CS_READY( m_nCharSelect, FALSE );
			m_nCharSelect = -1;
		}
		break;
	case LOBBY_BACK:
		m_scnNext	= IScene::SCENE_MENU;
		m_scnState	= IScene::SCENE_END;
		break;
	case LOBBY_SELECT_1:
	case LOBBY_SELECT_2:
	case LOBBY_SELECT_3:
	case LOBBY_SELECT_4:
		if ( m_nSelectState[pOM->Get_ClientNumber()] == -1 )
		{
			m_nCharSelect = dID - LOBBY_SELECT_1;
			EnableRotate( dID - LOBBY_SELECT_1 );
		}
		break;
	}
/*
	if( GetKeyState( '1' ) & 0x8000 )
		ChangeStateButton( GUIBTN_LOBBY_START, GUIBTN_NORMAL );
	if( GetKeyState( '2' ) & 0x8000 )
		ChangeStateButton( GUIBTN_LOBBY_START, GUIBTN_DISABLE );
	if( GetKeyState( '3' ) & 0x8000 )
		DisableRotate( 2 );
	if( GetKeyState( '4' ) & 0x8000 )
		DisableRotate( 3 );
*/	
	if( m_fChrRotate > 3140000.0f )
		m_fChrRotate = 3.14f;

	m_fChrRotate += m_fIncRotate;

	for( INT i=0 ; i<4 ; i++ )
	{
		if( m_aData[ i ].bRotate )
			m_aData[ i ].pCharacter->UpdateByValue( m_aData[ i ].vecPosition, m_fChrRotate );
		else
			m_aData[ i ].pCharacter->UpdateByValue( m_aData[ i ].vecPosition, 3.14f );
	}

	//	Chatting Massage
	WCHAR aStr[ 256 ] = { 0, };
	if( m_pLobbyGUI->TakeChattingMassage( aStr ) )
	{
		CNetwork::GetInstance()->CS_CHAT( aStr );
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
	m_pLight->EnableLight( D3DXVECTOR3( 1.0f, 0.0f, -1.0f ) );

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

VOID LobbyScene::UpdateCharArray()
{
	//INT						m_nCharSelect;		// 선택된 케릭터 번호.
	//INT						m_nSelectState[4];	// 호스트의 선택상태.

	CObjectManage::GetInstance()->Set_CharTable( m_nSelectState );
	//for (int i = 0; i < 4; ++i)
	//{
	//	if( m_nSelectState[i] != -1 )
	//	{
	//		CObjectManage::GetInstance()->Get_Charactors()[i]->Set_Active( TRUE );
	//		//CObjectManage::GetInstance()->Set_Char( i, m_nSelectState[i], TRUE );
	//		//CObjectManage::GetInstance()->Set_Char( m_nSelectState[i], i );
	//	}		
	//}
}

VOID LobbyScene::EnableButton( DWORD _dID, BOOL _bEnable )
{
	m_pLobbyGUI->EnableButton( _dID, _bEnable );
}

VOID LobbyScene::SetText( LPWSTR _pText )
{
	m_pLobbyGUI->SetText( _pText );
}

VOID LobbyScene::GetText( LPWSTR _pText )
{
	m_pLobbyGUI->GetText( _pText );
}

VOID LobbyScene::ChangeStateButton( DWORD _dID, DWORD _dState )
{
	m_pLobbyGUI->ChangeScene( _dID, _dState );
}

VOID LobbyScene::SetSceneState( INT a_iSceneState )
{
	m_scnState = a_iSceneState;
}